%%% PD0 Variable Leader
classdef RDIVariableLeader < handle

    properties
    
        ID               = 0
        EnsembleNumber   = 0
        RTCTSYear        = 0
        RTCTSMonth       = 0
        RTCTSDay         = 0
        RTCTSHour        = 0
        RTCTSMin         = 0
        RTCTSSec         = 0
        RTCTSHSec        = 0
        EnsembleMSB      = 0
        BITResult        = 0
        SpeedOfSoundMs   = 0
        DepthOfTxdrDm    = 0
        HeadingCD        = 0
        PitchCD          = 0
        RollCD           = 0
        SalinityPPT      = 0
        TempCD           = 0
        MptMins          = 0
        MptSecs          = 0
        MptHsecs         = 0
        HeadingSdev      = 0
        PitchSdev        = 0
        RollSdev         = 0
        AdcChan0         = 0
        AdcChan1         = 0
        AdcChan2         = 0
        AdcChan3         = 0
        AdcChan4         = 0
        AdcChan5         = 0
        AdcChan6         = 0
        AdcChan7         = 0
        StatusWord       = 0
        PressureDP       = 0
        PressureVarDP    = 0
        Spare            = 0
        RTCCentury       = 0
        RTCYear          = 0
        RTCMonth         = 0
        RTCDay           = 0
        RTCHour          = 0
        RTCMin           = 0
        RTCSec           = 0
        RTCHSec          = 0
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0080;
        end      
    end      

    
    methods
        
        function hdr = RDIVariableLeader()
            
        end
    
        function h = create(obj)
          
          h = RDIVariableLeader();
          
        end
            
        function length_bytes = calc_length(obj)
            length_bytes = 65;
        end
        
        function deserialise(obj, fid)
        
            obj.ID               = fread(fid, 1, "uint16");        
            obj.EnsembleNumber   = fread(fid, 1, "uint16");
            obj.RTCTSYear        = fread(fid, 1, "uint8");
            obj.RTCTSMonth       = fread(fid, 1, "uint8");
            obj.RTCTSDay         = fread(fid, 1, "uint8");
            obj.RTCTSHour        = fread(fid, 1, "uint8");
            obj.RTCTSMin         = fread(fid, 1, "uint8");
            obj.RTCTSSec         = fread(fid, 1, "uint8");
            obj.RTCTSHSec        = fread(fid, 1, "uint8");
            obj.EnsembleMSB      = fread(fid, 1, "uint8");
            obj.BITResult        = fread(fid, 1, "uint16");
            obj.SpeedOfSoundMs   = fread(fid, 1, "uint16");
            obj.DepthOfTxdrDm    = fread(fid, 1, "uint16");
            obj.HeadingCD        = fread(fid, 1, "uint16");
            obj.PitchCD          = fread(fid, 1, "int16");
            obj.RollCD           = fread(fid, 1, "int16");
            obj.SalinityPPT      = fread(fid, 1, "uint16");
            obj.TempCD           = fread(fid, 1, "uint16");
            obj.MptMins          = fread(fid, 1, "uint8");
            obj.MptSecs          = fread(fid, 1, "uint8");
            obj.MptHsecs         = fread(fid, 1, "uint8");
            obj.HeadingSdev      = fread(fid, 1, "uint8");
            obj.PitchSdev        = fread(fid, 1, "uint8");
            obj.RollSdev         = fread(fid, 1, "uint8");
            obj.AdcChan0         = fread(fid, 1, "uint8");
            obj.AdcChan1         = fread(fid, 1, "uint8");
            obj.AdcChan2         = fread(fid, 1, "uint8");
            obj.AdcChan3         = fread(fid, 1, "uint8");
            obj.AdcChan4         = fread(fid, 1, "uint8");
            obj.AdcChan5         = fread(fid, 1, "uint8");
            obj.AdcChan6         = fread(fid, 1, "uint8");
            obj.AdcChan7         = fread(fid, 1, "uint8");
            obj.StatusWord       = fread(fid, 1, "uint32");        
            obj.PressureDP       = fread(fid, 1, "uint32");        
            obj.PressureVarDP    = fread(fid, 1, "uint32");        
            obj.Spare            = fread(fid, 3, "uint8");
            obj.RTCCentury       = fread(fid, 1, "uint8");
            obj.RTCYear          = fread(fid, 1, "uint8");
            obj.RTCMonth         = fread(fid, 1, "uint8");
            obj.RTCDay           = fread(fid, 1, "uint8");
            obj.RTCHour          = fread(fid, 1, "uint8");
            obj.RTCMin           = fread(fid, 1, "uint8");
            obj.RTCSec           = fread(fid, 1, "uint8");
            obj.RTCHSec          = fread(fid, 1, "uint8");
        
        end        

        function copy(obj, c)
          
            obj.ID = c.ID; 
            obj.EnsembleNumber = c.EnsembleNumber;
            obj.RTCTSYear = c.RTCTSYear;
            obj.RTCTSMonth = c.RTCTSMonth;
            obj.RTCTSDay = c.RTCTSDay;
            obj.RTCTSHour = c.RTCTSHour;
            obj.RTCTSMin = c.RTCTSMin;
            obj.RTCTSSec = c.RTCTSSec;
            obj.RTCTSHSec = c.RTCTSHSec;
            obj.EnsembleMSB = c.EnsembleMSB;
            obj.BITResult = c.BITResult;
            obj.SpeedOfSoundMs = c.SpeedOfSoundMs;
            obj.DepthOfTxdrDm = c.DepthOfTxdrDm;
            obj.HeadingCD = c.HeadingCD;
            obj.PitchCD = c.PitchCD;
            obj.RollCD = c.RollCD;
            obj.SalinityPPT = c.SalinityPPT;
            obj.TempCD = c.TempCD;
            obj.MptMins = c.MptMins;
            obj.MptSecs = c.MptSecs;
            obj.MptHsecs = c.MptHsecs;
            obj.HeadingSdev = c.HeadingSdev;
            obj.PitchSdev = c.PitchSdev;
            obj.RollSdev = c.RollSdev;
            obj.AdcChan1 = c.AdcChan1;
            obj.AdcChan2 = c.AdcChan2;
            obj.AdcChan3 = c.AdcChan3;
            obj.AdcChan4 = c.AdcChan4;
            obj.AdcChan5 = c.AdcChan5;
            obj.AdcChan6 = c.AdcChan6;
            obj.AdcChan7 = c.AdcChan7;
            obj.StatusWord = c.StatusWord;       
            obj.PressureDP = c.PressureDP;      
            obj.PressureVarDP = c.PressureVarDP;   
            obj.Spare = c.Spare;
            obj.RTCCentury = c.RTCCentury;
            obj.RTCYear = c.RTCYear;
            obj.RTCMonth = c.RTCMonth;
            obj.RTCDay = c.RTCDay;
            obj.RTCHour = c.RTCHour;
            obj.RTCMin = c.RTCMin;
            obj.RTCSec = c.RTCSec;
            obj.RTCHSec = c.RTCHSec;
          
        end        
        
    end
end
