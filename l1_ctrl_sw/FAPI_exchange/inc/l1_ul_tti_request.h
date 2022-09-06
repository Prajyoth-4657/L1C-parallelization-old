#ifndef __L1_UL_TTI_REQUEST_H
#define __L1_UL_TTI_REQUEST_H

#include "constants.h"
#include <inttypes.h>

void get_ul_tti_request_data_from_hex_string(char* fapiHexData);
void get_ul_tti_request_data_from_file(char* bin_file);
void sort_PDU_UL();
void generate_RE_demapper_config();
void generate_dmrs_config_UL(int);
void cbs_configs_UL();
void generate_ch_eq_config();
void generate_UE_interleave_config();
void generate_ldpc_config_UL(int PUSCH_pdu_index, int UE_index, int CB_index, int overal_cb_index);
void generate_filler_bit_remover_config(int UE_index);
void generate_depuncturing_config(int UE_index, int CB_index);
void generate_rate_dematching_config(int UE_index, int CB_index);
void generate_deinterleaver_config(int UE_index, int CB_index);
void generate_descrambler_config();
void generate_LLR_gen_config();
void generate_CBCRC_config();
void generate_TBCRC_config();
void generate_UL_ORAN_config();
void generate_pucchF0_config();
void generate_pucchF0_UE_interleave_config();
void generate_pucchF2_config();
void generate_pucchF2_descrambler();
void generate_pucchF2_dmrs();
void generate_pucchF2_UE_interleave_config();
void generate_pucchF2_UE_interleave_config_old();
void generate_RX_Data_indication();
void generate_RACH_indication();
void generate_PRACH_PDU_length();
void generate_UCI_indication();
void generate_srs_grid_config();
void generate_srs_seq_config();
void generate_srs_fapi_out_config();
void generate_HARQ_config_user();
void generate_HARQ_config_cb(int cb_idx);
void ul_config();
void generate_pucch_pseudorandom_seq(int cinit);

/** This structure holds the body of the UL_TTI.request message
	*
	* @see table 3-44, SCF222
	*/

typedef struct ULPDU_t{
	uint16_t	PDUType;
	uint16_t	PDUSize;
} ULPDU_t;

typedef struct ul_UeList_t{
	uint8_t	nUe;
	uint8_t	PduIdx[MaxUE]; //TODO
} ul_UeList_t;

typedef struct ul_NumOfPRGs_t{
	uint16_t	beamIdx[MAX_BEAMIDX];
} ul_NumOfPRGs_t;

typedef struct RxPrecoding_Beamforming_t{
	uint16_t	numPRGs;
	uint16_t	prgSize;
	uint8_t		digBFInterface;
	ul_NumOfPRGs_t	NumOfPRGs[MAX_PRG];
} RxPrecoding_Beamforming_t;

typedef struct PRACH_PDU_t{
	uint16_t	physCellID;
	uint8_t		NumPrachOcas;
	uint8_t		prachFormat;
	uint8_t		numRa;
	uint8_t		prachStartSymbol;
	uint16_t	numCs;
	RxPrecoding_Beamforming_t	RxPrecoding_Beamforming;
} PRACH_PDU_t;

typedef struct puschData_t{
	uint8_t		rvIndex;
	uint8_t		harqProcessID;
	uint8_t		newDataIndicator;
	uint32_t	TBSize;
	uint16_t	numCb;
	uint8_t		cbPresentAndPosition[MAX_CEIL_NUM]; //TODO
} puschData_t;

typedef struct puschUci_t{
	uint16_t	harqAckBitLength;
	uint16_t	csiPart1BitLength;
	uint16_t	csiPart2BitLength;
	uint8_t		AlphaScaling;
	uint8_t		betaOffsetHarqAck;
	uint8_t		betaOffsetCsi1;
	uint8_t		betaOffsetCsi2;
} puschUci_t;

typedef struct PtrsPortConf_t{
	uint16_t	PTRSPortIndex;
	uint8_t		PTRSDmrsPort;
	uint8_t		PTRSReOffset;
} PtrsPortConf_t;

typedef struct puschPtrs_t{
	uint8_t		numPtrsPorts;
	PtrsPortConf_t	PtrsPortConf[MAX_PTRSPORT_CONF]; //:TODO
	uint8_t		PTRSTimeDensity;
	uint8_t		PTRSFreqDensity;
	uint8_t		ulPTRSPower;
} puschPtrs_t;

