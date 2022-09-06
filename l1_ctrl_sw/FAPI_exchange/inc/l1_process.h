/*
	* param_l1_1.h
	*
	*	Created on: 25-MAY-2020
	*		Author: Rahul Dev
	*/
#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdlib.h>
#include <stdint.h>

#include <inttypes.h>
#include "tlv.h"
#include "constants.h"

uint16_t
mesg_length_16b();
void l1_process();

//States
typedef enum l1_state{
	INITIAL_STATE,
	WAIT_STATE,
	L1_FAPI_RX_STATE,
	L1_DOWNLINK_SLOT_STATE,
	ERROR_STATE
} l1_state ;




enum param_config_error_codes{
	MSG_OK,
	MSG_INVALID_STATE,
};



#endif
