//#include <fapi_dma_config.h>
#define _GNU_SOURCE
#include "l1_dl_tti_request.h"
#include "stdlib.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <x86intrin.h>
#include "l1_config_request.h"
#include "l1_sw_common.h"
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>




ConfigRequest_t ConfigRequest;
Dl_ttiRequest_t	Dl_ttiRequest;
Dl_tti_intermediate_t Dl_tti_intermediate;
pdcch_intermediate_t pdcch_intermediate;
ORAN_sections_t ORAN_sections[Max_ORAN_sections];

extern FILE *fp;
extern FILE *fp_configsONLY;
FILE *ptr_dl_tti; //File ptr declaration

uint64_t HW_OP[MAX_CONFIG_PACKETS];
// int HW_OP_idx;
uint64_t HW_OP_idx; //changed on may 25 2022

// Sorted array of lifting sizes- this is the order (and indexing) used by the hardware module
const uint16_t LIFTING_SIZE_Z_DL[51] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		18, 20, 22, 24, 26, 28, 30, 32, 36, 40, 44, 48, 52, 56, 60, 64, 72, 80,
		88, 96, 104, 112, 120, 128, 144, 160, 176, 192, 208, 224, 240, 256, 288,
		320, 352, 384 };

const csirs_table_t csirs_table[18] = {
		{1,1,1,3,1,0},
		{1,1,1,1,1,0},
		{2,2,1,1,1,0},
		{4,2,1,2,1,0},
		{4,2,1,2,2,0},
		{8,2,1,4,1,0},
		{8,2,1,4,2,0},
		{8,2,2,2,1,0},
		{12,2,1,6,1,0},
		{12,2,2,3,1,0},
		{16,2,1,8,2,0},
		{16,2,2,4,1,0},
		{24,2,1,12,2,2},
		{24,2,2,6,1,1},
		{24,2,4,3,1,0},
		{32,2,1,16,2,2},
		{32,2,2,8,1,1},
		{32,2,4,4,1,0}
};

const int polar_reliability_table[1024] = {
	0, 1, 2, 4, 8, 16, 32, 3, 5, 64, 9, 6, 17, 10, 18, 128, 12, 33, 65, 20, 256, 34, 24, 36, 7, 129, 66, 512, 11, 40, 68, 130, 19,
	13, 48, 14, 72, 257, 21, 132, 35, 258, 26, 513, 80, 37, 25, 22, 136, 260, 264, 38, 514, 96, 67, 41, 144, 28, 69, 42, 516, 49,
	74, 272, 160, 520, 288, 528, 192, 544, 70, 44, 131, 81, 50, 73, 15, 320, 133, 52, 23, 134, 384, 76, 137, 82, 56, 27, 97, 39, 259,
	84, 138, 145, 261, 29, 43, 98, 515, 88, 140, 30, 146, 71, 262, 265, 161, 576, 45, 100, 640, 51, 148, 46, 75, 266, 273, 517, 104,
	162, 53, 193, 152, 77, 164, 768, 268, 274, 518, 54, 83, 57, 521, 112, 135, 78, 289, 194, 85, 276, 522, 58, 168, 139, 99, 86, 60,
	280, 89, 290, 529, 524, 196, 141, 101, 147, 176, 142, 530, 321, 31, 200, 90, 545, 292, 322, 532, 263, 149, 102, 105, 304, 296, 163,
	92, 47, 267, 385, 546, 324, 208, 386, 150, 153, 165, 106, 55, 328, 536, 577, 548, 113, 154, 79, 269, 108, 578, 224, 166, 519, 552,
	195, 270, 641, 523, 275, 580, 291, 59, 169, 560, 114, 277, 156, 87, 197, 116, 170, 61, 531, 525, 642, 281, 278, 526, 177, 293, 388,
	91, 584, 769, 198, 172, 120, 201, 336, 62, 282, 143, 103, 178, 294, 93, 644, 202, 592, 323, 392, 297, 770, 107, 180, 151, 209, 284,
	648, 94, 204, 298, 400, 608, 352, 325, 533, 155, 210, 305, 547, 300, 109, 184, 534, 537, 115, 167, 225, 326, 306, 772, 157, 656, 329,
	110, 117, 212, 171, 776, 330, 226, 549, 538, 387, 308, 216, 416, 271, 279, 158, 337, 550, 672, 118, 332, 579, 540, 389, 173, 121, 553,
	199, 784, 179, 228, 338, 312, 704, 390, 174, 554, 581, 393, 283, 122, 448, 353, 561, 203, 63, 340, 394, 527, 582, 556, 181, 295, 285,
	232, 124, 205, 182, 643, 562, 286, 585, 299, 354, 211, 401, 185, 396, 344, 586, 645, 593, 535, 240, 206, 95, 327, 564, 800, 402, 356,
	307, 301, 417, 213, 568, 832, 588, 186, 646, 404, 227, 896, 594, 418, 302, 649, 771, 360, 539, 111, 331, 214, 309, 188, 449, 217, 408,
	609, 596, 551, 650, 229, 159, 420, 310, 541, 773, 610, 657, 333, 119, 600, 339, 218, 368, 652, 230, 391, 313, 450, 542, 334, 233, 555,
	774, 175, 123, 658, 612, 341, 777, 220, 314, 424, 395, 673, 583, 355, 287, 183, 234, 125, 557, 660, 616, 342, 316, 241, 778, 563, 345,
	452, 397, 403, 207, 674, 558, 785, 432, 357, 187, 236, 664, 624, 587, 780, 705, 126, 242, 565, 398, 346, 456, 358, 405, 303, 569, 244,
	595, 189, 566, 676, 361, 706, 589, 215, 786, 647, 348, 419, 406, 464, 680, 801, 362, 590, 409, 570, 788, 597, 572, 219, 311, 708, 598,
	601, 651, 421, 792, 802, 611, 602, 410, 231, 688, 653, 248, 369, 190, 364, 654, 659, 335, 480, 315, 221, 370, 613, 422, 425, 451, 614,
	543, 235, 412, 343, 372, 775, 317, 222, 426, 453, 237, 559, 833, 804, 712, 834, 661, 808, 779, 617, 604, 433, 720, 816, 836, 347, 897,
	243, 662, 454, 318, 675, 618, 898, 781, 376, 428, 665, 736, 567, 840, 625, 238, 359, 457, 399, 787, 591, 678, 434, 677, 349, 245, 458,
	666, 620, 363, 127, 191, 782, 407, 436, 626, 571, 465, 681, 246, 707, 350, 599, 668, 790, 460, 249, 682, 573, 411, 803, 789, 709, 365,
	440, 628, 689, 374, 423, 466, 793, 250, 371, 481, 574, 413, 603, 366, 468, 655, 900, 805, 615, 684, 710, 429, 794, 252, 373, 605, 848,
	690, 713, 632, 482, 806, 427, 904, 414, 223, 663, 692, 835, 619, 472, 455, 796, 809, 714, 721, 837, 716, 864, 810, 606, 912, 722, 696,
	377, 435, 817, 319, 621, 812, 484, 430, 838, 667, 488, 239, 378, 459, 622, 627, 437, 380, 818, 461, 496, 669, 679, 724, 841, 629, 351,
	467, 438, 737, 251, 462, 442, 441, 469, 247, 683, 842, 738, 899, 670, 783, 849, 820, 728, 928, 791, 367, 901, 630, 685, 844, 633, 711,
	253, 691, 824, 902, 686, 740, 850, 375, 444, 470, 483, 415, 485, 905, 795, 473, 634, 744, 852, 960, 865, 693, 797, 906, 715, 807, 474,
	636, 694, 254, 717, 575, 913, 798, 811, 379, 697, 431, 607, 489, 866, 723, 486, 908, 718, 813, 476, 856, 839, 725, 698, 914, 752, 868,
	819, 814, 439, 929, 490, 623, 671, 739, 916, 463, 843, 381, 497, 930, 821, 726, 961, 872, 492, 631, 729, 700, 443, 741, 845, 920, 382,
	822, 851, 730, 498, 880, 742, 445, 471, 635, 932, 687, 903, 825, 500, 846, 745, 826, 732, 446, 962, 936, 475, 853, 867, 637, 907, 487,
	695, 746, 828, 753, 854, 857, 504, 799, 255, 964, 909, 719, 477, 915, 638, 748, 944, 869, 491, 699, 754, 858, 478, 968, 383, 910, 815,
	976, 870, 917, 727, 493, 873, 701, 931, 756, 860, 499, 731, 823, 922, 874, 918, 502, 933, 743, 760, 881, 494, 702, 921, 501, 876, 847,
	992, 447, 733, 827, 934, 882, 937, 963, 747, 505, 855, 924, 734, 829, 965, 938, 884, 506, 749, 945, 966, 755, 859, 940, 830, 911, 871,
	639, 888, 479, 946, 750, 969, 508, 861, 757, 970, 919, 875, 862, 758, 948, 977, 923, 972, 761, 877, 952, 495, 703, 935, 978, 883, 762,
	503, 925, 878, 735, 993, 885, 939, 994, 980, 926, 764, 941, 967, 886, 831, 947, 507, 889, 984, 751, 942, 996, 971, 890, 509, 949, 973,
	1000, 892, 950, 863, 759, 1008, 510, 979, 953, 763, 974, 954, 879, 981, 982, 927, 995, 765, 956, 887, 985, 997, 986, 943, 891, 998, 766,
	511, 988, 1001, 951, 1002, 893, 975, 894, 1009, 955, 1004, 1010, 957, 983, 958, 987, 1012, 999, 1016, 767, 989, 1003, 990, 1005, 959, 1011,
	1013, 895, 1006, 1014, 1017, 1018, 991, 1020, 1007, 1015, 1019, 1021, 1022, 1023
};

// index is 0:5, muxPattern = 1,
const int coreset0_table_scs_15_30[4][16] = {{15,15,15,15,15,15,15,15,15,15,255,255,255,255,255,255},// Bit map with nrb continuous ones
		                             {24,24,24,24,24,24,24,24,24,24,48,48,48,48,48,48}, // Number of resource blocks (nrb)
					     {2,2,2,2,2,3,3,3,3,3,1,1,1,2,2,2}, // Number of symbols (nsym)
                                             {0, 1, 2, 3, 4, 0, 1, 2, 3, 4,12,14,16,12,14,16}}; // Offset

int sorted_CB_idx[MAX_CB];
int CB_TB_idx[MAX_CB];
int CB_idx_in_TB[MAX_CB];
int sorted_CSIRS_idx[MaxUE];
int sorted_DMRS_idx[MaxUE];
int sorted_PDCCH_idx[MaxUE];
int sorted_PDSCH_idx[MaxUE];
int sorted_SSB_idx[MaxUE];
int coreset0_dmrs_ref = 0;

// Values needed to sort the CBs using mergesort
uint8_t cb_start_time[MAX_CB];
uint16_t cb_start_freq[MAX_CB];






void get_dl_tti_request_data_from_hex_string(char* fapiHexDlttiReq_i)
{

	uint32_t Pdu_Count=0;
	uint32_t Pdcch_Count = 0;
	uint32_t Pdsch_Count = 0;
	uint32_t Csirs_Count = 0;
	uint32_t Ssb_Count = 0;
	uint32_t nUe_Count = 0;
	uint32_t nUe_Pdu_Count = 0,Group_Count =0;
	uint32_t var1=0,var2=0,var3=0,var4=0;
	uint32_t pdu_size = 0;
	uint32_t *dltti_array_ptr_32b;
	uint8_t *dltti_array_ptr;

	/* For sw code, read fapi message in hex format and convert it to binary */
	uint8_t fapiBytesDlttiReq[MAX_FAPI_BYTES];
	int numFapiBytes = convertHex2DecBytes(fapiHexDlttiReq_i, fapiBytesDlttiReq);
	// validate fapi message
	if(numFapiBytes > MAX_FAPI_BYTES)
	{
		//printf("Error in get_dl_tti_request_data(): Number of Fapi message bytes exceeds allocated limit\n");
		return;
	}

	char fapiHeader[9];
	memcpy(fapiHeader,fapiHexDlttiReq_i,8);
	fapiHeader[8]='\0';
	if(0!=strcmp("01008000",fapiHeader))//0100 will be the message header, 8000 is message id
	{
		//printf("Error in get_dl_tti_request_data(): Invalid Fapi message bytes\n");
		return;
	}

	float dci_payload_byte_len;
	dltti_array_ptr_32b = (uint32_t *)(fapiBytesDlttiReq + 4);
	Dl_ttiRequest.MsgLength = (dltti_array_ptr_32b[0]);
	Dl_ttiRequest.SFN=(uint16_t) *((uint16_t *)(dltti_array_ptr_32b+1));
	pdu_size = (Dl_ttiRequest.MsgLength);
	////////////////xil_//printf("pdu_size = %x ", pdu_size);
	dltti_array_ptr = fapiBytesDlttiReq + 10;
	Dl_ttiRequest.Slot = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
	Dl_ttiRequest.nPDUs = (uint8_t)(*dltti_array_ptr++);
	Dl_ttiRequest.nGroup = (uint8_t)(*dltti_array_ptr++);
	//     xil_//printf("s:%x, p:%x\r\n",Dl_ttiRequest.Slot,Dl_ttiRequest.nPDUs);
	////xil_//printf("Dl_ttiRequest.MsgLength = %d\r\n", Dl_ttiRequest.MsgLength);
	//     //xil_//printf("Dl_ttiRequest.SFN = %d \r\n", Dl_ttiRequest.SFN);
	//xil_//printf("%d \r\n", Dl_ttiRequest.Slot);
	//      //xil_//printf("Dl_ttiRequest.nPDUs = %d \r\n", Dl_ttiRequest.nPDUs);
	//      //xil_//printf("Dl_ttiRequest.nGroup = %d \r\n ", Dl_ttiRequest.nGroup);

	while(Pdu_Count < (int)Dl_ttiRequest.nPDUs)//size of total dltti pdu count
	{
		Dl_ttiRequest.DLPDU.PDUType = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		Dl_ttiRequest.DLPDU.PDUSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		//        xil_//printf("PT:%d,s:%x \r\n", Dl_ttiRequest.DLPDU.PDUType,Dl_ttiRequest.DLPDU.PDUSize);

		switch((int)Dl_ttiRequest.DLPDU.PDUType)
		{
		case PDCCH_PDU_TAG:
		{
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].SubcarrierSpacing = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CyclicPrefix = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].StartSymbolIndex = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].DurationSymbols = (uint8_t)(*dltti_array_ptr++);
			//           xil_//printf("%x, %x, %x, %x, %x, %x\r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPSize,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart,
			//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].SubcarrierSpacing,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CyclicPrefix,
			//				   Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].StartSymbolIndex,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].DurationSymbols);
			//          xil_//printf("%d",MAX_FREQ_DOM_RESOURCE);

			for(var1 = 0;var1<MAX_FREQ_DOM_RESOURCE;var1++)
			{
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1] = (uint8_t)(*dltti_array_ptr++);
				////////////////xil_//printf("fr[%d]= %d ",var1,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1]);
			}
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CceRegMappingType = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].RegBundleSize = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].InterleaverSize = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CoreSetType = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].ShiftIndex = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].precoderGranularity = (uint8_t)(*dltti_array_ptr++);
			//  ////xil_//printf("precoder granu= %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].precoderGranularity);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8)); //23Bytes

			//           if(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci!=1){
			//xil_//printf("E_cch,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci \r\n");
			//           }
			for(var1 = 0;var1<(int)Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci;var1++)
			{
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].RNTI = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));// number dldcis yet to know the values
				////xil_//printf("%d",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[0].RNTI);

				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingId = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingRNTI = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].CceIndex = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].AggregationLevel = (uint8_t)(*dltti_array_ptr++);//31B
				//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.prgSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces = (uint8_t)(*dltti_array_ptr++);

				//          xil_//printf("PRGs: %d \r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs);
				//  ////xil_//printf("digbf-interface = %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);
				//           if(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs!=1){
				//xil_//printf("E_cch@TxPrecoding_Beamforming.numPRGs \r\n");
				//           }
				//           for(var2=0;var2<(uint16_t)(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs);var2++)
				//           {
				//            Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				//   ////xil_//printf("[0]pmidx=%d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
				//           if(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces!=4){
				//xil_//printf("E_cch@TxPrecoding_Beamforming.digBFInterfaces \r\n");
				//           }
				//            for(var3=0;var3<(uint8_t)(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);var3++)
				//            	{
				//            		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				//   ////xil_//printf(" %d , var3 = %d\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] ,var3);
				//            	}
				//           }

				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0 = (uint8_t)(*dltti_array_ptr++);
				// ////xil_//printf("beta_pdcch=%d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].powerControlOffsetSS = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				//dci_payload_byte_len = ((float)Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits/(float)8);
				//dci_payload_byte_len = ceil(dci_payload_byte_len);
				////xil_//printf("Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits = %d \r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits);
				////printf("dci_payload_byte_len = %f \r\n",dci_payload_byte_len);
				//35B
				//           xil_//printf("Po:%x\r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].powerControlOffsetSS);
				for(var2 = 0;var2<(int)((Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits+7)/8) ;var2++)
				{
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] = (uint8_t)(*dltti_array_ptr++);
					//       xil_//printf("payload[%d] = %d \n ",var2,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] );

				}
			}
			Pdcch_Count = Pdcch_Count+1;
			//          //xil_//printf("------------------------------------------Pdcch_Count--------------------------------------- %d\r\n",Pdcch_Count);
			break;
		}


		case PDSCH_PDU_TAG:
		{
			//            ////xil_//printf("PDsCH_PDU_TAG\r\n");
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//////xil_//printf("i=%d , Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap = %d \r\n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].RNTI = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].RNTI = %d \r\n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].RNTI);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduIndex =  (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].BWPSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].BWPStart = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].SubcarrierSpacing = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CyclicPrefix = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords = (uint8_t)(*dltti_array_ptr++);
			//////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords = %d \r\n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords);
			for(var1=0;var1<(int)(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords);var1++)
			{
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].targetCodeRate = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].qamModOrder =  (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].mcsIndex = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].mcsTable = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].rvIndex = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].TBSize = (uint32_t)(((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8)) | ((*dltti_array_ptr++)<<16) | ((*dltti_array_ptr++)<<24));
				// ////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].TBSize = %d \r\n",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].TBSize);
			}

			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dataScramblingId = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			// ////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dataScramblingId = %d \r\n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dataScramblingId);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].nrOfLayers = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].transmissionScheme = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].refPoint = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlDmrsSymbPos = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dmrsConfigType = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlDmrsScramblingId = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].SCID = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].numDmrsCdmGrpsNoData = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dmrsPorts = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc = (uint8_t)(*dltti_array_ptr++);
			// ////xil_//printf(" Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc = %d \r\n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc);
			for(var2 = 0;var2<(int)(MAX_RB_BITMAP);var2++)
			{
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap[var2] = (uint8_t)(*dltti_array_ptr++);
				//////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap[var2] = %d \r\n ",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap[var2] );
			}
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbStart = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			// ////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize = %d \r\n ",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].VRBtoPRBMapping = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].StartSymbolIndex = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfSymbols = (uint8_t)(*dltti_array_ptr++);
			if(((int)Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap & 0b0001))
			{
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSPortIndex = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSTimeDensity = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSFreqDensity = (uint8_t)(*dltti_array_ptr++);
				//////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSFreqDensity = %d \r\n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSFreqDensity);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSReOffset = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].nEpreRatioOfPDSCHToPTRS = (uint8_t)(*dltti_array_ptr++);
			}
			// 	       Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//          // ////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs = %d \r\n ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs);
			// 	       Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.prgSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			// 	       Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces = (uint8_t)(*dltti_array_ptr++);
			// ////xil_//printf(" Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces = %d \r\n ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces);
			// 	       for(var2=0;var2<(int)(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs);var2++)
			//           {
			//            Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//            for(var3=0;var3<(int)(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces);var3++)
			//           {
			//        	   Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3]  = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           }
			//          // ////xil_//printf(" Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[%d].PMidx = %d \r\n",var2,Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
			//           }

			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffset = (uint8_t)(*dltti_array_ptr++);
			//           xil_//printf("p: %d \r\n",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffset);
			Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffsetSS = (uint8_t)(*dltti_array_ptr++);
			if(((int)Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap & 0b0010))
			{
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].IsLastCbPresent = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].isInlineTbCrc = (uint8_t)(*dltti_array_ptr++);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlTbCrc = (uint32_t)(((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8)) | ((*dltti_array_ptr++)<<16) | ((*dltti_array_ptr++)<<24));
				//////xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlTbCrc = %d \r\n ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlTbCrc);
			}
			// Currently we support only the contiguous allocation, so we convert the bitmap to contiguous allocation if Resource Allocation = 0 and process the Allocation as type 1
			if( Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc == 0 ){
				int rbStart, rbSize ;
				get_contiguous_RB_alloc_from_bitmap(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap,&rbStart,&rbSize);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc = 1;
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbStart = rbStart;
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize = rbSize;
			}

			Pdsch_Count = Pdsch_Count+1;
			//xil_//printf("Pd= %d",Pdsch_Count);
			break;
		}


		case CSIRS_PDU_TAG:
			//        	   //////////xil_//printf("csirs_PDU_TAG");
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			////////////////xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPSize = %d", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPSize);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPStart = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			////////////////xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPStart = %x ", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPStart);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].SubcarrierSpacing = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].CyclicPrefix = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].StartRB = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].NrOfRBs = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].CSIType = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].Row = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].FreqDomain = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].SymbL0 = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].SymbL1 = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].CDMType = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].FreqDensity = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].ScrambId = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffset = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffsetSS = (uint8_t)(*dltti_array_ptr++);
			////////////////xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffset = %x ", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffset);
			//          Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.numPRGs = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//          Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.prgSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//          Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.digBFInterfaces = (uint8_t)(*dltti_array_ptr++);
			//           ////////////////xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.digBFInterfaces = %x ", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.digBFInterfaces);
			//           for(var2=0;var2<(int)(Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.numPRGs);var2++)
			//           {
			//         Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           for(var3=0;var3<(int)(Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.prgSize);var3++)
			//           {
			//          Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3]  = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           }
			//       ////////////////xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[%d].PMidx=%d",var2,Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
			//           }
			Csirs_Count = Csirs_Count+1;
			//          //xil_//printf("------------------------------------------csirs_Count--------------------------------------- %d",Csirs_Count);
			break;

		case SSB_PDU_TAG:
			//            	 //////////xil_//printf("ssb_PDU_TAG");
			Dl_ttiRequest.SSB_PDU[Ssb_Count].physCellId = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           xil_//printf("celid =%d \n",Dl_ttiRequest.SSB_PDU[Ssb_Count].physCellId);
			Dl_ttiRequest.SSB_PDU[Ssb_Count].betaPss = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex = (uint8_t)(*dltti_array_ptr++);
			////////////////xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex = %x ", Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex);
			Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbSubcarrierOffset = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.SSB_PDU[Ssb_Count].SsbOffsetPointA = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayloadFlag = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload = (uint32_t)(((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8)) | ((*dltti_array_ptr++)<<16) | ((*dltti_array_ptr++)<<24));
			//           xil_//printf("%d",Dl_ttiRequest.SSB_PDU[Ssb_Count].physCellId);//%d,%d,%d,%d,%d,%d \r\n",Dl_ttiRequest.SSB_PDU[Ssb_Count].physCellId,Dl_ttiRequest.SSB_PDU[Ssb_Count].betaPss,Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex,
			//           Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbSubcarrierOffset,Dl_ttiRequest.SSB_PDU[Ssb_Count].SsbOffsetPointA,
			//		   Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayloadFlag,Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload);
			////////////////xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload  = %x ", Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload );
			//           Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.prgSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces = (uint8_t)(*dltti_array_ptr++);
			//           ////////////////xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs = %x", Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs);
			//           ////////////////xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces = %x ", Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces);
			//           if(Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs!=1){
			//        	   //xil_//printf("E_ssb@TxPrecoding_Beamforming.numPRGs \r\n");
			//           }
			//           for(var2=0;var2<(int)(Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs);var2++)
			//           {
			//           Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx= (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           for(var3=0;var3<(int)(Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces);var3++)
			//           {
			//           Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           }
			//            ////////////////xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].numOfDlDcis[0].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = %x ",var2,var3, Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3]);
			//           }
			ConfigRequest.SSBTable.MIB.Value = Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload;
			Ssb_Count = Ssb_Count +1;
			//xil_//printf("Ss %d \r\n",Ssb_Count);
			break;

		default:
			break;

		}

		Pdu_Count=Pdu_Count+1;
		////xil_//printf("Pdu Count = %d\r\n",Pdu_Count);
	}
	//condition checks
	if(Dl_ttiRequest.nGroup!=8)
	{
		//xil_//printf("Error at DL_tti:Dl_ttiRequest.nGroup \r\n");
	}
	while(Group_Count<(int)(Dl_ttiRequest.nGroup))
	{
		Dl_ttiRequest.UeList[nUe_Count].nUe = (uint8_t)(*dltti_array_ptr++);
		if(Dl_ttiRequest.UeList[nUe_Count].nUe>4){  //nUe range from 1 to 4
			//xil_//printf("E@Dl_ttiRequest.UeList[nUe_Count].nUe \r\n");
		}
		for(var1=0;var1<(int)(Dl_ttiRequest.UeList[nUe_Count].nUe);var1++)
		{
			Dl_ttiRequest.UeList[nUe_Count].PduIdx[var1] = (uint8_t)(*dltti_array_ptr++);
			////////////////xil_//printf("Dl_ttiRequest.UeList[nUe_Count].PduIdx[%d]  = %x",var1,Dl_ttiRequest.UeList[nUe_Count].PduIdx[var1]);
		}
		nUe_Count = nUe_Count + 1;
		////////////////xil_//printf("------------------------------------------nUe_Count--------------------------------------- %d\n",nUe_Count);
		Group_Count = Group_Count+1;
	}
	//l1_downlink_chain
	Dl_tti_intermediate.nPDSCH = Pdsch_Count;
	Dl_tti_intermediate.nPDCCH = Pdcch_Count;
	Dl_tti_intermediate.nCSIRS = Csirs_Count;
	Dl_tti_intermediate.nSSB = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// SSB symbol index calculation in a half-frame: Case C (TS 38.213): {2,8}+14*n, n=0,1,2,3.
	//  So, possible SSB symbol positions are: 2,8,16,22,30,36,44,50
	//          -- Minimum = 2  in slot0  ( repeat for slot10 if SsbPeriod=0 for 5ms period)
	//          -- Maximum = 50 in slot3  ( repeat for slot13 if SsbPeriod=0 for 5ms period)
	// So SSB will be transmitted only during the first 4 slots of every "half-frame".
	// SSB Mask[0] bitmap determines which of these symbols are actually present
	//   -- bitmap order is MSB first. Only consider first 8 MSB bits, i.e.
	//             -- bit31 = 1 => SSB present at symbol 2  (slot0)
	//             -- bit30 = 1 => SSB present at symbol 8  (slot0)
	//             -- bit29 = 1 => SSB present at symbol 16 (slot1, symbol2)
	//             -- bit28 = 1 => SSB present at symbol 22 (slot1, symbol8)
	//             -- bit27 = 1 => SSB present at symbol 30 (slot2, symbol2)
	//             -- bit26 = 1 => SSB present at symbol 36 (slot2, symbol8)
	//             -- bit25 = 1 => SSB present at symbol 44 (slot3, symbol2)
	//             -- bit24 = 1 => SSB present at symbol 50 (slot3, symbol8)
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (Dl_ttiRequest.Slot%10 <=3 ){ // Only needs to check first 4 slots of every "half-frame" for possible SSB transmission
		if (ConfigRequest.SSBTable.SsbPeriod.Value == 0){ // 5ms period
			if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*(Dl_ttiRequest.Slot%10)-1)))
				Dl_tti_intermediate.nSSB += 1; // symbol2 present
			if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*(Dl_ttiRequest.Slot%10)-2)))
				Dl_tti_intermediate.nSSB += 2; // symbol8 present
		}
		else{
			if (Dl_ttiRequest.SFN%ConfigRequest.SSBTable.SsbPeriod.Value == 0){
				if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*Dl_ttiRequest.Slot-1)))
					Dl_tti_intermediate.nSSB += 1; // symbol2 present
				if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*Dl_ttiRequest.Slot-2)))
					Dl_tti_intermediate.nSSB += 2; // symbol8 present
			}
		}
	}

	// validation: check if SSB is received as per configuration
	if ((Ssb_Count>0) && (Dl_tti_intermediate.nSSB==0)){
		///////////////////////////////////////////
		// TODO: When ConfigRequest.SSBTable.SsbPeriod.Value > 1 ( i.e. period >= 20 ms), there could be an error scenario such that
		// SSB pdu is received from Sooktha in the current SFN, but  Dl_ttiRequest.SFN%ConfigRequest.SSBTable.SsbPeriod.Value != 0
		// This case happens when SSB transmitted with a frame number offset, say "ssbFrameOffset" such that
		// (Dl_ttiRequest.SFN+ssbFrameOffset)%ConfigRequest.SSBTable.SsbPeriod.Value = 0. Not sure if this will happen. But if it happens
		//  we may have to find value of "ssbFrameOffset" using the SFN for which SSB pdu is received from Sooktha and then replace the condition
		//     if  ((Dl_ttiRequest.SFN)%ConfigRequest.SSBTable.SsbPeriod.Value == 0)
		//   to
		//     if  ((Dl_ttiRequest.SFN+ssbFrameOffset)%ConfigRequest.SSBTable.SsbPeriod.Value == 0)
		// For that we can use ssbFrameOffset as a static variable and find its value only when Ssb_Count > 0.
		////////////////////////////////////////////
		//printf("\nError: SSB pdu is received in wrong slot=%d or wrong SFN=%d where SsbPeriod=%d, SsbMask=0x%x\n",Dl_ttiRequest.Slot,Dl_ttiRequest.SFN,ConfigRequest.SSBTable.SsbPeriod.Value,ConfigRequest.SSBTable.SsbMask[0].Value);
	}
}


