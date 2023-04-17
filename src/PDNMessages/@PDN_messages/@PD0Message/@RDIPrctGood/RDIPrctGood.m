%%% PD0 PrctGood
classdef RDIPrctGood < handle

    properties
    
        ID       = 0x0400
        PrctGood = [0]
        
    endproperties
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0400;
        endfunction      
    endmethods      


    methods
        
        function hdr = RDIPrctGood()
            
        endfunction
    
        function h = create(obj)
          
          h = RDIPrctGood();
          
        endfunction
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + numel(obj.PrctGood);
        
        endfunction
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID        = fread(fid, 1, "uint16");
            obj.PrctGood  = fread(fid, NumBeams * NumCells, "uint8");
        
      endfunction
      
        function copy(obj, c)

            obj.ID       = c.ID;
            obj.PrctGood = c.PrctGood;
        
        endfunction      
      
    endmethods
endclassdef
