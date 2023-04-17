function aGramFileDemonstrator(filename)
  
  printf("Reading %s\n", filename);
  
  addpath("../../src/DSPGram/octave");
  addpath("../../src/util/@adcputil");
  
  % create an agram object
  agram = SDSP_v1_0_ADCPGram();
  
  % decode the file of agrams
  ensembles = agram.decode_file(filename);

  % access a few fields from the first agram
  no_beams     = ensembles(1).sCommon.u16_NoBeams;
  f_carrier_Hz = ensembles(1).sCommon.u32_CarrierFrequencyHz;
  BT_product   = ensembles(1).sCommon.u16_SIG_BT;
  TxBW_Hz      = ensembles(1).sCommon.u32_TxBandwidthHz;
  
  % convert an array of agram records to arrays of data
  data_arrays = agram.ensembles_to_data_arrays(ensembles);
  
  % convert dphases to velocities
  velocities_ms = adcputil.dphases_to_velocities_ms(data_arrays.Dphases, f_carrier_Hz, BT_product, TxBW_Hz);
    
endfunction
