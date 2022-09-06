// #include <fapi_dma_config.h>
#include "l1_dl_tti_request.h"
#include "stdlib.h"
//#include "l1_process.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "math.h"
//#include <x86intrin.h>
#include "l1_config_request.h"
#include "l1_sw_common.h"
//// #include "xil_printf.h"

#define BILLION 1000000000L


#define IO_DIR_PATH  "../../IO_files/TestSookthaLog/"     
 
// Three options for input fapi data
enum INPUT_OPTIONS{BIN_FILE_INPUT, HEX_STRING_INPUT, HEX_LOG_FILE_INPUT};

// slot 3, sfn = 225
char fapiHexDlttiReq0[] = "01 00 80 00 06 00 00 00 e1 00 03 00 00 00";

// slot 4, sfn = 225
//char fapiHexDlttiReq1[] = "01 00 80 00 89 00 00 00 e1 00 04 00 02 00 00 00 2c 00 30 00 72 00 01 00 00 02 ff 00 00 00 00 00 01 06 02 00 07 00 00 01 00 ff ff 07 00 00 00 00 04 08 01 27 00 83 e0 00 00 00 01 00 57 00 00 00 ff ff 00 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 58 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 00 02 0c 08 01";
// char fapiHexDlttiReq1[] = "01 00 80 00 f1 00 00 00 d8 00 04 00 03 00 00 00 3d 00 30 00 72 00 01 00 00 02 ff 00 00 00 00 00 01 06 02 00 07 00 00 02 00 ff ff 07 00 00 00 00 04 08 01 27 00 83 e0 00 00 00 0b 01 07 00 00 00 04 04 08 01 27 00 0f 80 00 00 00 01 00 57 00 00 00 ff ff 00 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 58 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 00 02 0c 08 01 01 00 57 00 00 00 0b 01 01 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 09 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 03 00 00 02 0c 08 01";

char fapiHexDlttiReqSFN418Slot4[] = "01 00 80 00 f1 00 00 00 a2 01 04 00 03 00 00 00 3d 00 30 00 72 00 01 00 00 02 ff 00 00 00 00 00 01 06 02 00 07 00 00 02 00 ff ff 07 00 00 00 00 04 08 01 27 00 83 e0 00 00 00 0b 01 07 00 00 00 04 04 08 01 27 00 0f 80 00 00 00 01 00 57 00 00 00 ff ff 00 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 58 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 00 02 0c 08 01 01 00 57 00 00 00 0b 01 01 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 09 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 03 00 00 02 0c 08 01";

//char fapiHexDlttiReq1[] = "01 00 80 00 f1 00 00 00 1a 00 04 00 03 00 00 00 3d 00 30 00 72 00 01 00 00 02 ff 00 00 00 00 00 01 06 02 00 07 00 00 02 00 ff ff 07 00 00 00 00 04 08 01 27 00 83 e0 00 00 00 0b 01 07 00 00 00 04 04 08 01 27 00 21 80 00 00 00 01 00 57 00 00 00 ff ff 00 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 58 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 00 02 0c 08 01 01 00 57 00 00 00 0b 01 01 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 12 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 06 00 00 02 0c 08 01";

char fapiHexDlttiReq[] = "01 00 80 00 f1 00 00 00 a2 01 04 00 03 00 00 00 3d 00 30 00 72 00 01 00 00 02 ff 00 00 00 00 00 01 06 02 00 07 00 00 02 00 ff ff 07 00 00 00 00 04 08 01 27 00 83 e0 00 00 00 0b 01 07 00 00 00 04 04 08 01 27 00 0f 80 00 00 00 01 00 57 00 00 00 ff ff 00 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 58 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 00 02 0c 08 01 01 00 57 00 00 00 0b 01 01 00 30 00 72 00 01 00 01 b0 04 02 00 00 00 09 00 00 00 07 00 01 00 01 84 08 00 07 00 00 02 01 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1c 00 03 00 00 02 0c 08 01";
char fapiHexUciReq[] = "01 00 83 00 32 00 00 00 d5 03 05 00 01 00 00 2d 00 11 01 00 00 01 00 00 02 00 00 00 1f ff f8 01 06 02 01 07 00 00 01 00 a6 ae 07 00 a6 ae 14 04 08 01 2c 00 00 89 83 70 30 00";

FILE *fp;
FILE *fp_configsONLY;

