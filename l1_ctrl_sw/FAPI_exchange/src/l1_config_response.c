#include <fapi_dma_config.h>
#include "l1_config_response.h"
#include "constants.h"
#include "l1_process.h"

ConfigResponse_t ConfigResponse;

void
initialize_config_response_data()
{
	ConfigResponse.MsgId = CONFIG_RESPONSE;
	ConfigResponse.MsgLength = 0x04;
	ConfigResponse.ErrorCode = MSG_OK;
	ConfigResponse.NumofInvalidTLVs = 0;
	ConfigResponse.NumofInvalidTLVsIDLE = 0;
	ConfigResponse.NumofMissingTLVs = 0;
}

void
l1_tx_ConfigResponse()
{
	fapi_transmit_data((char*)&ConfigResponse,sizeof(ConfigResponse));
	//xil_printf("config response is done \r\n");
}
