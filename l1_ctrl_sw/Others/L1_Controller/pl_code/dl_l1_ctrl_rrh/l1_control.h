//Author: Raghav Subbaraman
//Date Created: ?
//Modifed: 15 Jan, 2019

#ifndef __L1CTRL_H__
#define __L1CTRL_H__

#include "ap_int.h"
#include "hls_stream.h"
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

using namespace std;

#define NUM_USER_CW 48

typedef ap_uint<64> eth_data_t;
typedef ap_uint<8> keep_data_t;
typedef ap_uint<32> user_data_t;
typedef ap_uint<32> fifo_count;
typedef ap_uint<16> payload_s;
typedef ap_uint<4> keep_count;
typedef ap_uint<4> de_mux_out;
typedef uint8_t wire_type;
typedef uint32_t long_wire_type;
typedef uint16_t pc_id_data;
typedef ap_uint<1> bit_wire;
typedef ap_uint<16> apuint_16;
typedef ap_uint<4> apuint_4;
typedef ap_uint<64> apuint_64;
typedef ap_uint<4> dest_t;

typedef struct cnfg_axis_t {
	eth_data_t conf;  //64bits
//	user_data_t user;
//	keep_data_t keep;
	dest_t dest;     //4bits
//	ap_uint<1> last;
} cnfg_axis_t;

typedef struct ecpri_axis {
	ap_uint<64> data;
	user_data_t user;
	keep_data_t keep;
	ap_uint<1> last;
} ecpri_axis;

typedef enum ecpri_RX_STATE {
	ecpri_RX_INIT = 0,
	ecpri_RX_WAIT,
	CTRL_ecpri_RCV_0,
	CTRL_ecpri_RCV_1,
	CTRL_ecpri_RCV_2,
	CTRL_ecpri_RCV_3,
	CTRL_ecpri_RCV_4,
	CTRL_ecpri_RCV_5,
	CTRL_ecpri_RCV_6,
	CTRL_ecpri_RCV_7,
	CTRL_ecpri_RCV_8,
	CTRL_ecpri_RCV_9,
	CTRL_ecpri_RCV_10,
	CTRL_ecpri_RCV_11,
	CTRL_ecpri_RCV_12,
	CTRL_ecpri_RCV_13,
	CTRL_ecpri_RCV_14,
} ecpri_RX_STATE;

typedef enum RRH_TX_STATE {
	RRH_TX_INIT = 0,
	RRH_TX_WAIT,
	CTRL_RRH_SEND_0,
	CTRL_RRH_SEND_1,
	CTRL_RRH_SEND_2,
	CTRL_RRH_SEND_3,
	CTRL_RRH_SEND_4,
	CTRL_RRH_SEND_5,
	CTRL_RRH_SEND_6,
} RRH_TX_STATE;

typedef enum L1_CTRL_STATE_t {
	PU_WAIT = 0,
	PU_INIT,
	PU_ecpri_RX,
	PU_CALC_0,
	PU_CALC_1,
	PU_SCHED,
	PU_POP,
	PU_POP_1,
	PU_POP_2,
	PU_RRH_TX,
	PU_WORK
} L1_CTRL_STATE_t;

typedef enum L1_CTRL_POP_STATE_t {
	POP_INIT,
	POP_UPLANE,
	POP_CRC,
	POP_LDPC,
	POP_PUNCT,
	POP_RM,
	POP_BI,
	POP_SC,
	POP_MOD,
	POP_ECPRI,
	POP_UINT,
	POP_DMRS,
	POP_CSIRS,
	POP_WAIT
} L1_CTRL_POP_STATE_t;


const ap_uint<9> LIFTING_SIZE_Z[51] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                                        18, 20, 22, 24, 26, 28, 30, 32, 36, 40, 44, 48, 52, 56, 60, 64, 72, 80,
                                        88, 96, 104, 112, 120, 128, 144, 160, 176, 192, 208, 224, 240, 256, 288,
                                        320, 352, 384
                                      };

const ap_uint<12> LIFTING_SIZE_10Z[51] = {20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160,
                                          180, 200, 220, 240, 260, 280, 300, 320, 360, 400, 440, 480, 520, 560, 600, 640, 720, 800,
                                          880, 960, 1040, 1120, 1200, 1280, 1440, 1600, 1760, 1920, 2080, 2240, 2400, 2560, 2880,
                                          3200, 3520, 3840
                                         };

const ap_uint<14> LIFTING_SIZE_22Z[51] = {44, 66, 88, 110, 132, 154, 176, 198, 220, 242, 264, 286, 308, 330, 352,
                                          396, 440, 484, 528, 572, 616, 660, 704, 792, 880, 968, 1056, 1144, 1232, 1320, 1408, 1584, 1760,
                                          1936, 2112, 2288, 2464, 2640, 2816, 3168, 3520, 3872, 4224, 4576, 4928, 5280, 5632, 6336,
                                          7040, 7744, 8448
                                         };

