#include <fapi_dma_config.h>
#include "l1_ul_dci_request.h"
#include <inttypes.h>
#include "l1_process.h"
#include "math.h"
#include "xil_printf.h"

UL_dci_Request_t	UL_dci_Request;
//u8 *ul_dci_array_ptr;

//uint8_t
//get_ul_dci_value_8b()
//{
//	return (uint8_t)(*ul_dci_array_ptr++);
//}
//
//uint16_t
//get_ul_dci_value_16b()
//{
//	return (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
//}
//
//uint32_t
//get_ul_dci_value_32b()
//{
//	return (uint32_t)(((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8)) | ((*ul_dci_array_ptr++)<<16) | ((*ul_dci_array_ptr++)<<24));
//}

void get_ul_dci_request_data()
{
	int Pdu_Count=0;
	int Pdcch_Count = 0;
	int var1=0,var2=0,var3=0,var4=0;
	int ul_dci_pdu_size = 0;
//	int ul_dci_32b_pdu[1] = {0};// not optimized
//	uint8_t	ul_dci_8b_pdu[1200] = {0};// not optimized
	u32 *ul_dci_32b_pdu;
	u8 *ul_dci_array_ptr;
	ul_dci_32b_pdu = (u32 *)((0x01300004)+get_inc_address());/// optimized(to avoid memcpy)
//	fapi_rcv_invalidate_dma_cache_32b(ul_dci_32b_pdu,4,1);// not optimized
	UL_dci_Request.MsgLength = (ul_dci_32b_pdu[0]<<16) | mesg_length_16b();
	UL_dci_Request.SFN = ul_dci_32b_pdu[0];
	ul_dci_pdu_size = UL_dci_Request.MsgLength;
	ul_dci_array_ptr = (u8 *)((0x01300008)+get_inc_address());/// optimized(to avoid memcpy)
	//fapi_rcv_invalidate_dma_cache_8b(ul_dci_8b_pdu,ul_dci_pdu_size,2);// not optimized
	//ul_dci_array_ptr = &ul_dci_8b_pdu[0];// not optimized
	UL_dci_Request.Slot = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
	UL_dci_Request.nPDUs = (uint8_t)(*ul_dci_array_ptr++);
	//xil_printf("UL_dci_Request.nPDUs = %d",UL_dci_Request.nPDUs);
	while(Pdu_Count <(int)(UL_dci_Request.nPDUs))
	{
		UL_dci_Request.UL_DCI_PDU.PDUType = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
		UL_dci_Request.UL_DCI_PDU.PDUSize = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
		//xil_printf("UL_dci_Request.UL_DCI_PDU.PDUType = %x \n", UL_dci_Request.UL_DCI_PDU.PDUType);
		switch((int)(UL_dci_Request.UL_DCI_PDU.PDUType))
		{
			case PDCCH_PDU_TAG:
			{
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].BWPSize = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].BWPStart = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].SubcarrierSpacing = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].CyclicPrefix = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].StartSymbolIndex = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].DurationSymbols = (uint8_t)(*ul_dci_array_ptr++);
				//xil_printf("%d, %d",UL_dci_Request.PDCCH_PDU[Pdcch_Count].BWPSize,UL_dci_Request.PDCCH_PDU[Pdcch_Count].DurationSymbols);
				//xil_printf("%d",MAX_FREQ_DOM_RESOURCE);
				for(var1 = 0;var1<(int)(MAX_FREQ_DOM_RESOURCE);var1++)
				{
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1] = (uint8_t)(*ul_dci_array_ptr++);
					//xil_printf("fr[%d]= %d ",var1,UL_dci_Request.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1]);
				}
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].CceRegMappingType = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].RegBundleSize = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].InterleaverSize = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].CoreSetType = (uint8_t)(*ul_dci_array_ptr++);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].ShiftIndex = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].precoderGranularity = (uint8_t)(*ul_dci_array_ptr++);
				//xil_printf("precoder granu= %d \n",UL_dci_Request.PDCCH_PDU[Pdcch_Count].precoderGranularity);
				UL_dci_Request.PDCCH_PDU[Pdcch_Count].numDlDci = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
				for(var1 = 0;var1<(int)(UL_dci_Request.PDCCH_PDU[Pdcch_Count].numDlDci);var1++)
				{
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].RNTI = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));// number dldcis yet to know the values
					//xil_printf("RNTI = %d",UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].RNTI);

					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingId = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingRNTI = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].CceIndex = (uint8_t)(*ul_dci_array_ptr++);
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].AggregationLevel = (uint8_t)(*ul_dci_array_ptr++);
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.prgSize = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces = (uint8_t)(*ul_dci_array_ptr++);

					//xil_printf("UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs = %d \n",UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs );
					//xil_printf("digbf-interface = %d \n",UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);
					for(var2=0;var2<(int)(UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs);var2++)
					{
						UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
						//xil_printf("[0]pmidx=%d \n",UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
						for(var3=0;var3<(int)(UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);var3++)
						{
							UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
							//xil_printf(" beamIdx = %d , v4=%d\n",UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] ,var4);
						}
					}

					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0 = (uint8_t)(*ul_dci_array_ptr++);
					//xil_printf("beta_pdcch=%d \n",UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0);
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].powerControlOffsetSS = (uint8_t)(*ul_dci_array_ptr++);
					UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits = (uint16_t)((*ul_dci_array_ptr++) | ((*ul_dci_array_ptr++)<<8));
					for(var2 = 0;var2<(int)(DCI_PAYLOAD_BTYE_LEN);var2++)
					{
						UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] = (uint8_t)(*ul_dci_array_ptr++);
						//xil_printf("payload[%d] = %d \n ",var2,UL_dci_Request.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] );
					}
				}
				Pdcch_Count = Pdcch_Count+1;
				//xil_printf("------------------------------------------Pdcch_Count--------------------------------------- %d",Pdcch_Count);
				break;
			}

			default:
				break;
		}
		Pdu_Count=Pdu_Count+1;
	}
	//xil_printf("ul_dci is done \r\n");
}
