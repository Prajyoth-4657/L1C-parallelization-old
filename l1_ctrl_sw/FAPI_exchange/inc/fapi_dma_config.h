/*
	* fapi_dma_config.h
	*
	*	Created on: 06-Aug-2020
	*		Author: Pavan
	*/

#ifndef FAPI_DMA_CONFIG_H_
#define FAPI_DMA_CONFIG_H_


/*----------Macros Start----------*/
#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID
#define DDR_BASE_ADDR	XPAR_PSU_DDR_0_S_AXI_BASEADDR
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x1000000)//For dma 0
#define TX_BD_SPACE_BASE	(MEM_BASE_ADDR)
#define TX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00000FFF)
#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR + 0x00001000)
#define RX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00003710)
#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)
#define MAX_PKT_LEN		8000// verify before running the code
#define MARK_UNCACHEABLE		0x701
#define NUM_BD					3// verify before running the code
/*----------Macros End----------*/

void
fapi_dma_init();

void
fapi_receive_data(int num_bd);

void
fapi_bd_free(int num_bd);

void
fapi_transmit_data(int *tx_data,int size);



#endif /* SRC_DMA_CONFIG_H_ */
