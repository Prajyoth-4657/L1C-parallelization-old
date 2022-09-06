#include "l1_control.h"

extern l1_ctrl_inst_t l1_ctrl_inst;
extern pdsch_l1_ctrl_t pdsch_l1_ctrl[NUM_USER_CW];

void l1_ctrl_bbu_calc_tb_params(){

CALCTBP:for(uint8_t i = 0; i<l1_ctrl_inst.n_users; i+=1){
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=1 max=48 avg=16

		// Calculate base graph value
		// TS 38.212 7.2.2
		// TS 38.214 5.1.3.2
		if(pdsch_l1_ctrl[i].pdsch_pdu.targetCodeRate <= 256 || pdsch_l1_ctrl[i].pdsch_pdu.TBSize <= 292){

			pdsch_l1_ctrl[i].pdsch_c_pdu.bg = 2;

			// Calculate cb size and number of cbs
			// TODO: Case where tbsize+24 is an exact multiple of 3816
			pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb = (pdsch_l1_ctrl[i].pdsch_pdu.TBSize+24)/3816 + 1;
		}
		else if(pdsch_l1_ctrl[i].pdsch_pdu.TBSize  <= 3824 && pdsch_l1_ctrl[i].pdsch_pdu.targetCodeRate <= 686){

			pdsch_l1_ctrl[i].pdsch_c_pdu.bg = 2;

			// Calculate cb size and number of cbs
			// TODO: Case where tbsize+24 is an exact multiple of 3816
			pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb = (pdsch_l1_ctrl[i].pdsch_pdu.TBSize+24)/3816 + 1;
		}
		else{
			pdsch_l1_ctrl[i].pdsch_c_pdu.bg = 1;

			// Calculate cb size and number of cbs
			// TODO: Case where tbsize+24 is an exact multiple of 8424
			pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb = (pdsch_l1_ctrl[i].pdsch_pdu.TBSize+24)/8424 + 1 ;
		}

		// Codeblock Size Calculation: TS 38.214 5.1.3.2
		pdsch_l1_ctrl[i].pdsch_c_pdu.k = 24 + (pdsch_l1_ctrl[i].pdsch_pdu.TBSize+24)/(ap_uint<14>)pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb;

		// Lifting size calculation Formula based TS 36.212 Sec. 5.2.2
		l1_ctrl_bbu_calc_z(pdsch_l1_ctrl[i].pdsch_c_pdu.bg, pdsch_l1_ctrl[i].pdsch_c_pdu.k, pdsch_l1_ctrl[i].pdsch_c_pdu.Z_ind);

		// Calculate G and E
		// TS 38.214 5.4.2
		// TODO: Fix all overflow/remainder problems
		pdsch_l1_ctrl[i].pdsch_c_pdu.G = 1 + (pdsch_l1_ctrl[i].pdsch_pdu.TBSize << 10)/(uint32_t)pdsch_l1_ctrl[i].pdsch_pdu.targetCodeRate;

#ifdef TB_DEBUG
		// DEBUG
		cout << endl << "-------" << endl;
		cout << "DEBUG::l1_ctrl_bbu_calc_tb_params | User Index: " << (int)i << endl;
		cout << "TB Size: " << pdsch_l1_ctrl[i].pdsch_pdu.TBSize << ", Rate: " << pdsch_l1_ctrl[i].pdsch_pdu.targetCodeRate << ", BG:" << pdsch_l1_ctrl[i].pdsch_c_pdu.bg << endl;
		cout << "Num CB: " << (int)pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb << ", CB Size: " << pdsch_l1_ctrl[i].pdsch_c_pdu.k << ", Z: " << pdsch_l1_ctrl[i].pdsch_c_pdu.Z_ind << endl;
//		cout << "nZ : " << dl_pdsch_params[i].kcb << endl;
		cout << "G: " << pdsch_l1_ctrl[i].pdsch_c_pdu.G << endl;
//		cout << "punct.ncb: "<< dl_pdsch_params[i].bbu_params.puncturing_params.ncb << endl;
		cout << "-------" << endl << endl;
#endif
	}

}

