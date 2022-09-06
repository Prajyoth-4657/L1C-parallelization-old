#ifndef __L1_CONFIG_RESPONSE_H
#define __L1_CONFIG_RESPONSE_H

#include <stdlib.h>
#include <stdint.h>

void initialize_config_response_data();
void l1_tx_ConfigResponse();

typedef struct ConfigResponse_t{
	uint16_t	MsgId ;
	uint32_t	MsgLength;
	uint8_t		ErrorCode;
	uint8_t		NumofInvalidTLVs;
	uint8_t		NumofInvalidTLVsIDLE;
	uint8_t		NumofMissingTLVs;
} ConfigResponse_t;



#endif