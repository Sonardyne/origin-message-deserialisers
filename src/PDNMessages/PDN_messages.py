import array
import math
import struct
from .RDIPD0Interface import *
from enum import Enum

class Frame(Enum):
    BEAM = 1
    INSTRUMENT = 2
    VESSEL = 3
    EARTH = 4

def rotx(c, s, v):

    mx = [[1, 0, 0],[0, c, s],[0, -s, c]]

    w = [0, 0, 0]

    for i in range(3):
        for j in range(3):
            w[i] += mx[i][j] * v[j]

    return w

def roty(c, s, v):

    my = [[c, 0, -s],[0, 1, 0],[s, 0, c]]

    w = [0, 0, 0]

    for i in range(3):
        for j in range(3):
            w[i] += my[i][j] * v[j]

    return w

def rotz(c, s, v):

    mz = [[c, s, 0],[-s, c, 0],[0, 0, 1]]

    w = [0, 0, 0]

    for i in range(3):
        for j in range(3):
            w[i] += mz[i][j] * v[j]

    return w
        

class RDIPD0Header(RDIPD0Interface):

    SIZE_BYTES = 6
    ID         = 0x7F7F
    
    def __init__(self):
        self.HeaderId        = 0x7F
        self.DataSource      = 0x7F
        self.EnsembleBytes   = 0
        self.Spare           = 0
        self.NumberDataTypes = 0
        self.Indices         = array.array('H')

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return (self.SIZE_BYTES + 2 * self.NumberDataTypes)
    
    def serialise(self, buf):
        
        struct.pack_into('B', buf, 0,  self.HeaderId)
        struct.pack_into('B', buf, 1,  self.DataSource)
        struct.pack_into('H', buf, 2,  self.EnsembleBytes)
        struct.pack_into('B', buf, 4,  self.Spare)
        struct.pack_into('B', buf, 5,  self.NumberDataTypes)

        if (len(buf) == self.SIZE_BYTES):
            return self.SIZE_BYTES

        for i in range (0, self.NumberDataTypes):

            struct.pack_into('H', buf, self.SIZE_BYTES + 2*i,  self.Indices[i])
            
        return (self.SIZE_BYTES + (self.NumberDataTypes * 2))
    
    def deserialise(self, buf):

        if buf[0] != self.HeaderId:
            return -1

        if buf[1] != self.DataSource:
            return -1

        self.EnsembleBytes   = struct.unpack("H", buf[2 : 4])[0]
        self.Spare           = buf[4]
        self.NumberDataTypes = buf[5]

        if (len(buf) == self.SIZE_BYTES):
            return self.SIZE_BYTES

        for i in range (0, self.NumberDataTypes):
            l = self.SIZE_BYTES + 2*i
            h = self.SIZE_BYTES + 2*i + 2
            
            offset = struct.unpack("H", buf[l : h])[0]

            self.Indices.append(offset)

        return (self.SIZE_BYTES + (self.NumberDataTypes * 2))
        
    def show(self):
        print("--- RDI PD0 Header ---")
        print(self.HeaderId)
        print(self.DataSource)
        print("Ensemble bytes: " + str(self.EnsembleBytes))
        print(self.Spare)
        print("# data types: " + str(self.NumberDataTypes))
        for i in self.Indices:
            print(i)


class RDIFixedLeader(RDIPD0Interface):

    SIZE_BYTES = 59
    ID         = 0x0000
    NUM_SPARES = 8
    
    def __init__(self):
        self.HeaderId = self.ID
        self.CpuFirmwareVersion = 0
        self.CpuFirmwareRev = 0
        self.SystemConfig = 0
        self.RealOrSim = 0
        self.LagLengthCm = 0
        self.NumberOfBeams = 0
        self.NumberOfCells = 0
        self.PingsPerEnsemble = 0
        self.DepthCellLengthCm = 0
        self.BlankAfterTransmitCm = 0
        self.ProfilingMode = 0
        self.LowcorrThresh = 0
        self.CodeRepeats = 0
        self.PercentGood = 0
        self.ErrorVelMax = 0
        self.TPPMins = 0
        self.TPPSecs = 0
        self.TPPHunds = 0
        self.CoordTxfrm = 0
        self.HeadingAlignment = 0
        self.HeadingBias = 0
        self.SensorSource = 0
        self.SensorAvail = 0
        self.Bin1DistanceCm = 0
        self.TxPulseLengthCm = 0
        self.StartEndDepthCell = 0
        self.FalseTargetThresh = 0
        self.Spare1 = 0
        self.TxLagDistanceCm = 0
        self.Spares = [0, 0, 0, 0, 0, 0, 0, 0]
        self.WBSystemBandwidth = 0
        self.SystemPower = 0
        self.Spare2 = 0
        self.SerialNumber = 0
        self.BeamAngle = 0
        
    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('B', buf, 2,  self.CpuFirmwareVersion)
        struct.pack_into('B', buf, 3,  self.CpuFirmwareRev)
        struct.pack_into('H', buf, 4,  self.SystemConfig)
        struct.pack_into('B', buf, 6,  self.RealOrSim)
        struct.pack_into('B', buf, 7,  self.LagLengthCm)
        struct.pack_into('B', buf, 8,  self.NumberOfBeams)
        struct.pack_into('B', buf, 9,  self.NumberOfCells)
        struct.pack_into('H', buf, 10,  self.PingsPerEnsemble)
        struct.pack_into('H', buf, 12,  self.DepthCellLengthCm)
        struct.pack_into('H', buf, 14,  self.BlankAfterTransmitCm)
        struct.pack_into('B', buf, 16,  self.ProfilingMode)
        struct.pack_into('B', buf, 17,  self.LowcorrThresh)
        struct.pack_into('B', buf, 18,  self.CodeRepeats)
        struct.pack_into('B', buf, 19,  self.PercentGood)
        struct.pack_into('H', buf, 20,  self.ErrorVelMax)
        struct.pack_into('B', buf, 22,  self.TPPMins)
        struct.pack_into('B', buf, 23,  self.TPPSecs)
        struct.pack_into('B', buf, 24,  self.TPPHunds)
        struct.pack_into('B', buf, 25,  self.CoordTxfrm)
        struct.pack_into('H', buf, 26,  self.HeadingAlignment)
        struct.pack_into('H', buf, 28,  self.HeadingBias)
        struct.pack_into('B', buf, 30,  self.SensorSource)
        struct.pack_into('B', buf, 31,  self.SensorAvail)
        struct.pack_into('H', buf, 32,  self.Bin1DistanceCm)
        struct.pack_into('H', buf, 34,  self.TxPulseLengthCm)
        struct.pack_into('H', buf, 36,  self.StartEndDepthCell)
        struct.pack_into('B', buf, 38,  self.FalseTargetThresh)
        struct.pack_into('B', buf, 39,  self.Spare1)
        struct.pack_into('H', buf, 40,  self.TxLagDistanceCm)
        for i in range(self.NUM_SPARES):
            
            struct.pack_into('B', buf, 42 + i,  self.Spares[i])

        struct.pack_into('H', buf, 50,  self.WBSystemBandwidth)
        struct.pack_into('B', buf, 52,  self.SystemPower)
        struct.pack_into('B', buf, 53,  self.Spare2)
        struct.pack_into('I', buf, 54,  self.SerialNumber)
        struct.pack_into('B', buf, 58,  self.BeamAngle)

        return self.SIZE_BYTES

    def deserialise(self, buf):

        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        self.CpuFirmwareVer       = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.CpuFirmwareRev       = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.SystemConfig         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.RealOrSim            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.LagLengthCm          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.NumberOfBeams        = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.NumberOfCells        = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.PingsPerEnsemble     = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.DepthCellLengthCm    = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.BlankAfterTransmitCm = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.ProfilingMode        = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.LowcorrThresh        = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.CodeRepeats          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.PercentGood          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.ErrorVelMax          = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.TPPMins              = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.TPPSecs              = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.TPPHunds             = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.CoordTxfrm           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.HeadingAlignment     = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.HeadingBias          = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.SensorSource         = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.SensorAvail          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.Bin1DistanceCm       = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.TxPulseLengthCm      = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.StartEndDepthCell    = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.FalseTargetThresh    = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.Spare1               = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.TxLagDistanceCm      = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        for i in range(8):
            self.Spares[i]        = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1
        
        self.WBSystemBandwidth    = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.SystemPower          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.Spare2               = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        self.SerialNumber         = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.BeamAngle            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1
        
        return offset

    def show(self):
        print("--- RDI PD0 Fixed Leader ---")
        print("Beams:             " + str(self.NumberOfBeams))
        print("Cells:             " + str(self.NumberOfCells)) 
        print("Depth cell length: " + str(self.DepthCellLengthCm))
        print("Beam angle:        " + str(self.BeamAngle))
        
