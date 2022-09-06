//	Author: Raghav Subbaraman
//	Created: 25 June, 2019
//
//	Revision History:
//		10/08/2019: Created file (Raghav Subbaraman)
//
#include "fapi_uplane_sim.h"

void fapi_uplane_sim(
    hls::stream<data_axis_t> &data_out,
    ap_uint<8> &state_fapi_uplane_sim,
    ap_uint<1> sfn_clk,
    ap_uint<10> space_var
)
{
// Inputs
#pragma HLS INTERFACE axis port = data_out
#pragma HLS INTERFACE ap_none port = sfn_clk
#pragma HLS INTERFACE ap_none port = space_var
// Outputs
#pragma HLS INTERFACE ap_none port = state_fapi_uplane_sim
// TODO: MAKE the interfaces ap_vld
#pragma HLS interface ap_ctrl_none port=return

#pragma HLS PIPELINE

	static fapi_uplane_sim_state_t fapi_uplane_sim_state = SIM_INIT;
	static ap_uint<10> num_entries = 0;
	static ap_uint<10> idx = 0;
	static ap_uint<10> cnt = 0;
	static ap_uint<10> internal_space_var = 0;

	data_axis_t dout;

	state_fapi_uplane_sim = fapi_uplane_sim_state;

	switch (fapi_uplane_sim_state) {
	case SIM_INIT:

		if (sfn_clk) {
			fapi_uplane_sim_state = SIM_0;
			idx = 0;
			cnt = 0;
			num_entries = 610;
			internal_space_var = space_var;
		}

		break;

	case SIM_0:

		if (idx > num_entries - 2) {
			fapi_uplane_sim_state = SIM_INIT;
		}
		else {

			if ((idx == 0 || idx == 122 || idx == 244 || idx == 366 || idx == 488) && cnt == 0) {
				cnt = 0;
				fapi_uplane_sim_state = SIM_1;
			}
			else {

				cnt = 0;

				// Gather the data to send
				dout.tdata.range(63, 0) = data_vec[idx];
				dout.tdata.range(127, 64) = data_vec[idx + 1];
				dout.tuser = 0x0;

				if ( (idx == 120) || (idx == 242) || (idx == 364) || (idx == 486) || (idx == 608) ) {

					dout.tlast = 1;
					dout.tkeep = 0x70;

				} else {

					dout.tlast = 0;
					dout.tkeep = 0x80;

				}



				data_out.write(dout);

				idx += 2;
			}

		}

		break;

	case SIM_1:

		if (cnt >= internal_space_var) {
			fapi_uplane_sim_state = SIM_0;
		}
		else {
			cnt += 1;
		}


		break;

	default:
		fapi_uplane_sim_state = SIM_INIT;
	}


}

