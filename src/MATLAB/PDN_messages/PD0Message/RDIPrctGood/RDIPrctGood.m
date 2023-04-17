%%% PD0 PrctGood
classdef RDIPrctGood < handle

    properties
    
        ID       = 0x0400
        PrctGood = [0]
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0400;
        end      
    end      


    methods
        
        function hdr = RDIPrctGood()
            
        end
    
        function h = create(obj)
          
          h = RDIPrctGood();
          
        end
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + numel(obj.PrctGood);
        
        end
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID        = fread(fid, 1, "uint16");
            obj.PrctGood  = fread(fid, NumBeams * NumCells, "uint8");
        
        end
      
        function copy(obj, c)

            obj.ID       = c.ID;
            obj.PrctGood = c.PrctGood;
        
        end      
      
    end
end
