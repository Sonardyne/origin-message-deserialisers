%%% PD0 XC
classdef RDIXC < handle

    properties
    
        ID = 0x0200;
        XC = [0]
        
    endproperties
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0200;
        endfunction      
    endmethods      


    methods
        
        function hdr = RDIXC()
            
        endfunction
    
        function h = create(obj)
          
          h = RDIXC();
          
        endfunction
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + numel(obj.XC);
        
        endfunction
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID = fread(fid, 1, "uint16");
            obj.XC = fread(fid, NumBeams * NumCells, "uint8");
        
        endfunction        
        
        function copy(obj, c)

            obj.ID = c.ID;
            obj.XC = c.XC;
        
        endfunction      
      
    endmethods
endclassdef
