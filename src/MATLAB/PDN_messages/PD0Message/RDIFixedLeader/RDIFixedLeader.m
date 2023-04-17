%%% PD0 Fixed Leader
classdef RDIFixedLeader < handle

    properties
    
        ID                   = 0x0000
        CpuFirmwareVer       = 0
        CpuFirmwareRev       = 0
        SystemConfig         = 0
        RealOrSim            = 0
        LagLengthCm          = 0
        NumberOfBeams        = 0
        NumberOfCells        = 0
        PingsPerEnsemble     = 0
        DepthCellLengthCm    = 0
        BlankAfterTransmitCm = 0
        ProfilingMode        = 0
        LowcorrThresh        = 0
        CodeRepeats          = 0
        PercentGood          = 0
        ErrorVelMax          = 0
        TPPMins              = 0
        TPPSecs              = 0
        TPPHunds             = 0
        CoordTxfrm           = 0
        HeadingAlignment     = 0
        HeadingBias          = 0
        SensorSource         = 0
        SensorAvail          = 0
        Bin1DistanceCm       = 0
        TxPulseLengthCm      = 0
        StartEndDepthCell    = 0
        FalseTargetThresh    = 0
        Spare1               = 0
        TxLagDistanceCm      = 0
        Spares               = 0
        WBSystemBandwidth    = 0
        SystemPower          = 0
        Spare2               = 0
        SerialNumber         = 0
        BeamAngle            = 0
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0000;
        end      
    end      
        

    methods
        
        function hdr = RDIFixedLeader()
            
        end
    
        function h = create(obj)
          
          h = RDIFixedLeader();
          
        end
            
        function length_bytes = calc_length(obj)
            length_bytes = 59;
        end
        
        function deserialise(obj, fid)
        
            obj.ID                   = fread(fid, 1, "uint16");        
            obj.CpuFirmwareVer       = fread(fid, 1, "uint8");
            obj.CpuFirmwareRev       = fread(fid, 1, "uint8");
            obj.SystemConfig         = fread(fid, 1, "uint16");        
            obj.RealOrSim            = fread(fid, 1, "uint8");        
            obj.LagLengthCm          = fread(fid, 1, "uint8");        
            obj.NumberOfBeams        = fread(fid, 1, "uint8");        
            obj.NumberOfCells        = fread(fid, 1, "uint8");        
            obj.PingsPerEnsemble     = fread(fid, 1, "uint16");        
            obj.DepthCellLengthCm    = fread(fid, 1, "uint16");        
            obj.BlankAfterTransmitCm = fread(fid, 1, "uint16");        
            obj.ProfilingMode        = fread(fid, 1, "uint8");        
            obj.LowcorrThresh        = fread(fid, 1, "uint8");        
            obj.CodeRepeats          = fread(fid, 1, "uint8");        
            obj.PercentGood          = fread(fid, 1, "uint8");        
            obj.ErrorVelMax          = fread(fid, 1, "uint16");        
            obj.TPPMins              = fread(fid, 1, "uint8");        
            obj.TPPSecs              = fread(fid, 1, "uint8");        
            obj.TPPHunds             = fread(fid, 1, "uint8");        
            obj.CoordTxfrm           = fread(fid, 1, "uint8");        
            obj.HeadingAlignment     = fread(fid, 1, "uint16");        
            obj.HeadingBias          = fread(fid, 1, "uint16");        
            obj.SensorSource         = fread(fid, 1, "uint8");        
            obj.SensorAvail          = fread(fid, 1, "uint8");        
            obj.Bin1DistanceCm       = fread(fid, 1, "uint16");        
            obj.TxPulseLengthCm      = fread(fid, 1, "uint16");        
            obj.StartEndDepthCell    = fread(fid, 1, "uint16");        
            obj.FalseTargetThresh    = fread(fid, 1, "uint8");        
            obj.Spare1               = fread(fid, 1, "uint8");        
            obj.TxLagDistanceCm      = fread(fid, 1, "uint16");        
            obj.Spares               = fread(fid, 8, "uint8");        
            obj.WBSystemBandwidth    = fread(fid, 1, "uint16");        
            obj.SystemPower          = fread(fid, 1, "uint8");        
            obj.Spare2               = fread(fid, 1, "uint8");        
            obj.SerialNumber         = fread(fid, 1, "uint32");        
            obj.BeamAngle            = fread(fid, 1, "uint8");        
        
        end        

        function copy(obj, c)
          
            obj.ID = c.ID; 
            obj.CpuFirmwareVer = c.CpuFirmwareVer;
            obj.CpuFirmwareRev = c.CpuFirmwareRev;
            obj.SystemConfig = c.SystemConfig;
            obj.RealOrSim = c.RealOrSim;
            obj.LagLengthCm = c.LagLengthCm;
            obj.NumberOfBeams = c.NumberOfBeams;
            obj.NumberOfCells = c.NumberOfCells;
            obj.PingsPerEnsemble = c.PingsPerEnsemble;
            obj.DepthCellLengthCm = c.DepthCellLengthCm;
            obj.BlankAfterTransmitCm = c.BlankAfterTransmitCm;
            obj.ProfilingMode = c.ProfilingMode;
            obj.LowcorrThresh = c.LowcorrThresh;
            obj.CodeRepeats = c.CodeRepeats;
            obj.PercentGood = c.PercentGood;
            obj.ErrorVelMax = c.ErrorVelMax;
            obj.TPPMins = c.TPPMins;
            obj.TPPSecs = c.TPPSecs;
            obj.TPPHunds = c.TPPHunds;
            obj.CoordTxfrm = c.CoordTxfrm;
            obj.HeadingAlignment = c.HeadingAlignment;
            obj.HeadingBias = c.HeadingBias;
            obj.SensorSource = c.SensorSource;
            obj.SensorAvail = c.SensorAvail;
            obj.Bin1DistanceCm = c.Bin1DistanceCm;
            obj.TxPulseLengthCm = c.TxPulseLengthCm;
            obj.StartEndDepthCell = c.StartEndDepthCell;
            obj.FalseTargetThresh = c.FalseTargetThresh;
            obj.Spare1 = c.Spare1;
            obj.TxLagDistanceCm = c.TxLagDistanceCm;
            obj.Spares = c.Spares;
            obj.WBSystemBandwidth = c.WBSystemBandwidth;       
            obj.SystemPower = c.SystemPower;      
            obj.Spare2 = c.Spare2;   
            obj.SerialNumber = c.SerialNumber;
            obj.BeamAngle = c.BeamAngle;
            
        end        
        
        % method to transform between velocity frames
        %
        % Octave does not support enum, so stick to the following convention:
        %
        % 0 = beam frame
        % 1 = instrument frame
        % 2 = vessel frame
        % 3 = earth frame
        function transform(obj, velocity_frame)

          if velocity_frame == 0
            
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 4, 0);
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 5, 0);
            
          elseif velocity_frame == 1
            
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 4, 1);
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 5, 0);
            
          elseif velocity_frame == 2
            
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 4, 0);
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 5, 1);
            
          elseif velocity_frame == 3
          
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 4, 1);
            obj.CoordTxfrm = bitset(obj.CoordTxfrm, 5, 1);
          
          end  
              
        end
           
    end
end
