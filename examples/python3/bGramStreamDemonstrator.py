import socket
import sys
import argparse
import time
sys.path.insert(1, '../../src/DSPGram/')

from adcp_coredata_messages import *

# interpret command line arguments
# Creating the parser
parser = argparse.ArgumentParser()
parser.add_argument('--ip', type=str, required=True)
parser.add_argument('--port', type=int, required=True)
# Parsing the argument
args = parser.parse_args()

ping = SDSP_v1_0_BDGram()
framesize = 14 + ping.SIZE_BYTES

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = (args.ip, args.port)

print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

data = bytearray()

while 1:

    try:
        print('connected, waiting for ' + str(framesize) + 'bytes')
        data.extend(sock.recv(framesize))
        print('received ' + str(len(data)))
        if len(data) >= framesize :
            ping.deserialise(data[14:framesize])
            ping.show() 
            print('Ping: {!r}'.format(ping.sCommon.u32_PingCounter))
            data = data[framesize:]


    except:
        time.sleep(3)
        sock.close()
        sock.connect(server_address)
