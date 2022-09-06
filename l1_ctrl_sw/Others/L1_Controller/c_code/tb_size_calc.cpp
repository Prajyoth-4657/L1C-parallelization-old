
// Code to calculate phy param
// Author: Raghav Vaidyanathan Subbaraman
// 5G Testbed Project, Indian Insitute of Technology Madras
// All variables and algorithm from 38.214
// Look up https://www.sharetechnote.com/html/5G/5G_MCS_TBS_CodeRate.html


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <time.h>
using namespace std;


typedef struct tb_params_t{

	uint32_t tb_size;
	uint16_t C;
	uint8_t BG;
	uint16_t cb_size;

	// Lifting size
	uint16_t Z;
	
	// G is the number of bits after tb has been channel coded
	// MATLAB says G = ceil(tb_size/rate)
	uint32_t G;
	// Number of bits per codeblock at output of rate match block
	uint16_t E_0;
	uint16_t E_1;
	// The last cb index where E_0 should be used
	uint16_t E_shift;

}tb_params_t;

const uint16_t TB_SIZE_U3840[93] = {24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,152,160,168,176,184,192,208,224,240,256,272,
	288,304,320,336,352,368,384,408,432,456,480,504,528,552,576,608,640,672,704,736,768,808,848,888,928,984,1032,1064,1128,1160,1192,
	1224,1256,1288,1320,1352,1416,1480,1544,1608,1672,1736,1800,1864,1928,2024,2088,2152,2216,2280,2408,2472,2536,2600,2664,2728,2792,
	2856,2976,3104,3240,3368,3496,3624,3752,3824};

const uint16_t LIFTING_SIZE_Z[51] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		18, 20, 22, 24, 26, 28, 30, 32, 36, 40, 44, 48, 52, 56, 60, 64, 72, 80,
		88, 96, 104, 112, 120, 128, 144, 160, 176, 192, 208, 224, 240, 256, 288,
		320, 352, 384 };

const uint16_t LIFTING_SIZE_10Z[51] = {20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,
		180,200,220,240,260,280,300,320,360,400,440,480,520,560,600,640,720,800,
		880,960,1040,1120,1200,1280,1440,1600,1760,1920,2080,2240,2400,2560,2880,
		3200,3520,3840};

const uint16_t LIFTING_SIZE_22Z[51] = {44,66,88,110,132,154,176,198,220,242,264,286,308,330,352,
		396,440,484,528,572,616,660,704,792,880,968,1056,1144,1232,1320,1408,1584,1760,
		1936,2112,2288,2464,2640,2816,3168,3520,3872,4224,4576,4928,5280,5632,6336,
		7040,7744,8448};

typedef struct tb_calc_in_t{

	// Variables
	uint16_t num_sc_rb; // Number of sub carriers in an RB
	uint16_t num_sh_sym ; // Number of symbols of PDSCH allocation in this slot
	uint16_t num_dmrs_prb; // Number of PRBS allocated to DMRS - Refer DCI 1_0 or DCI 1_1
	uint16_t num_oh_prb; // Number of overhead PRBs, RRC Param- Xoh-PDSCH. Valid val - 0,6,12,8

	uint16_t num_prb; // Total number of allocated PRBS for UE

	// Mod and coding params:
	uint16_t Q_m; // Modulation order
	uint16_t v_l; // Number of layers
	uint16_t R; // Rate. Actual rate is R/1024

}tb_calc_in_t;


