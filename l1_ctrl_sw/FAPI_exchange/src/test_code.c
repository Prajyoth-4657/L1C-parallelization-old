//#include "xaxidma.h"
//#include "xparameters.h"
//#include "xdebug.h"
//#include "xcounter.h"
//#include "gpio_config.h"
//#include "xil_mmu.h"
//#include "platform.h"
//
//#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID
//#define DDR_BASE_ADDR	XPAR_PSU_DDR_0_S_AXI_BASEADDR
//#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x1000000)
//
//#define TX_BD_SPACE_BASE	(MEM_BASE_ADDR)
//#define TX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00000FFF)
//#define RX_BD_SPACE_BASE	(MEM_BASE_ADDR + 0x00001000)
//#define RX_BD_SPACE_HIGH	(MEM_BASE_ADDR + 0x00003710)
//#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
//#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000)
//#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)
//
//#define MAX_PKT_LEN		8000
//#define MARK_UNCACHEABLE		0x701
//
//static int RxSetup_cyclic(XAxiDma * AxiDmaInstPtr,int num_bd);
//static int TxSetup(XAxiDma * AxiDmaInstPtr);
//static int SendPacket(XAxiDma * AxiDmaInstPtr);
//static int CheckData(void);
//
//XAxiDma AxiDma;
//u32 *Packet = (u32 *) TX_BUFFER_BASE;
//
//int main(void)
//{
//	init_platform();
//	int Status;
//	XAxiDma_Config *Config;
//	gpio_trigger_fapi_config();
//	XCounter xc;
//	XCounter* xcptr = &xc;
//	XCounter_Initialize(xcptr,0);
//	XCounter_EnableAutoRestart(xcptr);
//	XCounter_Start(xcptr);
//
//	xil_printf("\r\n--- Entering main() --- \r\n");
//
//#ifdef __aarch64__
//	Xil_SetTlbAttributes(TX_BD_SPACE_BASE, MARK_UNCACHEABLE);
//	Xil_SetTlbAttributes(RX_BD_SPACE_BASE, MARK_UNCACHEABLE);
//#endif
//
//	Xil_DCacheDisable();//imp
//
//	Config = XAxiDma_LookupConfig(DMA_DEV_ID);
//
//	/* Initialize DMA engine */
//	XAxiDma_CfgInitialize(&AxiDma, Config);
//	TxSetup(&AxiDma);
//	RxSetup_cyclic(&AxiDma,3);
//
////------------------------------------------Testing code----------------------------//
//	u32 *test1,*test2,*test3,*test4,*test5;
//	u32 *test6,*test7,*test8,*test9,*test10;
//	u32 *test11,*test12,*test13,*test14,*test15;
//	u32 *test16,*test17,*test18,*test19,*test20;
//
//	receive_data(1);//preq//bd1
//	test1 = (u32 *) RX_BUFFER_BASE+0;
//	int param_req = test1[0];
//	bd_free(1);
//	send_data(1);//pres
//	receive_data(1);//creq//bd2
//	test2 = (u32 *) (RX_BUFFER_BASE+8000);
//	int config_req = test2[0];
//	bd_free(1);
//	send_data(3);//cres
//	int t1 = XCounter_Get_return(xcptr);
//	receive_data(1);
//	test3 = (u32 *) (RX_BUFFER_BASE+16000);
//	int dl_tti1 = test3[0];
//	bd_free(1);
//	receive_data(1);
//	test4 = (u32 *) (RX_BUFFER_BASE+0);
//	int ul_tti1 = test4[0];
//	bd_free(1);
//	receive_data(1);
//	test5 = (u32 *) (RX_BUFFER_BASE+8000);
//	int ul_dci1 = test5[0];
//	bd_free(1);
//	int t2 = XCounter_Get_return(xcptr);
//	int t3 = t2-t1;
//	memset((void *)RX_BUFFER_BASE, 0, MAX_PKT_LEN*3);
//	receive_data(1);
//	test6 = (u32 *) (RX_BUFFER_BASE+16000);
//	int dl_tti2 = test6[0];
//	bd_free(1);
//	receive_data(1);
//	test7 = (u32 *) (RX_BUFFER_BASE+0);
//	int ul_tti2 = test7[0];
//	bd_free(1);
//	receive_data(1);
//	test8 = (u32 *) (RX_BUFFER_BASE+8000);
//	int ul_dci2 = test8[0];
//	bd_free(1);
//	memset((void *)RX_BUFFER_BASE, 0, MAX_PKT_LEN*3);
//	receive_data(1);
//	test9 = (u32 *) (RX_BUFFER_BASE+16000);
//	int dl_tti3 = test9[0];
//	bd_free(1);
//	receive_data(1);
//	test10 = (u32 *) (RX_BUFFER_BASE+0);
//	int ul_tti3 = test10[0];
//	bd_free(1);
//	receive_data(1);
//	test11 = (u32 *) (RX_BUFFER_BASE+8000);
//	int ul_dci3 = test11[0];
//	bd_free(1);
//	memset((void *)RX_BUFFER_BASE, 0, MAX_PKT_LEN*3);
//	receive_data(1);
//	test12 = (u32 *) (RX_BUFFER_BASE+16000);
//	int dl_tti4 = test12[0];
//	bd_free(1);
//	receive_data(1);
//	test13 = (u32 *) (RX_BUFFER_BASE+0);
//	int ul_tti4 = test13[0];
//	bd_free(1);
//	receive_data(1);
//	test14 = (u32 *) (RX_BUFFER_BASE+8000);
//	int ul_dci4 = test14[0];
//	bd_free(1);
//	memset((void *)RX_BUFFER_BASE, 0, MAX_PKT_LEN*3);
//	receive_data(1);
//	test15 = (u32 *) (RX_BUFFER_BASE+16000);
//	int dl_tti5 = test15[0];
//	bd_free(1);
//	receive_data(1);
//	test16 = (u32 *) (RX_BUFFER_BASE+0);
//	int ul_tti5 = test16[0];
//	bd_free(1);
//	receive_data(1);
//	test17 = (u32 *) (RX_BUFFER_BASE+8000);
//	int ul_dci5 = test17[0];
//	bd_free(1);
//	xil_printf("t1 = %d",t1);
//	xil_printf("t2 = %d",t2);
//	xil_printf("t3 = %d",t3);
//	xil_printf("param_req = %x\r\n",param_req);
//	xil_printf("config_req = %x\r\n",config_req);
//		xil_printf("dl_tti1 = %x\r\n",dl_tti1);
//	xil_printf("ul_tti1 = %x\r\n",ul_tti1);
//		xil_printf("ul_dci1 = %x\r\n",ul_dci1);
//		xil_printf("dl_tti2 = %x\r\n",dl_tti2);
//	xil_printf("ul_tti2 = %x\r\n",ul_tti2);
//		xil_printf("ul_dci2 = %x\r\n",ul_dci2);
//		xil_printf("dl_tti3 = %x\r\n",dl_tti3);
//	xil_printf("ul_tti3 = %x\r\n",ul_tti3);
//		xil_printf("ul_dci3 = %x\r\n",ul_dci3);
//		xil_printf("dl_tti4 = %x\r\n",dl_tti4);
//	xil_printf("ul_tti4 = %x\r\n",ul_tti4);
//		xil_printf("ul_dci4 = %x\r\n",ul_dci4);
//		xil_printf("dl_tti5 = %x\r\n",dl_tti5);
//	xil_printf("ul_tti5 = %x\r\n",ul_tti5);
//		xil_printf("ul_dci5 = %x\r\n",ul_dci5);
////		CheckData();
////---------------------------------------Testing 5 iterations ----------------------
//
//	xil_printf("Successfully ran AXI DMA SG Polling Example\r\n");
//	xil_printf("--- Exiting main() --- \r\n");
//
//	cleanup_platform();
//}
//
//
//static int RxSetup_cyclic(XAxiDma * AxiDmaInstPtr,int num_bd)//dma-sg-example-function for setting up the rx bd ring
//{
//	XAxiDma_BdRing *RxRingPtr;
//	XAxiDma_Bd BdTemplate;
//	XAxiDma_Bd *BdPtr;
//	XAxiDma_Bd *BdCurPtr;
//	u32 BdCount;
//	u32 FreeBdCount;
//	UINTPTR RxBufferPtr;
//	int Index;
//	RxRingPtr = XAxiDma_GetRxRing(&AxiDma);
//	XAxiDma_BdRingIntDisable(RxRingPtr, XAXIDMA_IRQ_ALL_MASK);//we are not using IRQ
//	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,RX_BD_SPACE_HIGH - RX_BD_SPACE_BASE + 1);//According to the XAxiDma_BdRingCntCalc ,bdcount = 64
//	XAxiDma_BdRingCreate(RxRingPtr, RX_BD_SPACE_BASE,RX_BD_SPACE_BASE,XAXIDMA_BD_MINIMUM_ALIGNMENT,num_bd);// 3 BD has been created
//	XAxiDma_BdClear(&BdTemplate);//clear the all 64 BD
//	XAxiDma_BdRingClone(RxRingPtr, &BdTemplate);//This prevents modification of BDs while they are in use by hardware or the application
//	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);//number of BDs allocatable with XAxiDma_BdRingAlloc() for pre-processing.
//	XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);//to modify individual BDs. Traversal of the BD set can be done using XAxiDma_BdRingNext() and XAxiDma_BdRingPrev()
//	BdCurPtr = BdPtr;
//	RxBufferPtr = RX_BUFFER_BASE;
//	for (Index = 0; Index < FreeBdCount; Index++) {
//		XAxiDma_BdSetBufAddr(BdCurPtr, RxBufferPtr);
//		XAxiDma_BdSetLength(BdCurPtr, MAX_PKT_LEN,RxRingPtr->MaxTransferLen);
//		XAxiDma_BdSetCtrl(BdCurPtr, 0);
//		XAxiDma_BdSetId(BdCurPtr, RxBufferPtr);
//		RxBufferPtr += MAX_PKT_LEN;
//		BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
//	}
//	memset((void *)RX_BUFFER_BASE, 0, MAX_PKT_LEN*3);
//	XAxiDma_BdRingToHw(RxRingPtr,FreeBdCount,BdPtr);//pre-process
//	XAxiDma_BdRingEnableCyclicDMA(RxRingPtr);
//	XAxiDma_SelectCyclicMode(AxiDmaInstPtr, XAXIDMA_DEVICE_TO_DMA, 1);
//	XAxiDma_BdRingStart(RxRingPtr);
//}
//
//static int TxSetup(XAxiDma * AxiDmaInstPtr)//dma-sg-example-function for setting up the tx bd ring
//{
//	XAxiDma_BdRing *TxRingPtr;
//	XAxiDma_Bd BdTemplate;
//	u32 BdCount;
//	TxRingPtr = XAxiDma_GetTxRing(&AxiDma);
//	XAxiDma_BdRingIntDisable(TxRingPtr, XAXIDMA_IRQ_ALL_MASK);
//	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,TX_BD_SPACE_HIGH - TX_BD_SPACE_BASE + 1);//here bd =64
//	XAxiDma_BdRingCreate(TxRingPtr, TX_BD_SPACE_BASE,TX_BD_SPACE_BASE,XAXIDMA_BD_MINIMUM_ALIGNMENT, BdCount);// XAXIDMA_BD_MINIMUM_ALIGNMENT =64
//	XAxiDma_BdClear(&BdTemplate);
//	XAxiDma_BdRingClone(TxRingPtr, &BdTemplate);
//	XAxiDma_BdRingStart(TxRingPtr);
//}
//
//
//static int CheckData(void)////////////////this function is used to check the received data
//{
//	u32 *RxPacket;
//	int Index = 0;
//	RxPacket = (u32 *) RX_BUFFER_BASE;
//	Xil_DCacheInvalidateRange((UINTPTR)RxPacket,MAX_PKT_LEN);
//	for(Index = 0; Index <1500; Index++) {
//		xil_printf("RxPacket[%p] = %x \r\n",&RxPacket[Index],RxPacket[Index]);
//	}
//}
//
//
//
//void
//receive_data(int value)//this function is used to received data based on the max length i.e defined in macro
//{
//	XAxiDma_BdRing *RxRingPtr;
//	XAxiDma_Bd *BdPtr;
//	int ProcessedBdCount;
//	RxRingPtr = XAxiDma_GetRxRing(&AxiDma);
//	Xil_DCacheFlushRange((UINTPTR)RX_BUFFER_BASE,MAX_PKT_LEN * value);
//	while ((ProcessedBdCount = XAxiDma_BdRingFromHw(RxRingPtr,value,&BdPtr)) == 0) {
//	}
//}
//
//void
//bd_free(int value)
//{
//	XAxiDma_BdRing *RxRingPtr;
//	XAxiDma_Bd *BdPtr;
//	XAxiDma_BdRingFree(RxRingPtr,value,BdPtr);
//}
//
//void
//send_data(int constant)//this function is used to send data
//{
//	XAxiDma_BdRing *TxRingPtr;
//	u32 *TxPacket;
//	XAxiDma_Bd *BdPtr;
//	int Index;
//	TxRingPtr = XAxiDma_GetTxRing(&AxiDma);
//	TxPacket = (u32 *) Packet;
//	for(Index = 0; Index < 100; Index ++) {
//		TxPacket[Index] = constant;
//	}
//	Xil_DCacheFlushRange((UINTPTR)TxPacket,100);
//	XAxiDma_BdRingAlloc(TxRingPtr,1, &BdPtr);
//	XAxiDma_BdSetBufAddr(BdPtr, (UINTPTR) Packet);
//	XAxiDma_BdSetLength(BdPtr,100,TxRingPtr->MaxTransferLen);
//	XAxiDma_BdSetAppWord(BdPtr,XAXIDMA_LAST_APPWORD,100);
//	XAxiDma_BdSetCtrl(BdPtr, XAXIDMA_BD_CTRL_TXEOF_MASK |XAXIDMA_BD_CTRL_TXSOF_MASK);
//	XAxiDma_BdSetId(BdPtr, (UINTPTR)Packet);
//	XAxiDma_BdRingToHw(TxRingPtr,1, BdPtr);
//}
//
//
