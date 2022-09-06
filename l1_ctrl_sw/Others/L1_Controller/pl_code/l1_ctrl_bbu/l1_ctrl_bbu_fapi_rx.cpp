#include "l1_control.h"

extern l1_ctrl_inst_t l1_ctrl_inst;
extern pdsch_l1_ctrl_t pdsch_l1_ctrl[NUM_USER_CW];

void fapi_rx(hls::stream<fapi_axis> &fapi_in, wire_type &state_fapi_rx, bit_wire sfn_ctrl_clk, bool &fapi_done)
{

//#pragma HLS PIPELINE

	fapi_axis data_in;
//	data_axis dout;
	static ap_uint<8> cur_user = 0;       //** ?
	static bit_wire prev_sfn_ctrl_clk;
	static enum FAPI_RX_STATE fapi_rx_state = FAPI_RX_INIT;
	static ap_uint<9> cur_sched_ind = 0;          //** ?
//	static ecpri_rx_instance_par ecpri_rx_instance_params;
	state_fapi_rx = fapi_rx_state;

	switch (fapi_rx_state) {

	case FAPI_RX_INIT:

		fapi_rx_state = CTRL_FAPI_RCV_0;
		fapi_done = 0;

		break;

	case FAPI_RX_WAIT:

		fapi_rx_state = CTRL_FAPI_RCV_0;
		cur_sched_ind = 0;
		cur_user = 0;
		fapi_done = 0;

		break;
//1.
	case CTRL_FAPI_RCV_0 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();
			// nan
			l1_ctrl_inst.n_users = data_in.data.range(7, 0);  //l1_ctrl_bbu_l1_ctrl.h
			// nan
			l1_ctrl_inst.param_calc_tb_dirt_bit = 0;  //** ?
			// nan
			l1_ctrl_inst.param_calc_rm_dirt_bit = 0;
			// nan
			l1_ctrl_inst.max_cb_sched = data_in.data.range(18, 10);
			// nan
			l1_ctrl_inst.cur_phy_sched = data_in.data.range(27, 19);
			// nan
			// l1_ctrl_inst.cw_phy_index_sched[512] = data_in.data.range(35,28);


			fapi_rx_state = CTRL_FAPI_RCV_1 ;
			fapi_done = 0;
		}
		break;
// 2.
	case CTRL_FAPI_RCV_1 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// Bitmap indicating presence of optional PDUsBit 0: pdschPtrs  -IndicatesPTRS included (FR2) Bit 1:cbgRetxCtrl (Present when CBG based retransmit is used)All other bits reserved
			pdsch_l1_ctrl[cur_user].pdsch_pdu.pduBitmap = data_in.data.range(15, 0);
			// The RNTI used for identifying the UE when receiving the PDUValue: 1 -> 65535
			pdsch_l1_ctrl[cur_user].pdsch_pdu.RNTI = data_in.data.range(31, 16);  //MMM //UUU
			// PDU index incremented for each PDSCH PDU sent in TX control message. This is used to associate control information to data and is reset every slot. Value:0 ->65535
			pdsch_l1_ctrl[cur_user].pdsch_pdu.pduIndex = data_in.data.range(47, 32);
			// Bandwidthpart size[TS38.213 sec12]. Number of contiguous PRBs allocated to the BWPValue: 1->275
			pdsch_l1_ctrl[cur_user].pdsch_pdu.BWPSize = data_in.data.range(63, 48);
			fapi_rx_state = CTRL_FAPI_RCV_2 ;
			fapi_done = 0;
		}
		break;
//3.
	case CTRL_FAPI_RCV_2 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// bandwidth part start RB indexfrom reference CRB [TS38.213 sec 12]Value: 0->274
			pdsch_l1_ctrl[cur_user].pdsch_pdu.BWPStart = data_in.data.range(15, 0);
			// subcarrierSpacing [TS38.211 sec 4.2]Value:0->4
			pdsch_l1_ctrl[cur_user].pdsch_pdu.SubcarrierSpacing = data_in.data.range(23, 16);
			// Cyclic prefix type[TS38.211 sec 4.2]0: Normal; 1: Extended
			pdsch_l1_ctrl[cur_user].pdsch_pdu.CyclicPrefix = data_in.data.range(31, 24);
			// Number ofcodewordsfor this RNTI (UE)Value: 1 -> 2
			pdsch_l1_ctrl[cur_user].pdsch_pdu.NrOfCodewords = data_in.data.range(39, 32);  ////MMM
			// Target coding rate [TS38.212sec5.4.2.1and 38.214 sec 5.1.3.1]. This is the number of information bits per 1024 coded bits expressed in 0.1 bit units
			pdsch_l1_ctrl[cur_user].pdsch_pdu.targetCodeRate = data_in.data.range(55, 40);  //yy TT
			// QAM modulation[TS38.212sec5.4.2.1and 38.214 sec 5.1.3.1] Value: 2,4,6,8
			pdsch_l1_ctrl[cur_user].pdsch_pdu.qamModOrder = data_in.data.range(63, 56); //TT //HH //BBB //UUU
			fapi_rx_state = CTRL_FAPI_RCV_3 ;
			fapi_done = 0;
		}
		break;