// initializeFlag - should be 1 to initialize the number of pdus to zero, otherwise it will append to existing dl_tti
void append_ul_dci_request_data_from_hex_string(char* fapiHexUciReq_i, int initializeFlag){
	uint32_t Pdcch_Count = 0;
	if(0 == initializeFlag){
		Pdcch_Count = Dl_tti_intermediate.nPDCCH;
	}
	else{
		Dl_tti_intermediate.nPDSCH = 0;
		Dl_tti_intermediate.nPDCCH = 0;
		Dl_tti_intermediate.nCSIRS = 0;
		Dl_tti_intermediate.nSSB = 0;
		Dl_tti_intermediate.nPDCCH = 0;
		Dl_ttiRequest.nPDUs = 0;
	}

	uint32_t Pdu_Count=0;
	uint32_t var1=0,var2=0,var3=0,var4=0;
	uint32_t pdu_size = 0;
	uint32_t *dltti_array_ptr_32b;
	uint8_t *dltti_array_ptr;
	uint32_t MsgLength;
	uint8_t nPDUs;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* For sw code, read fapi message in hex format and convert it to binary */
	uint8_t fapiBytesUciReq[MAX_FAPI_BYTES];
	int numFapiBytes = convertHex2DecBytes(fapiHexUciReq_i, fapiBytesUciReq);
	// validate fapi message
	if(numFapiBytes > MAX_FAPI_BYTES)
	{
		//printf("Error in append_ul_dci_request_data_from_hex_string(): Number of Fapi message bytes exceeds allocated limit\n");
		return;
	}

	char fapiHeader[9];
	memcpy(fapiHeader,fapiHexUciReq_i,8);
	fapiHeader[8]='\0';
	if(0!=strcmp("01008300",fapiHeader))//0100 will be the message header, 8300 is message id
	{
		//printf("Error in append_ul_dci_request_data_from_hex_string(): Invalid Fapi message bytes\n");
		return;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	dltti_array_ptr_32b = (uint32_t *)(fapiBytesUciReq + 4);
	MsgLength = (dltti_array_ptr_32b[0]);
	Dl_ttiRequest.SFN = (uint16_t) *((uint16_t *)(dltti_array_ptr_32b+1));
	////////////////xil_//printf("pdu_size = %x ", pdu_size);
	dltti_array_ptr = fapiBytesUciReq + 10;
	Dl_ttiRequest.Slot = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
	nPDUs = (uint8_t)(*dltti_array_ptr++);

	while(Pdu_Count < (int)nPDUs)//size of total dltti pdu count
	{
		Dl_ttiRequest.DLPDU.PDUType = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		Dl_ttiRequest.DLPDU.PDUSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		//        xil_//printf("PT:%d,s:%x \r\n", Dl_ttiRequest.DLPDU.PDUType,Dl_ttiRequest.DLPDU.PDUSize);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].SubcarrierSpacing = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CyclicPrefix = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].StartSymbolIndex = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].DurationSymbols = (uint8_t)(*dltti_array_ptr++);
		//           xil_//printf("%x, %x, %x, %x, %x, %x\r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPSize,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart,
		//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].SubcarrierSpacing,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CyclicPrefix,
		//				   Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].StartSymbolIndex,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].DurationSymbols);
		//          xil_//printf("%d",MAX_FREQ_DOM_RESOURCE);

		for(var1 = 0;var1<MAX_FREQ_DOM_RESOURCE;var1++)
		{
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1] = (uint8_t)(*dltti_array_ptr++);
			////////////////xil_//printf("fr[%d]= %d ",var1,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1]);
		}
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CceRegMappingType = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].RegBundleSize = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].InterleaverSize = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CoreSetType = (uint8_t)(*dltti_array_ptr++);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].ShiftIndex = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].precoderGranularity = (uint8_t)(*dltti_array_ptr++);
		//  ////xil_//printf("precoder granu= %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].precoderGranularity);
		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8)); //23Bytes

		//           if(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci!=1){
		//xil_//printf("E_cch,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci \r\n");
		//           }
		for(var1 = 0;var1<(int)Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci;var1++)
		{
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].RNTI = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));// number dldcis yet to know the values
			////xil_//printf("%d",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[0].RNTI);

			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingId = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingRNTI = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].CceIndex = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].AggregationLevel = (uint8_t)(*dltti_array_ptr++);//31B
			//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.prgSize = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//           Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces = (uint8_t)(*dltti_array_ptr++);

			//          xil_//printf("PRGs: %d \r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs);
			//  ////xil_//printf("digbf-interface = %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);
			//           if(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs!=1){
			//xil_//printf("E_cch@TxPrecoding_Beamforming.numPRGs \r\n");
			//           }
			//           for(var2=0;var2<(uint16_t)(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs);var2++)
			//           {
			//            Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//   ////xil_//printf("[0]pmidx=%d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
			//           if(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces!=4){
			//xil_//printf("E_cch@TxPrecoding_Beamforming.digBFInterfaces \r\n");
			//           }
			//            for(var3=0;var3<(uint8_t)(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);var3++)
			//            	{
			//            		Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//   ////xil_//printf(" %d , var3 = %d\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] ,var3);
			//            	}
			//           }

			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0 = (uint8_t)(*dltti_array_ptr++);
			// ////xil_//printf("beta_pdcch=%d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].powerControlOffsetSS = (uint8_t)(*dltti_array_ptr++);
			Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits = (uint16_t)((*dltti_array_ptr++) | ((*dltti_array_ptr++)<<8));
			//dci_payload_byte_len = ((float)Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits/(float)8);
			//dci_payload_byte_len = ceil(dci_payload_byte_len);
			////xil_//printf("Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits = %d \r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits);
			////printf("dci_payload_byte_len = %f \r\n",dci_payload_byte_len);
			//35B
			//           xil_//printf("Po:%x\r\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].powerControlOffsetSS);
			for(var2 = 0;var2<(int)((Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits+7)/8) ;var2++)
			{
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] = (uint8_t)(*dltti_array_ptr++);
				//       xil_//printf("payload[%d] = %d \n ",var2,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] );

			}
		}
		Pdcch_Count = Pdcch_Count+1;
		Pdu_Count=Pdu_Count+1;
	}
	Dl_tti_intermediate.nPDCCH = Pdcch_Count;
	Dl_ttiRequest.nPDUs = Dl_ttiRequest.nPDUs + Pdu_Count;
}

/*
Function: reverseBits

Function to reverse bits in a given Byte

COPIED FROM
https://www.geeksforgeeks.org/write-an-efficient-c-program-to-reverse-bits-of-a-number/

Input:
num (uint8_t): input byte

Output:
      Output byte with bits reversed

*/
uint8_t reverseBits(uint8_t num)
{
    uint8_t reverse_num = num;
    int count = 7;

    num >>= 1;
    while(num)
    {
       reverse_num <<= 1;
       reverse_num |= num & 1;
       num >>= 1;
       count--;
    }
    reverse_num <<= count;
    return reverse_num;
}



/*
Function: get_dl_tti_value_bits

	Function to read bitwise from a binary file containing FAPI DL_TTI configurations

	Inputs:
		Number of bits to be read from the binary configurations file
		 
	Output:
		Returns an integer of input bits length read from the configuration file

*/

int get_dl_tti_value_bits(int bits){

	uint8_t a1;
	uint16_t b1;
	uint32_t c1;
	int read_success;
	
	switch(bits){
		case BITS_EIGHT:
			read_success = fread(&a1,sizeof(uint8_t),1,ptr_dl_tti);
			return a1;
			break;
				
		case BITS_SIXTEEN:
			read_success = fread(&b1,sizeof(uint16_t),1,ptr_dl_tti);
			return b1;
			break;
				
		case BITS_THIRTYTWO:
			read_success = fread(&c1,sizeof(uint32_t),1,ptr_dl_tti);
			return c1;
			break;

		default:
			break;
	}
}


/*
Function: get_dl_tti_request_data_from_file

	Function to populate the dl_tti_request structure from a binary file containing FAPI configurations

	Inputs:
		bin_file : binary file generated by GUI
		 
	Output:
		None.

*/

void get_dl_tti_request_data_from_file(char* bin_file)
{
	//int pdu[1]={0};/// not optimized
	int Pdu_Count = 0;
	int Pdcch_Count = 0;
	int Pdsch_Count = 0;
	int Csirs_Count = 0;
	int Ssb_Count = 0;
	int nUe_Count = 0;
	int nUe_Pdu_Count = 0,Group_Count = 0;
	int var1=0, var2=0, var3=0, var4=0;

	//Arrays to store DL_TTI information
	// int dl_tti_req_data[400] = {0};
	// uint8_t	dl_tti_8b_pdu[1600] = {0};

	//uint8_t pdu8_t[1500] = {0};/// not optimized
	// uint32_t pdu_size = 0;
	// uint32_t *dltti_array_ptr_32b;
	// u8 *dltti_array_ptr;
	// dltti_array_ptr_32b = (u32 *)((0x01300004)+ get_inc_address());/// optimized(to avoid memcpy)
	// //fapi_rcv_invalidate_dma_cache_32b(pdu,4,1);// not optimized
	// Dl_ttiRequest.MsgLength = (dltti_array_ptr_32b[0]<<16 )| mesg_length_16b();
	// Dl_ttiRequest.SFN=(uint16_t) *((uint16_t *)&dltti_array_ptr_32b[0]+1);
	// pdu_size = (Dl_ttiRequest.MsgLength);
	// //xil_//printf("pdu_size = %x ", pdu_size);
	// dltti_array_ptr = (u8 *)((0x01300008) + get_inc_address());/// optimized
	// fapi_rcv_invalidate_dma_cache_8b(pdu8_t,pdu_size,2);/// not optimized
	// for(int i=0;i<100;i++)
	// {
	// 	xil_//printf("dltti_array_ptr[%p] = %x \r\n",&dltti_array_ptr[i],dltti_array_ptr[i]);
	// }
	//dltti_array_ptr = &pdu8_t[0];/// not optimized

	// #include "../../IO_files/dl_tti_request_array.txt"
	// memcpy(dl_tti_8b_pdu,dl_tti_req_data,sizeof(dl_tti_8b_pdu));
	// dl_tti_array_ptr = &dl_tti_8b_pdu[8]; //start pointer to the 8th element

	//Opening the configuration binary file
	ptr_dl_tti = fopen(bin_file,"rb");
	if (!ptr_dl_tti)
	{
		//printf("Unable to open file!");
		//return 1;
	}

	// fseek(ptr_dl_tti,sizeof(uint16_t),SEEK_CUR);

	Dl_ttiRequest.MsgId = get_dl_tti_value_bits(16);
	Dl_ttiRequest.MsgLength = get_dl_tti_value_bits(32);
	Dl_ttiRequest.SFN = get_dl_tti_value_bits(16);
	Dl_ttiRequest.Slot = get_dl_tti_value_bits(16);
	Dl_ttiRequest.nPDUs = get_dl_tti_value_bits(8);
	Dl_ttiRequest.nGroup = get_dl_tti_value_bits(8);
	// xil_//printf("Dl_ttiRequest.MsgLength = %x\n", Dl_ttiRequest.MsgLength);
	// xil_//printf("Dl_ttiRequest.Slot = %x\n", Dl_ttiRequest.Slot);
	// xil_//printf("Dl_ttiRequest.nPDUs = %x\n", Dl_ttiRequest.nPDUs);
	// xil_//printf("Dl_ttiRequest.nGroup = %x ", Dl_ttiRequest.nGroup);

	while(Pdu_Count < (int)Dl_ttiRequest.nPDUs)//size of total dltti pdu count
	{
		Dl_ttiRequest.DLPDU.PDUType = get_dl_tti_value_bits(16);
		Dl_ttiRequest.DLPDU.PDUSize = get_dl_tti_value_bits(16);
		//xil_//printf("Dl_ttiRequest.DLPDU.PDUType = %x \n", Dl_ttiRequest.DLPDU.PDUType);
		////printf("Dl_ttiRequest.DLPDU.PDUType = %d \n", Dl_ttiRequest.DLPDU.PDUType);

		switch((int)Dl_ttiRequest.DLPDU.PDUType)
		{
			case PDCCH_PDU_TAG:
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPSize = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].SubcarrierSpacing = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CyclicPrefix = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].StartSymbolIndex = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].DurationSymbols = get_dl_tti_value_bits(8);
				//xil_//printf("%d, %d",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPSize,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].DurationSymbols);
				////printf("Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart = %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].BWPStart);
				//xil_//printf("%d",MAX_FREQ_DOM_RESOURCE);
				for(var1 = 0;var1<MAX_FREQ_DOM_RESOURCE;var1++)
				{
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1] = get_dl_tti_value_bits(8);
					//xil_//printf("fr[%d]= %d ",var1,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].FreqDomainResource[var1]);
				}
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CceRegMappingType = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].RegBundleSize = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].InterleaverSize = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].CoreSetType = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].ShiftIndex = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].precoderGranularity = get_dl_tti_value_bits(8);
				//xil_//printf("precoder granu= %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].precoderGranularity);
				Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci = get_dl_tti_value_bits(16);
				for(var1 = 0;var1<(int)Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numDlDci;var1++)
				{
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].RNTI = get_dl_tti_value_bits(16);// number dldcis yet to know the values
					//xil_//printf("%d",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[0].RNTI);

					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingId = get_dl_tti_value_bits(16);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].ScramblingRNTI = get_dl_tti_value_bits(16);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].CceIndex = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].AggregationLevel = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs = get_dl_tti_value_bits(16);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.prgSize = get_dl_tti_value_bits(16);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces = get_dl_tti_value_bits(8);

					//xil_//printf("Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs = %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs );
					//xil_//printf("digbf-interface = %d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);
					for(var2=0;var2<(uint16_t)(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.numPRGs);var2++)
					{
						Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = get_dl_tti_value_bits(16);
						//xil_//printf("[0]pmidx=%d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
						for(var3=0;var3<(uint8_t)(Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.digBFInterfaces);var3++)
						{
							Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = get_dl_tti_value_bits(16);
							//xil_//printf(" %d , v4=%d\n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] ,var4);
						}
					}

					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0 = get_dl_tti_value_bits(8);
					//xil_//printf("beta_pdcch=%d \n",Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].beta_PDCCH_1_0);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].powerControlOffsetSS = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].PayloadSizeBits = get_dl_tti_value_bits(16);
					for(var2 = 0;var2<(int)(DCI_PAYLOAD_BTYE_LEN);var2++)
					{
						Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] = get_dl_tti_value_bits(8);
						//xil_//printf("payload[%d] = %d \n ",var2,Dl_ttiRequest.PDCCH_PDU[Pdcch_Count].numOfDlDcis[var1].Payload[var2] );
					}
				}
				Pdcch_Count = Pdcch_Count+1;
				//xil_//printf("------------------------------------------Pdcch_Count--------------------------------------- %d",Pdcch_Count);
				break;

			case PDSCH_PDU_TAG:
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap = get_dl_tti_value_bits(16);
				//xil_//printf("i=%d , Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap = %d \n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].RNTI = get_dl_tti_value_bits(16);
				//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].RNTI = %x \n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].RNTI);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduIndex =	get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].BWPSize = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].BWPStart = get_dl_tti_value_bits(16);
				////printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].BWPStart = %d \n",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].BWPStart);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].SubcarrierSpacing = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CyclicPrefix = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords = get_dl_tti_value_bits(8);
				//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords = %x \n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords);
				for(var1=0;var1<(int)(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfCodewords);var1++)
				{
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].targetCodeRate = get_dl_tti_value_bits(16);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].qamModOrder = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].mcsIndex = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].mcsTable = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].rvIndex = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].TBSize = get_dl_tti_value_bits(32);
					//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].TBSize = %d",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].CodewordInfo[var1].TBSize);
				}

				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dataScramblingId = get_dl_tti_value_bits(16);
				//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dataScramblingId = %d \n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dataScramblingId);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].nrOfLayers = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].transmissionScheme = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].refPoint = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlDmrsSymbPos = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dmrsConfigType = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlDmrsScramblingId = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].SCID = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].numDmrsCdmGrpsNoData = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dmrsPorts = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc = get_dl_tti_value_bits(8);
				//xil_//printf(" Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc = %d \n", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc);
				for(var2 = 0;var2<(int)(MAX_RB_BITMAP);var2++)
				{
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap[var2] = get_dl_tti_value_bits(8);
					//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap[var2] = %d \n ",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap[var2] );
				}
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbStart = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize = get_dl_tti_value_bits(16);
				//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize = %d \n ",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].VRBtoPRBMapping = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].StartSymbolIndex = get_dl_tti_value_bits(8);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].NrOfSymbols = get_dl_tti_value_bits(8);
				if ((int)Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap & 0b0001)
				{
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSPortIndex = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSTimeDensity = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSFreqDensity = get_dl_tti_value_bits(8);
					//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSFreqDensity = %x ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSFreqDensity);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].PTRSReOffset = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].nEpreRatioOfPDSCHToPTRS = get_dl_tti_value_bits(8);
				}
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs = get_dl_tti_value_bits(16);
				//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs = %x ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.prgSize = get_dl_tti_value_bits(16);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces = get_dl_tti_value_bits(8);
				//xil_//printf(" Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces = %x ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces);
				for(var2=0;var2<(int)(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.numPRGs);var2++)
				{
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = get_dl_tti_value_bits(16);
					for(var3=0;var3<(int)(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.digBFInterfaces);var3++)
					{
						Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = get_dl_tti_value_bits(16);
					}
					//xil_//printf(" Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[%d].PMidx = %d\n",var2,Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
				}

				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffset = get_dl_tti_value_bits(8);
				//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffset =%d	\n",Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffset);
				Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].powerControlOffsetSS = get_dl_tti_value_bits(8);
				if ((int)Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].pduBitmap & 0b0010)
				{
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].IsLastCbPresent = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].isInlineTbCrc = get_dl_tti_value_bits(8);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlTbCrc = get_dl_tti_value_bits(32);
					//xil_//printf("Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlTbCrc = %x ", Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].dlTbCrc);
				}
				// Currently we support only the contiguous allocation, so we convert the bitmap to contiguous allocation if Resource Allocation = 0 and process the Allocation as type 1
				if( Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc == 0 ){
					int rbStart, rbSize ;
					get_contiguous_RB_alloc_from_bitmap(Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbBitmap,&rbStart,&rbSize);
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].resourceAlloc = 1;
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbStart = rbStart;
					Dl_ttiRequest.PDSCH_PDU[Pdsch_Count].rbSize = rbSize;
				}
				Pdsch_Count = Pdsch_Count+1;
				//xil_//printf("------------------------------------------Pdsch_Count--------------------------------------- %d",Pdsch_Count);
				break;

			case CSIRS_PDU_TAG:
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPSize = get_dl_tti_value_bits(16);
				//xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPSize = %d", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPSize);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPStart = get_dl_tti_value_bits(16);
				//xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPStart = %x ", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].BWPStart);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].SubcarrierSpacing = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].CyclicPrefix = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].StartRB = get_dl_tti_value_bits(16);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].NrOfRBs = get_dl_tti_value_bits(16);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].CSIType = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].Row = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].FreqDomain = get_dl_tti_value_bits(16);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].SymbL0 = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].SymbL1 = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].CDMType = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].FreqDensity = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].ScrambId = get_dl_tti_value_bits(16);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffset = get_dl_tti_value_bits(8);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffsetSS = get_dl_tti_value_bits(8);
				//xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffset = %x ", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].powerControlOffset);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.numPRGs = get_dl_tti_value_bits(16);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.prgSize = get_dl_tti_value_bits(16);
				Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.digBFInterfaces = get_dl_tti_value_bits(8);
				//xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.digBFInterfaces = %x ", Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.digBFInterfaces);
				for(var2=0;var2<(int)(Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.numPRGs);var2++)
				{
					Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx = get_dl_tti_value_bits(16);
					for(var3=0;var3<(int)(Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.prgSize);var3++)
					{
						Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3]	= get_dl_tti_value_bits(16);
					}
					//xil_//printf("Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[%d].PMidx=%d",var2,Dl_ttiRequest.CSIRS_PDU[Csirs_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx);
				}
				Csirs_Count = Csirs_Count+1;
				//xil_//printf("------------------------------------------csirs_Count--------------------------------------- %d",Csirs_Count);
				break;

			case SSB_PDU_TAG:
				Dl_ttiRequest.SSB_PDU[Ssb_Count].physCellId = get_dl_tti_value_bits(16);
				//xil_//printf("cellid =%x \n",Dl_ttiRequest.SSB_PDU[Ssb_Count].physCellId);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].betaPss = get_dl_tti_value_bits(8);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex = get_dl_tti_value_bits(8);
				//xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex = %x ", Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbBlockIndex);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].ssbSubcarrierOffset = get_dl_tti_value_bits(8);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].SsbOffsetPointA = get_dl_tti_value_bits(16);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayloadFlag = get_dl_tti_value_bits(8);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload = get_dl_tti_value_bits(32);
				//xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload	= %x ", Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload );
				Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs = get_dl_tti_value_bits(16);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.prgSize = get_dl_tti_value_bits(16);
				Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces = get_dl_tti_value_bits(8);
				//xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs = %x", Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs);
				//xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces = %x ", Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces);
				for(var2=0;var2<(int)(Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.numPRGs);var2++)
				{
					Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].PMidx= get_dl_tti_value_bits(16);
					for(var3=0;var3<(int)(Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.digBFInterfaces);var3++)
					{
						Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = get_dl_tti_value_bits(16);
					}
					//xil_//printf("Dl_ttiRequest.SSB_PDU[Ssb_Count].numOfDlDcis[0].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3] = %x ",var2,var3, Dl_ttiRequest.SSB_PDU[Ssb_Count].TxPrecoding_Beamforming.NumOfPRGs[var2].beamIdx[var3]);
				}
				ConfigRequest.SSBTable.MIB.Value = Dl_ttiRequest.SSB_PDU[Ssb_Count].bchPayload.MACgenMIBPDU.bchPayload;
				Ssb_Count = Ssb_Count +1;
				//xil_//printf("------------------------------------------Ssb_Count--------------------------------------- %d",Ssb_Count);
				break;

			default:
				break;
		}
		Pdu_Count=Pdu_Count+1;
	}

	while(Group_Count<(int)(Dl_ttiRequest.nGroup))
	{
		Dl_ttiRequest.UeList[nUe_Count].nUe = get_dl_tti_value_bits(8);
		for(var1=0;var1<(int)(Dl_ttiRequest.UeList[nUe_Count].nUe);var1++)
		{
			Dl_ttiRequest.UeList[nUe_Count].PduIdx[var1] = get_dl_tti_value_bits(8);
			//xil_//printf("Dl_ttiRequest.UeList[nUe_Count].PduIdx[%d]	= %x",var1,Dl_ttiRequest.UeList[nUe_Count].PduIdx[var1]);
		}
		nUe_Count = nUe_Count + 1;
		//xil_//printf("------------------------------------------nUe_Count--------------------------------------- %d\n",nUe_Count);
		Group_Count = Group_Count+1;
	}

	Dl_tti_intermediate.nPDSCH = Pdsch_Count;
	Dl_tti_intermediate.nPDCCH = Pdcch_Count;
	Dl_tti_intermediate.nCSIRS = Csirs_Count;
	Dl_tti_intermediate.nSSB = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// SSB symbol index calculation in a half-frame: Case C (TS 38.213): {2,8}+14*n, n=0,1,2,3.
	//  So, possible SSB symbol positions are: 2,8,16,22,30,36,44,50
	//          -- Minimum = 2  in slot0  ( repeat for slot10 if SsbPeriod=0 for 5ms period)
	//          -- Maximum = 50 in slot3  ( repeat for slot13 if SsbPeriod=0 for 5ms period)
	// So SSB will be transmitted only during the first 4 slots of every "half-frame".
	// SSB Mask[0] bitmap determines which of these symbols are actually present
	//   -- bitmap order is MSB first. Only consider first 8 MSB bits, i.e.
	//             -- bit31 = 1 => SSB present at symbol 2  (slot0)
	//             -- bit30 = 1 => SSB present at symbol 8  (slot0)
	//             -- bit29 = 1 => SSB present at symbol 16 (slot1, symbol2)
	//             -- bit28 = 1 => SSB present at symbol 22 (slot1, symbol8)
	//             -- bit27 = 1 => SSB present at symbol 30 (slot2, symbol2)
	//             -- bit26 = 1 => SSB present at symbol 36 (slot2, symbol8)
	//             -- bit25 = 1 => SSB present at symbol 44 (slot3, symbol2)
	//             -- bit24 = 1 => SSB present at symbol 50 (slot3, symbol8)
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (Dl_ttiRequest.Slot%10 <=3 ){ // Only needs to check first 4 slots of every "half-frame" for possible SSB transmission
		if (ConfigRequest.SSBTable.SsbPeriod.Value == 0){ // 5ms period
			if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*(Dl_ttiRequest.Slot%10)-1)))
				Dl_tti_intermediate.nSSB += 1; // symbol2 present
			if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*(Dl_ttiRequest.Slot%10)-2)))
				Dl_tti_intermediate.nSSB += 2; // symbol8 present
		}
		else{
			if (Dl_ttiRequest.SFN%ConfigRequest.SSBTable.SsbPeriod.Value == 0){
				if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*Dl_ttiRequest.Slot-1)))
					Dl_tti_intermediate.nSSB += 1; // symbol2 present
				if (ConfigRequest.SSBTable.SsbMask[0].Value & (uint32_t)((uint32_t)1<<(32-2*Dl_ttiRequest.Slot-2)))
					Dl_tti_intermediate.nSSB += 2; // symbol8 present
			}
		}
	}

	// validation: check if SSB is received as per configuration
	if ((Ssb_Count>0) && (Dl_tti_intermediate.nSSB==0)){
		///////////////////////////////////////////
		// TODO: When ConfigRequest.SSBTable.SsbPeriod.Value > 1 ( i.e. period >= 20 ms), there could be an error scenario such that
		// SSB pdu is received from Sooktha in the current SFN, but  Dl_ttiRequest.SFN%ConfigRequest.SSBTable.SsbPeriod.Value != 0
		// This case happens when SSB transmitted with a frame number offset, say "ssbFrameOffset" such that
		// (Dl_ttiRequest.SFN+ssbFrameOffset)%ConfigRequest.SSBTable.SsbPeriod.Value = 0. Not sure if this will happen. But if it happens
		//  we may have to find value of "ssbFrameOffset" using the SFN for which SSB pdu is received from Sooktha and then replace the condition
		//     if  ((Dl_ttiRequest.SFN)%ConfigRequest.SSBTable.SsbPeriod.Value == 0)
		//   to
		//     if  ((Dl_ttiRequest.SFN+ssbFrameOffset)%ConfigRequest.SSBTable.SsbPeriod.Value == 0)
		// For that we can use ssbFrameOffset as a static variable and find its value only when Ssb_Count > 0.
		////////////////////////////////////////////
		//printf("\nError: SSB pdu is received in wrong slot=%d or wrong SFN=%d where SsbPeriod=%d, SsbMask=0x%x\n",Dl_ttiRequest.Slot,Dl_ttiRequest.SFN,ConfigRequest.SSBTable.SsbPeriod.Value,ConfigRequest.SSBTable.SsbMask[0].Value);
	}
}


