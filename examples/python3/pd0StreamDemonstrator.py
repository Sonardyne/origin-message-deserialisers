import socket
import sys
import argparse
import time
sys.path.insert(1, '../../src/')

from PDNMessages.PDN_messages import *

# interpret command line arguments
# Creating the parser
parser = argparse.ArgumentParser()
parser.add_argument('--ip', type=str, default="192.168.179.20")
parser.add_argument('--port', type=int, default=4010)
# Parsing the argument
args = parser.parse_args()

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = (args.ip, args.port)

print('connecting to {} port {}'.format(*server_address))
sock.connect(server_address)

while True:

    try:
        hdr = RDIPD0Header()
        hdrbuf = sock.recv(hdr.SIZE_BYTES, socket.MSG_PEEK)

        print('PD0 hdr read: {!r}'.format(len(hdrbuf)))

        hdr.deserialise(hdrbuf)

        print('PD0 HeaderID: {!r}'.format(hdr.HeaderId))
        print('PD0 DataSource: {!r}'.format(hdr.DataSource))
        print('PD0 NumberDataTypes: {!r}'.format(hdr.NumberDataTypes))

        ping = PD0Message()
        msgbuf = sock.recv(hdr.EnsembleBytes + 2)

        print('PD0 msg read: {!r}'.format(len(msgbuf)))

        ping.deserialise(msgbuf)

        if hdr.EnsembleBytes > 0:
            # insert tests here...
            var_ldr = ping.get_section(RDIVariableLeader.ID)
            print(var_ldr.EnsembleNumber)

    except Exception:
        time.sleep(3)
        sock.close()
        sock.connect(server_address)
