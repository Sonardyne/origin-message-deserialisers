%%% PD0 Message %%%

classdef PD0Message < handle
        
    properties
        
      Fields = containers.Map('KeyType', 'uint64', 'ValueType', 'any')
        
    endproperties
        
        
    methods    
        function p = PD0Message()
            
        endfunction        
    
        function length_bytes = calc_length(obj)

            length_bytes = 0;
        
            vs = values(obj.Fields);
        
            for l = 1 : obj.Fields.Count
              
              code = vs{l}.get_id()
              length_bytes += c.Fields(code).calc_length();
              
            endfor

            length_bytes += 2; % for CRC
            
        endfunction
        
        function deserialise(obj, fid)
        
            % we have to reset the fields of the ensemble when it gets copied
            % to prevent the same reference being passed around
            obj.Fields = containers.Map('KeyType', 'uint64', 'ValueType', 'any');

            nBeams = 0;
            nCells = 0;
        
            ensemble_offset = ftell(fid);
        
            header_code = RDIPD0Header.get_id();
            Header = RDIPD0Header();
            Header.deserialise(fid);   
            %printf ("ensemblebytes=%d\n",Header.EnsembleBytes)         
            obj.Fields(RDIPD0Header.get_id()) = Header;
            
            fseek(fid, ensemble_offset);

            % decode each section
            for k = 1:numel(obj.Fields(header_code).Indices)
              
              % offset of section into buffer
              idx = obj.Fields(header_code).Indices(k);
              
              % find out what the section is
              fseek(fid, ensemble_offset + idx);
              code = fread(fid, 1, "uint16");
              
              % now decode that section, including the ID
              fseek(fid, ensemble_offset + idx);
              
              if code == RDIFixedLeader.get_id()
                
                s = RDIFixedLeader();
                s.deserialise(fid);            
                obj.Fields(code) = s;
                
                nBeams = obj.Fields(code).NumberOfBeams;  
                nCells = obj.Fields(code).NumberOfCells;       
                
              elseif code == RDIVariableLeader.get_id()
                
                s = RDIVariableLeader();                
                s.deserialise(fid);
                obj.Fields(code) = s;
                
              elseif code == RDIVelocity.get_id()
                
                s = RDIVelocity();
                s.deserialise(fid, nBeams, nCells);            
                obj.Fields(code) = s;
                
              elseif code == RDIXC.get_id()
                
                s = RDIXC();
                s.deserialise(fid, nBeams, nCells);            
                obj.Fields(code) = s;
                
              elseif code == RDIIntensity.get_id()
                
                s = RDIIntensity();
                s.deserialise(fid, nBeams, nCells);            
                obj.Fields(code) = s;
                
              elseif code == RDIPrctGood.get_id()
                
                s = RDIPrctGood();
                s.deserialise(fid, nBeams, nCells);            
                obj.Fields(code) = s;
                
              elseif code == PIESVariableLeader.get_id()

                s = PIESVariableLeader();
                s.deserialise(fid);            
                obj.Fields(code) = s;

              endif
              
              
            endfor
            
            fseek(fid, ensemble_offset + obj.Fields(0x7F7F).EnsembleBytes + 2);
        
        endfunction
        
        % method to make a copy of pd0 frame
        function copy(obj, c)
         
            % we have to reset the fields of the ensemble when it gets copied
            % to prevent the same reference being passed around
            obj.Fields = containers.Map('KeyType', 'uint64', 'ValueType', 'any');
            
            vs = values(c.Fields);
            
            for l = 1 : c.Fields.Count
              
              code = vs{l}.get_id()
              t = c.Fields(code).create()
              t.copy(c.Fields(code));
              obj.Fields(code) = t;
              
            endfor
                      
        endfunction
        
        % method to transform between velocity frames
        %
        % Octave does not support enum, so stick to the following convention:
        %
        % 0 = beam frame
        % 1 = instrument frame
        % 2 = vessel frame
        % 3 = earth frame
        function transform(obj, velocity_frame)
          
          % access metadata
          f_l = obj.Fields(RDIFixedLeader.get_id());          
          f_l.transform(velocity_frame);
          
          v_l = obj.Fields(RDIVariableLeader.get_id());
          
          % access velocity data
          obj.Fields(RDIVelocity.get_id()).transform(velocity_frame, ...
                                                     f_l.NumberOfBeams, ...
                                                     f_l.NumberOfCells, ...
                                                     f_l.BeamAngle, ...
                                                     v_l.RollCD * 0.01, ...
                                                     v_l.PitchCD * 0.01, ...
                                                     v_l.HeadingCD * 0.01);
              
        endfunction
                
        
        % Decode an entire file
        function ensembles = decode_file(obj, filename)

          fid = fopen(filename, 'r');

          fseek(fid, 0, SEEK_END);
          file_size_bytes = ftell(fid)
          fseek(fid, 0);
                    
          k = 1;
          do

              ensembles(k) = PD0Message();
              ensembles(k).deserialise(fid);
  
              k = k + 1;
              
          until (ftell(fid) >= file_size_bytes)
          
          fclose(fid);
          
        endfunction
        
        function data_arrays = ensembles_to_data_arrays(obj, ensembles)
          
            data_arrays = [];
              
            NumberOfBeams = ensembles(1).Fields(RDIFixedLeader.get_id()).NumberOfBeams;
            NumberOfCells = ensembles(1).Fields(RDIFixedLeader.get_id()).NumberOfCells;
            NumberOfPings = numel(ensembles);
            
            try
              
              printf("PIES present\n");
              pies_leader = ensembles(1).Fields(PIESVariableLeader.get_id())
              pies_present = true;
            
            catch
            
              printf("PIES not present\n");
              pies_present = false;
            
            end_try_catch
            
            % check to exit cleanly if the number of beams is zero
            if (NumberOfBeams == 0)
              
              printf("Error: Beams %d Cells %d; No beams!\n", NumberOfBeams, NumberOfCells);            
              
              return 
              
            endif
            
            printf("Number of beams: %d; Number of pings: %d; Number of cells: %d\n", NumberOfBeams, NumberOfPings, NumberOfCells);
            
            data_arrays.VelocitiesMms  = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.Intensities    = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.XC             = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.PrctGood       = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.Rollcdeg       = zeros(1, NumberOfPings);  
            data_arrays.Pitchcdeg      = zeros(1, NumberOfPings);   
            data_arrays.Headingcdeg    = zeros(1, NumberOfPings);
            data_arrays.Pings          = zeros(1, NumberOfPings);
            data_arrays.RxBWHz         = zeros(1, NumberOfPings);
            data_arrays.EventTime      = zeros(1, NumberOfPings);
            
            if pies_present == true
              
              data_arrays.PIES.SV1_ms       = zeros(1, NumberOfPings);
              data_arrays.PIES.SV2_ms       = zeros(1, NumberOfPings);
              data_arrays.PIES.TOF1_s       = zeros(1, NumberOfPings);
              data_arrays.PIES.TOF2_s       = zeros(1, NumberOfPings);
              data_arrays.PIES.Magnitude    = zeros(1, NumberOfPings);
              data_arrays.PIES.Pressure_kPa = zeros(1, NumberOfPings);
              data_arrays.PIES.Depth_m      = zeros(1, NumberOfPings);
              data_arrays.PIES.TimeNS       = zeros(1, NumberOfPings);
            
            endif
            
            for i = 1:NumberOfPings
             
              %data_arrays.temperatureC(i) = ensembles((i - 1) * NumberOfBeams + 1).u16_Tmp_mdegC;
              %data_arrays.pressurePa(i)   = ensembles((i - 1) * NumberOfBeams + 1).u32_PressurePa;
              %data_arrays.Rollcdeg(i)     = ensembles((i - 1) * NumberOfBeams + 1).n16_Roll_cdeg;
              %data_arrays.Pitchcdeg(i)    = ensembles((i - 1) * NumberOfBeams + 1).n16_Pitch_cdeg;
              %data_arrays.Headingcdeg(i)  = ensembles((i - 1) * NumberOfBeams + 1).n32_Heading_cdeg;
              %data_arrays.Pings(i)        = ensembles((i - 1) * NumberOfBeams + 1).u32_PingCounter;
              %data_arrays.RxBWHz(i)       = ensembles((i - 1) * NumberOfBeams + 1).u32_RxBandwidthHz;
              
              %ciT_EventStartTime          = ensembles((i - 1) * NumberOfBeams + 1).ciT_EventStartTime;
              %data_arrays.EventTime_s(i)  = ciT_EventStartTime.u32_seconds + ciT_EventStartTime.u32_microseconds .* 1e-6;
                         
              VelocitiesMms = ensembles(i).Fields(RDIVelocity.get_id()).VelocitiesMms;
              XC            = ensembles(i).Fields(RDIXC.get_id()).XC;
              Intensity     = ensembles(i).Fields(RDIIntensity.get_id()).Intensity;
              PrctGood      = ensembles(i).Fields(RDIPrctGood.get_id()).PrctGood;
                     
              VariableLeader = ensembles(i).Fields(RDIVariableLeader.get_id());                     
              data_arrays.EventTime(i) = VariableLeader.RTCTSDay  * 24 * 60 * 60 + ...
                                         VariableLeader.RTCTSHour * 60 * 60 + ...
                                         VariableLeader.RTCTSMin  * 60 + ...
                                         VariableLeader.RTCTSSec;
                     
              for k = 1:NumberOfBeams

                beam_idx = (1:NumberOfBeams:NumberOfBeams .* NumberOfCells) + (k - 1);
              
                data_arrays.VelocitiesMms(k, i, :) = VelocitiesMms(beam_idx);
                data_arrays.Intensities(k, i, :)   = Intensity(beam_idx);
                data_arrays.XC(k, i, :)            = XC(beam_idx);
                data_arrays.PrctGood(k, i, :)      = PrctGood(beam_idx);

              endfor
                
              if pies_present == true
                
                pies_leader = ensembles(i).Fields(PIESVariableLeader.get_id());
                
                data_arrays.PIES.SV1_ms(i)       = pies_leader.SV1_ms;
                data_arrays.PIES.SV2_ms(i)       = pies_leader.SV2_ms;
                data_arrays.PIES.TOF1_s(i)       = pies_leader.TOF1_s;
                data_arrays.PIES.TOF2_s(i)       = pies_leader.TOF2_s;
                data_arrays.PIES.Magnitude(i)    = pies_leader.Magnitude;
                data_arrays.PIES.Pressure_kPa(i) = pies_leader.Pressure_kPa;
                data_arrays.PIES.Depth_m(i)      = pies_leader.Depth_m; 
                data_arrays.PIES.TimeNS(i)       = pies_leader.TimeNS;
                
              endif
                
            endfor
          
        endfunction
        
    endmethods
endclassdef
