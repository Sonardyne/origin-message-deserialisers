classdef rotation < handle

    properties
            
    endproperties
        
    % static methods
    methods (Static = true)

      function vp = x(v, angle_deg)
            
        Rx = [1    0                0;
              0   +cosd(angle_deg) +sind(angle_deg);
              0   -sind(angle_deg) +cosd(angle_deg)];
            
        vp = Rx * v;
        
      endfunction      

      function vp = y(v, angle_deg)
            
        Ry = [+cosd(angle_deg) 0 -sind(angle_deg);
               0               1  0;
              +sind(angle_deg) 0 +cosd(angle_deg)];
            
        vp = Ry * v;
        
      endfunction      
        
      function vp = z(v, angle_deg)
            
        Rz = [+cosd(angle_deg) +sind(angle_deg) 0;
              -sind(angle_deg) +cosd(angle_deg) 0;
               0                0               1];
            
        vp = Rz * v;
        
      endfunction      

    endmethods      

endclassdef