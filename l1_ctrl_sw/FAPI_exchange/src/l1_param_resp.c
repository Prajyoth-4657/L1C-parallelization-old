#include <fapi_dma_config.h>
#include "l1_param_resp.h"
#include "stdio.h"
#include "xparameters.h"
#include "xdebug.h"
#include <xil_io.h>


ParamResponse_t ParamResp;


void initialize_param_resp_data()
{
	// This file contains all the parameters (that can be changed).
	#include "../IO_files/param_response.txt"
}

void l1_tx_ParamResp()
{
	fapi_transmit_data((char*)&ParamResp,sizeof(ParamResp));
	//xil_printf("Param response is done \r\n");
}
