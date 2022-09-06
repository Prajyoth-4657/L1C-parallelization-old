#include <fapi_dma_config.h>
#include <stdio.h>
#include <string.h>
#include "l1_config_request.h"
#include "l1_sw_common.h"

// Fapi hex input string -- Modify this for input config request change
char fapiHexConfigReq[] = "01 00 02 00 a2 09 00 00 30 01 01 10 02 00 64 00 00 00 02 10 04 00 00 c4 09 00 03 10 0c 00 00 00 00 00 00 00 00 00 00 00 00 00 04 10 0c 00 11 01 00 00 00 00 00 00 00 00 00 00 05 10 02 00 04 00 00 00 06 10 02 00 64 00 00 00 07 10 04 00 00 c4 09 00 08 10 0c 00 00 00 00 00 00 00 00 00 00 00 00 00 09 10 0c 00 11 01 00 00 00 00 00 00 00 00 00 00 0a 10 02 00 04 00 00 00 0b 10 01 00 00 00 00 00 0c 10 01 00 07 00 00 00 0d 10 01 00 01 00 00 00 0e 10 04 00 32 00 00 00 0f 10 01 00 01 00 00 00 10 10 01 00 01 00 00 00 11 10 01 00 01 00 00 00 12 10 01 00 01 00 00 00 13 10 01 00 00 00 00 00 14 10 01 00 01 00 00 00 15 10 02 00 00 00 00 00 16 10 01 00 01 00 00 00 17 10 02 00 04 00 00 00 18 10 01 00 00 00 00 00 19 10 01 00 01 00 00 00 1a 10 01 00 01 00 00 00 1b 10 01 00 03 00 00 00 1c 10 01 00 00 00 00 00 1d 10 02 00 fc 00 00 00 1e 10 01 00 01 00 00 00 1f 10 01 00 01 00 00 00 20 10 01 00 00 00 00 00 21 10 04 00 00 00 00 00 22 10 04 00 00 00 00 80 23 10 01 00 00 00 00 00 24 10 01 00 00 00 00 00 25 10 01 00 00 00 00 00 26 10 01 00 07 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 00 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 27 10 01 00 01 00 00 00 ";

extern ConfigRequest_t ConfigRequest;