class RDIVariableLeader(RDIPD0Interface):

    SIZE_BYTES = 65
    ID         = 0x0080
    NUM_SPARES = 3
    
    def __init__(self):
        self.HeaderId = self.ID
        self.EnsembleNumber = 0
        self.RTCTSYear = 0
        self.RTCTSMonth = 0         
        self.RTCTSDay = 0      
        self.RTCTSHour = 0        
        self.RTCTSMin = 0         
        self.RTCTSSec = 0        
        self.RTCTSHSec = 0        
        self.EnsembleMSB = 0      
        self.BITResult = 0       
        self.SpeedOfSoundMs = 0    
        self.DepthOfTxdrDm = 0 
        self.HeadingCD = 0  
        self.PitchCD = 0        
        self.RollCD = 0         
        self.SalinityPPT = 0       
        self.TempCD = 0    
        self.MptMins = 0            
        self.MptSecs = 0         
        self.MptHsecs = 0           
        self.HeadingSdev = 0        
        self.PitchSdev = 0     
        self.RollSdev = 0       
        self.AdcChan0 = 0 
        self.AdcChan1 = 0        
        self.AdcChan2 = 0        
        self.AdcChan3 = 0        
        self.AdcChan4 = 0        
        self.AdcChan5 = 0        
        self.AdcChan6 = 0
        self.AdcChan7 = 0
        self.StatusWord = 0
        self.PressureDP = 0
        self.PressureVarDP = 0
        self.Spare = [0, 0, 0]
        self.RTCCentury = 0   
        self.RTCYear = 0 
        self.RTCMonth = 0    
        self.RTCDay = 0   
        self.RTCHour = 0     
        self.RTCMin = 0    
        self.RTCSec = 0     
        self.RTCHSec = 0     
 
    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):
    
        struct.pack_into('H', buf, 0,  self.HeaderId)

        struct.pack_into('H', buf, 2,  self.EnsembleNumber)
        struct.pack_into('B', buf, 4,  self.RTCTSYear)
        struct.pack_into('B', buf, 5,  self.RTCTSMonth)
        struct.pack_into('B', buf, 6,  self.RTCTSDay)
        struct.pack_into('B', buf, 7,  self.RTCTSHour)
        struct.pack_into('B', buf, 8,  self.RTCTSMin)
        struct.pack_into('B', buf, 9,  self.RTCTSSec)
        struct.pack_into('B', buf, 10, self.RTCTSHSec)
        struct.pack_into('B', buf, 11, self.EnsembleMSB)
        struct.pack_into('H', buf, 12, self.BITResult)
        struct.pack_into('H', buf, 14, self.SpeedOfSoundMs)
        struct.pack_into('H', buf, 16, self.DepthOfTxdrDm)
        struct.pack_into('H', buf, 18, self.HeadingCD)
        struct.pack_into('h', buf, 20, self.PitchCD % 180)
        struct.pack_into('h', buf, 22, self.RollCD % 180)
        struct.pack_into('H', buf, 24, self.SalinityPPT)
        struct.pack_into('H', buf, 26, self.TempCD)
        struct.pack_into('B', buf, 28, self.MptMins)
        struct.pack_into('B', buf, 29, self.MptSecs)
        struct.pack_into('B', buf, 30, self.MptHsecs)
        struct.pack_into('B', buf, 31, self.HeadingSdev)
        struct.pack_into('B', buf, 32, self.PitchSdev)
        struct.pack_into('B', buf, 33, self.RollSdev)
        struct.pack_into('B', buf, 34, self.AdcChan0)
        struct.pack_into('B', buf, 35, self.AdcChan1)
        struct.pack_into('B', buf, 36, self.AdcChan2)
        struct.pack_into('B', buf, 37, self.AdcChan3)
        struct.pack_into('B', buf, 38, self.AdcChan4)
        struct.pack_into('B', buf, 39, self.AdcChan5)
        struct.pack_into('B', buf, 40, self.AdcChan6)
        struct.pack_into('B', buf, 41, self.AdcChan7)
        struct.pack_into('I', buf, 42, self.StatusWord)
        struct.pack_into('I', buf, 46, self.PressureDP)
        struct.pack_into('I', buf, 50, self.PressureVarDP)

        for i in range(self.NUM_SPARES):
            
            struct.pack_into('B', buf, 54 + i,  self.Spare[i])

        struct.pack_into('B', buf, 57, self.RTCCentury)
        struct.pack_into('B', buf, 58, self.RTCYear)
        struct.pack_into('B', buf, 59, self.RTCMonth)
        struct.pack_into('B', buf, 60, self.RTCDay)
        struct.pack_into('B', buf, 61, self.RTCHour)
        struct.pack_into('B', buf, 62, self.RTCMin)
        struct.pack_into('B', buf, 63, self.RTCSec)
        struct.pack_into('B', buf, 64, self.RTCHSec)

        return self.SIZE_BYTES

    def deserialise(self, buf):

        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        self.EnsembleNumber    = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.RTCTSYear         = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCTSMonth        = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCTSDay          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCTSHour         = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCTSMin          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCTSSec          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCTSHSec         = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.EnsembleMSB       = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BITResult         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.SpeedOfSoundMs    = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.DepthOfTxdrDm     = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.HeadingCD         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.PitchCD           = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.RollCD            = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.SalinityPPT       = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.TempCD            = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.MptMins           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.MptSecs           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.MptHsecs          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.HeadingSdev       = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.PitchSdev         = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RollSdev          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan0          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan1          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan2          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan3          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan4          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan5          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan6          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.AdcChan7          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.StatusWord        = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.PressureDP        = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.PressureVarDP     = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        for i in range(3):
            self.Spare[i]      = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1

        self.RTCCentury        = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCYear           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCMonth          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCDay            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCHour           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCMin            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCSec            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RTCHSec           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1       

        return offset
        
    def show(self):
        print("--- RDI PD0 Variable Leader ---")
        print("Ensemble number: " + str(self.EnsembleNumber))
        print("Speed of sound: " + str(self.SpeedOfSoundMs))
        print("Transducer depth: " + str(self.DepthOfTxdrDm))
        print("Heading: " + str(self.HeadingCD))
        print("Pitch: " + str(self.PitchCD))
        print("Time: " + 

        str(self.RTCCentury) + "-" + 
        str(self.RTCYear) + "-" + 
        str(self.RTCMonth) + "-" + 
        str(self.RTCDay) + " " +
        str(self.RTCHour) + ":" +    
        str(self.RTCMin) + ":" +   
        str(self.RTCSec) + "." +   
        str(self.RTCHSec)  )   


class RDIVelocity(RDIPD0Interface):

    ID = 0x0100

    def __init__(self):
        self.HeaderId = self.ID
        self.NumBeams           = 0
        self.NumCells           = 0
        self.BeamVelocitiesMms  = array.array('h')
        self.VelocitiesMms      = array.array('h')

    def set(self, NumBeams, NumCells):
        self.NumBeams       = NumBeams
        self.NumCells       = NumCells
        self.BeamVelocitiesMms = [-32768] * NumBeams * NumCells
        self.VelocitiesMms     = [-32768] * NumBeams * NumCells

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return (2 + 2 * self.NumBeams * self.NumCells)

    def serialise(self, buf):
    
        offset = 0
    
        struct.pack_into('H', buf, offset,  self.HeaderId)

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            struct.pack_into('h', buf, offset, self.BeamVelocitiesMms[i])
            offset += 2

        return (2 + 2 * self.NumBeams * self.NumCells)

    def deserialise(self, buf):
        
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            # for now, assume velocities are only ever serialised in beam frame
            self.BeamVelocitiesMms[i] = struct.unpack("h", buf[offset : offset + 2])[0]
            
            self.VelocitiesMms[i]     = self.BeamVelocitiesMms[i]
            offset += 2

        return offset
        
    def show(self):
        print("--- RDI PD0 Velocities ---")
        print("Beams: " + str(self.NumBeams))
        print("Cells: " + str(self.NumCells))
        for i in range(self.NumBeams*2):
            s = str(self.VelocitiesMms[i])
            print(s)

    def transform(self, frame, fixed_leader, variable_leader):
        
        # factors to convert from beam frame to instrument frame
        a = 1 / (2 * math.sin(math.radians(fixed_leader.BeamAngle)))
        b = 1 / (4 * math.cos(math.radians(fixed_leader.BeamAngle)))
        c = 1
        d = a / math.sqrt(2)
        
        # factors to convert from instrument frame to vessel frame
        sin_yawoffset = math.sin(math.radians(fixed_leader.HeadingAlignment))
        cos_yawoffset = math.cos(math.radians(fixed_leader.HeadingAlignment))

        sin_heading   = math.sin(math.radians(+variable_leader.HeadingCD * 0.01))
        cos_heading   = math.cos(math.radians(+variable_leader.HeadingCD * 0.01))

        sin_pitch     = math.sin(math.radians(-variable_leader.PitchCD * 0.01))
        cos_pitch     = math.cos(math.radians(-variable_leader.PitchCD * 0.01))

        sin_roll      = math.sin(math.radians(-variable_leader.RollCD * 0.01 + 180))
        cos_roll      = math.cos(math.radians(-variable_leader.RollCD * 0.01 + 180))
        
        # index array
        beam_index = [0] * fixed_leader.NumberOfBeams
        
        for i in range(fixed_leader.NumberOfCells):

            cell_offset = i * fixed_leader.NumberOfBeams
            
            # first, reset all beams to be beam-frame
            for j in range(fixed_leader.NumberOfBeams):
                
                beam_index[j] = cell_offset + j
            
                self.VelocitiesMms[beam_index[j]] = self.BeamVelocitiesMms[beam_index[j]]
                    
            if (frame == Frame.BEAM):
                continue

            # transform to instrument frame
            
            if (fixed_leader.NumberOfBeams > 3 ):
            
                # use beams 1-4 to calculate instrument x, y, z
                vx = c* a * (self.VelocitiesMms[beam_index[0]] - self.VelocitiesMms[beam_index[1]]) # 1 - 2
                vy = c* a * (self.VelocitiesMms[beam_index[3]] - self.VelocitiesMms[beam_index[2]]) # 4 - 3
                vz = b *    (self.VelocitiesMms[beam_index[0]] + self.VelocitiesMms[beam_index[1]] + self.VelocitiesMms[beam_index[2]] + self.VelocitiesMms[beam_index[3]])
                ve = d *    (self.VelocitiesMms[beam_index[0]] + self.VelocitiesMms[beam_index[1]] - self.VelocitiesMms[beam_index[2]] - self.VelocitiesMms[beam_index[3]])

                self.VelocitiesMms[cell_offset + 0] = vx
                self.VelocitiesMms[cell_offset + 1] = vy
                self.VelocitiesMms[cell_offset + 2] = vz
                self.VelocitiesMms[cell_offset + 3] = ve

            if (frame == Frame.INSTRUMENT):
                
                continue

            # transform to vessel - yaw offset only
            v = [self.VelocitiesMms[cell_offset + 0], self.VelocitiesMms[cell_offset + 1], self.VelocitiesMms[cell_offset + 2]];
            v = rotz(cos_yawoffset, sin_yawoffset, v)
            
            for j in range(3):
                
                self.VelocitiesMms[cell_offset + j] = v[j]

            # beam 4 index kept as error velocity

            # if present, beam 5 kept as direct measure of vz

            if(frame == Frame.VESSEL):
                
                continue
            

            # might need a copy of x and y components to rotate 5th beam        
            vVesselCopy = v
          
            v = roty(cos_roll,    sin_roll,    v)
            v = rotx(cos_pitch,   sin_pitch,   v)
            v = rotz(cos_heading, sin_heading, v)
            
            for j in range(3):
                
                self.VelocitiesMms[cell_offset + j] = v[j]

            # beam 4 index kept as error velocity

            if (fixed_leader.NumberOfBeams == 5):

                # beam 5 transforms by replacing vz in the previous transforms
                v = [vVesselCopy[0], vVesselCopy[1], self.VelocitiesMms[cell_offset + 4]];

                v = roty(cos_roll,    sin_roll,    v)
                v = rotx(cos_pitch,   sin_pitch,   v)
                v = rotz(cos_heading, sin_heading, v)
            
                self.VelocitiesMms[cell_offset + 4] = v[2]
                
            if(frame == Frame.EARTH):
                
                continue
                
            else:
                continue