void l1_ctrl_bbu_calc_rm_params(){

CALCRM: for(uint8_t i = 0; i<l1_ctrl_inst.n_users; i+=1){
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=1 max=48 avg=16
		// Calculate E
		// TS 38.214 5.4.2
		// Fix all overflow/remainder problems
		uint32_t div_G = pdsch_l1_ctrl[i].pdsch_c_pdu.G/(uint32_t)(pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder) + 1;
		// E_shift is increased by 1 so that it's easier for the state machine in l1_ctrl_bbu_pop, system still works according to spec
		// TODO: Fix all overflow/remainder problems
		pdsch_l1_ctrl[i].pdsch_c_pdu.E_shift = pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb - (div_G%pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb);
		pdsch_l1_ctrl[i].pdsch_c_pdu.E_shift_1 = pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb - (div_G%pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb);
		pdsch_l1_ctrl[i].pdsch_c_pdu.E_0 = (pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder)*(pdsch_l1_ctrl[i].pdsch_c_pdu.G/(pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder*pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb));
		pdsch_l1_ctrl[i].pdsch_c_pdu.E_1 = (pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder)*(1 + (pdsch_l1_ctrl[i].pdsch_c_pdu.G/(pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder*pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb)));

		pdsch_l1_ctrl[i].pdsch_c_pdu.S_0 = (pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers)*(pdsch_l1_ctrl[i].pdsch_c_pdu.G/(pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder*pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb));
		pdsch_l1_ctrl[i].pdsch_c_pdu.S_1= (pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers)*(1 + (pdsch_l1_ctrl[i].pdsch_c_pdu.G/(pdsch_l1_ctrl[i].pdsch_pdu.nrOfLayers*pdsch_l1_ctrl[i].pdsch_pdu.qamModOrder*pdsch_l1_ctrl[i].pdsch_c_pdu.num_cb)));

		// cinit calculation
		// TS 36.211 Sec. 7.3.1.1
		// cinit = rnti << 15 + q << 14 + n_ID
		pdsch_l1_ctrl[i].pdsch_c_pdu.cinit = ((ap_uint<32>)pdsch_l1_ctrl[i].pdsch_pdu.RNTI << 15) + ((ap_uint<32>)(pdsch_l1_ctrl[i].pdsch_pdu.NrOfCodewords - 1 )<< 14) + pdsch_l1_ctrl[i].pdsch_pdu.dataScramblingId;

#ifdef TB_DEBUG
		// DEBUG
		cout << endl << "-------" << endl;
		cout << "DEBUG::l1_ctrl_bbu_calc_tb_params | User Index: " << (int)i << endl;
		cout << "E_shift+1: " << (int)pdsch_l1_ctrl[i].pdsch_c_pdu.E_shift << ", E_0: " << pdsch_l1_ctrl[i].pdsch_c_pdu.E_0 << ", E_1: " << pdsch_l1_ctrl[i].pdsch_c_pdu.E_1 << endl;
		cout << "cinit: " << pdsch_l1_ctrl[i].pdsch_pdu.RNTI << endl;
		cout << "-------" << endl << endl;
#endif
	}

}

void l1_ctrl_bbu_calc_z(ap_uint<2> bg, ap_uint<14> cb_size,ap_uint<6> &Z_ind){
// Binary search to get the value of the lifting factor
// Algorithm tested for all possible lifting factors for bg = 1 on MATLAB

	uint8_t cur_idx = 50>>2;  //** ?
	uint8_t hi_idx = 50;
	uint8_t lo_idx = 0;

	if(bg == 1){
		for(ap_uint<3> i = 0;i <= 5;i+=1){
		#pragma HLS PIPELINE
			if(LIFTING_SIZE_22Z[cur_idx]>cb_size){
				hi_idx = cur_idx;
				cur_idx = (lo_idx + cur_idx) >> 1;
			}
			else{
				lo_idx = cur_idx;
				cur_idx = (hi_idx + cur_idx) >> 1 ;
			}
		}
		if(cb_size <= LIFTING_SIZE_22Z[cur_idx]){
			Z_ind = cur_idx;
		}
		else{
			Z_ind = cur_idx + 1;
		}
	}
	else{
		for(ap_uint<3> i = 0;i <= 5;i+=1){
		#pragma HLS PIPELINE
			if(LIFTING_SIZE_10Z[cur_idx]>cb_size){
				hi_idx = cur_idx;
				cur_idx = (lo_idx + cur_idx) >> 1;
			}
			else{
				lo_idx = cur_idx;
				cur_idx = (hi_idx + cur_idx) >> 1 ;
			}
		}
		if(cb_size <= LIFTING_SIZE_10Z[cur_idx]){
			Z_ind = cur_idx;
		}
		else{
			Z_ind = cur_idx + 1;
		}
	}
}

