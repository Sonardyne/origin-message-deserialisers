function pd0FileDemonstrator(filename)
  
  fprintf("Reading %s\n", filename);
  
  addpath(genpath('../../src/MATLAB/PDN_messages/'))
  addpath(genpath('../../src/MATLAB'))
  
  % create a PD0 object
  pd0 = PD0Message();
  
  % decode the file of PD0 - beam frame
  ensembles = pd0.decode_file(filename);

  % --- Example velocity transformations --- %
  % Transform velocities to a different reference frame (3 = Earth frame is used as an example)
  % 0 = beam frame
  % 1 = instrument frame
  % 2 = vessel frame
  % 3 = Earth frame
  reference_frame = 3;
  
  % Loop through all ensembles
  for k = 1 : numel(ensembles)
    
    ensembles(k).transform(reference_frame);
    
  end
    
  % --- Example PD0 field inspection --- %
  ensemble_to_check = 1;  
  
  % Inspect the fixed leader in the first ensemble
  try   
    RDIFixedLeader = ensembles(ensemble_to_check).Fields(0);
    fprintf("%d beams; %d cells\n", RDIFixedLeader.NumberOfBeams, RDIFixedLeader.NumberOfCells);
  catch  
    disp('No RDI variable leader available');
  end 
  
  % Inspect the variable leader in the first ensemble
  try 
    RDIVariableLeader = ensembles(ensemble_to_check).Fields(128);    
    
    fprintf("1st Heading reading: %f deg\n", RDIVariableLeader.HeadingCD ./ 100);
  catch  
    disp('No RDI variable leader available');
  end 
  
  % --- Example data processing --- %
  % convert an array of PD0 records to arrays of data
  data_arrays = pd0.ensembles_to_data_arrays(ensembles);    

  % Velocity components to average
  East_index  = 1;
  North_index = 2;
  Up_index    = 3;

  % Average velocities over the central 50% of the profile
  number_of_cells = size(data_arrays.VelocitiesMms, 3)
  cell_average_index = ceil(number_of_cells ./ 4 : 3 .* number_of_cells ./ 4);
  
  vE_data     = squeeze(data_arrays.VelocitiesMms(East_index, :, cell_average_index));
  vE_mean_mms = mean(vE_data(:));
  
  vN_data     = squeeze(data_arrays.VelocitiesMms(North_index, :, cell_average_index));
  vN_mean_mms = mean(vN_data(:));
  
  vU_data     = squeeze(data_arrays.VelocitiesMms(Up_index, :, cell_average_index));
  vU_mean_mms = mean(vU_data(:));
  
  % display the answer
  disp('Average velocity in mm/s (East, North, Up)');
  [vE_mean_mms, vN_mean_mms, vU_mean_mms]

end
