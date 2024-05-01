%%% PD0 Vertical Beam XC
classdef RDIVerticalBeamXC < handle

    properties
    
        ID = 0x0B00;
        XC = [0]
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0B00;
        end      
    end      


    methods
        
        function hdr = RDIVerticalBeamXC()
            
        end
    
        function h = create(obj)
          
          h = RDIVerticalBeamXC();
          
        end
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + numel(obj.XC);
        
        end
        
        function deserialise(obj, fid, NumCells)
        
            obj.ID = fread(fid, 1, "uint16");
            obj.XC = fread(fid, NumCells, "uint8");
        
        end        
        
        function copy(obj, c)

            obj.ID = c.ID;
            obj.XC = c.XC;
        
        end      
      
    end
end