void get_config_request_data()
{
    uint16_t counter = 0;
    uint8_t count_1003 = 0;
    uint8_t count_1004 = 0;
    uint8_t count_1008 = 0;
    uint8_t count_1009 = 0;
    uint8_t count_1015 = 0;
    uint8_t count_1016 = 0;
    uint8_t count_1017 = 0;
    uint8_t count_1018 = 0;
    uint8_t count_1019 = 0;
    uint8_t count_101A = 0;
    uint8_t count_1022 = 0;
    uint8_t count_1023 = 0;
    uint16_t count_1027 = 0;

    uint16_t tag;
    uint16_t length;
    uint8_t val8;
    uint16_t val16;
    uint32_t val32;
    uint16_t config_count=0;
    uint32_t *config_intial_value;
    uint32_t *tlv;

    /* For sw code, read fapi message in hex format and convert it to binary */
    uint8_t fapiBytesConfigReq[MAX_FAPI_BYTES];
    int numFapiBytes = convertHex2DecBytes(fapiHexConfigReq, fapiBytesConfigReq);
    // validate fapi message
    if(numFapiBytes > MAX_FAPI_BYTES)
    {
    	printf("Error in get_config_request_data(): Number of Fapi message bytes exceeds allocated limit\n");
    	return;
    }

    char fapiHeader[9];
    memcpy(fapiHeader,fapiHexConfigReq,8);
	fapiHeader[8]='\0';
	if(0!=strcmp("01000200",fapiHeader))//0100 will be the message header, 0200 is message id
	{
		printf("Error in get_config_request_data(): Invalid Fapi message bytes\n");
		return;
	}

    // config_intial_value = (uint32_t *)((0x01300000) + get_inc_address());
	// ConfigRequest.init_headers.NumOfTLVs = (uint16_t) * ((uint16_t *)&config_intial_value[2]);

	// NumOfTLVs needs 16 bits instead of 8 bits (i.e. it is different from the fapi document format)
    ConfigRequest.init_headers.NumOfTLVs = (uint16_t) *((uint16_t*)(fapiBytesConfigReq+8));

     //xil_printf("temp = %x",config_data[0]);
//     xil_printf(" number of tags = %x \r\n",ConfigRequest.init_headers.NumOfTLVs);
     config_count = 10;
    while (counter < ConfigRequest.init_headers.NumOfTLVs)
    {
    	tlv = (uint32_t *)(fapiBytesConfigReq + config_count);
        tag = (uint16_t) * ((uint16_t *)&tlv[0]); // Tag is the first 16 bits of the 32 bit word.
        // Length is the next 16 bits of tlv1 which we dont care about (Since we know from the tag value)
        // tlv2 contains the values...
        val8 = (uint8_t) * ((uint8_t *)&tlv[1]);
        val16 = (uint16_t) * ((uint16_t *)&tlv[1]);
        val32 = tlv[1];
        ////xil_printf(" tag = %x,value = %x \r\n",tag,val16);
        switch (tag)
        {

        //------------------- Carrier config -------------------------
        case 0x1001:                             //param_tlv_16_16_16_t
            ConfigRequest.CarrierConfig.dlBandwidth.Value = val16; // 16 bits
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1002:                                               //param_tlv_16_16_32_t
            ConfigRequest.CarrierConfig.dlFrequency.Value = val32; // 32 bits of value.
            //xil_printf(" tag = %x,value = %x \r\n",tag,val32);
            break;

        case 0x1003: //param_tlv_16_16_16_t dlk0[5]: Represented by counter count_1003.
            ConfigRequest.CarrierConfig.dlk0[count_1003].Value = val16;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            count_1003 = count_1003 + 1; // Increment the counter if this tag ever comes again.
            break;

        case 0x1004: //param_tlv_16_16_16_t dlGridSize[5] Represented by counter count_1004;
            ConfigRequest.CarrierConfig.dlGridSize[count_1004].Value = val16;
            count_1004 = count_1004 + 1; // Increment the counter if this tag ever comes again.
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1005: //param_tlv_16_16_16_t  numTxAnt;
            ConfigRequest.CarrierConfig.numTxAnt.Value = val16;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1006: //param_tlv_16_16_16_t  uplinkBandwidth
            ConfigRequest.CarrierConfig.uplinkBandwidth.Value = val16;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1007: //param_tlv_16_16_32_t  uplinkFrequency
            ConfigRequest.CarrierConfig.uplinkFrequency.Value = val32;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val32);
            break;

        case 0x1008: //param_tlv_16_16_16_t  ulk0[5] represented by counter count_1008
            ConfigRequest.CarrierConfig.ulk0[count_1008].Value = val16;
            count_1008 = count_1008 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1009: //param_tlv_16_16_16_t  ulGridSize[5] ; represented by counter count_1009
            ConfigRequest.CarrierConfig.ulGridSize[count_1009].Value = val16;
            count_1009 = count_1009 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x100A: //	param_tlv_16_16_16_t  numRxAnt
            ConfigRequest.CarrierConfig.numRxAnt.Value = val16;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x100B: //	param_tlv_16_16_08_t  FrequencyShift7p5KHz
            ConfigRequest.CarrierConfig.FrequencyShift7p5KHz.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        //------------------ Cell config------------------------------------
        case 0x100C: //	param_tlv_16_16_08_t  phyCellId
            ConfigRequest.CellConfig.phyCellId.Value = val8;
                         //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x100D: //	param_tlv_16_16_08_t  FrameDuplexType;
            ConfigRequest.CellConfig.FrameDuplexType.Value = val8;
                         //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;
        //------------------- SSB Config -------------------------------------
        case 0x100E: //	param_tlv_16_16_32_t  ssPbchPower;
            ConfigRequest.SSBConfig.ssPbchPower.Value = val32;
                         //xil_printf(" tag = %x,value = %x \r\n",tag,val32);
            break;

        case 0x100F: //	param_tlv_16_16_08_t  BchPayload;
            ConfigRequest.SSBConfig.BchPayload.Value = val8;
                         //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1010: //	param_tlv_16_16_08_t  ScsCommon;
            ConfigRequest.SSBConfig.ScsCommon.Value = val8;
             //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;
            // -------------------- PRACHConfig --------------------------------

        case 0x1011: //	param_tlv_16_16_08_t prachSequenceLength;
            ConfigRequest.PRACHConfig.prachSequenceLength.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1012: //	param_tlv_16_16_08_t prachSubCSpacing
            ConfigRequest.PRACHConfig.prachSubCSpacing.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1013: //	param_tlv_16_16_08_t restrictedSetConfig
            ConfigRequest.PRACHConfig.restrictedSetConfig.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1014: //	param_tlv_16_16_08_t numPrachFdOccasions;
            ConfigRequest.PRACHConfig.numPrachFdOccasions.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1015: //	param_tlv_16_16_16_t prachRootSequenceIndex;
            ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1015].prachRootSequenceIndex.Value = val16;
            count_1015 = count_1015 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1016: //param_tlv_16_16_08_t numRootSequences;
            ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1016].numRootSequences.Value = val8;
            count_1016 = count_1016 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1017: //param_tlv_16_16_16_t k1
            ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1017].k1.Value = val16;
            count_1017 = count_1017 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x1018: //param_tlv_16_16_08_t prachZeroCorrConf
            ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1018].prachZeroCorrConf.Value = val8;
            count_1018 = count_1018 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1019: //param_tlv_16_16_08_t numUnusedRootSequences
            ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1019].numUnusedRootSequences.Value = val8;
            count_1019 = count_1019 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x101A: //param_tlv_16_16_08_t unusedRootSequences[MAX_numUnusedRootSequences]
            // 0x1019 and 0x101A are related very much and 0x1019 always happens before 0x101A
            ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1019 - 1].unusedRootSequences[count_101A].Value = val8;
            count_101A = (count_101A + 1) % (ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[count_1019 - 1].numUnusedRootSequences.Value);
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x101B: //param_tlv_16_16_08_t SsbPerRach;
            ConfigRequest.PRACHConfig.SsbPerRach.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x101C: //param_tlv_16_16_08_t prachMultipleCarriersInABand;
            ConfigRequest.PRACHConfig.prachMultipleCarriersInABand.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;
            // -------------------- SSB Table--------------------------------
        case 0x101D: //param_tlv_16_16_16_t	SsbOffsetPointA
            ConfigRequest.SSBTable.SsbOffsetPointA.Value = val16;
            //printf(" tag = %x,value = %x \r\n",tag,val16);
            break;

        case 0x101E: //param_tlv_16_16_08_t 	betaPss;
            ConfigRequest.SSBTable.betaPss.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x101F: //param_tlv_16_16_08_t 	SsbPeriod
            ConfigRequest.SSBTable.SsbPeriod.Value = val8;
            //printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1020: //param_tlv_16_16_08_t 	SsbSubcarrierOffset
            ConfigRequest.SSBTable.SsbSubcarrierOffset.Value = val8;
            //printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1021: //param_tlv_16_16_32_t	MIB;
            ConfigRequest.SSBTable.MIB.Value = val32;
            //printf(" tag = %x,value = %x \r\n",tag,val32);
            break;

        case 0x1022: //param_tlv_16_16_32_t  	SsbMask[2];
            ConfigRequest.SSBTable.SsbMask[count_1022].Value = val32;
            count_1022 = count_1022 + 1;
            //printf(" tag = %x,value = %x \r\n",tag,val32);
            break;

        case 0x1023: //param_tlv_16_16_08_t  	BeamId[64];
            ConfigRequest.SSBTable.BeamId[count_1023].Value = val8;
            count_1023 = count_1023 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1024: //param_tlv_16_16_08_t  	ssPbchMultipleCarriersInABand
            ConfigRequest.SSBTable.ssPbchMultipleCarriersInABand.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1025: //param_tlv_16_16_08_t  	multipleCellsSsPbchInACarrier
            ConfigRequest.SSBTable.multipleCellsSsPbchInACarrier.Value = val8;
            ////xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

            // -------------------- TDDTable--------------------------------

        case 0x1026: //param_tlv_16_16_08_t	TddPeriod;
            ConfigRequest.TDDTable.TddPeriod.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        case 0x1027: //param_tlv_16_16_08_t	SlotConfig[MAX_TDD_PERIODICITY*MAX_NUM_OF_SYMBOL_PER_SLOT]
            ConfigRequest.TDDTable.SlotConfig[count_1027].Value = val8;
            count_1027 = count_1027 + 1;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;
        // -------------------- MeasurementConfig--------------------------------
        case 0x1028: //param_tlv_16_16_08_t	RssiMeasurement
            ConfigRequest.MeasurementConfig.RssiMeasurement.Value = val8;
            //xil_printf(" tag = %x,value = %x \r\n",tag,val8);
            break;

        default:
            break;
        }
        config_count = config_count + 8;
        counter = counter + 1;
        //xil_printf("config_count = %d,counter = %d \r\r\n",config_count,counter);
    }
    //xil_printf("config request is done \r\n");
}
