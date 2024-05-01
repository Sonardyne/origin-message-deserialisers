%%% BDGram

classdef SDSP_v1_0_BDGram < handle

    properties
    
        ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES = 5000;
        
        sCommon                  = SPingCommon();
        eProcessingStatus        = 0;
        u16_SampleBitDepth       = 0;        
        u16_StartBin             = 0;
        u16_Samples              = 0;
        an16_IPingData           = 0;
        an16_QPingData           = 0;
        
    end
         
    methods
        
        function bgram = SDSP_v1_0_BDGram()
            
        end
    
        function length_bytes = calc_length(obj)
            
            length_bytes = obj.sCommon.calc_length() + ...
			                     2 + ... 
                           2 + ...  
                           2 + ...  
                           2 + ...
                           2 * obj.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES + ...  
                           2 * obj.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES;
    
        end

                        
        function obj = deserialise(obj, fid)
            header = fread(fid, 14, "uint8");
                
            obj.sCommon                 = obj.sCommon.deserialise(fid);  
            obj.eProcessingStatus       = fread(fid, 1, "uint16");
            obj.u16_SampleBitDepth      = fread(fid, 1, "uint16");
            obj.u16_StartBin            = fread(fid, 1, "uint16");
            obj.u16_Samples             = fread(fid, 1, "uint16");
            obj.an16_IPingData          = fread(fid, obj.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES, "int16");
            obj.an16_QPingData          = fread(fid, obj.ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES, "int16");
            
        end 
        
        % Decode an entire file
            function output_bgrams = decode_file(obj, filename)

            % open bgram file at the beginning
            fid = fopen(filename, 'r');

            % find out file size in bytes
            fseek(fid, 0, 'eof');      % this line jumps to the end of the file
            file_size_bytes = ftell(fid); % this line tells us how far through reading the file we are
            fseek(fid, 0,'bof');                % this line jumps back to the beginning of the file so we can read in all the agrams

             % decode 1st bgram to figure out # beams
            bgram = SDSP_v1_0_BDGram;
            bgram.deserialise(fid);
            NumberOfBeams = bgram.sCommon.u16_NoBeams;
            
            fseek(fid, 0,'bof');
  
            % calculate # agrams in file
            BgramSizeBytes = 14 + bgram.calc_length();
            
            NumberOfBgrams = file_size_bytes ./ BgramSizeBytes        
          
            NumberOfPings = NumberOfBgrams / NumberOfBeams;            
                        
            fprintf("Decoding %s of size %d bytes; bgram size: %d bytes\n", filename, file_size_bytes, BgramSizeBytes);
            
            bgram_dummy = SDSP_v1_0_BDGram;
            
            parse_started = 0;
            k = 1;
            while (ftell(fid) < file_size_bytes)
                bgram = SDSP_v1_0_BDGram;
                bgram.deserialise(fid);

                if parse_started == 0 && bgram.sCommon.u16_BeamIndex ~= 0 % not start condition
                  
                  fprintf ('Ping Index %d; Beam Index %d ignored at start of file\n', bgram.sCommon.u32_PingCounter, bgram.sCommon.u16_BeamIndex);
                  
                elseif parse_started == 0 && bgram.sCommon.u16_BeamIndex == 0 % start condition
                  
                  parse_started  = 1;
                  
                  output_bgrams(k) = bgram;                  
                  k = k + 1;
                  
                else % parsing has started

                   %if mod(bgram.sCommon.u32_PingCounter, Ndecimation) == 0
                
                     if bgram.sCommon.u16_BeamIndex ~= mod(output_bgrams(k - 1).sCommon.u16_BeamIndex + 1, bgram.sCommon.u16_NoBeams) % are we on the next beam?
                    
                      np  = bgram.sCommon.u16_NoBeams * (bgram.sCommon.u32_PingCounter - output_bgrams(k - 1).sCommon.u32_PingCounter + 1);
                      np  = np - (output_bgrams(k - 1).sCommon.u16_BeamIndex + 1);
                      np  = np - (bgram.sCommon.u16_NoBeams - bgram.sCommon.u16_BeamIndex);
                     
                      for p = 1 : np
                        
                        bgram_dummy.sCommon.u16_TxLevel     = output_bgrams(k - 1).sCommon.u16_TxLevel;
                        bgram_dummy.sCommon.u16_RxGaindB    = output_bgrams(k - 1).sCommon.u16_RxGaindB;
                        bgram_dummy.sCommon.u16_BeamIndex   = output_bgrams(k - 1).sCommon.u16_BeamIndex + 1;
                        bgram_dummy.sCommon.u32_PingCounter = output_bgrams(k - 1).sCommon.u32_PingCounter;
                        if bgram_dummy.sCommon.u16_BeamIndex == bgram.sCommon.u16_NoBeams
                          
                          bgram_dummy.sCommon.u16_TxLevel      = bgram.sCommon.u16_TxLevel;
                          bgram_dummy.sCommon.u16_RxGaindB    = bgram.sCommon.u16_RxGaindB;
                          bgram_dummy.sCommon.u16_BeamIndex   = 0;
                          bgram_dummy.sCommon.u32_PingCounter = output_bgrams(k - 1).u32_PingCounter + 1;
                          
                        end
                        
                        output_bgrams(k) = bgram_dummy;                  
                        k = k + 1;                                                                  
                        
                      end
                     
                     end
                  %endif
                  
                  output_bgrams(k) = bgram;                  
                  k = k + 1;                    
                  
                end  
  
            end 
                 
            fprintf("output_bgrams size: %d\n", numel(output_bgrams));   
            
            if isinteger(numel(output_bgrams) / NumberOfBeams) == false
              
              fprintf("Additional unwanted bgrams\n")
              
            end
          
            fclose(fid);

        end  
        
        function data_arrays = ensembles_to_data_arrays(obj, ensembles)

            fprintf("--- BGRAM ensembles_to_data_arrays ---\n")

            data_arrays = [];
              
            NumberOfBeams = ensembles(1).sCommon.u16_NoBeams;
            NumberOfCells = ensembles(1).ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES;
            
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
            
            fprintf("Number of beams: %d; Number of pings: %d; Number of cells: %d\n", NumberOfBeams, NumberOfPings, NumberOfCells);
            
            data_arrays.an16_IPingData = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.an16_QPingData = zeros(NumberOfBeams, NumberOfPings, NumberOfCells);
            data_arrays.temperatureC   = zeros(1, NumberOfPings);
            data_arrays.pressurePa     = zeros(1, NumberOfPings);
            data_arrays.Rollcdeg       = zeros(1, NumberOfPings);
            data_arrays.Pitchcdeg      = zeros(1, NumberOfPings);
            data_arrays.Headingcdeg    = zeros(1, NumberOfPings);
            data_arrays.Pings          = zeros(1, NumberOfPings);
            data_arrays.RxBWHz         = zeros(1, NumberOfPings);
            data_arrays.EventTime_s    = zeros(1, NumberOfPings);
            data_arrays.TxBWHz         = zeros(1, NumberOfPings);
            data_arrays.RxGdB          = zeros(1, NumberOfPings);
            data_arrays.TxGdB          = zeros(1, NumberOfPings);
            data_arrays.SIGBC          = zeros(1, NumberOfPings);
            data_arrays.SIGBT          = zeros(1, NumberOfPings);
            
            
            for i = 1:NumberOfPings
              
              data_arrays.temperatureC(i) = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n32_Temp_mdegC;
              data_arrays.pressurePa(i)   = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_PressurePa;
              data_arrays.Rollcdeg(i)     = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n16_Roll_cdeg;
              data_arrays.Pitchcdeg(i)    = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n16_Pitch_cdeg;
              data_arrays.Headingcdeg(i)  = ensembles((i - 1) * NumberOfBeams + 1).sCommon.n32_Heading_cdeg;
              data_arrays.Pings(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_PingCounter;
              data_arrays.RxBWHz(i)       = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_RxBandwidthHz;
              data_arrays.CarrierHz(i)    = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_CarrierFrequencyHz;
              data_arrays.TxBWHz(i)       = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u32_TxBandwidthHz;
              data_arrays.RxGdB(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_RxGaindB;
              data_arrays.TxG(i)          = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_TxLevel;
              data_arrays.SIGBC(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_SIG_BaseCounts;
              data_arrays.SIGBT(i)        = ensembles((i - 1) * NumberOfBeams + 1).sCommon.u16_SIG_BT;
              ciT_EventStartTime          = ensembles((i - 1) * NumberOfBeams + 1).sCommon.ciT_EventStartTime;

              data_arrays.EventTime_s(i)  = ciT_EventStartTime.u32_seconds + ciT_EventStartTime.u32_microseconds .* 1e-6;
              
              for k = 1:NumberOfBeams

                data_arrays.an16_IPingData(k, i, :) = ensembles((i - 1) * NumberOfBeams + k).an16_IPingData; 
                data_arrays.an16_QPingData(k, i, :) = ensembles((i - 1) * NumberOfBeams + k).an16_QPingData; 

              end
                
            end
                                            
        end
        end
        end


