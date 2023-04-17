classdef adcputil < handle

    properties
            
    endproperties
        
    % static methods
    methods (Static = true)

      function velocities_ms = dphases_to_velocities_ms(Dphases, Carrier_freq_Hz, BT_product, TxBWHz)
        
            Dphases_scaled = Dphases ./ 8192;
            tp = BT_product ./ TxBWHz;
            b  = (1/((2*pi)*tp));
            df = b .* Dphases_scaled;
            a  = 1500 ./ (2 .* Carrier_freq_Hz);
            v  = a*df;
         
            velocities_ms = v;
            
      endfunction

     endmethods      

endclassdef