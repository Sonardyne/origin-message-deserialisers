%%% ADCPGram
classdef SDSP_v1_0_ADCPGram

    properties
    
        ADCP55_SENSOR_MAX_RAWBINS = 2500;
        
        sCommon                  = SPingCommon();
        u16_XCStartSample        = 0;
        u16_XCEndSample          = 0;
        u16_Stride               = 0;
        an16_DPhases             = 0;
        au16_DPhasesErr          = 0;
        au16_AVIntensityMag      = 0;
        au16_AVXCMag             = 0;
        
    end
         
        
    methods
        
        function agram = SDSP_v1_0_ADCPGram()
            
        end
    
        function length_bytes = calc_length(obj)
            
            length_bytes = obj.sCommon.calc_length() + ...
                           2 + ... 
                           2 + ...  
                           2 + ...  
                           2 * obj.ADCP55_SENSOR_MAX_RAWBINS + ...  
                           2 * obj.ADCP55_SENSOR_MAX_RAWBINS + ...  
                           2 * obj.ADCP55_SENSOR_MAX_RAWBINS + ...  
                           2 * obj.ADCP55_SENSOR_MAX_RAWBINS;
    
        end

                        
        function obj = deserialise(obj, fid)
            header = fread(fid, 14, "uint8");
            
            obj.sCommon                 = obj.sCommon.deserialise(fid);           
            obj.u16_XCStartSample       = fread(fid, 1, "uint16");
            obj.u16_XCEndSample         = fread(fid, 1, "uint16");
            obj.u16_Stride              = fread(fid, 1, "uint16");
            obj.an16_DPhases            = fread(fid, obj.ADCP55_SENSOR_MAX_RAWBINS, "int16");
            obj.au16_DPhasesErr         = fread(fid, obj.ADCP55_SENSOR_MAX_RAWBINS, "uint16");
            obj.au16_AVIntensityMag     = fread(fid, obj.ADCP55_SENSOR_MAX_RAWBINS, "uint16");
            obj.au16_AVXCMag            = fread(fid, obj.ADCP55_SENSOR_MAX_RAWBINS, "uint16");
             
        end 
                    
        % Decode an entire file
        function output_agrams = decode_file(obj, filename, Ndecimation) %MATLAB doesn't allow default values

            % open agram file at the beginning
            fid = fopen(filename, 'r');

            % find out file size in bytes
            fseek(fid, 0, 'eof');      % this line jumps to the end of the file
            file_size_bytes = ftell(fid); % this line tells us how far through reading the file we are
            fseek(fid, 0,'bof');                % this line jumps back to the beginning of the file so we can read in all the agrams

             % decode 1st agram to figure out # beams
            agram = SDSP_v1_0_ADCPGram;
            agram.deserialise(fid);
            
            NumberOfBeams = agram.sCommon.u16_NoBeams;
            
            fseek(fid, 0, 'bof');
  
            % calculate # agrams in file
            AgramSizeBytes = 14 + agram.calc_length();
            
            NumberOfAgrams = file_size_bytes ./ AgramSizeBytes;            
          
            NumberOfPings = NumberOfAgrams / NumberOfBeams;            
                        
            fprintf("Decoding %s of size %d bytes; agram size: %d bytes\n", filename, file_size_bytes, AgramSizeBytes);
            
            agram_dummy = agram;
            
            parse_started = 0;
            k = 1;
            while ftell(fid) < file_size_bytes

                % decimate if requested
                if parse_started == 1 && mod(numel(output_agrams), NumberOfBeams) == 0 
                  
                  fseek(fid, Ndecimation * NumberOfBeams * AgramSizeBytes, SEEK_CUR);
                  
                end
            
                agram = SDSP_v1_0_ADCPGram;
                agram = agram.deserialise(fid);

                if parse_started == 0 && agram.sCommon.u16_BeamIndex ~= 0 % not start condition
                  
                  fprintf ('Ping Index %d; Beam Index %d ignored at start of file\n', agram.sCommon.u32_PingCounter, agram.sCommon.u16_BeamIndex);
                  
                elseif parse_started == 0 && agram.sCommon.u16_BeamIndex == 0 % start condition
                  
                  parse_started  = 1;
                  
                  output_agrams(k) = agram;                  
                  k = k + 1;
                  
                else % parsing has started

                  %if mod(agram.u32_PingCounter, Ndecimation) == 0
                
                    if agram.sCommon.u16_BeamIndex ~= mod(output_agrams(k - 1).sCommon.u16_BeamIndex + 1, agram.sCommon.u16_NoBeams) % are we on the next beam?
                    
                      np  =  agram.sCommon.u16_NoBeams * (agram.sCommon.u32_PingCounter - output_agrams(k - 1).sCommon.u32_PingCounter + 1);
                      np = np - (output_agrams(k - 1).sCommon.u16_BeamIndex + 1);
                      np = np - (agram.sCommon.u16_NoBeams - agram.sCommon.u16_BeamIndex);
                     
                      for p = 1 : np
                        
                        agram_dummy.sCommon.u16_TxLevel     = output_agrams(k - 1).sCommon.u16_TxLevel;
                        agram_dummy.sCommon.u16_RxGaindB    = output_agrams(k - 1).sCommon.u16_RxGaindB;
                        agram_dummy.sCommon.u16_BeamIndex   = output_agrams(k - 1).sCommon.u16_BeamIndex + 1;
                        agram_dummy.sCommon.u32_PingCounter = output_agrams(k - 1).sCommon.u32_PingCounter;
                        if agram_dummy.sCommon.u16_BeamIndex == agram.sCommon.u16_NoBeams
                          
                          agram_dummy.sCommon.u16_TxLevel     = agram.sCommon.u16_TxLevel;
                          agram_dummy.sCommon.u16_RxGaindB    = agram.sCommon.u16_RxGaindB;
                          agram_dummy.sCommon.u16_BeamIndex   = 0;
                          agram_dummy.sCommon.u32_PingCounter = output_agrams(k - 1).sCommon.u32_PingCounter + 1;
                          
                        end
                        
                        output_agrams(k) = agram_dummy;
                        output_agrams(k).sCommon = agram_dummy.sCommon;                   
                        k = k + 1;                                                                  
                        
                      end
                    
                    end
                  %endif
                  
                  output_agrams(k) = agram; 
                  output_agrams(k).sCommon = agram.sCommon;   

                  k = k + 1;                    
                                    
                end  
  
            end
                 
            fprintf("output_agrams size: %d\n", numel(output_agrams));   
            
            if isinteger(numel(output_agrams) / NumberOfBeams) == false
              
              fprintf("Additional unwanted agrams\n")
              
            end
          
            fclose(fid);             
            
            end  
                    
        function data_arrays = ensembles_to_data_arrays(obj, ensembles)

            fprintf("--- AGRAM ensembles_to_data_arrays ---\n")

            NumberOfBeams = ensembles(1).sCommon.u16_NoBeams
            NumberOfCells = ensembles(1).ADCP55_SENSOR_MAX_RAWBINS
            
            % this code just checks to exit cleanly if the number of beams is zero
            if (NumberOfBeams == 0)
              
              fprintf("Error\n")
              fprintf("Beams %d Cells %d\n", NumberOfBeams, NumberOfCells);            
              fprintf("No beams!\n")
              
              data_arrays = 0
              
              return 
              
            end
                        
            NumberOfPings = floor(numel(ensembles) / NumberOfBeams);
            
            fprintf("--- ---\n");
            fprintf("%d %d %d\n", NumberOfBeams, NumberOfPings, NumberOfCells);
            
            data_arrays.Dphases        = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.DPhasesErr     = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.AVIntensityMag = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.AVXCMag        = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.temperatureC   = zeros(1, NumberOfPings);
            data_arrays.pressurePa     = zeros(1, NumberOfPings);
            data_arrays.Rollcdeg       = zeros(1, NumberOfPings);  
	          data_arrays.Pitchcdeg      = zeros(1, NumberOfPings);   
	          data_arrays.Headingcdeg    = zeros(1, NumberOfPings);
            data_arrays.Pings          = zeros(1, NumberOfPings);
            data_arrays.RxBWHz         = zeros(1, NumberOfPings);
            data_arrays.Stride         = zeros(1, NumberOfPings);
            data_arrays.SIGBT          = zeros(1, NumberOfPings);
            data_arrays.SIGBC          = zeros(1, NumberOfPings);
            data_arrays.TxG            = zeros(1, NumberOfPings);
            data_arrays.RxGdB          = zeros(1, NumberOfPings);
            data_arrays.TxBWHz         = zeros(1, NumberOfPings);
            data_arrays.CfHz           = zeros(1, NumberOfPings);
           
            
            for i = 1:NumberOfPings
              
              data_arrays.temperatureC(i) = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n32_Temp_mdegC;
              data_arrays.pressurePa(i)   = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_PressurePa;
              data_arrays.Rollcdeg(i)     = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n16_Roll_cdeg;
              data_arrays.Pitchcdeg(i)    = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n16_Pitch_cdeg;
              data_arrays.Headingcdeg(i)  = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n32_Heading_cdeg;
              data_arrays.Pings(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_PingCounter;
              data_arrays.RxBWHz(i)       = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_RxBandwidthHz;
              data_arrays.Stride(i)       = ensembles((i - 1) * NumberOfBeams + 1).u16_Stride;
              data_arrays.SIGBT(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_SIG_BT;
              data_arrays.SIGBC(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_SIG_BaseCounts;
              data_arrays.TxG(i)          = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_TxLevel;
              data_arrays.RxGdB(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_RxGaindB;
              data_arrays.TxBWHz(i)       = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_TxBandwidthHz;
              data_arrays.CfHz(i)         = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_CarrierFrequencyHz;
              
              for k = 1:NumberOfBeams

                data_arrays.Dphases(k, i, :)        = ensembles((i - 1) * NumberOfBeams + k).an16_DPhases; 
                data_arrays.DPhasesErr(k, i, :)     = ensembles((i - 1) * NumberOfBeams + k).au16_DPhasesErr; 
                data_arrays.AVIntensityMag(k, i, :) = ensembles((i - 1) * NumberOfBeams + k).au16_AVIntensityMag; 
                data_arrays.AVXCMag(k, i, :)        = ensembles((i - 1) * NumberOfBeams + k).au16_AVXCMag; 

              end                
            end
        end
    end
end


