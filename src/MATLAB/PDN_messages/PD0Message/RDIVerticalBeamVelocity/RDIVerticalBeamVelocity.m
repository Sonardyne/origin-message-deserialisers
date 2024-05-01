%%% PD0 Vertical Beam Velocity

classdef RDIVerticalBeamVelocity < handle

    properties
    
        ID = 0x0A00
        VelocitiesMms = [0]
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id()
            id = 0x0A00;
        end      
    end      

    
    methods
        
        function hdr = RDIVerticalBeamVelocity()

        end
    
        function h = create(obj)
          
          h = RDIVerticalBeamVelocity();
          
        end
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + 2 * numel(obj.VelocitiesMms);
        
        end
        
        function deserialise(obj, fid, NumCells)
        
            obj.ID                     = fread(fid, 1, "uint16");
            obj.VelocitiesMms          = fread(fid, NumCells, "int16");        
        
        end 
      
        function copy(obj, c)

            obj.ID                     = c.ID;
            obj.VelocitiesMms          = c.VelocitiesMms;
        
        end      

    end
end
