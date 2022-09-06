//Author: Raghav Subbaraman
//Date Created: 21 May, 2019

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "l1_control.h"

//int main()
//{
//
//	// File IO Variables
//	int i;
//	int count=0;
//	char a1[1000]; // String to store one line from csv file
//	FILE *fp_in,*fp_out; // File pointers for IO
//
//	// Test IO file names
////	char in_file_name[]="/home/jaswanthi/Desktop/eCPRI_VIVADO_1/testdata/v2_test_files/ecpri_iq_msg_gen.csv";
////	char out_file_name[]="/home/jaswanthi/Desktop/eCPRI_VIVADO_1/testdata/v2_test_files/ecpri_iq_msg_gen_out.csv";
////
////	fp_in = fopen(in_file_name, "r");
////	if (fp_in == NULL){
////		  perror("Error while opening the input file.\n");
////		  exit(EXIT_FAILURE);
////	   }
////	fgets(a1, 1000, fp_in);
////
////	fp_out = fopen(out_file_name, "w");
////	if(fp_out == NULL){
////		perror("Error while opening the output file.\n");
////		  exit(EXIT_FAILURE);
////	}
//
//	// Interfaces
//	hls::stream<fapi_axis> fapi_in;
//	hls::stream<cnfg_axis_t> cnfg_out;
//	bit_wire sfn_clk;
//	bit_wire sfn_ctrl_clk;
//
//	// Input Valid Signals
////	int cnfg_in_valid,data_in_valid;
//
//	// Input Data Variables - Read from csv file
//	fapi_axis fapi_msg_in;
//
//	// Output Data Variables - Written to csv file
//	// TODO: write code to write to csv file
//	cnfg_axis_t cnfg_pop_out;
//	wire_type state_fapi_rx;
//	wire_type state_ctrl_bbu_pop;
//	wire_type state_l1_ctrl;
//
//	// Output print flags
//	int state_out_flag = 0, data_read_flag = 1;
//
//	eth_data_t print_data;
//
//	for(i = 0; i<1000; i++){
//
//		fapi_msg_in.data = 0;
////		fapi_msg_in.user = 0;
//		fapi_msg_in.last = 0;
//		if(i<6*48)
//			fapi_in.write(fapi_msg_in);
//
//		if(i<5)
//			sfn_clk = 1;
//		else
//			sfn_clk = 0;
//		sfn_ctrl_clk = 0;
//
//		l1_ctrl_bbu(fapi_in, cnfg_out, state_fapi_rx, state_ctrl_bbu_pop,state_l1_ctrl,sfn_clk,sfn_ctrl_clk);
//
//		if(state_out_flag)
//			cout << "state_fapi_rx: " << (int)state_fapi_rx << ", state_ctrl_bbu_pop: " << (int)state_ctrl_bbu_pop << ", state_l1_ctrl: " << (int)state_l1_ctrl << endl;
//
//		if((!cnfg_out.empty())){
//			cnfg_pop_out = cnfg_out.read();
//			count++;
//			print_data = cnfg_pop_out.conf;
//
//			if(data_read_flag)
//				cout  <<"Reading Conf Out: "<< std::hex << print_data << "/" << cnfg_pop_out.dest << "/" << cnfg_pop_out.user <<"/" << cnfg_pop_out.last<< endl;
//
//			cout<< endl;
//		}
//
//
//	}
//
////	fclose(fp_in);
//
//	printf("Count =  %d\n", count);
//	return 0;
//}


int main()
{

	// File IO Variables
	int i;
	int count=0;
	char a1[1000]; // String to store one line from csv file

	// Interfaces
	hls::stream<fapi_axis> fapi_in;
	hls::stream<cnfg_axis_t> cnfg_out;
	hls::stream<cnfg_axis_t> rrh_out;
	bit_wire sfn_clk;
	bit_wire bbu_reset;

	// Input Valid Signals
//	int cnfg_in_valid,data_in_valid;

	// Input Data Variables - Read from csv file
	fapi_axis fapi_msg_in;

	// Output Data Variables - Written to csv file
	// TODO: write code to write to csv file
	cnfg_axis_t cnfg_pop_out;
	wire_type state_fapi_rx;
	wire_type state_ctrl_bbu_pop;
	wire_type state_l1_ctrl;

	// Output print flags
	int state_out_flag = 0, data_read_flag = 1;

	eth_data_t print_data;

	sfn_clk = 1;

	l1_ctrl_bbu(fapi_in, cnfg_out, rrh_out, state_fapi_rx, state_ctrl_bbu_pop,state_l1_ctrl,sfn_clk,bbu_reset);

	fapi_msg_in.last = 0;

	fapi_msg_in.data = 0x01701;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x03C000004D20000;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x0803750100010000;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x0800009818000000;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x040001010000;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x010001010004D2;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x0A020000320000;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data =0;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x0280000;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data =0;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x08CC000000000003;
	fapi_in.write(fapi_msg_in);
	fapi_msg_in.data = 0x08CC000000000467;
	fapi_in.write(fapi_msg_in);

	for(i = 0; i<150; i++){

		if(i<5)
			sfn_clk = 1;
		else
			sfn_clk = 0;

//		fapi_in.write(fapi_msg_in);

		bbu_reset = 0;

		l1_ctrl_bbu(fapi_in, cnfg_out, rrh_out, state_fapi_rx, state_ctrl_bbu_pop,state_l1_ctrl,sfn_clk,bbu_reset);

		if(state_out_flag)
			cout << "state_fapi_rx: " << (int)state_fapi_rx << ", state_ctrl_bbu_pop: " << (int)state_ctrl_bbu_pop << ", state_l1_ctrl: " << (int)state_l1_ctrl << endl;

		if((!cnfg_out.empty())){
			cnfg_pop_out = cnfg_out.read();
			count++;
			print_data = cnfg_pop_out.conf;

			if(data_read_flag)
				cout  <<"Reading Conf Out: "<< std::hex << print_data << "/" << cnfg_pop_out.dest << "/" << cnfg_pop_out.last<< endl;

			cout<< endl;
		}

		if((!rrh_out.empty())){
			cnfg_pop_out = rrh_out.read();
			count++;
			print_data = cnfg_pop_out.conf;

			if(data_read_flag)
				cout  <<"Reading RRH Conf Out: "<< std::hex << print_data << "/" << cnfg_pop_out.dest << "/" << cnfg_pop_out.last<< endl;

			cout<< endl;
		}


	}

//	fclose(fp_in);

	printf("Count =  %d\n", count);
	return 0;
}

