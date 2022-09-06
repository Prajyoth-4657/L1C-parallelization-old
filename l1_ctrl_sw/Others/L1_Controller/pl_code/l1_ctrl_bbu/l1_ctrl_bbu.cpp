//	Author: Raghav Subbaraman
//	Created: 19 May, 2019
//
//	Revision History:
//		19/05/2019: Created file (Raghav Subbaraman)
//
#include "l1_control.h"

l1_ctrl_inst_t l1_ctrl_inst;
pdsch_l1_ctrl_t pdsch_l1_ctrl[NUM_USER_CW];

void l1_ctrl_bbu(
		hls::stream<fapi_axis> &fapi_in,
		hls::stream<cnfg_axis_t> &cnfg_out,
		hls::stream<cnfg_axis_t> &rrh_out,
		wire_type &state_fapi_rx,
		wire_type &state_ctrl_bbu_pop,
		wire_type &state_l1_ctrl,
		bit_wire sfn_clk,
		bit_wire &bbu_reset
		)
{
// Inputs
#pragma HLS INTERFACE axis port = fapi_in
#pragma HLS INTERFACE ap_none port = sfn_clk
// Outputs
#pragma HLS INTERFACE ap_vld port = bbu_reset
#pragma HLS INTERFACE axis port = cnfg_out
#pragma HLS INTERFACE axis port = rrh_out
#pragma HLS INTERFACE ap_none port = state_ctrl_bbu_pop
#pragma HLS INTERFACE ap_none port = state_fapi_rx
// TODO: MAKE the interfaces ap_vld
#pragma HLS interface ap_ctrl_none port=return

	static L1_CTRL_STATE_t l1_ctrl_state = PU_INIT;

	wire_type cw_phy_index = 0;    //** ? EVERY WHERE,  //UUU
	bool phy_sched_up,fapi_done;
	cnfg_axis_t data_out;

	state_l1_ctrl = l1_ctrl_state;

	switch(l1_ctrl_state){
	case PU_INIT:

		// TODO: Maybe some init?

		l1_ctrl_inst.cur_phy_sched = 0;
		l1_ctrl_inst.max_cb_sched = 4;             //No. of code blocks  //changes
		l1_ctrl_inst.cw_phy_index_sched[0] = 0;
		l1_ctrl_inst.cw_phy_index_sched[1] = 0;
		l1_ctrl_inst.cw_phy_index_sched[2] = 0;
		l1_ctrl_inst.cw_phy_index_sched[3] = 0;
		l1_ctrl_inst.cw_phy_index_sched[4] = 0;

		l1_ctrl_inst.num_rrh_trans = 0;     //KKKK

		if(sfn_clk){
			bbu_reset = 1;
			l1_ctrl_state = PU_FAPI_RX;
			}

		break;
	case PU_FAPI_RX:

		// Reads all the control information from the FAPI CONF read module
//		fapi_sim_rx(fapi_in,state_fapi_rx,sfn_clk,fapi_done);
		fapi_rx(fapi_in,state_fapi_rx,sfn_clk,fapi_done);

		if(fapi_done){

			l1_ctrl_state = PU_CALC_0;
		}

		break;


	case PU_CALC_0:

		// Do some minor calculations to start calculating PHY schedule
		// Consider doing these minor calculations insid the fapi_rx function itself

		l1_ctrl_bbu_calc_tb_params();

		l1_ctrl_state = PU_SCHED;

		break;

	case PU_SCHED:

		// Calculate the PHY schedule - maybe suboptimal, but do something
		// TODO: Function that calculates the PHY schedule


		// Sending some information to the RRH:
		data_out.conf.range(7,0) = l1_ctrl_inst.n_users;
		data_out.conf.range(18,10) = l1_ctrl_inst.max_cb_sched;
		data_out.conf.range(27,19) = l1_ctrl_inst.cur_phy_sched;

		rrh_out.write(data_out);


		l1_ctrl_state = PU_CALC_1;

		break;
	case PU_CALC_1:

		// Calculate all other parameters that may be required for PHY
		// Consider clubbing with the above/below function

		l1_ctrl_bbu_calc_rm_params();

		l1_ctrl_state = PU_CALC_2;
		break;

	case PU_CALC_2:

		// This is for DMRS
		if (l1_ctrl_inst.cur_phy_sched < l1_ctrl_inst.n_users) {
			dmrs_pop(rrh_out, state_ctrl_bbu_pop, l1_ctrl_inst.cur_phy_sched);
			l1_ctrl_inst.cur_phy_sched = l1_ctrl_inst.cur_phy_sched+1;
		} else {
			l1_ctrl_inst.cur_phy_sched = 0;
			// This resets the PDSCH user interleaver:
			l1_ctrl_state = PU_CALC_3;
		}

		break;

	case PU_CALC_3:

		calc_u_int_params(rrh_out);


		l1_ctrl_state = PU_CALC_4;


		break;

	case PU_CALC_4:

		// Let's send to RRH!

		data_out.conf.range(15,0) = 0xffff;

		data_out.conf.range(31,16) = 0x0000;

		data_out.conf.range(47,32) = l1_ctrl_inst.num_rrh_trans + 1;

		data_out.conf.range(63,48) = 0x0;

		data_out.keep = 0xff;

		data_out.dest = 0x7;

		cnfg_out.write(data_out);

		l1_ctrl_state = PU_POP;


		break;

	case PU_POP:

		// Populate CTRL FIFOs
		if(l1_ctrl_inst.cur_phy_sched < l1_ctrl_inst.max_cb_sched){
			l1_ctrl_bbu_pop(cnfg_out,state_ctrl_bbu_pop,l1_ctrl_inst.cw_phy_index_sched[l1_ctrl_inst.cur_phy_sched],phy_sched_up);
			if(phy_sched_up)
				l1_ctrl_inst.cur_phy_sched +=1;
		}
		else{
			l1_ctrl_state = PU_INIT;
		}

		break;
	default:
		l1_ctrl_state = PU_INIT;
	}


}