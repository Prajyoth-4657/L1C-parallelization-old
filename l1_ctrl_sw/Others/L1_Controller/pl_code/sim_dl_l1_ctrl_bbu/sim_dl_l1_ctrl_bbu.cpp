#include "l1_control.h"

void fapi_sim_tx_cnfg_1(hls::stream<ecpri_axis> &ecpri_out)
{

#pragma HLS INTERFACE axis port=ecpri_out
#pragma HLS interface ap_ctrl_none port=return

#pragma HLS PIPELINE

	ecpri_axis data_out;
//	1. data_axis dout;

	data_out.data.range(7, 0) = 1;
	// nan
	data_out.data.range(8, 8) = 1;
	// nan
	data_out.data.range(9, 9) = 1;
	// nan
	data_out.data.range(18, 10) = 4; //code block size  //changes
	// nan
	data_out.data.range(27, 19) = 0;
	ecpri_out.write(data_out);

	// 2. #BEGIN#
// Bitmap indicating presence of optional PDUsBit 0: pdschPtrs  -IndicatesPTRS included (FR2) Bit    1:cbgRetxCtrl (Present when CBG based retransmit is used)All other bits reserved
	data_out.data.range(15, 0) = 0;
// The RNTI used for identifying the UE when receiving the PDUValue: 1 -> 65535
	data_out.data.range(31, 16) = 1234;  //**UserEquipment's ID number //changes
// PDU index incremented for each PDSCH PDU sent in TX control message. This is used to associate control information to data and is reset every slot. Value:0 ->65535
	data_out.data.range(47, 32) = 0;
// Bandwidthpart size[TS38.213 sec12]. Number of contiguous PRBs allocated to the BWPValue: 1->275
	data_out.data.range(63, 48) = 256;  //changes
	ecpri_out.write(data_out);

// 3. #BEGIN#
// bandwidth part start RB indexfrom reference CRB [TS38.213 sec 12]Value: 0->274
	data_out.data.range(15, 0) = 0;
// subcarrierSpacing [TS38.211 sec 4.2]Value:0->4
	data_out.data.range(23, 16) = 1;  //pdsch_pdu_t.SubcarrierSpacing;   1means 30KHz
// Cyclic prefix type[TS38.211 sec 4.2]0: Normal; 1: Extended
	data_out.data.range(31, 24) = 0;       //***n c req
// Number ofcodewords for this RNTI (UE)Value: 1 -> 2
	data_out.data.range(39, 32) = 1;       //***n c req
// Target coding rate [TS38.212sec5.4.2.1and 38.214 sec 5.1.3.1]. This is the number of information bits per 1024 coded bits expressed in 0.1 bit units
	data_out.data.range(55, 40) = 885;  //* used in puncturing 885/1024,   //yy
// QAM modulation[TS38.212sec5.4.2.1and 38.214 sec 5.1.3.1] Value: 2,4,6,8
	data_out.data.range(63, 56) = 6;
	ecpri_out.write(data_out);

// 4. #BEGIN#
// MCS index[TS38.214, sec 5.1.3.1], should match value sent in DCIValue : 0->31
	data_out.data.range(7, 0) = 19;  // **selects 1 row out of selected table
// MCS-Table-PDSCH[TS38.214, sec 5.1.3.1]0: notqam256    1: qam256       2: qam64LowSE
	data_out.data.range(15, 8) = 0x1;   //selects 1 table out of 3   //changes
// Redundancy versionindex [TS38.212, Table 5.4.2.1-2 and 38.214, Table 5.1.2.1-2], should match value sent in DCIValue : 0->3  //**sections in which o/p of sdfec can be devided
	data_out.data.range(23, 16) = 0x0; //*starting point..
// Transmit block size (in bits)[TS38.214 sec 5.1.3.2]Value: 0->65535
	data_out.data.range(55, 24) = 28168;  //changes
	ecpri_out.write(data_out);

// 5. #BEGIN#
// dataScramblingIdentityPdsch [TS38.211, sec 7.3.1.1]It equals the higher-layer parameter Data-scrambling-Identity if configured and the RNTI equals the C-RNTI, otherwise L2 needs to set it to physical cell id.Value: 0->65535
	data_out.data.range(15, 0) = 20;  //changes
// Number of layers [TS38.211, sec 7.3.1.3]Value : 1->8
	data_out.data.range(23, 16) = 1; //**not used, probably
// PDSCH transmission schemes [TS38.214, sec 5.1.1]0: Up to 8 transmission layers
	data_out.data.range(31, 24) = 1; //**not used, probably
// Reference point forPDSCH DMRS "k" -used for tone mapping [TS38.211, sec 7.4.1.1.2]Resource block bundles [TS38.211, sec 7.3.1.6]Value: 0 -> 1If 0, the 0 reference point for PDSCH DMRSis at Point A [TS38.211 sec 4.4.4.2]. Resource block bundles generated per sub-bullets 2 and 3 in [TS38.211, sec 7.3.1.6]. For sub-bullet 2, the start of bandwidth part must be set to the start of actual bandwidth part +NstartCORESETand the bandwidth of the bandwidth part must be set to the bandwidth of the initial bandwidth part.If 1, the DMRS reference point is at the lowest VRB/PRB of the allocation. Resource block bundles generated per sub-bullets 1 [TS38.211, sec7.3.1.6]
	data_out.data.range(39, 32) = 0;//pdsch_pdu_t.refPoint; //**not used, probably
// DMRS symbol positions [TS38.211, sec7.4.1.1.2andTables7.4.1.1.2-3 and 7.4.1.1.2-4]Bitmapoccupying the14 LSBswith: bit 0: first symboland for each bit0: no DMRS1: DMRS
	data_out.data.range(55, 40) = 4;//pdsch_pdu_t.dlDmrsSymbPos;
// DL DMRS config type[TS38.211, sec 7.4.1.1.2]0: type 1         1: type2
	data_out.data.range(63, 56) = 0;//pdsch_pdu_t.dmrsConfigType;
	ecpri_out.write(data_out);

// 6. #BEGIN#
// DL-DMRS-Scrambling-ID [TS38.211, sec 7.4.1.1.2 ]If provided by the higher-layer and the PDSCH is scheduled by PDCCH with CRC scrambled by C-RNTI or CS-RNTI, otherwise, L2 should set thisto physical cell id.Value: 0->65535
	data_out.data.range(15, 0) = 1234;//pdsch_pdu_t.dlDmrsScramblingId;
// DMRS sequence initialization [TS38.211, sec 7.4.1.1.2]. Should match what is sent in DCI 1_1, otherwise set to 0.Value : 0->1
	data_out.data.range(23, 16) = 0;//pdsch_pdu_t.SCID;   //??what does it do
// Number of DM-RS CDM groups without data [TS38.212 sec 7.3.1.2.2] [TS38.214 Table 4.1-1]it determines the ratio of PDSCH EPRE to DM-RS EPRE.Value: 1->3
	data_out.data.range(31, 24) = 1;//pdsch_pdu_t.numDmrsCdmGrpsNoData;
// DMRS ports.[TS38.212 7.3.1.2.2] provides description between DCI 1-1 content and DMRS ports.Bitmap occupying the 11 LSBswith: bit 0: antenna port 1000bit 11: antenna port 1011and for each bit0: DMRS port not used1: DMRS port used
	data_out.data.range(47, 32) = 1;//pdsch_pdu_t.dmrsPorts; //??what does it do
// Resource Allocation Type [TS38.214, sec 5.1.2.2]0: Type 0        1: Type 1
	data_out.data.range(55, 48) = 1; //? not using it
// For resource alloc type 0.TS 38.212 V15.0.x, 7.3.1.2.2 bitmap of RBs, 273 rounded up to multiple of 32. This bitmap is in units of VRBs. LSB of byte 0 of the bitmap represents the first RB of the bwp
	// data_out.data.range(62, 56) = 0;
	ecpri_out.write(data_out);

// 7. #BEGIN#
// For resource allocationtype 1. [TS38.214, sec 5.1.2.2.2]The starting resource block within the BWP for this PDSCH.Value: 0->274
	data_out.data.range(15, 0) = 0;//pdsch_pdu_t.rbStart;  //changes
// For resource allocation type 1. [TS38.214, sec 5.1.2.2.2]The number of resource block within for this PDSCH Value: 1->275
	data_out.data.range(31, 16) = 40;//pdsch_pdu_t.rbSize; //changes
// VRB-to-PRB-mapping[TS38.211, sec 7.3.1.6]0: non-interleaved     1: interleaved with RB size 2            2: Interleaved with RB size 4
	data_out.data.range(39, 32) = 0;
// Start symbol index of PDSCH mappingfrom the start of the slot, S. [TS38.214, Table 5.1.2.1-1]Value: 0->13
	data_out.data.range(47, 40) = 2;//pdsch_pdu_t.StartSymbolIndex;  //changes
// PDSCH duration in symbols, L [TS38.214, Table 5.1.2.1-1]Value: 1->14
	data_out.data.range(55, 48) = 12;//pdsch_pdu_t.NrOfSymbols;  //changes
// PT-RS antenna ports [TS38.214, sec 5.1.6.3] [TS38.211, table 7.4.1.2.2-1]Bitmap occupying the 6LSBswith: bit 0: antenna port 1000    bit 5: antenna port 1005     and for each bit0: PTRSport not used    1: PTRSport used
	data_out.data.range(63, 56) = 0;//pdsch_pdu_t.PTRSPortIndex;  //?not used
	ecpri_out.write(data_out);

//8. #BEGIN#    //?not used 
// PT-RS time density[TS38.214, table 5.1.6.3-1]0: 1     1: 2          2: 4
	data_out.data.range(7, 0) = 0;//pdsch_pdu_t.PTRSTimeDensity;
// PT-RS frequency density[TS38.214, table 5.1.6.3-2]0: 2     1: 4
	data_out.data.range(15, 8) = 0;//pdsch_pdu_t.PTRSFreqDensity;
// PT-RS resource elementoffset[TS38.211, table 7.4.1.2.2-1]Value: 0->3
	data_out.data.range(23, 16) = 0;//pdsch_pdu_t.PTRSReOffset;
// PT-RS-to-PDSCH EPRE ratio [TS38.214, table 4.1-2]Value :0->3
	data_out.data.range(31, 24) = 0;//pdsch_pdu_t.nEpreRatioOfPDSCHToPTRS;
// See Table 3-43 for structure, NOT IMPLEMENTED
	// data_out.data.range(32, 32) = 0;
// Ratio of PDSCH EPRE to NZP CSI-RS    EPRE [TS38.214, sec 5.2.2.3.1]Value :0->23  representing -8 to 15 dB in 1dB steps
	data_out.data.range(40, 33) = 0;//pdsch_pdu_t.powerControlOffset;
// Ratio of SSB/PBCH block EPRE to NZP CSI-RS EPRES [TS38.214, sec 5.2.2.3.1] Values:0: -3dB,1: 0dB,2: 3dB,3: 6dB
	data_out.data.range(48, 41) = 0;//pdsch_pdu_t.powerControlOffsetSS;
// Indicates whether last CB is present in the CBG retransmission 0: CBG retransmission does not include last CB 1: CBG retransmission includes last CB If CBG Re-Tx includes last CB, L1 will add the TB CRC to the last CB in the payload before it is read into the LDPC HW unit
	data_out.data.range(56, 49) = 0;//pdsch_pdu_t.IsLastCbPresent;
	ecpri_out.write(data_out);

//9. #BEGIN#
// Indicates whether TB CRC is part of data payload or control message 0: TB CRC is part of data payload 1: TB CRC is part of control message
	data_out.data.range(7, 0) = 0;//pdsch_pdu_t.isInlineTbCrc;
// TB CRC: to be used in the last CB, applicable only if last CB is present
	data_out.data.range(15, 8) = 0;//pdsch_pdu_t.dlTbCrc;
// 0
	data_out.data.range(16, 16) = 0;
    data_out.data.range(48, 17) = 0x0;
// // Base Graph Value
// 	data_out.data.range(18, 17) = 0x0;//pdsch_c_pdu_t.bg;
// // Number of codeblocks
// 	data_out.data.range(26, 19) = 5;//pdsch_c_pdu_t.num_cb;
// // nan
// 	data_out.data.range(34, 27) = 0; // CRC Polynomial
// // number of message bits in the code block after CRC
// 	data_out.data.range(48, 35) = 0x0;//pdsch_c_pdu_t.k;
// Index of the Z factor in the table
	data_out.data.range(54, 49) = 0x0;//pdsch_c_pdu_t.Z_ind;
	ecpri_out.write(data_out);

// 10. #BEGIN#
	data_out.data.range(59, 0) = 0;
// // nan
// 	data_out.data.range(31, 0) = 0x0;//pdsch_c_pdu_t.G; //*length of block after sdfec
// // RM block output, first series
// 	data_out.data.range(45, 32) = 0x0;//pdsch_c_pdu_t.E_0;
// // Second Series
// 	data_out.data.range(59, 46) = 0x0;//pdsch_c_pdu_t.E_1; //* length decide d after starting point
	ecpri_out.write(data_out);

// 11. #BEGIN#
	data_out.data.range(59, 0) = 0;
// // Index where the series shifts
// 	data_out.data.range(7, 0) = 3;//pdsch_c_pdu_t.E_shift;  //? not used
// // Formula based TS 36.211 Sec. 7.3.1.1, n_ID = dataScramblingIdentityPDSCH
// 	data_out.data.range(39, 8) = 0x0;//pdsch_c_pdu_t.cinit;
// // nan
// 	data_out.data.range(47, 40) = 0;//pdsch_c_pdu_t.tb_phy_ind;
// // nan
// 	data_out.data.range(48, 48) = 0x0;//pdsch_c_pdu_t.dirty_bit;
// // nan
// 	data_out.data.range(59, 49) = 1126;//pdsch_c_pdu_t.S_0;
	ecpri_out.write(data_out);

// 12. #BEGIN#
	data_out.data.range(10, 0) = 0;
// // nan
// 	data_out.data.range(10, 0) = 1127;//pdsch_c_pdu_t.S_1;
	ecpri_out.write(data_out);

}
