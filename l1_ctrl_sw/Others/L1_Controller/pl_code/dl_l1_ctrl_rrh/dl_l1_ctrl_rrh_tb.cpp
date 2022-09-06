#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "l1_control.h"

int main(){
	// File IO Variables
		int i;
		int count=0;
		char a1[1000]; // String to store one line from csv file
		FILE *fp_in,*fp_out; // File pointers for IO


		// Interfaces
		hls::stream<ecpri_axis> ecpri_in;
		hls::stream<cnfg_axis_t> cnfg_out;
		bit_wire sfn_clk;
		bit_wire rrh_reset;

		// Input Data Variables - Read from csv file
		ecpri_axis ecpri_msg_in;

		// Output Data Variables - Written to csv file
		// TODO: write code to write to csv file
		cnfg_axis_t cnfg_pop_out;
		wire_type state_ecpri_rx;
		wire_type state_ctrl_bbu_pop;
		wire_type state_l1_ctrl;

		// Output print flags
		int state_out_flag = 0, data_read_flag = 1;

		eth_data_t print_data;

		sfn_clk = 1;

		l1_ctrl_rrh_pdsch(ecpri_in, cnfg_out, state_ecpri_rx, state_ctrl_bbu_pop,state_l1_ctrl,sfn_clk,rrh_reset);

		ecpri_msg_in.user = 0;
		ecpri_msg_in.last = 0;

		ecpri_msg_in.data = 0x01701;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x03C000004D20000;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x0803750100010000;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x0800000000000000;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x040001010000;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x010001010004D2;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x0A020000320000;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data =0;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x0280000;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data =0;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x08CC000000000003;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data = 0x08CC000000000467;
		ecpri_in.write(ecpri_msg_in);
		ecpri_msg_in.data =0;
		ecpri_in.write(ecpri_msg_in);
		//
//		ecpri_msg_in.data =0;
//		ecpri_in.write(ecpri_msg_in);



		for(i = 0; i<150; i++){

//			if(i>5)
//				sfn_clk = 0;

//			rrh_reset = 0;

			l1_ctrl_rrh_pdsch(ecpri_in, cnfg_out, state_ecpri_rx, state_ctrl_bbu_pop,state_l1_ctrl,sfn_clk,rrh_reset);

			if(state_out_flag)
				cout << "state_ecpri_rx: " << (int)state_ecpri_rx << ", state_ctrl_bbu_pop: " << (int)state_ctrl_bbu_pop << ", state_l1_ctrl: " << (int)state_l1_ctrl << endl;

			if((!cnfg_out.empty())){
				cnfg_pop_out = cnfg_out.read();
				count++;
				print_data = cnfg_pop_out.conf;

				if(data_read_flag)
					cout  <<"Reading Conf Out: "<< std::hex << print_data << "/" << cnfg_pop_out.dest <<"/" << endl;

				cout<< endl;
			}


		}

	//	fclose(fp_in);

		printf("Count =  %d\n", count);



	return 0;
}
