%%% PD0 Velocity

classdef RDIVelocity < handle

    properties
    
        ID = 0x0100
        VelocitiesMms = [0]
        % always keep a copy of beam frame velocities because once you transform
        % to a different frame, the information is lost
        BeamFrameVelocitiesMms = [0] 
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id()
            id = 0x0100;
        end      
    end      

    
    methods
        
        function hdr = RDIVelocity()
            
            % wrinkle - assumes execution path of calling function
            addpath("../../src/MATLAB/maths");

        end
    
        function h = create(obj)
          
          h = RDIVelocity();
          
        end
            
        function length_bytes = calc_length(obj)
            
            length_bytes = 2 + 2 * numel(obj.VelocitiesMms);
        
        end
        
        function deserialise(obj, fid, NumBeams, NumCells)
        
            obj.ID                     = fread(fid, 1, "uint16");
            obj.VelocitiesMms          = fread(fid, NumBeams * NumCells, "int16");        
            obj.BeamFrameVelocitiesMms = obj.VelocitiesMms;
        
        end 
      
        function copy(obj, c)

            obj.ID                     = c.ID;
            obj.VelocitiesMms          = c.VelocitiesMms;
            obj.BeamFrameVelocitiesMms = c.BeamFrameVelocitiesMms;
        
        end      
   
        % method to transform between velocity frames
        %
        % Octave does not support enum, so stick to the following convention:
        %
        % 0 = beam frame
        % 1 = instrument frame
        % 2 = vessel frame
        % 3 = earth frame
        function obj = transform(obj, ...
                           velocity_frame, ...
                           n_beams, ...
                           n_cells, ...
                           janus_angle_deg, ...
                           roll_deg, ...
                           pitch_deg, ...
                           heading_deg)

          if velocity_frame == 0 || velocity_frame > 3 % beam frame or undefined
            
            obj.VelocitiesMms = obj.BeamFrameVelocitiesMms;
        
          end
      
          if velocity_frame > 0 % instrument transformation

            % indices
            x_index = 1 : n_beams : n_beams * n_cells;
            y_index = 2 : n_beams : n_beams * n_cells;
            z_index = 3 : n_beams : n_beams * n_cells;
            e_index = 4 : n_beams : n_beams * n_cells;

            % X-component
            obj.VelocitiesMms(x_index) = obj.BeamFrameVelocitiesMms(x_index) - obj.BeamFrameVelocitiesMms(y_index);
            obj.VelocitiesMms(x_index) = obj.VelocitiesMms(x_index) ./ (2 .* sind(janus_angle_deg));
          
            % Y-component
            obj.VelocitiesMms(y_index) = obj.BeamFrameVelocitiesMms(e_index) - obj.BeamFrameVelocitiesMms(z_index);
            obj.VelocitiesMms(y_index) = obj.VelocitiesMms(y_index) ./ (2 .* sind(janus_angle_deg));
            
            % Z-component
            obj.VelocitiesMms(z_index) = obj.BeamFrameVelocitiesMms(x_index) + ...
                                         obj.BeamFrameVelocitiesMms(y_index) + ...
                                         obj.BeamFrameVelocitiesMms(z_index) + ...
                                         obj.BeamFrameVelocitiesMms(e_index);

            obj.VelocitiesMms(z_index) = obj.VelocitiesMms(z_index) ./ (4 .* cosd(janus_angle_deg));

            % error velocity
            obj.VelocitiesMms(e_index) = obj.BeamFrameVelocitiesMms(x_index) + ...
                                         obj.BeamFrameVelocitiesMms(y_index) - ...
                                         obj.BeamFrameVelocitiesMms(z_index) - ...
                                         obj.BeamFrameVelocitiesMms(e_index);

            obj.VelocitiesMms(e_index) = obj.VelocitiesMms(e_index) ./ sqrt(2);            
  
            if n_beams == 5

              v_index = 5 : n_beams : n_beams * n_cells;
              obj.VelocitiesMms(v_index) = obj.BeamFrameVelocitiesMms(v_index);

            end

          end  
        
          if velocity_frame > 1 % vessel frame
            
            % to be implemented
            
          end
            
          if velocity_frame == 3 % earth frame
  
            % store a local copy of the vessel-frame velocities
            vx = obj.VelocitiesMms(x_index);
            vy = obj.VelocitiesMms(y_index);
            vz = obj.VelocitiesMms(z_index);
            
            v_mms = obj.earth_rotation(vx, vy, vz, roll_deg, pitch_deg, heading_deg);
                        
            % convert back to PD0 storage format
            obj.VelocitiesMms(x_index) = v_mms(1, :);
            obj.VelocitiesMms(y_index) = v_mms(2, :);
            obj.VelocitiesMms(z_index) = v_mms(3, :);

            % repeat but using 5th beam
            if n_beams == 5

              % perform identical calculation but with 5th beam as the vertical component
              vz = obj.VelocitiesMms(v_index);

              v_mms = obj.earth_rotation(vx, vy, vz, roll_deg, pitch_deg, heading_deg);

              obj.VelocitiesMms(v_index) = v_mms(3, :);

            end

          end
        
        end

        function v_mms = earth_rotation(obj, vx, vy, vz, roll_deg, pitch_deg, heading_deg)

          v_mms = zeros(3, numel(vx));
          v_mms(1, :) = vx;
          v_mms(2, :) = vy;
          v_mms(3, :) = vz;

          r = rotation();

          % rotate by -roll about the Y axis - add 180 deg to go from
          % towards transducer face to up
          v_mms = r.y(v_mms, -roll_deg + 180);

          % rotate by -pitch about the X axis
          v_mms = r.x(v_mms, -pitch_deg);

          % rotate by +heading about the Z axis
          v_mms = r.z(v_mms, +heading_deg);

        end

    end
end