typedef struct dftsOfdm_t{
	uint8_t		lowPaprGroupNumber;
	uint16_t	lowPaprSequenceNumber;
	uint8_t		ulPtrsSampleDensity;
	uint8_t		ulPtrsTimeDensityTransformPrecoding;
} dftsOfdm_t;

typedef struct PUSCH_PDU_t{
	uint16_t	pduBitmap;
	uint16_t	RNTI;
	uint32_t	Handle;
	uint16_t	BWPSize;
	uint16_t	BWPStart;
	uint8_t		SubcarrierSpacing;
	uint8_t		CyclicPrefix;
	uint16_t	targetCodeRate;
	uint8_t		qamModOrder;
	uint8_t		mcsIndex;
	uint8_t		mcsTable;
	uint8_t		TransformPrecoding;
	uint16_t	dataScramblingId;
	uint8_t		nrOfLayers;
	uint16_t	ulDmrsSymbPos;
	uint8_t		dmrsConfigType;
	uint16_t	ulDmrsScramblingId;
	uint8_t		SCID;
	uint8_t		numDmrsCdmGrpsNoData;
	uint16_t	dmrsPorts;
	uint8_t		resourceAlloc;
	uint8_t		rbBitmap[MAX_RB_BITMAP];
	uint16_t	rbStart;
	uint16_t	rbSize;
	uint8_t		VRBtoPRBMapping;
	uint8_t		FrequencyHopping;
	uint16_t	txDirectCurrentLocation;
	uint8_t		uplinkFrequencyShift7p5khz;
	uint8_t		StartSymbolIndex;
	uint8_t		NrOfSymbols;

	puschData_t	puschData;
	puschUci_t	puschUci;
	puschPtrs_t	puschPtrs;
	dftsOfdm_t	dftsOfdm;
	RxPrecoding_Beamforming_t	RxPrecoding_Beamforming;
} PUSCH_PDU_t;

typedef struct PUCCH_PDU_t{
	uint16_t	RNTI;
	uint32_t	Handle;
	uint16_t	BWPSize;
	uint16_t	BWPStart;
	uint8_t		SubcarrierSpacing;
	uint8_t		CyclicPrefix;
	uint8_t		FormatType;
	uint8_t		multiSlotTxIndicator;
	uint8_t		pi2Bpsk;
	uint16_t	prbStart;
	uint16_t	prbSize;
	uint8_t		StartSymbolIndex;
	uint8_t		NrOfSymbols;
	uint8_t		freqHopFlag;
	uint16_t	secondHopPRB;
	uint8_t		groupHopFlag;
	uint8_t		sequenceHopFlag;
	uint16_t	hoppingId;
	uint16_t	InitialCyclicShift;
	uint16_t	dataScramblingId;
	uint8_t		TimeDomainOccIdx;
	uint8_t		PreDftOccIdx;
	uint8_t		PreDftOccLen;
	uint8_t		AddDmrsFlag;
	uint16_t	DmrsScramblingId;
	uint8_t		DMRScyclicshift;
	uint8_t		SRFlag;
	uint8_t		BitLenHarq;
	uint16_t	BitLenCsiPart1;
	uint16_t	BitLenCsiPart2;
	RxPrecoding_Beamforming_t	RxPrecoding_Beamforming;
} PUCCH_PDU_t;

typedef struct SRS_PDU_t{
	uint16_t	RNTI;
	uint32_t	Handle;
	uint16_t	BWPSize;
	uint16_t	BWPStart;
	uint8_t		SubcarrierSpacing;
	uint8_t		CyclicPrefix;
	uint8_t		numAntPorts;
	uint8_t		numSymbols;
	uint8_t		numRepetitions;
	uint8_t		timeStartPosition;
	uint8_t		configIndex;
	uint16_t	sequenceId;
	uint8_t		bandwidthIndex;
	uint8_t		combSize;
	uint8_t		combOffset;
	uint8_t		cyclicShift;
	uint8_t		frequencyPosition;
	uint8_t		frequencyShift;
	uint8_t		frequencyHopping;
	uint8_t		groupOrSequenceHopping;
	uint8_t		resourceType;
	uint16_t	Tsrs;
	uint16_t	Toffset;
	RxPrecoding_Beamforming_t	RxPrecoding_Beamforming;
} SRS_PDU_t;

