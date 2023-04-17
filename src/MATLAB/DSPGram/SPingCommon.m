%%% SPingCommon

classdef SPingCommon

    properties
    
        u32_SerialNum            = 0;
        u16_SubSerialNum         = 0;
        u16_TypeNum              = 0;
        u16_SubTypeNum           = 0;
        u16_BeamControl          = 0;                                
        u16_EventID              = 0;
        u16_PingID               = 0;
        u16_BurstID              = 0;
        u16_CaptureDurationS     = 0;
        u32_SleepDurationS       = 0;
        u16_PingFrequency_mHz    = 0;                        
        u16_BeamIndex            = 0;
        u32_PingCounter          = 0;
        u16_NoBeams              = 0;
        ciT_EventStartTime       = CInstrumentTime();
        u64_UnixEpochTime_ms     = 0;
        u32_CarrierFrequencyHz   = 0;
        u32_RxBandwidthHz        = 0;
        u32_TxBandwidthHz        = 0;
        u16_RxGaindB             = 0;
        u16_TxLevel              = 0;
        u16_SIG_BaseCounts       = 0;
        u16_SIG_BT               = 0;
        n16_Roll_cdeg            = 0;
        n16_Pitch_cdeg           = 0;
        n32_Heading_cdeg         = 0;
        ciT_LocalSensorTimestamp = CInstrumentTime();
        n16_accX_mms2            = 0;
        n16_accY_mms2            = 0;
        n16_accZ_mms2            = 0;
        u32_PressurePa           = 0;
        n32_Temp_mdegC           = 0;
        ciT_RemoteSensorTimestamp = CInstrumentTime();
        u16_PingsPerEnsemble     = 0;
        u16_PingErr_BndStatus    = 0; 
        u32_BndMaxXC             = 0;
        u16_BndMaxXCpos          = 0;
        f32_BndSnr               = 0;
        f32_BndFine_Hz           = 0;
        f32_BndFit_err           = 0;
        
    end
         
    methods
        
        function hdr = SPingCommon()
            
        end
    
        function length_bytes = calc_length(obj)
            
            length_bytes = 4 + ... 
                           2 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ... 
                           2 + ... 
                           2 + ... 
                           4 + ...
                           2 + ...
                           2 + ...
                           4 + ...
                           2 + ...
                           obj.ciT_EventStartTime.calc_length() + ...
                           8 + ... 
                           4 + ... 
                           4 + ... 
                           4 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ...  
                           2 + ... 
                           4 + ...  
                           obj.ciT_LocalSensorTimestamp.calc_length() + ... 
                           2 + ...  
                           2 + ...
                           2 + ...  
                           4 + ...  
                           4 + ...
                           obj.ciT_RemoteSensorTimestamp.calc_length() + ... 
                           2 + ...
                           2 + ...
                           4 + ... 
                           2 + ... 
                           4 + ... 
                           4 + ... 
                           4;            
    
        end
                        
        function obj = deserialise(obj, fid)
          
            obj.u32_SerialNum           = fread(fid, 1, "uint32");       
            obj.u16_SubSerialNum        = fread(fid, 1, "uint16");
            obj.u16_TypeNum             = fread(fid, 1, "uint16");
            obj.u16_SubTypeNum          = fread(fid, 1, "uint16");
            obj.u16_BeamControl         = fread(fid, 1, "uint16");
            obj.u16_EventID             = fread(fid, 1, "uint16");
            obj.u16_PingID              = fread(fid, 1, "uint16");
            obj.u16_BurstID             = fread(fid, 1, "uint16");
            obj.u16_CaptureDurationS    = fread(fid, 1, "uint16");
            obj.u32_SleepDurationS      = fread(fid, 1, "uint32");
            obj.u16_PingFrequency_mHz   = fread(fid, 1, "uint16");
            obj.u16_BeamIndex           = fread(fid, 1, "uint16");
            obj.u32_PingCounter         = fread(fid, 1, "uint32");
            obj.u16_NoBeams             = fread(fid, 1, "uint16");
            obj.ciT_EventStartTime.deserialise(fid);
            obj.u64_UnixEpochTime_ms    = fread(fid, 1, "uint64");
            obj.u32_CarrierFrequencyHz  = fread(fid, 1, "uint32");
            obj.u32_RxBandwidthHz       = fread(fid, 1, "uint32");
            obj.u32_TxBandwidthHz       = fread(fid, 1, "uint32");
            obj.u16_RxGaindB            = fread(fid, 1, "uint16");
            obj.u16_TxLevel             = fread(fid, 1, "uint16");
            obj.u16_SIG_BaseCounts      = fread(fid, 1, "uint16");
            obj.u16_SIG_BT              = fread(fid, 1, "uint16");
            obj.n16_Roll_cdeg           = fread(fid, 1, "int16");
            obj.n16_Pitch_cdeg          = fread(fid, 1, "int16");
            obj.n32_Heading_cdeg        = fread(fid, 1, "int32");
            obj.ciT_LocalSensorTimestamp.deserialise(fid);
            obj.n16_accX_mms2           = fread(fid, 1, "int16");
            obj.n16_accY_mms2           = fread(fid, 1, "int16");
            obj.n16_accZ_mms2           = fread(fid, 1, "int16");
            obj.u32_PressurePa          = fread(fid, 1, "uint32");
            obj.n32_Temp_mdegC          = fread(fid, 1, "int32");
            obj.ciT_RemoteSensorTimestamp.deserialise(fid);
            obj.u16_PingsPerEnsemble    = fread(fid, 1, "uint16");
            obj.u16_PingErr_BndStatus   = fread(fid, 1, "uint16");
            obj.u32_BndMaxXC            = fread(fid, 1, "uint32");
            obj.u16_BndMaxXCpos         = fread(fid, 1, "uint16");
            obj.f32_BndSnr              = fread(fid, 1, "float");
            obj.f32_BndFine_Hz          = fread(fid, 1, "float");
            obj.f32_BndFit_err          = fread(fid, 1, "float");
            
        end
    end
end