/*
Function: merge_DL

	Function to merge two arrays in ascending order. A part of merge sort.
	The mergesort_DL is specific to the sorting of code blocks in the cbs_configs_DL
	function. Thus, it requires that cb_start_time, cb_start_freq and
	sorted_CB_idx be globally available. Given the values l,m and r, the
	function merges sorted_CB_idx[l...m] and sorted_CB_idx[m+1...r].

	This is specific to sorting the sorted_CB_idx array!!!

	COPIED FROM
	https://www.geeksforgeeks.org/merge-sort/

	Input:
		l (int): value of l from above description
		m (int): value of m from above description
		r (int): value of r from above description

	Output:

*/
void merge_DL(int l, int m, int r){
	int n1 = m - l + 1;
    int n2 = r - m;
    int i = 0;
    int j = 0;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data into temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = sorted_CB_idx[l + i];
    for (j = 0; j < n2; j++)
        R[j] = sorted_CB_idx[m + 1 + j];

    // Merge the temp arrays back into sorted_CB_idx[l..r]

    // Initial index of first subarray
    i = 0;

    // Initial index of second subarray
    j = 0;

    // Initial index of merged subarray
    int k = l;

    while (i < n1 && j < n2) {
        if (cb_start_time[L[i]] > cb_start_time[R[j]] || cb_start_time[L[i]] == cb_start_time[R[j]] && cb_start_freq[L[i]] > cb_start_freq[R[j]]){
            sorted_CB_idx[k] = R[j];
            j++;

        }
        else {
            sorted_CB_idx[k] = L[i];
            i++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        sorted_CB_idx[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        sorted_CB_idx[k] = R[j];
        j++;
        k++;
    }


}





/*
Function: mergesort_DL

	Function to sort the sort_CB_idx array in ascending order using merge sort.
	The mergesort_DL is specific to the sorting of code blocks in the cbs_configs_DL
	function. Thus, it requires that cb_start_time, cb_start_freq and
	sorted_CB_idx be globally available. Given the values l and r, the
	function sorts sorted_CB_idx[l...r].

	This is specific to sorting the sorted_CB_idx array!!!

	COPIED FROM
	https://www.geeksforgeeks.org/merge-sort/

	Input:
		l (int): value of l from above description
		r (int): value of r from above description

	Output:

*/

void mergesort_DL(int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergesort_DL(l, m);
        mergesort_DL(m + 1, r);

        merge_DL(l, m, r);
    }
}





/*
Function: cbs_configs_DL
Author: Aniruddh V

	Function to split ALL the TBs into CBs. Also, decides the order in which the CBs are processed.
	The TB properties (mainly number of CBs and CB size) are stored in the Dl_tti_intermediate structure.
	The CBs are ordered based on their "projected" starting RE in the final grid.

	Inputs:
		No inputs. The function uses the global structures of Dl_tti_intermediate and Dl_ttiRequest
		 
	Output (to file):
		The number of codeblocks and codeblock size for each transport block.
		The address in which the TB is written, the address range from which each CB is read
		The codeblock size and the CRC option (24B) is sent for the CB CRC module
		The order of the codeblock processing (These are printed to file for DEBUGGING ONLY)

*/

void cbs_configs_DL(){
	
	int CB_sizes[MAX_CB];
	int CB_start_loc[MAX_CB];
	int addr_loc = 0;
	

	int freq_start;
	int freq_len;
	int nLayers;
	int time_start;
	int TB_size;
	int TBcrc_size , num_CB , CB_size;
	double R;
	int tb_size;
	int idx;
	int write_reset_variable = 1;
	int read_reset_variable = 1;

	int i,j,k,tb_num = 0;
	int n_sf = (int)Dl_ttiRequest.Slot;
	Dl_tti_intermediate.nCB = 0;
	int Qm;

	for (i = 0; i < Dl_tti_intermediate.nPDSCH; ++i)
	{	
		// This definition of freq_start assumes that PDSCH allocation type 1 is being used
		// TODO: fix this
		freq_start = (int)Dl_ttiRequest.PDSCH_PDU[i].BWPStart + Dl_ttiRequest.PDSCH_PDU[i].rbStart;
		freq_len = (int)Dl_ttiRequest.PDSCH_PDU[i].rbSize;
		nLayers = (int)Dl_ttiRequest.PDSCH_PDU[i].nrOfLayers;
		time_start = (int)Dl_ttiRequest.PDSCH_PDU[i].StartSymbolIndex;

		for (j = 0; j < Dl_ttiRequest.PDSCH_PDU[i].NrOfCodewords; ++j)
		{

			Dl_tti_intermediate.TB_prop[tb_num].q = (uint8_t)j;

			TB_size = (int)Dl_ttiRequest.PDSCH_PDU[i].CodewordInfo[j].TBSize*8;
			
			R = (double)Dl_ttiRequest.PDSCH_PDU[i].CodewordInfo[j].targetCodeRate;
			Qm = (int)Dl_ttiRequest.PDSCH_PDU[i].CodewordInfo[j].qamModOrder;

			if (TB_size > 3824)
				TBcrc_size = TB_size + 24;
			else
				TBcrc_size = TB_size + 16;
			Dl_tti_intermediate.TB_prop[tb_num].TBcrc = (uint16_t)TBcrc_size;
			Dl_tti_intermediate.TB_prop[tb_num].rvIndex = Dl_ttiRequest.PDSCH_PDU[i].CodewordInfo[j].rvIndex;
			Dl_tti_intermediate.TB_prop[tb_num].qamModOrder = Dl_ttiRequest.PDSCH_PDU[i].CodewordInfo[j].qamModOrder;
			Dl_tti_intermediate.TB_prop[tb_num].user_id = (uint8_t)i;

			// Write FSM
			int tb_size = (int)ceil((double)TBcrc_size/128);
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CBS_TB<<56) + (uint64_t)((uint64_t)tb_size<<5) + (uint64_t)n_sf + (uint64_t)((uint64_t)write_reset_variable<<19) + (uint64_t)((uint64_t)Dl_ttiRequest.SFN<<20);
			write_reset_variable = 0;
			//fprintf(fp, "Hardware packet for write FSM is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			if ( (TB_size <= 292) || (R <= 0.25*10240) || (TB_size <= 3824 && R <= 0.67*10240) ){
				// BG 2
				if (TBcrc_size <= 3840){
					num_CB = 1;
					CB_size = TBcrc_size;
					Dl_tti_intermediate.TB_prop[tb_num].K_dash = (uint16_t)CB_size;
				}
				else{
					num_CB = (int)ceil((double)TBcrc_size/(3840-24));
					CB_size = (int)ceil((double)TBcrc_size/num_CB);
					Dl_tti_intermediate.TB_prop[tb_num].K_dash = (uint16_t)CB_size+24;
				}
				Dl_tti_intermediate.TB_prop[tb_num].base_graph_index = 2;
				// Computing K_b based on the size of B = TB size A + CRC size
				if (TBcrc_size > 640)
					Dl_tti_intermediate.TB_prop[tb_num].K_b = 10;
				else if (TBcrc_size > 560)
					Dl_tti_intermediate.TB_prop[tb_num].K_b = 9;
				else if (TBcrc_size > 192)
					Dl_tti_intermediate.TB_prop[tb_num].K_b = 8;
				else 
					Dl_tti_intermediate.TB_prop[tb_num].K_b = 6;
			}
			else{
				// BG 1
				if (TBcrc_size <= 8448){
					num_CB = 1;
					CB_size = TBcrc_size;
					Dl_tti_intermediate.TB_prop[tb_num].K_dash = (uint16_t)CB_size;
				}
				else{
					num_CB = (int)ceil((double)TBcrc_size/(8424));
					CB_size = (int)ceil((double)TBcrc_size/num_CB);
					Dl_tti_intermediate.TB_prop[tb_num].K_dash = (uint16_t)CB_size+24;
				}
				Dl_tti_intermediate.TB_prop[tb_num].base_graph_index = 1;
				Dl_tti_intermediate.TB_prop[tb_num].K_b = 22; 				// K_b is 22 for BG1
			}
			Dl_tti_intermediate.TB_prop[tb_num].num_CB = (uint8_t)num_CB;

			//fprintf(fp,"PDU ID %d , Codeword %d , Number of codeblocks %d , Codeblock size %d\n",i,j,num_CB,CB_size);

			int layers;
			if (Dl_ttiRequest.PDSCH_PDU[i].NrOfCodewords == 1)
				layers = nLayers;
			else{
				layers = floor((nLayers+j)/2);
			}
			Dl_tti_intermediate.TB_prop[tb_num].numLayers = (uint8_t)layers;
			addr_loc = 128*ceil((double)addr_loc/128);

			int CB_numRE_layer = (CB_size*10240/(R*Qm*nLayers));
			int dmrs_locs = Dl_ttiRequest.PDSCH_PDU[i].dlDmrsSymbPos;
			int num_dmrs_sym = (6-2*Dl_ttiRequest.PDSCH_PDU[i].dmrsConfigType)*Dl_ttiRequest.PDSCH_PDU[i].numDmrsCdmGrpsNoData*Dl_ttiRequest.PDSCH_PDU[i].rbSize;
			for (k = 0; k < num_CB; ++k){
				int total_RE = k*CB_numRE_layer;
				for (int j = 0; j <= total_RE/(12*freq_len); ++j){
					if (dmrs_locs & (1<<j)){
						total_RE += num_dmrs_sym;
					}
				}

				cb_start_time[Dl_tti_intermediate.nCB] = time_start + total_RE/(12*freq_len);
				cb_start_freq[Dl_tti_intermediate.nCB] = freq_start + total_RE%(12*freq_len);
				// //printf("%d \n", cb_start_freq[Dl_tti_intermediate.nCB]);
				CB_sizes[Dl_tti_intermediate.nCB] = CB_size;
				CB_start_loc[Dl_tti_intermediate.nCB] = addr_loc;
				CB_TB_idx[Dl_tti_intermediate.nCB] = tb_num;
				CB_idx_in_TB[Dl_tti_intermediate.nCB] = k;
				addr_loc += CB_size;
				++Dl_tti_intermediate.nCB;
			}
			++tb_num;
		}
	}
	Dl_tti_intermediate.nTB = tb_num;


	
	// Sort the CBs according to the time-frequency order (Merge-Sort)
	for (i = 0; i < Dl_tti_intermediate.nCB; ++i){
		sorted_CB_idx[i] = i;
	}

	mergesort_DL(0, Dl_tti_intermediate.nCB-1);	

	// int dummy_array[MAX_CB] = {};
	// for (i = 0; i < Dl_tti_intermediate.nCB; ++i){
	// 	dummy_array[i] = i;
	// }
	// int cur_index,next_index;
	// for (int i = 0; i < Dl_tti_intermediate.nCB - 1; ++i){
	// 	for (int j = 0; j < Dl_tti_intermediate.nCB - i - 1; ++j){
	// 		cur_index = dummy_array[j];
	// 		next_index = dummy_array[j+1];
	// 		if (cb_start_time[cur_index] > cb_start_time[next_index] || cb_start_time[cur_index] == cb_start_time[next_index] && cb_start_freq[cur_index] > cb_start_freq[next_index]){
	// 			dummy_array[j] = next_index;
	// 			dummy_array[j+1] = cur_index;
	// 		}
	// 	}
	// }

	// // Code to verify equivalence between bubble and merge sorts
	// FILE *dummy_fp;
	// dummy_fp = fopen("array_comparison.txt", "w");
	// int dummy_i;
	// printf(dummy_fp, "Bubble Merge Error\n");
	// for (dummy_i = 0; dummy_i < Dl_tti_intermediate.nCB; dummy_i++){
	// 	printf(dummy_fp, "%d %d %d \n", dummy_array[dummy_i], sorted_CB_idx[dummy_i], dummy_array[dummy_i] - sorted_CB_idx[dummy_i]);
	// 	if (dummy_array[dummy_i] - sorted_CB_idx[dummy_i] != 0){
	// 		printf(dummy_fp, "Error: sorting function working incorrectly!");
	// 		break;
	// 	}
	// }
	// fclose(dummy_fp);

	for (i = 0; i < Dl_tti_intermediate.nCB; ++i){
		int idx = sorted_CB_idx[i];

		// Read FSM
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CBS<<56) + (uint64_t)n_sf + (uint64_t)((uint64_t)(CB_start_loc[idx]/128)<<5) + (uint64_t)((uint64_t)(CB_start_loc[idx]%128)<<20);
		//fprintf(fp, "\nHardware packet1 for READ FSM is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CBS<<56) + (uint64_t)CB_sizes[idx] + (uint64_t)((uint64_t)read_reset_variable<<21) + (uint64_t)((uint64_t)Dl_ttiRequest.SFN<<22) ;
		read_reset_variable = 0;
		//fprintf(fp, "Hardware packet2 for READ FSM is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "Codeblock index : %d\n", idx);
		//fprintf(fp, "Slot number : %d\n", n_sf);
		//fprintf(fp, "Starting Address : %d\n", CB_start_loc[idx]>>7);
		//fprintf(fp, "Starting Bit location : %d\n", CB_start_loc[idx]%128);
		//fprintf(fp, "CB length : %d\n", CB_sizes[idx]);

		// CB CRC
		int is_crc = (Dl_tti_intermediate.TB_prop[CB_TB_idx[idx]].num_CB > 1)?1:3;
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CBCRC<<56) + (uint64_t)(CB_sizes[idx]%128) + (uint64_t)((uint64_t)(CB_sizes[idx]>>7)<<8) + (uint64_t)((uint64_t)is_crc<<24);
		//fprintf(fp, "\nHardware packet for CB CRC is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "l1_r is : %d\n", CB_sizes[idx]%128);
		//fprintf(fp, "l1_k is : %d\n", CB_sizes[idx]>>7);
		//fprintf(fp, "l1_poly_type : %d\n", is_crc);
	}

}





/*
Function: generate_ldpc_config_DL
Author: Milind Kumar V

	Function to generate the values requried for the LDPC mdoule. The outputs are written to a file. The 
	parameter generation is based on TS 38.212. Requires the presence of the variable Dl_ttiRequest and 
	Dl_tti_intermediate_t. The required parameters are generated for each code block. The function 
	deals with each code block separately instead of with individual PDUs. However, as the configuration
	to be generated is the same across all code blocks belonging to a given transport block, only the 
	Transport block index is given as input. 

	Inputs:
		TB_index (int): Given the Dl_tti_intermediate structure, this is an index into the array TB_prop
			and indicates the TB number that is being processed.
		 
	Output (to file):
		total_no_of_message_bits (hexadecimal): a 14 bit value indicating the total number of bits in the the 
			code block before the addition of filler bits. This is directly obtained from the Dl_tti_intermediate
			structure. 
		base_graph_index (hexadecimal): a single bit value indicating whether base graph 1 should be selected or 
			base graph 2. This is directly obtained from the Dl_tti_intermediate structure.
		lifting_size_index (hexadecimal): a 6 bit value indicating the lifting size index. The index corresponds 
			to the location of Z_c in a sorted array of lifting sizes from table 5.3.2-1 
		user_id (int): A common value assinged to all code blocks of a given user 

*/

void generate_ldpc_config_DL(int TB_index){

	int K; 								// number of bits in each code block, this is the same for all code blocks
	int Z_c;							// minimum value of lifting size according to Section 5.2.2, TS 38.212
	int lifting_size_index = 0;			// location of lifting size in the sorted array of lifting sizes from Table 5.3.2-1, TS 38.212
	int total_no_of_message_bits;		// Total number of message bits in one codeblock before addition of filler bits, equal to K_dash from Sectoin 5.2.2, TS 38.212
	int base_graph_index;				// Base graph index
	int user_id;

	// Determining the total number of bits
	total_no_of_message_bits = Dl_tti_intermediate.TB_prop[TB_index].K_dash;

	// Obtaining the base_graph_index for the TB
	base_graph_index = Dl_tti_intermediate.TB_prop[TB_index].base_graph_index;

	// Obtaining the user_id
	user_id = Dl_tti_intermediate.TB_prop[TB_index].user_id;

	// Obtaining the lifting size Z_c and lifting size index
	// TODO : Would binary search be better than a linear scan?
	Z_c = (int)LIFTING_SIZE_Z_DL[lifting_size_index];
	while ((int)Dl_tti_intermediate.TB_prop[TB_index].K_b*Z_c < (int)Dl_tti_intermediate.TB_prop[TB_index].K_dash){
		lifting_size_index = lifting_size_index + 1;
		Z_c = (int)LIFTING_SIZE_Z_DL[lifting_size_index];
	}

	// Adding parameters to the Dl_tti_intermediate structure for future use by puncturing and rate matching modules
	// //printf("%d", lifting_size_index);
	Dl_tti_intermediate.TB_prop[TB_index].Z_c = (uint16_t)Z_c;
	Dl_tti_intermediate.TB_prop[TB_index].lifting_size_index = (uint8_t)lifting_size_index; 

	// Determining the number of bits in the code block after filler bit addition
	if ((int)Dl_tti_intermediate.TB_prop[TB_index].base_graph_index == 1){
		K = 22*Z_c;		
	} else {
		K = 10*Z_c;	
	}
	// Adding K to the Dl_tti_intermediate structure for use by puncturing and rate matching modules
	Dl_tti_intermediate.TB_prop[TB_index].K = (uint16_t)K; 

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_LDPC<<56) + (uint64_t)total_no_of_message_bits + (uint64_t)((uint64_t)base_graph_index<<14) + (uint64_t)((uint64_t)lifting_size_index<<16) + (uint64_t)((uint64_t)user_id<<22);
	//fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Writing to a file
	// TODO: Replace with write to hardware
	//fprintf(fp, "total_no_of_message_bits: %d \n", total_no_of_message_bits);
	//fprintf(fp, "base_graph_index %d \n", base_graph_index);
	//fprintf(fp, "lifting_size_index: %d \n", lifting_size_index);
	//fprintf(fp, "user_id: %d \n", user_id);

}





/*
Function: generate_puncturing config
Author: Milind Kumar V

	Function to generate configurations for the puncturing module. Outputs are written to a file. Requires the presence of the 
	Dl_ttiRequest and Dl_tti_intermediate variables. Output is generated for individual code blocks. However, as the configuration
	to be generated is the same for all the code blocks of a given transport block, only the transport block index is given as the 
	input.

	Input:
		TB_index (int): Given the Dl_tti_intermediate structure, this is an index into the array TB_prop
			and indicates the TB number that is being processed.

	Output (to file):
		k_cb (hexadecimal): number of message bits in each codeblock of the TB before addition of filler bits
		k_cb_filler (hexadecimal): number of message bits in each codeblock after addition of filler bits
		Z (hexadecimal): Lifting factor obtained from Table 5.3.2-1, TS 38.212 for the given TB
		N_cb (hexadecimal): Value equal to N from Section 5.3.2, TS 38.212

*/

void generate_puncturing_config(int TB_index){

	int k_cb = (int)Dl_tti_intermediate.TB_prop[TB_index].K_dash;
	int k_cb_filler = 0;
	int Z = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c;
	int N_cb = 0;
	int K_b = (int)Dl_tti_intermediate.TB_prop[TB_index].K_b;		// parameter Kb from Section 5.2.2, TS 38.212

	if ((int)Dl_tti_intermediate.TB_prop[TB_index].base_graph_index == 1){
		N_cb = 66*Z;
		k_cb_filler = (int)Dl_tti_intermediate.TB_prop[TB_index].K;		
	} else {
		N_cb = (40 + K_b)*Z;
		k_cb_filler = K_b*Z;	
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PUNCTURING<<56) + (uint64_t)k_cb + (uint64_t)((uint64_t)k_cb_filler<<14);
	//fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PUNCTURING<<56) + (uint64_t)Z + (uint64_t)((uint64_t)N_cb<<9);
	//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Writing to file
	// TODO: Replace with write to hardware
	//fprintf(fp, "k_cb: %d \n", k_cb);
	//fprintf(fp, "K_cb: %d \n", k_cb_filler);
	//fprintf(fp, "Z: %d \n", Z);
	//fprintf(fp, "N_cb: %d \n", N_cb);

}





/*
Function: generate_rate_matching_config
Author: Milind Kumar V

	Input:
		PDSCH_pdu_index (int): index of the PDSCH PDU from which the Transport Block is obtained
		TB_index (int): index of the Transport Block being processed
		CB_index (int): index of the code block being processed, belongs to {0,1,..., num_CB-1}. Specific 
			to the TB being processed (given by TB_index) i.e a CB_index of i indicates the ith CB of the 
			TB corresponding to the TB given by TB_index. 	

	Output (to file):
		Ncb (int): circular buffer size 
		N_dash (int): Number of bits sent by puncturing module, N - (K - K') where N, K and K' are defined
			in Sections 5.2.2 and 5.3.2 of TS 38.212
		E (int): Number of bits after rate matching
		start_pos (int): starting position as defined by Table 5.4.2.1-2, TS 38.212 with correction for the
			behaviour of the puncturing module
*/

