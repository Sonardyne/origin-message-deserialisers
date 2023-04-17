import sys
import os
import math

sys.path.insert(1, '../../src/DSPGram/')

from adcp_coredata_messages import *

if len(sys.argv) == 2:
    
    path = sys.argv[1]

    filesize = os.path.getsize(path)
    filename, file_extension = os.path.splitext(path)

    print(filename)
    print(file_extension)
    
    with open(path, "rb", buffering = 0) as f:    

        ping = SDSP_v1_0_ADCPGram()

        framesize = 14 + ping.SIZE_BYTES
        
        num_ensembles = math.ceil(filesize / framesize)
        cnt = 0
        
        while (bytes := f.read(framesize)):
            ping.deserialise(bytes[14:framesize])
            ping.show()

            cnt = cnt + 1

            print(str(cnt) + " / " + str(num_ensembles))