void calc_tb_params(tb_calc_in_t tb_calc_in,tb_params_t &tb_params){

	uint32_t N_d_re = tb_calc_in.num_sc_rb*tb_calc_in.num_sh_sym - tb_calc_in.num_dmrs_prb - tb_calc_in.num_oh_prb;
	uint32_t N_re,N_info,two_p_n,N_d_info;

	uint16_t n;

	if(N_d_re < 156)
		N_re = N_d_re * tb_calc_in.num_prb;
	else
		N_re = N_d_re * tb_calc_in.num_prb;

	N_info = (N_re * tb_calc_in.R * tb_calc_in.Q_m * tb_calc_in.v_l) >> 10;

	if(N_info <= 3824){
		// TODO: Step 3

		n = floor(log2(N_info))-6;

		if(n<3)
			n = 3;

		two_p_n = 1 << n;

		N_d_info = two_p_n * floor(float(N_info)/float(two_p_n));

		// Binary search for the nearest tb_size
		uint8_t cur_idx = 92>>2;
		uint8_t hi_idx = 92;
		uint8_t lo_idx = 0;

		for(uint8_t i = 0;i <= 7;i+=1){
			if(TB_SIZE_U3840[cur_idx]>N_d_info){

				hi_idx = cur_idx;
				cur_idx = (lo_idx + cur_idx) >> 1;

			}
			else{

				lo_idx = cur_idx;
				cur_idx = (hi_idx + cur_idx) >> 1 ;

			}
		}

		if(N_d_info <= TB_SIZE_U3840[cur_idx]){

			tb_params.tb_size = TB_SIZE_U3840[cur_idx];

		}
		else{
			tb_params.tb_size = TB_SIZE_U3840[cur_idx+1];
		}

		tb_params.C = 1;

	}
	else{

		n = floor(log2(N_info-24))-5;

		two_p_n = 1 << n;

		N_d_info = two_p_n * round( float(N_info-24)/float(two_p_n));

		if(N_d_info<3840)
			N_d_info = 3840;

		if(tb_calc_in.R <= 256){

			tb_params.C = ceil(float(N_d_info+24)/3816);

			tb_params.tb_size = 8*tb_params.C*ceil(float(N_d_info+24)/float(8*tb_params.C))-24;

		}
		else{
			if(N_d_info > 8424){

				tb_params.C = ceil(float(N_d_info+24)/8424);

				tb_params.tb_size = 8*tb_params.C*ceil(float(N_d_info+24)/float(8*tb_params.C))-24;

			}
			else{

				tb_params.C = 1;

				tb_params.tb_size = 8*ceil(float(N_d_info+24)/8)-24;

			}
		}

	}

	tb_params.cb_size = (tb_params.tb_size + 24)/tb_params.C;

	// Base Graph Calculation

	if(tb_calc_in.R <= 256 || tb_params.tb_size <= 292){

		tb_params.BG = 2;

	}
	else if(tb_params.tb_size <= 3824 && tb_calc_in.R <= 686){

		tb_params.BG = 2;

	}
	else{
		tb_params.BG = 1;
	}

	// Rate Matching Parameter Calculation

	tb_params.G = ceil(float(tb_params.tb_size)/float(float(tb_calc_in.R)/1024));
	uint32_t div_G = ceil(tb_params.G/float(tb_calc_in.v_l*tb_calc_in.Q_m));
	tb_params.E_shift = tb_params.C - (div_G%tb_params.C) - 1;
	tb_params.E_0 = tb_calc_in.v_l*tb_calc_in.Q_m*floor(tb_params.G/float(tb_calc_in.v_l*tb_calc_in.Q_m*tb_params.C));
	tb_params.E_1 = tb_calc_in.v_l*tb_calc_in.Q_m*ceil(tb_params.G/float(tb_calc_in.v_l*tb_calc_in.Q_m*tb_params.C));


	// Calculate the lifting factor:

	uint8_t cur_idx = 50>>2;
	uint8_t hi_idx = 50;
	uint8_t lo_idx = 0;

	if(tb_params.BG == 1){

		for(uint8_t i = 0;i <= 5;i+=1){
			if(LIFTING_SIZE_22Z[cur_idx]>tb_params.cb_size){

				hi_idx = cur_idx;
				cur_idx = (lo_idx + cur_idx) >> 1;

			}
			else{

				lo_idx = cur_idx;
				cur_idx = (hi_idx + cur_idx) >> 1 ;

			}

		}

		if(tb_params.cb_size <= LIFTING_SIZE_22Z[cur_idx]){

			tb_params.Z = LIFTING_SIZE_Z[cur_idx];

		}
		else{
			tb_params.Z = LIFTING_SIZE_Z[cur_idx+1];
		}

	}
	else{

		for(uint8_t i = 0;i <= 5;i+=1){
			if(LIFTING_SIZE_10Z[cur_idx]>tb_params.cb_size){

				hi_idx = cur_idx;
				cur_idx = (lo_idx + cur_idx) >> 1;

			}
			else{

				lo_idx = cur_idx;
				cur_idx = (hi_idx + cur_idx) >> 1 ;

			}

		}

		if(tb_params.cb_size <= LIFTING_SIZE_10Z[cur_idx]){

			tb_params.Z = LIFTING_SIZE_Z[cur_idx];

		}
		else{
			tb_params.Z = LIFTING_SIZE_Z[cur_idx+1];
		}

	}

}

