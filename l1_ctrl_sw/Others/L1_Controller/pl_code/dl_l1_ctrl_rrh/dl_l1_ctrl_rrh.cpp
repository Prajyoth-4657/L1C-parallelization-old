//	Author: Raghav Subbaraman
//	Created: 19 May, 2019
//
//	Revision History:
//		19/05/2019: Created file (Raghav Subbaraman)
//
#include "l1_control.h"

l1_ctrl_inst_t l1_ctrl_inst;
ap_uint<64> dmrs_cfg_array[NUM_USER_CW];

void l1_ctrl_rrh_pdsch(
    hls::stream<ecpri_axis> &ecpri_in,
    hls::stream<cnfg_axis_t> &cnfg_out,
    wire_type &state_ecpri_rx,
    wire_type &state_ctrl_rrh_pop,
    wire_type &state_l1_ctrl,
    bit_wire sfn_clk,
	bit_wire &rrh_reset
)
{
// Inputs
#pragma HLS INTERFACE axis port = ecpri_in
#pragma HLS INTERFACE ap_none port = sfn_clk
// Outputs
#pragma HLS INTERFACE axis port = cnfg_out
#pragma HLS INTERFACE ap_none port = state_ctrl_rrh_pop
#pragma HLS INTERFACE ap_none port = state_ecpri_rx
#pragma HLS INTERFACE ap_none port = state_l1_ctrl
#pragma HLS INTERFACE ap_vld port = rrh_reset
// TODO: MAKE the interfaces ap_vld
#pragma HLS interface ap_ctrl_none port=return
//#pragma HLS array_partition variable = l1_ctrl_inst.cw_phy_index_sched

	static L1_CTRL_STATE_t l1_ctrl_state = PU_INIT;
	static wire_type phy_ind_count = 0;

	wire_type cw_phy_index = 0;
	bool phy_sched_up, ecpri_done;

	cnfg_axis_t cnfg;
	ecpri_axis ecpri_data;

	state_l1_ctrl = l1_ctrl_state;

	switch (l1_ctrl_state) {
	case PU_INIT:

		// TODO: Maybe some init?
		rrh_reset = 0;
		phy_ind_count = 0;

		if (sfn_clk)
			l1_ctrl_state = PU_ecpri_RX;

		break;
	case PU_ecpri_RX:

		// Reads all the control information from the ecpri CONF read module
		ecpri_rx(ecpri_in, state_ecpri_rx, sfn_clk, ecpri_done);

		if (ecpri_done){
			l1_ctrl_state = PU_CALC_0;
			rrh_reset = 0;
		}

		break;

	case PU_CALC_0:

		// Calculations for the User Interleaver Module
		rrh_reset = 0;

		l1_ctrl_state = PU_POP;

		break;

	case PU_POP:

		// Populate CTRL FIFOs

		// DMRS Pop
		if (phy_ind_count < l1_ctrl_inst.n_users) {
			dmrs_pop_rrh(cnfg_out, state_ctrl_rrh_pop, phy_ind_count);
			phy_ind_count = phy_ind_count+1;
		} else if (phy_ind_count < NUM_USER_CW) {

			cnfg.conf = 0x0;
			cnfg.dest = 0x0;

			cnfg_out.write(cnfg);

			phy_ind_count = phy_ind_count+1;

		} else {
			phy_ind_count = 0;
			// This resets the PDSCH user interleaver:
			rrh_reset = 1;
			l1_ctrl_state = PU_POP_1;
		}
		// Now put all CSIRS
		// TODO: CSIRS
		break;

	case PU_POP_1:

		// PDSCH User Interleaver read config is written in advance
		cnfg.conf = 0x0000a80004b0;
		cnfg.dest = 0x1;

		cnfg_out.write(cnfg);

		l1_ctrl_state = PU_POP_2;
	
		break;

	case PU_POP_2:

		// Now the write configs are written.

		if(l1_ctrl_inst.n_users >0 ){
			ecpri_in.read(ecpri_data);

			cnfg.conf = ecpri_data.data;
			cnfg.dest = 0x2;

			cnfg_out.write(cnfg);

			if(ecpri_data.last){
				l1_ctrl_state = PU_INIT;
			}

		} else {

			l1_ctrl_state = PU_INIT;
		}

		break;

	default:
		l1_ctrl_state = PU_INIT;
	}


}