//4.
	case CTRL_FAPI_RCV_3 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// MCS index[TS38.214, sec 5.1.3.1], should match value sent in DCIValue : 0->31
			pdsch_l1_ctrl[cur_user].pdsch_pdu.mcsIndex = data_in.data.range(7, 0);
			// MCS-Table-PDSCH[TS38.214, sec 5.1.3.1]0: notqam2561: qam2562: qam64LowSE
			pdsch_l1_ctrl[cur_user].pdsch_pdu.mcsTable = data_in.data.range(15, 8);
			// Redundancy versionindex [TS38.212, Table 5.4.2.1-2 and 38.214, Table 5.1.2.1-2], should match value sent in DCIValue : 0->3
			pdsch_l1_ctrl[cur_user].pdsch_pdu.rvIndex = data_in.data.range(23, 16);
			// Transmit block size (in bits)[TS38.214 sec 5.1.3.2]Value: 0->65535
			pdsch_l1_ctrl[cur_user].pdsch_pdu.TBSize = data_in.data.range(55, 24);  //yy //TT
			fapi_rx_state = CTRL_FAPI_RCV_4 ;
			fapi_done = 0;
		}
		break;
//5.
	case CTRL_FAPI_RCV_4 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// dataScramblingIdentityPdsch [TS38.211, sec 7.3.1.1]It equals the higher-layer parameter Data-scrambling-Identity if configured and the RNTI equals the C-RNTI, otherwise L2 needs to set it to physical cell id.Value: 0->65535
			pdsch_l1_ctrl[cur_user].pdsch_pdu.dataScramblingId = data_in.data.range(15, 0); //MMM
			// Number of layers [TS38.211, sec 7.3.1.3]Value : 1->8
			pdsch_l1_ctrl[cur_user].pdsch_pdu.nrOfLayers = data_in.data.range(23, 16); //TT
			// PDSCH transmission schemes [TS38.214, sec 5.1.1]0: Up to 8 transmission layers
			pdsch_l1_ctrl[cur_user].pdsch_pdu.transmissionScheme = data_in.data.range(31, 24);
			// Reference point forPDSCH DMRS "k" -used for tone mapping [TS38.211, sec 7.4.1.1.2]Resource block bundles [TS38.211, sec 7.3.1.6]Value: 0 -> 1If 0, the 0 reference point for PDSCH DMRSis at Point A [TS38.211 sec 4.4.4.2]. Resource block bundles generated per sub-bullets 2 and 3 in [TS38.211, sec 7.3.1.6]. For sub-bullet 2, the start of bandwidth part must be set to the start of actual bandwidth part +NstartCORESETand the bandwidth of the bandwidth part must be set to the bandwidth of the initial bandwidth part.If 1, the DMRS reference point is at the lowest VRB/PRB of the allocation. Resource block bundles generated per sub-bullets 1 [TS38.211, sec7.3.1.6]
			pdsch_l1_ctrl[cur_user].pdsch_pdu.refPoint = data_in.data.range(39, 32);
			// DMRS symbol positions [TS38.211, sec7.4.1.1.2andTables7.4.1.1.2-3 and 7.4.1.1.2-4]Bitmapoccupying the14 LSBswith: bit 0: first symboland for each bit0: no DMRS1: DMRS
			pdsch_l1_ctrl[cur_user].pdsch_pdu.dlDmrsSymbPos = data_in.data.range(55, 40);   //KKKK
			// DL DMRS config type[TS38.211, sec 7.4.1.1.2]0: type 11: type2
			pdsch_l1_ctrl[cur_user].pdsch_pdu.dmrsConfigType = data_in.data.range(63, 56);  //KKKK
			fapi_rx_state = CTRL_FAPI_RCV_5 ;
			fapi_done = 0;
		}
		break;
