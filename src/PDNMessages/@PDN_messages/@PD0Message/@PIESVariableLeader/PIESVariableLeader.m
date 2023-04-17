%%% PIES Variable Leader
classdef PIESVariableLeader < handle

    properties
    
        ID               = 0
        SV1_ms           = 0
        SV2_ms           = 0
        TOF1_s           = 0
        TOF2_s           = 0
        Magnitude        = 0
        PeakIndex        = 0
        RMLEIndex        = 0
        RMLERange        = 0
        PretiggerRMS     = 0
        HalfLife_s       = 0
        R2               = 0
        Pressure_kPa     = 0
        Depth_m          = 0
        Zoffset_m        = 0
        Latitude_DD      = 0
        Patmos_kPa       = 0
        TimeNS           = 0
        
    endproperties
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x62ca;
        endfunction      
    endmethods      

    
    methods
        
        function hdr = PIESVariableLeader()
            
        endfunction
    
        function h = create(obj)
          
          h = PIESVariableLeader();
          
        endfunction
            
        function length_bytes = calc_length(obj)
            length_bytes = 74;
        endfunction
        
        function deserialise(obj, fid)
        
            obj.ID               = fread(fid, 1, "uint16");        
            obj.SV1_ms           = fread(fid, 1, "float");
            obj.SV2_ms           = fread(fid, 1, "float");
            obj.TOF1_s           = fread(fid, 1, "float");
            obj.TOF2_s           = fread(fid, 1, "float");
            obj.Magnitude        = fread(fid, 1, "float");
            obj.PeakIndex        = fread(fid, 1, "uint32");
            obj.RMLEIndex        = fread(fid, 1, "uint32");
            obj.RMLERange        = fread(fid, 1, "float");
            obj.PretiggerRMS     = fread(fid, 1, "float");
            obj.HalfLife_s       = fread(fid, 1, "float");
            obj.R2               = fread(fid, 1, "float");
            obj.Pressure_kPa     = fread(fid, 1, "float");
            obj.Depth_m          = fread(fid, 1, "float");
            obj.Zoffset_m        = fread(fid, 1, "float");
            obj.Latitude_DD      = fread(fid, 1, "float");
            obj.Patmos_kPa       = fread(fid, 1, "float");
            obj.TimeNS           = fread(fid, 1, "uint64");
        
        endfunction        

        function copy(obj, c)
          
            obj.ID = c.ID; 
            obj.SV1_ms = c.SV1_ms;
            obj.SV2_ms = c.SV2_ms;
            obj.TOF1_s = c.TOF1_s;
            obj.TOF2_s = c.TOF2_s;
            obj.Magnitude = c.Magnitude;
            obj.PeakIndex = c.PeakIndex;
            obj.RMLEIndex = c.RMLEIndex;
            obj.RMLERange = c.RMLERange;
            obj.PretiggerRMS = c.PretiggerRMS;
            obj.HalfLife_s = c.HalfLife_s;
            obj.R2 = c.R2;
            obj.Pressure_kPa = c.Pressure_kPa;
            obj.Depth_m = c.Depth_m;
            obj.Zoffset_m = c.Zoffset_m;
            obj.Latitude_DD = c.Latitude_DD;
            obj.Patmos_kPa = c.Patmos_kPa;
            obj.TimeNS = c.TimeNs;

        endfunction        
        
    endmethods
endclassdef
