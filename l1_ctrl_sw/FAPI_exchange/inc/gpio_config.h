/*
	* gpio_config.h
	*
	*	Created on: 09-Sep-2020
	*		Author: pavank
	*/

#ifndef INC_GPIO_CONFIG_H_
#define INC_GPIO_CONFIG_H_

#include "xgpio.h"
#include "xparameters.h"

void
gpio_trigger_fapi_config();

void
gpio_trigger_fapi_set_reset(int value);


#endif /* INC_GPIO_CONFIG_H_ */
