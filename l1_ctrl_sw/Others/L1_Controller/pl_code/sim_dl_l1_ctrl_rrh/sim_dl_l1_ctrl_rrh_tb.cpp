#include "l1_control.h"

void ecpri_sim_tx(hls::stream<ecpri_axis> &ecpri_out);

int main(){

	hls::stream<ecpri_axis> ecpri_out;

	ecpri_axis data_out;

	for(int i = 0; i<50;i++){
		 ecpri_sim_tx(ecpri_out);
		 ecpri_out.read(data_out);

cout << hex << data_out.data << endl;
	}


	return 0;

}
