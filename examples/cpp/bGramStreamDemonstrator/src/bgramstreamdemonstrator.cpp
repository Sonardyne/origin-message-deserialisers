#include <stdlib.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include "adcp_coredata_messages.h"

using namespace sonardyne_edge;

int  connect_to_server(const std::string& server_ip_address, const int& server_port, int& fd);
void close_socket(int& fd);

int main(int argc, char** argv) 
{
    
    if (argc != 3)
    {
        std::cout << "Usage: ./bgramstreamdemonstrator <IPv4 address> <port>" << std::endl;

        return 0;
    }
    
    std::string server_ip_address(argv[1]);
    int server_port = atoi(argv[2]);
    
    std::cout << "Connecting to BGRAM server at " << server_ip_address << " " << server_port << std::endl;

    // TCP read buffer
    int MAX_BUF_SIZE = 8192 * 32;
    char read_buf[MAX_BUF_SIZE];
    memset(&read_buf[0], 0, MAX_BUF_SIZE);
    int bytes_read = 0;
    
    // socket & state machine variables
    int fd = -1;
    int error = 0;
    
    // message to decode
    adcp_coredata_messages::SDSP_v1_0_BDGram msg;
    size_t msg_len = msg.calc_length();
    
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
            int recv_bytes = recv(fd, &read_buf[bytes_read], MAX_BUF_SIZE - bytes_read, 0);

            if (recv_bytes > 0)
            {
                std::cout << "Read " << recv_bytes << " bytes" << std::endl;
                
                bytes_read += recv_bytes;
                
                // there are enough bytes in memory to decode one record
                if (bytes_read >= 14 + msg_len)
                {
                    // decode the record
                    msg.deserialise(&read_buf[0] + 14, msg_len);
                    
                    std::cout << "Beam index: " << msg.sCommon.u16_BeamIndex << "; Ping counter: " << msg.sCommon.u32_PingCounter << std::endl;
                    
                    // update the number of undecoded bytes
                    bytes_read -= (14 + msg_len);
                    
                    // shift buffer memory down ready for next record
                    memcpy(&read_buf[0], &read_buf[14 + msg_len], bytes_read);
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
