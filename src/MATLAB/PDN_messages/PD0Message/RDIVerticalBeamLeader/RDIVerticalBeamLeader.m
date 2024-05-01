%%% RDI Vertical Beam Leader
classdef RDIVerticalBeamLeader < handle

    properties
    
        ID                  = 0x0F01
        DepthCells          = 0
        VerticalPings       = 0
        DepthCellSizeCm     = 0
        FirstCellRangeCm    = 0
        VerticalMode        = 0
        VerticalTransmitCm  = 0
        VerticalLagLengthCm = 0
        TransmitCodeEls     = 0
        VertRSSIThresh      = 0
        VertShallowBin      = 0
        VertStartBin        = 0
        VertShallowRSSIBin  = 0
        MaxCoreThreshold    = 0
        MinCoreThreshold    = 0
        PingOffsetTimeMs    = 0
        Spare1              = 0
        DepthScreen         = 0
        PercentGoodThresh   = 0
        VerticalDOProofing  = 0
        
    end
        
    % static methods
    methods (Static = true)
        function id = get_id(obj)
            id = 0x0F01;
        end      
    end      

    
    methods
        
        function hdr = RDIVerticalBeamLeader()
            
        end
    
        function h = create(obj)
          
          h = RDIVerticalBeamLeader();
          
        end
            
        function length_bytes = calc_length(obj)
            length_bytes = 40;
        end
        
        function deserialise(obj, fid)
        
            obj.ID                  = fread(fid, 1, "uint16");
            obj.DepthCells          = fread(fid, 1, "uint16"); 
            obj.VerticalPings       = fread(fid, 1, "uint16"); 
            obj.DepthCellSizeCm     = fread(fid, 1, "uint16"); 
            obj.FirstCellRangeCm    = fread(fid, 1, "uint16"); 
            obj.VerticalMode        = fread(fid, 1, "uint16"); 
            obj.VerticalTransmitCm  = fread(fid, 1, "uint16"); 
            obj.VerticalLagLengthCm = fread(fid, 1, "uint16"); 
            obj.TransmitCodeEls     = fread(fid, 1, "uint16"); 
            obj.VertRSSIThresh      = fread(fid, 1, "uint16"); 
            obj.VertShallowBin      = fread(fid, 1, "uint16"); 
            obj.VertStartBin        = fread(fid, 1, "uint16"); 
            obj.VertShallowRSSIBin  = fread(fid, 1, "uint16"); 
            obj.MaxCoreThreshold    = fread(fid, 1, "uint16"); 
            obj.MinCoreThreshold    = fread(fid, 1, "uint16"); 
            obj.PingOffsetTimeMs    = fread(fid, 1, "int16"); 
            obj.Spare1              = fread(fid, 1, "uint16"); 
            obj.DepthScreen         = fread(fid, 1, "uint16"); 
            obj.PercentGoodThresh   = fread(fid, 1, "uint16"); 
            obj.VerticalDOProofing  = fread(fid, 1, "uint16"); 
       
        end        

        function copy(obj, c)
          
            obj.ID = c.ID; 
            obj.DepthCells = c.DepthCells;
            obj.VerticalPings = c.VerticalPings;
            obj.DepthCellSizeCm = c.DepthCellSizeCm;
            obj.FirstCellRangeCm = c.FirstCellRangeCm;
            obj.VerticalMode = c.VerticalMode;
            obj.VerticalTransmitCm = c.VerticalTransmitCm;
            obj.VerticalLagLengthCm = c.VerticalLagLengthCm;
            obj.TransmitCodeEls = c.TransmitCodeEls;
            obj.VertRSSIThresh = c.VertRSSIThresh;
            obj.VertShallowBin = c.VertShallowBin;
            obj.VertStartBin = c.VertStartBin;
            obj.VertShallowRSSIBin = c.VertShallowRSSIBin;
            obj.MaxCoreThreshold = c.MaxCoreThreshold;
            obj.MinCoreThreshold = c.MinCoreThreshold;
            obj.PingOffsetTimeMs = c.PingOffsetTimeMs;
            obj.Spare1 = c.Spare1;
            obj.DepthScreen = c.DepthScreen;
            obj.PercentGoodThresh = c.PercentGoodThresh;
            obj.VerticalDOProofing = c.VerticalDOProofing;                        

        end        
        
    end
end
