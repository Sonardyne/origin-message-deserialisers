%%% Instrument time
classdef CInstrumentTime < handle

    properties
    
        u32_seconds = 0
        u32_microseconds = 0
        
    end
        

    methods
        
        function hdr = CInstrumentTime()
            
        end
    
        function length_bytes = calc_length(obj)
            length_bytes = 4 + 4;
        end
        
        function deserialise(obj, fid)
            
            obj.u32_seconds      = fread(fid, 1, "uint32");        
            obj.u32_microseconds = fread(fid, 1, "uint32");
        
        end    
    end
end
