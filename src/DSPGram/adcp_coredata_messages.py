import array
import struct

class CInstrumentTime:

    SIZE_BYTES = 8
    
    def __init__(self):
        self.u32_seconds = 0
        self.u32_microseconds = 0

    def deserialise(self, buf):
        
        offset = 0
        self.u32_seconds = struct.unpack("<I", buf[offset : offset + 4])[0]
        offset += 4
        
        self.u32_microseconds = struct.unpack("<I", buf[offset : offset + 4])[0]
        offset += 4
        
        return self.SIZE_BYTES
        
    def show(self):
        print("--- CInstrumentTime ---")
        print("u32_seconds: " + str(self.u32_seconds))
        print("u32_microseconds: " + str(self.u32_microseconds))


class SPingCommon:

    SIZE_BYTES = 130

    def __init__(self):
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
        self.ciT_EventStartTime = CInstrumentTime()
        self.u64_UnixEpochTime_ms = 0
        self.u32_CarrierFrequencyHz = 0
        self.u32_RxBandwidthHz = 0
        self.u32_TxBandwidthHz = 0
        self.u16_RxGaindB = 0
        self.u16_TxLevel = 0
        self.u16_SIG_BaseCounts = 0
        self.u16_SIG_BT = 0
        self.n16_Roll_cdeg = 0
        self.n16_Pitch_cdeg = 0
        self.n32_Heading_cdeg = 0
        self.ciT_LocalSensorTimestamp = CInstrumentTime()
        self.n16_accX_mms2 = 0
        self.n16_accY_mms2 = 0
        self.n16_accZ_mms2 = 0
        self.u32_PressurePa = 0
        self.n32_Temp_mdegC = 0
        self.ciT_RemoteSensorTimestamp = CInstrumentTime()
        self.u16_PingsPerEnsemble = 0
        self.u16_PingErr_BndStatus = 0
        self.u32_BndMaxXC = 0
        self.u16_BndMaxXCpos = 0
        self.f32_BndSnr = 0
        self.f32_BndFine_Hz = 0
        self.f32_BndFit_err = 0
        
    def deserialise(self, buf):
        
        offset = 0
        
        self.u32_SerialNum   = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u16_SubSerialNum = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.u16_TypeNum   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2
        
        self.u16_SubTypeNum   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_BeamControl  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_EventID   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_PingID   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_BurstID   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_CaptureDurationS   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u32_SleepDurationS   = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u16_PingFrequency_mHz   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_BeamIndex   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u32_PingCounter  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u16_NoBeams  = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        offset += self.ciT_EventStartTime.deserialise(buf[offset : offset + CInstrumentTime.SIZE_BYTES])

        self.u64_UnixEpochTime_ms   = struct.unpack("Q", buf[offset : offset + 8])[0]
        offset += 8

        self.u32_CarrierFrequencyHz  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u32_RxBandwidthHz  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u32_TxBandwidthHz  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u16_RxGaindB   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_TxLevel   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_SIG_BaseCounts   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_SIG_BT   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.n16_Roll_cdeg   = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.n16_Pitch_cdeg   = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.n32_Heading_cdeg  = struct.unpack("i", buf[offset : offset + 4])[0]
        offset += 4

        offset += self.ciT_LocalSensorTimestamp.deserialise(buf[offset : offset + CInstrumentTime.SIZE_BYTES])

        self.n16_accX_mms2   = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.n16_accY_mms2   = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.n16_accZ_mms2   = struct.unpack("h", buf[offset : offset + 2])[0]
        offset += 2

        self.u32_PressurePa  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.n32_Temp_mdegC   = struct.unpack("i", buf[offset : offset + 4])[0]
        offset += 4

        offset += self.ciT_RemoteSensorTimestamp.deserialise(buf[offset : offset + CInstrumentTime.SIZE_BYTES])

        self.u16_PingsPerEnsemble = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_PingErr_BndStatus = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u32_BndMaxXC   = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u16_BndMaxXCpos   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.f32_BndSnr   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4
        
        self.f32_BndFine_Hz   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4
        
        self.f32_BndFit_err   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4
        
        return self.SIZE_BYTES

    def show(self):
        print("--- SPingCommon ---")
        print("No. Beams:    " + str(self.u16_NoBeams))
        print("Beam index:   " + str(self.u16_BeamIndex))
        print("Ping Counter: " + str(self.u32_PingCounter))



class SDSP_v1_0_ADCPGram:

    SIZE_BYTES = 20136
    PING_PROCESSOR_MAXBEAMS = 5
    PING_PROCESSOR_MAXDATA_SAMPLES = 2500
    PING_PROCESSOR_MAXCOUNTS = 15