class RDIXC(RDIPD0Interface):

    ID = 0x0200

    def __init__(self):
        self.HeaderId = self.ID
        self.XC = array.array('B')
        self.NumBeams = 0
        self.NumCells = 0

    def set(self, NumBeams, NumCells):
        self.NumBeams = NumBeams
        self.NumCells = NumCells
        self.XC       = [0] * NumBeams * NumCells

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return (2 + self.NumBeams * self.NumCells)

    def serialise(self, buf):
        
        offset = 0
    
        struct.pack_into('H', buf, offset, self.HeaderId)

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            struct.pack_into('B', buf, offset, self.XC[i])
            offset += 1

        return (2 + self.NumBeams * self.NumCells)

    def deserialise(self, buf):
        
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            self.XC[i] = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1

        return offset
        
    def show(self):
        print("--- RDI PD0 XC ---")
        print("Beams: " + str(self.NumBeams))
        print("Cells: " + str(self.NumCells))
        for i in range(self.NumBeams*2):
            s = str(self.XC[i])
            print(s)

class RDIIntensity(RDIPD0Interface):

    ID = 0x0300
    
    def __init__(self):
        self.HeaderId = self.ID
        self.Intensities = array.array('B')
        self.NumBeams = 0
        self.NumCells = 0

    def set(self, NumBeams, NumCells):
        self.NumBeams    = NumBeams
        self.NumCells    = NumCells
        self.Intensities = [0] * NumBeams * NumCells

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return (2 + self.NumBeams * self.NumCells)

    def serialise(self, buf):
        
        offset = 0
    
        struct.pack_into('H', buf, offset, self.HeaderId)

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            struct.pack_into('B', buf, offset, self.Intensities[i])
            offset += 1

        return (2 + self.NumBeams * self.NumCells)

    def deserialise(self, buf):
        
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            self.Intensities[i] = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1

        return offset
        
    def show(self):
        print("--- RDI PD0 Intensities ---")
        print("Beams: " + str(self.NumBeams))
        print("Cells: " + str(self.NumCells))
        for i in range(self.NumBeams*2):
            s = str(self.Intensities[i])
            print(s)

class RDIPrctGood(RDIPD0Interface):

    ID = 0x0400

    def __init__(self):
        self.HeaderId = self.ID
        self.PrctGood = array.array('B')
        self.NumBeams = 0
        self.NumCells = 0

    def set(self, NumBeams, NumCells):
        self.NumBeams = NumBeams
        self.NumCells = NumCells
        self.PrctGood = [0] * NumBeams * NumCells

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return (2 + self.NumBeams * self.NumCells)

    def serialise(self, buf):
        
        offset = 0
    
        struct.pack_into('H', buf, offset, self.HeaderId)

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            struct.pack_into('B', buf, offset, self.PrctGood[i])
            offset += 1

        return (2 + self.NumBeams * self.NumCells)

    def deserialise(self, buf):
        
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        for i in range(self.NumBeams * self.NumCells):
            self.PrctGood[i] = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1

        return offset
        
    def show(self):
        print("--- RDI PD0 PrctGood ---")
        print("Beams: " + str(self.NumBeams))
        print("Cells: " + str(self.NumCells))
        for i in range(self.NumBeams*2):
            s = str(self.PrctGood[i])
            print(s)

