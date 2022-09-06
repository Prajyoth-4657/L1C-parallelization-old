%%%%% GUI generated config file %%%% 

addpath('common','downlink','uplink',genpath('Other_codes'),'-end'); 

SYS.No_users = 5;
SYS.fc = 3500000000;
SYS.SC_RB = 12;
SYS.SCS = 30000;
SYS.Nprb = 273;
SYS.Noise_Power = -10;
SYS.No_slots = 1;
SYS.slot_offset = 7;
SYS.No_symb_slots = 14;
SYS.CELL.N_ID_cell = 20;
SYS.FFT_size = 4096;
SYS.SR = 122880000;
SYS.SR_scaling = 16;
SYS.CP_Len = 288;
SYS.CP_Len1 = 352;
SYS.num_ant = 8;
SYS.max_layers = 8;
SYS.BCH.L_Max = 8;
SYS.BCH.SSB_sym_loc = 2;
SYS.BCH.nhf = 0;
SYS.BCH.start_prb = 110;
SYS.MIB.dmrs_TypeA_Position = "pos2";
SYS.SFN = 853;
UE(1).UID = 1;
UE(1).C_RNTI = 1;
UE(1).num_ant = 1;
UE(1).SNR = 25;
UE(1).cfo = 0;
UE(1).speed = 5;
UE(1).DelaySpread = 3e-08;
UE(1).DL.Sym_len = 11;
UE(1).DL.Sym_start = 3;
UE(1).DL.mappingType = "typeA";
UE(1).DL.PRB_start = 0;
UE(1).DL.PRB_len = 48;
UE(1).DL.N_BWP_SIZE = 48;
UE(1).DL.newTX = true;
UE(1).DL.rv = 0;
UE(1).DL.rv_idx = [0  2  3  1];
UE(1).DL.soft_bits{1} = 0;
UE(1).DL.soft_bits{2} = 0;
UE(1).DL.nLayers = 1;
UE(1).DL.MCS_table_no = 2;
UE(1).DL.MCS_index = 26;
UE(1).DL.Modulation = '256QAM';
UE(1).DL.Mod = 8;
UE(1).DL.R = 0.86426;
UE(1).DL.DMRS.MIMO_UE_group = 1;
UE(1).DL.DMRS.n_scid = 0;
UE(1).DL.DMRS.scramblingID0 = 1000;
UE(1).DL.DMRS.scramblingID1 = 1000;
UE(1).DL.DMRS.dmrs_Type = "Type1";
UE(1).DL.DMRS.powerControlOffset = 0;
UE(1).DL.DMRS.reference_RB = 0;
UE(1).DL.DMRS.dmrs_TypeA_Position = "pos3";
UE(1).DL.DMRS.dmrs_AdditionalPosition = "pos1";
UE(1).DL.DMRS.maxLength = "len2";
UE(1).DL.DMRS.ports = 1000;
UE(1).DL.DMRS.cdm_nodata = 1;
UE(1).DL.DMRS.startingRB = 0;
UE(1).DL.DMRS.nrofRBs = 48;
UE(1).DL.PRB_bundle = 1;
UE(1).DL.DCI.Format = "1-1";
UE(1).DL.DCI.DMRS_len = "double_symbol";
UE(1).DL.DCI.DCI11.DMRS_sequence_initialization = 0;
UE(1).DL.CSI.enabled = false;
UE(1).DL.CSI.sequenceGenerationConfig = 1;
UE(1).DL.CSI.nrofPorts = 1;
UE(1).DL.CSI.density = 3;
UE(1).DL.CSI.denenum = "odd";
UE(1).DL.CSI.powerControlOffsetSS = 0;
UE(1).DL.CSI.cdm_Type = "No_CDM";
UE(1).DL.CSI.row = 1;
UE(1).DL.CSI.T_offset = 2;
UE(1).DL.CSI.T_Periodicity = 4;
UE(1).DL.CSI.firstOFDMSymbolInTimeDomain = 4;
UE(1).DL.CSI.firstOFDMSymbolInTimeDomain2 = 7;
UE(1).DL.CSI.K = 0;
UE(1).DL.CSI.startingRB = 0;
UE(1).DL.CSI.nrofRBs = 10;
UE(1).DL.CSI.ports = [3000  3001  3002  3003  3004  3005  3006  3007];
UE(1).DL.CSI.frequencyDomainAllocation_row1 = [0  1  0  1];
UE(1).DL.CSI.frequencyDomainAllocation_row2 = [0  1  0  1  0  1  1  1  0  1  0  1];
UE(1).DL.CSI.frequencyDomainAllocation_row4 = [0  1  1];
UE(1).DL.CSI.frequencyDomainAllocation_other = [1  1  1  1  1  1];
UE(1).DL.NrOfCodewords = 1;
UE(1).DL.CODEWORD.rv = 0;
UE(1).DL.CODEWORD.newTX = true;
UE(1).DL.CODEWORD.MCS_table_no = 2;
UE(1).DL.CODEWORD.MCS_index = 26;
UE(1).DL.CODEWORD.Modulation = '256QAM';
UE(1).DL.CODEWORD.Mod = 8;
UE(1).DL.CODEWORD.R = 0.86426;
UE(1).LDPC_iter = 25;
UE(2).UID = 2;
UE(2).C_RNTI = 5;
UE(2).num_ant = 1;
UE(2).SNR = 25;
UE(2).cfo = 0;
UE(2).speed = 5;
UE(2).DelaySpread = 3e-08;
UE(2).DL.Sym_len = 12;
UE(2).DL.Sym_start = 2;
UE(2).DL.mappingType = "typeA";
UE(2).DL.PRB_start = 48;
UE(2).DL.PRB_len = 24;
UE(2).DL.N_BWP_SIZE = 24;
UE(2).DL.newTX = true;
UE(2).DL.rv = 0;
UE(2).DL.rv_idx = [0  2  3  1];
UE(2).DL.soft_bits{1} = 0;
UE(2).DL.soft_bits{2} = 0;
UE(2).DL.nLayers = 1;
UE(2).DL.MCS_table_no = 2;
UE(2).DL.MCS_index = 10;
UE(2).DL.Modulation = '16QAM';
UE(2).DL.Mod = 4;
UE(2).DL.R = 0.60156;
UE(2).DL.DMRS.MIMO_UE_group = 2;
UE(2).DL.DMRS.n_scid = 0;
UE(2).DL.DMRS.scramblingID0 = 2000;
UE(2).DL.DMRS.scramblingID1 = 2000;
UE(2).DL.DMRS.dmrs_Type = "Type1";
UE(2).DL.DMRS.powerControlOffset = 0;
UE(2).DL.DMRS.reference_RB = 0;
UE(2).DL.DMRS.dmrs_TypeA_Position = "pos2";
UE(2).DL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(2).DL.DMRS.maxLength = "len1";
UE(2).DL.DMRS.ports = 1000;
UE(2).DL.DMRS.cdm_nodata = 1;
UE(2).DL.DMRS.startingRB = 48;
UE(2).DL.DMRS.nrofRBs = 24;
UE(2).DL.PRB_bundle = 1;
UE(2).DL.DCI.Format = "1-1";
UE(2).DL.DCI.DMRS_len = "single_symbol";
UE(2).DL.DCI.DCI11.DMRS_sequence_initialization = 0;
UE(2).DL.CSI.enabled = false;
UE(2).DL.CSI.sequenceGenerationConfig = 1;
UE(2).DL.CSI.nrofPorts = 1;
UE(2).DL.CSI.density = 3;
UE(2).DL.CSI.denenum = "odd";
UE(2).DL.CSI.powerControlOffsetSS = 0;
UE(2).DL.CSI.cdm_Type = "No_CDM";
UE(2).DL.CSI.row = 1;
UE(2).DL.CSI.T_offset = 2;
UE(2).DL.CSI.T_Periodicity = 4;
UE(2).DL.CSI.firstOFDMSymbolInTimeDomain = 4;
UE(2).DL.CSI.firstOFDMSymbolInTimeDomain2 = 7;
UE(2).DL.CSI.K = 0;
UE(2).DL.CSI.startingRB = 0;
UE(2).DL.CSI.nrofRBs = 10;
UE(2).DL.CSI.ports = [3000  3001  3002  3003  3004  3005  3006  3007];
UE(2).DL.CSI.frequencyDomainAllocation_row1 = [0  1  0  1];
UE(2).DL.CSI.frequencyDomainAllocation_row2 = [0  1  0  1  0  1  1  1  0  1  0  1];
UE(2).DL.CSI.frequencyDomainAllocation_row4 = [0  1  1];
UE(2).DL.CSI.frequencyDomainAllocation_other = [1  1  1  1  1  1];
UE(2).DL.NrOfCodewords = 1;
UE(2).DL.CODEWORD.rv = 0;
UE(2).DL.CODEWORD.newTX = true;
UE(2).DL.CODEWORD.MCS_table_no = 2;
UE(2).DL.CODEWORD.MCS_index = 10;
UE(2).DL.CODEWORD.Modulation = '16QAM';
UE(2).DL.CODEWORD.Mod = 4;
UE(2).DL.CODEWORD.R = 0.60156;
UE(2).LDPC_iter = 25;
UE(3).UID = 3;
UE(3).C_RNTI = 7;
UE(3).num_ant = 1;
UE(3).SNR = 25;
UE(3).cfo = 0;
UE(3).speed = 5;
UE(3).DelaySpread = 3e-08;
UE(3).DL.Sym_len = 12;
UE(3).DL.Sym_start = 2;
UE(3).DL.mappingType = "typeA";
UE(3).DL.PRB_start = 72;
UE(3).DL.PRB_len = 24;
UE(3).DL.N_BWP_SIZE = 24;
UE(3).DL.newTX = true;
UE(3).DL.rv = 0;
UE(3).DL.rv_idx = [0  2  3  1];
UE(3).DL.soft_bits{1} = 0;
UE(3).DL.soft_bits{2} = 0;
UE(3).DL.nLayers = 1;
UE(3).DL.MCS_table_no = 2;
UE(3).DL.MCS_index = 3;
UE(3).DL.Modulation = 'QPSK';
UE(3).DL.Mod = 2;
UE(3).DL.R = 0.30078;
UE(3).DL.DMRS.MIMO_UE_group = 3;
UE(3).DL.DMRS.n_scid = 0;
UE(3).DL.DMRS.scramblingID0 = 3000;
UE(3).DL.DMRS.scramblingID1 = 3000;
UE(3).DL.DMRS.dmrs_Type = "Type1";
UE(3).DL.DMRS.powerControlOffset = 0;
UE(3).DL.DMRS.reference_RB = 0;
UE(3).DL.DMRS.dmrs_TypeA_Position = "pos2";
UE(3).DL.DMRS.dmrs_AdditionalPosition = "pos3";
UE(3).DL.DMRS.maxLength = "len1";
UE(3).DL.DMRS.ports = 1000;
UE(3).DL.DMRS.cdm_nodata = 1;
UE(3).DL.DMRS.startingRB = 72;
UE(3).DL.DMRS.nrofRBs = 24;
UE(3).DL.PRB_bundle = 1;
UE(3).DL.DCI.Format = "1-1";
UE(3).DL.DCI.DMRS_len = "single_symbol";
UE(3).DL.DCI.DCI11.DMRS_sequence_initialization = 0;
UE(3).DL.CSI.enabled = false;
UE(3).DL.CSI.sequenceGenerationConfig = 1;
UE(3).DL.CSI.nrofPorts = 1;
UE(3).DL.CSI.density = 3;
UE(3).DL.CSI.denenum = "odd";
UE(3).DL.CSI.powerControlOffsetSS = 0;
UE(3).DL.CSI.cdm_Type = "No_CDM";
UE(3).DL.CSI.row = 1;
UE(3).DL.CSI.T_offset = 2;
UE(3).DL.CSI.T_Periodicity = 4;
UE(3).DL.CSI.firstOFDMSymbolInTimeDomain = 4;
UE(3).DL.CSI.firstOFDMSymbolInTimeDomain2 = 7;
UE(3).DL.CSI.K = 0;
UE(3).DL.CSI.startingRB = 0;
UE(3).DL.CSI.nrofRBs = 10;
UE(3).DL.CSI.ports = [3000  3001  3002  3003  3004  3005  3006  3007];
UE(3).DL.CSI.frequencyDomainAllocation_row1 = [0  1  0  1];
UE(3).DL.CSI.frequencyDomainAllocation_row2 = [0  1  0  1  0  1  1  1  0  1  0  1];
UE(3).DL.CSI.frequencyDomainAllocation_row4 = [0  1  1];
UE(3).DL.CSI.frequencyDomainAllocation_other = [1  1  1  1  1  1];
UE(3).DL.NrOfCodewords = 1;
UE(3).DL.CODEWORD.rv = 0;
UE(3).DL.CODEWORD.newTX = true;
UE(3).DL.CODEWORD.MCS_table_no = 2;
UE(3).DL.CODEWORD.MCS_index = 3;
UE(3).DL.CODEWORD.Modulation = 'QPSK';
UE(3).DL.CODEWORD.Mod = 2;
UE(3).DL.CODEWORD.R = 0.30078;
UE(3).LDPC_iter = 25;
UE(4).UID = 4;
UE(4).C_RNTI = 8;
UE(4).num_ant = 1;
UE(4).SNR = 25;
UE(4).cfo = 0;
UE(4).speed = 5;
UE(4).DelaySpread = 3e-08;
UE(4).DL.Sym_len = 11;
UE(4).DL.Sym_start = 3;
UE(4).DL.mappingType = "typeA";
UE(4).DL.PRB_start = 116;
UE(4).DL.PRB_len = 36;
UE(4).DL.N_BWP_SIZE = 36;
UE(4).DL.newTX = true;
UE(4).DL.rv = 0;
UE(4).DL.rv_idx = [0  2  3  1];
UE(4).DL.soft_bits{1} = 0;
UE(4).DL.soft_bits{2} = 0;
UE(4).DL.nLayers = 1;
UE(4).DL.MCS_table_no = 2;
UE(4).DL.MCS_index = 18;
UE(4).DL.Modulation = '64QAM';
UE(4).DL.Mod = 6;
UE(4).DL.R = 0.75391;
UE(4).DL.DMRS.MIMO_UE_group = 4;
UE(4).DL.DMRS.n_scid = 0;
UE(4).DL.DMRS.scramblingID0 = 4000;
UE(4).DL.DMRS.scramblingID1 = 4000;
UE(4).DL.DMRS.dmrs_Type = "Type1";
UE(4).DL.DMRS.powerControlOffset = 0;
UE(4).DL.DMRS.reference_RB = 0;
UE(4).DL.DMRS.dmrs_TypeA_Position = "pos3";
UE(4).DL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(4).DL.DMRS.maxLength = "len2";
UE(4).DL.DMRS.ports = 1000;
UE(4).DL.DMRS.cdm_nodata = 1;
UE(4).DL.DMRS.startingRB = 116;
UE(4).DL.DMRS.nrofRBs = 36;
UE(4).DL.PRB_bundle = 1;
UE(4).DL.DCI.Format = "1-1";
UE(4).DL.DCI.DMRS_len = "double_symbol";
UE(4).DL.DCI.DCI11.DMRS_sequence_initialization = 0;
UE(4).DL.CSI.enabled = false;
UE(4).DL.CSI.sequenceGenerationConfig = 1;
UE(4).DL.CSI.nrofPorts = 1;
UE(4).DL.CSI.density = 3;
UE(4).DL.CSI.denenum = "odd";
UE(4).DL.CSI.powerControlOffsetSS = 0;
UE(4).DL.CSI.cdm_Type = "No_CDM";
UE(4).DL.CSI.row = 1;
UE(4).DL.CSI.T_offset = 2;
UE(4).DL.CSI.T_Periodicity = 4;
UE(4).DL.CSI.firstOFDMSymbolInTimeDomain = 4;
UE(4).DL.CSI.firstOFDMSymbolInTimeDomain2 = 7;
UE(4).DL.CSI.K = 0;
UE(4).DL.CSI.startingRB = 0;
UE(4).DL.CSI.nrofRBs = 10;
UE(4).DL.CSI.ports = [3000  3001  3002  3003  3004  3005  3006  3007];
UE(4).DL.CSI.frequencyDomainAllocation_row1 = [0  1  0  1];
UE(4).DL.CSI.frequencyDomainAllocation_row2 = [0  1  0  1  0  1  1  1  0  1  0  1];
UE(4).DL.CSI.frequencyDomainAllocation_row4 = [0  1  1];
UE(4).DL.CSI.frequencyDomainAllocation_other = [1  1  1  1  1  1];
UE(4).DL.NrOfCodewords = 1;
UE(4).DL.CODEWORD.rv = 0;
UE(4).DL.CODEWORD.newTX = true;
UE(4).DL.CODEWORD.MCS_table_no = 2;
UE(4).DL.CODEWORD.MCS_index = 18;
UE(4).DL.CODEWORD.Modulation = '64QAM';
UE(4).DL.CODEWORD.Mod = 6;
UE(4).DL.CODEWORD.R = 0.75391;
UE(4).LDPC_iter = 25;
UE(5).UID = 5;
UE(5).C_RNTI = 11;
UE(5).num_ant = 1;
UE(5).SNR = 25;
UE(5).cfo = 0;
UE(5).speed = 5;
UE(5).DelaySpread = 3e-08;
UE(5).DL.Sym_len = 12;
UE(5).DL.Sym_start = 2;
UE(5).DL.mappingType = "typeA";
UE(5).DL.PRB_start = 152;
UE(5).DL.PRB_len = 40;
UE(5).DL.N_BWP_SIZE = 40;
UE(5).DL.newTX = true;
UE(5).DL.rv = 0;
UE(5).DL.rv_idx = [0  2  3  1];
UE(5).DL.soft_bits{1} = 0;
UE(5).DL.soft_bits{2} = 0;
UE(5).DL.nLayers = 1;
UE(5).DL.MCS_table_no = 2;
UE(5).DL.MCS_index = 23;
UE(5).DL.Modulation = '256QAM';
UE(5).DL.Mod = 8;
UE(5).DL.R = 0.73633;
UE(5).DL.DMRS.MIMO_UE_group = 5;
UE(5).DL.DMRS.n_scid = 0;
UE(5).DL.DMRS.scramblingID0 = 5000;
UE(5).DL.DMRS.scramblingID1 = 5000;
UE(5).DL.DMRS.dmrs_Type = "Type1";
UE(5).DL.DMRS.powerControlOffset = 0;
UE(5).DL.DMRS.reference_RB = 0;
UE(5).DL.DMRS.dmrs_TypeA_Position = "pos2";
UE(5).DL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(5).DL.DMRS.maxLength = "len2";
UE(5).DL.DMRS.ports = 1000;
UE(5).DL.DMRS.cdm_nodata = 1;
UE(5).DL.DMRS.startingRB = 152;
UE(5).DL.DMRS.nrofRBs = 40;
UE(5).DL.PRB_bundle = 1;
UE(5).DL.DCI.Format = "1-1";
UE(5).DL.DCI.DMRS_len = "double_symbol";
UE(5).DL.DCI.DCI11.DMRS_sequence_initialization = 0;
UE(5).DL.CSI.enabled = false;
UE(5).DL.CSI.sequenceGenerationConfig = 1;
UE(5).DL.CSI.nrofPorts = 1;
UE(5).DL.CSI.density = 3;
UE(5).DL.CSI.denenum = "odd";
UE(5).DL.CSI.powerControlOffsetSS = 0;
UE(5).DL.CSI.cdm_Type = "No_CDM";
UE(5).DL.CSI.row = 1;
UE(5).DL.CSI.T_offset = 2;
UE(5).DL.CSI.T_Periodicity = 4;
UE(5).DL.CSI.firstOFDMSymbolInTimeDomain = 4;
UE(5).DL.CSI.firstOFDMSymbolInTimeDomain2 = 7;
UE(5).DL.CSI.K = 0;
UE(5).DL.CSI.startingRB = 0;
UE(5).DL.CSI.nrofRBs = 10;
UE(5).DL.CSI.ports = [3000  3001  3002  3003  3004  3005  3006  3007];
UE(5).DL.CSI.frequencyDomainAllocation_row1 = [0  1  0  1];
UE(5).DL.CSI.frequencyDomainAllocation_row2 = [0  1  0  1  0  1  1  1  0  1  0  1];
UE(5).DL.CSI.frequencyDomainAllocation_row4 = [0  1  1];
UE(5).DL.CSI.frequencyDomainAllocation_other = [1  1  1  1  1  1];
UE(5).DL.NrOfCodewords = 1;
UE(5).DL.CODEWORD.rv = 0;
UE(5).DL.CODEWORD.newTX = true;
UE(5).DL.CODEWORD.MCS_table_no = 2;
UE(5).DL.CODEWORD.MCS_index = 23;
UE(5).DL.CODEWORD.Modulation = '256QAM';
UE(5).DL.CODEWORD.Mod = 8;
UE(5).DL.CODEWORD.R = 0.73633;
UE(5).LDPC_iter = 25;
UE(1).DL.Precoder = repmat(eye(UE(1).DL.nLayers),1,1,UE(1).DL.PRB_len/UE(1).DL.PRB_bundle);
UE(2).DL.Precoder = repmat(eye(UE(2).DL.nLayers),1,1,UE(2).DL.PRB_len/UE(2).DL.PRB_bundle);
UE(3).DL.Precoder = repmat(eye(UE(3).DL.nLayers),1,1,UE(3).DL.PRB_len/UE(3).DL.PRB_bundle);
UE(4).DL.Precoder = repmat(eye(UE(4).DL.nLayers),1,1,UE(4).DL.PRB_len/UE(4).DL.PRB_bundle);
UE(5).DL.Precoder = repmat(eye(UE(5).DL.nLayers),1,1,UE(5).DL.PRB_len/UE(5).DL.PRB_bundle);
clearvars -except SYS UE 
