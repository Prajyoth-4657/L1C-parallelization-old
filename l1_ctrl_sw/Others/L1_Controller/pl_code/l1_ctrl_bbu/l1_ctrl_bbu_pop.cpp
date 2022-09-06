#include "l1_control.h"

extern pdsch_l1_ctrl_t pdsch_l1_ctrl[NUM_USER_CW];
extern l1_ctrl_inst_t l1_ctrl_inst;

void l1_ctrl_bbu_pop(hls::stream<cnfg_axis_t> &cnfg_out,wire_type &state_ctrl_bbu_pop, wire_type cw_phy_index, bool &phy_sched_up){
// Function that populates the CTRL fifos everywhere.

//#pragma HLS PIPELINE II = 2
#pragma HLS PIPELINE enable_flush

	static enum L1_CTRL_POP_STATE_t l1_ctrl_pop_state;
	cnfg_axis_t cnfg_data;
	static ap_uint<14> E_stat;
	ap_uint<11> num_ecpri_msg;

	state_ctrl_bbu_pop = l1_ctrl_pop_state;

	switch(l1_ctrl_pop_state){

	case POP_INIT:
		// TODO: Do some INIT
		l1_ctrl_pop_state = POP_CRC;
		phy_sched_up = 0;
		break;

	case POP_CRC:   //pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.k

		// TODO: Package parameters for CRC

		l1_ctrl_pop_state = POP_UPLANE;
		phy_sched_up = 0;

		break;

	case POP_UPLANE:

		// TODO: Package parameters for UPLANE

		l1_ctrl_pop_state = POP_LDPC;
		phy_sched_up = 0;

		break;
//1. YY
	case POP_LDPC:

//		typedef struct cnfg_ldpc_param_t{
//			// Parameters for the LDPC Configuration Block
//			ap_uint<14> k; // cb size including CRC
//			ap_uint<2> bg; // base graph
//			ap_uint<9> Z; // lifting factor Z
//			uint8_t tb_phy_ind; // PHY index of transport block
//		}cnfg_ldpc_param_t;
//		csim works as expected

		cnfg_data.conf = 0x0;

		cnfg_data.conf.range(13,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.k;  // .k depends on >> pdsch_l1_ctrl[i].pdsch_pdu.TBSize >> pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb   , num_cb depends on >> pdsch_l1_ctrl[i].pdsch_pdu.TBSize >> pdsch_l1_ctrl[i].pdsch_pdu.targetCodeRate 
		cnfg_data.conf.range(15,14) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.bg;   //Variable, can be 1 or 2
		cnfg_data.conf.range(21,16) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind;  //
		cnfg_data.conf.range(32,25) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.tb_phy_ind;  //not being used, value is saved as 0 in fapi_rx

		cnfg_data.user = 0;
		cnfg_data.keep = 33;
		cnfg_data.last = 1;
		cnfg_data.dest = 0;

		cnfg_out.write(cnfg_data);

		l1_ctrl_pop_state = POP_PUNCT;
		phy_sched_up = 0;


		break;
//2. PP
	case POP_PUNCT:

//		typedef struct puncturing_param_t{
//			// Parameters for the puncturing block
//			ap_uint<14> k; // Number of bits in CB after CRC
//			ap_uint<14> kcb; // Number of bits in CB after adding filler bits - 22Z or 10Z
//			ap_uint<9> zcb; // Value of lifting factor Z
//			uint16_t ncb; // Number of bits in codeword after LDPC 66Z or 52Z
//		} puncturing_param_t;
//		csim works as expected

		cnfg_data.conf = 0x0;

		cnfg_data.conf.range(13,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.k;
		cnfg_data.conf.range(36,28) = LIFTING_SIZE_Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind];  //

		if(pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.bg == 1){
			cnfg_data.conf.range(27,14) = LIFTING_SIZE_22Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind];
			cnfg_data.conf.range(52,37) = LIFTING_SIZE_66Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind];
		}
		else{
			cnfg_data.conf.range(27,14) = LIFTING_SIZE_10Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind];
			cnfg_data.conf.range(52,37) = LIFTING_SIZE_52Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind];
		}

		cnfg_data.user = 0;
		cnfg_data.keep = 53;
		cnfg_data.last = 1;
		cnfg_data.dest = 1;

		cnfg_out.write(cnfg_data);

		l1_ctrl_pop_state = POP_RM;
		phy_sched_up = 0;


		break;
//3. TT
	case POP_RM:

//		typedef struct rate_matching_param_t{
//			// Parameters for the nrRateMatching block
//			uint16_t N; // Number of useful bits after LDPC and punct = ncb - 2*Z - (kcb-k)
//			ap_uint<18> E; // Number of bits at the output of RM for this codeblock
//			ap_uint<2> rvid; // Redundancy version id, fix to 0 if no HARQ
//			ap_uint<2> bg; // Base graph value
//			ap_uint<9> Z; // Lifting factor Z
//
//			// These are variables that are used to decide
//			// the value of E to use for the current codeblock
//			ap_uint<32> G;
//			ap_uint<14> E_0;
//			ap_uint<14> E_1;
//			uint8_t E_shift;
//		} rate_matching_param_t;
// 		csim working as expected

		cnfg_data.conf = 0x0;

		if(pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.bg == 1){
			cnfg_data.conf.range(15,0) = LIFTING_SIZE_42Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind] + 2*LIFTING_SIZE_Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind] + pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.k;
		}
		else{
			cnfg_data.conf.range(15,0) = LIFTING_SIZE_40Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind] + pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.k;
		}



		if(pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.E_shift_1 > 0){  //depends on num_cb , div_g
			E_stat = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.E_0;
			cnfg_data.conf.range(33,16) = E_stat;
			pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.E_shift_1 -= 1;
		}
		else{
			E_stat = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.E_1;
			cnfg_data.conf.range(33,16) = E_stat;
		}
		cnfg_data.conf.range(35,34) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.rvIndex;
		cnfg_data.conf.range(37,36) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.bg;
		cnfg_data.conf.range(46,38) = LIFTING_SIZE_Z[pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.Z_ind];

		cnfg_data.user = 0;
		cnfg_data.keep = 47;
		cnfg_data.last = 1;
		cnfg_data.dest = 2;

		cnfg_out.write(cnfg_data);

		l1_ctrl_pop_state = POP_BI;
		phy_sched_up = 0;

		break;