typedef struct Ul_ttiRequest_t{
	uint16_t	MsgId;
	uint32_t	MsgLength;
	uint16_t	SFN;
	uint16_t	Slot;
	uint8_t		nPDUs;
	uint8_t		RachPresent;
	uint8_t		nULSCH;
	uint8_t		nULCCH;
	uint8_t		nGroup;
	ULPDU_t	ULPDU;
	PRACH_PDU_t PRACH_PDU[MaxPRACH_PDU];
	PUSCH_PDU_t PUSCH_PDU[MaxPUSCH_PDU];
	PUCCH_PDU_t PUCCH_PDU[MaxPUCCH_PDU];
	SRS_PDU_t	SRS_PDU[MaxSRS_PDU];
	ul_UeList_t UeList[MaxUE_List];
}Ul_ttiRequest_t;





// In the UL reception context, the TB_prop structure doesn't essentially correspond to a Transport block,
// instead it corresponds to a single user
typedef struct UE_prop_t{

	int BWP_idx;				// The BWP(RE allocation) allotted for this MIMO group

	// Values from Dl_tti_request
	int TBcrc;				// Size of TB after crc
	uint8_t rvIndex;			// rvIndex given for each TB
	uint8_t qamModOrder;		// QAM modulation order
	int TBS_LBRM;				// Parameter used to compute 
	uint8_t numLayers;			// Number of layers associated with one TB, see Section 5.4.2.1, TS 38.212 and Section 7.3.1.3, TS 38.211
	int reserved_RE;			// Number of REs UNAVAILABLE for TB transmission

	// Value assigned by L1 controller
	uint8_t user_id;			// A user id assigned to each TB by the L1 controller

	// Values computed in the CBS configs modules
	uint8_t base_graph_index;
	uint16_t K_dash;			// Size of CB before filler	
	uint8_t num_CB;				// number of codeblocks
	uint8_t K_b;				// Value used to compute K; Section 5.2.2, TS 38.212
	int start_CB_idx;			// The index of the first CB in the entire array of CB

	// Values computed in the LDPC_config module
	uint16_t Z_c;				// Value of lifting size from Table 5.3.2, TS 38.212
	uint16_t K;					// Size of CB after addition of filler bits
	uint8_t	lifting_size_index;	// Index of Z_c in a sorted array of all lifting size values from Table 5.3.2, TS 38.212
	int N_circular_buffer;		// Size of the circular buffer as computed in Section 5.4.2.1, TS 38.212
	
	// Values obtained from the rate matching module
	int G;
	uint16_t E[MAX_CB];			// Number of bits after rate matching for each code block


} UE_prop_t;

typedef struct BWP_prop_t{
	int numLayers;
} BWP_prop_t;

typedef struct Ul_tti_intermediate_t{
	uint8_t		nPUSCH;
	uint8_t		nPUCCH;
	uint8_t		nSRS;
	uint8_t		nPRACH;
	uint8_t		nPTRS;
	UE_prop_t 	TB_prop[MaxUE];
	uint8_t		nTB;
	uint8_t 	nCB;
	uint8_t		nBWP;
	BWP_prop_t	BWP_prop[MaxUE];

} Ul_tti_intermediate_t;

typedef struct CB_prop_t{
	int TB_idx;
	int CB_in_TB;
	int cb_end_time;
	int cb_end_freq;
	int cb_size;
	int cb_start_addr_loc;
	int layer_info;
	int E_per_layer;
} CB_prop_t;

typedef struct preamble_format_t{
	int format_type;
	int prach_duration;
} preamble_format_t;

typedef struct srs_BW_config_t{
	int m;
	int N;
} srs_BW_config_t;

typedef struct ORAN_sections_t{
	int start_freq;
	int num_freq;
	int start_symb;
	int num_symb;
	int reMask;
	int beamIdx[8];				// One beam index for each layer
	int PDU_type;				// 0:PUSCH , 1:PUCCH , 2:PRACH , 3:SRS , 4:empty(zeros)
} ORAN_sections_t;

#endif