void calc_u_int_params(hls::stream<cnfg_axis_t> &cnfg_out)
{
// Function to find the values for reading from the user interleaver.

	ap_uint<9> max_cb_sched;

	ap_uint<9> cur_phy_ind;
	ap_uint<14> num_samples;

	static ap_uint<16> write_addr_track[NUM_USER_CW];
	cnfg_axis_t cnfg_data;

	ap_uint<16> write_from, write_to;
	ap_uint<12> write_mask;
	ap_uint<4> sym_ind;

	max_cb_sched = l1_ctrl_inst.max_cb_sched;

L1:	for (ap_uint<9> i = 0; i < max_cb_sched; i = i+ 1) {
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=1 max=40 avg=20
		cur_phy_ind = l1_ctrl_inst.cw_phy_index_sched[i];

		// TODO: Have to increment E_shift by 1 before the following if condition runs
		// TODO: Have to add number of samples in a codeblock in pdsch_c_pdu - calculate at BBU.
		if (pdsch_l1_ctrl[cur_phy_ind].pdsch_c_pdu.E_shift == 0) {

			num_samples = pdsch_l1_ctrl[cur_phy_ind].pdsch_c_pdu.S_1;

		} else {

			num_samples = pdsch_l1_ctrl[cur_phy_ind].pdsch_c_pdu.S_0;
			pdsch_l1_ctrl[cur_phy_ind].pdsch_c_pdu.E_shift -= 1;
		}

		if (write_addr_track[cur_phy_ind] != 0) {
			write_from = write_addr_track[cur_phy_ind];
			sym_ind = write_addr_track[cur_phy_ind]/3072;
//			cout << "#Here" << write_addr_track[cur_phy_ind] << endl;
		} else {
			sym_ind = pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.StartSymbolIndex;
			write_from = sym_ind * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12;
		}

//		cout<< "#HERE," << pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbSize << "," << pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart << '\n';

L2:		while (num_samples > 0) {
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=2 max=10 avg=4
			write_mask = 0xfff;

//cout << "#HERE"<< pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.dlDmrsSymbPos[sym_ind] << "," << sym_ind << endl;
			cnfg_data.conf.range(31, 16) = write_from;
			write_to = sym_ind * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbSize * 12;
//			cout << "#Here " << write_from << ',' << num_samples;
			if (pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.dlDmrsSymbPos[sym_ind] == 1) {

				// TODO: Fix this according to DMRS config from standard:
				write_mask = 0xaaa;

				if (num_samples <= pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbSize * 6) {
					// TODO: Have to fix indexing here
					write_to = sym_ind * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12 + 2 * num_samples;
					num_samples = 0;
					write_addr_track[cur_phy_ind] = write_to;
					write_from = write_to;
				} else {

					num_samples -= (write_to - write_from) >> 1;
					write_addr_track[cur_phy_ind] = (sym_ind + 1) * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12;
					write_from = (sym_ind + 1) * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12;
				}

			} else {

				if (num_samples <= pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbSize * 12) {

					write_to = sym_ind * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12 + num_samples;
					num_samples = 0;
					write_addr_track[cur_phy_ind] = write_to;
					write_from = write_to;
				} else {

					num_samples -= (write_to - write_from);
					write_addr_track[cur_phy_ind] = (sym_ind + 1) * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12;
					write_from = (sym_ind + 1) * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12;
				}
			}

			cnfg_data.conf.range(15, 0) = write_to;
			cnfg_data.conf.range(43, 32) = write_mask;
			cnfg_data.dest = 0x2;
			cnfg_data.keep = 0xff;

//			cout << ','<< write_to << "," << num_samples << "\n";
			l1_ctrl_inst.num_rrh_trans +=1;
			cnfg_out.write(cnfg_data);

			// TODO: Put sym_ind*3072 in an array
//			write_from = (sym_ind + 1) * 3072 + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.BWPStart + pdsch_l1_ctrl[cur_phy_ind].pdsch_pdu.rbStart * 12;

			sym_ind = sym_ind+ 1;

		}

		// pdsch_l1_ctrl[cur_phy_ind].pdsch_c_pdu.;

	}

for (ap_uint<9> i = 0; i < max_cb_sched; i = i+ 1){
#pragma HLS PIPELINE
#pragma HLS loop_tripcount min=1 max=40 avg=20
	write_addr_track[i] = 0;
}


}
