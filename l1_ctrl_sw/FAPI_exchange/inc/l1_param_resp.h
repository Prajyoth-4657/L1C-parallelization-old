/*
	* param_l1_1.h
	*
	*	Created on: 25-MAY-2020
	*		Author: Rahul Dev
	*/
#ifndef __L1_PARAM_RESP_H
#define __L1_PARAM_RESP_H

#include <stdlib.h>
#include <stdint.h>
//include "mb_interface.h"
#include <inttypes.h>
#include "tlv.h"
#include "l1_config.h"






void initialize_param_resp_data();
void l1_tx_ParamResp();

//For PARAM MESSAGE EXCHANGES

typedef struct init_headers_t{
	uint16_t	MsgId ;
	uint16_t	MsgLength;
	uint8_t		ErrorCode;
	uint8_t		NumOfTLVs;
	uint16_t	Pad;
}init_headers_t;


typedef struct CellParam_t{
	param_tlv_16_16_16_t	ReleaseCapability;		//TAG = 0x0001
	param_tlv_16_16_16_t	PHYState;				//TAG = 0x0002
	param_tlv_16_16_08_t	Skip_blank_DL_CONFIG ;	//TAG = 0x0003
	param_tlv_16_16_08_t	Skip_blank_UL_CONFIG ;	//TAG = 0x0004
	param_tlv_16_16_16_t	NumConfigTLVsToReport;	// TAG = 0x0005
	param_tlv_16_08_08_t	ConfigTLVsToReport[MaxTLV_PARAM];
} CellParam_t;

typedef struct CarrierParam_t{
	param_tlv_16_16_08_t	cyclicPrefix;					//TAG = 0x0006
	param_tlv_16_16_08_t	supportedSubcarrierSpacingsDl;	//TAG = 0x0007
	param_tlv_16_16_16_t	supportedBandwidthDl;			//TAG = 0x0008
	param_tlv_16_16_08_t	supportedSubcarrierSpacingsUl;	//TAG = 0x0009
	param_tlv_16_16_16_t	supportedBandwidthUl;			//TAG = 0x000A
} CarrierParam_t;

typedef struct PDCCHParam_t{
	param_tlv_16_16_08_t	cceMappingType;						//TAG = 0x000B
	param_tlv_16_16_08_t	coresetOutsideFirst3OfdmSymsOfSlot; //TAG = 0x000C
	param_tlv_16_16_08_t	precoderGranularityCoreset;			//TAG = 0x000D
	param_tlv_16_16_08_t	pdcchMuMimo;						//TAG = 0x000E
	param_tlv_16_16_08_t	pdcchPrecoderCycling;				//TAG = 0x000F
	param_tlv_16_16_08_t	maxPdcchsPerSlot;					//TAG = 0x0010
} PDCCHParam_t;

typedef struct PUCCHParam_t{
	param_tlv_16_16_08_t pucchFormats;	//TAG = 0x0011
	param_tlv_16_16_08_t maxPucchsPerSlot; //TAG = 0x0012
} PUCCHParam_t;

typedef struct PDSCHParam_t{
	param_tlv_16_16_08_t pdschMappingType;				//TAG = 0x0013
	param_tlv_16_16_08_t pdschAllocationTypes;			//TAG = 0x0014
	param_tlv_16_16_08_t pdschVrbToPrbMapping;			//TAG = 0x0015
	param_tlv_16_16_08_t pdschCbg; 						//TAG = 0x0016
	param_tlv_16_16_08_t pdschDmrsConfigTypes; 			//TAG = 0x0017
	param_tlv_16_16_08_t pdschDmrsMaxLength;			//TAG = 0x0018
	param_tlv_16_16_08_t pdschDmrsAdditionalPos;		//TAG = 0x0019
	param_tlv_16_16_08_t maxPdschsTBsPerSlot;			//TAG = 0x001A
	param_tlv_16_16_08_t maxNumberMimoLayersPdsch;		//TAG = 0x001B
	param_tlv_16_16_08_t supportedMaxModulationOrderDl;	//TAG = 0x001C
	param_tlv_16_16_08_t maxMuMimoUsersDl;				//TAG = 0x001D
	param_tlv_16_16_08_t pdschDataInDmrsSymbols;		//TAG = 0x001E
	param_tlv_16_16_08_t premptionSupport;				//TAG = 0x001F
	param_tlv_16_16_08_t pdschNonSlotSupport;			//TAG = 0x0020
} PDSCHParam_t;

typedef struct PUSCHParam_t{
	param_tlv_16_16_08_t	uciMuxUlschInPusch;				//TAG = 0x0021
	param_tlv_16_16_08_t	uciOnlyPusch;					//TAG = 0x0022
	param_tlv_16_16_08_t	puschFrequencyHopping;			//TAG = 0x0023
	param_tlv_16_16_08_t	puschDmrsConfigTypes;			//TAG = 0x0024
	param_tlv_16_16_08_t	puschDmrsMaxLen;				//TAG = 0x0025
	param_tlv_16_16_08_t 	puschDmrsAdditionalPos;			//TAG = 0x0026
	param_tlv_16_16_08_t 	puschCbg;						//TAG = 0x0027
	param_tlv_16_16_08_t 	puschMappingType;				//TAG = 0x0028
	param_tlv_16_16_08_t 	puschAllocationTypes;			//TAG = 0x0029
	param_tlv_16_16_08_t 	puschVrbToPrbMapping;			//TAG = 0x002A
	param_tlv_16_16_08_t 	puschMaxPtrsPorts;				//TAG = 0x002B
	param_tlv_16_16_08_t 	maxPduschsTBsPerSlot;			//TAG = 0x002C
	param_tlv_16_16_08_t 	maxNumberMimoLayersNonCbPusch;	//TAG = 0x002D
	param_tlv_16_16_08_t 	supportedModulationOrderUl;		//TAG = 0x002E
	param_tlv_16_16_08_t 	maxMuMimoUsersUl;				//TAG = 0x002F
	param_tlv_16_16_08_t 	dftsOfdmSupport;				//TAG = 0x0030
	param_tlv_16_16_08_t 	puschAggregationFactor;			//TAG = 0x0031
} PUSCHParam_t;

typedef struct PRACHParam_t{
	param_tlv_16_16_08_t prachLongFormats;			//TAG = 0x0032
	param_tlv_16_16_08_t prachShortFormats;			//TAG = 0x0033
	param_tlv_16_16_08_t prachRestrictedSets;		//TAG = 0x0034
	param_tlv_16_16_08_t maxPrachFdOccasionsInASlot; //TAG = 0x0035
} PRACHParam_t;

typedef struct MeasurementParam_t{
	param_tlv_16_16_08_t RssiMeasurementSupport;	//TAG = 0x0036
} MeasurementParam_t;


typedef struct ParamResponse_t{
	init_headers_t init_headers;
	CellParam_t CellParam;
	CarrierParam_t CarrierParam;
	PDCCHParam_t PDCCHParam;
	PUCCHParam_t PUCCHParam;
	PDSCHParam_t PDSCHParam;
	PUSCHParam_t PUSCHParam;
	PRACHParam_t PRACHParam;
	MeasurementParam_t MeasurementParam;
} ParamResponse_t;


#endif
