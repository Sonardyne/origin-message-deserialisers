%%% Instrument time
classdef CInstrumentTime

    properties
    
        u32_seconds = 0
        u32_microseconds = 0
        
    endproperties
        

    methods
        
        function hdr = CInstrumentTime()
            
        endfunction
    
        function length_bytes = calc_length(obj)
            length_bytes = 4 + 4;
        endfunction
        
        function deserialise(obj, fid)
            
            obj.u32_seconds      = fread(fid, 1, "uint32");        
            obj.u32_microseconds = fread(fid, 1, "uint32");
        
        endfunction        
    endmethods
endclassdef
