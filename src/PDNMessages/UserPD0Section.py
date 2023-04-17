import struct

from .RDIPD0Interface import *

class UserPD0Section(RDIPD0Interface):

    SIZE_BYTES = 30
    ID         = 0x3333
    
    def __init__(self):
        self.HeaderId   = self.ID
        self.UserInt    = 0
        self.UserUInt   = 0
        self.UserInt64  = 0
        self.UserFloat  = 0
        self.UserDouble = 0

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES
    
    def serialise(self, buf):
        
        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('i', buf, 2,  self.UserInt)
        struct.pack_into('I', buf, 6,  self.UserUInt)
        struct.pack_into('q', buf, 10, self.UserInt64)
        struct.pack_into('f', buf, 18, self.UserFloat)
        struct.pack_into('d', buf, 22, self.UserDouble)
                    
        return self.SIZE_BYTES
    
    def deserialise(self, buf):
        
        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        self.UserInt    = struct.unpack('i', buf[2:6])[0]
        self.UserUInt   = struct.unpack('I', buf[6:10])[0]
        self.UserInt64  = struct.unpack('q', buf[10:18])[0]
        self.UserFloat  = struct.unpack('f', buf[18:22])[0]
        self.UserDouble = struct.unpack('d', buf[22:30])[0]

        return self.SIZE_BYTES
        
    def show(self):
        print("--- User PD0 Section ---")
        print("0x{0:02x}".format(self.HeaderId))
        print("User int: "    + str(self.UserInt ))
        print("User uint: "   + str(self.UserUInt ))
        print("User int64: "  + str(self.UserInt64 ))
        print("User float: "  + str(self.UserFloat ))
        print("User double: " + str(self.UserDouble ))