class RDIBottomTrack(RDIPD0Interface):

    SIZE_BYTES = 85
    ID         = 0x0600
    
    def __init__(self):
        self.HeaderId = self.ID
        self.BTPingsPerEnsemble = 0
        self.BTDelayBeforeReaquire = 0
        self.BTCorrMagMin = 0
        self.BTEvalAmpMin = 0
        self.BTPercentGood = 0
        self.BTMode = 0
        self.BTErrVelMax = 0
        self.BTReserved = 0
        self.BTBeam0RangeCm = 0
        self.BTBeam1RangeCm = 0
        self.BTBeam2RangeCm = 0
        self.BTBeam3RangeCm = 0
        self.BTBeam0VelMms = 0
        self.BTBeam1VelMms = 0
        self.BTBeam2VelMms = 0
        self.BTBeam3VelMms = 0
        self.BTBeam0Corr = 0
        self.BTBeam1Corr = 0
        self.BTBeam2Corr = 0
        self.BTBeam3Corr = 0
        self.BTBeam0Amp = 0
        self.BTBeam1Amp = 0
        self.BTBeam2Amp = 0
        self.BTBeam3Amp = 0
        self.BTBeam0PercentGood = 0
        self.BTBeam1PercentGood = 0
        self.BTBeam2PercentGood = 0
        self.BTBeam3PercentGood = 0
        self.RefLayerMinDm = 0
        self.RefLayerNearDm = 0
        self.RefLayerFarDm = 0
        self.BTRefLayerVelBeam0Mms = 0
        self.BTRefLayerVelBeam1Mms = 0
        self.BTRefLayerVelBeam2Mms = 0
        self.BTRefLayerVelBeam3Mms = 0
        self.BTBeam0RefLayerCorr = 0
        self.BTBeam1RefLayerCorr = 0
        self.BTBeam2RefLayerCorr = 0
        self.BTBeam3RefLayerCorr = 0
        self.BTBeam0RefLayerIntensity = 0
        self.BTBeam1RefLayerIntensity = 0
        self.BTBeam2RefLayerIntensity = 0
        self.BTBeam3RefLayerIntensity = 0
        self.BTRefPercentGoodBeam0 = 0
        self.BTRefPercentGoodBeam1 = 0
        self.BTRefPercentGoodBeam2 = 0
        self.BTRefPercentGoodBeam3 = 0
        self.BTMaxDepthDm = 0
        self.BTRSSIBeam0 = 0
        self.BTRSSIBeam1 = 0
        self.BTRSSIBeam2 = 0
        self.BTRSSIBeam3 = 0
        self.Gain = 0
        self.BTBeam0RangeCmMbs = 0
        self.BTBeam1RangeCmMbs = 0
        self.BTBeam2RangeCmMbs = 0
        self.BTBeam3RangeCmMbs = 0
        self.Reserved = 0


    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):
        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('H', buf, 2,  self.BTPingsPerEnsemble)
        struct.pack_into('H', buf, 4,  self.BTDelayBeforeReaquire)
        struct.pack_into('B', buf, 6,  self.BTCorrMagMin)
        struct.pack_into('B', buf, 7,  self.BTEvalAmpMin)
        struct.pack_into('B', buf, 8,  self.BTPercentGood)
        struct.pack_into('B', buf, 9,  self.BTMode)
        struct.pack_into('H', buf, 10, self.BTErrVelMax)
        struct.pack_into('I', buf, 12, self.BTReserved)
        struct.pack_into('H', buf, 16, self.BTBeam0RangeCm)
        struct.pack_into('H', buf, 18, self.BTBeam1RangeCm)
        struct.pack_into('H', buf, 20, self.BTBeam2RangeCm)
        struct.pack_into('H', buf, 22, self.BTBeam3RangeCm)
        struct.pack_into('h', buf, 24, self.BTBeam0VelMms)
        struct.pack_into('h', buf, 26, self.BTBeam1VelMms)
        struct.pack_into('h', buf, 28, self.BTBeam2VelMms)
        struct.pack_into('h', buf, 30, self.BTBeam3VelMms)
        struct.pack_into('B', buf, 32, self.BTBeam0Corr)
        struct.pack_into('B', buf, 33, self.BTBeam1Corr)
        struct.pack_into('B', buf, 34, self.BTBeam2Corr)
        struct.pack_into('B', buf, 35, self.BTBeam3Corr)
        struct.pack_into('B', buf, 36, self.BTBeam0Amp)
        struct.pack_into('B', buf, 37, self.BTBeam1Amp)
        struct.pack_into('B', buf, 38, self.BTBeam2Amp)
        struct.pack_into('B', buf, 39, self.BTBeam3Amp)
        struct.pack_into('B', buf, 40, self.BTBeam0PercentGood)
        struct.pack_into('B', buf, 41, self.BTBeam1PercentGood)
        struct.pack_into('B', buf, 42, self.BTBeam2PercentGood)
        struct.pack_into('B', buf, 43, self.BTBeam3PercentGood)
        struct.pack_into('H', buf, 44, self.RefLayerMinDm)
        struct.pack_into('H', buf, 46, self.RefLayerNearDm)
        struct.pack_into('H', buf, 48, self.RefLayerFarDm)
        struct.pack_into('h', buf, 50, self.BTRefLayerVelBeam0Mms)
        struct.pack_into('h', buf, 52, self.BTRefLayerVelBeam1Mms)
        struct.pack_into('h', buf, 54, self.BTRefLayerVelBeam2Mms)
        struct.pack_into('h', buf, 56, self.BTRefLayerVelBeam3Mms)
        struct.pack_into('B', buf, 58, self.BTBeam0RefLayerCorr)
        struct.pack_into('B', buf, 59, self.BTBeam1RefLayerCorr)
        struct.pack_into('B', buf, 60, self.BTBeam2RefLayerCorr)
        struct.pack_into('B', buf, 61, self.BTBeam3RefLayerCorr)
        struct.pack_into('B', buf, 62, self.BTBeam0RefLayerIntensity)
        struct.pack_into('B', buf, 63, self.BTBeam1RefLayerIntensity)
        struct.pack_into('B', buf, 64, self.BTBeam2RefLayerIntensity)
        struct.pack_into('B', buf, 65, self.BTBeam3RefLayerIntensity)
        struct.pack_into('B', buf, 66, self.BTRefPercentGoodBeam0)
        struct.pack_into('B', buf, 67, self.BTRefPercentGoodBeam1)
        struct.pack_into('B', buf, 68, self.BTRefPercentGoodBeam2)
        struct.pack_into('B', buf, 69, self.BTRefPercentGoodBeam3)
        struct.pack_into('H', buf, 70, self.BTMaxDepthDm)
        struct.pack_into('B', buf, 72, self.BTRSSIBeam0)
        struct.pack_into('B', buf, 73, self.BTRSSIBeam1)
        struct.pack_into('B', buf, 74, self.BTRSSIBeam2)
        struct.pack_into('B', buf, 75, self.BTRSSIBeam3)
        struct.pack_into('B', buf, 76, self.Gain)
        struct.pack_into('B', buf, 77, self.BTBeam0RangeCmMbs)
        struct.pack_into('B', buf, 78, self.BTBeam1RangeCmMbs)
        struct.pack_into('B', buf, 79, self.BTBeam2RangeCmMbs)
        struct.pack_into('B', buf, 80, self.BTBeam3RangeCmMbs)
        struct.pack_into('I', buf, 81, self.Reserved)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        
        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        self.BTPingsPerEnsemble     = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTDelayBeforeReaquire  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTCorrMagMin           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTEvalAmpMin           = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTPercentGood          = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTMode                 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTErrVelMax            = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTReserved             = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.BTBeam0RangeCm         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam1RangeCm         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam2RangeCm         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam3RangeCm         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam0VelMms          = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam1VelMms          = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam2VelMms          = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam3VelMms          = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam0Corr            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam1Corr            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam2Corr            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam3Corr            = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam0Amp             = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam1Amp             = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam2Amp             = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam3Amp             = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam0PercentGood     = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam1PercentGood     = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam2PercentGood     = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam3PercentGood     = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.RefLayerMinDm          = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.RefLayerNearDm         = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.RefLayerFarDm          = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTRefLayerVelBeam0Mms  = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTRefLayerVelBeam1Mms  = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTRefLayerVelBeam2Mms  = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTRefLayerVelBeam3Mms  = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BTBeam0RefLayerCorr    = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam1RefLayerCorr    = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam2RefLayerCorr    = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam3RefLayerCorr    = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam0RefLayerIntensity = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam1RefLayerIntensity = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam2RefLayerIntensity = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam3RefLayerIntensity = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRefPercentGoodBeam0 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRefPercentGoodBeam1 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRefPercentGoodBeam2 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRefPercentGoodBeam3 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTMaxDepthDm = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BTRSSIBeam0 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRSSIBeam1 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRSSIBeam2 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTRSSIBeam3 = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.Gain = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam0RangeCmMbs = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam1RangeCmMbs = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam2RangeCmMbs = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.BTBeam3RangeCmMbs = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.Reserved = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        
        return offset
        
    def show(self):
        print("--- RDI PD0 Bottom Track ---")
        print("Beam 0 Range (cm) " + str(self.BTBeam0RangeCm))
        print("Beam 1 Range (cm) " + str(self.BTBeam1RangeCm))
        print("Beam 2 Range (cm) " + str(self.BTBeam2RangeCm))
        print("Beam 3 Range (cm) " + str(self.BTBeam3RangeCm))
        print("Beam 0 Vel (mm/s) " + str(self.BTBeam0VelMms))
        print("Beam 1 Vel (mm/s) " + str(self.BTBeam1VelMms))
        print("Beam 2 Vel (mm/s) " + str(self.BTBeam2VelMms))
        print("Beam 3 Vel (mm/s) " + str(self.BTBeam3VelMms))


class RDINMEAGGABinary(RDIPD0Interface):

    SIZE_BYTES        = 57
    NMEA_HEADER_BYTES = 14
    ID                = 0x2022
    SID               = 104
    SZ_HEADER_BYTES   = 7
    SZ_UTC_BYTES      = 10

    def __init__(self):
        self.HeaderId    = self.ID
        self.SpecificId  = self.SID
        self.MessageSize = 0
        self.DeltaTime   = 0
        self.szHeader = [0, 0, 0, 0, 0, 0, 0]
        self.szUTC = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.NumCells = 0
        self.dLatitude = 0
        self.tcNS = 0
        self.dLongitude = 0
        self.tcEW = 0
        self.ucQuality = 0
        self.ucNmbSat = 0
        self.fHDOP = 0
        self.fAltitude = 0
        self.tcAltUnit = 0
        self.fGeoid = 0
        self.tcGeoidUnit = 0
        self.fAgeDGPS = 0
        self.sRefStationId = 0

    def get_id(self):
        # return the SID here rather than the ID to keep binary NMEA messages unique
        return self.SID
    
    def calc_length(self):
        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('H', buf, 2,  self.SpecificId)
        struct.pack_into('H', buf, 4,  self.MessageSize)
        struct.pack_into('Q', buf, 6,  self.DeltaTime)

        offset = self.NMEA_HEADER_BYTES + self.SZ_HEADER_BYTES + self.SZ_UTC_BYTES

        struct.pack_into('d', buf, 0  + offset, self.dLatitude)
        struct.pack_into('c', buf, 8  + offset, self.tcNS)
        struct.pack_into('d', buf, 9  + offset, self.dLongitude)
        struct.pack_into('c', buf, 17 + offset, self.tcEW)
        struct.pack_into('B', buf, 18 + offset, self.ucQuality)
        struct.pack_into('B', buf, 19 + offset, self.ucNmbSat)
        struct.pack_into('f', buf, 20 + offset, self.fHDOP)
        struct.pack_into('f', buf, 24 + offset, self.fAltitude)
        struct.pack_into('c', buf, 28 + offset, self.tcAltUnit)
        struct.pack_into('f', buf, 29 + offset, self.fGeoid)
        struct.pack_into('c', buf, 33 + offset, self.tcGeoidIUnit)
        struct.pack_into('f', buf, 34 + offset, self.fAgeDGPS)
        struct.pack_into('H', buf, 38 + offset, self.sRefStationId)

        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def deserialise(self, buf):
        
        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        self.SpecificId  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.MessageSize = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.DeltaTime = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 8

        for i in range(self.SZ_HEADER_BYTES):
            self.szHeader[i] = struct.unpack("c", buf[offset : offset + 1])[0]
            offset += 1

        self.szUTC          = int.from_bytes(buf[offset : offset + self.SZ_UTC_BYTES], byteorder='little')   
        offset += self.SZ_UTC_BYTES

        self.dLatitude      = struct.unpack("d", buf[offset : offset + 8])[0]
        offset += 8;

        self.tcNS           = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.dLongitude     = struct.unpack("d", buf[offset : offset + 8])[0]
        offset += 8;

        self.tcEW           = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.ucQuality      = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1;

        self.ucNmbSat       = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1;

        self.fHDOP          = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.fAltitude      = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcAltUnit      = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fGeoid         = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcGeoidIUnit   = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fAgeDGPS       = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.sRefStationId  = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2;

        return offset

    def show(self):
        print("--- RDINMEAGGABinary ---")
        print("Latitude:  " + str(self.dLatitude))
        print("Longitude: " + str(self.dLongitude))
        print("Altitude: "  + str(self.fAltitude))


