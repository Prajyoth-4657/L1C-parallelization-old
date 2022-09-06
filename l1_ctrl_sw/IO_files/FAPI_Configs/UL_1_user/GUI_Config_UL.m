%%%%% GUI generated config file %%%% 

addpath('common','uplink','downlink',genpath('Other_codes'),'-end'); 

SYS.No_users = 1;
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

UE.UID = 1;
UE.C_RNTI = 444;
UE.num_ant = 1;
UE.SNR = 25;
UE.cfo = 0;
UE.speed = 5;
UE.DelaySpread = 3e-08;
UE.UL.Sym_len = 13;
UE.UL.Sym_start = 1;
UE.UL.mappingType = "typeA";
UE.UL.PRB_start = 244;
UE.UL.PRB_len = 29;
UE.UL.N_BWP_SIZE = 29;
UE.UL.newTX = true;
UE.UL.rv = 0;
UE.UL.rv_idx = 0;
UE.UL.soft_bits{1} = 0;
UE.UL.nLayers = 1;
UE.UL.MCS_table_no = 2;
UE.UL.MCS_index = 17;
UE.UL.DMRS.MIMO_UE_group = 1;
UE.UL.DMRS.n_scid = 0;
UE.UL.DMRS.scramblingID0 = 40004;
UE.UL.DMRS.scramblingID1 = 40004;
UE.UL.DMRS.dmrs_Type = "Type1";
UE.UL.DMRS.powerControlOffset = 0;
UE.UL.DMRS.reference_RB = 0;
UE.UL.DMRS.dmrs_TypeA_Position = "pos3";
UE.UL.DMRS.dmrs_AdditionalPosition = "pos0";
UE.UL.DMRS.maxLength = "len1";
UE.UL.DMRS.ports = 1000;
UE.UL.DMRS.cdm_nodata = 2;
UE.UL.DMRS.startingRB = 244;
UE.UL.DMRS.nrofRBs = 29;
UE.UL.PRB_bundle = 1;
UE.LDPC_iter = 25;
for NU = 1:1
    UE(NU).UL.Precoder = repmat(eye(UE(NU).UL.nLayers),1,1,UE(NU).UL.PRB_len/UE(NU).UL.PRB_bundle);
    [UE(NU).UL.Modulation, UE(NU).UL.Mod, UE(NU).UL.R] = MCS_nr_mapping(UE(NU).UL.MCS_table_no, UE(NU).UL.MCS_index);
end
clearvars -except SYS UE 
