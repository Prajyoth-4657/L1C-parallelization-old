
#include <fapi_dma_config.h>
#include "l1_process.h"
#include "xil_printf.h"
#include "platform.h"
#include "xcounter.h"
#include "dl_ul_dma_config.h"
#include "gpio_config.h"
//#include "l1_param_resp.h"
//#include "xcounter.h"
//#include "gpio_config.h"
//#include "dl_ul_dma_config.h"


int main()
{
	init_platform();
	uint16_t i;
//--------------------------------dma 0 test code for stream module--------//
//	fapi_dma_init();
//
//		u32 *test1,*test2,*test3,*test4,*test5;
//		u32 *test6,*test7,*test8,*test9,*test10;
//		u32 *test11,*test12,*test13,*test14,*test15;
//		u32 *test16,*test17,*test18,*test19,*test20;
//
//		fapi_receive_data(1);//preq//bd1
//		test1 = (u32 *) 0x01300000+0;
//		int param_req = test1[0];
//		fapi_bd_free(1);
//
//		int test_array[256]={0};
//		for(i=0;i<256;i++)
//		{
//			test_array[i] = 1;
//		}
//		fapi_transmit_data(test_array,1024);
//		fapi_receive_data(1);//creq//bd2
//		test2 = (u32 *) (0x01300000+8000);
//		int config_req = test2[0];
//		fapi_bd_free(1);
//
//		int test_array1[256]={0};
//		for(i=0;i<256;i++)
//		{
//			test_array1[i] = 3;
//		}
//		fapi_transmit_data(test_array1,1024); //cresp
//		fapi_receive_data(1);//creq//bd2
//		test3 = (u32 *) (0x01300000+0);
//		int dl_tti1 = test3[0];
//		fapi_bd_free(1);
//		fapi_receive_data(1);
//		test4 = (u32 *) (0x01300000+8000);
//		int ul_tti1 = test4[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test5 = (u32 *) (0x01300000+16000);
//		int ul_dci1 = test5[0];
//		fapi_bd_free(1);
////		int t2 = XCounter_Get_return(xcptr);
////		int t3 = t2-t1;
//		memset((void *)0x01300000, 0, 8000*3);
//		fapi_receive_data(1);
//		test6 = (u32 *) (0x01300000+0);
//		int dl_tti2 = test6[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test7 = (u32 *) (0x01300000+8000);
//		int ul_tti2 = test7[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test8 = (u32 *) (0x01300000+16000);
//		int ul_dci2 = test8[0];
//		fapi_bd_free(1);
//
//		memset((void *)0x01300000, 0, 8000*3);
//		fapi_receive_data(1);
//		test9 = (u32 *) (0x01300000+0);
//		int dl_tti3 = test9[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test10 = (u32 *) (0x01300000+8000);
//		int ul_tti3 = test10[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test11 = (u32 *) (0x01300000+16000);
//		int ul_dci3 = test11[0];
//		fapi_bd_free(1);
//		memset((void *)0x01300000, 0, 8000*3);
//
//		fapi_receive_data(1);
//		test12 = (u32 *) (0x01300000+0);
//		int dl_tti4 = test12[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test13 = (u32 *) (0x01300000+8000);
//		int ul_tti4 = test13[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test14 = (u32 *) (0x01300000+16000);
//		int ul_dci4 = test14[0];
//		fapi_bd_free(1);
//		memset((void *)0x01300000, 0, 8000*3);
//
//		fapi_receive_data(1);
//		test15 = (u32 *) (0x01300000+0);
//		int dl_tti5 = test15[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test16 = (u32 *) (0x01300000+8000);
//		int ul_tti5 = test16[0];
//		fapi_bd_free(1);
//
//		fapi_receive_data(1);
//		test17 = (u32 *) (0x01300000+16000);
//		int ul_dci5 = test17[0];
//		fapi_bd_free(1);
////	xil_printf("t1 = %d",t1);
////	xil_printf("t2 = %d",t2);
////	xil_printf("t3 = %d",t3);
//		xil_printf("param_req = %x\r\n",param_req);
//		xil_printf("config_req = %x\r\n",config_req);
//		xil_printf("dl_tti1 = %x\r\n",dl_tti1);
//		xil_printf("ul_tti1 = %x\r\n",ul_tti1);
//		xil_printf("ul_dci1 = %x\r\n",ul_dci1);
//		xil_printf("dl_tti2 = %x\r\n",dl_tti2);
//		xil_printf("ul_tti2 = %x\r\n",ul_tti2);
//		xil_printf("ul_dci2 = %x\r\n",ul_dci2);
//		xil_printf("dl_tti3 = %x\r\n",dl_tti3);
//		xil_printf("ul_tti3 = %x\r\n",ul_tti3);
//		xil_printf("ul_dci3 = %x\r\n",ul_dci3);
//		xil_printf("dl_tti4 = %x\r\n",dl_tti4);
//		xil_printf("ul_tti4 = %x\r\n",ul_tti4);
//		xil_printf("ul_dci4 = %x\r\n",ul_dci4);
//		xil_printf("dl_tti5 = %x\r\n",dl_tti5);
//		xil_printf("ul_tti5 = %x\r\n",ul_tti5);
//		xil_printf("ul_dci5 = %x\r\n",ul_dci5);
//--------------------------------test code for stream module--------//
//
	for(i=0;i<15;i++)
	{
		l1_process();
	}


//--------------------------------dma 1 test code for stream module--------//
//	dl_ul_dma_init();
////	int *test13;
//	int test_array18[256]={0};
//	for(i=0;i<256;i++)
//	{
//	test_array18[i] = i;
//	}
//	dl_ul_transmit_data(test_array18,1024);
//	dl_ul_receive_data(1);
//	test19 = (u32 *) (0x02300000+0);
//	int a= test19[0];
//	xil_printf("dl_ul = %x\r\n",a);
//	for(i=0;i<256;i++)
//	{
//		xil_printf("test19[%d] = %x\r\n",i,test19[i]);
//	}
//-------------------------------------------------------------------------//
	cleanup_platform();
	return 0;
}