class RDINMEAGGAString(RDIPD0Interface):

    SIZE_BYTES = 98
    GGA_BYTES  = 84
    ID         = 0x2101

    def __init__(self):
        self.HeaderId = self.ID
        self.GGAString = bytes(self.GGA_BYTES)

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('s', buf, 2,  self.GGAString)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    

        #self.GGAString = struct.unpack("s", buf[offset : offset + self.GGA_BYTES])

        return self.SIZE_BYTES

    def show(self):
        print("--- RDINMEAGGAString ---")
        

class RDINMEAVTGBinary(RDIPD0Interface):
    
    SIZE_BYTES        = 28
    NMEA_HEADER_BYTES = 14
    ID                = 0x2022
    SID               = 105
    SZ_HEADER_BYTES   = 7

    def __init__(self):
        self.HeaderId    = self.ID
        self.SpecificId  = self.SID
        self.MessageSize = self.NMEA_HEADER_BYTES + self.SIZE_BYTES
        self.DeltaTime   = 0
        self.szHeader = [0, 0, 0, 0, 0, 0, 0]
        self.fCOGTrue = 0
        self.tcTrueIndicator = 0
        self.fCOGMag = 0
        self.tcMagnIndicator = 0
        self.fSpdOverGroundKts = 0
        self.tcKtsIndicator = 0
        self.fSpdOverGroundKmh = 0
        self.tcKmhIndicator = 0
        self.tcModeIndicator = 0

    def get_id(self):
        return self.SID
    
    def calc_length(self):
        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def serialise(self, buf):
        
        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('H', buf, 2,  self.SpecificId)
        struct.pack_into('H', buf, 4,  self.MessageSize)
        struct.pack_into('Q', buf, 6,  self.DeltaTime)

        for i in range(self.SZ_HEADER_BYTES):
            
            struct.pack_into('c', buf, self.NMEA_HEADER_BYTES + i,  self.szHeader[i])

        offset = self.NMEA_HEADER_BYTES + self.SZ_HEADER_BYTES

        struct.pack_into('f', buf, 0  + offset, self.fCOGTrue)
        struct.pack_into('c', buf, 4  + offset, self.tcTrueIndicator)
        struct.pack_into('f', buf, 5  + offset, self.fCOGMag)
        struct.pack_into('c', buf, 9  + offset, self.tcMagnIndicator)
        struct.pack_into('f', buf, 10 + offset, self.fSpdOverGroundKts)
        struct.pack_into('c', buf, 14 + offset, self.tcKtsIndicator)
        struct.pack_into('f', buf, 15 + offset, self.fSpdOverGroundKmh)
        struct.pack_into('c', buf, 19 + offset, self.tcKmhIndicator)
        struct.pack_into('c', buf, 20 + offset, self.tcModeIndicator)

        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def deserialise(self, buf):

        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        self.SpecificId  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.MessageSize = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.DeltaTime = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 8

        for i in range(self.SZ_HEADER_BYTES):
            self.szHeader[i] = struct.unpack("c", buf[offset : offset + 1])[0]
            offset += 1

        self.fCOGTrue           = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcTrueIndicator    = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fCOGMag            = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcMagnIndicator    = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fSpdOverGroundKts  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcKtsIndicator     = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fSpdOverGroundKmh  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcKmhIndicator     = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.tcModeIndicator    = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        return offset

    def show(self):
        print("--- RDINMEAVTGBinary ---")
        print("COG True : " + str(self.fCOGTrue))
        print("Speed Kmh: " + str(self.fSpdOverGroundKmh))

class RDINMEAVTGString(RDIPD0Interface):

    SIZE_BYTES = 60
    VTG_BYTES  = 46
    ID         = 0x2102

    def __init__(self):
        self.HeaderId  = self.ID
        self.VTGString = bytes(self.VTG_BYTES)

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):
        
        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('s', buf, 2,  self.VTGString)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        #self.VTGString = struct.unpack("s", buf[offset : offset + self.VTG_BYTES])

        return self.SIZE_BYTES

    def show(self):
        print("--- RDINMEAVTGString ---")


class RDINMEADBTBinary(RDIPD0Interface):
    
    SIZE_BYTES        = 22
    NMEA_HEADER_BYTES = 14
    ID                = 0x2022
    SID               = 106    
    SZ_HEADER_BYTES   = 7
    
    def __init__(self):
        self.HeaderId    = self.ID
        self.SpecificId  = self.SID
        self.MessageSize = self.NMEA_HEADER_BYTES + self.SIZE_BYTES
        self.DeltaTime   = 0
        self.szHeader = [0, 0, 0, 0, 0, 0, 0]
        self.fWaterDepth_ft = 0
        self.tcFeetIndicator = 0
        self.fWaterDepth_m = 0
        self.tcMeterIndicator = 0
        self.fWaterDepth_F = 0
        self.tcFathomIndicator = 0

    def get_id(self):
        return self.SID
    
    def calc_length(self):
        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('H', buf, 2,  self.SpecificId)
        struct.pack_into('H', buf, 4,  self.MessageSize)
        struct.pack_into('Q', buf, 6,  self.DeltaTime)

        for i in range(self.SZ_HEADER_BYTES):
            
            struct.pack_into('c', buf, self.NMEA_HEADER_BYTES + i,  self.szHeader[i])

        offset = self.NMEA_HEADER_BYTES + self.SZ_HEADER_BYTES

        struct.pack_into('f', buf, 0  + offset, self.fWaterDepth_ft)
        struct.pack_into('c', buf, 4  + offset, self.tcFeetIndicator)
        struct.pack_into('f', buf, 5  + offset, self.fWaterDepth_m)
        struct.pack_into('c', buf, 9  + offset, self.tcMeterIndicator)
        struct.pack_into('f', buf, 10 + offset, self.fWaterDepth_F)
        struct.pack_into('c', buf, 14 + offset, self.tcFathomIndicator)

        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def deserialise(self, buf):
        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        self.SpecificId  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.MessageSize = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.DeltaTime   = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 8

        for i in range(self.SZ_HEADER_BYTES):
            self.szHeader[i] = struct.unpack("c", buf[offset : offset + 1])[0]
            offset += 1

        self.fWaterDepth_ft     = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcFeetIndicator    = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fWaterDepth_m      = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcMeterIndicator   = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        self.fWaterDepth_F      = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4;

        self.tcFathomIndicator  = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        return offset

    def show(self):
        print("--- RDINMEADBTBinary ---")

class RDINMEADBTString(RDIPD0Interface):

    SIZE_BYTES = 44
    DBT_BYTES  = 30
    ID         = 0x2100

    def __init__(self):
        self.HeaderId = self.ID
        self.DBTString = bytes(self.DBT_BYTES)

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):
        
        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('s', buf, 2,  self.DBTString)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        #for i in range(self.DBT_BYTES):
        #    self.DBTString[i] = int.from_bytes(buf[offset : offset + 1], byteorder='little')
        #    offset += 1

        return self.SIZE_BYTES

    def show(self):
        print("--- RDINMEADBTString ---")