void generate_rate_matching_config(int PDSCH_pdu_index, int TB_index, int CB_index){

	int Ncb; 				// Size fo the circular buffer used for rate matching
	int Nref;				// Intermediate value used in the computation of Ncb
	int N;					// Number of bits after LDPC encoding
	int N_dash;				// Number of bits sent by the puncturing module (without the filler bits)
	int E;					// Number of bits in the output of the rate matching module
	int C_dash;				// Number of code blocks to be considered- depends on CBGTI (?)
	int Nl;					// Number of layers the TB is associated with, see Section 5.4.2.1, TS 38.212
	int Qm;					// Modulation order for the TB, see Section 5.4.2.1, TS 38.212
	int G;					// Number of coded bits available for transmission of TB, see Section 5.4.2.1, TS 38.212
	int start_pos; 			// Starting position, obtained from Table 5.4.2.1-2, TS 38.212, excluding the position of NULL bits
	double TBS_LBRM;		// Value of TBS_LBRM used to compute Nref

	int base_graph_index = (int)Dl_tti_intermediate.TB_prop[TB_index].base_graph_index;
	int rvIndex = (int)Dl_tti_intermediate.TB_prop[TB_index].rvIndex;
	TBS_LBRM = (double)Dl_tti_intermediate.TB_prop[TB_index].TBS_LBRM;
	// TODO: Remove the use of this dummy TBS_LBRM value
	TBS_LBRM = 100000000;	// Setting to 1e8 as setting to DBL_MAX caused overflow


	// Determining the value of N from Section 5.3.2, TS 38.212
	if ((int)Dl_tti_intermediate.TB_prop[TB_index].base_graph_index == 1){
		N = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*66;
	} else {
		N = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*50;
	}

	// Computing the value of Ncb
	Nref = floor(TBS_LBRM/((double)Dl_tti_intermediate.TB_prop[TB_index].num_CB*(2.0/3.0)));
	if (N <= Nref){
		Ncb = N;
	} else {
		Ncb = Nref;
	}
	Ncb = Ncb - (int)(Dl_tti_intermediate.TB_prop[TB_index].K - Dl_tti_intermediate.TB_prop[TB_index].K_dash);

	// Computing N_dash
	N_dash = N - (int)(Dl_tti_intermediate.TB_prop[TB_index].K - Dl_tti_intermediate.TB_prop[TB_index].K_dash);

	// Obtaining the value of E, see Section 5.4.2.1, TS 38.212
	// TODO: using CBGTI
	Nl = (int)Dl_tti_intermediate.TB_prop[TB_index].numLayers;
	Qm = (int)Dl_tti_intermediate.TB_prop[TB_index].qamModOrder;
	
	// TODO: Compute G in the general case
	// G is computed under the following assumptions- PDSCH resource allocation Type 1, non-interleaved VRB-to-PRB mapping
	if ((int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].resourceAlloc == 1){
		if ((int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].VRBtoPRBMapping == 0){
			G = Nl*(int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].rbSize*((int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].NrOfSymbols)*12 - (int)Dl_tti_intermediate.TB_prop[TB_index].reserved_RE; 
			Dl_tti_intermediate.TB_prop[TB_index].G_modulation = G;
			G = Qm*G;
		}
	}

	C_dash = (int)Dl_tti_intermediate.TB_prop[TB_index].num_CB; 	// CBGTI?

	if (0){					// Include CBGTI condition later

	} else { 				// using r not j, assumes that all CBs are being transmitted without regard to the if statement of Pg 29, 5.4.2.1, TS 38.212
		if (CB_index <= C_dash - ((G/(Nl*Qm))%C_dash) - 1){
			E = Nl*Qm*floor((double)G/((double)(Nl*Qm*C_dash)));
		} else {
			E = Nl*Qm*ceil((double)G/((double)(Nl*Qm*C_dash)));
		}
	}

	// Adding the E parameter to Dl_tti_intermediate for use by the bit interleaving module
	Dl_tti_intermediate.TB_prop[TB_index].E[CB_index] = (uint16_t)E;

	// }

	// Determining the starting position from Table 5.4.2.1-2, TS 38.212
	if (base_graph_index == 1){
		if (rvIndex == 0){
			start_pos = 0;
		} else if (rvIndex == 1){
			start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*floor((double)Ncb*17/((double)Dl_tti_intermediate.TB_prop[TB_index].Z_c*66));
		} else if (rvIndex == 2){
			start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*floor((double)Ncb*33/((double)Dl_tti_intermediate.TB_prop[TB_index].Z_c*66));
		} else if (rvIndex == 3){
			start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*floor((double)Ncb*56/((double)Dl_tti_intermediate.TB_prop[TB_index].Z_c*66));
		}
	} else if (base_graph_index == 2){
		if (rvIndex == 0){
			start_pos = 0;
		} else if (rvIndex == 1){
			start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*floor((double)Ncb*13/((double)Dl_tti_intermediate.TB_prop[TB_index].Z_c*50));
		} else if (rvIndex == 2){
			start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*floor((double)Ncb*25/((double)Dl_tti_intermediate.TB_prop[TB_index].Z_c*50));
		} else if (rvIndex == 3){
			start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*floor((double)Ncb*43/((double)Dl_tti_intermediate.TB_prop[TB_index].Z_c*50));
		}
	}

	// Puncturing module removes 2Zc bits from the start of the 22Zc bits of the code block
	// The lab implementation also removes the NULL bits added. Therefore, start_pos needs to be 
	// adjusted accordingly.

	if ((start_pos >= (int)Dl_tti_intermediate.TB_prop[TB_index].K_dash - (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*2) &&
	 (start_pos < (int)Dl_tti_intermediate.TB_prop[TB_index].K - (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*2) ){
		start_pos = (int)Dl_tti_intermediate.TB_prop[TB_index].K_dash - (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*2;
	} else if (start_pos >= (int)Dl_tti_intermediate.TB_prop[TB_index].K - (int)Dl_tti_intermediate.TB_prop[TB_index].Z_c*2){
		start_pos = start_pos - ((int)Dl_tti_intermediate.TB_prop[TB_index].K - (int)Dl_tti_intermediate.TB_prop[TB_index].K_dash);
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RATE_MATCH<<56) + (uint64_t)Ncb + (uint64_t)((uint64_t)N_dash<<16);
	//fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RATE_MATCH<<56) + (uint64_t)E + (uint64_t)((uint64_t)start_pos<<17);
	//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Writing outptus to a fileADDRESS_TX_RATE_MATCH
	// TODO: Write outputs to hardware
	//fprintf(fp, "Ncb: %d \n", Ncb);
	//fprintf(fp, "N_dash: %d \n", N_dash);
	//fprintf(fp, "E: %d \n", E);
	//fprintf(fp, "start_pos: %d \n", start_pos);

}





/*
Function: generate_bit_interleaving_config
Author: Milind Kumar V

	Function to generate configurations for the bit interleaving module

	Input:
		TB_index (int): index of the Transsport block being processed
		CB_index (int): index of the code block being processed, belongs to {0,1,..., num_CB-1}. Specific 
			to the TB being processed (given by TB_index) i.e a CB_index of i indicates the ith CB of the 
			TB corresponding to the TB given by TB_index. 	

	Output (to file):
		Q (int): modulation order
		Rmsize (int): number of bits to be transmitted after rate matching

*/

void generate_bit_interleaving_config(int TB_index, int CB_index){
	int Q;					// Modulation order
	int Rmsize;				// Number of bits in the output of the rate matching module

	Q = (int)Dl_tti_intermediate.TB_prop[TB_index].qamModOrder;
	Rmsize = (int)Dl_tti_intermediate.TB_prop[TB_index].E[CB_index];

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_BIT_INTERLEAVE<<56) + (uint64_t)Q + (uint64_t)((uint64_t)Rmsize<<4);
	//fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write to file
	// TODO: Replace with write to hardware
	//fprintf(fp, "Q: %d \n", Q);
	//fprintf(fp, "Rmsize: %d \n", Rmsize);

}





/*
Function: sort_PDU_DL
Author: Aniruddh V

	Function to sort the PDUs according to the beginning of the BWP they belong to.
	Each PDUtype is sorted individually within it's type.

	Inputs:
		The function expects the Dl_ttiRequest variable of type Dl_ttiRequest_t to be available as a global variable that can
		be accessed within the function. 
	Outputs:
		The global variable arrays are sorted for each PDUtype

*/

void sort_PDU_DL(){

	int PDU_BWP_start[MaxUE];
	int PDU_symb_start[MaxUE];
	int PDU_port_start[MaxUE];

	// A simple selection sort is used because nPDUs <= 256 and selection sort is O(n^2).
	int nPDU, cur_index, next_index,var1,var2;

	// Adding PDCCH PDUs
	nPDU = Dl_tti_intermediate.nPDCCH;
	for (int i = 0; i < nPDU; ++i){		
		// sort DCIs based on the cce index - this should be there
		numOfDlDcis_t tmpDCI;
		for(var1 = 0; var1<(int)Dl_ttiRequest.PDCCH_PDU[i].numDlDci; var1++){
			for(var2 = var1+1; var2<(int)Dl_ttiRequest.PDCCH_PDU[i].numDlDci; var2++){
				if(Dl_ttiRequest.PDCCH_PDU[i].numOfDlDcis[var1].CceIndex > Dl_ttiRequest.PDCCH_PDU[i].numOfDlDcis[var2].CceIndex){
					tmpDCI = Dl_ttiRequest.PDCCH_PDU[i].numOfDlDcis[var1];
					Dl_ttiRequest.PDCCH_PDU[i].numOfDlDcis[var1] = Dl_ttiRequest.PDCCH_PDU[i].numOfDlDcis[var2];
					Dl_ttiRequest.PDCCH_PDU[i].numOfDlDcis[var2] = tmpDCI;
				}
			}
		}
		
		sorted_PDCCH_idx[i] = i;
		PDU_BWP_start[i] = Dl_ttiRequest.PDCCH_PDU[i].BWPStart;
		for (int j = 0; j < 45; ++j){
			if (Dl_ttiRequest.PDCCH_PDU[i].FreqDomainResource[j/8] & (1<<(j%8))) {
				PDU_BWP_start[i] += 6*j;
				break;
			}
		}
		PDU_symb_start[i] = Dl_ttiRequest.PDCCH_PDU[i].StartSymbolIndex;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_PDCCH_idx[j];
			next_index = sorted_PDCCH_idx[j+1];
			if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_PDCCH_idx[j] = next_index;
				sorted_PDCCH_idx[j+1] = cur_index;
			}
		}
	}

	// Adding CSIRS PDUs
	nPDU = Dl_tti_intermediate.nCSIRS;
	for (int i = 0; i < nPDU; ++i){
		sorted_CSIRS_idx[i] = i;
		PDU_BWP_start[i] = Dl_ttiRequest.CSIRS_PDU[i].BWPStart + Dl_ttiRequest.CSIRS_PDU[i].StartRB;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_CSIRS_idx[j];
			next_index = sorted_CSIRS_idx[j+1];
			if (PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_CSIRS_idx[j] = next_index;
				sorted_CSIRS_idx[j+1] = cur_index;
			}
		}
	}

	// Adding DMRS PDUs
	nPDU = Dl_tti_intermediate.nPDSCH;
	for (int i = 0; i < nPDU; ++i){
		sorted_DMRS_idx[i] = i;
		PDU_BWP_start[i] = Dl_ttiRequest.PDSCH_PDU[i].BWPStart + Dl_ttiRequest.PDSCH_PDU[i].rbStart;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_DMRS_idx[j];
			next_index = sorted_DMRS_idx[j+1];
			if (PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_DMRS_idx[j] = next_index;
				sorted_DMRS_idx[j+1] = cur_index;
			}
		}
	}

	// Adding PDSCH PDUs
	nPDU = Dl_tti_intermediate.nPDSCH;
	for (int i = 0; i < nPDU; ++i){
		sorted_PDSCH_idx[i] = i;
		PDU_BWP_start[i] = Dl_ttiRequest.PDSCH_PDU[i].BWPStart + Dl_ttiRequest.PDSCH_PDU[i].rbStart;
		PDU_symb_start[i] = Dl_ttiRequest.PDSCH_PDU[i].StartSymbolIndex;
		int a = Dl_ttiRequest.PDSCH_PDU[i].dmrsPorts;
		for (int j = 0; j < 12; ++j){
			if (a%2 == 1){
				PDU_port_start[i] = j;
				break;
			}
			a /= 2;
		}
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_PDSCH_idx[j];
			next_index = sorted_PDSCH_idx[j+1];
			if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_PDSCH_idx[j] = next_index;
				sorted_PDSCH_idx[j+1] = cur_index;
			}
			else if (PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] == PDU_BWP_start[next_index] && PDU_port_start[cur_index] > PDU_port_start[next_index]){
				sorted_PDSCH_idx[j] = next_index;
				sorted_PDSCH_idx[j+1] = cur_index;
			}
		}
	}

	// // Adding SSB PDUs
	// nPDU = Dl_tti_intermediate.nSSB;
	// for (int i = 0; i < nPDU; ++i){
	// 	sorted_SSB_idx[i] = i;
	// 	PDU_BWP_start[i] = Dl_ttiRequest.SSB_PDU[i].SsbOffsetPointA;
	// 	PDU_symb_start[i] = Dl_ttiRequest.SSB_PDU[i].ssbBlockIndex;
	// }
	// for (int i = 0; i < nPDU - 1; ++i){
	// 	for (int j = 0; j < nPDU - i - 1; ++j){
	// 		cur_index = sorted_SSB_idx[j];
	// 		next_index = sorted_SSB_idx[j+1];
	// 		if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
	// 			sorted_SSB_idx[j] = next_index;
	// 			sorted_SSB_idx[j+1] = cur_index;
	// 		}
	// 	}
	// }
}





/*
Function: generate_dmrs_config_DL
Author: Aniruddh V

	Function to generate the configs for DMRS. Also calculates the REs reserved for DMRS
	parameter generation is based on TS 38.211; Requires the presence of the variable Dl_ttiRequest and 
	Dl_tti_intermediate_t. The required parameters are generated for each PDSCH PDU.

	Inputs:
		PDU_idx (int): This index tells the PDSCH PDU for which DMRS is generated
		 
	Output (to file):
		The start and length RB, nscid and scrambling ID, ports and OFDM symbols occupied, configuration

*/

void generate_dmrs_config_DL(int PDU_idx){
	int start_RB , num_RB , dmrs_type , n_SCID , layer_present , scrambling_ID , dmrs_sym_pos , n_sf , l_prime=0 ,start_ref_dmrs , beta_index;

	start_RB = Dl_ttiRequest.PDSCH_PDU[PDU_idx].BWPStart + Dl_ttiRequest.PDSCH_PDU[PDU_idx].rbStart;
	num_RB = Dl_ttiRequest.PDSCH_PDU[PDU_idx].rbSize;
	dmrs_sym_pos = Dl_ttiRequest.PDSCH_PDU[PDU_idx].dlDmrsSymbPos;
	dmrs_type = Dl_ttiRequest.PDSCH_PDU[PDU_idx].dmrsConfigType;
	n_SCID = Dl_ttiRequest.PDSCH_PDU[PDU_idx].SCID;
	scrambling_ID = Dl_ttiRequest.PDSCH_PDU[PDU_idx].dlDmrsScramblingId;
	n_sf = Dl_ttiRequest.Slot;
	layer_present = Dl_ttiRequest.PDSCH_PDU[PDU_idx].dmrsPorts;
	int a = dmrs_sym_pos , dmrs_sym_count = 0;
	for(int i = 0 ; i < 14 ; ++i){
		if (a%4 == 3)
			l_prime += (1<<(1+i));
		dmrs_sym_count += a%2;
		a /= 2;
	}

        start_ref_dmrs = 0;
        if ((Dl_ttiRequest.PDSCH_PDU[PDU_idx].refPoint == 1) &&
        	(Dl_ttiRequest.PDSCH_PDU[PDU_idx].RNTI == 65535)){ // check if SI_RNTI
                start_ref_dmrs = coreset0_dmrs_ref;
                start_RB = coreset0_dmrs_ref/12 + Dl_ttiRequest.PDSCH_PDU[PDU_idx].rbStart;
        }
    // beta index change starts 
        beta_index = 0; // incase it doesn't go to any loop
        if(Dl_ttiRequest.PDSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData == 1){
        	beta_index = 0;
        }
        else if(Dl_ttiRequest.PDSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData == 2){
        	beta_index = 1;
        }
        else if(Dl_ttiRequest.PDSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData == 3 && Dl_ttiRequest.PDSCH_PDU[PDU_idx].dmrsConfigType == 1 ){
        	beta_index = 2;
        }
    //

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)dmrs_type<<18) + (uint64_t)((uint64_t)n_SCID<<19) + (uint64_t)((uint64_t)layer_present<<20);
	//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56) + (uint64_t)scrambling_ID + (uint64_t)((uint64_t)dmrs_sym_pos<<16) + (uint64_t)((uint64_t)beta_index<<30);
	//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56) + (uint64_t)l_prime + (uint64_t)((uint64_t)n_sf<<14) + (uint64_t)((uint64_t)start_ref_dmrs<<18 );
	//fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	//fprintf(fp, "PDU index : %d \n", PDU_idx);
	//fprintf(fp, "Starting RB is : %d \n", start_RB);
	//fprintf(fp, "PRB length is : %d \n", num_RB);
	//fprintf(fp, "DMRS configuration type is : %d \n", dmrs_type);
	//fprintf(fp, "n_scid is : %d \n", n_SCID);
	//fprintf(fp, "The layers which are present are : 0X%04X \n", layer_present);
	//fprintf(fp, "DMRS scrambling ID is : %d \n", scrambling_ID);
	//fprintf(fp, "DMRS symbols are : 0X%04X \n", dmrs_sym_pos);
	//fprintf(fp, "The value of l_prime is : 0X%04X \n", l_prime);
	//fprintf(fp, "Slot number in the frame is : %d \n", n_sf);
    //fprintf(fp, "Reference RE for dmrs is : %d \n", start_ref_dmrs);
    //fprintf(fp, "beta_index is : %d \n", beta_index);

	// Update the reserved REs
	int G;
	if (dmrs_type == 0)
		// Type 1 config
		G = num_RB*dmrs_sym_count*(Dl_ttiRequest.PDSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData)*6;
	else
		// Type 2 config
		G = num_RB*dmrs_sym_count*(Dl_ttiRequest.PDSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData)*4;

	for (int i = 0; i < Dl_tti_intermediate.nTB; ++i){
		if (Dl_tti_intermediate.TB_prop[i].user_id == PDU_idx){
			Dl_tti_intermediate.TB_prop[i].reserved_RE = G*Dl_tti_intermediate.TB_prop[i].numLayers;
			//fprintf(fp, "The reserved REs for TB %d by DMRS is %d\n", i, Dl_tti_intermediate.TB_prop[i].reserved_RE);
		}
	}
}





/*
Function: generate_csirs_config
Author: Aniruddh V

	Function to generate the configs for CSIRS. Also calculates the REs reserved for CSIRS
	parameter generation is based on TS 38.211; Requires the presence of the variable Dl_ttiRequest and 
	Dl_tti_intermediate_t. The required parameters are generated for each CSIRS PDU.

	Inputs:
		PDU_idx (int): This index tells the CSIRS PDU for which CSIRS is generated
		 
	Output (to file):
		The start and length RB, scrambling ID, configuration and row number , density , power scaling

*/

void generate_csirs_config(int PDU_idx){
	int start_RB , num_RB , csirs_density , csirs_row , csirs_l0 , csirs_l1 , scrambling_ID , n_sf , csirs_beta;

	start_RB = Dl_ttiRequest.CSIRS_PDU[PDU_idx].BWPStart + Dl_ttiRequest.CSIRS_PDU[PDU_idx].StartRB;
	num_RB = Dl_ttiRequest.CSIRS_PDU[PDU_idx].NrOfRBs;
	csirs_density = Dl_ttiRequest.CSIRS_PDU[PDU_idx].FreqDensity;
	csirs_row = Dl_ttiRequest.CSIRS_PDU[PDU_idx].Row;
	csirs_l0 =  Dl_ttiRequest.CSIRS_PDU[PDU_idx].SymbL0;
	csirs_l1 =  Dl_ttiRequest.CSIRS_PDU[PDU_idx].SymbL1;
	scrambling_ID = Dl_ttiRequest.CSIRS_PDU[PDU_idx].ScrambId;
	n_sf = Dl_ttiRequest.Slot;
	csirs_beta = Dl_ttiRequest.CSIRS_PDU[PDU_idx].powerControlOffsetSS;

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<8) + (uint64_t)((uint64_t)csirs_density<<17) + (uint64_t)((uint64_t)csirs_row<<19) + (uint64_t)((uint64_t)csirs_l0<<24) + (uint64_t)((uint64_t)csirs_l1<<28);
	//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56) + (uint64_t)scrambling_ID + (uint64_t)((uint64_t)csirs_beta<<16);
	//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	//fprintf(fp, "PDU index : %d \n", PDU_idx);
	//fprintf(fp, "Starting RB is : %d \n", start_RB);
	//fprintf(fp, "PRB length is : %d \n", num_RB);
	//fprintf(fp, "CSIRS density is : %d \n", csirs_density);
	//fprintf(fp, "CSIRS row is : %d \n", csirs_row);
	//fprintf(fp, "l0 is : %d \n", csirs_l0);
	//fprintf(fp, "l1 is : %d \n", csirs_l1);
	//fprintf(fp, "DMRS scrambling ID is : %d \n", scrambling_ID);
	//fprintf(fp, "Beta offset is : 0X%04X \n", csirs_beta);

	// Update the reserved REs
	int G;
	for (int i = 0; i < Dl_tti_intermediate.nTB; ++i){
		int PDU_idx = Dl_tti_intermediate.TB_prop[i].user_id;
		int tb_start = Dl_ttiRequest.PDSCH_PDU[PDU_idx].BWPStart + Dl_ttiRequest.PDSCH_PDU[PDU_idx].rbStart;
		int tb_len = Dl_ttiRequest.PDSCH_PDU[PDU_idx].rbSize;

		int max_start_rb = (tb_start > start_RB)?tb_start:start_RB;
		int min_stop_rb = (tb_start + tb_len > start_RB + num_RB)?(start_RB+num_RB):(tb_start+tb_len);

		if (max_start_rb < min_stop_rb){
			G = (min_stop_rb - max_start_rb)*csirs_table[csirs_row - 1].numPorts*Dl_tti_intermediate.TB_prop[i].numLayers;
			Dl_tti_intermediate.TB_prop[i].reserved_RE += G;
			//fprintf(fp, "The reserved REs for TB %d by CSIRS is %d\n", i, G);
		}
	}
}





/*
Function: generate_golden_sequence_config
Author: Milind Kumar V

	Function to generate configurations for the golden sequence module. The parameters in the configuration 
	are used to generate the scrambling sequence c^q(i) specified in Section 7.3.1.1 in TS 38.211. Requries
	the availability of the Dl_ttiRequest and Dl_tti_intermediate variables.

	Input:
		PDSCH_pdu_index (int): index of the PDSCH PDU from which the Transport Block is obtained 
		TB_index (int): index of the Transport Block being processed	
		
	Output (to file):
		NRNTI (int): radio network temporary identifier
		q (int): index of the codeword- either 0 or 1 as a given PDSCH PDU can have 1 or 2 codewords
		N_id (int): the parameter n_ID used to create the scrambling sequence; specified in Section 7.3.1.1,
			TS 38.211

*/

void generate_golden_sequence_config(int PDSCH_pdu_index, int TB_index){
	int NRNTI;				// value of NRNTI
	int q;					// index of the codeword in a given PdSCH PDU, Section 7.3.1.1, TS 38.211
	int N_id;				// value of the parameter n_ID given in Section 7.3.1.1, TS 38.211

	// Determining the value of NRNTI
	NRNTI = (int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].RNTI;

	// Determining the value of q
	q = (int)Dl_tti_intermediate.TB_prop[TB_index].q;

	// Determining the value of N_id
	// This given by the value of dataScramblingIdentity (dataScramblingId) from the PDSCH PDU
	// TODO: incorporate the conditions regarding RNTIs
	N_id = (int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].dataScramblingId;

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_GOLD_SEQ<<56) + (uint64_t)NRNTI + (uint64_t)((uint64_t)q<<16) + (uint64_t)((uint64_t)N_id<<17);
	//fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write to a file
	// TODO: write to hardware instead of write to file
	//fprintf(fp, "NRNTI: %d \n", NRNTI);
	//fprintf(fp, "q: %d \n", q);
	//fprintf(fp, "N_id: %d \n", N_id);

}





/*
Function: generate_scrambler_config
Author: Milind Kumar V

	Function to generate configurations for the scrambler module. Requires the availability of the 
	Dl_tti_intermediate variable.

	Input:
		TB_index (int): index of the Transport Block being processed
		CB_index (int): index of the code block being processed, belongs to {0,1,..., num_CB-1}. Specific 
			to the TB being processed (given by TB_index) i.e a CB_index of i indicates the ith CB of the 
			TB corresponding to the TB given by TB_index.
		

	Output (to file):
		Userid (int): user id indicating the user to which the code block being processed belongs
		q (int): index of the codeword- either 0 or 1 as a given PDSCH PDU can have 1 or 2 codewords
		CB_size (int): number of bits in the code block after rate matching, i.e E from Section 5.4.2, 
			TS 38.212
		End_TB (int): a single bit value indicating if the code block being processed is the last of 
			those associated with the transport block

*/

void generate_scrambler_config(int TB_index, int CB_index){
	int Userid;					// value indicating the user to which the CB belongs
	int q; 						// index of the codeword in a given PdSCH PDU, Section 7.3.1.1, TS 38.211
	int CB_size;				// number of bits in the code block after rate matching
	int End_TB;					// 1 if CB being processed is the last of that TB, 0 otherwise

	// Obtaining the user id
	Userid = (int)Dl_tti_intermediate.TB_prop[TB_index].user_id;

	// Obtaining the value of q
	q = (int)Dl_tti_intermediate.TB_prop[TB_index].q;

	// Value of CB_size
	CB_size = (int)Dl_tti_intermediate.TB_prop[TB_index].E[CB_index];

	// Value of End_TB
	if (CB_index == (int)Dl_tti_intermediate.TB_prop[TB_index].num_CB - 1){
		End_TB = 1;
	} else {
		End_TB = 0;
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_SCRAMBLING<<56) + (uint64_t)Userid + (uint64_t)((uint64_t)q<<5) + (uint64_t)((uint64_t)CB_size<<6) + (uint64_t)((uint64_t)End_TB<<22);
	//fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write to file
	// TODO: Write to hardware
	//fprintf(fp, "Userid: %d \n", Userid);
	//fprintf(fp, "q: %d \n", q);
	//fprintf(fp, "CB_size: %d \n", CB_size);
	//fprintf(fp, "End_TB: %d \n", End_TB);

}





/*
Function: generate_modulator_config
Author: Milind Kumar V

	Function to generate configurations for the modulator module. Requries the presence of the global variable
	Dl_tti_intermediate. TB_index is a sufficient input as all code blocks associated with a given Transport 
	Block have the same modulation order and code block size.

	Input:
		TB_index (int): index of the Transport Block being processed
		CB_index (int): index of the code block being processed, belongs to {0,1,..., num_CB-1}. Specific 
			to the TB being processed (given by TB_index) i.e a CB_index of i indicates the ith CB of the 
			TB corresponding to the TB given by TB_index.

	Output (to file):
		Mod_Order (int): Modulation order- QPSK or 16, 64, 256 QAM. Obtained in the codewordinfo given for
			each codeword in Dl_ttiRequest.  
		No_of_packets (int): computed as CB_size where CB_size is the number of bits in the code block after 
			rate matching, i.e E from Section 5.4.2, TS 38.212

*/

void generate_modulator_config(int TB_index, int CB_index){
	int Mod_Order;			// Modulation order for constellation, specified for each TB
	int No_of_packets;		// The value E from Section 5.4.2, TS 38.212 i.e number of bits in the CB after rate matching

	Mod_Order = (int)Dl_tti_intermediate.TB_prop[TB_index].qamModOrder;
	Mod_Order = 1<<Mod_Order;
	No_of_packets = (int)Dl_tti_intermediate.TB_prop[TB_index].E[CB_index];

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_MODULATOR<<56) + (uint64_t)Mod_Order + (uint64_t)((uint64_t)No_of_packets<<9);
	//fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Writing to a file
	// TODO: replace with write to hardware
	//fprintf(fp, "Mod_Order: %d \n", Mod_Order);
	//fprintf(fp, "No_of_packets: %d \n", No_of_packets);

}





/*
Function: generate_layer_mapper_config
Author: Milind Kumar V

	Function to generate configurations for the layer mapper module. Requires the presence of the 
	Dl_ttiRequest and Dl_tti_intermediate variables. Code based on Section 7.3.1.3, TS 38.211. Final
	mapping from the aforementioned section to the DMRS ports is done using the dmrsPorts parameter
	in the PDSCH PDU. dmrsPorts is a 12 bit bitmap. However only the first 8 bits are relevant even 
	though the last four will still be stored in any code written.

	Eg. If num_layers_pdu = 2 and dmrsPorts = 000000001010, then
	layer0 = 1
	layer1 = 3
	layer2 = 1
	layer3 = 3

	Input:
		PDSCH_pdu_index (int): index of the PDSCH PDU from which the Transport Block is obtained 
		TB_index (int): index of the Transport Block being processed
		CB_index (int): index of the code block being processed, belongs to {0,1,..., num_CB-1}. Specific 
			to the TB being processed (given by TB_index) i.e a CB_index of i indicates the ith CB of the 
			TB corresponding to the TB given by TB_index.		

	Output (to file):
		layer0 (int): layer that the first bit of the code block is mapped to
		layer1 (int): layer that the second bit of the code block is mapped to
		layer2 (int): layer that the third bit of the code block is mapped to
		layer3 (int): layer that the fourth bit of the code block is mapped to
		mapping_type (int): number of layers used for the codeword transmission, belongs to {1,2,3,4}
		no_modulated_symbols (int): number of bits in the code block after rate matching, 

*/

