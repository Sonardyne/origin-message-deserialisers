%%% SON Origin System Leader
classdef SONOriginSystemLeader < handle

    properties
    
        ID               = 0
        u32_SerialNum    = 0
        u16_SubSerialNum = 0
        u16_TypeNum      = 0
        u16_SubTypeNum   = 0
        u16_BeamControl  = 0
        u16_EventID      = 0
        u16_PingID       = 0
        u16_BurstID      = 0
        u16_CaptureDurationS = 0
        u32_SleepDurationS = 0
        u16_PingFrequency_mHz = 0
        u16_BeamIndex = 0
        u32_PingCounter = 0
        u16_NoBeams = 0
        u32_EventStartTimeS = 0
        u32_EventStartTime_us = 0
        u64_UnixEpochTime_ms = 0
        u32_CarrierFrequencyHz = 0
        u32_RxBandwidthHz = 0
        u32_TxBandwidthHz = 0
        u16_RxGaindB = 0
        u16_TxLevel = 0
        u16_SIG_BaseCounts = 0
        u16_SIG_BT = 0
        
    endproperties
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x6201;
        endfunction      
    endmethods      

    
    methods
        
        function hdr = SONOriginSystemLeader()
            
        endfunction
    
        function h = create(obj)
          
          h = SONOriginSystemLeader();
          
        endfunction
            
        function length_bytes = calc_length(obj)
            length_bytes = 72;
        endfunction
        
        function deserialise(obj, fid)
        
            obj.ID               = fread(fid, 1, "uint16");
            obj.u32_SerialNum    = fread(fid, 1, "uint32");
            obj.u16_SubSerialNum = fread(fid, 1, "uint16");
            obj.u16_TypeNum      = fread(fid, 1, "uint16");
            obj.u16_SubTypeNum   = fread(fid, 1, "uint16");
            obj.u16_BeamControl  = fread(fid, 1, "uint16");
            obj.u16_EventID      = fread(fid, 1, "uint16");
            obj.u16_PingID       = fread(fid, 1, "uint16");
            obj.u16_BurstID      = fread(fid, 1, "uint16");
            obj.u16_CaptureDurationS = fread(fid, 1, "uint16");
            obj.u32_SleepDurationS = fread(fid, 1, "uint32");
            obj.u16_PingFrequency_mHz = fread(fid, 1, "uint16");
            obj.u16_BeamIndex = fread(fid, 1, "uint16");
            obj.u32_PingCounter = fread(fid, 1, "uint32");
            obj.u16_NoBeams = fread(fid, 1, "uint16");
            obj.u32_EventStartTimeS = fread(fid, 1, "uint32");
            obj.u32_EventStartTime_us = fread(fid, 1, "uint32");
            obj.u64_UnixEpochTime_ms = fread(fid, 1, "uint64");
            obj.u32_CarrierFrequencyHz = fread(fid, 1, "uint32");
            obj.u32_RxBandwidthHz = fread(fid, 1, "uint32");
            obj.u32_TxBandwidthHz = fread(fid, 1, "uint32");
            obj.u16_RxGaindB = fread(fid, 1, "uint16");
            obj.u16_TxLevel = fread(fid, 1, "uint16");
            obj.u16_SIG_BaseCounts = fread(fid, 1, "uint16");
            obj.u16_SIG_BT = fread(fid, 1, "uint16");
        
        endfunction        

        function copy(obj, c)
          
            obj.ID = c.ID; 
            obj.u32_SerialNum    = c.u32_SerialNum;
            obj.u16_SubSerialNum = c.u16_SubSerialNum;
            obj.u16_TypeNum      = c.u16_TypeNum;
            obj.u16_SubTypeNum   = c.u16_SubTypeNum;
            obj.u16_BeamControl  = c.u16_BeamControl;
            obj.u16_EventID      = c.u16_EventID;
            obj.u16_PingID       = c.u16_PingID;
            obj.u16_BurstID      = c.u16_BurstID;
            obj.u16_CaptureDurationS = c.u16_CaptureDurationS;
            obj.u32_SleepDurationS = c.u32_SleepDurationS;
            obj.u16_PingFrequency_mHz = c.u16_PingFrequency_mHz;
            obj.u16_BeamIndex = c.u16_BeamIndex;
            obj.u32_PingCounter = c.u32_PingCounter;
            obj.u16_NoBeams = c.u16_NoBeams;
            obj.u32_EventStartTimeS = c.u32_EventStartTimeS;
            obj.u32_EventStartTime_us = c.u32_EventStartTime_us;
            obj.u64_UnixEpochTime_ms = c.u64_UnixEpochTime_ms;
            obj.u32_CarrierFrequencyHz = c.u32_CarrierFrequencyHz;
            obj.u32_RxBandwidthHz = c.u32_RxBandwidthHz;
            obj.u32_TxBandwidthHz = c.u32_TxBandwidthHz;
            obj.u16_RxGaindB = c.u16_RxGaindB;
            obj.u16_TxLevel = c.u16_TxLevel;
            obj.u16_SIG_BaseCounts = c.u16_SIG_BaseCounts;
            obj.u16_SIG_BT = c.u16_SIG_BT;

        endfunction        
        
    endmethods
endclassdef
