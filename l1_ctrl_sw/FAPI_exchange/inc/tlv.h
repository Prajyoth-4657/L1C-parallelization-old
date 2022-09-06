#ifndef __TLV_H
#define __TLV_H

typedef struct param_tlv_16_16_16_t{
	uint16_t Tag;
	uint16_t Length;
	uint16_t Value;
	uint16_t Pad0;
}param_tlv_16_16_16_t;

typedef struct param_tlv_16_16_08_t{
	uint16_t Tag;
	uint16_t Length;
	uint8_t	Value;
	uint8_t	Pad0;
	uint16_t Pad1;
}param_tlv_16_16_08_t;

typedef struct param_tlv_16_08_08_t{
	uint16_t Tag;
	uint8_t Length;
	uint8_t Value;
}param_tlv_16_08_08_t;

typedef struct param_tlv_16_16_32_t{	
	uint16_t Tag;
	uint16_t Length;
	uint32_t Value;
	uint8_t	Pad0;
} param_tlv_16_16_32_t;

typedef struct param_tlv_16_08_16_t{	// not being used till now// Might require packed directive.
	uint16_t Tag;
	uint8_t Length;
	uint16_t Value;
	uint8_t Pad0;
	uint16_t Pad1;
}param_tlv_16_08_16_t;




#endif