const uint16_t LIFTING_SIZE_52Z[51] = {104, 156, 208, 260, 312, 364, 416, 468, 520, 572, 624, 676, 728, 780, 832,
                                       936, 1040, 1144, 1248, 1352, 1456, 1560, 1664, 1872, 2080, 2288, 2496, 2704, 2912, 3120, 3328, 3744, 4160,
                                       4576, 4992, 5408, 5824, 6240, 6656, 7488, 8320, 9152, 9984, 10816, 11648, 12480, 13312, 14976,
                                       16640, 18304, 19968
                                      };

const uint16_t LIFTING_SIZE_66Z[51] = {132, 198, 264, 330, 396, 462, 528, 594, 660, 726, 792, 858, 924, 990, 1056,
                                       1188, 1320, 1452, 1584, 1716, 1848, 1980, 2112, 2376, 2640, 2904, 3168, 3432, 3696, 3960, 4224, 4752, 5280,
                                       5808, 6336, 6864, 7392, 7920, 8448, 9504, 10560, 11616, 12672, 13728, 14784, 15840, 16896, 19008,
                                       21120, 23232, 25344
                                      };

const uint16_t LIFTING_SIZE_42Z[51] = {84, 126, 168, 210, 252, 294, 336, 378, 420, 462, 504, 546, 588, 630, 672,
                                       756, 840, 924, 1008, 1092, 1176, 1260, 1344, 1512, 1680, 1848, 2016, 2184, 2352, 2520, 2688, 3024, 3360,
                                       3696, 4032, 4368, 4704, 5040, 5376, 6048, 6720, 7392, 8064, 8736, 9408, 10080, 10752, 12096,
                                       13440, 14784, 16128
                                      };

const uint16_t LIFTING_SIZE_40Z[51] = {80, 120, 160, 200, 240, 280, 320, 360, 400, 440, 480, 520, 560, 600, 640,
                                       720, 800, 880, 960, 1040, 1120, 1200, 1280, 1440, 1600, 1760, 1920, 2080, 2240, 2400, 2560, 2880, 3200,
                                       3520, 3840, 4160, 4480, 4800, 5120, 5760, 6400, 7040, 7680, 8320, 8960, 9600, 10240, 11520,
                                       12800, 14080, 15360
                                      };