void generate_layer_mapper_config(int PDSCH_pdu_index, int TB_index, int CB_index){
	int layer0;					// layer to which the 1st bit is mapped
	int layer1;					// layer to which the 2nd bit is mapped
	int layer2;					// layer to which the 3rd bit is mapped
	int layer3;					// layer to which the 4th bit is mapped
	int mapping_type;			// number of layers used for mapping bits of the CB
	int no_modulated_symbols;	// number of modulated symbols in the CB after rate matching, scrambling and modulation
	int codeword_idx;			// 0 or 1: the codeword, indexed from 0, to which the TB associated with TB_index corresponds
	int num_layers_pdu;			// total number of layers used in the PDSCH PDU 
	int available_ports[12];	// List of DMRS ports which are active, obtained from dmrsPorts, Table 3-38, FAPI document
	int port_val = 0;			// Value used to iterate through the ports, dummy variable
	int i = 0;					// dummy variable

	// Computing mapping_type
	mapping_type = (int)Dl_tti_intermediate.TB_prop[TB_index].numLayers - 1;

	// Computation of the values of layer0, layer1, layer2, layer3 happens from Table 7.3.1.3-1, TS
	// 38.211 and depends on the codeword index and number of layers being used.	
	num_layers_pdu = (int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].nrOfLayers;
	codeword_idx = (int)Dl_tti_intermediate.TB_prop[TB_index].q;

	// Determining active ports
	for (port_val = 0; port_val < 12; port_val++){
		if (((int)Dl_ttiRequest.PDSCH_PDU[PDSCH_pdu_index].dmrsPorts) & 1<<port_val){
			available_ports[i] = port_val;
			i++;
		}
	}

	if (num_layers_pdu == 1 ){
		layer0 = available_ports[0];
		layer1 = available_ports[0];
		layer2 = available_ports[0];
		layer3 = available_ports[0];
	} else if (num_layers_pdu == 2){
		layer0 = available_ports[0];
		layer1 = available_ports[1];
		layer2 = available_ports[0];
		layer3 = available_ports[1];
	} else if (num_layers_pdu == 3){
		layer0 = available_ports[0];
		layer1 = available_ports[1];
		layer2 = available_ports[2];
		layer3 = available_ports[0];
	} else if (num_layers_pdu == 4){
		layer0 = available_ports[0];
		layer1 = available_ports[1];
		layer2 = available_ports[2];
		layer3 = available_ports[3];
	} else if (num_layers_pdu == 5){
		if (codeword_idx == 0){
			layer0 = available_ports[0];
			layer1 = available_ports[1];
			layer2 = available_ports[0];
			layer3 = available_ports[1];
		} else {
			layer0 = available_ports[2];
			layer1 = available_ports[3];
			layer2 = available_ports[4];
			layer3 = available_ports[2];
		}
	} else if (num_layers_pdu == 6){
		if (codeword_idx == 0){
			layer0 = available_ports[0];
			layer1 = available_ports[1];
			layer2 = available_ports[2];
			layer3 = available_ports[0];
		} else {
			layer0 = available_ports[3];
			layer1 = available_ports[4];
			layer2 = available_ports[5];
			layer3 = available_ports[3];
		}
	} else if (num_layers_pdu == 7){
		if (codeword_idx == 0){
			layer0 = available_ports[0];
			layer1 = available_ports[1];
			layer2 = available_ports[2];
			layer3 = available_ports[0];
		} else {
			layer0 = available_ports[3];
			layer1 = available_ports[4];
			layer2 = available_ports[5];
			layer3 = available_ports[6];
		}
	} else if (num_layers_pdu == 8){
		if (codeword_idx == 0){
			layer0 = available_ports[0];
			layer1 = available_ports[1];
			layer2 = available_ports[2];
			layer3 = available_ports[3];
		} else {
			layer0 = available_ports[4];
			layer1 = available_ports[5];
			layer2 = available_ports[6];
			layer3 = available_ports[7];
		}
	}

	// Computing no_modulated_symbols
	// The number of modulated symbols is given as the number of bits in the code block after 
	// scrambling (same as E) divided by the number of bits per symbol for a given modulation 
	// order

	no_modulated_symbols = (int)Dl_tti_intermediate.TB_prop[TB_index].E[CB_index]/((int)Dl_tti_intermediate.TB_prop[TB_index].qamModOrder);

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_LAYER_MAPPER<<56) + (uint64_t)layer0 + (uint64_t)((uint64_t)layer1<<3) + (uint64_t)((uint64_t)layer2<<6) + (uint64_t)((uint64_t)layer3<<9) + (uint64_t)((uint64_t)mapping_type<<12) + (uint64_t)((uint64_t)no_modulated_symbols<<14);
	//fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write output to file
	// TODO: Write to hardware
	//fprintf(fp, "layer0: %d \n", layer0);
	//fprintf(fp, "layer1: %d \n", layer1);
	//fprintf(fp, "layer2: %d \n", layer2);
	//fprintf(fp, "layer3: %d \n", layer3);
	//fprintf(fp, "mapping_type: %d \n", mapping_type);
	//fprintf(fp, "no_modulated_symbols: %d \n", no_modulated_symbols);

}





/*
Function: generate_UEinterelaved_RE_mapper_config
Author: Aniruddh V

	Function to generate configurations for the user interleaver and RE mapper module. Requires the presence of the 
	Dl_ttiRequest and Dl_tti_intermediate variables. The 2 modules are grouped together as the configs required are mostly common.

	Input:
		None

	Output (to file):
		First output the configs for each codeblock in its order.
		Function outputs length of CB, the user it belongs to, the layers it occupies.
		Next output all the PDSCH (for each PDU) configs in increasing order of frequency allocation.
		Function outputs the frequency start and length, time locations, dmrs locations, layers occupied, user_id, number of modulation symbols
		Next output all the CSIRS (for each PDU) configs in increasing order of frequency and time allocation.
		Function outputs the frequency start and length, time locations, layers occupied
		Next output all the SSB (for each PDU) configs in increasing order of frequency and time allocation.
		Function outputs the frequency start, time locations

*/

void generate_UEinterleaved_RE_mapper_config(){

	int i;
	// CB configs
	//fprintf(fp, "\n==================Codeblock configs===========================\n");
	for (i = 0; i < Dl_tti_intermediate.nCB; ++i){
		int user_id, length_CB, layer_info;
		int idx = sorted_CB_idx[i];

		user_id = Dl_tti_intermediate.TB_prop[CB_TB_idx[idx]].user_id;
		length_CB = Dl_tti_intermediate.TB_prop[CB_TB_idx[idx]].E[CB_idx_in_TB[idx]]/(Dl_tti_intermediate.TB_prop[CB_TB_idx[idx]].numLayers*Dl_tti_intermediate.TB_prop[CB_TB_idx[idx]].qamModOrder);
		layer_info = Dl_ttiRequest.PDSCH_PDU[user_id].dmrsPorts;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER2<<56) + (uint64_t)user_id + (uint64_t)((uint64_t)length_CB<<6) + (uint64_t)((uint64_t)layer_info<<22);
		//fprintf(fp, "\nHardware packet for UE interleaver 2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "User ID is : %d \n", user_id);
		//fprintf(fp, "Length of modulation symbols in layer is : %d \n", length_CB);
		//fprintf(fp, "Layer Info is : 0X%02X \n", layer_info);
	}

	// PDSCH configs
	//fprintf(fp, "\n==================PDSCH configs===========================\n");
	for (i = 0; i < Dl_tti_intermediate.nPDSCH; ++i){
		PDSCH_PDU_t cur_pdsch = Dl_ttiRequest.PDSCH_PDU[sorted_DMRS_idx[i]];
		int start_RB, num_RB, pdsch_present, dmrs_present, dmrs_config_type, nLayers, user_id, max_length=0;

                if (cur_pdsch.refPoint == 1){
                        start_RB = coreset0_dmrs_ref/12 + cur_pdsch.rbStart;
                }
                else{

		        start_RB = cur_pdsch.BWPStart + cur_pdsch.rbStart;
                }
		num_RB = cur_pdsch.rbSize;
		dmrs_present = cur_pdsch.dlDmrsSymbPos;
		pdsch_present = 0;
		for (int j = 0; j < cur_pdsch.NrOfSymbols; ++j){
			pdsch_present += (int)(1<<j);
		}
		pdsch_present = (int)(pdsch_present<<cur_pdsch.StartSymbolIndex);
		if (cur_pdsch.numDmrsCdmGrpsNoData - cur_pdsch.dmrsConfigType == 2)
			pdsch_present = (int)(pdsch_present ^ dmrs_present);
		dmrs_config_type = cur_pdsch.dmrsConfigType;
		nLayers = cur_pdsch.dmrsPorts;
		user_id = sorted_DMRS_idx[i];
		for (int j = 0; j < Dl_tti_intermediate.nTB; ++j)
		{
			TB_prop_t cur_TB = Dl_tti_intermediate.TB_prop[j];
			if (cur_TB.user_id == user_id)
			{
				max_length = cur_TB.G_modulation/cur_TB.numLayers;
				break;
			}
		}

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)user_id + (uint64_t)((uint64_t)start_RB<<6) + (uint64_t)((uint64_t)num_RB<<16);
		//fprintf(fp, "\nHardware packet 1 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)max_length + (uint64_t)((uint64_t)pdsch_present<<16);
		//fprintf(fp, "Hardware packet 2 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)dmrs_present + (uint64_t)((uint64_t)dmrs_config_type<<14) + (uint64_t)((uint64_t)nLayers<<15);
		//fprintf(fp, "Hardware packet 3 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)pdsch_present<<18);
		//fprintf(fp, "Hardware packet 1 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)dmrs_present + (uint64_t)((uint64_t)dmrs_config_type<<14) + (uint64_t)((uint64_t)nLayers<<15);
		//fprintf(fp, "Hardware packet 2 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "Starting RB is : %d \n", start_RB);
		//fprintf(fp, "Length of RB is : %d \n", num_RB);
		//fprintf(fp, "Time locations are : 0X%04X \n", pdsch_present);
		//fprintf(fp, "DMRS locations are : 0X%04X \n", dmrs_present);
		//fprintf(fp, "DMRS configuration is : %d \n", dmrs_config_type);
		//fprintf(fp, "Layers are : 0X%04X \n", nLayers);
		//fprintf(fp, "User ID is : %d \n", user_id);
		//fprintf(fp, "Maximum number of modulation symbols is : %d \n", max_length);
	}
	for (int i = Dl_tti_intermediate.nPDSCH; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)i;
		//fprintf(fp, "\nHardware packet 1 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56);
		//fprintf(fp, "Hardware packet 2 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56);
		//fprintf(fp, "Hardware packet 3 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56);
		//fprintf(fp, "Hardware packet 1 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56);
		//fprintf(fp, "Hardware packet 2 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}
	//printf("Number of hardware packets after PDSCH REmapper : %d\n", HW_OP_idx);

	// CSIRS configs
	//fprintf(fp, "\n==================CSIRS configs===========================\n");
	for (i = 0; i < Dl_tti_intermediate.nCSIRS; ++i){
		CSIRS_PDU_t cur_csirs = Dl_ttiRequest.CSIRS_PDU[sorted_CSIRS_idx[i]];
		int row = cur_csirs.Row;
		int start_RB, num_RB, csirs_present, csirs_bitmap, layer_info;

		start_RB = cur_csirs.BWPStart + cur_csirs.StartRB;
		num_RB = cur_csirs.NrOfRBs;
		if (csirs_table[row].TDM == 1)
			csirs_present = (2<<csirs_table[row].num_L0-1)<<cur_csirs.SymbL0 + (2<<csirs_table[row].num_L1-1)<<cur_csirs.SymbL1;
		else
			csirs_present = (2<<csirs_table[row].TDM-1)<<cur_csirs.SymbL0 + ((2<<csirs_table[row].TDM-1)<<cur_csirs.SymbL1)*(csirs_table[row].num_L1 != 0);
		csirs_bitmap = cur_csirs.FreqDomain;
		layer_info = 2<<cur_csirs.CDMType-1;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)csirs_present<<18);
		//fprintf(fp, "\nHardware packet 1 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)csirs_bitmap + (uint64_t)((uint64_t)layer_info<<12);
		//fprintf(fp, "Hardware packet 2 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)csirs_present<<18);
		//fprintf(fp, "Hardware packet 1 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)csirs_bitmap + (uint64_t)((uint64_t)layer_info<<12);
		//fprintf(fp, "Hardware packet 2 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "Starting RB is : %d \n", start_RB);
		//fprintf(fp, "Length of RB is : %d \n", num_RB);
		//fprintf(fp, "Time locations are : 0X%04X \n", csirs_present);
		//fprintf(fp, "Frequency RB bitmap is : 0X%04X \n", csirs_bitmap);
		//fprintf(fp, "Layers are : 0X%04X \n", layer_info);
	}
	for (int i = Dl_tti_intermediate.nCSIRS; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56);
		//fprintf(fp, "\nHardware packet 1 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56);
		//fprintf(fp, "Hardware packet 2 for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56);
		//fprintf(fp, "Hardware packet 1 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56);
		//fprintf(fp, "Hardware packet 2 for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}
	//printf("Number of hardware packets after CSIRS REmapper : %d\n", HW_OP_idx);

	// PDCCH config
	//fprintf(fp, "\n==================PDCCH configs===========================\n");
	for (int i = 0; i < Dl_tti_intermediate.nPDCCH; ++i){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)cur_pdcch.BWPStart + (uint64_t)((uint64_t)cur_pdcch.BWPSize<<9) + (uint64_t)((uint64_t)pdcch_intermediate.pdcch_present[i]<<18);
		//fprintf(fp, "Hardware packet 1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		//fprintf(fp, "User Number is %d\n", i);
		//fprintf(fp, "start_RB_user is : %d \n", cur_pdcch.BWPStart);
		//fprintf(fp, "num_RB_user is : %d \n", cur_pdcch.BWPSize);
		//fprintf(fp, "pdcch_present is : %d \n\n", pdcch_intermediate.pdcch_present[i]);

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)1;  // Layer is always 1 for PDCCH.
		//fprintf(fp, "Hardware packet 2 is : 0X%lX\n\n", HW_OP[HW_OP_idx-1]);
		//fprintf(fp, "Layer is %d\n\n", 1);
	}
	for (int i = Dl_tti_intermediate.nPDCCH; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56);
		//fprintf(fp, "\nHardware packet is 1 : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56);
		//fprintf(fp, "\nHardware packet is 2 : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}

	// SSB config
	//fprintf(fp, "\n==================SSB configs===========================\n");
	// Based on Case C from Section 4.1, TS 38.213 for fc = 3.5 GHz. For this case,
	// there can be 8 SSB PDUs in 5ms which is 10 slots at SCS = 30kHz. Consequently,
	// the first 4 slots can have at most 2 SSB transmissions (PDUs) each. The 
	// subsequent 6 can not have any SSB. This condition of only the first four being 
	// allowed to possess SSB and the latter 6 not is handled during the config 
	// generation done by the GUI and is not handled in the L1C code. The below code
	// simply acts slotwise and is agnostic to the slot number.

	int start_RB = ConfigRequest.SSBTable.SsbOffsetPointA.Value/2; // This is for SSB SCS = 15 kHz and SCS = 30 kHz
	int SSB_present = 0; // If no SSB PDU is present SSB_present is set to zero
	if (Dl_tti_intermediate.nSSB & 0b1)
		SSB_present += (int)(15<<2);
	if (Dl_tti_intermediate.nSSB & 0b10)
		SSB_present += (int)(15<<8);

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_UE_INTERLEAVER1<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)SSB_present<<9);
	//fprintf(fp, "\nHardware packet for UE interleaver is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_RE_MAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)SSB_present<<9);
	//fprintf(fp, "Hardware packet for RE mapper is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	//fprintf(fp, "Starting RB is : %d \n", start_RB);
	//fprintf(fp, "Time locations are : 0X%04X \n", SSB_present);

}





/*
Function: generate_pbch_config
Author: Aniruddh V

	Function to generate configurations for the SSB generation.
	Requires the presence of the Dl_ttiRequest and Dl_tti_intermediate variables.

	Input:
		None

	Output (to file):
		MIB bits, SSB index, frame number, half-frame number, Cell ID, subcarrier offset

*/

void generate_pbch_config(){

	int index, SFN, HFN, Kssb, NID1, NID2, NID;

	SFN = Dl_ttiRequest.SFN;
	HFN = (int)(Dl_ttiRequest.Slot > 9);
	Kssb = ((ConfigRequest.SSBTable.SsbSubcarrierOffset.Value)&0x10)>>4;
	NID = ConfigRequest.CellConfig.phyCellId.Value;
	NID1 = NID / 3;
	NID2 = NID % 3;

	uint32_t MIB;// = (uint32_t)(ConfigRequest.SSBTable.MIB.Value & 0x00FFFFFF);

	//MIB = 0x10694;
	MIB = ((uint32_t)(ConfigRequest.SSBTable.MIB.Value & 0x81FFFF)) + ((SFN&0x000003f0)<<13);
	//MIB = ((0x10694)&(0x81FFFF)) + ((SFN&0x000003f0)<<13);
	//xil_//printf("Frame number : %x \n", SFN);
	//xil_//printf("Frame number is : %x \n", (SFN&0x000003f0)<<13);
	if (Dl_tti_intermediate.nSSB & 0b1){
		index = 0 + 2*(Dl_ttiRequest.Slot%10);

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PBCH<<56) + (uint64_t)MIB + (uint64_t)((uint64_t)index<<24);
		//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PBCH<<56) + (uint64_t)SFN + (uint64_t)((uint64_t)HFN<<10) + (uint64_t)((uint64_t)Kssb<<11) + (uint64_t)((uint64_t)NID1<<12) + (uint64_t)((uint64_t)NID2<<21);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PBCH<<56) + (uint64_t)NID;
		//fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "MIB bits are : 0X%04X \n", MIB);
		//fprintf(fp, "SSB index is : %d \n", index);
		//fprintf(fp, "Frame number is : %d \n", SFN);
		//fprintf(fp, "Half frame number is : %d \n", HFN);
		//fprintf(fp, "Subcarrier offset Kssb is : %d \n", Kssb);
		//fprintf(fp, "Cell NID is : 0X%04X \n", NID);
		//fprintf(fp, "Cell NID1 is : 0X%04X \n", NID1);
		//fprintf(fp, "Cell NID2 is : 0X%04X \n", NID2);
	}
	if (Dl_tti_intermediate.nSSB & 0b10){
		index = 1 + 2*(Dl_ttiRequest.Slot%10);

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PBCH<<56) + (uint64_t)MIB + (uint64_t)((uint64_t)index<<24);
		//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PBCH<<56) + (uint64_t)SFN + (uint64_t)((uint64_t)HFN<<10) + (uint64_t)((uint64_t)Kssb<<11) + (uint64_t)((uint64_t)NID1<<12) + (uint64_t)((uint64_t)NID2<<21);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PBCH<<56) + (uint64_t)NID;
		//fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		//fprintf(fp, "MIB bits are : 0X%04X \n", MIB);
		//fprintf(fp, "SSB index is : %d \n", index);
		//fprintf(fp, "Frame number is : %d \n", SFN);
		//fprintf(fp, "Half frame number is : %d \n", HFN);
		//fprintf(fp, "Subcarrier offset Kssb is : %d \n", Kssb);
		//fprintf(fp, "Cell NID is : 0X%04X \n", NID);
		//fprintf(fp, "Cell NID1 is : 0X%04X \n", NID1);
		//fprintf(fp, "Cell NID2 is : 0X%04X \n", NID2);
	}
}





/*
Function: generate_ORAN_config
Author: Aniruddh V

	Function to generate configurations for ORAN TX C-plane and U-plane.
	One section for each MU-MIMO user group(PDSCH, DMRS, PTRS), one section for SSB
	(TBD later) Separate section for USS-PDCCH, CSIRS, CSS-PDCCH
	Requires the presence of the Dl_ttiRequest and Dl_tti_intermediate variables.

	Input:
		None

	Output (to file):
	C-plane requires application header for each section header.
	U-plane requires application header for each OFDM symbol.
	Configs are repeated AS-IS for every layer (8-times)
	Section header, Application header, Section extension header (possibly eCPRI and ethernet headers)

*/