//4. HH, done
	case POP_BI:

//		typedef struct bit_interleaver_param_t{
//			// Parameters for the bit interleaver block
//			apuint_4 Q_V; // Modulation order 2,4,6,8
//			uint32_t RMsize; // Output of rate matching block for this codeblock = E
//		}bit_interleaver_param_t;
//		csim working as expected
		cnfg_data.conf = 0x0;

		cnfg_data.conf.range(3,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.qamModOrder;
		cnfg_data.conf.range(35,4) = E_stat;  //SAME AS POP_RM

		cnfg_data.user = 0;
		cnfg_data.keep = 36;
		cnfg_data.last = 1;
		cnfg_data.dest = 3;

		cnfg_out.write(cnfg_data);

		l1_ctrl_pop_state = POP_SC;
		phy_sched_up = 0;

		break;
//5. MM, DONE
	case POP_SC:

//		prs_precoder_params.dirty_bit
//		typedef struct prs_precoder_param_t{
//			uint32_t cinit;
//			bool dirty_bit;
//		}prs_precoder_param_t;
//		csim works as expected
		cnfg_data.conf = 0x0;

		if(!pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.dirty_bit){
			pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.dirty_bit = 1;
			cnfg_data.conf.range(31,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_c_pdu.cinit;

			cnfg_data.user = 0;
			cnfg_data.keep = 32;
			cnfg_data.last = 1;
			cnfg_data.dest = 4;

			cnfg_out.write(cnfg_data);

		}
		else{
			// Do nothing
			cnfg_data.conf = 0;
			cnfg_data.user = 255;
			cnfg_data.keep = 32;
			cnfg_data.last = 1;
			cnfg_data.dest = 4;

//			cnfg_out.write(cnfg_data);
		}


		l1_ctrl_pop_state = POP_MOD;
		phy_sched_up = 0;

		break;
//6. BBB , DONE
// MODULATION:
	case POP_MOD:

//		csim works as expected
		cnfg_data.conf = 0x0;

		cnfg_data.conf.range(31,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.qamModOrder;
		cnfg_data.conf.range(63,32) = 0x0;
		cnfg_data.user = 0;
		cnfg_data.keep = 32;
		cnfg_data.last = 1;
		cnfg_data.dest = 5;

		cnfg_out.write(cnfg_data);


		l1_ctrl_pop_state = POP_ECPRI;
		phy_sched_up = 0;

		break;
//7. //UUU,done
	case POP_ECPRI:

		cnfg_data.conf = 0x0;

		// PCID
		cnfg_data.conf.range(15,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.RNTI;

		// User Phy index
		cnfg_data.conf.range(31,16) = cw_phy_index;

		// Payload size in bytes
		num_ecpri_msg = (E_stat/pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.qamModOrder); 

		if(num_ecpri_msg[0]){
			cnfg_data.conf.range(47,32) = (num_ecpri_msg >> 1) + 1;
		} else{
			cnfg_data.conf.range(47,32) = num_ecpri_msg >> 1;
		}


		cnfg_data.user = 0;
		cnfg_data.keep = 48;
		cnfg_data.last = 1;
		cnfg_data.dest = 6;

		cnfg_out.write(cnfg_data);

		l1_ctrl_pop_state = POP_INIT;
		phy_sched_up = 1;

		break;

	default:
		l1_ctrl_pop_state = POP_INIT;
	}



}

//KKKK, DONE
void dmrs_pop(hls::stream<cnfg_axis_t> &cnfg_out, wire_type &state_ctrl_bbu_pop, wire_type cw_phy_index){

	#pragma HLS PIPELINE

	cnfg_axis_t cnfg_data;

	state_ctrl_bbu_pop = 0xdd;

	cnfg_data.conf.range(8,0) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.rbStart;
	cnfg_data.conf.range(17,9) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.rbSize;
	cnfg_data.conf.range(18,18) = !(pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.dmrsConfigType);
	cnfg_data.conf.range(32,19) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.dlDmrsSymbPos.range(13,0);
	cnfg_data.conf.range(36,33) = 0; // TODO: Fix this: Subframe number
	cnfg_data.conf.range(37,37) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.SCID;
	cnfg_data.conf.range(53,38) = pdsch_l1_ctrl[cw_phy_index].pdsch_pdu.dlDmrsScramblingId;

	cnfg_data.dest = 0x0;
	cnfg_data.keep = 0xff;

	l1_ctrl_inst.num_rrh_trans +=1;
	cnfg_out.write(cnfg_data);

}
