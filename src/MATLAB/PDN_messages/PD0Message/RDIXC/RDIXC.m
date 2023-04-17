%%% PD0 XC
classdef RDIXC < handle

    properties
    
        ID = 0x0200;
        XC = [0]
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0200;
        end      
    end      


    methods
        
        function hdr = RDIXC()
            
        end
    
        function h = create(obj)
          
          h = RDIXC();
          
        end
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + numel(obj.XC);
        
        end
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID = fread(fid, 1, "uint16");
            obj.XC = fread(fid, NumBeams * NumCells, "uint8");
        
        end        
        
        function copy(obj, c)

            obj.ID = c.ID;
            obj.XC = c.XC;
        
        end      
      
    end
end
