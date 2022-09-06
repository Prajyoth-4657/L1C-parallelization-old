#include "l1_control.h"

extern ap_uint<64> dmrs_cfg_array[NUM_USER_CW];

void dmrs_pop_rrh(hls::stream<cnfg_axis_t> &cnfg_out, wire_type &state_ctrl_bbu_pop, wire_type cw_phy_index){

	#pragma HLS PIPELINE

	cnfg_axis_t cnfg_data;

	state_ctrl_bbu_pop = 0xdd;

	cnfg_data.conf = dmrs_cfg_array[cw_phy_index];

	cnfg_data.dest = 0x0;

	cnfg_out.write(cnfg_data);

}
