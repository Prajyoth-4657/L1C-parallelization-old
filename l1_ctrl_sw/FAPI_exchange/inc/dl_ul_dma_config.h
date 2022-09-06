/***********************************************************************IIT MADRAS CONFIDENTIAL****************************************************************************************
*
*																5G TESTBED
*
												ALL RIGHTS ARE RESERVED © 2018 IIT MADRAS , CHENNAI, INDIA

*
*
*
NOTICE : ALL INFORMATION CONTAINED IN HERE IS PURELY CONFIDENTIAL AND BELONGS TO IIT MADRAS. UNAUTHORIZED USE, REVERSE ENGINEERING, DUPLICATION IN ANY FORM IS STRICTLY FORBIDDEN.

*****************************************************************************************************************************************************************************************/

/*Author = Pavan and Deepika
Data Created = 09-10-2020,
Description = Dma 0 Drivers
*/

#ifndef INC_DL_UL_DMA_CONFIG_H_
#define INC_DL_UL_DMA_CONFIG_H_

/*----------Macros Start----------*/
#define DMA_DEV_ID_1		XPAR_AXIDMA_1_DEVICE_ID
#define DDR_BASE_ADDR		XPAR_PSU_DDR_0_S_AXI_BASEADDR
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x2000000)//For dma 1
#define TX_BD_SPACE_BASE	(MEM_BASE_ADDR)
#define TX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00000FFF)
#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR + 0x00001000)
#define RX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00003710)
#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)
#define MAX_PKT_LEN		8000 // verify before running the code // need to confirm
#define MARK_UNCACHEABLE		0x701
#define NUM_BD					3// verify before running the code
/*----------Macros End----------*/

void
dl_ul_dma_init();

void
dl_ul_receive_data(int num_bd);

void
dl_ul_bd_free(int num_bd);

void
dl_ul_transmit_data(int *tx_data,int size);

#endif /* INC_DL_UL_DMA_CONFIG_H_ */
