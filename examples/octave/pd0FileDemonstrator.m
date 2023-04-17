function pd0FileDemonstrator(filename)
  
  printf("Reading %s\n", filename);
  
  addpath('../../src/PDNMessages/@PDN_messages/@PD0Message')
  
  % create a PD0 object
  pd0 = PD0Message();
  
  % decode the file of PD0
  ensembles = pd0.decode_file(filename);
  
  % access a few fields from the first PD0
  try   
    RDIFixedLeader = ensembles(1).Fields(RDIFixedLeader.get_id());
    printf("%d beams; %d cells\n", RDIFixedLeader.NumberOfBeams, RDIFixedLeader.NumberOfCells);
  catch  
    disp('No RDI variable leader available');
  end 
  
  try   
    RDIVariableLeader = ensembles(1).Fields(RDIVariableLeader.get_id());
    printf("1st Heading reading: %f deg\n", RDIVariableLeader.HeadingCD ./ 100);
  catch  
    disp('No RDI variable leader available');
  end 
  
  % convert an array of PD0 records to arrays of data
  data_arrays = pd0.ensembles_to_data_arrays(ensembles);    

endfunction