void generate_ORAN_config(){

	int Application_SectionType = 1;
	int dataDir = 1;				// DL
	int payloadVer = 1;
	int filterIndex = 0;
	int FrameID = Dl_ttiRequest.SFN%256;
	int subframeID = Dl_ttiRequest.Slot/2;
	int slotID = Dl_ttiRequest.Slot%2;

	int numSections = 1;
	int udCompressionHeader = 0;

	int Section_SectionType = 1;
	int rb = 0;
	int symInc = 0;

	int extensionType = 11;
	int ef = 1;
	int extLen = 1;
	int disableBFW = 0;
	int RAD = 0;
	int BFWcompressionHeader = 0;
	int BFWcompressionParameter = 0;

	uint64_t Application_Header_packet1 = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<54) + (uint64_t)((uint64_t)1<<52) + (uint64_t)((uint64_t)Application_SectionType<<44) + (uint64_t)((uint64_t)0<<43) + (uint64_t)((uint64_t)dataDir<<42) + (uint64_t)((uint64_t)payloadVer<<39) + (uint64_t)((uint64_t)filterIndex<<35) + (uint64_t)((uint64_t)FrameID<<27) + (uint64_t)((uint64_t)subframeID<<23) + (uint64_t)((uint64_t)slotID<<17) + (uint64_t)((uint64_t)numSections<<5);
	uint64_t Application_Header_packet2 = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<54) + (uint64_t)((uint64_t)1<<52) + (uint64_t)((uint64_t)Application_SectionType<<44) + (uint64_t)((uint64_t)1<<43) + (uint64_t)((uint64_t)udCompressionHeader<<35);
	uint64_t Section_Header_packet1 = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<54) + (uint64_t)((uint64_t)2<<52) + (uint64_t)((uint64_t)Section_SectionType<<44) + (uint64_t)((uint64_t)0<<43) + (uint64_t)((uint64_t)rb<<30) + (uint64_t)((uint64_t)symInc<<29);
	uint64_t Section_Header_packet2 = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<54) + (uint64_t)((uint64_t)2<<52) + (uint64_t)((uint64_t)Section_SectionType<<44) + (uint64_t)((uint64_t)1<<43) + (uint64_t)((uint64_t)ef<<23);
	uint64_t SectionExtension_Header_packet1 = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<54) + (uint64_t)((uint64_t)3<<52) + (uint64_t)((uint64_t)extensionType<<44) + (uint64_t)((uint64_t)0<<43) + (uint64_t)((uint64_t)ef<<42) + (uint64_t)((uint64_t)extLen<<26) + (uint64_t)((uint64_t)disableBFW<<25) + (uint64_t)((uint64_t)RAD<<24) + (uint64_t)((uint64_t)BFWcompressionHeader<<8);
	uint64_t SectionExtension_Header_packet2 = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<54) + (uint64_t)((uint64_t)3<<52) + (uint64_t)((uint64_t)extensionType<<44) + (uint64_t)((uint64_t)1<<43) + (uint64_t)((uint64_t)BFWcompressionParameter<<35);
	int Mux_Config_Address = HW_OP_idx;
	HW_OP_idx += 2;

	// C-plane configs
	//fprintf(fp, "=====================================================C-plane configs=====================================================================\n");

	int n_ORAN_PDSCH_PDU = 0;
	int start_loc_PDSCH = 14*MAX_PRB_BW;
	PDSCH_PDU_t cur_pdsch;
	if (Dl_tti_intermediate.nPDSCH > 0){
		cur_pdsch = Dl_ttiRequest.PDSCH_PDU[sorted_PDSCH_idx[0]];
		start_loc_PDSCH = MAX_PRB_BW*cur_pdsch.StartSymbolIndex + cur_pdsch.BWPStart + cur_pdsch.rbStart;
	}
	int n_ORAN_PDCCH_PDU = 0;
	int start_loc_PDCCH = 14*MAX_PRB_BW;
	PDCCH_PDU_t cur_pdcch;
	if (Dl_tti_intermediate.nPDCCH > 0){
		cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[0]];
		start_loc_PDCCH = MAX_PRB_BW*cur_pdcch.StartSymbolIndex + cur_pdcch.BWPStart;
	}
	int n_ORAN_SSB_PDU = 0;
	int start_loc_SSB = 14*MAX_PRB_BW;
	SSB_PDU_t cur_SSB;
	if (Dl_tti_intermediate.nSSB > 0){
		start_loc_SSB = ConfigRequest.SSBTable.SsbOffsetPointA.Value/2;			// MAX_PRB_BW*((cur_SSB.ssbBlockIndex%2==0)?2:8) +
	}

	int n_ORAN_section = 0;
	int start_prb = 0;
	while (start_loc_PDSCH < 14*MAX_PRB_BW || start_loc_SSB < 14*MAX_PRB_BW || start_loc_PDCCH < 14*MAX_PRB_BW){
		
		// Sections for PDSCH
		if (start_loc_PDSCH < start_loc_SSB && start_loc_PDSCH < start_loc_PDCCH){
			int startSymbolID = cur_pdsch.StartSymbolIndex;
			int sectionID = n_ORAN_section;
			int start_RB = cur_pdsch.BWPStart + cur_pdsch.rbStart;
			int num_RB = cur_pdsch.rbSize;
			int reMask = (int)0b111111111111;
			int numSym = cur_pdsch.NrOfSymbols;
			int numBundlePRB = 4;										// cur_pdsch.TxPrecoding_Beamforming.numPRGs;

			int num_MUMIMO_UE = 1;
			TxPrecoding_Beamforming_t cur_TxPrecoding_Beamforming[8];
			int dmrsPorts[8];
			for (int i = 0; i < 8; ++i){
				if (n_ORAN_PDSCH_PDU+i >= Dl_tti_intermediate.nPDSCH){
					num_MUMIMO_UE = i;
					break;
				}
				PDSCH_PDU_t cur_pdsch = Dl_ttiRequest.PDSCH_PDU[sorted_PDSCH_idx[n_ORAN_PDSCH_PDU+i]];
				if (start_RB != cur_pdsch.BWPStart + cur_pdsch.rbStart || numSym != cur_pdsch.NrOfSymbols){
					num_MUMIMO_UE = i;
					break;
				}
				cur_TxPrecoding_Beamforming[i] = Dl_ttiRequest.PDSCH_PDU[sorted_PDSCH_idx[n_ORAN_PDSCH_PDU+i]].TxPrecoding_Beamforming;
				dmrsPorts[i] = Dl_ttiRequest.PDSCH_PDU[sorted_PDSCH_idx[n_ORAN_PDSCH_PDU+i]].dmrsPorts;
			}

			for (int i = 0; i < num_RB; i+=20){
				ORAN_sections[n_ORAN_section].start_freq = start_RB + i;
				ORAN_sections[n_ORAN_section].num_freq = (num_RB>=i+20)?20:(num_RB-i);
				ORAN_sections[n_ORAN_section].start_symb = startSymbolID;
				ORAN_sections[n_ORAN_section].num_symb = numSym;
				ORAN_sections[n_ORAN_section].reMask = reMask;
				ORAN_sections[n_ORAN_section].PDU_type = 0;
				n_ORAN_section++;
				int num_beams = ceil(ORAN_sections[n_ORAN_section-1].num_freq/4.0);

				for (int j = 0; j < num_MUMIMO_UE; ++j){

					int available_ports[12];
					int num_ports = 0;

					// Determining active ports
					for (int port_val = 0; port_val < 12; port_val++){
						if (dmrsPorts[j] & 1<<port_val){
							available_ports[num_ports++] = port_val;
						}
					}

					for (int k = 0; k < num_ports; ++k){
						// Application header
						HW_OP[HW_OP_idx++] = Application_Header_packet1 + (uint64_t)((uint64_t)startSymbolID<<13);
						//fprintf(fp, "\nHardware packet 1 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						HW_OP[HW_OP_idx++] = Application_Header_packet2;
						//fprintf(fp, "Hardware packet 2 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						//fprintf(fp, "Section type is : %d \n", Application_SectionType);
						//fprintf(fp, "Data Direction is : %d \n", dataDir);
						//fprintf(fp, "Frame number is : %d \n", FrameID);
						//fprintf(fp, "Subframe number is : %d \n", subframeID);
						//fprintf(fp, "Slot number is : %d \n", slotID);
						//fprintf(fp, "Starting symbol is : %d \n", startSymbolID);
						//fprintf(fp, "Number of sections are : %d \n", numSections);
						//fprintf(fp, "Compression header is : %d \n", udCompressionHeader);

						// Section header
						int beamID = 1;
						int RTCid = (int)((1+available_ports[k])<<12);

						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)sectionID<<31) + (uint64_t)((uint64_t)start_RB<<21) + (uint64_t)((uint64_t)ORAN_sections[n_ORAN_section-1].num_freq<<13) + (uint64_t)((uint64_t)reMask<<1);
						//fprintf(fp, "\nHardware packet 1 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						HW_OP[HW_OP_idx++] = Section_Header_packet2 + (uint64_t)((uint64_t)numSym<<39) + (uint64_t)((uint64_t)beamID<<24) + (uint64_t)((uint64_t)RTCid<<7);
						//fprintf(fp, "Hardware packet 2 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						//fprintf(fp, "Section type is : %d \n", Section_SectionType);
						//fprintf(fp, "Section ID is : %d \n", sectionID);
						//fprintf(fp, "RB is : %d \n", rb);
						//fprintf(fp, "Symbol Increment is : %d \n", symInc);
						//fprintf(fp, "Start PRB is : %d \n", start_RB);
						//fprintf(fp, "Number of PRB are : %d \n", ORAN_sections[n_ORAN_section-1].num_freq);
						//fprintf(fp, "RE mask is : 0X%04X \n", reMask);
						//fprintf(fp, "Number of symbols are : %d \n", numSym);
						//fprintf(fp, "Beam ID is : %d \n", beamID);
						//fprintf(fp, "RTC ID is : 0X%x \n", RTCid);

						// Section Extension header
						for (int l = 0; l < num_beams; ++l){
							beamID = cur_TxPrecoding_Beamforming[j].NumOfPRGs[l+i/4].beamIdx[k];
							HW_OP[HW_OP_idx++] = SectionExtension_Header_packet1 + (uint64_t)((uint64_t)numBundlePRB<<16);
							//fprintf(fp, "\nHardware packet %d of Section Extension Header is : 0X%lX\n", 2*l+1, HW_OP[HW_OP_idx-1]);
							HW_OP[HW_OP_idx++] = SectionExtension_Header_packet2 + (uint64_t)((uint64_t)beamID<<20);
							//fprintf(fp, "Hardware packet %d of Section Extension Header is : 0X%lX\n", 2*l+2, HW_OP[HW_OP_idx-1]);
						}

						//fprintf(fp, "Extension type is : %d \n", extensionType);
						//fprintf(fp, "ef is : %d \n", ef);
						//fprintf(fp, "extLen is : %d \n", extLen);
						//fprintf(fp, "disableBFW is : %d \n", disableBFW);
						//fprintf(fp, "RAD is : %d \n", RAD);
						//fprintf(fp, "Number of PRBs in a bundle are : %d \n", numBundlePRB);
						//fprintf(fp, "BFW compression header is : 0X%04X \n", BFWcompressionHeader);
						//fprintf(fp, "BFW CompParam is : %d \n", BFWcompressionParameter);
						for (int l = 0; l < num_beams; ++l){
							beamID = cur_TxPrecoding_Beamforming[j].NumOfPRGs[l+i/4].beamIdx[k];
							//fprintf(fp, "Beam%d ID is : 0X%04X \n", 1+l, beamID);
						}
					}
				}
			}

			n_ORAN_PDSCH_PDU += num_MUMIMO_UE;
			start_prb += cur_pdsch.rbSize;
			if (n_ORAN_PDSCH_PDU == Dl_tti_intermediate.nPDSCH){
				start_loc_PDSCH = 14*MAX_PRB_BW;
			}
			else{
				cur_pdsch = Dl_ttiRequest.PDSCH_PDU[sorted_PDSCH_idx[n_ORAN_PDSCH_PDU]];
				start_loc_PDSCH = MAX_PRB_BW*cur_pdsch.StartSymbolIndex + cur_pdsch.BWPStart + cur_pdsch.rbStart;
			}
		}

		// Sections for PDCCH
		else if (start_loc_PDCCH < start_loc_SSB && start_loc_PDCCH < start_loc_PDSCH){
			/* DUMMY code */
			int reMask = (int)0b111111111111;
			ORAN_sections[n_ORAN_section].start_freq = cur_pdcch.BWPStart;
			ORAN_sections[n_ORAN_section].num_freq = 4;
			ORAN_sections[n_ORAN_section].start_symb = cur_pdcch.StartSymbolIndex;
			ORAN_sections[n_ORAN_section].num_symb = 1;
			ORAN_sections[n_ORAN_section].reMask = reMask;
			ORAN_sections[n_ORAN_section].PDU_type = 1;

			n_ORAN_section++;
			n_ORAN_PDCCH_PDU++;

			// Application header
			HW_OP[HW_OP_idx++] = Application_Header_packet1 + (uint64_t)((uint64_t)cur_pdcch.StartSymbolIndex<<13);
			//fprintf(fp, "\nHardware packet 1 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = Application_Header_packet2;
			//fprintf(fp, "Hardware packet 2 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			// Section header
			HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_section<<31) + (uint64_t)((uint64_t)cur_pdcch.BWPStart<<21) + (uint64_t)((uint64_t)4<<13) + (uint64_t)((uint64_t)reMask<<1);
			//fprintf(fp, "\nHardware packet 1 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = Section_Header_packet2 + (uint64_t)((uint64_t)1<<39) + (uint64_t)((uint64_t)1<<24) + (uint64_t)((uint64_t)1024<<7);
			//fprintf(fp, "Hardware packet 2 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			// Section Extension header
			HW_OP[HW_OP_idx++] = SectionExtension_Header_packet1 + (uint64_t)((uint64_t)4<<16);
			//fprintf(fp, "\nHardware packet1 of Section Extension Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = SectionExtension_Header_packet2 + (uint64_t)((uint64_t)1<<20);
			//fprintf(fp, "Hardware packet2 of Section Extension Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			start_prb += cur_pdcch.BWPSize;
			if (n_ORAN_PDCCH_PDU == Dl_tti_intermediate.nPDCCH){
				start_loc_PDCCH = 14*MAX_PRB_BW;
			}
			else{
				cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[n_ORAN_PDCCH_PDU]];
				start_loc_PDCCH = MAX_PRB_BW*cur_pdcch.StartSymbolIndex + cur_pdcch.BWPStart;
			}
		}

		//Sections for SSB
		else if (start_loc_SSB < start_loc_PDSCH && start_loc_SSB < start_loc_PDCCH){

			int startSymbolID = (cur_SSB.ssbBlockIndex%2==0)?2:8;
			int sectionID = n_ORAN_section;
			int start_RB = start_loc_SSB;
			int num_RB = 20;
			int reMask = (int)0b111111111111;
			int numSym = 4;
			int numBundlePRB = 4;							// cur_SSB.TxPrecoding_Beamforming.numPRGs;
			int beamID = 1;
			int RTCid = 0x1000;

			ORAN_sections[n_ORAN_section].start_freq = start_RB;
			ORAN_sections[n_ORAN_section].num_freq = num_RB;
			ORAN_sections[n_ORAN_section].start_symb = startSymbolID;
			ORAN_sections[n_ORAN_section].num_symb = numSym;
			ORAN_sections[n_ORAN_section].reMask = reMask;
			ORAN_sections[n_ORAN_section].PDU_type = 2;
			n_ORAN_section++;

			// Application header
			HW_OP[HW_OP_idx++] = Application_Header_packet1 + (uint64_t)((uint64_t)startSymbolID<<13);
			//fprintf(fp, "\nHardware packet 1 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = Application_Header_packet2;
			//fprintf(fp, "Hardware packet 2 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			//fprintf(fp, "PDU type is : SSB \n");
			//fprintf(fp, "Section type is : %d \n", Application_SectionType);
			//fprintf(fp, "Data Direction is : %d \n", dataDir);
			//fprintf(fp, "Frame number is : %d \n", FrameID);
			//fprintf(fp, "Subframe number is : %d \n", subframeID);
			//fprintf(fp, "Slot number is : %d \n", slotID);
			//fprintf(fp, "Starting symbol is : %d \n", startSymbolID);
			//fprintf(fp, "Number of sections are : %d \n", numSections);
			//fprintf(fp, "Compression header is : %d \n", udCompressionHeader);

			// Section header
			HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)sectionID<<31) + (uint64_t)((uint64_t)start_RB<<21) + (uint64_t)((uint64_t)num_RB<<13) + (uint64_t)((uint64_t)reMask<<1);
			//fprintf(fp, "\nHardware packet 1 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = Section_Header_packet2 + (uint64_t)((uint64_t)numSym<<39) + (uint64_t)((uint64_t)beamID<<24) + (uint64_t)((uint64_t)RTCid<<7);
			//fprintf(fp, "Hardware packet 2 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			//fprintf(fp, "Section type is : %d \n", Section_SectionType);
			//fprintf(fp, "Section ID is : %d \n", sectionID);
			//fprintf(fp, "RB is : %d \n", rb);
			//fprintf(fp, "Symbol Increment is : %d \n", symInc);
			//fprintf(fp, "Start PRB is : %d \n", start_RB);
			//fprintf(fp, "Number of PRB are : %d \n", num_RB);
			//fprintf(fp, "RE mask is : 0X%04X \n", reMask);
			//fprintf(fp, "Number of symbols are : %d \n", numSym);
			//fprintf(fp, "Beam ID is : %d \n", beamID);
			//fprintf(fp, "RTC ID is : %d \n", RTCid);

			// Section Extension header
			for (int i = 0; i < 5; ++i){
				beamID = cur_SSB.TxPrecoding_Beamforming.NumOfPRGs[i].beamIdx[0];
				HW_OP[HW_OP_idx++] = SectionExtension_Header_packet1 + (uint64_t)((uint64_t)numBundlePRB<<16);
				//fprintf(fp, "\nHardware packet %d of Section Extension Header is : 0X%lX\n", 2*i+1, HW_OP[HW_OP_idx-1]);
				HW_OP[HW_OP_idx++] = SectionExtension_Header_packet2 + (uint64_t)((uint64_t)beamID<<20);
				//fprintf(fp, "Hardware packet %d of Section Extension Header is : 0X%lX\n", 2*i+2, HW_OP[HW_OP_idx-1]);
			}

			//fprintf(fp, "Extension type is : %d \n", extensionType);
			//fprintf(fp, "ef is : %d \n", ef);
			//fprintf(fp, "extLen is : %d \n", extLen);
			//fprintf(fp, "disableBFW is : %d \n", disableBFW);
			//fprintf(fp, "RAD is : %d \n", RAD);
			//fprintf(fp, "Number of PRBs in a bundle are : %d \n", numBundlePRB);
			//fprintf(fp, "BFW compression header is : 0X%04X \n", BFWcompressionHeader);
			//fprintf(fp, "BFW CompParam is : %d \n", BFWcompressionParameter);
			for (int i = 0; i < 5; ++i){
				beamID = cur_SSB.TxPrecoding_Beamforming.NumOfPRGs[i].beamIdx[0];
				//fprintf(fp, "Beam%d ID is : 0X%04X \n", 1+i, beamID);
			}

			n_ORAN_SSB_PDU++;
			start_prb += 20;
			start_loc_SSB = 14*MAX_PRB_BW;				// ASSUME ONLY ONE SECTION FOR THE SSB RESOURCE ACROSS ALL SYMBOLS IN THE SLOT
		}

		// Invalid configs if starting point is SAME
		else{
			//printf("Error in ORAN config\n");
			exit(1);
		}
	}

	// Dummy config in case of an empty slot
	if (n_ORAN_section == 0){

		ORAN_sections[n_ORAN_section].start_freq = 50;
		ORAN_sections[n_ORAN_section].num_freq = 4;
		int num_freq_dummy = 4; // for empty slot.
		ORAN_sections[n_ORAN_section].start_symb = 2;
		ORAN_sections[n_ORAN_section].num_symb = 1;
		ORAN_sections[n_ORAN_section].reMask = 1;
		ORAN_sections[n_ORAN_section].PDU_type = 15;
		n_ORAN_section++;

		// Application header
		HW_OP[HW_OP_idx++] = Application_Header_packet1 + (uint64_t)((uint64_t)2<<13);
		//fprintf(fp, "\nHardware packet 1 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = Application_Header_packet2;
		//fprintf(fp, "Hardware packet 2 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		// Section header
		HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_section<<31) + (uint64_t)((uint64_t)50<<21) + (uint64_t)((uint64_t)num_freq_dummy<<13) + (uint64_t)((uint64_t)1<<1);
		//fprintf(fp, "\nHardware packet 1 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = Section_Header_packet2 + (uint64_t)((uint64_t)1<<39) + (uint64_t)((uint64_t)1<<24) + (uint64_t)((uint64_t)1024<<7);
		//fprintf(fp, "Hardware packet 2 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		// Section Extension header
		HW_OP[HW_OP_idx++] = SectionExtension_Header_packet1 + (uint64_t)((uint64_t)4<<16);
		//fprintf(fp, "\nHardware packet1 of Section Extension Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = SectionExtension_Header_packet2 + (uint64_t)((uint64_t)1<<20);
		//fprintf(fp, "Hardware packet2 of Section Extension Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}

	// Mux config packets
	HW_OP[Mux_Config_Address] = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<52) + (uint64_t)((uint64_t)0<<51) + (uint64_t)((uint64_t)n_ORAN_section<<39) + (uint64_t)((uint64_t)n_ORAN_section<<27) + (uint64_t)((uint64_t)n_ORAN_section<<15) + (uint64_t)((uint64_t)n_ORAN_section<<3);
	//fprintf(fp, "\nHardware packet1 of Mux config is : 0X%lX\n", HW_OP[Mux_Config_Address]);
	HW_OP[Mux_Config_Address+1] = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<52) + (uint64_t)((uint64_t)1<<51) + (uint64_t)((uint64_t)n_ORAN_section<<39) + (uint64_t)((uint64_t)n_ORAN_section<<27) + (uint64_t)((uint64_t)n_ORAN_section<<15) + (uint64_t)((uint64_t)n_ORAN_section<<3);
	//fprintf(fp, "Hardware packet2 of Mux config is : 0X%lX\n", HW_OP[Mux_Config_Address+1]);

	for (int i = 0; i < MAX_ORAN_DL_LAYERS; ++i){
		//fprintf(fp, "Number of sections in layer %d is %d \n", 1+i, n_ORAN_section);
	}

	// U-plane configs
	//fprintf(fp, "=============================================================U-plane configs=============================================================================\n");

	Application_Header_packet1 = (uint64_t)((uint64_t)ADDRESS_ORAN_U_PLANE<<56) + (uint64_t)((uint64_t)0<<52) + (uint64_t)((uint64_t)dataDir<<51) + (uint64_t)((uint64_t)payloadVer<<48) + (uint64_t)((uint64_t)filterIndex<<44) + (uint64_t)((uint64_t)FrameID<<36) + (uint64_t)((uint64_t)subframeID<<32) + (uint64_t)((uint64_t)slotID<<26);
	Section_Header_packet1 = (uint64_t)((uint64_t)ADDRESS_ORAN_U_PLANE<<56) + (uint64_t)((uint64_t)1<<52) + (uint64_t)((uint64_t)rb<<39) + (uint64_t)((uint64_t)symInc<<38) + (uint64_t)((uint64_t)udCompressionHeader<<12);

	// Sort the sections according to frequency occasion
	int sorted_section_idx[Max_ORAN_sections];
	int PDU_BWP_start[Max_ORAN_sections], cur_index, next_index, nPDU;
	nPDU = n_ORAN_section;
	for (int i = 0; i < nPDU; ++i){
		sorted_section_idx[i] = i;
		PDU_BWP_start[i] = ORAN_sections[i].start_freq;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_section_idx[j];
			next_index = sorted_section_idx[j+1];
			if (PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_section_idx[j] = next_index;
				sorted_section_idx[j+1] = cur_index;
			}
		}
	}

	for (int i = 0; i < 14; ++i){
		for (int l = 0; l < MAX_ORAN_DL_LAYERS; ++l){
			int startSymbolID = i+1;
			int n_ORAN_empty_section = 4095;
			int start_prb = 0;
			int num_prb = MAX_PRB_BW;
			int sectionID = 0;

			// Application Header
			HW_OP[HW_OP_idx++] = Application_Header_packet1 + (uint64_t)((uint64_t)startSymbolID<<20);
			//fprintf(fp, "\nHardware packet of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			//fprintf(fp, "Data direction is : %d \n", dataDir);
			//fprintf(fp, "Payload version is : %d \n", payloadVer);
			//fprintf(fp, "Filter index is : %d \n", filterIndex);
			//fprintf(fp, "Frame ID is : %d \n", FrameID);
			//fprintf(fp, "Subframe ID is : %d \n", subframeID);
			//fprintf(fp, "Slot ID is : %d \n", slotID);
			//fprintf(fp, "Start symbol is : %d \n", startSymbolID);
			//fprintf(fp, "Layer id is %d\n", 1+l);

			// Section header hardcoded to only two sections (255 and 18 PRBs)
			int num_prb_section1 = 255, num_prb_section2 = 18, start_prb_section1 = 0,start_prb_section2= 255;
			HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)(start_prb_section1)<<28) + (uint64_t)((uint64_t)(num_prb_section1)<<20);
			//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			--n_ORAN_empty_section;
			//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
			//fprintf(fp, "rb is : %d \n", num_prb_section1);
			//fprintf(fp, "symInc is : %d \n", symInc);
			//fprintf(fp, "Start prb is : %d \n", start_prb_section1);
			//fprintf(fp, "Number of prb are : %d \n", num_prb_section1);
			//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);

			HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)(start_prb_section2)<<28) + (uint64_t)((uint64_t)(num_prb_section2)<<20);
			//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
			//fprintf(fp, "rb is : %d \n", num_prb_section2);
			//fprintf(fp, "symInc is : %d \n", symInc);
			//fprintf(fp, "Start prb is : %d \n", start_prb_section2);
			//fprintf(fp, "Number of prb are : %d \n", num_prb_section2);
			//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
			--n_ORAN_empty_section;

			// Section header modification ended

			// Section Header
/*			for (int k = 0; k < n_ORAN_section; ++k){
				int j = sorted_section_idx[k];
				if ((i - ORAN_sections[j].start_symb) < 0 || (i - ORAN_sections[j].start_symb) >= ORAN_sections[j].num_symb){
					continue;
				}

				if (start_prb != ORAN_sections[j].start_freq){
					if (ORAN_sections[j].start_freq - start_prb > 255){
						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)255<<20);
						//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						--n_ORAN_empty_section;

						//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
						//fprintf(fp, "rb is : %d \n", rb);
						//fprintf(fp, "symInc is : %d \n", symInc);
						//fprintf(fp, "Start prb is : %d \n", start_prb);
						//fprintf(fp, "Number of prb are : %d \n", 255);
						//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);

						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)(start_prb+255)<<28) + (uint64_t)((uint64_t)(ORAN_sections[j].start_freq - start_prb - 255)<<20);
						//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						--n_ORAN_empty_section;

						//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
						//fprintf(fp, "rb is : %d \n", rb);
						//fprintf(fp, "symInc is : %d \n", symInc);
						//fprintf(fp, "Start prb is : %d \n", start_prb+255);
						//fprintf(fp, "Number of prb are : %d \n", ORAN_sections[j].start_freq - start_prb - 255);
						//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
					}
					else{
						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)(ORAN_sections[j].start_freq - start_prb)<<20);
						//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						--n_ORAN_empty_section;

						//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
						//fprintf(fp, "rb is : %d \n", rb);
						//fprintf(fp, "symInc is : %d \n", symInc);
						//fprintf(fp, "Start prb is : %d \n", start_prb);
						//fprintf(fp, "Number of prb are : %d \n", ORAN_sections[j].start_freq - start_prb);
						//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
					}
				}
				sectionID = j;
				num_prb = ORAN_sections[j].num_freq;
				start_prb = ORAN_sections[j].start_freq + num_prb;

				HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)sectionID<<40) + (uint64_t)((uint64_t)ORAN_sections[j].start_freq<<28) + (uint64_t)((uint64_t)num_prb<<20);
				//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

				//fprintf(fp, "Section ID is : %d \n", sectionID);
				//fprintf(fp, "rb is : %d \n", rb);
				//fprintf(fp, "symInc is : %d \n", symInc);
				//fprintf(fp, "Start prb is : %d \n", ORAN_sections[j].start_freq);
				//fprintf(fp, "Number of prb are : %d \n", num_prb);
				//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
			} */
			/* if (start_prb != MAX_PRB_BW){
				if (MAX_PRB_BW - start_prb > 255){
					HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)255<<20);
					//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
					--n_ORAN_empty_section;

					//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
					//fprintf(fp, "rb is : %d \n", rb);
					//fprintf(fp, "symInc is : %d \n", symInc);
					//fprintf(fp, "Start prb is : %d \n", start_prb);
					//fprintf(fp, "Number of prb are : %d \n", 255);
					//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);

					HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)(start_prb+255)<<28) + (uint64_t)((uint64_t)(MAX_PRB_BW - start_prb - 255)<<20);
					//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
					--n_ORAN_empty_section;

					//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
					//fprintf(fp, "rb is : %d \n", rb);
					//fprintf(fp, "symInc is : %d \n", symInc);
					//fprintf(fp, "Start prb is : %d \n", start_prb+255);
					//fprintf(fp, "Number of prb are : %d \n", MAX_PRB_BW - start_prb - 255);
					//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
				}
				else{
					HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)(MAX_PRB_BW - start_prb)<<20);
					//fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
					--n_ORAN_empty_section;

					//fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section+1);
					//fprintf(fp, "rb is : %d \n", rb);
					//fprintf(fp, "symInc is : %d \n", symInc);
					//fprintf(fp, "Start prb is : %d \n", start_prb);
					//fprintf(fp, "Number of prb are : %d \n", MAX_PRB_BW - start_prb);
					//fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
				}
			} */
		}
	}
}






/*
Function: generate_pdcch_config
Author: Aniruddh V

	Function to generate the configs for PDCCH. Requires the presence of the variable Dl_ttiRequest and 
	Dl_tti_intermediate_t. The required parameters are generated for each PDCCH PDU.

	Inputs:
		PayloadSizeBits
		ScramblingId
		ScramblingRNTI
		AggregationLevel
		 
	Output (to file):
		"K (DCI Length)                : [7:0 ]      8  bits 
         N (Length of encoder data)    : [18:8 ]     11  bits
         E(length after rate matching) : [29:19 ]    11 bits
         Cell ID                       : [15:0]      16 bits
         nRNTI                         : [31:16]     16 bits
         BA Table  (16 entries)        : [31:0]      16bits (Each entry of the BA Table is a 0-2 and hence takes 2 bits)
         DCI data                      : [31:0]      32 bits


*/

