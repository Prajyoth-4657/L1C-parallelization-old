#ifndef __L1_DLTTI_REQUEST_H
#define __L1_DLTTI_REQUEST_H

#include "constants.h"
#include <inttypes.h>

void get_dl_tti_request_data_from_hex_string(char* fapiHexDlttiReq_i);
void append_ul_dci_request_data_from_hex_string(char* fapiHexUciReq_i, int initializeFlag);
void get_dl_tti_request_data_from_file(char* bin_file);
void cbs_configs_DL();
void sort_PDU_DL();
void generate_ldpc_config_DL(int TB_index);
void generate_puncturing_config(int TB_index);
void generate_rate_matching_config(int PDSCH_pdu_index, int TB_index, int CB_index);
void generate_bit_interleaving_config(int TB_index, int CB_index);
void generate_dmrs_config_DL(int PDU_idx);
void generate_modulator_config(int TB_index, int CB_index);
void generate_golden_sequence_config(int PDSCH_pdu_index, int TB_index);
void generate_scrambler_config(int TB_index, int CB_index);
void generate_layer_mapper_config(int PDSCH_pdu_index, int TB_index, int CB_index);
void generate_csirs_config(int PDU_idx);
void generate_UEinterleaved_RE_mapper_config();
void generate_pbch_config();
void generate_ORAN_config();
void generate_pdcch_config();
void generate_pdcch_RE_Mapper_config();
void generate_PDCCH_DMRS_config();
void generate_PDCCH_User_Interleaver_config();
void generate_PDCCH_User_Interleaver_CB_Config();
void generate_REG_interleaver_config();
void dl_config();
void dl_config_1();
void* thread_1_run();
void* thread_2_run();
void get_contiguous_RB_alloc_from_bitmap(uint8_t* bitMapArr, int* pStartRB_out, int* pRBSize_out);

// forward declaration
// struct	DLPDU_t;
// struct TxPrecoding_Beamforming_t;
// struct numOfDlDcis_t;
// struct PDCCH_PDU_t;
// struct Dl_ttiRequest_t;
// struct UeList_t;
// struct NumOfPRGs_t;
// struct PDSCH_PDU_t;
// struct CodewordInfo_t;
// struct CSIRS_PDU_t;
// struct SSB_PDU_t;
// struct bchPayload_t;
// struct MACgenMIBPDU_t;
// struct PHYgenMIBPDU_t;

typedef struct DLPDU_t{
	uint16_t	PDUType;
	uint16_t	PDUSize;
} DLPDU_t;

typedef struct NumOfPRGs_t{
	uint16_t	PMidx;
	uint16_t	beamIdx[MAX_BEAMIDX];
} NumOfPRGs_t;


typedef struct TxPrecoding_Beamforming_t{
	uint16_t	numPRGs;
	uint16_t	prgSize;
	uint8_t		digBFInterfaces;
	NumOfPRGs_t	NumOfPRGs[MAX_PRG];
} TxPrecoding_Beamforming_t;

typedef struct numOfDlDcis_t{
	uint16_t	RNTI;
	uint16_t	ScramblingId;
	uint16_t	ScramblingRNTI;
	uint8_t		CceIndex;
	uint8_t		AggregationLevel;
	TxPrecoding_Beamforming_t	TxPrecoding_Beamforming;
	uint8_t		beta_PDCCH_1_0;
	uint8_t		powerControlOffsetSS;
	uint16_t	PayloadSizeBits;
	uint8_t		Payload[DCI_PAYLOAD_BTYE_LEN];
} numOfDlDcis_t;



typedef struct PDCCH_PDU_t {
	uint16_t	BWPSize;
	uint16_t	BWPStart;
	uint8_t		SubcarrierSpacing;
	uint8_t		CyclicPrefix;
	uint8_t		StartSymbolIndex;
	uint8_t		DurationSymbols;
	uint8_t		FreqDomainResource[MAX_FREQ_DOM_RESOURCE];
	uint8_t		CceRegMappingType;
	uint8_t		RegBundleSize;
	uint8_t		InterleaverSize;
	uint8_t		CoreSetType;
	uint16_t	ShiftIndex;
	uint8_t		precoderGranularity;
	uint16_t	numDlDci;
	numOfDlDcis_t	numOfDlDcis[MAX_DLDCI];
} PDCCH_PDU_t;

