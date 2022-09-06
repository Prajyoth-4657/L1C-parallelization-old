#ifndef __SDFECA_H__
#define __SDFECA_H__

#include "ap_int.h"
#include "hls_stream.h"
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

using namespace std;

typedef ap_uint<16> user_data_t;
typedef ap_uint<16> keep_data_t;
typedef ap_uint<128> data_t;

typedef struct stream_axi_t {
	data_t tdata;
	user_data_t tuser;
	keep_data_t tkeep;
	ap_uint<1> tlast;
} stream_axi_t;

typedef struct cnfg_axi_t {
	ap_uint<40> tdata;
} cnfg_axi_t;

typedef enum sdfec_state_t {
	INIT,
	CNFG,
	PASS
} sdfec_state_t;

void sdfec_aux(hls::stream<stream_axi_t> &data_in, hls::stream<cnfg_axi_t> &cnfg_in, hls::stream<stream_axi_t> &data_out);

#endif