void generate_pdcch_config(){
	int K, N, E, nID, NRNTI, n1, n2, K_index, SI_RNTI;
	int BA_table[512], DCI_payload;

	for (int j = 0; j < Dl_tti_intermediate.nPDCCH; ++j){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[j]];
		
		for (int k = 0; k < cur_pdcch.numDlDci; ++k)
		{
			numOfDlDcis_t cur_DCI = cur_pdcch.numOfDlDcis[k];
			nID = cur_DCI.ScramblingId;
			NRNTI = cur_DCI.ScramblingRNTI;
			K = cur_DCI.PayloadSizeBits;
			E = (cur_DCI.AggregationLevel*6)*(12*3/4)*2;
                        SI_RNTI = cur_DCI.RNTI;

			// n2 = ceil(log2(8*(K+24)));
			if (K+24 > 32)
				n2 = 9;
			else
				n2 = 8;

			// n1 = ceil(log2(E));
			switch (cur_DCI.AggregationLevel) {
				case 1:
					n1 = 7+0;
					// if ((16*(K+24) < 9*E) && (E <= 9*2^(n1-4)))
					// 	n1 = 6;
					break;
				case 2:
					n1 = 7+1;
					// if ((16*(K+24) < 9*E) && (E <= 9*2^(n1-4)))
					// 	n1 = 7;
					break;
				case 4:
					n1 = 7+2;
					// if ((16*(K+24) < 9*E) && (E <= 9*2^(n1-4)))
					// 	n1 = 8;
					break;
				case 8:
					n1 = 9;
					break;
				case 16:
					n1 = 9;
					break;
				default:
					n1 = 9;
					break;
			}

			// N = 2^max{min{n1,n2,9},5};
			N = (n1<n2)?n1:n2;
			N = (int)(1<<N);

			// Generate polar encoding table entries
			K_index = 0;

			for (int i = 0; i < 1024; ++i){
				if (polar_reliability_table[i] >= N)
					continue;

				++K_index;
				if (K_index <= N-K-24)
					BA_table[polar_reliability_table[i]] = 0;
				else if (K_index <= N-24)
					BA_table[polar_reliability_table[i]] = 2;
				else
					BA_table[polar_reliability_table[i]] = 1;
			}



			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56) + (uint64_t)K + (uint64_t)((uint64_t)N<<8) + (uint64_t)((uint64_t)E<<19);
			//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56) + (uint64_t)nID + (uint64_t)((uint64_t)NRNTI<<16);
			//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
                        HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56) + (uint64_t)SI_RNTI ;
                        //fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56);
			for (int i = 0; i < N; ++i){
				HW_OP[HW_OP_idx] += (uint64_t)((uint64_t)BA_table[i]<<(2*(i%16)));
				if (i%16 == 15){
					++HW_OP_idx;
					//fprintf(fp, "Hardware packet%d is : 0X%lX\n", 4+i/16, HW_OP[HW_OP_idx-1]);
					HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56);
				}
			}

                        uint8_t byteReversed;
                        for (int i = 0; i < 5; ++i){
                                HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56);
                                for (int i1 = 0; i1 < 4; ++i1){
                                        byteReversed = reverseBits(cur_DCI.Payload[4*i+i1]);
                                HW_OP[HW_OP_idx] += (uint64_t)((uint64_t)byteReversed<<(8*i1));
                                }
                                ++HW_OP_idx;
                                //fprintf(fp, "\nHardware packet%d is : 0X%lX\n", 3+N/16+i, HW_OP[HW_OP_idx-1]);
                        }
                        /*
			for (int i = 0; i < 5; ++i){
				HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH<<56);
				for (int i1 = 0; i1 < 4; ++i1){
					HW_OP[HW_OP_idx] += (uint64_t)((uint64_t)cur_DCI.Payload[4*i+i1]<<(8*i1));
				}
				++HW_OP_idx;
				//fprintf(fp, "\nHardware packet%d is : 0X%lX\n", 3+N/16+i, HW_OP[HW_OP_idx-1]);
			}
                        */

			//fprintf(fp, "K is : %d \n", K);
			//fprintf(fp, "N is : %d \n", N);
			//fprintf(fp, "E is : %d \n", E);
			//fprintf(fp, "nID is : %d \n", nID);
			//fprintf(fp, "NRNTI is : %d \n", NRNTI);

		}

	}
}



/*
Function: generate_pdcch_RE_Mapper_config
Author: Md Taufeeq Ahmed, Rohit singh, Vishnu Palakkal

	Function to generate the configs for PDCCH. Requires the presence of the variable Dl_ttiRequest and 
	Dl_tti_intermediate_t. The required parameters are generated for each PDCCH PDU.

	Inputs:
		AggregationLevel
		StartSymbolIndex
	  	DurationSymbols
		RegBundleSize
		BWPStart
		BWPSize

		 
	Output (to file):
		num REG             : [6:0]  7bits 

		start_RB_reg[i]     : [8:0]   9bits 
		num_RB_reg[i]       : [17:9]  9bits 

		start_RB_user[i]    : [8:0]     9 bits 
		num_RB_user[i]      : [17:9]   9 bits 
		pdcch_present[i]    : [31:18] 14 bits

*/

void generate_pdcch_RE_Mapper_config(){
    int num_REG_Bundle, num_RB_REG_Bundle, total_REG_Bundle=0;
    int i,j,Y,RB_offset,m,ii;
    int start_RB_reg[MAX_REG_BUNDLE], num_RB_reg[MAX_REG_BUNDLE];
   	int start_RB_user[Dl_tti_intermediate.nPDCCH], num_RB_user[Dl_tti_intermediate.nPDCCH];
    int A=0;
    int offset_multiple_of_six;

    for (i=0;i<Dl_tti_intermediate.nPDCCH;i++)
    {
    	PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];	
    	if (A<cur_pdcch.numDlDci) 
    	{
    		A=cur_pdcch.numDlDci;
    	}
    }
    int AggregationLevel[Dl_tti_intermediate.nPDCCH][A];       // Dl_ttiRequest.PDCCH_PDU_t.numOfDlDcis_t.AggregationLevel
    int RegBundleSize[Dl_tti_intermediate.nPDCCH];          	// Dl_ttiRequest.PDCCH_PDU_t.RegBundleSize
    int DurationSymbols[Dl_tti_intermediate.nPDCCH];			// Dl_ttiRequest.PDCCH_PDU_t.DurationSymbols
    int StartSymbolIndex[Dl_tti_intermediate.nPDCCH];        // Dl_ttiRequest.PDCCH_PDU_t.StartSymbolIndex
    int pdcch_present[Dl_tti_intermediate.nPDCCH];
    int num_CORESET_REG_Bundles[Dl_tti_intermediate.nPDCCH];

    // CORESET0 is configured by the PBCH or SIB1 (subcarrier 0 of CRB0 for DMRS mapping)
    uint32_t MIB_value = ConfigRequest.SSBTable.MIB.Value;

    /****************************************************************
    // Index of coreset 0 table (pdcch config sib1) is in the 15,16,17,18 positions (right side bit is 0th position in binary representation)
    // Example for MIB value , coreset table index = 11 in this example
    uint32_t MIB_value = 0b00000000000001010000000000000001;*/
    int index_from_mib =(MIB_value & ((uint32_t)15<<7))>>7; // from 24 bit LSB: 7,8,9,10 positions (taking from SSB pdu)
    //xil_//printf("index_from_mib : %d \n\n", index_from_mib);
    index_from_mib = 13;// Hardcoded in sofware version. no need to add in sdk.

	
    for (int i = 0; i < Dl_tti_intermediate.nPDCCH; i++){

		////// reverse bytes in Freq Domain Resource for easy processing //////////
    	if(Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].CoreSetType !=0 ){
			for(j=0;j<6;++j){
				Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].FreqDomainResource[j] =  reverseBits(Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].FreqDomainResource[j]);
			}
    	}
		///////////////////////////////////////////////////////////////////////////


		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];


		//num_RB_user[i] = cur_pdcch.BWPSize;
		pdcch_intermediate.max_length[i] = 0; // max number of QPSK modulated symbols

		// compute number of REGs in CORESET (num_CORESET_REG) and number of REG bundles in a CORESET
		// For coreset other than coreset0
		int num_CORESET_REG;
		int num_valid_bits = 0;
		pdcch_intermediate.reg_freq_start[i] = 0;
		if(0 == cur_pdcch.CoreSetType){
			num_valid_bits = coreset0_table_scs_15_30[1][index_from_mib]/6;

			// Check duration of symbols
			if (cur_pdcch.DurationSymbols != coreset0_table_scs_15_30[2][index_from_mib]){
				//printf("CORESET0 config Error, pdcch duration symbols = %d and table entry = %d mismatch\n", cur_pdcch.DurationSymbols, coreset0_table_scs_15_30[2][index_from_mib]);
				// changing values according to table
				cur_pdcch.DurationSymbols = coreset0_table_scs_15_30[2][index_from_mib];
				Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].DurationSymbols = cur_pdcch.DurationSymbols;
			}

			// Check BWPStart
			int startRBCoreset0 = ConfigRequest.SSBTable.SsbOffsetPointA.Value/2 - coreset0_table_scs_15_30[3][index_from_mib];

			// Change BWPStart (not sure if this is ok or not !)
			cur_pdcch.BWPStart = startRBCoreset0;
			pdcch_intermediate.reg_freq_start[i] = cur_pdcch.BWPStart * 12;
                        coreset0_dmrs_ref =  cur_pdcch.BWPStart * 12; 
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].BWPStart = startRBCoreset0;
			cur_pdcch.BWPSize = coreset0_table_scs_15_30[1][index_from_mib];
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].BWPSize = coreset0_table_scs_15_30[1][index_from_mib];
			// Check ShiftIndex
			if (cur_pdcch.ShiftIndex != ConfigRequest.CellConfig.phyCellId.Value){
				//printf("CORESET0 config Error, ShiftIndex = %d and phyCellId = %d mismatch\n", cur_pdcch.ShiftIndex, ConfigRequest.CellConfig.phyCellId.Value);
				// change
				cur_pdcch.ShiftIndex = ConfigRequest.CellConfig.phyCellId.Value;
				Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].ShiftIndex = cur_pdcch.ShiftIndex;
			}
			// REG Bundle Size and Interleaver Size and Mapping Type fixed
			cur_pdcch.RegBundleSize = 6;
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].RegBundleSize = 6;
			cur_pdcch.InterleaverSize = 2;
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].InterleaverSize = 2;
			cur_pdcch.CceRegMappingType = 1; // Interleaved
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].CceRegMappingType = 1; // Always interleaved
			////printf("BWP start = %d, shift index = %d, duration = %d, num rb = %d",startRBCoreset0,cur_pdcch.ShiftIndex,cur_pdcch.DurationSymbols, coreset0_table_scs_15_30[1][index_from_mib]);
		}
		else{
			uint64_t* freqDomBitmapPtr = (uint64_t*) (cur_pdcch.FreqDomainResource);
			uint64_t bitVal;
			// //printf("\n Bits: ");
			int newBWPStart = cur_pdcch.BWPStart;
			uint64_t newFreqDomBitmap = (*freqDomBitmapPtr) & (uint64_t) 0x1fffffffffff;
			int lastBit1_idx = -1;
			for(int nBitIdx=0; nBitIdx < 45; nBitIdx++){
				if(nBitIdx*6 >=  cur_pdcch.BWPSize) break;
				bitVal = (*(freqDomBitmapPtr)) & ((uint64_t)1 << nBitIdx);
				if(0 != bitVal){
					num_valid_bits++;
					lastBit1_idx = nBitIdx;
					//printf("1");
				}
				else{
					if(-1 == lastBit1_idx){// if bit=1 not encountered till now, change bwp start
						newBWPStart += 6;
						newFreqDomBitmap = (newFreqDomBitmap >> 1);
					}
					//printf("0");
				}
			}
			// change BWPSize according to the bitmap
			cur_pdcch.BWPSize = (lastBit1_idx+1)*6 - newBWPStart;
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].BWPSize = (lastBit1_idx+1)*6- newBWPStart;
			if(((lastBit1_idx+1)*6) < newBWPStart){
				//printf("Error: BWP and Frequency Domain Bitmap not matching, Check Config");
			} 

			// change BWPStart
			cur_pdcch.BWPStart = newBWPStart;
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].BWPStart = newBWPStart;

			// change bitmap
			for(int nBytes=0; nBytes < MAX_FREQ_DOM_RESOURCE; ++nBytes){
				cur_pdcch.FreqDomainResource[nBytes] = (newFreqDomBitmap & (((uint64_t)(0xff))<<(nBytes*8)))>>((nBytes*8));
				Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].FreqDomainResource[nBytes] = cur_pdcch.FreqDomainResource[nBytes];
			}

			/// for checking ///////
			//freqDomBitmapPtr = (uint64_t*) (cur_pdcch.FreqDomainResource);
			////printf("modified bitmap:\n");
			//for(int nBitIdx=0; nBitIdx < 45; nBitIdx++){
			//	bitVal = (*(freqDomBitmapPtr)) & ((uint64_t)1 << nBitIdx);
			//	if(0 != bitVal){
			//		//printf("1");
			//	}
			//	else{
			//		//printf("0");
			//	}
			//}
			///////////////////////
		}



		// num_RB_Coreset will be num_valid_bits*6
		num_CORESET_REG = num_valid_bits * 6 * cur_pdcch.DurationSymbols;

		int L = cur_pdcch.RegBundleSize;
		if((cur_pdcch.CceRegMappingType == 0) && (L != 6)){ // L = 6 for non-interleaved
			//printf("\n Error in PDCCH:  CceRegMappingType = %d is invalid, it should be 6\n",L);
			// making changes
			L = 6;
			cur_pdcch.RegBundleSize = 6;
			Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]].RegBundleSize = 6;
		}
		num_CORESET_REG_Bundles[i] = num_CORESET_REG/L;

		RegBundleSize[i]=cur_pdcch.RegBundleSize;
		DurationSymbols[i]=cur_pdcch.DurationSymbols;
		StartSymbolIndex[i]=cur_pdcch.StartSymbolIndex;
		start_RB_user[i] = cur_pdcch.BWPStart;

		int f[num_CORESET_REG_Bundles[i]];
		if(cur_pdcch.CceRegMappingType == 1){
			int R = cur_pdcch.InterleaverSize;
			if((num_CORESET_REG%(L*R)) != 0){
				//printf("\n Error in PDCCH:  num_CORESET_REG is invalid, it should be a multiple of L=%d and R=%d\n",L,R);
				// Make sure num_CORESET_REG is multiple of L * R
				num_CORESET_REG = ((int)((num_CORESET_REG/(L*R))+0.5))*L*R;
			}
			int nshift = cur_pdcch.ShiftIndex;
			int C = num_CORESET_REG/(L*R);
			////printf("\nnum_CORESET_REG = %d,L=%d,R=%d",num_CORESET_REG,L,R);
			////printf("C=%d, R= %d,\n",C,R);
			int c,r;
			for(j=0;j<C*R;j++){
				r= j%R;
				c= (j-r)/R;
				f[j] = ( r*C + c + nshift )%(C*R);
				//printf("\n c=%d\t, r=%d\t, f[%d] is %d \n  ",c,r,j,f[j]);
			}
		}

		pdcch_intermediate.total_REG_Bundles_per_user[i] = 0;
		for(int k = 0; k < cur_pdcch.numDlDci; ++k){
			numOfDlDcis_t cur_DCI = cur_pdcch.numOfDlDcis[k];
			AggregationLevel[i][k]=cur_DCI.AggregationLevel;
			total_REG_Bundle= total_REG_Bundle + ((AggregationLevel[i][k]*6)/RegBundleSize[i]);
			pdcch_intermediate.total_REG_Bundles_per_user[i] = pdcch_intermediate.total_REG_Bundles_per_user[i] + ((AggregationLevel[i][k]*6)/RegBundleSize[i]);
			pdcch_intermediate.max_length[i] = pdcch_intermediate.max_length[i] + (AggregationLevel[i][k]*6*12)-(AggregationLevel[i][k]*18);
			int num_REG_Bundles = cur_DCI.AggregationLevel*6/cur_pdcch.RegBundleSize;
		    for(int ii=0;ii<num_CORESET_REG_Bundles[i];ii++){
		    	if(cur_pdcch.CceRegMappingType == 1){
		    		pdcch_intermediate.interleaveMapping[i][k][ii] = f[ii];
		    		////printf("\ninterleaving ");
		    		////printf("\n f[%d] is %d f=%d\n",ii,pdcch_intermediate.interleaveMapping[i][k][ii],f[ii]);
		    	}
		    	else{
		    		pdcch_intermediate.interleaveMapping[i][k][ii] = ii;
		    		////printf("\nnon interleaving ");
		       }
		    }
		}
		//printf("\ntotal reg per user is %d\n",pdcch_intermediate.total_REG_Bundles_per_user[i]);
	}

	int start_RB_reg_all[total_REG_Bundle];
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_RE_MAPPER<<56) + (uint64_t)total_REG_Bundle;
	//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	//fprintf(fp, "num_REG is : %d \n\n", total_REG_Bundle);
	pdcch_intermediate.total_REG_Bundles = total_REG_Bundle;
	uint64_t coreset0bitMapDecimalVal = coreset0_table_scs_15_30[0][index_from_mib];
 	
	for (int i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		int index_total_REG = 0;
		//fprintf(fp, "User Number is : %d \n\n", i);
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
		///////////////////
		// Changing the starting to be a multiple of 6.
		offset_multiple_of_six = 6*ceil(cur_pdcch.BWPStart/6.0) - cur_pdcch.BWPStart;
		//printf("offset new is %d",offset_multiple_of_six);
		//offset_multiple_of_six = 5;
		///////////////////
		num_RB_user[i] = 0;
		for(int k = 0; k < cur_pdcch.numDlDci; ++k)
		{		
			num_REG_Bundle =  ((AggregationLevel[i][k]*6)/RegBundleSize[i]);
			num_RB_REG_Bundle = RegBundleSize[i]/DurationSymbols[i];
			////printf("numrbregbundles = %d",num_RB_REG_Bundle );
			uint64_t bitVal;
			uint64_t* freqDomBitmapPtr;
			if(0 == cur_pdcch.CoreSetType){// coreset0
				freqDomBitmapPtr = (uint64_t*) &coreset0bitMapDecimalVal;
			}
			else{
				freqDomBitmapPtr = (uint64_t*) (cur_pdcch.FreqDomainResource);
			}
			////printf("\nCCE Index is %d",cur_pdcch.numOfDlDcis[k].CceIndex);
			int CceRB = cur_pdcch.numOfDlDcis[k].CceIndex * 6/ DurationSymbols[i];
			int CceRB_to_skip = CceRB; // initially, These many RBs to be skipped
			int CceRB_offset_for_bit = CceRB%6;
			int start_regPerBitIdx;

			int regIdx = 0;
			int regPerBit = 6/num_RB_REG_Bundle;//num_RB_REG_Bundle will be 1,2,3 or 6
			////printf("regperbit is %d",regPerBit);
			int nRBOffset = cur_pdcch.BWPStart;
			for(int nBitIdx=0; nBitIdx < 45; nBitIdx++){
				if(nBitIdx*6 >=  cur_pdcch.BWPSize) break;
				bitVal = (*(freqDomBitmapPtr)) & ((uint64_t)1 << nBitIdx);
				////printf("btval = %x,bitidx = %d, cur_pdcch.BWPSize = %d",bitVal,nBitIdx,cur_pdcch.BWPSize);
				if(0==bitVal){
					nRBOffset = nRBOffset + 6;
				}
				else{
					for(int regPerBitIdx=0; regPerBitIdx < regPerBit;regPerBitIdx++){
						start_RB_reg[regIdx] = nRBOffset + regPerBitIdx*num_RB_REG_Bundle;
						start_RB_reg[regIdx] = start_RB_reg[regIdx] + offset_multiple_of_six; // making it a multiple of six.
						pdcch_intermediate.start_RB_reg[i][k][regIdx] = start_RB_reg[regIdx];
						////printf("\naaa = %d, num_CORESET_REG_Bundles= %d",pdcch_intermediate.start_RB_reg[i][k][regIdx],num_CORESET_REG_Bundles[i]);
						////printf("\t nBitIdx=%d,start_RB_reg[%d]=%d \n",nBitIdx,regIdx,start_RB_reg[regIdx]);
						regIdx = regIdx+1;
						if(regIdx >= num_CORESET_REG_Bundles[i]) break;
					}
					nRBOffset = nRBOffset + 6;
				}
				if(regIdx >= num_CORESET_REG_Bundles[i]) break;
			}
			int start_RB_reg_interleaved[num_CORESET_REG_Bundles[i]];
			for(int index=0;index < num_CORESET_REG_Bundles[i]; index++){
				start_RB_reg_interleaved[index]= start_RB_reg[pdcch_intermediate.interleaveMapping[i][k][index]];
				////printf("\nstart_RB_reg[%d] is %d\n",index,start_RB_reg_interleaved[index]);
			}
			int regbundles_per_cce = 6/RegBundleSize[i];
			int start_RB_reg_new[num_REG_Bundle];
			for (ii=0;ii<num_REG_Bundle;ii++){
				start_RB_reg_new[ii] = start_RB_reg_interleaved[(cur_pdcch.numOfDlDcis[k].CceIndex*regbundles_per_cce) + ii ];
				pdcch_intermediate.start_RB_reg_new[i][k][ii] = start_RB_reg_new[ii];
				pdcch_intermediate.start_RB_reg_new_indices[i][k][ii]=ii;
				start_RB_reg_all[index_total_REG]=start_RB_reg_new[ii];
				pdcch_intermediate.start_RB_reg_all[i][index_total_REG]=start_RB_reg_all[index_total_REG];
				//printf("\n new is %d",pdcch_intermediate.start_RB_reg_all[i][index_total_REG]);
				pdcch_intermediate.start_RB_reg_all_index[i][index_total_REG]= index_total_REG;
				index_total_REG=index_total_REG+1;
				////printf("\nnew start RB REG is %d",start_RB_reg_new[ii]);
			}
			//pdcch_intermediate.start_RB_reg_new[i][k][ii] = start_RB_reg_new[ii];
			//int start_RB_reg_new_sorted

			pdcch_intermediate.num_RB_REG_Bundle[i][k] = num_RB_REG_Bundle;
			pdcch_intermediate.num_REG_Bundle[i][k] = num_REG_Bundle;
			num_RB_user[i] = num_RB_user[i] + AggregationLevel[i][k]*(6/DurationSymbols[i]);
		}
		////printf("total reg bundle is %d",total_REG_Bundle);
		//for (int indexx1 = 0;indexx1<pdcch_intermediate.total_REG_Bundles_per_user;indexx1++){
		//	//printf("\n before sorting: %d",pdcch_intermediate.start_RB_reg_all[i][indexx1]);
		//}
		int total_REG_Bundles_per_user = pdcch_intermediate.total_REG_Bundles_per_user[i];
		for(int nSort0=0; nSort0 < total_REG_Bundles_per_user; nSort0++){
			for(int nSort1=nSort0+1; nSort1 < total_REG_Bundles_per_user; nSort1++){
				if(pdcch_intermediate.start_RB_reg_all[i][nSort0]>pdcch_intermediate.start_RB_reg_all[i][nSort1]){
					////printf("\nsort 0 = %d, sort1 is %d",pdcch_intermediate.start_RB_reg_all[i][nSort0],pdcch_intermediate.start_RB_reg_all[i][nSort0]);
					int tmp_val = pdcch_intermediate.start_RB_reg_all[i][nSort0];
					pdcch_intermediate.start_RB_reg_all[i][nSort0]= pdcch_intermediate.start_RB_reg_all[i][nSort1];
					pdcch_intermediate.start_RB_reg_all[i][nSort1] = tmp_val;

					tmp_val = pdcch_intermediate.start_RB_reg_all_index[i][nSort0] ;
					pdcch_intermediate.start_RB_reg_all_index[i][nSort0] = pdcch_intermediate.start_RB_reg_all_index[i][nSort1];
					pdcch_intermediate.start_RB_reg_all_index[i][nSort1] = tmp_val;
					////printf("\naftersort 0 = %d, aftersort1 is %d",pdcch_intermediate.start_RB_reg_all[i][nSort0],pdcch_intermediate.start_RB_reg_all[i][nSort0]);
				}
			}
		}
		

		//for (int indexx1 = 0;indexx1<pdcch_intermediate.total_REG_Bundles_per_user;indexx1++){
		//	//printf("\n after sorting: %d",pdcch_intermediate.start_RB_reg_all[i][indexx1]);
		//}
		//sorting not happening.
		for (ii=0;ii<total_REG_Bundles_per_user;ii++){
				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_RE_MAPPER<<56) + (uint64_t)pdcch_intermediate.start_RB_reg_all[i][ii] + (uint64_t)((uint64_t)num_RB_REG_Bundle<<9);
				//printf("\norder %d",pdcch_intermediate.start_RB_reg_all_index[i][ii]);
				//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				//fprintf(fp, "start_RB_reg is : %d \n", pdcch_intermediate.start_RB_reg_all[i][ii]);// ii<--f(ii)
				//fprintf(fp, "num_RB_reg is : %d \n\n", num_RB_REG_Bundle);
		}

		pdcch_intermediate.num_RB_user[i] = num_RB_user[i];

		switch (DurationSymbols[i]) {
			case 1:
				pdcch_present[i] = 1;
				pdcch_present[i] = pdcch_present[i] * pow(2,StartSymbolIndex[i]);
				break;
			case 2:
				pdcch_present[i] = 3;
				pdcch_present[i] = pdcch_present[i] * pow(2,StartSymbolIndex[i]);
				break;
			case 3:
				pdcch_present[i] = 7;
				pdcch_present[i] = pdcch_present[i] * pow(2,StartSymbolIndex[i]);
				break;
			default:
				pdcch_present[i] = 0;
		 }
		 pdcch_intermediate.pdcch_present[i] = pdcch_present[i];
	}     
	    
	for (i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_RE_MAPPER<<56) + (uint64_t)cur_pdcch.BWPStart + (uint64_t)((uint64_t)cur_pdcch.BWPSize<<9) + (uint64_t)((uint64_t)pdcch_present[i]<<18);
		//fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		//fprintf(fp, "User Number is %d\n", i);
		//fprintf(fp, "start_RB_user is : %d \n", cur_pdcch.BWPStart);
		//fprintf(fp, "num_RB_user is : %d \n", cur_pdcch.BWPSize);
		//fprintf(fp, "pdcch_present is : %d \n\n", pdcch_present[i]);
	}
	////printf("VAALLUUEE of I is %d",i);
	for (; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_RE_MAPPER<<56);
		//fprintf(fp, "\nHardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}

	for(i=0;i<14;i++){
		for(j=0;j<MaxUE;j++){
			pdcch_intermediate.userpersymbol[i][j] = -1;
		}
	}
	for(i=0;i<14;i++){
		for(j=0;j<Dl_tti_intermediate.nPDCCH;j++){
			PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[j]];
				if(i>=cur_pdcch.StartSymbolIndex && i<= cur_pdcch.StartSymbolIndex + cur_pdcch.DurationSymbols - 1){
				pdcch_intermediate.userpersymbol[i][j] = j;
				////printf("\npdcch_intermediate.userpersymbol[i][j] is %d\n",pdcch_intermediate.userpersymbol[i][j]);
			}
		}
	}
	int total_reg_traversed = 0;
	int total_reg_traversed_dci = 0;
	int index3;
	for(int index2=0 ;index2< Dl_tti_intermediate.nPDCCH; index2++){
		total_reg_traversed = 0;
		total_reg_traversed_dci = 0;
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[index2]];
		for(index3=0;index3<cur_pdcch.numDlDci;index3++){
			////printf("1");
			numOfDlDcis_t cur_DCI = cur_pdcch.numOfDlDcis[index3]; 
			for(int all_index=0; all_index< pdcch_intermediate.total_REG_Bundles_per_user[index2]; all_index++ ){
				////printf("\n2");
				//printf("\nall index is %d, value is %d", all_index,pdcch_intermediate.start_RB_reg_all[index2][all_index]);
				if((pdcch_intermediate.start_RB_reg_all_index[index2][all_index] < (total_reg_traversed_dci+pdcch_intermediate.num_REG_Bundle[index2][index3])) && pdcch_intermediate.start_RB_reg_all_index[index2][all_index] >= total_reg_traversed_dci){
					pdcch_intermediate.start_RB_reg_all_index[index2][all_index] = total_reg_traversed;
					//printf("\nsum is %d",total_reg_traversed );
					total_reg_traversed= total_reg_traversed+1;
					//printf("\nout order is %d",pdcch_intermediate.start_RB_reg_all_index[index2][all_index]);
				}
			} 

			total_reg_traversed_dci = total_reg_traversed_dci + pdcch_intermediate.num_REG_Bundle[index2][index3];
			//printf("\ntotal reg dci is %d",total_reg_traversed_dci);
		}

	}

}