typedef struct CodewordInfo_t{
	uint16_t	targetCodeRate;
	uint8_t		qamModOrder;
	uint8_t		mcsIndex;
	uint8_t		mcsTable;
	uint8_t		rvIndex;
	uint32_t	TBSize;
} CodewordInfo_t;


typedef struct PDSCH_PDU_t{
	uint16_t	pduBitmap;
	uint16_t	RNTI;
	uint16_t	pduIndex;
	uint16_t	BWPSize;
	uint16_t	BWPStart;
	uint8_t		SubcarrierSpacing;
	uint8_t		CyclicPrefix;
	uint8_t		NrOfCodewords;
	CodewordInfo_t	CodewordInfo[MAX_CODE_WORDS];
	uint16_t	dataScramblingId;
	uint8_t		nrOfLayers;
	uint8_t		transmissionScheme;
	uint8_t		refPoint;
	uint16_t	dlDmrsSymbPos;
	uint8_t		dmrsConfigType;
	uint16_t	dlDmrsScramblingId;
	uint8_t		SCID;
	uint8_t		numDmrsCdmGrpsNoData;
	uint16_t	dmrsPorts;
	uint8_t		resourceAlloc;
	uint8_t		rbBitmap[MAX_RB_BITMAP];
	uint16_t	rbStart;
	uint16_t	rbSize;
	uint8_t		VRBtoPRBMapping;
	uint8_t		StartSymbolIndex;
	uint8_t		NrOfSymbols;
	uint8_t		PTRSPortIndex;
	uint8_t		PTRSTimeDensity;
	uint8_t		PTRSFreqDensity;
	uint8_t		PTRSReOffset;
	uint8_t		nEpreRatioOfPDSCHToPTRS;
	TxPrecoding_Beamforming_t	TxPrecoding_Beamforming;
	uint8_t		powerControlOffset;
	uint8_t		powerControlOffsetSS;
	uint8_t		IsLastCbPresent;
	uint8_t		isInlineTbCrc;
	uint32_t	dlTbCrc;
} PDSCH_PDU_t;


typedef struct UeList_t{
	uint8_t nUe;
	uint8_t PduIdx[MaxUE];
} UeList_t;


typedef struct CSIRS_PDU_t{
	uint16_t	BWPSize;
	uint16_t	BWPStart;
	uint8_t		SubcarrierSpacing;
	uint8_t		CyclicPrefix;
	uint16_t	StartRB;
	uint16_t	NrOfRBs;
	uint8_t		CSIType;
	uint8_t		Row;
	uint16_t	FreqDomain;
	uint8_t		SymbL0;
	uint8_t		SymbL1;
	uint8_t		CDMType;
	uint8_t		FreqDensity;
	uint16_t	ScrambId;
	uint8_t		powerControlOffset;
	uint8_t		powerControlOffsetSS;
	TxPrecoding_Beamforming_t	TxPrecoding_Beamforming;
} CSIRS_PDU_t;

typedef struct MACgenMIBPDU_t{
	uint32_t	bchPayload;
} MACgenMIBPDU_t;


typedef struct bchPayload_t{
	MACgenMIBPDU_t	MACgenMIBPDU;
} bchPayload_t;

typedef struct SSB_PDU_t{
	uint16_t	physCellId;
	uint8_t		betaPss;
	uint8_t		ssbBlockIndex;
	uint8_t		ssbSubcarrierOffset;
	uint16_t	SsbOffsetPointA;
	uint8_t		bchPayloadFlag;
	bchPayload_t bchPayload;
	TxPrecoding_Beamforming_t	TxPrecoding_Beamforming;
} SSB_PDU_t;


typedef struct Dl_ttiRequest_t{
	uint8_t no_of_PHY_API_MSGS;
	uint8_t FAPI_HANDLE;
	uint16_t	MsgId;
	uint32_t 	MsgLength;
	uint16_t	SFN;
	uint16_t	Slot;
	uint8_t		nPDUs;
	uint8_t		nGroup;
	DLPDU_t		DLPDU;
	PDCCH_PDU_t PDCCH_PDU[MaxPDCCH_PDU];
	PDSCH_PDU_t PDSCH_PDU[MaxPDSCH_PDU];
	CSIRS_PDU_t CSIRS_PDU[MaxCSIRS_PDU];
	SSB_PDU_t	SSB_PDU[MaxSSB_PDU];
	UeList_t UeList[MAX_UElist];
} Dl_ttiRequest_t;






