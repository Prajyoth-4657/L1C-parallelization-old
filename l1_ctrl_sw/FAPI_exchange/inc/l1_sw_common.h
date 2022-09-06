#ifndef __L1_SW_COMMON_H
#define __L1_SW_COMMON_H

/** This file contains some common functions in the l1c sw version ***/

//#include "constants.h"
#include <inttypes.h>


#define MAX_FAPI_BYTES 10000 // Maximum number of bytes in the input Fapi config for a slot message
#define MAX_FAPI_STRING_LENGTH 10000 // Maximum length of the fapi hex string for a slot message

// Function to convert hexadecimal string inputs to decimal byte array
int convertHex2DecBytes(char* Hex_io, uint8_t* DecBytes_o);

// Function to extract hexadecimal message string corresponding to a particular slot and SFN from a fapi log file
int extractSlotMessageHexStringFromFapiLog(char* fapiLogFile_i, int SFN_i, int slot_i, char* fapiHexSlotMessage_io);


#endif