//6.
	case CTRL_FAPI_RCV_5 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// DL-DMRS-Scrambling-ID [TS38.211, sec 7.4.1.1.2 ]If provided by the higher-layer and the PDSCH is scheduled by PDCCH with CRC scrambled by C-RNTI or CS-RNTI, otherwise, L2 should set thisto physical cell id.Value: 0->65535
			pdsch_l1_ctrl[cur_user].pdsch_pdu.dlDmrsScramblingId = data_in.data.range(15, 0);  //KKKK
			// DMRS sequence initialization [TS38.211, sec 7.4.1.1.2]. Should match what is sent in DCI 1_1, otherwise set to 0.Value : 0->1
			pdsch_l1_ctrl[cur_user].pdsch_pdu.SCID = data_in.data.range(23, 16); //KKKK
			// Number of DM-RS CDM groups without data [TS38.212 sec 7.3.1.2.2] [TS38.214 Table 4.1-1]it determines the ratio of PDSCH EPRE to DM-RS EPRE.Value: 1->3
			pdsch_l1_ctrl[cur_user].pdsch_pdu.numDmrsCdmGrpsNoData = data_in.data.range(31, 24);
			// DMRS ports.[TS38.212 7.3.1.2.2] provides description between DCI 1-1 content and DMRS ports.Bitmap occupying the 11 LSBswith: bit 0: antenna port 1000bit 11: antenna port 1011and for each bit0: DMRS port not used1: DMRS port used
			pdsch_l1_ctrl[cur_user].pdsch_pdu.dmrsPorts = data_in.data.range(47, 32);
			// Resource Allocation Type [TS38.214, sec 5.1.2.2]0: Type 01: Type 1
			pdsch_l1_ctrl[cur_user].pdsch_pdu.resourceAlloc = data_in.data.range(55, 48);
			// For resource alloc type 0.TS 38.212 V15.0.x, 7.3.1.2.2 bitmap of RBs, 273 rounded up to multiple of 32. This bitmap is in units of VRBs. LSB of byte 0 of the bitmap represents the first RB of the bwp
			//pdsch_l1_ctrl[cur_user].pdsch_pdu.rbBitmap[36] = data_in.data.range(55,56);
			fapi_rx_state = CTRL_FAPI_RCV_6 ;
			fapi_done = 0;
		}
		break;
//7.
	case CTRL_FAPI_RCV_6 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// For resource allocationtype 1. [TS38.214, sec 5.1.2.2.2]The starting resource block within the BWP for this PDSCH.Value: 0->274
			pdsch_l1_ctrl[cur_user].pdsch_pdu.rbStart = data_in.data.range(15, 0); ////KKKK
			// For resource allocation type 1. [TS38.214, sec 5.1.2.2.2]The number of resource block within for this PDSCH Value: 1->275
			pdsch_l1_ctrl[cur_user].pdsch_pdu.rbSize = data_in.data.range(31, 16);//KKKK
			// VRB-to-PRB-mapping[TS38.211, sec 7.3.1.6]0: non-interleaved 1: interleaved with RB size 22: Interleaved with RB size 4
			pdsch_l1_ctrl[cur_user].pdsch_pdu.VRBtoPRBMapping = data_in.data.range(39, 32);
			// Start symbol index of PDSCH mappingfrom the start of the slot, S. [TS38.214, Table 5.1.2.1-1]Value: 0->13
			pdsch_l1_ctrl[cur_user].pdsch_pdu.StartSymbolIndex = data_in.data.range(47, 40);//TT
			// PDSCH duration in symbols, L [TS38.214, Table 5.1.2.1-1]Value: 1->14
			pdsch_l1_ctrl[cur_user].pdsch_pdu.NrOfSymbols = data_in.data.range(55, 48);//TT
			// PT-RS antenna ports [TS38.214, sec 5.1.6.3] [TS38.211, table 7.4.1.2.2-1]Bitmap occupying the 6LSBswith: bit 0: antenna port 1000bit 5: antenna port 1005and for each bit0: PTRSport not used1: PTRSport used
			pdsch_l1_ctrl[cur_user].pdsch_pdu.PTRSPortIndex = data_in.data.range(63, 56);
			fapi_rx_state = CTRL_FAPI_RCV_7 ;
			fapi_done = 0;
		}
		break;
