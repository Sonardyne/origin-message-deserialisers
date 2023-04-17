#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

// Core PDN message library
#include "PDN_messages.h"

using namespace sonardyne_edge;

using namespace PDN_messages;

int  connect_to_server(const std::string& server_ip_address, const int& server_port, int& fd);
void close_socket(int& fd);

int main(int argc, char** argv) 
{
    
    if (argc != 3)
    {
        std::cout << "Usage: ./pd0streamdemonstrator <IPv4 address> <port>" << std::endl;

        return 0;
    }
    
    std::string server_ip_address(argv[1]);
    int server_port = atoi(argv[2]);
    
    std::cout << "Connecting to PD0 server at " << server_ip_address << " " << server_port << std::endl;

    // TCP read buffer
    int MAX_BUF_SIZE = 8192 * 32;
    char read_buf[MAX_BUF_SIZE];
    memset(&read_buf[0], 0, MAX_BUF_SIZE);
    int bytes_read = 0;
    
    // socket & state machine variables
    int fd = -1;
    int error = 0;
    
    // message to decode
    size_t msg_len = 0;
    
    while(1)
    {
        if (fd == -1)
        {
            error = connect_to_server(server_ip_address, server_port, fd);
            
            if (error == 0)
            {
                // connected; can read from socket
                std::cout << "CONNECTED" << std::endl;                
            }
        }
        
        if (error != 0)
        {
            close_socket(fd);
            sleep(3);
        }
        else
        {            
            int recv_bytes = recv(fd, &read_buf[0], bytes_read, 0);

            if (recv_bytes > 0)
            {
                std::cout << "Read " << recv_bytes << " bytes" << std::endl;
                
                bytes_read += recv_bytes;

                // we can decode the PD0 header
                if (recv_bytes > RDIPD0Header::SIZE_BYTES && msg_len == 0)
                {
                    char hdr_buf[RDIPD0Header::SIZE_BYTES];
                    RDIPD0Header hdr;

                    hdr.deserialise(&read_buf[0], RDIPD0Header::SIZE_BYTES);

                    msg_len = hdr.EnsembleBytes + PD0Message::CHECKSUM_SIZE_BYTES;
                }
                
                // we can decode the full record
                if (msg_len > 0 && recv_bytes >= msg_len)
                {
                    PD0Message pd0;     
                    int bytes_deserialised = pd0.deserialise(&read_buf[0], msg_len);

                    // do something simple with the data
                }
            }
            else
            {
                close_socket(fd);            
            }
        }        
    }
    
    return EXIT_SUCCESS;
}

int connect_to_server(const std::string& server_ip_address, const int& server_port, int& fd)
{
    // +1 for null terminator
    int server_ip_address_length = strlen(server_ip_address.c_str()) + 1;
    
    char* _server_ip_address = new char[server_ip_address_length];
    memcpy(_server_ip_address, server_ip_address.c_str(), server_ip_address_length);

    // max chars required for 64-bit integer
    int server_port_length = 21;
    char server_port_str[server_port_length];
    sprintf(server_port_str, "%d", server_port);
    
    char* _server_port = new char[server_port_length];
    memcpy(_server_port, &server_port_str[0], server_port_length);    
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo* servinfo = NULL;
    struct addrinfo* p = NULL;

    int errors = getaddrinfo(_server_ip_address, _server_port, &hints, &servinfo);

    if (!errors)
    {    
        for (p = servinfo; p != NULL; p = p->ai_next)
        {
            // create socket if one doesn't already exist

            if (fd == -1)
            {
                fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            }

            errors = connect(fd, p->ai_addr, p->ai_addrlen);

            if (errors)
            {
                close_socket(fd);
            }

            std::cout << errors << " " << fd << std::endl;

            break;
        }
    }
    else
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(errors));
    }

    freeaddrinfo(servinfo);
    
    delete[] _server_ip_address;
    delete[] _server_port;

    return errors;
}

void close_socket(int& fd)
{
    if (fd != -1)
    {
        shutdown(fd, SHUT_RDWR);
        close(fd);
        fd = -1;
    }
}
