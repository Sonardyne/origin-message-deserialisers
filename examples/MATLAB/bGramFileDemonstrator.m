function bGramFileDemonstrator(filename)
  
  fprintf("Reading %s\n", filename);
  
  addpath("../../src/MATLAB/DSPGram");
  
  % create a bgram object
  bgram = SDSP_v1_0_BDGram();
  
  % decode the file of bgrams
  ensembles = bgram.decode_file(filename, 1);

  % access a few fields from the first bgram
  no_beams     = ensembles(1).sCommon.u16_NoBeams;
  f_carrier_Hz = ensembles(1).sCommon.u32_CarrierFrequencyHz;
  BT_product   = ensembles(1).sCommon.u16_SIG_BT;
  TxBW_Hz      = ensembles(1).sCommon.u32_TxBandwidthHz;
  
  % convert an array of bgram records to arrays of data
  data_arrays = bgram.ensembles_to_data_arrays(ensembles);  
  
end
