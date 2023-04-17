classdef rotation < handle

    properties
            
    end
        
    % static methods
    methods (Static = true)

      function vp = x(v, angle_deg)
            
        Rx = [1    0                0;
              0   +cosd(angle_deg) +sind(angle_deg);
              0   -sind(angle_deg) +cosd(angle_deg)];
            
        vp = Rx * v;
        
      end      

      function vp = y(v, angle_deg)
            
        Ry = [+cosd(angle_deg) 0 -sind(angle_deg);
               0               1  0;
              +sind(angle_deg) 0 +cosd(angle_deg)];
            
        vp = Ry * v;
        
      end   
        
      function vp = z(v, angle_deg)
            
        Rz = [+cosd(angle_deg) +sind(angle_deg) 0;
              -sind(angle_deg) +cosd(angle_deg) 0;
               0                0               1];
            
        vp = Rz * v;
        
      end    

    end      

end