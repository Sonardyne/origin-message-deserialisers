%%% PD0 Vertical Beam PrctGood
classdef RDIVerticalBeamPrctGood < handle

    properties
    
        ID       = 0x0D00
        PrctGood = [0]
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0D00;
        end      
    end      


    methods
        
        function hdr = RDIVerticalBeamPrctGood()
            
        end
    
        function h = create(obj)
          
          h = RDIVerticalBeamPrctGood();
          
        end
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + numel(obj.PrctGood);
        
        end
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID        = fread(fid, 1, "uint16");
            obj.PrctGood  = fread(fid, NumCells, "uint8");
        
        end
      
        function copy(obj, c)

            obj.ID       = c.ID;
            obj.PrctGood = c.PrctGood;
        
        end      
      
    end
end
