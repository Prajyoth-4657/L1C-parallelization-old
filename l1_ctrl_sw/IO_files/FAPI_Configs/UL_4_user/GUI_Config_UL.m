%%%%% GUI generated config file %%%% 

addpath('common','uplink','downlink',genpath('Other_codes'),'-end'); 

SYS.No_users = 4;
SYS.fc = 3500000000;
SYS.SC_RB = 12;
SYS.SCS = 30000;
SYS.Nprb = 273;
SYS.Noise_Power = -10;
SYS.No_slots = 1;
SYS.SFN = 853;
SYS.slot_offset = 15;
SYS.No_symb_slots = 14;
SYS.CELL.N_ID_cell = 20;
SYS.FFT_size = 4096;
SYS.SR = 122880000;
SYS.SR_scaling = 16;
SYS.CP_Len = 288;
SYS.CP_Len1 = 352;
SYS.num_ant = 4;
SYS.max_layers = 4;
SYS.MIB.dmrs_TypeA_Position = "pos2";

 %%% Adding default rach params - start %%%%%%%%%%%%%%%%%%%%%%%%%% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% PRACH parameters
SYS.RACH.granularity = 64;									% LC error of the timing advance
SYS.RACH.NTA_offset = 25600; % FR1 = 25600 ; FR2 = 13792
SYS.RACH.ConfigurationIndex = 71;
SYS.RACH.ROOT_seq_id = 53;
SYS.RACH.zeroCorrelationZoneConfig = 8;			% Value from 0-15
SYS.RACH.restrictedSetConfig = 'U';			% Restricted sets only for high speeds
SYS.RACH.RA_start = 110;					% Value is set such that all RACH occasions are in the same UL BWP
SYS.RACH.max_occasions_f = 1;				% Value can be 1,2,4,8