class RDINMEAHDTBinary(RDIPD0Interface):
    
    SIZE_BYTES        = 16
    NMEA_HEADER_BYTES = 14
    ID                = 0x2022
    SID               = 107    
    SZ_HEADER_BYTES   = 7

    def __init__(self):
        self.HeaderId    = self.ID
        self.SpecificId  = self.SID
        self.MessageSize = self.NMEA_HEADER_BYTES + self.SIZE_BYTES
        self.DeltaTime   = 0
        self.szHeader = [0, 0, 0, 0, 0, 0, 0]
        self.dHeading = 0
        self.tcHeadingIndicator = 0

    def get_id(self):
        return self.SID
    
    def calc_length(self):
        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('H', buf, 2,  self.SpecificId)
        struct.pack_into('H', buf, 4,  self.MessageSize)
        struct.pack_into('Q', buf, 6,  self.DeltaTime)

        for i in range(self.SZ_HEADER_BYTES):
            
            struct.pack_into('c', buf, self.NMEA_HEADER_BYTES + i,  self.szHeader[i])

        offset = self.NMEA_HEADER_BYTES + self.SZ_HEADER_BYTES

        struct.pack_into('d', buf, 0  + offset, self.dHeading)
        struct.pack_into('c', buf, 8  + offset, self.tcHeadingIndicator)
        
        return (self.NMEA_HEADER_BYTES + self.SIZE_BYTES)

    def deserialise(self, buf):
        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        self.SpecificId  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.MessageSize = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.DeltaTime   = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 8

        for i in range(self.SZ_HEADER_BYTES):
            self.szHeader[i] = struct.unpack("c", buf[offset : offset + 1])[0]
            offset += 1

        self.dHeading           = struct.unpack("d", buf[offset : offset + 8])[0]
        offset += 8;

        self.tcHeadingIndicator = struct.unpack("c", buf[offset : offset + 1])[0]
        offset += 1;

        return offset

    def show(self):
        print("--- RDINMEAHDTBinary ---")


class RDINMEAHDTString(RDIPD0Interface):

    SIZE_BYTES = 32
    HDT_BYTES  = 15
    ID         = 0x2104

    def __init__(self):
        self.HeaderId = self.ID
        self.HDTString = bytes(self.HDT_BYTES)

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):
        
        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('s', buf, 2,  self.HDTString)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId = int.from_bytes(buf[0:2], byteorder='little')   
        
        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        #for i in range(self.HDT_BYTES):
        #    self.HDTString[i] = int.from_bytes(buf[offset : offset + 1], byteorder='little')
        #    offset += 1

        return self.SIZE_BYTES

    def show(self):
        print("--- RDINMEAHDTString ---")


class SONSystemConfig(RDIPD0Interface):
    
    SIZE_BYTES          = 50
    SERIAL_NUMBER_BYTES = 3
    ID                  = 0x6060

    def __init__(self):
        self.HeaderId = self.ID
        self.TypeNumber = 0
        self.SubTypeField = 0
        self.SerialNumber = [None]*self.SERIAL_NUMBER_BYTES
        self.SequenceNumber = 0
        self.DSPFirmwareVersion = 0
        self.ARMFirmwareVersion = 0
        self.FPGAFirmwareVersion = 0
        self.WebServerFirmwareVersion = 0
        self.OffsetXGPS = 0
        self.OffsetYGPS = 0
        self.OffsetZGPS = 0
        self.PitchOffsetDegrees = 0
        self.RollOffsetDegrees = 0
        self.HeadingOffsetDegrees = 0

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):

        offset = 0

        struct.pack_into('H', buf, offset,  self.HeaderId)
        offset += 2

        struct.pack_into('H', buf, offset,  self.TypeNumber)
        offset += 2

        struct.pack_into('H', buf, offset,  self.SubTypeField)
        offset += 2


        for i in range(self.SERIAL_NUMBER_BYTES):
            struct.pack_into('B', buf, offset, self.SerialNumber[i])
            offset += 1

        struct.pack_into('B', buf, offset, self.SequenceNumber)
        offset += 1

        struct.pack_into('I', buf, offset,  self.DSPFirmwareVersion)
        offset += 4
        
        struct.pack_into('I', buf, offset,  self.ARMFirmwareVersion)
        offset += 4
        
        struct.pack_into('I', buf, offset,  self.FPGAFirmwareVersion)
        offset += 4
        
        struct.pack_into('I', buf, offset,  self.WebServerFirmwareVersion)
        offset += 4
        
        struct.pack_into('f', buf, offset,  self.OffsetXGPS)
        offset += 4
        
        struct.pack_into('f', buf, offset,  self.OffsetYGPS)
        offset += 4
        
        struct.pack_into('f', buf, offset,  self.OffsetZGPS)
        offset += 4
        
        struct.pack_into('f', buf, offset,  self.PitchOffsetDegrees)
        offset += 4
        
        struct.pack_into('f', buf, offset,  self.RollOffsetDegrees)
        offset += 4
        
        struct.pack_into('f', buf, offset,  self.HeadingOffsetDegrees)
        offset += 4

        return offset

    def deserialise(self, buf):
        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]

        if headerId != self.HeaderId:
            return -1

        offset += 2
    
        self.TypeNumber                 = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.SubTypeField               = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        for i in range(self.SERIAL_NUMBER_BYTES):
            self.SerialNumber[i] = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1

        self.SequenceNumber             = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.DSPFirmwareVersion         = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.ARMFirmwareVersion         = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.FPGAFirmwareVersion        = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.WebServerFirmwareVersion   = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.OffsetXGPS                 = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.OffsetYGPS                 = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.OffsetZGPS                 = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.PitchOffsetDegrees         = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.RollOffsetDegrees          = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.HeadingOffsetDegrees       = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4
 
        return offset

    def show(self):
        print("--- SONSystemConfig ---")
        print("Type number: " + str(self.TypeNumber))
        print("Sub-Type number: " + str(self.SubTypeField))
        print("Sequence number: " + str(self.SequenceNumber))

class SONVariableLeader(RDIPD0Interface):
    
    SIZE_BYTES   = 38
    T0TIME_BYTES = 6
    ID           = 0x6160

    def __init__(self):
        self.HeaderId = self.ID
        self.Status = 0
        self.PingId = 0
        self.T0Time = [0, 0, 0, 0, 0, 0]
        self.Waveform = 0
        self.RxGain = 0
        self.TxGain = 0
        self.SensorValidity = 0
        self.INSBeam0VelMms = 0
        self.INSBeam1VelMms = 0
        self.INSBeam2VelMms = 0
        self.INSBeam3VelMms = 0
        self.INSHeading = 0
        self.INSPitch = 0
        self.INSRoll = 0
        self.BeamAmbLimitMms = 0
        self.BeamVelRmsMms = 0
        self.Reserved = 0

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)
        struct.pack_into('B', buf, 2,  self.Status)
        struct.pack_into('B', buf, 3,  self.PingId)
        
        for i in range(self.T0TIME_BYTES):
            
            struct.pack_into('B', buf, 4 + i, self.T0Time[i])
        
        struct.pack_into('I', buf, 10,  self.Waveform)
        struct.pack_into('H', buf, 14,  self.RxGain)
        struct.pack_into('H', buf, 16,  self.TxGain)
        struct.pack_into('B', buf, 18,  self.SensorValidity)
        struct.pack_into('h', buf, 19,  self.INSBeam0VelMms)
        struct.pack_into('h', buf, 21,  self.INSBeam1VelMms)
        struct.pack_into('h', buf, 23,  self.INSBeam2VelMms)
        struct.pack_into('h', buf, 25,  self.INSBeam3VelMms)
        struct.pack_into('H', buf, 27,  self.INSHeading)
        struct.pack_into('h', buf, 29,  self.INSPitch)
        struct.pack_into('h', buf, 31,  self.INSRoll)
        struct.pack_into('H', buf, 33,  self.BeamAmbLimitMms)
        struct.pack_into('B', buf, 35,  self.BeamVelRmsMms)
        struct.pack_into('H', buf, 36,  self.Reserved)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId                 = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        self.Status = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.PingId = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        for i in range(self.T0TIME_BYTES):
            self.T0Time[i] = struct.unpack("B", buf[offset : offset + 1])[0]
            offset += 1

        self.Waveform = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.RxGain = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.TxGain = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.SensorValidity = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.INSBeam0VelMms = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.INSBeam1VelMms = struct.unpack("h", buf[offset : offset + 2])[0] 
        offset += 2

        self.INSBeam2VelMms = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.INSBeam3VelMms = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.INSHeading = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.INSPitch = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.INSRoll = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.BeamAmbLimitMms = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.BeamVelRmsMms = struct.unpack("B", buf[offset : offset + 1])[0]
        offset += 1

        self.Reserved = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        return offset

    def show(self):
        print("--- SONVariableLeader ---")
        print("ping ID: " + str(self.PingId))
        print("INS Heading: " + str(self.INSHeading))
        print("INS Pitch: "   + str(self.INSPitch))
        print("INS Roll: "    + str(self.INSRoll))
        print("Amb. Limit: "  + str(self.BeamAmbLimitMms))


