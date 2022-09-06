#include "dl_ul_dma_config.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"
#include "xil_mmu.h"
#include "xstatus.h"
/*----------Structure Intrances Start------ */
XAxiDma Dl_Ul_AxiDma;
/*----------Structure Intrances End------ */

/*---------- Global variables Start-------*/

/*---------- Global variables End-------*/

static void
dl_ul_RxSetup(int num_bd);

static void
dl_ul_TxSetup();

void
dl_ul_dma_init()
{
	int Status;
	XAxiDma_Config *dl_ul_Config;
	Xil_SetTlbAttributes(TX_BD_SPACE_BASE, MARK_UNCACHEABLE);
	Xil_SetTlbAttributes(RX_BD_SPACE_BASE, MARK_UNCACHEABLE);
	Xil_DCacheDisable();
	dl_ul_Config = XAxiDma_LookupConfig(DMA_DEV_ID_1);
	XAxiDma_CfgInitialize(&Dl_Ul_AxiDma,dl_ul_Config);
	dl_ul_TxSetup();
	dl_ul_RxSetup(1/*NUM_BD*/);
}

void
dl_ul_RxSetup(int num_bd)
{
	XAxiDma_BdRing *RxRingPtr;
	XAxiDma_Bd BdTemplate;
	XAxiDma_Bd *BdPtr;
	XAxiDma_Bd *BdCurPtr;
	u32 BdCount;
	u32 FreeBdCount;
	UINTPTR RxBufferPtr;
	int Index;
	RxRingPtr = XAxiDma_GetRxRing(&Dl_Ul_AxiDma);
	XAxiDma_BdRingIntDisable(RxRingPtr, XAXIDMA_IRQ_ALL_MASK);//we are not using IRQ
	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,RX_BD_SPACE_HIGH - RX_BD_SPACE_BASE + 1);//According to the XAxiDma_BdRingCntCalc ,bdcount = 64
	XAxiDma_BdRingCreate(RxRingPtr, RX_BD_SPACE_BASE,RX_BD_SPACE_BASE,XAXIDMA_BD_MINIMUM_ALIGNMENT,num_bd);// 3 BD has been created
	XAxiDma_BdClear(&BdTemplate);//clear the all 64 BD
	XAxiDma_BdRingClone(RxRingPtr, &BdTemplate);//This prevents modification of BDs while they are in use by hardware or the application
	FreeBdCount = XAxiDma_BdRingGetFreeCnt(RxRingPtr);//number of BDs allocatable with XAxiDma_BdRingAlloc() for pre-processing.
	XAxiDma_BdRingAlloc(RxRingPtr, FreeBdCount, &BdPtr);//to modify individual BDs. Traversal of the BD set can be done using XAxiDma_BdRingNext() and XAxiDma_BdRingPrev()
	BdCurPtr = BdPtr;
	RxBufferPtr = RX_BUFFER_BASE;
	for (Index = 0; Index < FreeBdCount; Index++) {
		XAxiDma_BdSetBufAddr(BdCurPtr, RxBufferPtr);
		XAxiDma_BdSetLength(BdCurPtr, MAX_PKT_LEN,RxRingPtr->MaxTransferLen);
		XAxiDma_BdSetCtrl(BdCurPtr, 0);
		XAxiDma_BdSetId(BdCurPtr, RxBufferPtr);
		RxBufferPtr += MAX_PKT_LEN;
		BdCurPtr = (XAxiDma_Bd *)XAxiDma_BdRingNext(RxRingPtr, BdCurPtr);
	}
	memset((void *)RX_BUFFER_BASE, 0, MAX_PKT_LEN*num_bd);
	XAxiDma_BdRingToHw(RxRingPtr,FreeBdCount,BdPtr);//pre-process
	XAxiDma_BdRingStart(RxRingPtr);
}

void
dl_ul_TxSetup()//dma-sg-example-function for setting up the tx bd ring
{
	XAxiDma_BdRing *TxRingPtr;
	XAxiDma_Bd BdTemplate;
	u32 BdCount;
	TxRingPtr = XAxiDma_GetTxRing(&Dl_Ul_AxiDma);
	XAxiDma_BdRingIntDisable(TxRingPtr, XAXIDMA_IRQ_ALL_MASK);
	BdCount = XAxiDma_BdRingCntCalc(XAXIDMA_BD_MINIMUM_ALIGNMENT,TX_BD_SPACE_HIGH - TX_BD_SPACE_BASE + 1);//here bd =64
	XAxiDma_BdRingCreate(TxRingPtr, TX_BD_SPACE_BASE,TX_BD_SPACE_BASE,XAXIDMA_BD_MINIMUM_ALIGNMENT, BdCount);// XAXIDMA_BD_MINIMUM_ALIGNMENT =64
	XAxiDma_BdClear(&BdTemplate);
	XAxiDma_BdRingClone(TxRingPtr, &BdTemplate);
	XAxiDma_BdRingStart(TxRingPtr);
}


void
dl_ul_receive_data(int num_bd)//this function is used to received data based on the max length i.e defined in macro
{
	XAxiDma_BdRing *RxRingPtr;
	XAxiDma_Bd *BdPtr;
	int ProcessedBdCount;
	RxRingPtr = XAxiDma_GetRxRing(&Dl_Ul_AxiDma);
	Xil_DCacheFlushRange((UINTPTR)RX_BUFFER_BASE,MAX_PKT_LEN * num_bd);
	while ((ProcessedBdCount = XAxiDma_BdRingFromHw(RxRingPtr,num_bd,&BdPtr)) == 0) {
	}
}

void
dl_ul_bd_free(int num_bd)
{
	XAxiDma_BdRing *RxRingPtr;
	XAxiDma_Bd *BdPtr;
	XAxiDma_BdRingFree(RxRingPtr,num_bd,BdPtr);
}

void
dl_ul_transmit_data(int *tx_data,int size)//this function is used to send data
{
	XAxiDma_BdRing *TxRingPtr;
	u32 *TxPacket;
	XAxiDma_Bd *BdPtr;
	TxRingPtr = XAxiDma_GetTxRing(&Dl_Ul_AxiDma);
	TxPacket = (u32 *) TX_BUFFER_BASE;
	memcpy(TxPacket,tx_data,size);
	Xil_DCacheFlushRange((UINTPTR)TxPacket,size);
	XAxiDma_BdRingAlloc(TxRingPtr,1, &BdPtr);
	XAxiDma_BdSetBufAddr(BdPtr, (UINTPTR) TX_BUFFER_BASE);
	XAxiDma_BdSetLength(BdPtr,size,TxRingPtr->MaxTransferLen);
	XAxiDma_BdSetAppWord(BdPtr,XAXIDMA_LAST_APPWORD,size);
	XAxiDma_BdSetCtrl(BdPtr, XAXIDMA_BD_CTRL_TXEOF_MASK |XAXIDMA_BD_CTRL_TXSOF_MASK);
	XAxiDma_BdSetId(BdPtr, (UINTPTR)TX_BUFFER_BASE);
	XAxiDma_BdRingToHw(TxRingPtr,1, BdPtr);
}

