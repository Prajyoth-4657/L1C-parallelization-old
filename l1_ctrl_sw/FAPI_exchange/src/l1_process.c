
#include <fapi_dma_config.h>
#include "l1_process.h"
#include "l1_param_resp.h"
#include "l1_config_request.h"
#include "l1_dl_tti_request.h"
#include "xil_printf.h"
#include "stdint.h"
#include "xcounter.h"
#include "gpio_config.h"
#include "dl_ul_dma_config.h"

static l1_state l1_present_state = INITIAL_STATE;
static uint16_t MsgId;
uint16_t MsgLength;
//int intial_value[1];// not optimized
u32 *intial_value;

int t1=0,t2=0,t3=0,t4=0;


static uint16_t inc_address;

uint16_t
mesg_length_16b()
{
	return MsgLength;
}

uint16_t
get_inc_address()
{
	return inc_address;
}

void l1_process()
{
	//xil_printf("i was in l1 process \r\n");
	switch (l1_present_state)
	{
		case INITIAL_STATE:
			fapi_dma_init();
			dl_ul_dma_init();
			gpio_trigger_fapi_config();
			XCounter xc;
			XCounter* xcptr = &xc;
			XCounter_Initialize(xcptr,0);
			XCounter_EnableAutoRestart(xcptr);
			XCounter_Start(xcptr);
			inc_address=0;
			//gpio_trigger_fapi_set_reset(1);//1->set,0->reset //need to set this gpio_trigger_fapi_set_reset to see in gtkterm
			l1_present_state = WAIT_STATE;
			break;

		case WAIT_STATE:
			if(inc_address > (MAX_PKT_LEN*(NUM_BD-1)))
			{
				inc_address=0;
			}
			fapi_receive_data(1);
			intial_value = (u32 *)((0x01300000) + (inc_address));
			MsgId = (uint16_t) * ((uint16_t *)&intial_value[0]);
			MsgLength = (uint16_t) * ((uint16_t *)&intial_value+1);
			//xil_printf("param id = %x \r\n",MsgId);

			if (MsgId == PARAM_REQUEST)
			{
				initialize_param_resp_data();//intialize all param data
				fapi_bd_free(1);
				l1_present_state = L1_FAPI_RX_STATE;
			}
			else if (MsgId == CONFIG_REQUEST)
			{
				get_config_request_data();
				initialize_config_response_data();
				l1_present_state = L1_FAPI_RX_STATE;
			}
			else if (MsgId == DL_TTI_REQUEST)
			{
				get_dl_tti_request_data();
				// int dl_ul_test_array[16]={0,1,2,3,4,5,6,7,8,9,8,7,0x611111,0x522222,0x433333,0x344444};
				// fapi_transmit_data(dl_ul_test_array,sizeof(dl_ul_test_array));
				inc_address = inc_address + MAX_PKT_LEN;
				//xil_printf("i am in dl tti");
				// l1_present_state = WAIT_STATE;
				l1_present_state = L1_DOWNLINK_SLOT_STATE;
			}
			else if (MsgId == UL_DCI_REQUEST)
			{
				get_ul_dci_request_data();
				inc_address = inc_address + MAX_PKT_LEN;
				// l1_present_state = WAIT_STATE;
				l1_present_state = L1_DOWNLINK_SLOT_STATE;
			}
			else if (MsgId == UL_TTI_REQUEST)
			{
				get_ul_tti_request_data();
				inc_address = inc_address + MAX_PKT_LEN;
				// l1_present_state = WAIT_STATE;
				l1_present_state = L1_DOWNLINK_SLOT_STATE;
			}
			else
			{
				//error indication message
				xil_printf("im in error state");
				l1_present_state = ERROR_STATE;
			}
			break;

		case L1_FAPI_RX_STATE:
			if (MsgId == PARAM_REQUEST)
			{
				l1_tx_ParamResp();
				inc_address = inc_address + MAX_PKT_LEN;
				l1_present_state = WAIT_STATE;
			}
			else if (MsgId == CONFIG_REQUEST)
			{
				l1_tx_ConfigResponse();
				inc_address = inc_address + MAX_PKT_LEN;
				l1_present_state = WAIT_STATE;
			}
			break;

		case L1_DOWNLINK_SLOT_STATE:
			if (MsgId == DL_TTI_REQUEST)
			{
				dl_chain_configs(Dl_ttiRequest);
				l1_present_state = WAIT_STATE;
			}
			else if (MsgId == UL_DCI_REQUEST)
			{
				l1_present_state = WAIT_STATE;
			}
			else if (MsgId == UL_TTI_REQUEST)
			{
				l1_present_state = WAIT_STATE;
			}
			//data for the downlink_slot
			break;

		case ERROR_STATE:
			l1_present_state = WAIT_STATE;
			break;

		default:
			break;
	}
}
