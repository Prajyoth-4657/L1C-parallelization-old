#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "l1_sw_common.h"

/** This file contains some common functions in the l1c sw version ***/


char fapiHexSlotMessage[10000]; // buffer to store one slot message


/*
Function: convertHex2DecBytes
Author: Vishnu P

	Function to convert hexadecimal string inputs to decimal byte array

	Inputs:
		Hex_io - character array containing hex string
		DecBytes_o - This is the pointer to the output decimal byte array

	Output:
		The number of bytes Output

*/
int convertHex2DecBytes(char* Hex_io, uint8_t* DecBytes_o)
{
    int numBytes = 0;

    /* remove spaces and compute number of bytes */
    // To keep track of non-space character count
    int count = 0;

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; Hex_io[i]; i++)
        if (Hex_io[i] != ' ')
            Hex_io[count++] = Hex_io[i]; // here count is
                                         // incremented
    Hex_io[count] = '\0';

    /* convert hex byte strings to decimal bytes */
    numBytes = strlen(Hex_io)/2;
    int val;
    char a,a2;
    for(int i=0; i < numBytes;++i)
    {
        // Convert hex strings to decimal
        a= Hex_io[i*2];
        a2 = Hex_io[i*2+1];
        val = ((((a <= '9') ? a - '0' : (a & 0x7) + 9)<<4) + ((a2 <= '9') ? a2 - '0' : (a2 & 0x7) + 9));
        DecBytes_o[i] = (uint8_t)val;
    }
    return numBytes;
}

/*
Function: findCharPos
Author: Vishnu P

	Function to find  positions of a character in the string

	Inputs:
		InputStr_i              : Input string
		chToFind_i              : character to find
		numCnt_i                : number of occurrences of character to find
		pos_array_io            : position array to be filled (pointer)
	Output:
		fapiHexSlotMessage_io   : position array to be filled (pointer)
        numPos                  : number of occurrences/output positions found
*/
int findCharPos(char* InputStr_i, char chToFind_i, int numCnt_i, int* pos_array_io)
{
    int numPos = 0;
    // Traverse the given string and find the character
    for (int i = 0; InputStr_i[i]; i++){
    	if( InputStr_i[i] == chToFind_i ){
    		pos_array_io[numPos++] = i;
    		if(numPos >= numCnt_i)
    			break;
    	}
    }
    return numPos;
}

// Function to extract value of the integer part from the given character array
// Position and length of the integer part are also taken as input
int extractIntPartFromString(char *strInput_i, int startPosIntPart, int lengthIntPart){
	int outValue, outValue2;
	int MaxLengthOfIntString = 20;
	char intStr[MaxLengthOfIntString];
	strncpy(intStr, strInput_i + startPosIntPart, lengthIntPart);
	intStr[lengthIntPart]='\0';
	sscanf(intStr, "%d", &outValue);
	outValue2 = atoi(intStr);
	if(outValue != outValue2){
		printf("string to int conversion mismatch: string = %s, val1=%d, val2=%d \n",intStr, outValue, outValue2 );
	}
}


/*
Function: extractSlotMessageHexStringFromFapiLog
Author: Vishnu P

	Function to extract hexadecimal message string corresponding to a particular slot and SFN from a fapi log file

	Inputs:
		fapiLogFile_i           : Fapi log file in text format (sooktha log format)
		SFN_i                   : System frame number in the message
		slot_i                  : Slot number in the message
		fapiHexSlotMessage_io   : Allocated character array  for output slotmessage in hex string format.
		                          Allocated length of fapiHexSlotMessage_io should be MAX_FAPI_STRING_LENGTH.
	Output:
		fapiHexSlotMessage_io   : Output slotmessage in hex string format
        outLen                  : Length of the output fapiHexSlotMessage_io string
*/
int extractSlotMessageHexStringFromFapiLog(char* fapiLogFile_i, int SFN_i, int slot_i, char* fapiHexSlotMessage_io)
{
	int outLen = 0, nLineCnt = 0;
	FILE* fpLog = fopen(fapiLogFile_i, "r"); // "r" for opening text file
	/* opening file for reading */
    if(fpLog == NULL) {
    	printf("Error opening log file: %s", fapiLogFile_i);
		return(outLen);
    }

    char strLine[MAX_FAPI_STRING_LENGTH];
    int lenStr;
    int spacePos[6];
    int outPos;
    char tmpStr[20];
    int SFN, slot;
    fapiHexSlotMessage_io[0]='\0';
    while(fgets(strLine, MAX_FAPI_STRING_LENGTH, fpLog) != NULL){
    	nLineCnt++;
    	lenStr = strlen(strLine);
    	if(lenStr > 1){
    		//printf("Length=%d, String=%s", lenStr, strLine);
    		outPos = findCharPos(strLine,' ', 6, spacePos); // find 6 spaces
    		if(outPos != 6){
    			printf("File read error: Invalid log file entry at line = %d, %s ", nLineCnt, strLine);
    			return(0);
    		}

    		// error checking
    		if(*(strLine+spacePos[0]+2) != 'S'){ // 'S' in SFN
    			printf("File read error: Couldn't find [SFN=....] in log file entry at line = %d, %s ", nLineCnt, strLine);
    			return(0);
    		}

    		// SFN
    		SFN = extractIntPartFromString(strLine, spacePos[1]+1, spacePos[2]-spacePos[1]-2);

    		// Slot
    		slot = extractIntPartFromString(strLine, spacePos[3]+1, spacePos[4]-spacePos[3]-2);

    		if( (SFN_i < SFN) || (SFN == SFN_i && slot_i < slot)){
    			printf("Error: Log entry starts at SFN = %d, slot = %d \n", SFN, slot);
    			break;
    		}
    		else if(SFN_i == SFN && slot_i == slot && (*(strLine+spacePos[4]+1) != 'T')){ // 'T' in TX_DATA.REQ, ignore it
    			strcpy(fapiHexSlotMessage_io,strLine+spacePos[5]);
    			outLen = strlen(fapiHexSlotMessage_io);
    			if(fapiHexSlotMessage_io[outLen-1]=='\n'){
    				outLen = outLen-1;
    				fapiHexSlotMessage_io[outLen]='\0';
    			}
    			printf("SFN=%d, Slot = %d, Message data=%s\n", SFN, slot, fapiHexSlotMessage_io);
    			break;
    		}
    		else{
    			// continue loop
    		}

    		//printf("[SFN = %d, Slot = %d] : %s \n", SFN, slot, fapiHexSlotMessage_io);

    	}
//    	if(nLineCnt > 10){
//    		break;
//    	}
    }
    if(outLen <= 0){
    	printf("Error: Could n't find message for SFN = %d, slot = %d\n", SFN_i, slot_i);
    }
    fclose(fpLog);
    return(outLen);
}


