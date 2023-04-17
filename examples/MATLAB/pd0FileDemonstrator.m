function pd0FileDemonstrator(filename)
  
  fprintf("Reading %s\n", filename);
  
  addpath(genpath('../../src/MATLAB/PDN_messages/'))
  
  % create a PD0 object
  pd0 = PD0Message();
  
  % decode the file of PD0
  ensembles = pd0.decode_file(filename);  
  % access a few fields from the first PD0
  
  try   
    %rdifl = RDIFixedLeader;
    RDIFixedLeader = ensembles(1).Fields(0);
    fprintf("%d beams; %d cells\n", RDIFixedLeader.NumberOfBeams, RDIFixedLeader.NumberOfCells);
  catch  
    disp('No RDI variable leader available');
  end 
  
  try 
    %rdivl = RDIVariableLeader
    RDIVariableLeader = ensembles(1).Fields(128);    
    
    fprintf("1st Heading reading: %f deg\n", RDIVariableLeader.HeadingCD ./ 100);
  catch  
    disp('No RDI variable leader available');
  end 
  
  % convert an array of PD0 records to arrays of data
  data_arrays = pd0.ensembles_to_data_arrays(ensembles, true);  
  assignin("base", "data_arrays", data_arrays)

end
