// #include <fapi_dma_config.h>
#include "l1_ul_tti_request.h"
#include "stdlib.h"
//#include "l1_process.h"
#include <stdio.h>
//#include <time.h>
#include <string.h>
#include "math.h"
//#include <x86intrin.h>
#include "l1_config_request.h"
#include "l1_sw_common.h"
//// #include "xil_printf.h"

// #define IO_DIR_PATH "../../IO_files/TestSookthaLog/"
// #define IO_DIR_PATH "../../IO_files/Jeeva_mail_test_3_sep/"
#define IO_DIR_PATH "../../IO_files/FAPI_Configs/PUCCH/Format2Configs/L1Configs/8/"
// #define IO_DIR_PATH "../../IO_files/FAPI_Configs/PUCCH/Format2Configs/fading/5dB/9/Slot16/"
// #define IO_DIR_PATH "../../IO_files/PUCCH/5/Slot16/"
// #define IO_DIR_PATH "../../IO_files/FAPI_Configs/PUCCH/Format2Configs/7/Slot19/"
// #define IO_DIR_PATH "../../IO_files/FAPI_Configs/PUCCH/Format0Configs/Multiuser/1/Slot13/"

// Three options for input fapi data
enum INPUT_OPTIONS{BIN_FILE_INPUT, HEX_STRING_INPUT, HEX_LOG_FILE_INPUT};

// slot 19, even frame
char fapiHexUlttiReq0[] = "01 00 81 00 09 00 00 00 06 00 13 00 00 00 00 00 00";

// slot 19, odd frame
//char fapiHexUlttiReq1[] = "01 00 81 00 15 00 00 00 07 00 13 00 01 01 00 00 00 00 00 0c 00 07 00 06 04 00 00 00 00";
char fapiHexUlttiReq1[] = "01 00 81 00 6b 00 00 00 96 03 0d 00 02 00 00 02 00 02 00 31 00 f2 b0 00 00 00 00 11 01 00 00 01 00 00 00 00 00 00 01 00 0c 02 01 10 01 00 00 00 00 01 00 00 00 00 00 00 00 00 00 00 01 01 00 00 00 00 02 00 31 00 f2 b0 00 00 01 00 11 01 00 00 01 00 00 00 00 00 00 01 00 0c 02 01 10 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 00 00 00";


FILE *fp;
FILE *fp_configsONLY;

int main(int argc, char* argv[])
{
	/* Populate ConfigRequest structure */
	get_config_request_data();

    if (argc < 3){
    	char bin_file[] = IO_DIR_PATH "configs_UL.txt";
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
    	char bin_file[] = IO_DIR_PATH "configs_UL_HWpacketsONLY.txt";
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
    enum INPUT_OPTIONS InputOptions = BIN_FILE_INPUT;// {BIN_FILE_INPUT, HEX_STRING_INPUT, HEX_LOG_FILE_INPUT}

    // Options are {BIN_FILE_INPUT, HEX_STRING_INPUT, HEX_LOG_FILE_INPUT}
    if(InputOptions == BIN_FILE_INPUT){
    	if(argc < 2) {
			char bin_file[] = IO_DIR_PATH "message.bin";
			printf("\nInput bin file: %s \n", bin_file);
			get_ul_tti_request_data_from_file(bin_file);
    	}
    	else{
    		printf("\nInput bin file: %s\n", argv[1]);
    		get_ul_tti_request_data_from_file(argv[1]);
    	}
    	ul_config();
    }
    else if(InputOptions == HEX_STRING_INPUT) {
    	printf("\n Input data option: hex string data given in the code : \n");

    	// slot x packets
    	//get_ul_tti_request_data_from_hex_string(fapiHexUlttiReq0);
    	//ul_config();

    	// slot y packets
    	get_ul_tti_request_data_from_hex_string(fapiHexUlttiReq1);
    	ul_config();
    }
    else{  // InputOptions == HEX_LOG_FILE_INPUT
    	char fapiLogFile[] = IO_DIR_PATH "gnb_tx_fapi_log_0_0_29_aug.fapilog";
    	printf("\n Input data option: fapi log file : %s \n", fapiLogFile);

    	char fapiHexSlotMessage[MAX_FAPI_STRING_LENGTH];
    	int startSFN = 260;
    	int numSFN = 4;// Number of SFN to be processed
    	int tmpMsgStrLen = 0;
    	for(int SFN = startSFN; SFN < (startSFN+numSFN); ++SFN){
    		for(int slot = 13; slot < 20; ++slot){
    			tmpMsgStrLen = extractSlotMessageHexStringFromFapiLog(fapiLogFile, SFN, slot, fapiHexSlotMessage);
				if( tmpMsgStrLen <= 0){
					printf("Error: exiting \n");
					exit(0);
				}
				get_ul_tti_request_data_from_hex_string(fapiHexSlotMessage);
				ul_config();
    		}
    	}
    }

	fclose(fp);
	fclose(fp_configsONLY);
	return 0;
}
