%%% PD0 Intensity
classdef RDIIntensity < handle

    properties
    
        ID        = 0x0300
        Intensity = [0]
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0300;
        end      
    end      


    methods
        
        function hdr = RDIIntensity()
            
        end
    
        function h = create(obj)
          
          h = RDIIntensity();
          
        end
            
        function length_bytes = calc_length(obj)
          
            length_bytes = 2 + numel(obj.Intensity);
        
        end
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID        = fread(fid, 1, "uint16");
            obj.Intensity = fread(fid, NumBeams * NumCells, "uint8");
        
        end        

        function copy(obj, c)

            obj.ID        = c.ID;
            obj.Intensity = c.Intensity;
        
        end      
      
    end
end