//8.
	case CTRL_FAPI_RCV_7 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// PT-RS time density[TS38.214, table 5.1.6.3-1]0: 11: 22: 4
			pdsch_l1_ctrl[cur_user].pdsch_pdu.PTRSTimeDensity = data_in.data.range(7, 0);
			// PT-RS frequency density[TS38.214, table 5.1.6.3-2]0: 21: 4
			pdsch_l1_ctrl[cur_user].pdsch_pdu.PTRSFreqDensity = data_in.data.range(15, 8);
			// PT-RS resource elementoffset[TS38.211, table 7.4.1.2.2-1]Value: 0->3
			pdsch_l1_ctrl[cur_user].pdsch_pdu.PTRSReOffset = data_in.data.range(23, 16);
			// PT-RS-to-PDSCH EPRE ratio [TS38.214, table 4.1-2]Value :0->3
			pdsch_l1_ctrl[cur_user].pdsch_pdu.nEpreRatioOfPDSCHToPTRS = data_in.data.range(31, 24);
			// See Table 3-43 for structure, NOT IMPLEMENTED
			//pdsch_l1_ctrl[cur_user].pdsch_pdu.Precoding and Beamforming = data_in.data.range(32,32);
			// Ratio of PDSCH EPRE to NZP CSI-RSEPRE [TS38.214, sec 5.2.2.3.1]Value :0->23representing -8 to 15 dB in 1dB steps
			pdsch_l1_ctrl[cur_user].pdsch_pdu.powerControlOffset = data_in.data.range(40, 33);
			// Ratio of SSB/PBCH block EPRE to NZP CSI-RS EPRES [TS38.214, sec 5.2.2.3.1] Values:0: -3dB,1: 0dB,2: 3dB,3: 6dB
			pdsch_l1_ctrl[cur_user].pdsch_pdu.powerControlOffsetSS = data_in.data.range(48, 41);
			// Indicates whether last CB is present in the CBG retransmission 0: CBG retransmission does not include last CB 1: CBG retransmission includes last CB If CBG Re-Tx includes last CB, L1 will add the TB CRC to the last CB in the payload before it is read into the LDPC HW unit
			pdsch_l1_ctrl[cur_user].pdsch_pdu.IsLastCbPresent = data_in.data.range(56, 49);
			fapi_rx_state = CTRL_FAPI_RCV_8 ;
			fapi_done = 0;
		}
		break;
//9.
	case CTRL_FAPI_RCV_8 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// Indicates whether TB CRC is part of data payload or control message 0: TB CRC is part of data payload 1: TB CRC is part of control message
			pdsch_l1_ctrl[cur_user].pdsch_pdu.isInlineTbCrc = data_in.data.range(7, 0);
			// TB CRC: to be used in the last CB, applicable only if last CB is present
			pdsch_l1_ctrl[cur_user].pdsch_pdu.dlTbCrc = data_in.data.range(15, 8);
			// 0
			//0.0 = data_in.data.range(17,16);
			// Base Graph Value
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.bg = 0;  //yy, //tt just initialised here, value gets updated from bbu_calc file
			// Number of codeblocks
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.num_cb = 0;  //yy tt
			// nan
			//pdsch_l1_ctrl[cur_user].pdsch_c_pdu.CRC Polynomial = data_in.data.range(35,28);
			// number of message bits in the code block after CRC
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.k = 0;  //yy
			// nan
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.Z_ind = data_in.data.range(54, 49);  //yy
			fapi_rx_state = CTRL_FAPI_RCV_9 ;
			fapi_done = 0;
		}
		break;
//10.
	case CTRL_FAPI_RCV_9 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// nan
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.G = 0;
			// RM block output, first series
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.E_0 = 0;
			// Second Series
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.E_1 = 0;
			fapi_rx_state = CTRL_FAPI_RCV_10 ;
			fapi_done = 0;
		}
		break;
//11.
	case CTRL_FAPI_RCV_10 :
		if (!fapi_in.empty()) {
			data_in =  fapi_in.read();		// Index where the series shifts
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.E_shift = 0;
			// Formula based TS 36.211 Sec. 7.3.1.1, n_ID = dataScramblingIdentityPDSCH
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.cinit = 0;
			// nan
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.tb_phy_ind = 0;
			// nan
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.dirty_bit = 0; //MM
			// nan
			pdsch_l1_ctrl[cur_user].pdsch_c_pdu.S_0 = 0;

			fapi_rx_state = CTRL_FAPI_RCV_11 ;
			fapi_done = 0;

		}
		break;
//12.
	case CTRL_FAPI_RCV_11:

		data_in =  fapi_in.read();  // what is its use??

		pdsch_l1_ctrl[cur_user].pdsch_c_pdu.S_1 = 0;


		if (cur_user >= l1_ctrl_inst.n_users - 1) {
			fapi_rx_state = FAPI_RX_WAIT;
			cur_user = 0;
			fapi_done = 1;
		} else {
			cur_user = cur_user + 1;
			fapi_rx_state = CTRL_FAPI_RCV_1;
			fapi_done = 0;
		}

		break;

	default:
		fapi_rx_state = FAPI_RX_INIT;

	}

}