typedef struct pdsch_pdu_t {
	/*
	Bitmap indicating presence of optional PDUsBit 0: pdschPtrs  -IndicatesPTRS included (FR2) Bit 1:cbgRetxCtrl (Present when CBG based retransmit is used)All other bits reserved
	*/
	ap_uint<16> pduBitmap;
	/*
	The RNTI used for identifying the UE when receiving the PDUValue: 1 -> 65535
	*/
	ap_uint<16> RNTI;
	/*
	PDU index incremented for each PDSCH PDU sent in TX control message. This is used to associate control information to data and is reset every slot. Value:0 ->65535
	*/
	ap_uint<16> pduIndex;
	/*
	Bandwidthpart size[TS38.213 sec12]. Number of contiguous PRBs allocated to the BWPValue: 1->275
	*/
	ap_uint<16> BWPSize;
	/*
	bandwidth part start RB indexfrom reference CRB [TS38.213 sec 12]Value: 0->274
	*/
	ap_uint<16> BWPStart;
	/*
	subcarrierSpacing [TS38.211 sec 4.2]Value:0->4
	*/
	ap_uint<8> SubcarrierSpacing;
	/*
	Cyclic prefix type[TS38.211 sec 4.2]0: Normal; 1: Extended
	*/
	ap_uint<8> CyclicPrefix;
	/*
	Number ofcodewordsfor this RNTI (UE)Value: 1 -> 2
	*/
	ap_uint<8> NrOfCodewords;
	/*
	Target coding rate [TS38.212sec5.4.2.1and 38.214 sec 5.1.3.1]. This is the number of information bits per 1024 coded bits expressed in 0.1 bit units
	*/
	ap_uint<16> targetCodeRate;
	/*
	QAM modulation[TS38.212sec5.4.2.1and 38.214 sec 5.1.3.1] Value: 2,4,6,8
	*/
	ap_uint<8> qamModOrder;
	/*
	MCS index[TS38.214, sec 5.1.3.1], should match value sent in DCIValue : 0->31
	*/
	ap_uint<8> mcsIndex;
	/*
	MCS-Table-PDSCH[TS38.214, sec 5.1.3.1]0: notqam2561: qam2562: qam64LowSE
	*/
	ap_uint<8> mcsTable;
	/*
	Redundancy versionindex [TS38.212, Table 5.4.2.1-2 and 38.214, Table 5.1.2.1-2], should match value sent in DCIValue : 0->3
	*/
	ap_uint<8> rvIndex;
	/*
	Transmit block size (in bytes)[TS38.214 sec 5.1.3.2]Value: 0->65535
	*/
	ap_uint<32> TBSize;
	/*
	dataScramblingIdentityPdsch [TS38.211, sec 7.3.1.1]It equals the higher-layer parameter Data-scrambling-Identity if configured and the RNTI equals the C-RNTI, otherwise L2 needs to set it to physical cell id.Value: 0->65535
	*/
	ap_uint<16> dataScramblingId;
	/*
	Number of layers [TS38.211, sec 7.3.1.3]Value : 1->8
	*/
	ap_uint<8> nrOfLayers;
	/*
	PDSCH transmission schemes [TS38.214, sec 5.1.1]0: Up to 8 transmission layers
	*/
	ap_uint<8> transmissionScheme;
	/*
	Reference point forPDSCH DMRS "k" -used for tone mapping [TS38.211, sec 7.4.1.1.2]Resource block bundles [TS38.211, sec 7.3.1.6]Value: 0 -> 1If 0, the 0 reference point for PDSCH DMRSis at Point A [TS38.211 sec 4.4.4.2]. Resource block bundles generated per sub-bullets 2 and 3 in [TS38.211, sec 7.3.1.6]. For sub-bullet 2, the start of bandwidth part must be set to the start of actual bandwidth part +NstartCORESETand the bandwidth of the bandwidth part must be set to the bandwidth of the initial bandwidth part.If 1, the DMRS reference point is at the lowest VRB/PRB of the allocation. Resource block bundles generated per sub-bullets 1 [TS38.211, sec7.3.1.6]
	*/
	ap_uint<8> refPoint;
	/*
	DMRS symbol positions [TS38.211, sec7.4.1.1.2andTables7.4.1.1.2-3 and 7.4.1.1.2-4]Bitmapoccupying the14 LSBswith: bit 0: first symboland for each bit0: no DMRS1: DMRS
	*/
	ap_uint<16> dlDmrsSymbPos;
	/*
	DL DMRS config type[TS38.211, sec 7.4.1.1.2]0: type 11: type2
	*/
	ap_uint<8> dmrsConfigType;
	/*
	DL-DMRS-Scrambling-ID [TS38.211, sec 7.4.1.1.2 ]If provided by the higher-layer and the PDSCH is scheduled by PDCCH with CRC scrambled by C-RNTI or CS-RNTI, otherwise, L2 should set thisto physical cell id.Value: 0->65535
	*/
	ap_uint<16> dlDmrsScramblingId;
	/*
	DMRS sequence initialization [TS38.211, sec 7.4.1.1.2]. Should match what is sent in DCI 1_1, otherwise set to 0.Value : 0->1
	*/
	ap_uint<8> SCID;
	/*
	Number of DM-RS CDM groups without data [TS38.212 sec 7.3.1.2.2] [TS38.214 Table 4.1-1]it determines the ratio of PDSCH EPRE to DM-RS EPRE.Value: 1->3
	*/
	ap_uint<8> numDmrsCdmGrpsNoData;
	/*
	DMRS ports.[TS38.212 7.3.1.2.2] provides description between DCI 1-1 content and DMRS ports.Bitmap occupying the 11 LSBswith: bit 0: antenna port 1000bit 11: antenna port 1011and for each bit0: DMRS port not used1: DMRS port used
	*/
	ap_uint<16> dmrsPorts;
	/*
	Resource Allocation Type [TS38.214, sec 5.1.2.2]0: Type 01: Type 1
	*/
	ap_uint<8> resourceAlloc;
	/*
	For resource allocationtype 1. [TS38.214, sec 5.1.2.2.2]The starting resource block within the BWP for this PDSCH.Value: 0->274
	*/
	ap_uint<16> rbStart;
	/*
	For resource allocation type 1. [TS38.214, sec 5.1.2.2.2]The number of resource block within for this PDSCH Value: 1->275
	*/
	ap_uint<16> rbSize;
	/*
	VRB-to-PRB-mapping[TS38.211, sec 7.3.1.6]0: non-interleaved 1: interleaved with RB size 22: Interleaved with RB size 4
	*/
	ap_uint<8> VRBtoPRBMapping;
	/*
	Start symbol index of PDSCH mappingfrom the start of the slot, S. [TS38.214, Table 5.1.2.1-1]
	Value: 0->13
	*/
	ap_uint<8> StartSymbolIndex;
	/*
	PDSCH duration in symbols, L [TS38.214, Table 5.1.2.1-1]
	Value: 1->14
	*/
	ap_uint<8> NrOfSymbols;
	/*
	PT-RS antenna ports [TS38.214, sec 5.1.6.3] [TS38.211, table 7.4.1.2.2-1]Bitmap occupying the 6LSBswith: bit 0: antenna port 1000bit 5: antenna port 1005and for each bit0: PTRSport not used1: PTRSport used
	*/
	ap_uint<8> PTRSPortIndex;
	/*
	PT-RS time density[TS38.214, table 5.1.6.3-1]0: 11: 22: 4
	*/
	ap_uint<8> PTRSTimeDensity;
	/*
	PT-RS frequency density[TS38.214, table 5.1.6.3-2]0: 21: 4
	*/
	ap_uint<8> PTRSFreqDensity;
	/*
	PT-RS resource elementoffset[TS38.211, table 7.4.1.2.2-1]
	Value: 0->3
	*/
	ap_uint<8> PTRSReOffset;
	/*
	PT-RS-to-PDSCH EPRE ratio [TS38.214, table 4.1-2]Value :0->3
	*/
	ap_uint<8> nEpreRatioOfPDSCHToPTRS;
	/*
	Ratio of PDSCH EPRE to NZP CSI-RSEPRE [TS38.214, sec 5.2.2.3.1]
	Value :0->23representing -8 to 15 dB in 1dB steps
	*/
	ap_uint<8> powerControlOffset;
	/*
	Ratio of SSB/PBCH block EPRE to NZP CSI-RS EPRES [TS38.214, sec 5.2.2.3.1]
	Values:0: -3dB,1: 0dB,2: 3dB,3: 6dB
	*/
	ap_uint<8> powerControlOffsetSS;
	/*
	Indicates whether last CB is present in the CBG retransmission 0: CBG retransmission does not include last CB 1: CBG retransmission includes last CB If CBG Re-Tx includes last CB, L1 will add the TB CRC to the last CB in the payload before it is read into the LDPC HW unit
	*/
	ap_uint<8> IsLastCbPresent;
	/*
	Indicates whether TB CRC is part of data payload or control message 0: TB CRC is part of data payload 1: TB CRC is part of control message
	*/
	ap_uint<8> isInlineTbCrc;
	/*
	TB CRC: to be used in the last CB, applicable only if last CB is present
	*/
	ap_uint<8> dlTbCrc;
} pdsch_pdu_t;


