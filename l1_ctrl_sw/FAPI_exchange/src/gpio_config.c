/*
 * gpio_config.c
 *
 *  Created on: 09-Sep-2020
 *      Author: pavank
 */

#include "gpio_config.h"

XGpio output;

void
gpio_trigger_fapi_config()
{
	XGpio_Initialize(&output,XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&output,1,0);//initialize the gpio as output=0
}

void
gpio_trigger_fapi_set_reset(int value)
{
	XGpio_DiscreteWrite(&output,1,value);//1=set,0-reset
}
