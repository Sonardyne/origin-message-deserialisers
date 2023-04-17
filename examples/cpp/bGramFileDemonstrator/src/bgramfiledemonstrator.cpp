#include <stdlib.h>
#include <iostream>

#include "adcp_coredata_messages.h"

// Utility function
long find_file_size(const char* filename);

using namespace sonardyne_edge;

int main(int argc, char** argv) 
{
    
    if (argc != 2)
    {
        std::cout << "Usage: ./bgramfiledemonstrator <filename> " << std::endl;

        return 0;
    }
    
    std::string filename(argv[1]);

    std::cout << "Decoding BGRAM file " << filename << std::endl;

    long file_size_bytes = find_file_size(filename.c_str());
       
    // open the file
    FILE* fh = fopen(filename.c_str(), "r");

    if (fh)
    {
        int file_bytes_read = 0;
        size_t len =0;

        while (file_bytes_read < file_size_bytes)
        {   
            adcp_coredata_messages::SDSP_v1_0_BDGram msg;

            len = msg.calc_length();

            // read out the sonardyne header - not necessary for EDGE applications
            int hdr_bytes = 14;
            char hdr_buf[hdr_bytes];
            fread(&hdr_buf[0], 1, hdr_bytes, fh);

            int buf_bytes = len;
            char buf[buf_bytes];

            fread(&buf[0], 1, buf_bytes, fh);

            msg.deserialise(buf, buf_bytes);

            std::cout << "Beam index: " << msg.sCommon.u16_BeamIndex << "; Ping counter: " << msg.sCommon.u32_PingCounter << std::endl;

            file_bytes_read += buf_bytes + hdr_bytes;                
        }

        std::cout << "Finished: Msg size = " << len << " Read " << file_bytes_read << " / " << file_size_bytes << " bytes" << std::endl;

        fclose(fh);
    }
    else
    {
        std::cout << "Couldn't open BGRAM file " << filename << std::endl;
    }

    return EXIT_SUCCESS;
}

long find_file_size(const char* filename)
{
    long bytes = -1;
    
    FILE* fh = fopen(filename, "r");
    
    if (fh)
    {
        int error = fseek(fh, 0, SEEK_END);
    
        if (error == 0)
        {
            bytes = ftell(fh);
        }            
        
        fclose(fh);
    }
    
    return bytes;
}

