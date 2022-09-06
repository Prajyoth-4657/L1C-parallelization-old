//	Author: Raghav Subbaraman
//	Created: 25 June, 2019
//
//	Revision History:
//		25/06/2019: Created file (Raghav Subbaraman)
//
#include "../l1_ctrl_bbu/l1_control.h"
#include "l1_control_sim.h"

void l1_ctrl_sim(
		hls::stream<data_axis_t> &data_out,
		hls::stream<cnfg_axis_t> &cnfg_out,
		wire_type &state_l1_ctrl_sim,
		bit_wire enable,
		ap_uint<10> space_var
		)
{
// Inputs
#pragma HLS INTERFACE axis port = data_out
#pragma HLS INTERFACE ap_none port = enable
#pragma HLS INTERFACE ap_none port = space_var
// Outputs
#pragma HLS INTERFACE axis port = cnfg_out
#pragma HLS INTERFACE ap_none port = state_l1_ctrl_sim
// TODO: MAKE the interfaces ap_vld

	#pragma HLS PIPELINE

	static l1_ctrl_sim_state_t l1_ctrl_sim_state = SIM_INIT;
	static ap_uint<10> num_entries =0;
	static ap_uint<10> idx = 0;
	static ap_uint<10> cnt = 0;
	static ap_uint<10> internal_space_var = 0;

	data_axis_t dout;
	cnfg_axis_t cfgout;

	state_l1_ctrl_sim = l1_ctrl_sim_state;

	switch(l1_ctrl_sim_state){
	case SIM_INIT:

		if(enable){
			l1_ctrl_sim_state = SIM_CTRL;
			idx = 0;
			cnt = 0;
			num_entries = 660;
			internal_space_var = space_var;
		}
		
		break;

	case SIM_CTRL:

		if(idx >= 5){
			l1_ctrl_sim_state = SIM_0;
			idx = 0;
		}
		else if(idx<=4){
			idx+=1;
			cfgout.conf = 0x2e0000400f;
			cnfg_out.write(cfgout);
		}
		else{
			idx+=1;
		}

	break;

	case SIM_0:

		if(idx > num_entries-2){
			l1_ctrl_sim_state = SIM_INIT;
		}
		else{

			if((idx==0||idx ==132||idx==264||idx==396||idx==528) && cnt == 0){
				cnt = 0;
				l1_ctrl_sim_state = SIM_1;
			}
			else{

				cnt = 0;

				// Gather the data to send
				dout.tdata.range(63,0) = data_vec[idx];
				dout.tdata.range(127,64) = data_vec[idx+1];
				data_out.write(dout);

				idx+=2;
			}

		}
		
		break;

	case SIM_1:

		if(cnt>=internal_space_var){
			l1_ctrl_sim_state = SIM_0;
		}
		else{
			cnt+=1;
		}


	break;

	default:
		l1_ctrl_sim_state = SIM_INIT;
	}


}

