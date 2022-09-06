#ifndef __L1_UL_DCI_REQUEST_H
#define __L1_UL_DCI_REQUEST_H

#include "constants.h"
#include <inttypes.h>
#include "l1_dl_tti_request.h"

void get_ul_dci_request_data();

typedef struct UL_DCI_PDU_t{
	uint16_t	PDUType;
	uint16_t	PDUSize;
} UL_DCI_PDU_t;


typedef struct UL_dci_Request_t{
	uint16_t	MsgId;
	uint32_t 	MsgLength;
	uint16_t	SFN;
	uint16_t	Slot;
	uint8_t		nPDUs;
	UL_DCI_PDU_t	UL_DCI_PDU;
	PDCCH_PDU_t PDCCH_PDU[MaxPDCCH_PDU];
} UL_dci_Request_t;

#endif