int main(int argc, char* argv[])
{
	/* Populate ConfigRequest structure */
	get_config_request_data();

    if (argc < 3){
    	char bin_file[] = IO_DIR_PATH "configs_DL_DCI.txt";
    	printf("Output configs txt file: %s \n", bin_file);
    	fp = fopen(bin_file, "w");
    }
    else{
    	printf("Output configs txt file: %s \n", argv[2]);
    	fp = fopen(argv[2], "w");
    }
	if(NULL == fp){
		printf("\n Error: fopen failed, fp = NULL \n");
		exit(0);
	}

    if (argc < 4){
    	char bin_file[] = IO_DIR_PATH "configs_DL_HWpacketsONLY_DL_DCI.txt";
    	printf("Output HWpacketsONLY txt file: %s \n", bin_file);
    	fp_configsONLY = fopen(bin_file, "w");
    }
    else{
    	printf("Output HWpacketsONLY txt file: %s \n", argv[3]);
    	fp_configsONLY = fopen(argv[3], "w");
    }
    if(NULL == fp_configsONLY){
		printf("\n Error: fopen failed, fp_configsONLY = NULL \n");
		exit(0);
	}

    // change the input data option here if needed
    enum INPUT_OPTIONS InputOptions = HEX_STRING_INPUT; // {BIN_FILE_INPUT, HEX_STRING_INPUT, HEX_LOG_FILE_INPUT}

    // Options are {BIN_FILE_INPUT, HEX_STRING_INPUT, HEX_LOG_FILE_INPUT}
    if(InputOptions == BIN_FILE_INPUT){
    	if(argc < 2) {
			char bin_file[] = IO_DIR_PATH "Slot5/message.bin";
			printf("\nInput bin file: %s \n", bin_file);
			get_dl_tti_request_data_from_file(bin_file);
    	}
    	else{
    		printf("\nInput bin file: %s\n", argv[1]);
    		get_dl_tti_request_data_from_file(argv[1]);
    	}
    	dl_config();
    }
    else if(InputOptions == HEX_STRING_INPUT) {
    	printf("\n Input data option: hex string data given in the code : \n");

    	// slot x packets
    	//get_dl_tti_request_data_from_hex_string(fapiHexDlttiReq0);
    	//dl_config();

    	// slot y packets
    	//get_dl_tti_request_data_from_hex_string(fapiHexDlttiReqSFN418Slot4);
    	get_dl_tti_request_data_from_hex_string(fapiHexDlttiReq);
    	int initializeFlag = 0;
    	//append_ul_dci_request_data_from_hex_string(fapiHexUciReq, initializeFlag);

    	clock_t begin,end;
    	struct timespec curTime, nowTime;
    	double cpu_time_used;
     	clock_gettime(CLOCK_REALTIME, &curTime); // ---> CLOCK_MONOTONIC??
    	begin = clock();    	
    	dl_config();
    	end = clock();
    	clock_gettime(CLOCK_REALTIME, &nowTime);
    	
    	cpu_time_used = ((double)(end - begin))/ CLOCKS_PER_SEC;
    	long int secs = nowTime.tv_sec - curTime.tv_sec;
    	long int nsec = (nowTime.tv_nsec - curTime.tv_nsec);
    	 
            
    	printf("CPU time is %f seconds  \n",cpu_time_used);
    	printf("Clock Time is %ld nanoseconds \n", nsec);  
    	
    }
    else{  // InputOptions == HEX_LOG_FILE_INPUT
    	char fapiLogFile[] = IO_DIR_PATH "gnb_tx_fapi_log_0_0_29_aug.fapilog";
    	printf("\n Input data option: fapi log file : %s \n", fapiLogFile);

    	char fapiHexSlotMessage[MAX_FAPI_STRING_LENGTH];
    	int startSFN = 260;
    	int numSFN = 1;// Number of SFN to be processed
    	int tmpMsgStrLen = 0;
    	for(int SFN = startSFN; SFN < (startSFN+numSFN); ++SFN){
    		for(int slot = 0; slot < 12; ++slot){
    			tmpMsgStrLen = extractSlotMessageHexStringFromFapiLog(fapiLogFile, SFN, slot, fapiHexSlotMessage);
				if( tmpMsgStrLen <= 0){
					printf("Error: exiting \n");
					exit(0);
				}
				get_dl_tti_request_data_from_hex_string(fapiHexSlotMessage);
				dl_config();
    		}
    	}
    }

	fclose(fp);
	fclose(fp_configsONLY);
	return 0;
}