#    def __init__(self):
#        self.u32_NoBeams = 0

    def __init__(self):       
        self.sCommon = SPingCommon()
        self.u16_XCStartSample = 0
        self.u16_XCEndSample = 0
        self.u16_Stride = 0

        #        self.an16_DPhases  = array.array('h')
        #        self.au16_DPhasesErr      = array.array('H')
        #        self.au16_AVIntensityMag  = array.array('H')
        #        self.au16_AVXCMag      = array.array('H')

        self.an16_DPhases = [0]*self.PING_PROCESSOR_MAXDATA_SAMPLES
        self.au16_DPhasesErr = [0]*self.PING_PROCESSOR_MAXDATA_SAMPLES
        self.au16_AVIntensityMag = [0]*self.PING_PROCESSOR_MAXDATA_SAMPLES
        self.au16_AVXCMag = [0]*self.PING_PROCESSOR_MAXDATA_SAMPLES
        
    def deserialise(self, buf):
        
        offset = 0
        
        offset += self.sCommon.deserialise(buf[offset : offset + SPingCommon.SIZE_BYTES])

        self.u16_XCStartSample   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_XCEndSample   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_Stride   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.an16_DPhases = [t[0] for t in struct.Struct("<h").iter_unpack(buf[offset:offset + 2*self.PING_PROCESSOR_MAXDATA_SAMPLES])]
        offset += 2 * self.PING_PROCESSOR_MAXDATA_SAMPLES

        self.au16_DPhasesErr = [t[0] for t in struct.Struct("<H").iter_unpack(buf[offset:offset + 2*self.PING_PROCESSOR_MAXDATA_SAMPLES])]
        offset += 2 * self.PING_PROCESSOR_MAXDATA_SAMPLES

        self.au16_AVIntensityMag = [t[0] for t in struct.Struct("<H").iter_unpack(buf[offset:offset + 2*self.PING_PROCESSOR_MAXDATA_SAMPLES])]
        offset += 2 * self.PING_PROCESSOR_MAXDATA_SAMPLES

        self.au16_AVXCMag = [t[0] for t in struct.Struct("<H").iter_unpack(buf[offset:offset + 2*self.PING_PROCESSOR_MAXDATA_SAMPLES])]
        offset += 2 * self.PING_PROCESSOR_MAXDATA_SAMPLES

        return self.SIZE_BYTES
        
    def show(self):
        print("--- SDSP_v1_0_ADCPGram ---")
        self.sCommon.show()
        print("XCStart sample:    " + str(self.u16_XCStartSample))
        print("XCEnd sample:   " + str(self.u16_XCEndSample))
        print("stride: " + str(self.u16_Stride))
        


class SDSP_v1_0_BDGram:

    SIZE_BYTES = 20138
    ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES = 5000

    def __init__(self):
        self.sCommon = SPingCommon()
        self.eProcessingStatus = 0
        self.u16_SampleBitDepth = 0
        self.u16_StartBin = 0
        self.u16_Samples = 0
        self.an16_IPingData = [0]*self.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES
        self.an16_QPingData = [0]*self.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES

    def deserialise(self, buf):
        
        offset = 0
        
        offset += self.sCommon.deserialise(buf[offset : offset + SPingCommon.SIZE_BYTES])

        self.eProcessingStatus   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_SampleBitDepth   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_StartBin   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.u16_Samples   = struct.unpack("H", buf[offset : offset + 2])[0]
        offset += 2

        self.an16_IPingData = [t[0] for t in struct.Struct("<h").iter_unpack(buf[offset:offset + 2 * self.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES])]
        offset += 2 * self.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES

        self.an16_QPingData = [t[0] for t in struct.Struct("<h").iter_unpack(buf[offset:offset + 2 * self.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES])]
        offset += 2 * self.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES

        return self.SIZE_BYTES

    def show(self):
        print("--- SDSP_v1_0_BDGram ---")
        self.sCommon.show()
        print("StartBin:    " + str(self.u16_StartBin))
        print("samples :   " + str(self.u16_Samples))

        
class SDSP_PIESGram:

    def __init__(self):
        self.ciT_PIESRequestTime = CInstrumentTime()
        self.f32_SV1 = 0
        self.f32_SV2 = 0
        self.f32_TOF1 = 0
        self.f32_TOF2 = 0
        self.f32_Magnitude = 0
        self.u32_PeakIndex = 0
        self.u32_RMLEIndex = 0
        self.f32_RMLERange = 0
        self.f32_PretriggerRMS = 0
        self.f32_HalfLife = 0
        self.f32_R2 = 0
        self.f32_Pressure = 0
        self.f32_Depth = 0
        self.f32_Zoffset = 0
        self.f32_Latitude = 0
        self.f32_Patmos = 0

    def deserialise(self, buf):
        
        offset = 0

        offset += self.ciT_PIESRequestTime.deserialise(buf[offset : offset + CInstrumentTime.SIZE_BYTES])
        
        self.f32_SV1   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_SV2   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_TOF1   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_TOF2   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_Magnitude   = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.u32_PeakIndex  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.u32_RMLEIndex  = struct.unpack("I", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_RMLERange  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_PretriggerRMS  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_HalfLife  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_R2  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_Pressure  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_Depth  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_Zoffset  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_Latitude  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        self.f32_Patmos  = struct.unpack("f", buf[offset : offset + 4])[0]
        offset += 4

        return self.SIZE_BYTES

    def show(self):
        print("--- SDSP_PIESGram ---")
        self.ciT_PIESRequestTime.show()
        print("SV1:    " + str(self.f32_SV1))
        print("SV2:    " + str(self.f32_SV2))
        print("TOF1:    " + str(self.f32_TOF1))
        print("TOF2:    " + str(self.f32_TOF2))
        print("Magnitude: " + str(self.f32_Magnitude))
        print("Peak Index:    " + str(self.u32_PeakIndex))
        print("RMLE Index:    " + str(self.u32_RMLEIndex))
        print("Peak Range:    " + str(self.f32_RMLERange))
        print("Pretrigger RMS:    " + str(self.f32_PretriggerRMS))
        print("R2:    " + str(self.f32_R2))
        print("Pressure:    " + str(self.f32_Pressure))
        print("Depth:    " + str(self.f32_Depth))
        print("Zoffset:    " + str(self.f32_Zoffset))
        print("Latitude:    " + str(self.f32_Latitude))
        print("Patmos:    " + str(self.f32_Patmos))