typedef struct TB_prop_t{
	// Values from Dl_tti_request
	uint16_t TBcrc;				// Size of TB after crc
	uint8_t rvIndex;			// rvIndex given for each TB
	uint8_t qamModOrder;		// QAM modulation order
	int TBS_LBRM;				// Parameter used to compute 
	uint8_t numLayers;			// Number of layers associated with one TB, see Section 5.4.2.1, TS 38.212 and Section 7.3.1.3, TS 38.211
	int reserved_RE;			// Number of REs UNAVAILABLE for TB transmission
	int G_modulation;			// Number of modulated symbols per layer

	// Value assigned by L1 controller
	uint8_t user_id;			// A user id assigned to each TB by the L1 controller

	// Values computed in the CBS configs modules
	uint8_t base_graph_index;
	uint16_t K_dash;			// Size of CB before filler	
	uint8_t num_CB;				// number of codeblocks
	uint8_t K_b;				// Value used to compute K; Section 5.2.2, TS 38.212
	uint8_t q;					// Codeword index; Section 7.3.1.1, TS 38.211

	// Values computed in the LDPC_config module
	uint16_t Z_c;				// Value of lifting size from Table 5.3.2, TS 38.212
	uint16_t K;					// Size of CB after addition of filler bits
	uint8_t	lifting_size_index;	// Index of Z_c in a sorted array of all lifting size values from Table 5.3.2, TS 38.212
	
	// Values obtained from the rate matching module
	uint16_t E[MAX_CB];			// Number of bits after rate matching for each code block


} TB_prop_t;

typedef struct Dl_tti_intermediate_t{
	uint8_t		nPDSCH;
	uint8_t		nPDCCH;
	uint8_t		nCSIRS;
	uint8_t		nSSB;
	TB_prop_t	TB_prop[2*MaxUE];
	uint8_t		nTB;
	uint16_t	nCB;

} Dl_tti_intermediate_t;

typedef struct csirs_table_t{
	int numPorts;
	int FDM;
	int TDM;
	int num_CDM_groups;
	int num_L0;					// OFDM symbols [L0 : L0+num_L0-1] have CSIRS
	int num_L1;					// OFDM symbols [L1 : L1+num_L1-1] have CSIRS
} csirs_table_t;

typedef struct ORAN_sections_t{
	int start_freq;
	int num_freq;
	int start_symb;
	int num_symb;
	int reMask;
	int beamIdx[8];				// One beam index for each layer
	int PDU_type;				// 0:PDSCH , 1:PDCCH , 2:SSB , 3:CSIRS , 4:empty(zeros)
} ORAN_sections_t;

typedef struct pdcch_intermediate_t{
	int total_REG_Bundles;
	int pdcch_present[MaxPDCCH_PDU];
	int num_RB_user[MaxPDCCH_PDU];
	int max_length[MaxPDCCH_PDU];
	int start_RB_reg[MaxPDCCH_PDU][MAX_DLDCI][MAX_REG_BUNDLE];
	int num_RB_REG_Bundle[MaxPDCCH_PDU][MAX_DLDCI];
	int num_REG_Bundle[MaxPDCCH_PDU][MAX_DLDCI];
	int interleaveMapping[MaxPDCCH_PDU][MAX_DLDCI][MAX_REG_BUNDLE];
	int userpersymbol[14][MaxUE];
	int start_RB_reg_new[MaxPDCCH_PDU][MAX_DLDCI][MAX_REG_BUNDLE];
	int start_RB_reg_new_indices[MaxPDCCH_PDU][MAX_DLDCI][MAX_REG_BUNDLE];
	int reg_freq_start[MaxPDCCH_PDU];
	int start_RB_reg_all[MaxPDCCH_PDU][MAX_REG_BUNDLE];
	int start_RB_reg_all_index[MaxPDCCH_PDU][MAX_REG_BUNDLE];
	int total_REG_Bundles_per_user[MaxPDCCH_PDU];
} pdcch_intermediate_t;

#endif
