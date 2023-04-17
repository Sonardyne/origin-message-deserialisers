#include <stdlib.h>
#include <iostream>

// Core PDN message library
#include "PDN_messages.h"

// Custom user PD0 section
#include "UserPD0Block.h"

// Utility function
long find_file_size(const char* filename);

using namespace sonardyne_edge;

using namespace PDN_messages;

int main(int argc, char** argv) 
{
    
    if (argc < 2)
    {
        std::cout << "Usage: ./pd0filedemonstrator <filename> [Optional: output.filename] " << std::endl;
        std::cout << "       ./pd0filedemonstrator <filename> " << std::endl;
        std::cout << "       ./pd0filedemonstrator <input filename> <output filename>" << std::endl;
        
        return EXIT_FAILURE;
    }
    
    std::string filename(argv[1]);
    std::cout << "Decoding PD0 file " << filename << std::endl;

    if (argc == 2)
    {
        std::cout << "Adding user block to deserialise pd0" << std::endl;        
    }

    // In this simple demo, if an output file is specified, an example custom user PD0 section 
    // will be inserted (see UserPD0Block.h/cpp for the declaration/definition)
    FILE* fh_out = NULL;
    if (argc > 2)
    {
        std::cout << "Adding user custom block to output pd0" << std::endl;
        fh_out = fopen(argv[2], "w");    
    }
    
    long file_size_bytes = find_file_size(filename.c_str());
       
    // first, peek the header to see how much data to read
    if (file_size_bytes >= RDIPD0Header::SIZE_BYTES)
    {
        // open the file
        FILE* fh = fopen(filename.c_str(), "r");

        if (fh)
        {
            int file_bytes_read = 0;

            while (file_bytes_read < file_size_bytes)
            {   
                // first, read the header to figure out how much data there is
                char hdr_buf[RDIPD0Header::SIZE_BYTES];
                RDIPD0Header hdr;

                fread(&hdr_buf[0], 1, RDIPD0Header::SIZE_BYTES, fh);

                hdr.deserialise(&hdr_buf[0], RDIPD0Header::SIZE_BYTES);

                // seek the file back to the start of the header
                fseek(fh, -RDIPD0Header::SIZE_BYTES, SEEK_CUR);

                // read in the ensemble
                char ensemble_buf[hdr.EnsembleBytes + PD0Message::CHECKSUM_SIZE_BYTES];

                int bytes_read = fread(&ensemble_buf[0], 1, hdr.EnsembleBytes + PD0Message::CHECKSUM_SIZE_BYTES, fh);                    

                if (bytes_read > 0)
                {
                    std::cout << "Read " << bytes_read << " bytes ; total read: " << file_bytes_read << " / " << file_size_bytes << std::endl;

                    // We have the whole ensemble in memory. Deserialise it.
                    PD0Message pd0;     

                    // if a known user section is present in the PD0, add it here before deserialisation
                    if (argc == 2)
                    {
                        UserPD0Block* tmp_user = new UserPD0Block();
                        pd0.add_section(tmp_user);
                    }

                    int bytes_deserialised = pd0.deserialise(&ensemble_buf[0], hdr.EnsembleBytes + PD0Message::CHECKSUM_SIZE_BYTES);

                    SONOriginSystemLeader*       pSONVar        = (SONOriginSystemLeader*)pd0.get_section(SONOriginSystemLeader::Id);
                    if(pSONVar != NULL)
                    {
                        std::cout << "ping counter = " << pSONVar->u32_PingCounter << std::endl;
                    }
                    if (fh_out)
                    {
                        // if an output file is specified, add a custom user PD0 section
                        if (argc == 3)
                        {
                            UserPD0Block* user = new UserPD0Block();
                            pd0.add_section(user);                        
                        }

                        EFrame eFrame = EFrame::EARTH;
                        pd0.transform(eFrame);

                        int new_length_bytes = pd0.calc_length();

                        char* buf = new char[new_length_bytes];

                        int bytes_serialised = pd0.serialise(buf, new_length_bytes);

                        fwrite(buf, 1, bytes_serialised, fh_out);

                        delete[] buf;
                    }


                    file_bytes_read += bytes_read;
                }
                else
                {
                    std::cout << "Read " << bytes_read << " bytes ; total read: " << file_bytes_read << " / " << file_size_bytes << std::endl;
                    std::cout << "File read error: " << bytes_read << std::endl;
                    break;
                }
            } 

            std::cout << "Finished: Read " << file_bytes_read << " / " << file_size_bytes << " bytes" << std::endl;

            fclose(fh);
        }
    }
    else
    {
        std::cout << "File too small" << std::endl;
        return -1;
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

