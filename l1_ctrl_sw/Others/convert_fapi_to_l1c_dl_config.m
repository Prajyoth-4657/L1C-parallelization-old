% Script file for converting DL_TTI message.bin to corresponding L1C Configs_DL.txt
% It uses "testmake_DL" program of L1C 
%
% --------------------------------------------------------------------
% - In case l1_dl_tti_request.c is modified, then do the following in
% command line for updating (making) "testmake_DL" program
% -----------------------
% cd ../FAPI_exchange/src
% mkdir obj %-- if obj directory is not present
% make -f Makefile_DL.txt
% -----------------------
% ---------------------------------------------------------------

FAPI_config_dir = '../IO_files/FAPI_Configs/DL_TTI_5_users_single_layer_12_slots';
slot_indices = 0:11;


FAPI_config_dir = strip(FAPI_config_dir,'right','/'); % remove end slash
for k=slot_indices
    slot_dir = [FAPI_config_dir '/Slot' num2str(k)];
    [status, result] = system( ['../FAPI_exchange/src/testmake_DL ' slot_dir '/message.bin ' slot_dir '/configs_DL.txt']);
    if 0 ~= status
        disp('Error executing program testmake_DL');
        disp(result);
    else
       % movefile('configs_DL.txt',slot_dir); % copy to FAPI_config_dir/Slotk location
    end
end
