%%% PD0 Header
classdef RDIPD0Header < handle

    properties
    
        HeaderId = 0x7F
        DataSource = 0x7F
        EnsembleBytes = 0
        Spare = 0
        NumberDataTypes = 0
        Indices = 0
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x7F7F;
        end      
    end      
        
    % normal methods
    methods
        
        function hdr = RDIPD0Header()
            
        end
    
        function h = create(obj)
          
          h = RDIPD0Header();
          
        end
            
        function length_bytes = calc_length(obj)
            length_bytes = 6 + 2 * numel(obj.Indices);
        end
        
        function deserialise(obj, fid)
        
            obj.HeaderId        = fread(fid, 1, "uint8");        
            obj.DataSource      = fread(fid, 1, "uint8");
            obj.EnsembleBytes   = fread(fid, 1, "uint16");
            obj.Spare           = fread(fid, 1, "uint8");        
            obj.NumberDataTypes = fread(fid, 1, "uint8");        

            obj.Indices         = fread(fid, obj.NumberDataTypes, "uint16");
        
        end  
      
        function copy(obj, c)
          
            c.NumberDataTypes
          
            obj.HeaderId        = c.HeaderId;        
            obj.DataSource      = c.DataSource;
            obj.EnsembleBytes   = c.EnsembleBytes;
            obj.Spare           = c.Spare; 
            obj.NumberDataTypes = c.NumberDataTypes;
          
            obj.Indices         = c.Indices;
          
        end        
      
    end
end