t_vals = nrPRACHConfig.Tables.ConfigurationsFR1Unpaired(1+SYS.RACH.ConfigurationIndex,:);
SYS.RACH.format = t_vals.PreambleFormat{1};
SYS.RACH.slots = reshape(t_vals.SubframeNumber{1}*(SYS.SCS/15e3)+(0:t_vals.PRACHSlotsPerSubframe-1)',1,[]);
SYS.RACH.start_symbol = t_vals.StartingSymbol;
SYS.RACH.max_occasions_t = t_vals.NumTimeOccasions;
SYS.RACH.time_duration = t_vals.PRACHDuration;
[ SYS.RACH.L_RA, SYS.RACH.f_RA, SYS.RACH.N_u, SYS.RACH.N_cp ] = preamble_format(SYS.RACH.format, SYS.SCS);
SYS.RACH.N_u = SYS.RACH.N_u/SYS.SR_scaling;
SYS.RACH.N_cp = SYS.RACH.N_cp/SYS.SR_scaling;
N_CS = get_NCS(SYS.RACH.zeroCorrelationZoneConfig, SYS.RACH.restrictedSetConfig, SYS.RACH.f_RA);

base_seq_id = SYS.RACH.ROOT_seq_id;
SYS.RACH.preambles = [];
SYS.RACH.num_preambles = 64;
i = 0;
while i < SYS.RACH.num_preambles
	u = u_lookup(base_seq_id,SYS.RACH.L_RA);
	base_seq_id = base_seq_id + 1;
	C_v = get_Cv(SYS.RACH.restrictedSetConfig, SYS.RACH.L_RA, N_CS, u);
	i = i + length(C_v);
	for j = 1:length(C_v)
		SYS.RACH.preambles = [SYS.RACH.preambles ; [u C_v(j)]];
	end
end
SYS.RACH.preambles = SYS.RACH.preambles(1:SYS.RACH.num_preambles,:);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Adding default rach params - end %%%%%%%%%%%%%%%%%%%%%%%%%% 

UE(1).UID = 1;
UE(1).C_RNTI = 111;
UE(1).num_ant = 1;
UE(1).SNR = 25;
UE(1).cfo = 0;
UE(1).speed = 5;
UE(1).DelaySpread = 3e-08;
UE(1).UL.Sym_len = 13;
UE(1).UL.Sym_start = 1;
UE(1).UL.mappingType = "typeA";
UE(1).UL.PRB_start = 0;
UE(1).UL.PRB_len = 56;
UE(1).UL.N_BWP_SIZE = 56;
UE(1).UL.newTX = true;
UE(1).UL.rv = 0;
UE(1).UL.rv_idx = 0;
UE(1).UL.soft_bits{1} = 0;
UE(1).UL.nLayers = 1;
UE(1).UL.MCS_table_no = 2;
UE(1).UL.MCS_index = 26;
UE(1).UL.DMRS.MIMO_UE_group = 1;
UE(1).UL.DMRS.n_scid = 0;
UE(1).UL.DMRS.scramblingID0 = 10001;
UE(1).UL.DMRS.scramblingID1 = 10001;
UE(1).UL.DMRS.dmrs_Type = "Type1";
UE(1).UL.DMRS.powerControlOffset = 0;
UE(1).UL.DMRS.reference_RB = 0;
UE(1).UL.DMRS.dmrs_TypeA_Position = "pos2";
UE(1).UL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(1).UL.DMRS.maxLength = "len1";
UE(1).UL.DMRS.ports = 1000;
UE(1).UL.DMRS.cdm_nodata = 1;
UE(1).UL.DMRS.startingRB = 0;
UE(1).UL.DMRS.nrofRBs = 56;
UE(1).UL.PRB_bundle = 1;
UE(1).LDPC_iter = 25;
UE(2).UID = 2;
UE(2).C_RNTI = 222;
UE(2).num_ant = 1;
UE(2).SNR = 25;
UE(2).cfo = 0;
UE(2).speed = 5;
UE(2).DelaySpread = 3e-08;
UE(2).UL.Sym_len = 12;
UE(2).UL.Sym_start = 2;
UE(2).UL.mappingType = "typeA";
UE(2).UL.PRB_start = 83;
UE(2).UL.PRB_len = 29;
UE(2).UL.N_BWP_SIZE = 29;
UE(2).UL.newTX = true;
UE(2).UL.rv = 0;
UE(2).UL.rv_idx = 0;
UE(2).UL.soft_bits{1} = 0;
UE(2).UL.nLayers = 1;
UE(2).UL.MCS_table_no = 2;
UE(2).UL.MCS_index = 23;
UE(2).UL.DMRS.MIMO_UE_group = 2;
UE(2).UL.DMRS.n_scid = 0;
UE(2).UL.DMRS.scramblingID0 = 20002;
UE(2).UL.DMRS.scramblingID1 = 20002;
UE(2).UL.DMRS.dmrs_Type = "Type1";
UE(2).UL.DMRS.powerControlOffset = 0;
UE(2).UL.DMRS.reference_RB = 0;
UE(2).UL.DMRS.dmrs_TypeA_Position = "pos3";
UE(2).UL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(2).UL.DMRS.maxLength = "len2";
UE(2).UL.DMRS.ports = 1000;
UE(2).UL.DMRS.cdm_nodata = 2;
UE(2).UL.DMRS.startingRB = 83;
UE(2).UL.DMRS.nrofRBs = 29;
UE(2).UL.PRB_bundle = 1;
UE(2).LDPC_iter = 25;
UE(3).UID = 3;
UE(3).C_RNTI = 333;
UE(3).num_ant = 1;
UE(3).SNR = 25;
UE(3).cfo = 0;
UE(3).speed = 5;
UE(3).DelaySpread = 3e-08;
UE(3).UL.Sym_len = 12;
UE(3).UL.Sym_start = 2;
UE(3).UL.mappingType = "typeA";
UE(3).UL.PRB_start = 148;
UE(3).UL.PRB_len = 40;
UE(3).UL.N_BWP_SIZE = 40;
UE(3).UL.newTX = true;
UE(3).UL.rv = 0;
UE(3).UL.rv_idx = 0;
UE(3).UL.soft_bits{1} = 0;
UE(3).UL.nLayers = 1;
UE(3).UL.MCS_table_no = 2;
UE(3).UL.MCS_index = 20;
UE(3).UL.DMRS.MIMO_UE_group = 3;
UE(3).UL.DMRS.n_scid = 0;
UE(3).UL.DMRS.scramblingID0 = 30003;
UE(3).UL.DMRS.scramblingID1 = 30003;
UE(3).UL.DMRS.dmrs_Type = "Type1";
UE(3).UL.DMRS.powerControlOffset = 0;
UE(3).UL.DMRS.reference_RB = 0;
UE(3).UL.DMRS.dmrs_TypeA_Position = "pos2";
UE(3).UL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(3).UL.DMRS.maxLength = "len2";
UE(3).UL.DMRS.ports = 1000;
UE(3).UL.DMRS.cdm_nodata = 1;
UE(3).UL.DMRS.startingRB = 148;
UE(3).UL.DMRS.nrofRBs = 40;
UE(3).UL.PRB_bundle = 1;
UE(3).LDPC_iter = 25;
UE(4).UID = 4;
UE(4).C_RNTI = 444;
UE(4).num_ant = 1;
UE(4).SNR = 25;
UE(4).cfo = 0;
UE(4).speed = 5;
UE(4).DelaySpread = 3e-08;
UE(4).UL.Sym_len = 13;
UE(4).UL.Sym_start = 1;
UE(4).UL.mappingType = "typeA";
UE(4).UL.PRB_start = 244;
UE(4).UL.PRB_len = 29;
UE(4).UL.N_BWP_SIZE = 29;
UE(4).UL.newTX = true;
UE(4).UL.rv = 0;
UE(4).UL.rv_idx = 0;
UE(4).UL.soft_bits{1} = 0;
UE(4).UL.nLayers = 1;
UE(4).UL.MCS_table_no = 2;
UE(4).UL.MCS_index = 17;
UE(4).UL.DMRS.MIMO_UE_group = 4;
UE(4).UL.DMRS.n_scid = 0;
UE(4).UL.DMRS.scramblingID0 = 40004;
UE(4).UL.DMRS.scramblingID1 = 40004;
UE(4).UL.DMRS.dmrs_Type = "Type1";
UE(4).UL.DMRS.powerControlOffset = 0;
UE(4).UL.DMRS.reference_RB = 0;
UE(4).UL.DMRS.dmrs_TypeA_Position = "pos3";
UE(4).UL.DMRS.dmrs_AdditionalPosition = "pos0";
UE(4).UL.DMRS.maxLength = "len1";
UE(4).UL.DMRS.ports = 1000;
UE(4).UL.DMRS.cdm_nodata = 2;
UE(4).UL.DMRS.startingRB = 244;
UE(4).UL.DMRS.nrofRBs = 29;
UE(4).UL.PRB_bundle = 1;
UE(4).LDPC_iter = 25;
for NU = 1:4
    UE(NU).UL.Precoder = repmat(eye(UE(NU).UL.nLayers),1,1,UE(NU).UL.PRB_len/UE(NU).UL.PRB_bundle);
    [UE(NU).UL.Modulation, UE(NU).UL.Mod, UE(NU).UL.R] = MCS_nr_mapping(UE(NU).UL.MCS_table_no, UE(NU).UL.MCS_index);
end
clearvars -except SYS UE 