/*Function: generate_PDCCH_DMRS_config
Author: Md Taufeeq Ahmed

	Function to generate the configs for PDCCH. Requires the presence of the variable Dl_ttiRequest and 
	Dl_tti_intermediate_t. The required parameters are generated for each PDCCH PDU.

	Inputs:
		AggregationLevel
		StartSymbolIndex
	  	DurationSymbols
		RegBundleSize
		ScramblingId
		Slot
		 
	Output (to file):
config set 1
        num REG               : [6:0]  7bits 
config set 2
		Cell_ID_user          :[15,0]    16bits
        nsf_user              :[31,16]   16bits 

        Dmrs_present_user    :[13,0]    14 bits
Config set 3
		start_RB_reg[i]      :[8:0]     9bits
        num_RB_reg[i]        :[17:9]    9bits
        User_No_reg[I]       :[23:18]    6bits

*/

void generate_PDCCH_DMRS_config(){
    int i,j,k,ii; 
    // config set 1

    HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_DMRS<<56) + (uint64_t)pdcch_intermediate.total_REG_Bundles;
    //fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
    //fprintf(fp, "num_REG is : %d \n\n", pdcch_intermediate.total_REG_Bundles);

    // config set 2
    for (i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
			numOfDlDcis_t cur_DCI = cur_pdcch.numOfDlDcis[k];                  
			// printing packets of Config 2
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_DMRS<<56) + (uint64_t)cur_DCI.ScramblingId+ (uint64_t)((uint64_t)Dl_ttiRequest.Slot<<16);
		    //fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		    //fprintf(fp, "Cell ID is : %d \n", cur_DCI.ScramblingId);
		    //fprintf(fp, "Slot Number is : %d \n\n", Dl_ttiRequest.Slot);

		    HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_DMRS<<56) + (uint64_t)pdcch_intermediate.pdcch_present[i] + (uint64_t)((uint64_t)pdcch_intermediate.reg_freq_start[i]<<14);
		    //fprintf(fp, "Hardware packet2 is : 0X%lX\n\n", HW_OP[HW_OP_idx-1]);
		    //fprintf(fp, "DMRS present User is : %d \n", pdcch_intermediate.pdcch_present[i]);
		    //fprintf(fp, "reg_freq_start is : %d \n\n", pdcch_intermediate.reg_freq_start[i]);
	}
	for (; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_DMRS<<56);
		//fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_DMRS<<56);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n\n", HW_OP[HW_OP_idx-1]);
	}

    // config set 3 
 	
	for (i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
			int num_rb_reg_bundle = cur_pdcch.RegBundleSize/cur_pdcch.DurationSymbols;
			int total_REG_Bundles_per_user = pdcch_intermediate.total_REG_Bundles_per_user[i];		
			for (ii=0;ii<total_REG_Bundles_per_user;ii++){
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_DMRS<<56) + (uint64_t)pdcch_intermediate.start_RB_reg_all[i][ii] + (uint64_t)((uint64_t)num_rb_reg_bundle<<9) + (uint64_t)((uint64_t)i<<18);
			//fprintf(fp, "Hardware packet1 of Config 3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			//fprintf(fp, "start_RB_reg is : %d \n", pdcch_intermediate.start_RB_reg_all[i][ii]);
			//fprintf(fp, "num_RB_reg is : %d \n", num_rb_reg_bundle);
			//fprintf(fp, "User_num_reg is : %d \n\n",i); // sorted_PDCCH_idx[i] or i ?? 			
		}
		
	}     
}





/*
Function: generate_PDCCH_User_Interleaver_config
Author: Md Taufeeq Ahmed

    Function to generate the configs for PDCCH. Requires the presence of the variable Dl_ttiRequest and 
    Dl_tti_intermediate_t. The required parameters are generated for each PDCCH PDU.

    Inputs:
        AggregationLevel
		StartSymbolIndex
	  	DurationSymbols
		RegBundleSize
         
    Output (to file):
     config set 1   
        num_Reg           : [8:0]     9bits

     config set 2   
        User_No          : [5:0]     6 bits
        start_RB         : [15:6]   10bits 
        num_RB         : [25:16]  10 bits

        max_length          :[15:0]    16bits
        pdcch_present     :[29:16]   14 bits

    config set 3
        start_RB_reg       :[8:0]       9 bits
        num_RB_reg       :[15:9]     9 bits 

*/
void generate_PDCCH_User_Interleaver_config(){
    int i,j,k,ii,CceRB,num_RB_per_DCI,dci_startRB,dci_symb_length;
    // config set 1

    HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_USER_INTERLEAVER<<56) + (uint64_t)pdcch_intermediate.total_REG_Bundles;     
    //fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
    //fprintf(fp, "num_REG is : %d \n\n", pdcch_intermediate.total_REG_Bundles);
    
    int ue_ind = 0;
    // config set 2
    for (i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		//fprintf(fp, "User Number is : %d \n\n", i);
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
		for(int k = 0; k < cur_pdcch.numDlDci; ++k){
			CceRB = cur_pdcch.numOfDlDcis[k].CceIndex * 6/ cur_pdcch.DurationSymbols;
			//printf("\ncce index is %d, duration is %d, cce rb = %d\n",cur_pdcch.numOfDlDcis[k].CceIndex,cur_pdcch.DurationSymbols,CceRB);
			///////
//			dci_startRB = CceRB + cur_pdcch.BWPStart; //Commented for start rb user in case zeros are there in bitmap
			int regbundles_per_cce = 6/cur_pdcch.RegBundleSize;
			dci_startRB = pdcch_intermediate.start_RB_reg[i][k][(cur_pdcch.numOfDlDcis[k].CceIndex*regbundles_per_cce) + 0 ]; // change

			/////
			// num_REG_Bundles = cur_DCI.AggregationLevel*6/cur_pdcch.RegBundleSize;
			// num_RB_REG_Bundle = cur_pdcch.RegBundleSize/cur_pdcch.DurationSymbols;
			num_RB_per_DCI = cur_pdcch.numOfDlDcis[k].AggregationLevel*6/cur_pdcch.DurationSymbols;
			dci_symb_length = cur_pdcch.numOfDlDcis[k].AggregationLevel*6*9; // 9 REs per RB, other 3 REs are for dmrs


			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_USER_INTERLEAVER<<56) + (uint64_t)ue_ind + (uint64_t)((uint64_t)dci_startRB<<6)
																							 + (uint64_t)((uint64_t)num_RB_per_DCI<<16);
			//fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			//fprintf(fp,"User no of the given allocation is %d\n",ue_ind);
			//fprintf(fp, "start_RB_user is : %d \n", dci_startRB);
			//fprintf(fp, "num_RB_user is : %d \n\n", num_RB_per_DCI);

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_USER_INTERLEAVER<<56) + (uint64_t)dci_symb_length
																					         + (uint64_t)((uint64_t)pdcch_intermediate.pdcch_present[i]<<16);
			//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			//fprintf(fp, "Max Length is : %d \n", dci_symb_length);
			//fprintf(fp, "PDCCH Present is : %d \n\n", pdcch_intermediate.pdcch_present[i]);
			ue_ind++;
		}
	}
	for (i=ue_ind; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_USER_INTERLEAVER<<56) + (uint64_t)i;
		//fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_USER_INTERLEAVER<<56);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n\n", HW_OP[HW_OP_idx-1]);
	}
    
    // config set 3 
 	
	for (i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];		
		int regbundles_per_cce = 6/cur_pdcch.RegBundleSize;		
		for(k = 0; k < cur_pdcch.numDlDci; ++k)
		{		
			 // number of REG bundles
		 
		   	 for (ii=0;ii<pdcch_intermediate.num_REG_Bundle[i][k];ii++){
		   	 	int start_RB_reg_before_interleaver = pdcch_intermediate.start_RB_reg[i][k][(cur_pdcch.numOfDlDcis[k].CceIndex*regbundles_per_cce) + ii ];
				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_PDCCH_USER_INTERLEAVER<<56) + (uint64_t)start_RB_reg_before_interleaver  + (uint64_t)((uint64_t)pdcch_intermediate.num_RB_REG_Bundle[i][k]<<9);
			        //fprintf(fp, "Hardware packet 1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				//fprintf(fp, "start_RB_reg is : %d \n", start_RB_reg_before_interleaver );
				//fprintf(fp, "num_RB_reg is : %d \n\n", pdcch_intermediate.num_RB_REG_Bundle[i][k]);		 
			}
		}
	}     
}


/*
Function: generate_PDCCH_User_Interleaver_CB_Config
Author: Md Taufeeq Ahmed

    Function to generate the configs for PDCCH. Requires the presence of the variable Dl_ttiRequest and 
    Dl_tti_intermediate_t. The required parameters are generated for each PDCCH PDU.

*/
void generate_PDCCH_User_Interleaver_CB_Config(){
	int i,k,user_id=0;
	for (i = 0; i < Dl_tti_intermediate.nPDCCH; i++){
		PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[i]];
		for(k = 0; k < cur_pdcch.numDlDci; ++k)
		{
			numOfDlDcis_t cur_DCI = cur_pdcch.numOfDlDcis[k]; 
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_USER_INTERLEAVER_CB<<56) + (uint64_t)user_id + (uint64_t)((uint64_t)(cur_DCI.AggregationLevel*6)*(12*3/4)<<6);
			//fprintf(fp, "Hardware packet 1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			//fprintf(fp, "cur_user_cb : %d \n", user_id);
			//fprintf(fp, "length_cb is : %d \n\n", (cur_DCI.AggregationLevel*6)*(12*3/4)); // Maybe this will be pdcch_intermediate.max_length[sorted_PDCCH_idx[i]]
																					 // Similar changes will be in other modules.
			user_id++;
		}

	}

}

void generate_REG_interleaver_config(){
	int i,j,k,l,REGidex;
	for (i=0;i<14;i++){
		for(j=0;j<Dl_tti_intermediate.nPDCCH;j++){
			//PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[j]];
			if(pdcch_intermediate.userpersymbol[i][j]>=0){
				//fprintf(fp, "Time Domain Symbol is : %d \n\n", i);
				k=pdcch_intermediate.userpersymbol[i][j];
				PDCCH_PDU_t cur_pdcch = Dl_ttiRequest.PDCCH_PDU[sorted_PDCCH_idx[k]];
				//for(l = 0; l < cur_pdcch.numDlDci; ++l){
					//numOfDlDcis_t cur_DCI = cur_pdcch.numOfDlDcis[l];
					//Config Set 1
					int num_rb_reg_bundle = cur_pdcch.RegBundleSize/cur_pdcch.DurationSymbols;
					HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_REG_INTERLEAVER_CB<<56) + (uint64_t)pdcch_intermediate.total_REG_Bundles_per_user[k] + (uint64_t)((uint64_t)num_rb_reg_bundle<<9);
					//fprintf(fp, "Hardware packet 1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
					//fprintf(fp, "Num_REG_Bundle is : %d \n", pdcch_intermediate.total_REG_Bundles_per_user[k]);
					//fprintf(fp, "REG_Bundle_Size is : %d \n\n", num_rb_reg_bundle);

					int total_REG_Bundles_per_user = pdcch_intermediate.total_REG_Bundles_per_user[k];		
					for(REGidex=0;REGidex<total_REG_Bundles_per_user;REGidex++){
					// Config set 2
                    HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_REG_INTERLEAVER_CB<<56) + (uint64_t)pdcch_intermediate.start_RB_reg_all_index[k][REGidex];					
					//fprintf(fp, "Hardware packet 1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
                    //fprintf(fp, "Out sequence after interleaving is is : %d \n\n", pdcch_intermediate.start_RB_reg_all_index[k][REGidex]);
					//fprintf(fp, "Out sequence after interleaving is is : %d \n\n", pdcch_intermediate.start_RB_reg_new_indices[k][l][REGidex]);	
				}			
			}
		}
	}
}

void get_contiguous_RB_alloc_from_bitmap(uint8_t* bitMapArr, int* pStartRB_out, int* pRBSize_out){
    int bitmap_id = 0;
    int startRB=0;
    int RBSize=0;
    int arr_ind = 0;
    int tmpBitmapId;
    uint8_t tmpBitVal = 0;
    while(bitmap_id<273){
        arr_ind = (bitmap_id/8);
        tmpBitmapId = bitmap_id%8;
        tmpBitVal = (uint8_t) ((bitMapArr[arr_ind] >> tmpBitmapId) & ((uint8_t) 0b01));
        if(0 == tmpBitVal){
            if(RBSize > 0){
                break;
            }
            else{
                startRB++;
            }
        }
        else{
            RBSize++;
        }
        bitmap_id++;
    }
    *pStartRB_out = startRB;
    *pRBSize_out = RBSize;
}

void dl_config(){
	HW_OP_idx = 0;
	int i;
	for(double count = 0; count<500; count++){
	//fprintf(fp, "\n ====================== Begin Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);
	//printf("\n ====================== Begin Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);

	//fprintf(fp, "Total PDU count = %d \n", Dl_ttiRequest.nPDUs);
	//fprintf(fp, "PDSCH PDU count = %d \n", Dl_tti_intermediate.nPDSCH);
	//fprintf(fp, "PDCCH PDU count = %d \n", Dl_tti_intermediate.nPDCCH);
	//fprintf(fp, "CSIRS PDU count = %d \n", Dl_tti_intermediate.nCSIRS);
	//fprintf(fp, "SSB PDU count = %d \n", Dl_ttiRequest.nPDUs - Dl_tti_intermediate.nCSIRS - Dl_tti_intermediate.nPDCCH - Dl_tti_intermediate.nPDSCH);

	//Calling the sorting function
	sort_PDU_DL();

	// Call the PDCCH functions
	//fprintf(fp, "\n\n=====================PDCCH configurations========================\n");
	generate_pdcch_config();
	//fprintf(fp, "\n\n=====================PDCCH RE Mapper configurations========================\n");
	generate_pdcch_RE_Mapper_config();
	//fprintf(fp, "\n\n=====================PDCCH DMRS configurations========================\n");
	generate_PDCCH_DMRS_config();
	//fprintf(fp, "\n\n=====================PDCCH User Interleaver configurations========================\n");
	generate_PDCCH_User_Interleaver_config();
	//fprintf(fp, "\n\n=====================PDCCH User Interleaver CB configurations========================\n");
	generate_PDCCH_User_Interleaver_CB_Config();
	//fprintf(fp, "\n\n=====================PDCCH REG Interleaver configurations========================\n");
	generate_REG_interleaver_config();

	//printf("Number of hardware packets after PDCCH configs : %d\n", HW_OP_idx);

	// Call the IP config gen functions
	//fprintf(fp, "\n\n=====================CBS configurations========================\n");
	cbs_configs_DL();
	//printf("Number of hardware packets after CBS ordering : %d\n", HW_OP_idx);

	//fprintf(fp, "\n\n=====================DMRS configurations=======================\n");
	for (i = 0; i < Dl_tti_intermediate.nPDSCH; ++i)
		generate_dmrs_config_DL(sorted_DMRS_idx[i]);
	for (; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56);
		//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56);
		//fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}
	//printf("Number of hardware packets after DMRS configs : %d\n", HW_OP_idx);

	//fprintf(fp, "\n\n=====================CSIRS configurations======================\n");

	for (i = 0; i < Dl_tti_intermediate.nCSIRS; ++i)
		generate_csirs_config(sorted_CSIRS_idx[i]);
	for (; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56);
		//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56) + (uint64_t)Dl_ttiRequest.Slot;
	//fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	//fprintf(fp, "Slot number in the frame is : %d \n", Dl_ttiRequest.Slot);
	//printf("Number of hardware packets after CSIRS configs : %d\n", HW_OP_idx);

	//fprintf(fp, "\n==================SSB/PBCH configs===========================\n");
	generate_pbch_config();
	//printf("Number of hardware packets after SSB configs : %d\n", HW_OP_idx);

	//fprintf(fp, "\n\n====================PDSCH configurations=====================\n\n");
	for (i = 0; i < Dl_tti_intermediate.nCB; ++i){
		int CB_index_test = sorted_CB_idx[i];
		int TB_index_test = CB_TB_idx[CB_index_test];
		int PDSCH_pdu_index_test = Dl_tti_intermediate.TB_prop[TB_index_test].user_id;

		//fprintf(fp, "CB index: %d \n\n", CB_index_test);
		//fprintf(fp, "PDSCH PDU index: %d \n", PDSCH_pdu_index_test);
		//fprintf(fp, "TB index: %d \n", TB_index_test);
		//fprintf(fp, "CB index in TB: %d \n\n", CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_ldpc_config_DL function
		//fprintf(fp, "Configuration for LDPC module- \n");
		generate_ldpc_config_DL(TB_index_test);
		//fprintf(fp, "=======================\n");
		// Testing the generate_puncturing_config function
		//fprintf(fp, "Configuration for puncturing module- \n");
		generate_puncturing_config(TB_index_test);
		//fprintf(fp, "=======================\n");
		// Testing the generate_rate_matching_config function
		//fprintf(fp, "Configuration for rate matching module- \n");
		generate_rate_matching_config(PDSCH_pdu_index_test, TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_bit_interleaving_config function
		//fprintf(fp, "Configuration for bit interleaving module- \n");
		generate_bit_interleaving_config(TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_golden_sequence_config function
		// The configuration for the golden sequence module needs to be generated only once per TB
		// i.e the golden sequence configuration is transmitted only if the codeblock being transmitted
		// is the first codeblock of the TB it belongs to
		if (CB_idx_in_TB[CB_index_test] == 0){
			//fprintf(fp, "Configuration for golden sequence module- \n");
			generate_golden_sequence_config(PDSCH_pdu_index_test, TB_index_test);
			//fprintf(fp, "=======================\n");
		}
		// Testing the generate_scrambler_config function
		//fprintf(fp, "Configuration for scrambler module- \n");
		generate_scrambler_config(TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_modulator_config function
		//fprintf(fp, "Configuration for modulator module- \n");
		generate_modulator_config(TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_layer_mapper_config function
		//fprintf(fp, "Configuration for layer mapper module- \n");
		generate_layer_mapper_config(PDSCH_pdu_index_test, TB_index_test, CB_idx_in_TB[CB_index_test]);

		//fprintf(fp, "=========================================================\n\n");
		//printf("Number of hardware packets after codeblock %d : %d\n", i+1, HW_OP_idx);
	}

	//fprintf(fp, "\n\n===================User Interleaver and RE mapper configurations===================\n\n");
	generate_UEinterleaved_RE_mapper_config();
	//printf("Number of hardware packets after SSB and PDCCH REmapper : %d\n", HW_OP_idx);

	//fprintf(fp, "\n\n=================================================ORAN configurations===================================================\n\n");
	generate_ORAN_config();
	//printf("Number of hardware packets after ORAN : %d\n", HW_OP_idx);
	
	//printf("\n ====================== End Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);
	//fprintf(fp, "\n ====================== End Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);


	

	// Write hardware packets to hardware packets only file
	for (int i = 0; i < HW_OP_idx; ++i){
		//fprintf(fp_configsONLY, "0X%lX\n", HW_OP[i]);
	}
	}
	printf("Without Threads \n");
}


void* thread_1_run(){

	int i;
	for (int count = 0;count <100;count++){
	// Call the PDCCH functions
	//fprintf(fp, "\n\n=====================PDCCH configurations========================\n");
	generate_pdcch_config();
	//fprintf(fp, "\n\n=====================PDCCH RE Mapper configurations========================\n");
	generate_pdcch_RE_Mapper_config();
	//fprintf(fp, "\n\n=====================PDCCH DMRS configurations========================\n");
	generate_PDCCH_DMRS_config();
	//fprintf(fp, "\n\n=====================PDCCH User Interleaver configurations========================\n");
	generate_PDCCH_User_Interleaver_config();
	//fprintf(fp, "\n\n=====================PDCCH User Interleaver CB configurations========================\n");
	generate_PDCCH_User_Interleaver_CB_Config();
	//fprintf(fp, "\n\n=====================PDCCH REG Interleaver configurations========================\n");
	generate_REG_interleaver_config();

	//printf("Number of hardware packets after PDCCH configs : %d\n", HW_OP_idx);

	// Call the IP config gen functions
	//fprintf(fp, "\n\n=====================CBS configurations========================\n");
	cbs_configs_DL();
	////printf("Number of hardware packets after CBS ordering : %d\n", HW_OP_idx);
	
	//fprintf(fp, "\n\n=====================CSIRS configurations======================\n");

	for (i = 0; i < Dl_tti_intermediate.nCSIRS; ++i)
		generate_csirs_config(sorted_CSIRS_idx[i]);
	for (; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56);
		//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_CSIRS<<56) + (uint64_t)Dl_ttiRequest.Slot;
	//fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	//fprintf(fp, "Slot number in the frame is : %d \n", Dl_ttiRequest.Slot);
	//printf("Number of hardware packets after CSIRS configs : %d\n", HW_OP_idx);

	//fprintf(fp, "\n==================SSB/PBCH configs===========================\n");
	generate_pbch_config();
	//printf("Number of hardware packets after SSB configs : %d\n", HW_OP_idx);
	}
}



void* thread_2_run(){
	
	int i;
	for (int count = 0; count <100; count++){
	//fprintf(fp, "\n\n=====================DMRS configurations=======================\n");
	for (i = 0; i < Dl_tti_intermediate.nPDSCH; ++i)
	generate_dmrs_config_DL(sorted_DMRS_idx[i]);
	for (; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56);
		//fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56);
		//fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_TX_DMRS<<56);
		//fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	}
	//printf("Number of hardware packets after DMRS configs : %d\n", HW_OP_idx);

	//fprintf(fp, "\n\n====================PDSCH configurations=====================\n\n");
	for (i = 0; i < Dl_tti_intermediate.nCB; ++i){
		int CB_index_test = sorted_CB_idx[i];
		int TB_index_test = CB_TB_idx[CB_index_test];
		int PDSCH_pdu_index_test = Dl_tti_intermediate.TB_prop[TB_index_test].user_id;

		//fprintf(fp, "CB index: %d \n\n", CB_index_test);
		//fprintf(fp, "PDSCH PDU index: %d \n", PDSCH_pdu_index_test);
		//fprintf(fp, "TB index: %d \n", TB_index_test);
		//fprintf(fp, "CB index in TB: %d \n\n", CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_ldpc_config_DL function
		//fprintf(fp, "Configuration for LDPC module- \n");
		generate_ldpc_config_DL(TB_index_test);
		//fprintf(fp, "=======================\n");
		// Testing the generate_puncturing_config function
		//fprintf(fp, "Configuration for puncturing module- \n");
		generate_puncturing_config(TB_index_test);
		//fprintf(fp, "=======================\n");
		// Testing the generate_rate_matching_config function
		//fprintf(fp, "Configuration for rate matching module- \n");
		generate_rate_matching_config(PDSCH_pdu_index_test, TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_bit_interleaving_config function
		//fprintf(fp, "Configuration for bit interleaving module- \n");
		generate_bit_interleaving_config(TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_golden_sequence_config function
		// The configuration for the golden sequence module needs to be generated only once per TB
		// i.e the golden sequence configuration is transmitted only if the codeblock being transmitted
		// is the first codeblock of the TB it belongs to
		if (CB_idx_in_TB[CB_index_test] == 0){
			//fprintf(fp, "Configuration for golden sequence module- \n");
			generate_golden_sequence_config(PDSCH_pdu_index_test, TB_index_test);
			//fprintf(fp, "=======================\n");
		}
		// Testing the generate_scrambler_config function
		//fprintf(fp, "Configuration for scrambler module- \n");
		generate_scrambler_config(TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_modulator_config function
		//fprintf(fp, "Configuration for modulator module- \n");
		generate_modulator_config(TB_index_test, CB_idx_in_TB[CB_index_test]);
		//fprintf(fp, "=======================\n");
		// Testing the generate_layer_mapper_config function
		//fprintf(fp, "Configuration for layer mapper module- \n");
		generate_layer_mapper_config(PDSCH_pdu_index_test, TB_index_test, CB_idx_in_TB[CB_index_test]);
        //fprintf(fp, "=========================================================\n\n");
	    //printf("Number of hardware packets after codeblock %d : %d\n", i+1, HW_OP_idx);

	//fprintf(fp, "\n\n=================================================ORAN configurations===================================================\n\n");
	generate_ORAN_config();
	//printf("Number of hardware packets after ORAN : %d\n", HW_OP_idx);
	}
	}
}




	


void dl_config_1(){
	HW_OP_idx = 0;
	int i;


	pthread_t thread1,thread2,thread3;
	
	cpu_set_t cpuset, cpuset_1, cpuset_2;

	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);

	CPU_ZERO(&cpuset_1);
	CPU_SET(1, &cpuset_1);


	//pthread_setaffinity(pthread_self(),sizeof(),&cpuset_t); -- ??
	//pthread_getaffinity(); --??


	//fprintf(fp, "\n ====================== Begin Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);
	//printf("\n ====================== Begin Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);

	//fprintf(fp, "Total PDU count = %d \n", Dl_ttiRequest.nPDUs);
	//fprintf(fp, "PDSCH PDU count = %d \n", Dl_tti_intermediate.nPDSCH);
	//fprintf(fp, "PDCCH PDU count = %d \n", Dl_tti_intermediate.nPDCCH);
	//fprintf(fp, "CSIRS PDU count = %d \n", Dl_tti_intermediate.nCSIRS);
	//fprintf(fp, "SSB PDU count = %d \n", Dl_ttiRequest.nPDUs - Dl_tti_intermediate.nCSIRS - Dl_tti_intermediate.nPDCCH - Dl_tti_intermediate.nPDSCH);

	//Calling the sorting function
	sort_PDU_DL();


	pthread_create(&thread1,NULL,thread_1_run,NULL);
	pthread_create(&thread2,NULL,thread_2_run,NULL);
	


	pthread_setaffinity_np(thread1,sizeof(cpuset),&cpuset);
	pthread_setaffinity_np(thread2,sizeof(cpuset_1),&cpuset_1);
    

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);


	//fprintf(fp, "\n\n===================User Interleaver and RE mapper configurations===================\n\n");
	generate_UEinterleaved_RE_mapper_config();
	//printf("Number of hardware packets after SSB and PDCCH REmapper : %d\n", HW_OP_idx);

	

	//printf("\n ====================== End Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);
	//fprintf(fp, "\n ====================== End Packets for SFN = %d, Slot = %d ================  \n \n", Dl_ttiRequest.SFN, Dl_ttiRequest.Slot);
	
	

	

	//Write hardware packets to hardware packets only file
	for (int i = 0; i<HW_OP_idx; i++){
		//fprintf(fp_configsONLY, "0X%lX\n",HW_OP[i]);
	}

	printf("With Threads \n");
	//pthread_exit(NULL);
}