typedef struct l1_ctrl_inst_t {
	/*
	nan
	*/
	ap_uint<8> n_users;
	/*
	nan
	*/
	ap_uint<1> param_calc_tb_dirt_bit;
	/*
	nan
	*/
	ap_uint<1> param_calc_rm_dirt_bit;
	/*
	nan
	*/
	ap_uint<9> max_cb_sched;
	/*
	nan
	*/
	ap_uint<9> cur_phy_sched;
	/*
	nan
	*/
	ap_uint<8> cw_phy_index_sched[512];
} l1_ctrl_inst_t;
/*
{'axi_w': 64, 'mode': 3, 'filename': ['l1_ctrl_new.csv']}
*/



// Uncomment the following for debug output
//#ifndef TB_DEBUG
//#define TB_DEBUG
//#endif

// Top Function
void l1_ctrl_bbu(
    hls::stream<ecpri_axis> &ecpri_in,
    hls::stream<cnfg_axis_t> &cnfg_out,
    wire_type &state_ecpri_rx,
    wire_type &state_ctrl_bbu_pop,
    wire_type &state_l1_ctrl,
    bit_wire sfn_clk,
    bit_wire sfn_ctrl_clk
);

// AXIS Interface to read config information from ecpri
void ecpri_rx(hls::stream<ecpri_axis> &ecpri_in, wire_type &state_ecpri_rx, bit_wire sfn_ctrl_clk, bool &ecpri_done);

// Calculation function for various parameters
void l1_ctrl_bbu_calc_tb_params();
void l1_ctrl_bbu_calc_rm_params();
void l1_ctrl_bbu_calc_z(ap_uint<2> bg, ap_uint<14> cb_size, ap_uint<6> &Z_ind);

// Function that "pop"ulates config fifos in the phy layer by "pop"ping config messages one at a time
void l1_ctrl_bbu_pop(hls::stream<cnfg_axis_t> &cnfg_out, wire_type &state_ctrl_bbu_pop, wire_type cw_phy_index, bool &phy_sched_up);


// RRH Specific new funtions
void dmrs_pop_rrh(hls::stream<cnfg_axis_t> &cnfg_out, wire_type &state_ctrl_bbu_pop, wire_type cw_phy_index);
void calc_u_int_params(hls::stream<cnfg_axis_t> &cnfg_out);
void l1_ctrl_rrh_pdsch(
    hls::stream<ecpri_axis> &ecpri_in,
    hls::stream<cnfg_axis_t> &cnfg_out,
    wire_type &state_ecpri_rx,
    wire_type &state_ctrl_rrh_pop,
    wire_type &state_l1_ctrl,
    bit_wire sfn_clk,
	bit_wire &rrh_reset
);

#endif