void read_calc_in(tb_calc_in_t &tb_calc_in){

	cin >> tb_calc_in.num_sh_sym;
	cin >> tb_calc_in.num_dmrs_prb;
	cin >> tb_calc_in.num_oh_prb;
	cin >> tb_calc_in.num_prb;
	cin >> tb_calc_in.R;
	cin >> tb_calc_in.Q_m;
	cin >> tb_calc_in.v_l;

}

void print_tb_params(tb_params_t tb_params){

	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "TBS: " << tb_params.tb_size << ", C: " << tb_params.C << ", cb_size: " << tb_params.cb_size << endl;
	cout << "BG: " << int(tb_params.BG) << ", Z: " << tb_params.Z << endl;
	cout << "G: " << tb_params.G << ", E_shift: "<< tb_params.E_shift << ", E_0: " << tb_params.E_0 << ", E_1: " << tb_params.E_1 << endl;
	cout << "-------------------------------------------------------------------------------" << endl << endl;

}

int main(){

	// Bookkeeping params:
	time_t start_time, end_time;
	int n_iter = 10000;
	int n_config = 0; // Number of different UE configurations in input

	tb_calc_in_t tb_calc_in;
	tb_params_t tb_params;

	// // Setting input parameters 
	tb_calc_in.num_sc_rb = 12; // Number of sub carriers in an RB

	// // In config input file, write the params in the same order as below:
	// tb_calc_in.num_sh_sym = 8; // Number of symbols of PDSCH allocation in this slot
	// tb_calc_in.num_dmrs_prb = 6; // Number of RE per PRB for DMRS
	// tb_calc_in.num_oh_prb = 0; // Number of overhead PRBs, RRC Param- Xoh-PDSCH. Valid val - 0,6,12,8

	// tb_calc_in.num_prb = 50; // Total number of allocated PRBS for UE

	// // Mod and coding params:
	// tb_calc_in.R = 885; // Rate. Actual rate is R/1024
	// tb_calc_in.Q_m = 8; // Modulation order
	// tb_calc_in.v_l = 2; // Number of layers

	cin >> n_config;

	for(int ind = 0; ind< n_config; ind++){

		read_calc_in(tb_calc_in);

		start_time = clock();

		for(int i =0; i<n_iter; i+=1)
			calc_tb_params(tb_calc_in,tb_params);

		end_time = clock();

		cout << "Config Index " << ind << " | Time elapsed: " << float(end_time - start_time)/float(CLOCKS_PER_SEC*n_iter) << endl;

		print_tb_params(tb_params);


	}

	

	return 0;
}
// INFO: [SIM 2] *************** CSIM start ***************
// INFO: [SIM 4] CSIM will launch GCC as the compiler.
//    Compiling ../../../l1_ctrl_bbu_tb.cpp in debug mode
//    Compiling ../../../l1_ctrl_bbu.cpp in debug mode
//    Compiling ../../../l1_ctrl_bbu_calc.cpp in debug mode
//    Compiling ../../../l1_ctrl_bbu_fapi_rx.cpp in debug mode
//    Compiling ../../../l1_ctrl_bbu_pop.cpp in debug mode
//    Generating csim.exe

