

#include "sdfec_aux.h"

void sdfec_aux(hls::stream<stream_axi_t> &data_in, hls::stream<cnfg_axi_t> &cnfg_in, hls::stream<stream_axi_t> &data_out){

// Inputs
#pragma HLS INTERFACE axis port = data_in
#pragma HLS INTERFACE axis port = cnfg_in
// Outputs
#pragma HLS INTERFACE axis port = data_out
	#pragma HLS interface ap_ctrl_none port=return

	#pragma HLS PIPELINE enable_flush

	static sdfec_state_t aux_state = INIT;

	stream_axi_t din;
	cnfg_axi_t cin;

	static user_data_t cur_usr = 0x0;

	switch(aux_state){


		case INIT:

			cur_usr = 0x0;

			aux_state = CNFG;

		break;

		case CNFG:

			cnfg_in.read(cin);

			cur_usr.range(7,0) = cin.tdata.range(31,24);
			cur_usr.range(15,8) = 0x0;

			aux_state = PASS;


		break;


		case PASS:

			data_in.read(din);

			din.tuser = cur_usr;

			data_out.write(din);

			if(din.tlast){
				aux_state = CNFG;
			}


		break;



	}



}

