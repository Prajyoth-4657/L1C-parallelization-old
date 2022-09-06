#include "l1_control.h"

extern l1_ctrl_inst_t l1_ctrl_inst;
extern ap_uint<64> dmrs_cfg_array[NUM_USER_CW];

void ecpri_rx(hls::stream<ecpri_axis> &ecpri_in, wire_type &state_ecpri_rx, bit_wire sfn_ctrl_clk, bool &ecpri_done)
{

//#pragma HLS PIPELINE

	ecpri_axis data_in;
//	data_axis dout;
	static ap_uint<8> cur_user = 0;
	static bit_wire prev_sfn_ctrl_clk;
	static enum ecpri_RX_STATE ecpri_rx_state = ecpri_RX_INIT;
	static ap_uint<9> cur_sched_ind = 0;
//	static ecpri_rx_instance_par ecpri_rx_instance_params;
	state_ecpri_rx = ecpri_rx_state;

	switch (ecpri_rx_state) {

	case ecpri_RX_INIT:

		ecpri_rx_state = CTRL_ecpri_RCV_0;
		ecpri_done = 0;

		break;

	case ecpri_RX_WAIT:

		ecpri_rx_state = CTRL_ecpri_RCV_0;
		cur_sched_ind = 0;
		cur_user = 0;
		ecpri_done = 0;

		break;

	case CTRL_ecpri_RCV_0 :
		if (!ecpri_in.empty()) {
			data_in =  ecpri_in.read();
			// nan
			l1_ctrl_inst.n_users = data_in.data.range(7, 0);
//			// nan
//			l1_ctrl_inst.param_calc_tb_dirt_bit = data_in.data.range(8, 8);
//			// nan
//			l1_ctrl_inst.param_calc_rm_dirt_bit = data_in.data.range(9, 9);
			// nan
			l1_ctrl_inst.max_cb_sched = data_in.data.range(18, 10);
			// nan
			l1_ctrl_inst.cur_phy_sched = data_in.data.range(27, 19);
			// nan
			// l1_ctrl_inst.cw_phy_index_sched[512] = data_in.data.range(35,28);


			ecpri_rx_state = CTRL_ecpri_RCV_1 ;
			ecpri_done = 0;
		}
		break;

	case CTRL_ecpri_RCV_1:


		ecpri_rx_state = CTRL_ecpri_RCV_2;

		ecpri_done = 0;

		break;

	case CTRL_ecpri_RCV_2:

		if (cur_sched_ind < l1_ctrl_inst.n_users) {

			data_in =  ecpri_in.read();

			dmrs_cfg_array[cur_sched_ind] = data_in.data;

			cur_sched_ind = cur_sched_ind + 1;

			ecpri_done = 0;

		} else {

			ecpri_rx_state = ecpri_RX_WAIT;
			ecpri_done = 1;
		}

		break;
	default:
		ecpri_rx_state = ecpri_RX_INIT;

	}

}