// -------
// DEBUG::l1_ctrl_bbu_calc_tb_params | User Index: 0
// TB Size: 38936, Rate: 885, BG:1
// Num CB: 5, CB Size: 7816, Z: 384
// nZ : 8448
// G: 45052
// punct.ncb: 25344
// -------

// -------
// DEBUG::l1_ctrl_bbu_calc_tb_params | User Index: 0
// E_shift+1: 3, E_0: 9008, E_1: 9016
// cinit: 40435712
// Bits Padded per cb: 632, Useful bits after LDPC: 23944
// -------

// Reading Conf Out: 0x7ffc01805e88/0x21/0x0/0x1

// Reading Conf Out: 0x0c601808401e88/0x35/0x0/0x1

// Reading Conf Out: 0x601023305d88/0x2f/0x0/0x1

// Reading Conf Out: 0x7ff000023308/0x24/0x0/0x1

// Reading Conf Out: 0x7ffd02690000/0x20/0x0/0x1

// Reading Conf Out: 0x7ffd00000008/0x20/0x0/0x1

// Reading Conf Out: 0x7ffc01805e88/0x21/0x0/0x1

// Reading Conf Out: 0x0c601808401e88/0x35/0x0/0x1

// Reading Conf Out: 0x601023305d88/0x2f/0x0/0x1

// Reading Conf Out: 0x7ff000023308/0x24/0x0/0x1

// Reading Conf Out: 0x7ffd00000008/0x20/0x0/0x1

// Reading Conf Out: 0x7ffc01805e88/0x21/0x0/0x1

// Reading Conf Out: 0x0c601808401e88/0x35/0x0/0x1

// Reading Conf Out: 0x601023305d88/0x2f/0x0/0x1

// Reading Conf Out: 0x7ff000023308/0x24/0x0/0x1

// Reading Conf Out: 0x7ffd00000008/0x20/0x0/0x1

// Reading Conf Out: 0x7ffc01805e88/0x21/0x0/0x1

// Reading Conf Out: 0x0c601808401e88/0x35/0x0/0x1

// Reading Conf Out: 0x601023385d88/0x2f/0x0/0x1

// Reading Conf Out: 0x7ff000023388/0x24/0x0/0x1

// Reading Conf Out: 0x7ffd00000008/0x20/0x0/0x1

// Reading Conf Out: 0x7ffc01805e88/0x21/0x0/0x1

// Reading Conf Out: 0x0c601808401e88/0x35/0x0/0x1

// Reading Conf Out: 0x601023385d88/0x2f/0x0/0x1

// Reading Conf Out: 0x7ff000023388/0x24/0x0/0x1

// Reading Conf Out: 0x7ffd00000008/0x20/0x0/0x1

// Count =  26
// INFO: [SIM 1] CSim done with 0 errors.
// INFO: [SIM 3] *************** CSIM finish ***************

// ap_uint<6> idx = 0;
// ap_uint<6> l = 0, r = 50;
// 		ap_uint<1> found = 0;
// 		LIFT: for (ap_uint<3> i = 0; i < 6; i++) {
// #pragma HLS UNROLL
// 			if (found == 0) {
// 				ap_uint<6> m = l + ((r - l) >> 1);
// 				if (LIFTING_SIZE_SET[m] == z) {
// 					idx = m;
// 					found = 1;
// 				} else if (LIFTING_SIZE_SET[m] < z) {
// 					l = m + 1;
// 				} else {
// 					r = m - 1;
// 				}
// 			}
// 		}