import sys
import os
import math

sys.path.insert(1, '../../src/')

from PDNMessages.PDN_messages   import *
from PDNMessages.UserPD0Section import *

if len(sys.argv) == 2:

    path = sys.argv[1]

    filesize = os.path.getsize(path)

    with open(path, "rb", buffering = 0) as f:    

        # open an output file for writing
        output_file = open(path + ".out", 'wb')

        # figure out how big the ensemble is from the 6-byte header
        hdr = RDIPD0Header()
        cnt = 0
        num_ensembles = 0
        
        while (data := f.read(RDIPD0Header.SIZE_BYTES)):

            print("===== New ensemble =====")

            pd0 = PD0Message()

            # ---------------- read header ----------------- #
            # put data into byte array
            buf = bytearray(data)
            
            bytes_deserialised = pd0.deserialise(buf)

            ensemble_bytes = pd0.get_section(RDIPD0Header.ID).EnsembleBytes + PD0Message.CHECKSUM_SIZE_BYTES
            print("Ensemble size = "+ str(ensemble_bytes))
            if (cnt == 0):
                num_ensembles = math.ceil(filesize / ensemble_bytes)

            # seek back on the header size
            f.seek(-RDIPD0Header.SIZE_BYTES, 1)


            # ---------------- read entire ensemble ----------------- #
            data = f.read(ensemble_bytes)

            # put data into byte array
            orig_ensemble_in_buf = bytearray(data)
            
            bytes_deserialised = pd0.deserialise(orig_ensemble_in_buf)

            pd0.show()  # will print each Pd0 content.
            
            print(str(bytes_deserialised) + " bytes de-serialised")


            # ---------------- re-serialise the ensemble ----------------- #
            
            orig_ensemble_out_buf = bytearray(pd0.calc_length())

            bytes_serialised = pd0.serialise(orig_ensemble_out_buf)
            
            print(str(bytes_serialised) + " bytes re-serialised")


            # ---------------- add user-defined PD0 section --------- #
            
            # add a user field to modify the output PD0
            user = UserPD0Section()
            user.UserInt    = 1;
            user.UserUInt   = 2;
            user.UserInt64  = 3;
            user.UserFloat  = 4.56;
            user.UserDouble = 7.89;

            pd0.add_section(user)


            # ---------------- serialise the modified ensemble ----------------- #
            
            modified_ensemble_out_buf = bytearray(pd0.calc_length())

            bytes_serialised = pd0.serialise(modified_ensemble_out_buf)
            
            print(str(bytes_serialised) + " bytes serialised of modified ensemble")


            # ----- deserialise the modified ensemble to show how it's done ---- #
            
            # we add the user section so PD0 knows how to decode it
            new_pd0 = PD0Message()
            user = UserPD0Section() 
            new_pd0.add_section(user)

            bytes_deserialised = new_pd0.deserialise(modified_ensemble_out_buf)

            print(str(bytes_deserialised) + " bytes de-serialised of modified ensemble")

            # --------------- write the modified ensemble to disk -------------- #

            output_file.write(modified_ensemble_out_buf)

            cnt = cnt + 1
            
            print("Processing ensemble " + str(cnt) + " / " + str(num_ensembles))

    # close the output file
    output_file.close()

else:

    print("Usage: ")
    print("python3 pd0demonstrator.py /path/to/file.pd0")