class PD0Message:

    RESERVED_SIZE_BYTES = 2
    CHECKSUM_SIZE_BYTES = 2

    def __init__(self):
        
        self.Fields                  = {}
        self.Fields[RDIPD0Header.ID] = RDIPD0Header()
        
    def get_section(self, id):
         
        return self.Fields.get(id)
        
    def add_section(self, RDIInterface):            
        
        self.Fields[RDIInterface.get_id()] = RDIInterface

    def calc_length(self):

        RDIHeader                 = self.Fields[RDIPD0Header.ID]
        RDIHeader.Indices         = self.calc_indices()
        RDIHeader.NumberDataTypes = len(self.Fields) - 1
        
        length_bytes = 0
        
        for key, value in self.Fields.items():
            
            length_bytes += value.calc_length()


        length_bytes += self.RESERVED_SIZE_BYTES;
        length_bytes += self.CHECKSUM_SIZE_BYTES;

        RDIHeader.EnsembleBytes = length_bytes - self.CHECKSUM_SIZE_BYTES;

        return length_bytes;


    def calc_indices(self):
        
        offset  = RDIPD0Header.SIZE_BYTES + 2 * (len(self.Fields) - 1)

        indices = array.array('H')
        
        for key, value in self.Fields.items():
            
            if (key != RDIPD0Header.ID):
                
                length = value.calc_length();

                indices.append(offset)

                offset += length;
            

        return indices;
    

    def calc_checksum(self, buf):
        checksum = 0;

        for i in buf:
            checksum += i

        return (checksum % 65535);

    def serialise(self, buf):
        
        if (self.Fields.get(RDIPD0Header.ID) == None):
            return -1

        RDIHeader = self.Fields[RDIPD0Header.ID]
        
        RDIHeader.EnsembleBytes   = self.calc_length() - self.CHECKSUM_SIZE_BYTES ;
        RDIHeader.Indices         = self.calc_indices();
        RDIHeader.NumberDataTypes = len(self.Fields) - 1

        offset = 0
        offset = RDIHeader.serialise(buf)

        for key, value in self.Fields.items():
            
            if (key != RDIPD0Header.ID):                
                
                tmp = bytearray(value.calc_length())

                bytes_serialised = value.serialise(tmp);
                buf[offset:offset + bytes_serialised] = tmp

                #print("PD0 serialise: 0x{0:04x}".format(key) + " " + str(bytes_serialised) + " " + str(offset))

                offset += bytes_serialised;

        offset += self.RESERVED_SIZE_BYTES;

        # calculate checksum - untested
        checksum = self.calc_checksum(buf[0 : bytes_serialised]);
        struct.pack_into('H', buf, offset, checksum)

        offset += self.CHECKSUM_SIZE_BYTES;

        return offset;    

    def deserialise(self, buf):
        
        offset = 0
                
        if (len(buf) < RDIPD0Header.SIZE_BYTES):     
            # just exit if there aren't enough bytes
            return -1

        elif (len(buf) == RDIPD0Header.SIZE_BYTES): 
            # if the header is all that is available, decode the first six bytes so we know the number of fields        
            offset += self.Fields[RDIPD0Header.ID].deserialise(buf[offset : offset + RDIPD0Header.SIZE_BYTES])
            return RDIPD0Header.SIZE_BYTES
        
        else:
            # if we have more bytes than just the header, deserialise and continue
            offset += self.Fields[RDIPD0Header.ID].deserialise(buf)


        for i in range(self.Fields[RDIPD0Header.ID].NumberDataTypes):
            headerId = int.from_bytes(buf[self.Fields[RDIPD0Header.ID].Indices[i] : 
                                          self.Fields[RDIPD0Header.ID].Indices[i] + 2], 
                                      byteorder='little')   

            if   (headerId == RDIFixedLeader.ID):
                RDIInterface = RDIFixedLeader()
            
            elif (headerId == RDIVariableLeader.ID):
                RDIInterface = RDIVariableLeader()
            
            elif (headerId == RDIVelocity.ID):
                RDIInterface = RDIVelocity()
                RDIInterface.set(self.Fields[RDIFixedLeader.ID].NumberOfBeams, 
                                 self.Fields[RDIFixedLeader.ID].NumberOfCells)
            
            elif (headerId == RDIXC.ID):
                RDIInterface = RDIXC()
                RDIInterface.set(self.Fields[RDIFixedLeader.ID].NumberOfBeams, 
                                 self.Fields[RDIFixedLeader.ID].NumberOfCells)
            
            elif (headerId == RDIIntensity.ID):
                RDIInterface = RDIIntensity()
                RDIInterface.set(self.Fields[RDIFixedLeader.ID].NumberOfBeams, 
                                 self.Fields[RDIFixedLeader.ID].NumberOfCells)
            
            elif (headerId == RDIPrctGood.ID):
                RDIInterface = RDIPrctGood()
                RDIInterface.set(self.Fields[RDIFixedLeader.ID].NumberOfBeams, 
                                 self.Fields[RDIFixedLeader.ID].NumberOfCells)
            
            elif (headerId == RDIBottomTrack.ID):                         
                RDIInterface = RDIBottomTrack()
        
            elif (headerId == RDINMEAGGABinary.ID): # it says GGA here but it's really just the general NMEA header

                SID = struct.unpack("H", buf[self.Fields[RDIPD0Header.ID].Indices[i] + 2: 
                                             self.Fields[RDIPD0Header.ID].Indices[i] + 4])[0]

                if (SID == RDINMEAGGABinary.SID):
                    RDIInterface = RDINMEAGGABinary()
                
                elif (SID == RDINMEAVTGBinary.SID):
                    RDIInterface = RDINMEAVTGBinary()
        
                elif (SID == RDINMEADBTBinary.SID):
                    RDIInterface = RDINMEADBTBinary()
        
                elif (SID == RDINMEAHDTBinary.SID):
                    RDIInterface = RDINMEAHDTBinary()
        
        
            elif (headerId == RDINMEAGGAString.ID):
                RDIInterface = RDINMEAGGAString()
        
            elif (headerId == RDINMEAVTGString.ID):
                RDIInterface = RDINMEAVTGString()
        
            elif (headerId == RDINMEADBTString.ID):
                RDIInterface = RDINMEADBTString()
        
            elif (headerId == RDINMEAHDTString.ID):
                RDIInterface = RDINMEAHDTString()
        
            elif (headerId == SONSystemConfig.ID):
                RDIInterface = SONSystemConfig()
        
            elif (headerId == SONVariableLeader.ID):   
                RDIInterface = SONVariableLeader()

            elif (headerId == SONOriginSystemLeader.ID):
                RDIInterface = SONOriginSystemLeader()

            elif (headerId == SONPIESVariableLeader.ID):
                RDIInterface = SONPIESVariableLeader()

            else:
                section = self.get_section(headerId)
                
                if section is None:
                    print("No decoder for section " + "0x{0:04x}".format(headerId))                    
                else:
                    print("Found decoder for section " + "0x{0:04x}".format(headerId))
                    RDIInterface = section

            if 'RDIInterface' in locals():
                
                #self.Fields[RDIPD0Header.ID].Indices[i]
                bytes_deserialised = RDIInterface.deserialise(buf[self.Fields[RDIPD0Header.ID].Indices[i] : 
                                                                  self.Fields[RDIPD0Header.ID].Indices[i] + RDIInterface.calc_length()])
                
                #if (headerId != RDIPD0Header.ID):                
                #
                #    print("PD0 deserialise: 0x{0:04x}".format(headerId) + " index: " + 
                #          str(self.Fields[RDIPD0Header.ID].Indices[i]) + " length: " + str(RDIInterface.calc_length()) + " deser: " + str(bytes_deserialised) + " total: " + str(offset))

                offset += bytes_deserialised
                
                self.add_section(RDIInterface)
                
                del RDIInterface
        
        # should probably deserialise here
        offset += self.RESERVED_SIZE_BYTES;
        offset += self.CHECKSUM_SIZE_BYTES;
        
        return offset

    def show(self):
        print("=== RDI PD0 Message === " + str(len(self.Fields)))
        for key, value in self.Fields.items():
            print("=== RDI PD0 Message " + hex(key))
            
            value.show()

    def transform(self, frame):
        
        fixed_leader    = self.get_section(RDIFixedLeader.ID)
        variable_leader = self.get_section(RDIVariableLeader.ID)
                
        # should really use this to determine what transform to perform
        #fixed_leader.SystemConfig = 0
        
        fixed_leader.CoordTxfrm = 0
        
        if (frame == Frame.BEAM):
            pass
        
        elif (frame == Frame.INSTRUMENT):
                
            fixed_leader.CoordTxfrm |= (1 << 3)

        elif(frame == Frame.VESSEL):
                
            fixed_leader.CoordTxfrm |= (1 << 4)

        elif(frame == Frame.EARTH):
                
            fixed_leader.CoordTxfrm |= (1 << 2) # we have used tilts in this transformation
            fixed_leader.CoordTxfrm |= (1 << 3)
            fixed_leader.CoordTxfrm |= (1 << 4)
 
        else:
            pass
              

        self.get_section(RDIVelocity.ID).transform(frame, fixed_leader, variable_leader)

