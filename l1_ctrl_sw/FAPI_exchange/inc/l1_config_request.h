/*
	* param_l1_1.h
	*
	*	Created on: 7-June-2020
	*		Author: RK G
	*/
#ifndef __L1_CONFIG_REQUEST_H
#define __L1_CONFIG_REQUEST_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "tlv.h"
#include "l1_config.h"
#include "fapi_dma_config.h"
//#include "xil_printf.h"

//#pragma pack(1)


void get_config_request_data();




typedef struct CarrierConfig_t{ // No packing required
	param_tlv_16_16_16_t	dlBandwidth;				//TAG = 0x1001
	param_tlv_16_16_32_t	dlFrequency;				//TAG = 0x1002
	param_tlv_16_16_16_t	dlk0[5];					//TAG = 0x1003
	param_tlv_16_16_16_t	dlGridSize[5] ;				//TAG = 0x1004
	param_tlv_16_16_16_t	numTxAnt;					//TAG = 0x1005
	param_tlv_16_16_16_t	uplinkBandwidth;			//TAG = 0x1006
	param_tlv_16_16_32_t	uplinkFrequency;			//TAG = 0x1007
	param_tlv_16_16_16_t	ulk0[5] ;					//TAG = 0x1008
	param_tlv_16_16_16_t	ulGridSize[5] ;				//TAG = 0x1009
	param_tlv_16_16_16_t	numRxAnt ;					//TAG = 0x100A
	param_tlv_16_16_08_t	FrequencyShift7p5KHz ;		//TAG = 0x100B
} CarrierConfig_t;

typedef struct CellConfig_t{// No packing required
	param_tlv_16_16_08_t	phyCellId;					//TAG = 0x100C
	param_tlv_16_16_08_t	FrameDuplexType;			//TAG = 0x100D
} CellConfig_t;

typedef struct SSBConfig_t{// No packing required
	param_tlv_16_16_32_t	ssPbchPower;		//TAG = 0x100E
	param_tlv_16_16_08_t	BchPayload;			//TAG = 0x100F
	param_tlv_16_16_08_t	ScsCommon;			//TAG = 0x1010
} SSBConfig_t;

typedef struct numPRACHFdOccasions_data_t{
	param_tlv_16_16_16_t prachRootSequenceIndex; //TAG = 0x1015
	param_tlv_16_16_08_t numRootSequences;		//TAG = 0x1016
	param_tlv_16_16_16_t k1;					//TAG = 0x1017
	param_tlv_16_16_08_t prachZeroCorrConf;		//TAG = 0x1018
	param_tlv_16_16_08_t numUnusedRootSequences;	//TAG = 0x1019
	param_tlv_16_16_08_t unusedRootSequences[MAX_numUnusedRootSequences]; //TAG = 0x101A
}numPRACHFdOccasions_data_t;

typedef struct PRACHConfig_t{
	param_tlv_16_16_08_t prachSequenceLength;	//TAG = 0x1011
	param_tlv_16_16_08_t prachSubCSpacing;		//TAG = 0x1012
	param_tlv_16_16_08_t restrictedSetConfig;	//TAG = 0x1013
	param_tlv_16_16_08_t numPrachFdOccasions;	//TAG = 0x1014
	numPRACHFdOccasions_data_t numPRACHFdOccasions_data[MAX_numPrachFdOccasions];
	param_tlv_16_16_08_t SsbPerRach; //TAG = 0x101B
	param_tlv_16_16_08_t prachMultipleCarriersInABand;//TAG = 0x101C
}PRACHConfig_t;



typedef struct SSBTable_t{
param_tlv_16_16_16_t	SsbOffsetPointA; //TAG = 0x101D
param_tlv_16_16_08_t 	betaPss;		//TAG = 0x101E
param_tlv_16_16_08_t 	SsbPeriod;		//TAG = 0x101F
param_tlv_16_16_08_t 	SsbSubcarrierOffset; //TAG = 0x1020
param_tlv_16_16_32_t	MIB;			//TAG = 0x1021
param_tlv_16_16_32_t	SsbMask[2];		//TAG = 0x1022
param_tlv_16_16_08_t	BeamId[64];		//TAG = 0x1023
param_tlv_16_16_08_t	ssPbchMultipleCarriersInABand;	//TAG = 0x1024
param_tlv_16_16_08_t	multipleCellsSsPbchInACarrier;	//TAG = 0x1025
} SSBTable_t;



typedef struct TDDTable_t{
param_tlv_16_16_08_t	TddPeriod; //TAG = 0x1026
param_tlv_16_16_08_t	SlotConfig[MAX_TDD_PERIODICITY*MAX_NUM_OF_SYMBOL_PER_SLOT];	//TAG = 0x1027
} TDDTable_t;

typedef struct MeasurementConfig_t{
param_tlv_16_16_08_t	RssiMeasurement; //TAG = 0x1028
} MeasurementConfig_t;

typedef struct config_init_headers_t{
	uint8_t no_of_PHY_API_MSGS;
	uint8_t FAPI_HANDLE;
	uint16_t	MsgId ;
	uint32_t    MsgLength;
	uint16_t     NumOfTLVs; //changed from u8 to u16
//	uint8_t    Pad0;
//	uint8_t 	Pad1;
}config_init_headers_t;

typedef struct ConfigRequest_t{
config_init_headers_t	init_headers;// size is 8 bytes // 2 tlv
CarrierConfig_t CarrierConfig;//size is 216 bytes //54 tlv
CellConfig_t	CellConfig;
SSBConfig_t 	SSBConfig;
PRACHConfig_t	PRACHConfig;
SSBTable_t		SSBTable;
TDDTable_t		TDDTable;
MeasurementConfig_t MeasurementConfig;
} ConfigRequest_t ;



#endif