class SONPIESVariableLeader(RDIPD0Interface):
    
    SIZE_BYTES   = 74
    ID           = 0x62ca

    def __init__(self):
        self.HeaderId = self.ID
        self.SV1_ms = 0
        self.SV2_ms = 0
        self.TOF1_s = 0
        self.TOF2_s = 0
        self.Magnitude = 0
        self.PeakIndex = 0
        self.RMLEIndex = 0
        self.RMLERange = 0
        self.PretriggerRMS = 0
        self.HalfLife_s = 0
        self.R2 = 0
        self.Pressure_kPa = 0
        self.Depth_m = 0
        self.Zoffset_m = 0
        self.Latitdue_DD = 0
        self.Patmos_kPa = 0
        self.TimeNS = 0

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.HeaderId)

        struct.pack_into('f', buf, 2,  self.SV1_ms)
        struct.pack_into('f', buf, 6,  self.SV2_ms)
        struct.pack_into('f', buf, 10,  self.TOF1_s)
        struct.pack_into('f', buf, 14,  self.TOF2_s)
        struct.pack_into('f', buf, 18,  self.Magnitude)
        struct.pack_into('I', buf, 22,  self.PeakIndex)
        struct.pack_into('I', buf, 26,  self.RMLEIndex)
        struct.pack_into('f', buf, 30,  self.RMLERange)
        struct.pack_into('f', buf, 34,  self.PretriggerRMS)
        struct.pack_into('f', buf, 38,  self.HalfLife_s)
        struct.pack_into('f', buf, 42,  self.R2)
        struct.pack_into('f', buf, 46,  self.Pressure_kPa)
        struct.pack_into('f', buf, 50,  self.Depth_m)
        struct.pack_into('f', buf, 54,  self.Zoffset_m)
        struct.pack_into('f', buf, 58,  self.Latitdue_DD)
        struct.pack_into('f', buf, 62,  self.Patmos_kPa)
        struct.pack_into('Q', buf, 66,  self.TimeNS)

        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId                 = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.HeaderId:
            return -1

        offset += 2

        self.SV1_ms = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.SV2_ms = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.TOF1_s = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.TOF2_s = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.Magnitude = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.PeakIndex = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.RMLEIndex = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.RMLERange = struct.unpack("f", buf[offset : offset + 4])[0] 
        offset += 4

        self.PretriggerRMS = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.HalfLife_s = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.R2 = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.Pressure_kPa = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.Depth_m = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.Zoffset_m = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.Latitdue_kPa = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.Patmos_kPa = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.TimeNS = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 4

        return offset

    def show(self):
        print("--- SONVPIESariableLeader ---")
        self.ciT_PIESRequestTime.show()
        print("SV1:    " + str(self.SV1_ms))
        print("SV2:    " + str(self.SV2_ms))
        print("TOF1:    " + str(self.TOF1_s))
        print("TOF2:    " + str(self.TOF2_s))
        print("Magnitude: " + str(self.Magnitude))
        print("Peak Index:    " + str(self.PeakIndex))
        print("RMLE Index:    " + str(self.RMLEIndex))
        print("Peak Range:    " + str(self.RMLERange))
        print("Pretrigger RMS:    " + str(self.PretriggerRMS))
        print("Half Life:    " + str(self.HalfLife_s))
        print("R2:    " + str(self.R2))
        print("Pressure:    " + str(self.Pressure_kPa))
        print("Depth:    " + str(self.Depth_m))
        print("Zoffset:    " + str(self.Zoffset_m))
        print("Latitude:    " + str(self.Latitude_DD))
        print("Patmos:    " + str(self.Patmos_kPa))

class SONOriginSystemLeader(RDIPD0Interface):
    
    SIZE_BYTES   = 72
    ID           = 0x6201

    def __init__(self):
        self.IdCode = self.ID
        self.u32_SerialNum = 0
        self.u16_SubSerialNum = 0
        self.u16_TypeNum = 0
        self.u16_SubTypeNum = 0
        self.u16_BeamControl = 0
        self.u16_EventID = 0
        self.u16_PingID = 0
        self.u16_BurstID = 0
        self.u16_CaptureDurationS = 0
        self.u32_SleepDurationS = 0
        self.u16_PingFrequency_mHz = 0
        self.u16_BeamIndex = 0
        self.u32_PingCounter = 0
        self.u16_NoBeams = 0
        self.u32_EventStartTimeS = 0
        self.u32_EventStartTime_us = 0
        self.u64_UnixEpochTime_ms = 0
        self.u32_CarrierFrequencyHz = 0
        self.u32_RxBandwidthHz = 0
        self.u32_TxBandwidthHz = 0
        self.u16_RxGaindB = 0
        self.u16_TxLevel = 0
        self.u16_SIG_BaseCounts = 0
        self.u16_SIG_BT = 0

    def get_id(self):
        return self.ID
    
    def calc_length(self):
        return self.SIZE_BYTES

    def serialise(self, buf):

        struct.pack_into('H', buf, 0,  self.IdCode)
        struct.pack_into('I', buf, 2,   self.u32_SerialNum)
        struct.pack_into('H', buf, 6,   self.u16_SubSerialNum)
        struct.pack_into('H', buf, 8,  self.u16_TypeNum)
        struct.pack_into('H', buf, 10,  self.u16_SubTypeNum)
        struct.pack_into('H', buf, 12,  self.u16_BeamControl)
        struct.pack_into('H', buf, 14,  self.u16_EventID)
        struct.pack_into('H', buf, 16,  self.u16_PingID)
        struct.pack_into('H', buf, 18,  self.u16_BurstID)
        struct.pack_into('H', buf, 20,  self.u16_CaptureDurationS)
        struct.pack_into('I', buf, 22,  self.u32_SleepDurationS)
        struct.pack_into('H', buf, 26,  self.u16_PingFrequency_mHz)
        struct.pack_into('H', buf, 28,  self.u16_BeamIndex)
        struct.pack_into('I', buf, 30,  self.u32_PingCounter)
        struct.pack_into('H', buf, 34,  self.u16_NoBeams)
        struct.pack_into('I', buf, 36,  self.u32_EventStartTimeS)
        struct.pack_into('I', buf, 40,  self.u32_EventStartTime_us)
        struct.pack_into('Q', buf, 44,  self.u64_UnixEpochTime_ms)

        struct.pack_into('I', buf, 52,  self.u32_CarrierFrequencyHz)
        struct.pack_into('I', buf, 56,  self.u32_RxBandwidthHz)
        struct.pack_into('I', buf, 60,  self.u32_TxBandwidthHz)
        struct.pack_into('H', buf, 64,  self.u16_RxGaindB)
        struct.pack_into('H', buf, 66,  self.u16_TxLevel)
        struct.pack_into('H', buf, 68,  self.u16_SIG_BaseCounts)
        struct.pack_into('H', buf, 70,  self.u16_SIG_BT)
        return self.SIZE_BYTES

    def deserialise(self, buf):
        offset = 0

        headerId = struct.unpack("H", buf[offset : offset + 2])[0]
        
        if headerId != self.IdCode:
            return -1

        offset += 2

        self.u32_SerialNum = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u16_SubSerialNum = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_TypeNum = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_SubTypeNum = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_BeamControl = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_EventID = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u16_PingID = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u16_BurstID = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u16_CaptureDurationS = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u32_SleepDurationS = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u16_PingFrequency_mHz = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u16_BeamIndex = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u32_PingCounter = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u16_NoBeams = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u32_EventStartTimeS = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u32_EventStartTime_us = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u64_UnixEpochTime_ms = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 8
        self.u32_CarrierFrequencyHz = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u32_RxBandwidthHz = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u32_TxBandwidthHz = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4
        self.u16_RxGaindB = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u16_TxLevel = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        self.u16_SIG_BaseCounts = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2        
        self.u16_SIG_BT = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2 

        return offset

    def show(self):
        print("--- SONOriginSystemLeader ---")
        print("u32_SerialNum:    " + str(self.u32_SerialNum))
        print("u16_SubSerialNum:    " + str(self.u16_SubSerialNum))
        print("u16_TypeNum:    " + str(self.u16_TypeNum))
        print("u16_SubTypeNum:    " + str(self.u16_SubTypeNum))
        print("u16_BeamControl: " + str(self.u16_BeamControl))
        print("u16_EventID:    " + str(self.u16_EventID))
        print("u16_PingID:    " + str(self.u16_PingID))
        print("u16_BurstID:    " + str(self.u16_BurstID))
        print("u16_CaptureDurationS:    " + str(self.u16_CaptureDurationS))
        print("u16_CaptureDurationS:    " + str(self.u32_SleepDurationS))
        print("u16_CaptureDurationS:    " + str(self.u16_PingFrequency_mHz))
        print("u16_BeamIndex:    " + str(self.u16_BeamIndex))
        print("u32_PingCounter:    " + str(self.u32_PingCounter))
        print("u16_NoBeams:    " + str(self.u16_NoBeams))
        print("u32_EventStartTimeS:    " + str(self.u32_EventStartTimeS))
        print("u32_EventStartTime_us:    " + str(self.u32_EventStartTime_us))
        print("u64_UnixEpochTime_ms:    " + str(self.u64_UnixEpochTime_ms))
        print("u32_CarrierFrequencyHz:    " + str(self.u32_CarrierFrequencyHz))
        print("u32_RxBandwidthHz:    " + str(self.u32_RxBandwidthHz))
        print("u32_TxBandwidthHz:    " + str(self.u32_TxBandwidthHz))
        print("u16_RxGaindB:    " + str(self.u16_RxGaindB))
        print("u16_TxLevel:    " + str(self.u16_TxLevel))
        print("u16_SIG_BaseCounts:    " + str(self.u16_SIG_BaseCounts))
        print("u16_SIG_BT:    " + str(self.u16_SIG_BT))