// #include <fapi_dma_config.h>
#include "l1_ul_tti_request.h"
#include "stdlib.h"

#include "l1_process.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "math.h"
#include <x86intrin.h>
#include "l1_config_request.h"
#include "l1_sw_common.h"
#include "getBAUL.h"
// #include "xil_printf.h"

Ul_ttiRequest_t	Ul_ttiRequest;
ConfigRequest_t ConfigRequest;
Ul_tti_intermediate_t Ul_tti_intermediate;
ORAN_sections_t ORAN_sections[Max_ORAN_sections];


extern FILE *fp;
extern FILE *fp_configsONLY;
FILE *ptr_ul_tti;

uint64_t HW_OP[MAX_CONFIG_PACKETS];
int HW_OP_idx;

int pucch_x1[4000]; //8*14*19 + 8 + 7 1600 = 3743
int pucch_x2[4000];
int pucch_pr_seq_precomputed = 0;

// Sorted array of lifting sizes- this is the order (and indexing) used by the hardware module
// The values are obtained from Table 5.3.2-1, TS 38.212
const uint16_t LIFTING_SIZE_Z_UL[51] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		18, 20, 22, 24, 26, 28, 30, 32, 36, 40, 44, 48, 52, 56, 60, 64, 72, 80,
		88, 96, 104, 112, 120, 128, 144, 160, 176, 192, 208, 224, 240, 256, 288,
		320, 352, 384 };

const preamble_format_t preamble_format[9] = {
		{0,2},
		{1,4},
		{2,6},
		{3,2},
		{4,4},
		{5,6},
		{6,12},
		{7,2},
		{8,6},
};

const int polar_reliability_table_UL[1024] = {
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

const int low_PAPR_table_12[30][12] = {
	{-3,1,-3,-3,-3,3,-3,-1,1,1,1,-3},
	{-3,3,1,-3,1,3,-1,-1,1,3,3,3},
	{-3,3,3,1,-3,3,-1,1,3,-3,3,-3},
	{-3,-3,-1,3,3,3,-3,3,-3,1,-1,-3},
	{-3,-1,-1,1,3,1,1,-1,1,-1,-3,1},
	{-3,-3,3,1,-3,-3,-3,-1,3,-1,1,3},
	{1,-1,3,-1,-1,-1,-3,-1,1,1,1,-3},
	{-1,-3,3,-1,-3,-3,-3,-1,1,-1,1,-3},
	{-3,-1,3,1,-3,-1,-3,3,1,3,3,1},
	{-3,-1,-1,-3,-3,-1,-3,3,1,3,-1,-3},
	{-3,3,-3,3,3,-3,-1,-1,3,3,1,-3},
	{-3,-1,-3,-1,-1,-3,3,3,-1,-1,1,-3},
	{-3,-1,3,-3,-3,-1,-3,1,-1,-3,3,3},
	{-3,1,-1,-1,3,3,-3,-1,-1,-3,-1,-3},
	{1,3,-3,1,3,3,3,1,-1,1,-1,3},
	{-3,1,3,-1,-1,-3,-3,-1,-1,3,1,-3},
	{-1,-1,-1,-1,1,-3,-1,3,3,-1,-3,1},
	{-1,1,1,-1,1,3,3,-1,-1,-3,1,-3},
	{-3,1,3,3,-1,-1,-3,3,3,-3,3,-3},
	{-3,-3,3,-3,-1,3,3,3,-1,-3,1,-3},
	{3,1,3,1,3,-3,-1,1,3,1,-1,-3},
	{-3,3,1,3,-3,1,1,1,1,3,-3,3},
	{-3,3,3,3,-1,-3,-3,-1,-3,1,3,-3},
	{3,-1,-3,3,-3,-1,3,3,3,-3,-1,-3},
	{-3,-1,1,-3,1,3,3,3,-1,-3,3,3},
	{-3,3,1,-1,3,3,-3,1,-1,1,-1,1},
	{-1,1,3,-3,1,-1,1,-1,-1,-3,1,-1},
	{-3,-3,3,3,3,-3,-1,1,-3,3,1,-3},
	{1,-1,3,1,1,-1,-1,-1,1,3,-3,1},
	{-3,3,-3,3,-3,-3,3,-1,-1,1,3,-3}
};

const srs_BW_config_t srs_BW_config[64][4] = {
	{{4,1},{4,1},{4,1},{4,1}},
	{{8,1},{4,2},{4,1},{4,1}},
	{{12,1},{4,3},{4,1},{4,1}},
	{{16,1},{4,4},{4,1},{4,1}},
	{{16,1},{8,2},{4,2},{4,1}},
	{{20,1},{4,5},{4,1},{4,1}},
	{{24,1},{4,6},{4,1},{4,1}},
	{{24,1},{12,2},{4,3},{4,1}},
	{{28,1},{4,7},{4,1},{4,1}},
	{{32,1},{16,2},{8,2},{4,2}},
	{{36,1},{12,3},{4,3},{4,1}},
	{{40,1},{20,2},{4,5},{4,1}},
	{{48,1},{16,3},{8,2},{4,2}},
	{{48,1},{24,2},{12,2},{4,3}},
	{{52,1},{4,13},{4,1},{4,1}},
	{{56,1},{28,2},{4,7},{4,1}},
	{{60,1},{20,3},{4,5},{4,1}},
	{{64,1},{32,2},{16,2},{4,4}},
	{{72,1},{24,3},{12,2},{4,3}},
	{{72,1},{36,2},{12,3},{4,3}},
	{{76,1},{4,19},{4,1},{4,1}},
	{{80,1},{40,2},{20,2},{4,5}},
	{{88,1},{44,2},{4,11},{4,1}},
	{{96,1},{32,3},{16,2},{4,4}},
	{{96,1},{48,2},{24,2},{4,6}},
	{{104,1},{52,2},{4,13},{4,1}},
	{{112,1},{56,2},{28,2},{4,7}},
	{{120,1},{60,2},{20,3},{4,5}},
	{{120,1},{40,3},{8,5},{4,2}},
	{{120,1},{24,5},{12,2},{4,3}},
	{{128,1},{64,2},{32,2},{4,8}},
	{{128,1},{64,2},{16,4},{4,4}},
	{{128,1},{16,8},{8,2},{4,2}},
	{{132,1},{44,3},{4,11},{4,1}},
	{{136,1},{68,2},{4,17},{4,1}},
	{{144,1},{72,2},{36,2},{4,9}},
	{{144,1},{48,3},{24,2},{12,2}},
	{{144,1},{48,3},{16,3},{4,4}},
	{{144,1},{16,9},{8,2},{4,2}},
	{{152,1},{76,2},{4,19},{4,1}},
	{{160,1},{80,2},{40,2},{4,10}},
	{{160,1},{80,2},{20,4},{4,5}},
	{{160,1},{32,5},{16,2},{4,4}},
	{{168,1},{84,2},{28,3},{4,7}},
	{{176,1},{88,2},{44,2},{4,11}},
	{{184,1},{92,2},{4,23},{4,1}},
	{{192,1},{96,2},{48,2},{4,12}},
	{{192,1},{96,2},{24,4},{4,6}},
	{{192,1},{64,3},{16,4},{4,4}},
	{{192,1},{24,8},{8,3},{4,2}},
	{{208,1},{104,2},{52,2},{4,13}},
	{{216,1},{108,2},{36,3},{4,9}},
	{{224,1},{112,2},{56,2},{4,14}},
	{{240,1},{120,2},{60,2},{4,15}},
	{{240,1},{80,3},{20,4},{4,5}},
	{{240,1},{48,5},{16,3},{8,2}},
	{{240,1},{24,10},{12,2},{4,3}},
	{{256,1},{128,2},{64,2},{4,16}},
	{{256,1},{128,2},{32,4},{4,8}},
	{{256,1},{16,16},{8,2},{4,2}},
	{{264,1},{132,2},{44,3},{4,11}},
	{{272,1},{136,2},{68,2},{4,17}},
	{{272,1},{68,4},{4,17},{4,1}},
	{{272,1},{16,17},{8,2},{4,2}},
};

/* Nzc = largest prime number < M_zc for SRS, K_TC = 2 */
const int srs_Nzc_table_K_TC2[64][4]={
		{23,23,23,23},
		{47,23,23,23},
		{71,23,23,23},
		{89,23,23,23},
		{89,47,23,23},
		{113,23,23,23},
		{139,23,23,23},
		{139,71,23,23},
		{167,23,23,23},
		{191,89,47,23},
		{211,71,23,23},
		{239,113,23,23},
		{283,89,47,23},
		{283,139,71,23},
		{311,23,23,23},
		{331,167,23,23},
		{359,113,23,23},
		{383,191,89,23},
		{431,139,71,23},
		{431,211,71,23},
		{449,23,23,23},
		{479,239,113,23},
		{523,263,23,23},
		{571,191,89,23},
		{571,283,139,23},
		{619,311,23,23},
		{661,331,167,23},
		{719,359,113,23},
		{719,239,47,23},
		{719,139,71,23},
		{761,383,191,23},
		{761,383,89,23},
		{761,89,47,23},
		{787,263,23,23},
		{811,401,23,23},
		{863,431,211,23},
		{863,283,139,71},
		{863,283,89,23},
		{863,89,47,23},
		{911,449,23,23},
		{953,479,239,23},
		{953,479,113,23},
		{953,191,89,23},
		{997,503,167,23},
		{1051,523,263,23},
		{1103,547,23,23},
		{1151,571,283,23},
		{1151,571,139,23},
		{1151,383,89,23},
		{1151,139,47,23},
		{1237,619,311,23},
		{1291,647,211,23},
		{1327,661,331,23},
		{1439,719,359,23},
		{1439,479,113,23},
		{1439,283,89,47},
		{1439,139,71,23},
		{1531,761,383,23},
		{1531,761,191,23},
		{1531,89,47,23},
		{1583,787,263,23},
		{1627,811,401,23},
		{1627,401,23,23},
		{1627,89,47,23}
};

const int srs_Nzc_table_K_TC4[64][4]={
		{11,11,11,11},
		{23,11,11,11},
		{31,11,11,11},
		{47,11,11,11},
		{47,23,11,11},
		{59,11,11,11},
		{71,11,11,11},
		{71,31,11,11},
		{83,11,11,11},
		{89,47,23,11},
		{107,31,11,11},
		{113,59,11,11},
		{139,47,23,11},
		{139,71,31,11},
		{151,11,11,11},
		{167,83,11,11},
		{179,59,11,11},
		{191,89,47,11},
		{211,71,31,11},
		{211,107,31,11},
		{227,11,11,11},
		{239,113,59,11},
		{263,131,11,11},
		{283,89,47,11},
		{283,139,71,11},
		{311,151,11,11},
		{331,167,83,11},
		{359,179,59,11},
		{359,113,23,11},
		{359,71,31,11},
		{383,191,89,11},
		{383,191,47,11},
		{383,47,23,11},
		{389,131,11,11},
		{401,199,11,11},
		{431,211,107,11},
		{431,139,71,31},
		{431,139,47,11},
		{431,47,23,11},
		{449,227,11,11},
		{479,239,113,11},
		{479,239,59,11},
		{479,89,47,11},
		{503,251,83,11},
		{523,263,131,11},
		{547,271,11,11},
		{571,283,139,11},
		{571,283,71,11},
		{571,191,47,11},
		{571,71,23,11},
		{619,311,151,11},
		{647,317,107,11},
		{661,331,167,11},
		{719,359,179,11},
		{719,239,59,11},
		{719,139,47,23},
		{719,71,31,11},
		{761,383,191,11},
		{761,383,89,11},
		{761,47,23,11},
		{787,389,131,11},
		{811,401,199,11},
		{811,199,11,11},
		{811,47,23,11}
};

const int srs_phi_table_M_zc12[30][12] = {
		{0,2,0,0,0,3,0,1,2,2,2,0},
		{0,3,2,0,2,3,1,1,2,3,3,3},
		{0,3,3,2,0,3,1,2,3,0,3,0},
		{0,0,1,3,3,3,0,3,0,2,1,0},
		{0,1,1,2,3,2,2,1,2,1,0,2},
		{0,0,3,2,0,0,0,1,3,1,2,3},
		{2,1,3,1,1,1,0,1,2,2,2,0},
		{1,0,3,1,0,0,0,1,2,1,2,0},
		{0,1,3,2,0,1,0,3,2,3,3,2},
		{0,1,1,0,0,1,0,3,2,3,1,0},
		{0,3,0,3,3,0,1,1,3,3,2,0},
		{0,1,0,1,1,0,3,3,1,1,2,0},
		{0,1,3,0,0,1,0,2,1,0,3,3},
		{0,2,1,1,3,3,0,1,1,0,1,0},
		{2,3,0,2,3,3,3,2,1,2,1,3},
		{0,2,3,1,1,0,0,1,1,3,2,0},
		{1,1,1,1,2,0,1,3,3,1,0,2},
		{1,2,2,1,2,3,3,1,1,0,2,0},
		{0,2,3,3,1,1,0,3,3,0,3,0},
		{0,0,3,0,1,3,3,3,1,0,2,0},
		{3,2,3,2,3,0,1,2,3,2,1,0},
		{0,3,2,3,0,2,2,2,2,3,0,3},
		{0,3,3,3,1,0,0,1,0,2,3,0},
		{3,1,0,3,0,1,3,3,3,0,1,0},
		{0,1,2,0,2,3,3,3,1,0,3,3},
		{0,3,2,1,3,3,0,2,1,2,1,2},
		{1,2,3,0,2,1,2,1,1,0,2,1},
		{0,0,3,3,3,0,1,2,0,3,2,0},
		{2,1,3,2,2,1,1,1,2,3,0,2},
		{0,3,0,3,0,0,3,1,1,2,3,0}
};

const int srs_phi_table_M_zc24[30][24] = {
		{1,0,3,1,3,2,3,1,2,0,1,0,1,2,3,0,1,0,3,3,3,0,0,0},
		{1,0,3,2,2,0,2,0,0,2,0,1,1,3,0,3,3,3,0,2,3,3,0,0},
		{1,0,0,2,1,1,0,2,3,1,0,1,1,0,2,2,3,2,0,1,1,3,0,0},
		{2,0,3,1,0,1,3,3,2,1,2,2,3,0,1,0,0,0,1,3,0,1,0,0},
		{1,3,0,0,1,3,1,1,2,3,2,3,1,1,0,2,3,2,1,0,2,1,0,0},
		{0,1,2,0,0,2,2,0,3,1,1,0,2,3,2,1,0,1,0,2,0,0,0,0},
		{0,3,2,3,1,2,0,2,0,2,1,0,1,0,0,0,0,1,1,1,2,2,0,0},
		{0,2,3,1,2,1,3,0,3,1,0,1,0,3,1,1,1,0,1,1,0,3,3,0},
		{0,2,0,3,1,1,1,0,3,2,1,0,1,2,3,1,2,1,2,0,0,0,0,0},
		{2,2,1,0,1,2,2,0,2,1,2,0,3,0,0,3,1,0,2,3,0,2,0,0},
		{0,0,0,1,3,0,3,2,3,2,0,1,1,0,2,2,3,2,1,0,3,2,3,0},
		{0,3,1,3,2,1,1,1,3,3,2,2,2,3,3,2,0,0,1,2,0,2,3,0},
		{3,0,3,1,0,2,3,2,1,1,0,1,3,0,3,1,1,3,3,0,0,3,0,0},
		{0,3,1,3,1,3,3,2,2,0,2,3,0,3,0,0,1,2,3,0,1,1,0,0},
		{0,2,0,1,1,3,2,3,0,2,1,3,3,1,0,3,0,1,1,0,0,0,3,0},
		{0,1,1,0,2,0,0,1,1,3,1,2,1,3,2,0,1,3,2,2,1,1,0,0},
		{0,0,2,1,3,3,0,1,2,1,1,2,2,1,1,3,0,2,0,2,1,1,1,0},
		{3,1,3,1,2,0,2,2,0,0,3,0,1,1,1,1,1,0,0,1,2,2,0,0},
		{0,2,0,2,0,0,2,0,2,0,0,0,0,0,2,0,0,2,2,0,2,2,0,0},
		{0,0,3,3,2,1,1,1,2,0,1,2,1,3,0,1,0,1,1,2,0,3,1,0},
		{0,0,1,1,1,0,2,1,0,1,3,0,2,0,3,0,3,3,2,1,1,2,0,0},
		{3,1,2,1,3,0,2,2,3,1,0,3,2,0,3,1,1,1,1,2,0,0,0,0},
		{0,2,0,3,0,2,0,3,2,1,0,1,0,0,0,0,2,3,1,2,3,3,3,0},
		{0,1,2,0,1,1,2,2,2,3,3,1,2,1,2,1,1,0,0,0,3,2,1,0},
		{0,3,1,0,1,1,1,3,1,1,3,0,1,3,0,3,0,1,3,2,2,1,0,0},
		{0,2,1,0,0,1,2,0,1,0,2,2,1,2,2,3,3,3,1,2,1,2,1,0},
		{1,3,1,1,3,3,1,1,1,3,1,0,2,3,2,2,0,0,0,1,0,1,0,0},
		{3,0,0,1,3,3,0,1,3,2,2,2,3,1,3,0,1,3,1,3,2,1,0,0},
		{0,2,0,2,0,2,2,3,2,0,0,1,2,3,1,0,3,2,1,0,0,0,0,0},
		{3,0,1,2,3,1,1,0,1,3,1,0,1,0,3,1,3,2,2,0,3,0,0,0},
};

CB_prop_t CB_prop[MAX_CB];

int sorted_CB_idx[MAX_CB];
int sorted_SRS_idx[MaxUE];
int sorted_DMRS_idx[MaxUE];
int sorted_PUSCH_idx[MaxUE];
int sorted_PUCCH_DMRS_idx[MaxUE];
int sorted_PUCCH_idx[MaxUE];
int sorted_PRACH_idx[MaxUE];


void get_ul_tti_request_data_from_hex_string(char* fapiHexUlttiReq_i)
{
	uint32_t Pdu_Count=0;
	uint32_t Prach_Count = 0;
	uint32_t Pusch_Count = 0;
	uint32_t Pucch_Count = 0;
	uint32_t Srs_Count = 0;
	uint32_t nUe_Count = 0,Group_Count =0;
	uint32_t var1=0,var2=0,var3=0,var4=0;
	uint32_t ul_pdu_size = 0;
    uint8_t *ul_tti_array_ptr;

    ///////////////////////////////////////////////////////////////////////////////////////
    /* For sw code, read fapi message in hex format and convert it to binary */
	uint8_t fapiBytesUlttiReq[MAX_FAPI_BYTES];
	int numFapiBytes = convertHex2DecBytes(fapiHexUlttiReq_i, fapiBytesUlttiReq);
	// validate fapi message
	if(numFapiBytes > MAX_FAPI_BYTES)
	{
		printf("Error in get_dl_tti_request_data(): Number of Fapi message bytes exceeds allocated limit\n");
		exit(0);
	}
 
	char fapiHeader[9];
	memcpy(fapiHeader,fapiHexUlttiReq_i,8);
	fapiHeader[8]='\0';
	if(0!=strcmp("01008100",fapiHeader))//0100 will be the message header, 8100 is message id
	{
		printf("Error in get_dl_tti_request_data(): Invalid Fapi message bytes\n");
		exit(0);
	}
    //////////////////////////////////////////////////////////////////////////////
 



    uint32_t *ul_32b_pdu = (uint32_t *)(fapiBytesUlttiReq + 4);
    Ul_ttiRequest.MsgLength = ul_32b_pdu[0];
    Ul_ttiRequest.SFN = (uint16_t) *((uint16_t *)(ul_32b_pdu+1));
    //xil_printf("ul_32b_pdu = %p",&ul_32b_pdu);
    ul_pdu_size = Ul_ttiRequest.MsgLength;
    ul_tti_array_ptr = fapiBytesUlttiReq + 10;
     Ul_ttiRequest.Slot = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//     xil_printf("s:%d",Ul_ttiRequest.Slot);
     Ul_ttiRequest.nPDUs = (uint8_t)(*ul_tti_array_ptr++);
     Ul_ttiRequest.RachPresent = (uint8_t)(*ul_tti_array_ptr++);
     Ul_ttiRequest.nULSCH = (uint8_t)(*ul_tti_array_ptr++);
     Ul_ttiRequest.nULCCH = (uint8_t)(*ul_tti_array_ptr++);
     Ul_ttiRequest.nGroup = (uint8_t)(*ul_tti_array_ptr++);
       while(Pdu_Count <(int)(Ul_ttiRequest.nPDUs))//size of total dltti pdu count
        {
        Ul_ttiRequest.ULPDU.PDUType = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
        Ul_ttiRequest.ULPDU.PDUSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
        switch((int)Ul_ttiRequest.ULPDU.PDUType)
        {
            case PRACH_PDU_TAG:
            Ul_ttiRequest.PRACH_PDU[Prach_Count].physCellID = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
            ////xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].physCellID = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].physCellID);
            Ul_ttiRequest.PRACH_PDU[Prach_Count].NumPrachOcas= (uint8_t)(*ul_tti_array_ptr++);
            Ul_ttiRequest.PRACH_PDU[Prach_Count].prachFormat= (uint8_t)(*ul_tti_array_ptr++);
            Ul_ttiRequest.PRACH_PDU[Prach_Count].numRa= (uint8_t)(*ul_tti_array_ptr++);
            ////xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].numRa = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].numRa);
            Ul_ttiRequest.PRACH_PDU[Prach_Count].prachStartSymbol= (uint8_t)(*ul_tti_array_ptr++);
            Ul_ttiRequest.PRACH_PDU[Prach_Count].numCs = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            ////xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs);
//            Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.prgSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface = (uint8_t)(*ul_tti_array_ptr++);
//            //xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface);
//            for(var1=0;var1<(int)(Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs);var1++)
//            {
//             for(var2=0;var2<(int)(Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface);var2++)
//             {
//               Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//               ////xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2]);
//             }
//            }
            Prach_Count = Prach_Count + 1;
            //xil_printf("-----------------------Prach_Count = %d \r\n",Prach_Count);
            break;

             case PUSCH_PDU_TAG:
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             ////xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RNTI = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count]. Handle = (uint32_t)(((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8)) | ((*ul_tti_array_ptr++)<<16) | ((*ul_tti_array_ptr++)<<24));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].BWPSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].BWPStart = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].SubcarrierSpacing = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].CyclicPrefix = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].targetCodeRate = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].qamModOrder = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].mcsIndex = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].mcsTable = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].TransformPrecoding = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dataScramblingId = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].nrOfLayers = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].ulDmrsSymbPos = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsConfigType = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].ulDmrsScramblingId = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].SCID = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].numDmrsCdmGrpsNoData = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsPorts = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].resourceAlloc = (uint8_t)(*ul_tti_array_ptr++);
            // //xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsPorts = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsPorts);
             for(var1 = 0;var1<(int)(MAX_RB_BITMAP);var1++)
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbBitmap[var1] = (uint8_t)(*ul_tti_array_ptr++);
    }
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbStart = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             ////xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbSize = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbSize );
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].VRBtoPRBMapping= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].FrequencyHopping= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].txDirectCurrentLocation= (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].uplinkFrequencyShift7p5khz= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].StartSymbolIndex= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].NrOfSymbols= (uint8_t)(*ul_tti_array_ptr++);
             if(((int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap) & 0b0001 ))
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.rvIndex= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.harqProcessID= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.newDataIndicator= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.TBSize= (uint32_t)(((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8)) | ((*ul_tti_array_ptr++)<<16) | ((*ul_tti_array_ptr++)<<24));
             ////xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.TBSize = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.TBSize);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.numCb= (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             var4 = ceil(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.numCb/8);
             ////xil_printf("var4 = %d \r\n",var4);
             for(var3=0;var3<(int)var4;var3++)
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.cbPresentAndPosition[var3]= (uint8_t)(*ul_tti_array_ptr++);
             ////xil_printf("var3 = %d \n",var3);
}
             }
             if(((int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap) & 0b0010))
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.harqAckBitLength= (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.csiPart1BitLength= (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.csiPart2BitLength= (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.AlphaScaling= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.betaOffsetHarqAck= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.betaOffsetCsi1= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.betaOffsetCsi2= (uint8_t)(*ul_tti_array_ptr++);
             ////xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.harqAckBitLength = %d",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.harqAckBitLength);
             }
             if(((int)Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap & 0b0100))
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts = (uint8_t)(*ul_tti_array_ptr++);
             ////xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts);
             for(var2 = 0;var2<(int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts);var2++)
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PtrsPortConf[var2].PTRSPortIndex = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PtrsPortConf[var2].PTRSDmrsPort = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PtrsPortConf[var2].PTRSReOffset = (uint8_t)(*ul_tti_array_ptr++);
             }
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PTRSTimeDensity = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PTRSFreqDensity= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.ulPTRSPower= (uint8_t)(*ul_tti_array_ptr++);
             }
             if(((int)Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap & 0b1000))
             {
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.lowPaprGroupNumber= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.lowPaprSequenceNumber= (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.ulPtrsSampleDensity= (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.ulPtrsTimeDensityTransformPrecoding= (uint8_t)(*ul_tti_array_ptr++);
             }
//            Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//           // //xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs  = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs);
//            Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.prgSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.digBFInterface = (uint8_t)(*ul_tti_array_ptr++);
//           // //xil_printf(" \n digbf = %d \n", Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.digBFInterface);
//            for(var3=0;var3<(int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs);var3++)
//            {
//             for(var4=0;var4<(int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.digBFInterface);var4++)
//             {
//               Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.NumOfPRGs[var3].beamIdx[var4] =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//             }
//            }
             Pusch_Count = Pusch_Count +1;
             //xil_printf("-----------------------Pusch_Count = %d \r\n",Pusch_Count);
             break;


             case PUCCH_PDU_TAG:
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RNTI =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].Handle =  (uint32_t)(((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8)) | ((*ul_tti_array_ptr++)<<16) | ((*ul_tti_array_ptr++)<<24));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BWPSize =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BWPStart =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].SubcarrierSpacing = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].CyclicPrefix = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].FormatType = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].multiSlotTxIndicator = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].pi2Bpsk = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].prbStart =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].prbSize =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].StartSymbolIndex = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].NrOfSymbols = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].freqHopFlag = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].secondHopPRB =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].groupHopFlag = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].sequenceHopFlag = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].hoppingId =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].InitialCyclicShift =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].dataScramblingId =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].TimeDomainOccIdx = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].PreDftOccIdx = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].PreDftOccLen = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].AddDmrsFlag = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].DmrsScramblingId = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].DMRScyclicshift = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].SRFlag = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BitLenHarq = (uint16_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BitLenCsiPart1 = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BitLenCsiPart2 = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.numPRGs = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.prgSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//           // //xil_printf("\n prgSize = %d \n",Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.prgSize);
//            Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.digBFInterface = (uint8_t)(*ul_tti_array_ptr++);
//           // //xil_printf("digBFInterface = %d\n",Ul_ttiRequ`est.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.digBFInterface);
//            for(var1=0;var1<(int)(Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.numPRGs);var1++)
//{
//             for(var2=0;var2<(int)(Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.digBFInterface);var2++)
//{
//               Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//}
//            }
            Pucch_Count = Pucch_Count + 1;
            //xil_printf("------------------------------------Pucch_Count = %d \r\n",Pucch_Count);
             break;

             case SRS_PDU_TAG:
             Ul_ttiRequest.SRS_PDU[Srs_Count].RNTI =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             ////xil_printf("\n Ul_ttiRequest.SRS_PDU[Srs_Count].RNTI = %d \n",Ul_ttiRequest.SRS_PDU[Srs_Count].RNTI);
             Ul_ttiRequest.SRS_PDU[Srs_Count].Handle =  (uint32_t)(((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8)) | ((*ul_tti_array_ptr++)<<16) | ((*ul_tti_array_ptr++)<<24));
             Ul_ttiRequest.SRS_PDU[Srs_Count].BWPSize =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.SRS_PDU[Srs_Count].BWPStart =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.SRS_PDU[Srs_Count].SubcarrierSpacing = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].CyclicPrefix = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].numAntPorts = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].numSymbols = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].numRepetitions = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].timeStartPosition = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].configIndex = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].sequenceId = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.SRS_PDU[Srs_Count].bandwidthIndex = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].combSize = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].combOffset = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].cyclicShift = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].frequencyPosition = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].frequencyShift = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].frequencyHopping = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].groupOrSequenceHopping = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].resourceType = (uint8_t)(*ul_tti_array_ptr++);
             Ul_ttiRequest.SRS_PDU[Srs_Count].Tsrs =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
             Ul_ttiRequest.SRS_PDU[Srs_Count].Toffset =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.numPRGs = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.prgSize = (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//            Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface = (uint8_t)(*ul_tti_array_ptr++);
//            ////xil_printf("Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface = %d \n",Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface);
//            for(var1=0;var1<(int)(Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.numPRGs);var1++)
//{
//             for(var2=0;var2<(int)(Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface);var2++)
//             {
//               Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] =  (uint16_t)((*ul_tti_array_ptr++) | ((*ul_tti_array_ptr++)<<8));
//}
//            }
            Srs_Count = Srs_Count +1;
            //xil_printf("----------------------------------------Srs_Count = %d \r\n", Srs_Count);
             break;

         default:
           break;
        }
        Pdu_Count = Pdu_Count +1;
        }
       //xil_printf("Pdu_Count = %d \r\n",Pdu_Count);

        while(Group_Count<(int)(Ul_ttiRequest.nGroup))
        {
        Ul_ttiRequest.UeList[nUe_Count].nUe = (uint8_t)(*ul_tti_array_ptr++);
        ////xil_printf("Ul_ttiRequest.UeList[nUe_Count].nUe = %d \n",Ul_ttiRequest.UeList[nUe_Count].nUe);
        for(var1=0;var1<(int)(Ul_ttiRequest.UeList[nUe_Count].nUe);var1++)
        {
        Ul_ttiRequest.UeList[nUe_Count].PduIdx[var1] = (uint8_t)(*ul_tti_array_ptr++);
        }
        nUe_Count = nUe_Count + 1;
        Group_Count = Group_Count+1;
        }

    	Ul_tti_intermediate.nPUSCH = Pusch_Count;
		Ul_tti_intermediate.nPUCCH = Pucch_Count;
		Ul_tti_intermediate.nSRS = Srs_Count;
		Ul_tti_intermediate.nPRACH = Prach_Count;

      //xil_printf("Ul_tti is done \r\n");

}

// Function to read bitwise from array
int get_ul_tti_value_bits(int bits){
	uint8_t a1;
	uint16_t b1;
	uint32_t c1;
	int read_success;

	switch(bits){
		case BITS_EIGHT:
			read_success = fread(&a1,sizeof(uint8_t),1,ptr_ul_tti);
			return a1;
			break;

		case BITS_SIXTEEN:
			read_success = fread(&b1,sizeof(uint16_t),1,ptr_ul_tti);
			return b1;
			break;

		case BITS_THIRTYTWO:
			read_success = fread(&c1,sizeof(uint32_t),1,ptr_ul_tti);
			return c1;
			break;

		default:
			break;
	}
}


void get_ul_tti_request_data_from_file(char* bin_file)
{
	int Pdu_Count = 0;
	int Prach_Count = 0;
	int Pusch_Count = 0;
	int Pucch_Count = 0;
	int Srs_Count = 0;
	int nUe_Count = 0,Group_Count = 0;
	int var1=0,var2=0,var3=0,var4=0;

	//Arrays to store UL_TTI information
	//int ul_tti_req_data[400] = {0};
	//uint8_t	ul_tti_8b_pdu[1600] = {0};

	// int ul_pdu_size = 0;
	// u8 *ul_tti_array_ptr;
	// // int ul_32b_pdu[1] = {0};// not optimized
	// // uint8_t	ul_8b_pdu[1200] = {0};// not optimized
	// u32 *ul_32b_pdu;
	// //fapi_rcv_invalidate_dma_cache_32b(ul_32b_pdu,4,1);// not optimized
	// ul_32b_pdu = (u32 *)((0x01300004)+get_inc_address());/// optimized(to avoid memcpy)
	// //Xil_DCacheInvalidateRange((UINTPTR)ul_32b_pdu,4);
	// Ul_ttiRequest.MsgLength = (ul_32b_pdu[0]<<16)| mesg_length_16b();
	// Ul_ttiRequest.SFN = (uint16_t) *((uint16_t *)&ul_32b_pdu[0]+1);
	// ul_pdu_size = Ul_ttiRequest.MsgLength;
	// ul_tti_array_ptr = (u8 *)((0x01300008)+get_inc_address());/// optimized(to avoid memcpy)
	// //fapi_rcv_invalidate_dma_cache_8b(ul_8b_pdu,ul_pdu_size,2);// not optimized
	// //ul_tti_array_ptr = &ul_8b_pdu[0];// not optimized

	// #include "../../IO_files/ul_tti_request_array.txt"
	// memcpy(ul_tti_8b_pdu,ul_tti_req_data,sizeof(ul_tti_8b_pdu));
	// ul_tti_array_ptr = &ul_tti_8b_pdu[8]; //start pointer to the 8th element

	//Opening the configuration binary file
	ptr_ul_tti = fopen(bin_file,"rb");
	if (NULL == ptr_ul_tti)
	{
		printf("Error: Unable to open input binary file, ptr_ul_tti = NULL \n");
		exit(0);
		// return 1;
	}

	// fseek(ptr_ul_tti,sizeof(uint16_t),SEEK_CUR);

	Ul_ttiRequest.MsgId = get_ul_tti_value_bits(16);
	Ul_ttiRequest.MsgLength = get_ul_tti_value_bits(32);
	Ul_ttiRequest.SFN = get_ul_tti_value_bits(16);
	Ul_ttiRequest.Slot = get_ul_tti_value_bits(16);
	Ul_ttiRequest.nPDUs = get_ul_tti_value_bits(8);
	Ul_ttiRequest.RachPresent = get_ul_tti_value_bits(8);
	Ul_ttiRequest.nULSCH = get_ul_tti_value_bits(8);
	Ul_ttiRequest.nULCCH = get_ul_tti_value_bits(8);
	Ul_ttiRequest.nGroup = get_ul_tti_value_bits(8);
	//xil_printf("Ul_ttiRequest.nPDUs	= %x\n",Ul_ttiRequest.nPDUs);
	//xil_printf("Ul_ttiRequest.RachPresent	= %x\n",Ul_ttiRequest.RachPresent);
	//xil_printf("Ul_ttiRequest.nGroup	= %x\n",Ul_ttiRequest.nGroup);

	while(Pdu_Count <(int)(Ul_ttiRequest.nPDUs))//size of total ultti pdu count
	{
		Ul_ttiRequest.ULPDU.PDUType = get_ul_tti_value_bits(16);
		Ul_ttiRequest.ULPDU.PDUSize = get_ul_tti_value_bits(16);

		//printf("Ul_ttiRequest.ULPDU.PDUType %d\n", Ul_ttiRequest.ULPDU.PDUType);
		switch((int)Ul_ttiRequest.ULPDU.PDUType)
		{
			case PRACH_PDU_TAG:
				Ul_ttiRequest.PRACH_PDU[Prach_Count].physCellID = get_ul_tti_value_bits(16);
				//xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].physCellID = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].physCellID);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].NumPrachOcas= get_ul_tti_value_bits(8);
				/**** Hardcoding now for testing ****/
				// Ul_ttiRequest.PRACH_PDU[Prach_Count].NumPrachOcas= 1;

				Ul_ttiRequest.PRACH_PDU[Prach_Count].prachFormat= get_ul_tti_value_bits(8);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].numRa= get_ul_tti_value_bits(8);
				//xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].numRa = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].numRa);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].prachStartSymbol= get_ul_tti_value_bits(8);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].numCs = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs = get_ul_tti_value_bits(16);
				//xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.prgSize = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface = get_ul_tti_value_bits(8);
				//xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface);
				for(var1=0;var1<(int)(Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.numPRGs);var1++)
				{
					for(var2=0;var2<(int)(Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.digBFInterface);var2++)
					{
						Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] =	get_ul_tti_value_bits(16);
						//xil_printf("Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] = %x \n",Ul_ttiRequest.PRACH_PDU[Prach_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2]);
					}
				}
				//xil_printf("-----------------------Prach_Count = %d",Prach_Count);
				Prach_Count = Prach_Count + 1;
				break;

			case PUSCH_PDU_TAG:
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap = get_ul_tti_value_bits(16);
				//xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RNTI = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].Handle = get_ul_tti_value_bits(32);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].BWPSize = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].BWPStart = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].SubcarrierSpacing = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].CyclicPrefix = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].targetCodeRate = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].qamModOrder = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].mcsIndex = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].mcsTable = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].TransformPrecoding = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dataScramblingId = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].nrOfLayers = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].ulDmrsSymbPos = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsConfigType = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].ulDmrsScramblingId = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].SCID = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].numDmrsCdmGrpsNoData = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsPorts = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].resourceAlloc = get_ul_tti_value_bits(8);
				//xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsPorts = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dmrsPorts);
				for(var1 = 0;var1<(int)(MAX_RB_BITMAP);var1++)
				{
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbBitmap[var1] = get_ul_tti_value_bits(8);
				}
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbStart = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbSize = get_ul_tti_value_bits(16);
				//xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbSize = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].rbSize );
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].VRBtoPRBMapping= get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].FrequencyHopping= get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].txDirectCurrentLocation= get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].uplinkFrequencyShift7p5khz= get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].StartSymbolIndex= get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].NrOfSymbols= get_ul_tti_value_bits(8);
				if(((int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap) & 0b0001 ))
				{
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.rvIndex= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.harqProcessID= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.newDataIndicator= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.TBSize= get_ul_tti_value_bits(32);
					//xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.TBSize = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.TBSize);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.numCb= get_ul_tti_value_bits(16);
					var4 = ceil((double)Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.numCb/8);
					for(var3=0;var3<(int)var4;var3++)
					{
						Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschData.cbPresentAndPosition[var3]= get_ul_tti_value_bits(8);
						//xil_printf("var3 = %d \n",var3);
					}
				}
				if(((int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap) & 0b0010))
				{
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.harqAckBitLength= get_ul_tti_value_bits(16);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.csiPart1BitLength= get_ul_tti_value_bits(16);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.csiPart2BitLength= get_ul_tti_value_bits(16);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.AlphaScaling= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.betaOffsetHarqAck= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.betaOffsetCsi1= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschUci.betaOffsetCsi2= get_ul_tti_value_bits(8);
				}
				if(((int)Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap & 0b0100))
				{
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts = get_ul_tti_value_bits(8);
					//xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts = %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts);
					for(var2 = 0;var2<(int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.numPtrsPorts);var2++)
					{
						Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PtrsPortConf[var2].PTRSPortIndex = get_ul_tti_value_bits(16);
						Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PtrsPortConf[var2].PTRSDmrsPort = get_ul_tti_value_bits(8);
						Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PtrsPortConf[var2].PTRSReOffset = get_ul_tti_value_bits(8);
					}
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PTRSTimeDensity = get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.PTRSFreqDensity= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].puschPtrs.ulPTRSPower= get_ul_tti_value_bits(8);
				}
				if(((int)Ul_ttiRequest.PUSCH_PDU[Pusch_Count].pduBitmap & 0b1000))
				{
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.lowPaprGroupNumber= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.lowPaprSequenceNumber= get_ul_tti_value_bits(16);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.ulPtrsSampleDensity= get_ul_tti_value_bits(8);
					Ul_ttiRequest.PUSCH_PDU[Pusch_Count].dftsOfdm.ulPtrsTimeDensityTransformPrecoding= get_ul_tti_value_bits(8);
				}
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs = get_ul_tti_value_bits(16);
				//xil_printf("Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs	= %d \n",Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.prgSize = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.digBFInterface = get_ul_tti_value_bits(8);
				//xil_printf(" \n digbf = %d \n", Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.digBFInterface);
				for(var3=0;var3<(int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.numPRGs);var3++)
				{
					for(var4=0;var4<(int)(Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.digBFInterface);var4++)
					{
						Ul_ttiRequest.PUSCH_PDU[Pusch_Count].RxPrecoding_Beamforming.NumOfPRGs[var3].beamIdx[var4] =	get_ul_tti_value_bits(16);
					}
				}
				//xil_printf("-----------------------Pusch_Count = %d",Pusch_Count);
				Pusch_Count = Pusch_Count +1;
				break;


			case PUCCH_PDU_TAG:
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RNTI =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].Handle =	get_ul_tti_value_bits(32);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BWPSize =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BWPStart =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].SubcarrierSpacing = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].CyclicPrefix = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].FormatType = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].multiSlotTxIndicator = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].pi2Bpsk = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].prbStart =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].prbSize =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].StartSymbolIndex = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].NrOfSymbols = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].freqHopFlag = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].secondHopPRB =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].groupHopFlag = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].sequenceHopFlag = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].hoppingId =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].InitialCyclicShift =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].dataScramblingId =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].TimeDomainOccIdx = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].PreDftOccIdx = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].PreDftOccLen = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].AddDmrsFlag = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].DmrsScramblingId = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].DMRScyclicshift = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].SRFlag = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BitLenHarq = get_ul_tti_value_bits(8);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BitLenCsiPart1 = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].BitLenCsiPart2 = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.numPRGs = get_ul_tti_value_bits(16);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.prgSize = get_ul_tti_value_bits(16);
				//xil_printf("\n prgSize = %d \n",Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.prgSize);
				Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.digBFInterface = get_ul_tti_value_bits(8);
				for(var1=0;var1<(int)(Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.numPRGs);var1++)
				{
					for(var2=0;var2<(int)(Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.digBFInterface);var2++)
					{
						Ul_ttiRequest.PUCCH_PDU[Pucch_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] =	get_ul_tti_value_bits(16);
					}
				}
				//xil_printf("------------------------------------Pucch_Count = %d",Pucch_Count);
				Pucch_Count = Pucch_Count + 1;
				break;

			case SRS_PDU_TAG:
				Ul_ttiRequest.SRS_PDU[Srs_Count].RNTI =	get_ul_tti_value_bits(16);
				//xil_printf("\n Ul_ttiRequest.SRS_PDU[Srs_Count].RNTI = %d \n",Ul_ttiRequest.SRS_PDU[Srs_Count].RNTI);
				Ul_ttiRequest.SRS_PDU[Srs_Count].Handle =	get_ul_tti_value_bits(32);
				Ul_ttiRequest.SRS_PDU[Srs_Count].BWPSize =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].BWPStart =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].SubcarrierSpacing = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].CyclicPrefix = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].numAntPorts = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].numSymbols = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].numRepetitions = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].timeStartPosition = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].configIndex = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].sequenceId = get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].bandwidthIndex = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].combSize = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].combOffset = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].cyclicShift = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].frequencyPosition = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].frequencyShift = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].frequencyHopping = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].groupOrSequenceHopping = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].resourceType = get_ul_tti_value_bits(8);
				Ul_ttiRequest.SRS_PDU[Srs_Count].Tsrs =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].Toffset =	get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.numPRGs = get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.prgSize = get_ul_tti_value_bits(16);
				Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface = get_ul_tti_value_bits(8);
				//xil_printf("Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface = %d \n",Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface);
				for(var1=0;var1<(int)(Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.numPRGs);var1++)
				{
					for(var2=0;var2<(int)(Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.digBFInterface);var2++)
					{
						Ul_ttiRequest.SRS_PDU[Srs_Count].RxPrecoding_Beamforming.NumOfPRGs[var1].beamIdx[var2] =	get_ul_tti_value_bits(16);
					}
				}
				//xil_printf("----------------------------------------Srs_Count = %d", Srs_Count);
				Srs_Count = Srs_Count +1;
				break;

			default:
				break;
		}
		Pdu_Count = Pdu_Count +1;
	}

	while(Group_Count<(int)(Ul_ttiRequest.nGroup))
	{
		Ul_ttiRequest.UeList[nUe_Count].nUe = get_ul_tti_value_bits(8);
		//xil_printf("Ul_ttiRequest.UeList[nUe_Count].nUe = %d \n",Ul_ttiRequest.UeList[nUe_Count].nUe);
		for(var1=0;var1<(int)(Ul_ttiRequest.UeList[nUe_Count].nUe);var1++)
		{
			Ul_ttiRequest.UeList[nUe_Count].PduIdx[var1] = get_ul_tti_value_bits(8);
		}
		nUe_Count = nUe_Count + 1;
		Group_Count = Group_Count+1;
	}

	Ul_tti_intermediate.nPUSCH = Pusch_Count;
	Ul_tti_intermediate.nPUCCH = Pucch_Count;
	Ul_tti_intermediate.nSRS = Srs_Count;
	Ul_tti_intermediate.nPRACH = Prach_Count;
	//xil_printf("Ul_tti is done \r\n");

	fclose(ptr_ul_tti);
}

/*
Function: merge_UL

	Function to merge two arrays in ascending order. A part of merge sort.
	The mergesort_UL is specific to the sorting of code blocks in the cbs_configs_UL
	function. Thus, it requires that CB_prop and sorted_CB_idx be globally
	available. Given the values l, m and r, the function merges
	sorted_CB_idx[l...m] and sorted_CB_idx[m+1...r].

	This is specific to sorting the sorted_CB_idx array!!!

	COPIED FROM
	https://www.geeksforgeeks.org/merge-sort/

	Input:
		l (int): value of l from above description
		m (int): value of m from above description
		r (int): value of r from above description

	Output:

*/
void merge_UL(int l, int m, int r){
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
        if (CB_prop[L[i]].cb_end_time > CB_prop[R[j]].cb_end_time || CB_prop[L[i]].cb_end_time == CB_prop[R[j]].cb_end_time && CB_prop[L[i]].cb_end_freq > CB_prop[R[j]].cb_end_freq){
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
Function: mergesort_UL

	Function to sort the sort_CB_idx array in ascending order using merge sort.
	The mergesort_UL is specific to the sorting of code blocks in the cbs_configs_UL
	function. Thus, it requires that CB_prop and sorted_CB_idx be globally
	available. Given the values l and r, the function sorts sorted_CB_idx[l...r].

	This is specific to sorting the sorted_CB_idx array!!!

	COPIED FROM
	https://www.geeksforgeeks.org/merge-sort/

	Input:
		l (int): value of l from above description
		r (int): value of r from above description

	Output:

*/

void mergesort_UL(int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergesort_UL(l, m);
        mergesort_UL(m + 1, r);

        merge_UL(l, m, r);
    }
}


/*
Function generate_ldpc_config_UL 
Author: Milind Kumar V

	Function to generate configurations for the LDPC module. Requires the availability of the Ul_tti_intermediate 
	and Ul_ttiRequest variables.

	Input:
		PUSCH_pdu_index (int): index of the PUSCH PDU from which the Transport Block is obtained
		UE_index (int): UE index by which the TB_prop structure in UL_tti_intermediate can be indexed
		CB_index (int): index of the code block being processed, belongs to the user corresponding to UE_index 

	Output (to file): 
		ip_codeblock_size (int): Computed as E_r + K-K' + 2Z_c where E_r, K, K' and Z_c are obtained as specified in Section 6.2, TS 38.212
		base_graph_index (int): Base graph index for LDPC encoding, obtained from Section 6.2.2, TS 38.212. Note that while the specification
			requires it to be either 1 or 2 the output is either 0 or 1 as required by the RX L1 controller config document/sheet. 
		lifting_size_index (int): A value indicating the lifting size index. The index corresponds to the location of Z_c in a sorted array 
			of lifting sizes from table 5.3.2-1
		user_id (int): a value assigned by the L1 controller to codeblocks of each user

*/

void generate_ldpc_config_UL(int PUSCH_pdu_index, int UE_index, int CB_index, int overal_cb_index){

	int user_id;						// index allocated to each user by the L1 controller
	int Z_c;							// minimum value of lifting size according to Section 5.2.2, TS 38.212
	int lifting_size_index = 0;			// location of lifting size in the sorted array of lifting sizes from Table 5.3.2-1, TS 38.212
	int base_graph_index;				// Base graph index
	int K;								// The number of bits in each code block, see Section 5.2.2, TS 38.212
	int Nl;								// Number of layers the TB is associated with, see Section 5.4.2.1, TS 38.212
	int Qm;								// Modulation order for the TB, see Section 5.4.2.1, TS 38.212
	int E;								// Number of bits in the output of the rate matching module, see Section 5.4.2.1, TS 38.212
	int ip_codeblock_size;				// Computed as E + K-K' + 2*Z_c
	int K_b;

	user_id = (int)Ul_tti_intermediate.TB_prop[UE_index].user_id;
	lifting_size_index = (int)Ul_tti_intermediate.TB_prop[UE_index].lifting_size_index;
	K_b = Ul_tti_intermediate.TB_prop[UE_index].K_b;

	Nl = (int)Ul_tti_intermediate.TB_prop[UE_index].numLayers;
	Qm = (int)Ul_tti_intermediate.TB_prop[UE_index].qamModOrder;
	E = Ul_tti_intermediate.TB_prop[UE_index].E[CB_index];
	CB_prop[overal_cb_index].E_per_layer = E/(Nl*Qm);
	K = (int)Ul_tti_intermediate.TB_prop[UE_index].K;
	E = E + (K - (int)Ul_tti_intermediate.TB_prop[UE_index].K_dash);
	Z_c = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c;
	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 1)
		//if (Z_c*66 < E)
			E = Z_c*66;
	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 2)
	//	if (Z_c*(40+K_b) < E)
			E = Z_c*(40+K_b);

	// Computing ip_codeblock_size
	ip_codeblock_size = E + 2*Z_c;

	// Correction for base_graph_index- While Section 6.2.2, TS 38.212 determines base_graph_index to be either 1 or 2,
	// the RX L1 Controller Config sheet requries base_graph_index to be either 0 or 1. Therefore, the value is altered
	// right before it is supplied to the hardware module.
	// TODO: verify this!
	base_graph_index = (int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index;
	if (base_graph_index == 1){
		base_graph_index = 0;
	}
	if (base_graph_index == 2){
	switch (K_b) {
		case 10:
			base_graph_index = 1;
			break;
		case 9:
			base_graph_index = 2;
			break;
		case 8:
			base_graph_index = 3;
			break;
		case 6:
			base_graph_index = 4;
			break;
		}
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_LDPC<<56) + (uint64_t)ip_codeblock_size + (uint64_t)((uint64_t)base_graph_index<<15) + (uint64_t)((uint64_t)lifting_size_index<<18) + (uint64_t)((uint64_t)user_id<<24);
	fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write output to file
	// TODO: Write to hardware
	fprintf(fp, "ip_codeblock_size: %d \n", ip_codeblock_size);
	fprintf(fp, "base_graph_index: %d \n", base_graph_index);
	fprintf(fp, "lifting_size_index: %d \n", lifting_size_index);
	fprintf(fp, "user_id: %d \n", user_id);

}





/*
Function generate_filler_bit_remover_config 
Author: Milind Kumar V

	Function to generate configurations for the filler bit remover module. Requires the availability of the 
	Ul_tti_intermediate variable. 

	Input:
		UE_index (int): UE index by which the TB_prop structure in UL_tti_intermediate can be indexed

	Output (to file): 
		no_of_filler_bits (int): Number of filler bits inserted, equals K - K', see Section 5.2.2, TS 38.212
		msg_block_size (int): Computed as K where, see Section 5.2.2, TS 38.212

*/

void generate_filler_bit_remover_config(int UE_index){
	int no_of_filler_bits;				// K - K', see Section 5.2.2, TS 38.212
	int msg_block_size;					// 22*Z_c

	msg_block_size = (int)Ul_tti_intermediate.TB_prop[UE_index].K;
	no_of_filler_bits = msg_block_size - (int)Ul_tti_intermediate.TB_prop[UE_index].K_dash;
	
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_FILLER_BIT<<56) + (uint64_t)no_of_filler_bits + (uint64_t)((uint64_t)msg_block_size<<10);
	fprintf(fp, "Hardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write output to file
	// TODO: Write to hardware
	fprintf(fp, "no_of_filler_bits: %d \n", no_of_filler_bits);
	fprintf(fp, "msg_block_size: %d \n", msg_block_size);
}





/*
Function generate_depuncturing_config 
Author: Milind Kumar V

	Function to generate configurations for the depuncturing module. Requires the availability of the 
	Ul_tti_intermediate variable. The four outputs are split into 2 groups with kcb and Kcb being transmitted
	together and lifting_factor and Ncb being transmitted together.

	Input:
		UE_index (int): UE index by which the TB_prop structure in UL_tti_intermediate can be indexed
		CB_index (int): index of the code block being processed, belongs to the user corresponding to UE_index

	Output (to file): 
		kcb (int): K' from Section 5.2.2, TS 38.212
		Kcb (int): 22Z_c where Z_c is the lifting size index determined in Section 5.2.2, TS 38.212 (always
			use base graph 1?)
		lifting_factor (int): Z_c from Section 5.2.2, TS 38.212
		Ncb (int): E_r + K - K', where K and K' are obtained from Section 5.2.2, TS 38.212 and E_r is the output
			sequence length after rate matching obtained from Section 5.4.2, TS 38.212

*/

void generate_depuncturing_config(int UE_index, int CB_index){
	int kcb = 0;						// K'
	int Kcb = 0;						// K 
	int lifting_factor = 0;				// Z_c
	int Ncb = 0;						// E_r + K-K'
	int K_b = (int)Ul_tti_intermediate.TB_prop[UE_index].K_b;

	kcb = (int)Ul_tti_intermediate.TB_prop[UE_index].K_dash;
	lifting_factor = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c; 
	
	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 1){
		Kcb = (int)Ul_tti_intermediate.TB_prop[UE_index].K;
		//Ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index] + (Kcb - kcb);
		//if (lifting_factor*66 < Ncb)
			Ncb = lifting_factor*66;
	}		
	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 2){
		Kcb = K_b*lifting_factor;
		//Ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index] + (Kcb - kcb);
		//if ((lifting_factor*(40+K_b)) < Ncb)
			Ncb = lifting_factor*(40+K_b);
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DEPUNCTURING<<56) + (uint64_t)kcb + (uint64_t)((uint64_t)Kcb<<14);
	fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DEPUNCTURING<<56) + (uint64_t)lifting_factor + (uint64_t)((uint64_t)Ncb<<9);
	fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write output to file
	// TODO: Write to hardware
	fprintf(fp, "kcb: %d \n", kcb);
	fprintf(fp, "Kcb: %d \n", Kcb);
	fprintf(fp, "lifting_factor: %d \n", lifting_factor);
	fprintf(fp, "Ncb: %d \n", Ncb);
}



//**************** changes in rate dematching - to be merged after hardware changes for harq ***********************************
///*
//Function generate_rate_dematching_config
//Author: Milind Kumar V
//
//	Function to generate configurations for the rate dematching module. Requires the availability of the
//	Ul_tti_intermediate variable. The six outputs are split into 2 groups with rvid, lifting_factor and ncb being
//	transmitted together and tot_llrs_codeblock, base_graph_index and filler_bits being transmitted together.
//
//	Input:
//		UE_index (int): UE index by which the TB_prop structure in UL_tti_intermediate can be indexed
//		CB_index (int): index of the code block being processed, belongs to the user corresponding to UE_index
//
//	Output (to file):
//		rvid (int): rv index value, see Table 5.4.2.1-2, TS 38.212 for context
//		lifting_factor (int): Z_c from Section 5.2.2, TS 38.212
//		ncb (int): Ncb - no_of_filler_bits, where Ncb is the size of the circular buffer from Section 5.4.2.1, TS 38.212
//		tot_llrs_codeblock (int): E_r, size of the rate matching output sequence, from Section 5.4.2.1, TS 38.212
//		base_graph_index (int): Base graph index for LDPC encoding, obtained from Section 6.2.2, TS 38.212. Note that while the specification
//			requires it to be either 1 or 2 the output is either 0 or 1 as required by the RX L1 controller config document/sheet.
//		filler_bits (int): number of filler bits = K - K', where K and K' are obtained from Section 5.2.2, TS 38.212
//*/
//
//void generate_rate_dematching_config(int UE_index, int CB_index){
//	int rvid;					// rvindex
//	int lifting_factor;				// Z_c
//	int ncb;					// circular_buffer_size - number_of_filler_bits
//	int tot_llrs_codeblock;			// E_r
//	int base_graph_index;				// Base graph index for LDPC encoding, 0 indexed instead of 1 indexed as in the specification
//	int filler_bits;				// Number of filler bits added, equals K - K'
//	int startPos;					// Starting position in the circular buffer
//	int filler_start;
//	int N;
//	int Kb;
//
//	rvid = (int)Ul_tti_intermediate.TB_prop[UE_index].rvIndex;
//
//	lifting_factor = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c;
//
//	filler_bits = (int)Ul_tti_intermediate.TB_prop[UE_index].K - (int)Ul_tti_intermediate.TB_prop[UE_index].K_dash;
//
//	//ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index];		//
//	ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer - filler_bits;
//	printf("\nNCB is %d,ncb is %d, E is %d",(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer,ncb, (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index]);
//
//	//N = (int)Ul_tti_intermediate.CB_prop[cb_index].N;
//
//	Kb= (int)Ul_tti_intermediate.TB_prop[UE_index].K_b;
//
////	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 1)
////		if ((int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*66 - filler_bits < ncb)
////			ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*66 - filler_bits;
////	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 2)
////		if ((int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*50 - filler_bits < ncb)
////			ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*50 - filler_bits;
//
//	tot_llrs_codeblock = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index];
//
//	base_graph_index = (int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index;
//	//base_graph_index = base_graph_index - 1;
//
//	if (base_graph_index == 1)
//		N = lifting_factor*66;
//	else
//		N = lifting_factor*50;
//
//
//	if (base_graph_index == 1){
//		if (rvid == 0)
//			startPos = 0;
//		else if (rvid == 1)
//			startPos = floor(17*(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer/N)*lifting_factor;
//		else if (rvid == 2)
//			startPos = floor(33*(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer/N)*lifting_factor;
//		else
//			startPos = floor(56*(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer/N)*lifting_factor;
//	}
//	else{
//		if (rvid == 0)
//			startPos = 0;
//		else if (rvid == 1)
//			startPos = floor(13*(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer/N)*lifting_factor;
//		else if (rvid == 2)
//			startPos = floor(25*(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer/N)*lifting_factor;
//		else
//			startPos = floor(43*(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer/N)*lifting_factor;
//	}
//	filler_start = (Kb-2)*lifting_factor-filler_bits;
//	printf("\nkb is %d, Zc is %d, fillerbits is %d,startPos(matlab) is %d,filler start is %d, bg is %d, NCB is %d, N is %d", Kb,lifting_factor,filler_bits,startPos, filler_start,base_graph_index,(int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer,N);
//	if (startPos>filler_start){
//		startPos = startPos-filler_bits;
//	}
//	printf("\nstart position is %d",startPos);
//
//
//	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RATE_DEMATCH<<56) + (uint64_t)rvid + (uint64_t)((uint64_t)startPos<<2) + (uint64_t)((uint64_t)ncb<<17);
//	//HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RATE_DEMATCH<<56) + (uint64_t)rvid + (uint64_t)((uint64_t)lifting_factor<<2) + (uint64_t)((uint64_t)ncb<<17);
////	fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
////	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RATE_DEMATCH<<56) + (uint64_t)tot_llrs_codeblock + (uint64_t)((uint64_t)base_graph_index<<15) + (uint64_t)((uint64_t)filler_bits<<18);
////	fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
//
//	// Write output to file
//	// TODO: Write to hardware
//	fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
//	fprintf(fp, "rvid: %d \n", rvid);
//	fprintf(fp, "starting position: %d \n", startPos);
//	fprintf(fp, "ncb: %d \n", ncb);
//	//fprintf(fp, "tot_llrs_codeblock: %d \n", tot_llrs_codeblock);
//	//fprintf(fp, "base_graph_index: %d \n", base_graph_index);
//	//fprintf(fp, "filler_bits: %d \n", filler_bits);
//}
/////////// ***************************************************************************************************************************////////////////


/*
Function generate_rate_dematching_config
Author: Milind Kumar V

	Function to generate configurations for the rate dematching module. Requires the availability of the
	Ul_tti_intermediate variable. The six outputs are split into 2 groups with rvid, lifting_factor and ncb being
	transmitted together and tot_llrs_codeblock, base_graph_index and filler_bits being transmitted together.

	Input:
		UE_index (int): UE index by which the TB_prop structure in UL_tti_intermediate can be indexed
		CB_index (int): index of the code block being processed, belongs to the user corresponding to UE_index

	Output (to file):
		rvid (int): rv index value, see Table 5.4.2.1-2, TS 38.212 for context
		lifting_factor (int): Z_c from Section 5.2.2, TS 38.212
		ncb (int): Ncb - no_of_filler_bits, where Ncb is the size of the circular buffer from Section 5.4.2.1, TS 38.212
		tot_llrs_codeblock (int): E_r, size of the rate matching output sequence, from Section 5.4.2.1, TS 38.212
		base_graph_index (int): Base graph index for LDPC encoding, obtained from Section 6.2.2, TS 38.212. Note that while the specification
			requires it to be either 1 or 2 the output is either 0 or 1 as required by the RX L1 controller config document/sheet.
		filler_bits (int): number of filler bits = K - K', where K and K' are obtained from Section 5.2.2, TS 38.212
*/

void generate_rate_dematching_config(int UE_index, int CB_index){
	int rvid;						// rvindex
	int lifting_factor;				// Z_c
	int ncb;						// circular_buffer_size - number_of_filler_bits
	int tot_llrs_codeblock;			// E_r
	int base_graph_index;			// Base graph index for LDPC encoding, 0 indexed instead of 1 indexed as in the specification
	int filler_bits;				// Number of filler bits added, equals K - K'
	int K_b;

	rvid = (int)Ul_tti_intermediate.TB_prop[UE_index].rvIndex;

	lifting_factor = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c;

	filler_bits = (int)Ul_tti_intermediate.TB_prop[UE_index].K - (int)Ul_tti_intermediate.TB_prop[UE_index].K_dash;
	K_b = (int)Ul_tti_intermediate.TB_prop[UE_index].K_b;

	ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index];		//ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].N_circular_buffer - filler_bits;
	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 1)
		//if ((int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*66 - filler_bits < ncb)
			ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*66 - filler_bits;
	if ((int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index == 2)
		//if ((int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*(40+K_b) - filler_bits < ncb)
			ncb = (int)Ul_tti_intermediate.TB_prop[UE_index].Z_c*(40+K_b) - filler_bits;

	tot_llrs_codeblock = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index];

	base_graph_index = (int)Ul_tti_intermediate.TB_prop[UE_index].base_graph_index;
	if (base_graph_index == 1){
		base_graph_index = 0;
		}
	if (base_graph_index == 2){
		switch (K_b) {
			case 10:
				base_graph_index = 1;
				break;
			case 9:
				base_graph_index = 2;
				break;
			case 8:
				base_graph_index = 3;
				break;
			case 6:
				base_graph_index = 4;
				break;
			}
		}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RATE_DEMATCH<<56) + (uint64_t)rvid + (uint64_t)((uint64_t)lifting_factor<<2) + (uint64_t)((uint64_t)ncb<<11);
	fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RATE_DEMATCH<<56) + (uint64_t)tot_llrs_codeblock + (uint64_t)((uint64_t)base_graph_index<<15) + (uint64_t)((uint64_t)filler_bits<<18);
	fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write output to file
	// TODO: Write to hardware
	fprintf(fp, "rvid: %d \n", rvid);
	fprintf(fp, "lifting_factor: %d \n", lifting_factor);
	fprintf(fp, "ncb: %d \n", ncb);
	fprintf(fp, "tot_llrs_codeblock: %d \n", tot_llrs_codeblock);
	fprintf(fp, "base_graph_index: %d \n", base_graph_index);
	fprintf(fp, "filler_bits: %d \n", filler_bits);
}






/*
Function generate_deinterleaver_config 
Author: Milind Kumar V

	Function to generate configurations for the deinterleaver module. Requires the availability of the 
	Ul_tti_intermediate variable. 

	Input:
		UE_index (int): UE index by which the TB_prop structure in UL_tti_intermediate can be indexed
		CB_index (int): index of the code block being processed, belongs to the user corresponding to UE_index

	Output (to file):
		QAM_type (int): QAM modulation order for the codeblock, belongs to {0,1,2,3,4}, corresponding to
			{BPSK, QPSK, 16 QAM, 64 QAM, 256 QAM}
		tot_llrs_codeblock (int): E_r, size of the rate matching output sequence, from Section 5.4.2.1, TS 38.212
*/

void generate_deinterleaver_config(int UE_index, int CB_index){
	int QAM_type;					// QAM modulation order, belongs to {0,1,2,3,4} therefore, can be obtained as qamModOrder/2 where qamModOrder is defined in the FAPI document
	int tot_llrs_codeblock;			// E_r

	// QAM_type has to be in {0,1,2,3,4} according to RX L1 Controller Config
	// However 0 is not valid as BPSK is not supported in the FAPI document
	QAM_type = (int)Ul_tti_intermediate.TB_prop[UE_index].qamModOrder/2;
	
	tot_llrs_codeblock = (int)Ul_tti_intermediate.TB_prop[UE_index].E[CB_index];

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DEINTERLEAVER<<56) + (uint64_t)QAM_type + (uint64_t)((uint64_t)tot_llrs_codeblock<<4);
	fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	// Write output to file
	// TODO: Write to hardware
	fprintf(fp, "QAM_type: %d \n", QAM_type);
	fprintf(fp, "tot_llrs_codeblock: %d \n", tot_llrs_codeblock);
}





/*
Function generate_descrambler_config 
Author: Milind Kumar V

	Function to generate configurations for the descrambler module. Requires the availability of the 
	Ul_ttiRequest and Ul_tti_intermediate variables. Iterates through all PUSCH PDUs and picks out 
	the RNTI and dataScramblingId.

	TODO: verify if accessing from Ul_tti_intermediate is better than accessing from UL_ttiRequest 

	Input:
		
	Output (to file):
		n_id (array int): equal to the n_ID value from Section 6.3.1.1, TS 38.211, obtained as dataScramblingId 
			from Table 3-46, FAPI document. The array index corresponds to the user_id. The array size is given 
			by MaxUE in constants.h. If the number of PUSCH PDUs is less than MaxUE, the remaining values are 
			set to zero 
		rnti_user (array int): RNTI for users obtained from the value RNTI in Table 3-46 in the FAPI document.
			Array size is equal to MaxUE defined in constants.h and array index corresponds to user_id. If the 
			number of PUSCH PDUs is less than MaxUE, the remaining values are set to zero  
*/

void generate_descrambler_config(){
	int n_id[MaxUE];				// Value of dataScramblingIdPUSCH
	int rnti_user[MaxUE];			// rnti for users 
	int i;							// dummy variable

	for (i = 0; i < MaxUE; i++){
		if (i < (int)Ul_tti_intermediate.nPUSCH){
			// Note that user_id is allocated in the same order in which the PUSCH PDUs
			// appear. Therefore, a PUSCH PDU index of i (< nPUSCH) is the same as 
			// user_id i.
			n_id[i] = (int)Ul_ttiRequest.PUSCH_PDU[i].dataScramblingId;
			rnti_user[i] = (int)Ul_ttiRequest.PUSCH_PDU[i].RNTI;
		} else {
			// Set to dummy values
			n_id[i] = 0;
			rnti_user[i] = 0;
		}
	}

	for (i = 0; i < MaxUE; i++){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DESCRAMBLING<<56) + (uint64_t)rnti_user[i] + (uint64_t)((uint64_t)n_id[i]<<16);
		fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "rnti_user%d: %d \n", i, rnti_user[i]);
		fprintf(fp, "n_id_user%d: %d \n", i, n_id[i]);
	}
}





/*
Function generate_LLR_gen_config 
Author: Milind Kumar V

	Function to generate configurations for the LLR gen module. Requires the availability of the 
	Ul_ttiRequest and Ul_tti_intermediate variables. Iterates through all PUSCH PDUs and picks out 
	the qam_type_user value. 

	TODO: verify if accessing from Ul_tti_intermediate is better than accessing from UL_ttiRequest 

	Input:
		
	Output (to file):
		tot_iq_samples_slot (int): total number of PUSCH IQ samples in the slot- corresponds only to the data symbols,
			the DMRS, CSI-RS symbols and so on are ignored. Therefore, this is simply the sum of G over all the 
			PDUs 
		qam_type_user (array int): QAM modulation type for users obtained from the value qamModOrder in Table 3-46 
			in the FAPI document. Each value belongs to {0,1,2,3,4}, corresponding to {BPSK, QPSK, 16 QAM, 64 QAM, 
			256 QAM} Array size is equal to MaxUE defined in constants.h and array index corresponds to user_id. 
			If the number of PUSCH PDUs is less than MaxUE, the remaining values are set to zero  
*/

void generate_LLR_gen_config(){
	int tot_iq_samples_slot = 0;	// total number of PUSCH IQ samples in the slot- includes only the iq symbols corresponding to IQ samples
	int qam_type_user[MaxUE];		// Value of qamModOrder- belongs to {0,1,2,3,4}; however, 0 is not a feasible value as FAPI does not provide BPSK 
	int i,j;						// dummy variables

	for (i = 0; i < MaxUE; i++){

		if (i < (int)Ul_tti_intermediate.nPUSCH){
			// Note that user_id is allocated in the same order in which the PUSCH PDUs
			// appear. Therefore, a PUSCH PDU index of i (< nPUSCH) is the same as 
			// user_id i.
			qam_type_user[i] = (int)Ul_ttiRequest.PUSCH_PDU[i].qamModOrder/2; // {2,4,6,8}/2 = {1,2,3,4}
			// Assuming Type 1 PUSCH allocation
			// TODO: fix assumption about PUSCH freq allocation
			// The ith PUSCH_PDU index and ith TB_prop in UL_tti_intermediate are equivalent- see cbs_configs_UL
			if ((int)Ul_ttiRequest.PUSCH_PDU[i].resourceAlloc == 1){
				tot_iq_samples_slot = tot_iq_samples_slot + (int)Ul_tti_intermediate.TB_prop[i].G/(int)Ul_tti_intermediate.TB_prop[i].qamModOrder;
			}
		} else {
			// Set to dummy values
			qam_type_user[i] = 0;
		}
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_LLR_GEN_BIT<<56) + (uint64_t)tot_iq_samples_slot;
	fprintf(fp, "Hardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "tot_iq_samples_slot: %d \n", tot_iq_samples_slot);

	for (i = 0; i < (int)ceil((double)MaxUE/10.0); i++){
		HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_LLR_GEN_BIT<<56);
		for (j = 0; j < 10; j++){
			if (10*i + j < MaxUE){
				HW_OP[HW_OP_idx] = HW_OP[HW_OP_idx] + (uint64_t)((uint64_t)qam_type_user[10*i+j]<<(3*j));  
			}
		}
		HW_OP_idx = HW_OP_idx + 1;
		fprintf(fp, "Hardware packet%d is : 0X%lX\n", i,HW_OP[HW_OP_idx-1]);
		for (j = 0; j < 10; j++){
			if (10*i + j < MaxUE){
				fprintf(fp, "qam_type_user%d: %d \n", 10*i + j, qam_type_user[10*i + j]);  
			}
		}
		
	}
}





/*
Function: sort_PDU_UL
Author: Aniruddh V

	Function to sort the PDUs according to the beginning of the BWP they belong to.
	Each PDUtype is sorted individually within it's type.

	Inputs:
		The function expects the Ul_ttiRequest variable of type Ul_ttiRequest_t to be available as a global variable that can
		be accessed within the function. 
	Outputs:
		The global variable arrays are sorted for each PDUtype

*/

void sort_PDU_UL(){

	int PDU_BWP_start[MaxUE];
	int PDU_symb_start[MaxUE];

	// A simple selection sort is used because nPDUs <= 256 and selection sort is O(n^2).
	int nPDU, cur_index, next_index;

	// Adding PUCCH PDUs
	nPDU = Ul_tti_intermediate.nPUCCH;
	for (int i = 0; i < nPDU; ++i){
		sorted_PUCCH_idx[i] = i;
		PDU_BWP_start[i] = Ul_ttiRequest.PUCCH_PDU[i].BWPStart + Ul_ttiRequest.PUCCH_PDU[i].prbStart;
		PDU_symb_start[i] = Ul_ttiRequest.PUCCH_PDU[i].StartSymbolIndex;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_PUCCH_idx[j];
			next_index = sorted_PUCCH_idx[j+1];
			if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_PUCCH_idx[j] = next_index;
				sorted_PUCCH_idx[j+1] = cur_index;
			}
		}
	}

	// Adding PUCCH DMRS PDUs
	nPDU = Ul_tti_intermediate.nPUCCH;
	for (int i = 0; i < nPDU; ++i){
		sorted_PUCCH_DMRS_idx[i] = i;
		PDU_BWP_start[i] = Ul_ttiRequest.PUCCH_PDU[i].BWPStart + Ul_ttiRequest.PUCCH_PDU[i].prbStart;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_PUCCH_DMRS_idx[j];
			next_index = sorted_PUCCH_DMRS_idx[j+1];
			if (PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_PUCCH_DMRS_idx[j] = next_index;
				sorted_PUCCH_DMRS_idx[j+1] = cur_index;
			}
		}
	}

	// Adding SRS PDUs
	nPDU = Ul_tti_intermediate.nSRS;
	for (int i = 0; i < nPDU; ++i){
		sorted_SRS_idx[i] = i;
		PDU_BWP_start[i] = Ul_ttiRequest.SRS_PDU[i].BWPStart + Ul_ttiRequest.SRS_PDU[i].frequencyShift;
		PDU_symb_start[i] = Ul_ttiRequest.SRS_PDU[i].timeStartPosition;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_SRS_idx[j];
			next_index = sorted_SRS_idx[j+1];
			if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_SRS_idx[j] = next_index;
				sorted_SRS_idx[j+1] = cur_index;
			}
		}
	}

	// Adding DMRS PDUs
	nPDU = Ul_tti_intermediate.nPUSCH;
	for (int i = 0; i < nPDU; ++i){
		sorted_DMRS_idx[i] = i;
		int j, start_port = Ul_ttiRequest.PUSCH_PDU[i].dmrsPorts;
		for (j = 0; j < 8; ++j){
			if (start_port%2 == 1){
				break;
			}
			start_port = start_port/2;
		}
		PDU_BWP_start[i] = j;
		PDU_symb_start[i] = Ul_ttiRequest.PUSCH_PDU[i].BWPStart + Ul_ttiRequest.PUSCH_PDU[i].rbStart;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_DMRS_idx[j];
			next_index = sorted_DMRS_idx[j+1];
			if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_DMRS_idx[j] = next_index;
				sorted_DMRS_idx[j+1] = cur_index;
			}
		}
	}

	// Adding PUSCH PDUs
	nPDU = Ul_tti_intermediate.nPUSCH;
	for (int i = 0; i < nPDU; ++i){
		sorted_PUSCH_idx[i] = i;
		int j, start_port = Ul_ttiRequest.PUSCH_PDU[i].dmrsPorts;
		for (j = 0; j < 8; ++j){
			if (start_port%2 == 1){
				break;
			}
			start_port = start_port/2;
		}
		PDU_BWP_start[i] = j;
		PDU_symb_start[i] = Ul_ttiRequest.PUSCH_PDU[i].BWPStart + Ul_ttiRequest.PUSCH_PDU[i].rbStart;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_PUSCH_idx[j];
			next_index = sorted_PUSCH_idx[j+1];
			if (PDU_symb_start[cur_index] > PDU_symb_start[next_index] || PDU_symb_start[cur_index] == PDU_symb_start[next_index] && PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_PUSCH_idx[j] = next_index;
				sorted_PUSCH_idx[j+1] = cur_index;
			}
		}
	}

	// Adding PRACH PDUs
	nPDU = Ul_tti_intermediate.nPRACH;
	for (int i = 0; i < nPDU; ++i){
		sorted_PRACH_idx[i] = i;
		PDU_BWP_start[i] = Ul_ttiRequest.PRACH_PDU[i].numRa;
	}
	for (int i = 0; i < nPDU - 1; ++i){
		for (int j = 0; j < nPDU - i - 1; ++j){
			cur_index = sorted_PRACH_idx[j];
			next_index = sorted_PRACH_idx[j+1];
			if (PDU_BWP_start[cur_index] > PDU_BWP_start[next_index]){
				sorted_PRACH_idx[j] = next_index;
				sorted_PRACH_idx[j+1] = cur_index;
			}
		}
	}

	// Assigning all MU-MIMO users to the same BWP
	int num_idx=0;

	for (int i = 0; i < Ul_tti_intermediate.nPUSCH; ++i){
		int cur_idx = sorted_PUSCH_idx[i];
		if (Ul_ttiRequest.PUSCH_PDU[cur_idx].dmrsPorts %2 == 1){
			++num_idx;

			Ul_tti_intermediate.TB_prop[cur_idx].BWP_idx = num_idx-1;
			Ul_tti_intermediate.BWP_prop[num_idx-1].numLayers = Ul_tti_intermediate.TB_prop[cur_idx].numLayers;
		}
		else{
			Ul_tti_intermediate.TB_prop[cur_idx].BWP_idx = num_idx-1;
			Ul_tti_intermediate.BWP_prop[num_idx-1].numLayers += Ul_tti_intermediate.TB_prop[cur_idx].numLayers;
		}
	}
	Ul_tti_intermediate.nBWP = num_idx;

}





/*
Function: generate_RE_demapper_config
Author: Aniruddh V

	Function to generate configurations for the RE demapper module.

	Input:
		None. Requires the presence of the Ul_ttiRequest and Ul_tti_intermediate variables.

	Output (to file):
		First output all the PUSCH (for each PDU) configs in increasing order of frequency allocation.
		Function outputs the frequency start and length, time locations, dmrs locations, layers occupied, user_id, number of modulation symbols
		Next output all the SRS (for each PDU) configs in increasing order of frequency and time allocation.
		Function outputs the frequency start and length, time locations, layers occupied
		Next output all the PRACH (for each PDU) configs in increasing order of frequency and time allocation.
		Function outputs the frequency start, time locations

*/

void generate_RE_demapper_config(){

	int num_pusch_configs, num_prach_configs, num_srs_configs, num_pucch_configs=0, num_pucchF0_configs=0;
	num_pusch_configs = Ul_tti_intermediate.nPUSCH;
	num_prach_configs = Ul_tti_intermediate.nPRACH;
	num_srs_configs = Ul_tti_intermediate.nSRS;
	for (int i = 0; i < Ul_tti_intermediate.nPUCCH; ++i){
		if (Ul_ttiRequest.PUCCH_PDU[i].FormatType == 0)
			++num_pucchF0_configs;
		else
			++num_pucch_configs;
	}

	// Number of configs
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)num_pusch_configs + (uint64_t)((uint64_t)num_pusch_configs<<5) + (uint64_t)((uint64_t)num_prach_configs<<10) + (uint64_t)((uint64_t)num_srs_configs<<15) + (uint64_t)((uint64_t)num_pucch_configs<<20) + (uint64_t)((uint64_t)num_pucchF0_configs<<25);

	fprintf(fp, "\nHardware packet for number of configs is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	fprintf(fp, "Number of PUSCH/DMRS configs are : %d \n", num_pusch_configs);
	fprintf(fp, "Number of PTRS configs are : %d \n", num_pusch_configs);
	fprintf(fp, "Number of PRACH configs are : %d \n", num_prach_configs);
	fprintf(fp, "Number of SRS configs are : %d \n", num_srs_configs);
	fprintf(fp, "Number of PUCCH configs are : %d \n", num_pucch_configs);
	fprintf(fp, "Number of PUCCH Format0 configs are : %d \n", num_pucchF0_configs);

	// PUSCH configs
	fprintf(fp, "\n==================PUSCH/DMRS/PTRS configs===========================\n");
	for (int i = 0; i < num_pusch_configs; ++i){
		PUSCH_PDU_t cur_pusch = Ul_ttiRequest.PUSCH_PDU[sorted_DMRS_idx[i]];
		if (cur_pusch.dmrsPorts%2 == 0){
			continue;
		}

		int start_RB, num_RB, pusch_present, dmrs_present, ptrs_present, dmrs_data_interleaved;

		start_RB = cur_pusch.BWPStart + cur_pusch.rbStart;
		num_RB = cur_pusch.rbSize;
		dmrs_present = cur_pusch.ulDmrsSymbPos;
		dmrs_data_interleaved = (int)(Ul_tti_intermediate.BWP_prop[Ul_tti_intermediate.TB_prop[sorted_DMRS_idx[i]].BWP_idx].numLayers < 3);
		pusch_present = 0;
		for (int j = cur_pusch.StartSymbolIndex; j < cur_pusch.StartSymbolIndex+cur_pusch.NrOfSymbols; ++j){
			if ( 2 + cur_pusch.dmrsConfigType - cur_pusch.numDmrsCdmGrpsNoData > 0 ){
				pusch_present += (int)(1<<j);
			}
			else if ((dmrs_present>>j)%2 == 0){
				pusch_present += (int)(1<<j);
			}
		}
		if ((cur_pusch.pduBitmap/4)%2 == 0)
			ptrs_present = 0;

		HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)dmrs_present<<18) + (uint64_t)((uint64_t)dmrs_data_interleaved<<31);
		fprintf(fp, "\nHardware packet 1(DMRS) is : 0X%lX\n", HW_OP[HW_OP_idx]);
		HW_OP[HW_OP_idx + num_pusch_configs] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)pusch_present<<18);
		fprintf(fp, "Hardware packet 2(PUSCH) is : 0X%lX\n", HW_OP[HW_OP_idx+num_pusch_configs]);
		HW_OP[HW_OP_idx + 2*num_pusch_configs] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)ptrs_present<<18) + (uint64_t)((uint64_t)1<<31);
		fprintf(fp, "Hardware packet 3(PTRS) is : 0X%lX\n", HW_OP[HW_OP_idx+2*num_pusch_configs]);
		HW_OP_idx++;

		fprintf(fp, "Starting RB is : %d \n", start_RB);
		fprintf(fp, "Length of RB is : %d \n", num_RB);
		fprintf(fp, "PUSCH locations are : 0X%04X \n", pusch_present);
		fprintf(fp, "DMRS locations are : 0X%04X \n", dmrs_present);
		fprintf(fp, "PTRS locations are : 0X%04X \n", ptrs_present);
		fprintf(fp, "Interleaved DMRS is : %d \n", dmrs_data_interleaved);
	}
	HW_OP_idx += 2*num_pusch_configs;
	printf("Number of hardware packets after PUSCH REmapper : %d\n", HW_OP_idx);

	// PRACH config
	fprintf(fp, "\n==================PRACH configs===========================\n");
	for (int i = 0; i < num_prach_configs; ++i){
		PRACH_PDU_t cur_PRACH = Ul_ttiRequest.PRACH_PDU[i];
		int start_RB, num_RB, PRACH_present=0, duration;

		start_RB = ConfigRequest.PRACHConfig.numPRACHFdOccasions_data[cur_PRACH.numRa].k1.Value;	//cur_PRACH.numRa*12;
		num_RB = 12;
		for (int j = 0; j < 9; ++j){
			if (preamble_format[j].format_type == cur_PRACH.prachFormat){
				duration = preamble_format[j].prach_duration;
			}
		}
		for (int j = 0; j < duration*cur_PRACH.NumPrachOcas; ++j){
			if(j > 13) break; // To make sure that number of bits in symbol map doesn't go beyond 14 in error case
			PRACH_present += (int)(1<<j);
		}
		PRACH_present = PRACH_present<<cur_PRACH.prachStartSymbol;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)PRACH_present<<18);
		fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		fprintf(fp, "Starting RB is : %d \n", start_RB);
		fprintf(fp, "Number of RB are : %d \n", num_RB);
		fprintf(fp, "Time locations are : 0X%04X \n", PRACH_present);
	}

	// SRS configs
	fprintf(fp, "\n==================SRS configs===========================\n");
	for (int i = 0; i < num_srs_configs; ++i){
		SRS_PDU_t cur_srs = Ul_ttiRequest.SRS_PDU[sorted_SRS_idx[i]];
		int start_RB, num_RB, start_sym, num_sym, tr_comb, k_bar_tc_1, k_bar_tc_2;

		/* start_RB */
		/* compute the frequency position term which depends upon n_RRC (in RB units) */
			// n_RRC = cur_srs.frequencyPosition;
			// B_SRS = cur_srs.bandwidthIndex
		int sumFreqPos_RB = 0;
		// Check if Frequency hopping enabled or not
		if( cur_srs.frequencyHopping < cur_srs.bandwidthIndex){ // b_hop < B_SRS
			// Frequency hopping disabled
			// TODO: Not supported now, need to add code here
			printf("Error SRS config param: frequencyHopping -- Frequency hopping is unsupported but present in fapi param \n");
		}
		else{
			// Frequency hopping disabled
			int b, N_b, n_b, m_SRS_b;
			for(int b = 0; b <= cur_srs.bandwidthIndex; ++b){
				m_SRS_b = srs_BW_config[cur_srs.configIndex][b].m;
				N_b = srs_BW_config[cur_srs.configIndex][b].N;
				n_b = ((int)((double) cur_srs.frequencyPosition *4/(double)m_SRS_b));
				n_b = n_b%N_b;
				sumFreqPos_RB = sumFreqPos_RB + m_SRS_b * n_b;
			}
		}
		start_RB = cur_srs.frequencyShift + sumFreqPos_RB;

		num_RB = srs_BW_config[cur_srs.configIndex][cur_srs.bandwidthIndex].m;
		start_sym = cur_srs.timeStartPosition;
		num_sym = (int)(1<<cur_srs.numSymbols);
		tr_comb = cur_srs.combSize;

		/* k_bar_tc_1 into [28:27] and k_bar_tc_2 into [30:29] */
		k_bar_tc_1 = cur_srs.combOffset;
		k_bar_tc_2 = k_bar_tc_1; // will be modified for n_srs_cs >= (n_SRS_cs_max/2) and num_ports == 4
		/* num_ports (N_ap_SRS) into [31:29]*/
		if(2==cur_srs.numAntPorts){ // 4 ports
			int ktc, n_SRS_cs_max;
			if(1 == cur_srs.combSize){
				ktc = 4;
				n_SRS_cs_max = 12;
			}
			else{ // if cur_srs.combSize == 0
				ktc = 2;
				n_SRS_cs_max = 8;
			}
			if(cur_srs.cyclicShift >= (n_SRS_cs_max/2)){ // n_srs_cs >= (n_SRS_cs_max/2)
				k_bar_tc_2 = (k_bar_tc_2 + (ktc/2))%ktc; // modified
			}
		}

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)start_RB +
				             (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)start_sym<<18) +
							 (uint64_t)((uint64_t)num_sym<<22) + (uint64_t)((uint64_t)tr_comb<<26) +
							 (uint64_t)((uint64_t)k_bar_tc_1<<27) + (uint64_t)((uint64_t)k_bar_tc_2<<29);
		fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		fprintf(fp, "Starting RB is : %d \n", start_RB);
		fprintf(fp, "Length of RB is : %d \n", num_RB);
		fprintf(fp, "Start symbol is : %d \n", start_sym);
		fprintf(fp, "Number of symbols are : %d \n", num_sym);
		fprintf(fp, "Transmission comb is : %d \n", tr_comb);
		fprintf(fp, "Offset: k_bar_tc_1 is : %d \n", k_bar_tc_1);
		fprintf(fp, "Offset: k_bar_tc_2 is : %d \n", k_bar_tc_2);
	}

	// PUCCH config
	fprintf(fp, "\n==================PUCCH configs===========================\n");
	for (int i = 0; i < num_pucch_configs+num_pucchF0_configs; ++i){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];

		if (cur_pucch.FormatType == 0){
			// PUCCH Format0
			int rb_loc, symb_map;

			rb_loc = cur_pucch.BWPStart + cur_pucch.prbStart;
			symb_map = ((cur_pucch.NrOfSymbols==2)?3:1)<<cur_pucch.StartSymbolIndex;

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)((uint64_t)rb_loc<<0) + (uint64_t)((uint64_t)symb_map<<9);
			fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "Starting RB is : %d \n", rb_loc);
			fprintf(fp, "Symbol map is : %d \n", symb_map);
		}
		else{
			// PUCCH Format2
			int start_RB, num_RB, start_sym, num_sym;

			start_RB = cur_pucch.BWPStart + cur_pucch.prbStart;
			num_RB = cur_pucch.prbSize;
			start_sym = cur_pucch.StartSymbolIndex;
			num_sym = cur_pucch.NrOfSymbols;

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_RE_DEMAPPER<<56) + (uint64_t)((uint64_t)start_RB<<0) + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)start_sym<<18) + (uint64_t)((uint64_t)num_sym<<22);
			fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "Starting RB is : %d \n", start_RB);
			fprintf(fp, "Length of RB is : %d \n", num_RB);
			fprintf(fp, "Start symbol is : %d \n", start_sym);
			fprintf(fp, "Number of symbols are : %d \n", num_sym);
		}
	}
}





/*
Function: cbs_configs_UL
Author: Aniruddh V

	Function to split ALL the TBs into CBs. Also, decides the order in which the CBs are processed.
	The TB properties (mainly number of CBs and CB size) are stored in the Ul_tti_intermediate structure.
	The CBs are ordered based on their "projected" ending RE in the final grid.

	Inputs:
		No inputs. The function uses the global structures of Ul_tti_intermediate and Ul_ttiRequest
		 
	Output (to file):
		The number of codeblocks and codeblock size for each transport block.
		The order of the codeblock processing (These are printed to file for DEBUGGING ONLY)

*/

void cbs_configs_UL(){

	int i,j,k;
	int n_sf = Ul_ttiRequest.Slot;
	Ul_tti_intermediate.nCB = 0;

	int freq_start;
	int freq_len;
	int nLayers;
	int time_start;
	int TB_size;
	int TBcrc_size , num_CB , CB_size;
	double R;
	int Qm;
	int idx;
	int cb_size;

	int Z_c;							// minimum value of lifting size according to Section 5.2.2, TS 38.212
	int lifting_size_index;				// location of lifting size in the sorted array of lifting sizes from Table 5.3.2-1, TS 38.212
	int base_graph_index;				// Base graph index
	int K;								// The number of bits in each code block, see Section 5.2.2, TS 38.212
	int I_LBRM;							// Set using the higher layer parameter rateMatching, see Section 6.2.5, TS 38.212
	int N;								// Number of bits after LDPC encoding, see Section 5.3.2, TS 38.212
	int Ncb; 							// Size fo the circular buffer used for rate matching, see Section 5.4.2, TS 38.212
	double TBS_LBRM;					// Value of TBS_LBRM used to compute Nref, see Section 5.4.2.1, TS 38.212
	int Nref;							// Intermediate value used in the computation of Ncb, see Section 5.4.2.1, TS 38.212
	int puschCBG;						// Parameter used to determine whether CBG based transmission is supported or not, see Section 6.1.5.2, TS 38.214
	int Nl;								// Number of layers the TB is associated with, see Section 5.4.2.1, TS 38.212
	int C;								// Number of codeblocks in the transport block, see Section 5.4.2.1 TS 38.212
	int C_dash;							// Number of codeblocks to be considered- depends on CBGTI
	int newDataIndicator;				// Higher layer parameter used to indicate if the PUSCH transmission is a new transmission (set to 1) or a retransmission (set to 0)
	int G;								// Number of coded bits available for the Transport block, see Section 5.4.2.1, TS 38.212
	int E1, E2, nE1, nE2;				// Number of bits in the output of the rate matching module, see Section 5.4.2.1, TS 38.212
	int K_b;

	for (i = 0; i < Ul_tti_intermediate.nPUSCH; ++i)
	{
		freq_start = (int)Ul_ttiRequest.PUSCH_PDU[i].BWPStart + (int)Ul_ttiRequest.PUSCH_PDU[i].rbStart;
		freq_len = (int)Ul_ttiRequest.PUSCH_PDU[i].rbSize;
		nLayers = (int)Ul_ttiRequest.PUSCH_PDU[i].nrOfLayers;
		time_start = (int)Ul_ttiRequest.PUSCH_PDU[i].StartSymbolIndex;

		TB_size = (int)Ul_ttiRequest.PUSCH_PDU[i].puschData.TBSize*8;
		
		R = (double)Ul_ttiRequest.PUSCH_PDU[i].targetCodeRate;
		Qm = (int)Ul_ttiRequest.PUSCH_PDU[i].qamModOrder;

		if (TB_size > 3824)
			TBcrc_size = TB_size + 24;
		else
			TBcrc_size = TB_size + 16;
		Ul_tti_intermediate.TB_prop[i].TBcrc = (int)TBcrc_size;
		Ul_tti_intermediate.TB_prop[i].rvIndex = Ul_ttiRequest.PUSCH_PDU[i].puschData.rvIndex;
		Ul_tti_intermediate.TB_prop[i].qamModOrder = Qm;
		Ul_tti_intermediate.TB_prop[i].user_id = (uint8_t)i;

		if ( (TB_size <= 292) || (R <= 0.25*10240) || (TB_size <= 3824 && R <= 0.67*10240) ){
			// BG 2
			if (TBcrc_size <= 3840){
				num_CB = 1;
				CB_size = TBcrc_size;
				Ul_tti_intermediate.TB_prop[i].K_dash = (uint16_t)CB_size;
			}
			else{
				num_CB = (int)ceil((double)TBcrc_size/(3840-24));
				CB_size = (int)ceil((double)TBcrc_size/num_CB);
				Ul_tti_intermediate.TB_prop[i].K_dash = (uint16_t)CB_size+24;
			}
			base_graph_index = 2;
			Ul_tti_intermediate.TB_prop[i].base_graph_index = 2;
			// Computing K_b based on the size of B = TB size A + CRC size
			if (TBcrc_size > 640)
				Ul_tti_intermediate.TB_prop[i].K_b = 10;
			else if (TBcrc_size > 560)
				Ul_tti_intermediate.TB_prop[i].K_b = 9;
			else if (TBcrc_size > 192)
				Ul_tti_intermediate.TB_prop[i].K_b = 8;
			else 
				Ul_tti_intermediate.TB_prop[i].K_b = 6;
		}
		else{
			// BG 1
			if (TBcrc_size <= 8448){
				num_CB = 1;
				CB_size = TBcrc_size;
				Ul_tti_intermediate.TB_prop[i].K_dash = (uint16_t)CB_size;
			}
			else{
				num_CB = (int)ceil((double)TBcrc_size/(8424));
				CB_size = (int)ceil((double)TBcrc_size/num_CB);
				Ul_tti_intermediate.TB_prop[i].K_dash = (uint16_t)CB_size+24;
			}
			base_graph_index = 1;
			Ul_tti_intermediate.TB_prop[i].base_graph_index = 1;
			Ul_tti_intermediate.TB_prop[i].K_b = 22; 				// K_b is 22 for BG1
		}
		Ul_tti_intermediate.TB_prop[i].num_CB = (uint8_t)num_CB;
		fprintf(fp,"PDU ID %d , Number of codeblocks %d , Codeblock size %d\n",i,num_CB,CB_size);

		// Computing lifting size index
		lifting_size_index = 0;
		Z_c = (int)LIFTING_SIZE_Z_UL[lifting_size_index];
		while ((int)Ul_tti_intermediate.TB_prop[i].K_b*Z_c < (int)Ul_tti_intermediate.TB_prop[i].K_dash){
			lifting_size_index = lifting_size_index + 1;
			Z_c = (int)LIFTING_SIZE_Z_UL[lifting_size_index];
		}
		Ul_tti_intermediate.TB_prop[i].Z_c = (uint16_t)Z_c;
		Ul_tti_intermediate.TB_prop[i].lifting_size_index = (uint8_t)lifting_size_index;

		K_b = Ul_tti_intermediate.TB_prop[i].K_b;

		// Determining the number of bits in the code block after filler bit addition
		if (base_graph_index == 1)
			K = 22*Z_c;
		else
			K = K_b*Z_c;
		Ul_tti_intermediate.TB_prop[i].K = (uint16_t)K;

		// Computing N from Section 5.3.2, TS 38.212
		if (base_graph_index == 1)
			N = Z_c*66;
		else
			N = Z_c*50;

		// TODO: compute I_LBRM using the higher layer parameter rateMatching
		I_LBRM = 0;						// From Section 6.2.5, TS 38.212, this essentially amounts to rateMatching not being set to limitedBufferRM

		// Computing Ncb from Section 5.4.2.1, TS 38.212
		if (I_LBRM == 0){
			Ncb = N;
		}
		else {
			TBS_LBRM = (double)Ul_tti_intermediate.TB_prop[i].TBS_LBRM;
			// TODO: Remove the use of this dummy TBS_LBRM value
			TBS_LBRM = 100000000;	// Setting to 1e8
			Nref = floor(TBS_LBRM/((double)Ul_tti_intermediate.TB_prop[i].num_CB*(2.0/3.0)));
			if (N <= Nref)
				Ncb = N;
			else
				Ncb = Nref;
		}
		Ul_tti_intermediate.TB_prop[i].N_circular_buffer = Ncb;

		// Computing the value of E
		Nl = nLayers;
		if ((int)Ul_ttiRequest.PUSCH_PDU[i].resourceAlloc == 1){
			G = Nl*(int)Ul_ttiRequest.PUSCH_PDU[i].rbSize*((int)Ul_ttiRequest.PUSCH_PDU[i].NrOfSymbols)*12 - (int)Ul_tti_intermediate.TB_prop[i].reserved_RE;
			G = Qm*G;
			Ul_tti_intermediate.TB_prop[i].G = G;
		}

		// TODO: figure out how to obtain codeBlockGroupTransmission in PUSCH-ServingCellConfig as mentioned in Section 6.1.5.2, TS 38.214
		puschCBG = 0;				// All codeblocks are to be considered

		C = num_CB;
		newDataIndicator = (int)Ul_ttiRequest.PUSCH_PDU[i].puschData.newDataIndicator;

		// Obtaining C_dash according to Section 5.4.2.1, TS 38.212 
		// TODO: figure out how to determine if CBGTI is present in the DCI scheduling the TB or not
		if (puschCBG == 0){
			C_dash = C;
		}
		else {
			if (newDataIndicator == 1){
				C_dash = C;
			}
			else {
				C_dash = 0;
				for (i = 0; i < ceil((double)C/8); i++){
					for (j = 0; j < 8; j++){
						if (((int)Ul_ttiRequest.PUSCH_PDU[i].puschData.cbPresentAndPosition[i])&(1<<j)){
							C_dash = C_dash + 1;
						}
					}
				}
			}
		}

		// Calculate rate-matched output size
		E1 = Nl*Qm*floor((double)G/((double)(Nl*Qm*C_dash)));
		E2 = Nl*Qm*ceil((double)G/((double)(Nl*Qm*C_dash)));
		nE1 = C_dash - ((G/(Nl*Qm))%C_dash);
		nE2 = C_dash - nE1;

		// SORT CB
		Ul_tti_intermediate.TB_prop[i].numLayers = (uint8_t)nLayers;
		Ul_tti_intermediate.TB_prop[i].start_CB_idx = Ul_tti_intermediate.nCB;

		int CB_numRE_layer = 0;
		int cb_symb_loc = time_start;
		int cb_freq_loc = 0;
		int dmrs_locs = Ul_ttiRequest.PUSCH_PDU[i].ulDmrsSymbPos;
		int num_dmrs_RB = (6-2*Ul_ttiRequest.PUSCH_PDU[i].dmrsConfigType)*Ul_ttiRequest.PUSCH_PDU[i].numDmrsCdmGrpsNoData;
		for (int k = 0; k < num_CB; ++k){
			if ((puschCBG == 1) && (newDataIndicator == 0) && (((int)Ul_ttiRequest.PUSCH_PDU[i].puschData.cbPresentAndPosition[k/8])&(1<<(7-(k%8)))) ){
				Ul_tti_intermediate.TB_prop[i].E[k] = 0;
				continue;
			}
			else if (k < nE1){
				Ul_tti_intermediate.TB_prop[i].E[k] = (uint16_t)E1;
				CB_numRE_layer = E1/(Qm*Nl);
			}
			else{
				Ul_tti_intermediate.TB_prop[i].E[k] = (uint16_t)E2;
				CB_numRE_layer = E2/(Qm*Nl);
			}

			for (int j = cb_symb_loc; j < 14; ++j){
				if (dmrs_locs & (1<<j)){
					if (CB_numRE_layer < (12 - num_dmrs_RB)*freq_len - cb_freq_loc){
						cb_freq_loc += CB_numRE_layer*12/(12-num_dmrs_RB);
						break;
					}
					else{
						CB_numRE_layer -= ((12 - num_dmrs_RB)*freq_len - cb_freq_loc);
						cb_freq_loc = 0;
						cb_symb_loc += 1;
					}
				}
				else{
					if (CB_numRE_layer < 12*freq_len - cb_freq_loc){
						cb_freq_loc += CB_numRE_layer;
						break;
					}
					else{
						CB_numRE_layer -= (12*freq_len - cb_freq_loc);
						cb_freq_loc = 0;
						cb_symb_loc += 1;
					}
				}
			}

			CB_prop[Ul_tti_intermediate.nCB].cb_end_time = cb_symb_loc -1*(int)(cb_freq_loc==0);
			CB_prop[Ul_tti_intermediate.nCB].cb_end_freq = freq_start + cb_freq_loc + 12*freq_len*(int)(cb_freq_loc==0);
			CB_prop[Ul_tti_intermediate.nCB].cb_size = CB_size;
			CB_prop[Ul_tti_intermediate.nCB].TB_idx = i;
			CB_prop[Ul_tti_intermediate.nCB].CB_in_TB = k;
			CB_prop[Ul_tti_intermediate.nCB].layer_info = Ul_ttiRequest.PUSCH_PDU[i].dmrsPorts;
			++Ul_tti_intermediate.nCB;
		}
	}
	Ul_tti_intermediate.nTB = Ul_tti_intermediate.nPUSCH;

	// Sort the CBs according to the time-frequency order using merge sort
	for (i = 0; i < Ul_tti_intermediate.nCB; ++i)
		sorted_CB_idx[i] = i;

	mergesort_UL(0,Ul_tti_intermediate.nCB-1);
	// Bubble sort
	// int cur_index,next_index;
	// for (i = 0; i < Ul_tti_intermediate.nCB - 1; ++i){
	// 	for (j = 0; j < Ul_tti_intermediate.nCB - i - 1; ++j){
	// 		cur_index = sorted_CB_idx[j];
	// 		next_index = sorted_CB_idx[j+1];
	// 		if (CB_prop[cur_index].cb_end_time > CB_prop[next_index].cb_end_time || CB_prop[cur_index].cb_end_time == CB_prop[next_index].cb_end_time && CB_prop[cur_index].cb_end_freq > CB_prop[next_index].cb_end_freq){
	// 			sorted_CB_idx[j] = next_index;
	// 			sorted_CB_idx[j+1] = cur_index;
	// 		}
	// 	}
	// }

	// Write CB address in BRAM
	int all_cb_size = 0;
	for (int i = 0; i < Ul_tti_intermediate.nCB; ++i){
		int idx = sorted_CB_idx[i];
		CB_prop[idx].cb_start_addr_loc = all_cb_size;
		all_cb_size += ceil((double)CB_prop[idx].cb_size/128);
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CBS_WRITE<<56) + (uint64_t)n_sf + (uint64_t)((uint64_t)all_cb_size<<5);
	fprintf(fp, "\nHardware packet for CBS write is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	fprintf(fp, "Slot number : %d\n", n_sf);
	fprintf(fp, "Sum total of CB length : %d\n", all_cb_size);

	// Read CB address in BRAM
	int is_last_cb, start_pckt_addr, len_bits;
	for (i = 0; i < Ul_tti_intermediate.nPUSCH; ++i){
		idx = sorted_PUSCH_idx[i];
		for (j = 0; j < Ul_tti_intermediate.TB_prop[idx].num_CB; ++j){
			
			start_pckt_addr = CB_prop[Ul_tti_intermediate.TB_prop[idx].start_CB_idx+j].cb_start_addr_loc;
			len_bits = CB_prop[Ul_tti_intermediate.TB_prop[idx].start_CB_idx+j].cb_size;
			printf("CB size is %d, cb index is %d,idx = %d",len_bits ,Ul_tti_intermediate.TB_prop[idx].start_CB_idx+j,idx);
			is_last_cb = (j+1 == Ul_tti_intermediate.TB_prop[idx].num_CB)?1:0;

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CBS_READ<<56) + (uint64_t)start_pckt_addr + (uint64_t)((uint64_t)is_last_cb<<15) + (uint64_t)((uint64_t)len_bits<<16);
			fprintf(fp, "\nHardware packet for CBS read is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "TB/User index : %d\n", idx);
			fprintf(fp, "Codeblock index in TB : %d\n", j);
			fprintf(fp, "CB length in bits : %d\n", len_bits);
			fprintf(fp, "CB start address in packets : %d\n", start_pckt_addr);
			fprintf(fp, "Is the CB the last for the TB : %d\n", is_last_cb);
		}
	}
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CBS_READ<<56) + (uint64_t)((uint64_t)1<<30);
	fprintf(fp, "\nHardware packet for CBS read is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "End of slot, dummy config\n");
}





/*
Function: generate_dmrs_config_UL
Author: Aniruddh V

	Function to generate the configs for DMRS. Also calculates the REs reserved for DMRS parameter generation is based on TS 38.211;
	The required parameters are generated for each PUSCH PDU.

	Inputs:
		PDU_idx (int): This index tells the PUSCH PDU for which DMRS is generated
		Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		The start and length RB, nscid and scrambling ID, ports and OFDM symbols occupied, configuration

*/

void generate_dmrs_config_UL(int PDU_idx){
	int start_RB , num_RB , dmrs_type , n_SCID , layer_present , scrambling_ID , dmrs_sym_pos , n_sf , l_prime=0, beta_index;

	start_RB = Ul_ttiRequest.PUSCH_PDU[PDU_idx].BWPStart + Ul_ttiRequest.PUSCH_PDU[PDU_idx].rbStart;
	num_RB = Ul_ttiRequest.PUSCH_PDU[PDU_idx].rbSize;
	dmrs_sym_pos = Ul_ttiRequest.PUSCH_PDU[PDU_idx].ulDmrsSymbPos;
	dmrs_type = 1 - Ul_ttiRequest.PUSCH_PDU[PDU_idx].dmrsConfigType;			// DMRS type 1 is represented by '0' in FAPI, '1' in FPGA
	n_SCID = Ul_ttiRequest.PUSCH_PDU[PDU_idx].SCID;
	scrambling_ID = Ul_ttiRequest.PUSCH_PDU[PDU_idx].ulDmrsScramblingId;
	n_sf = Ul_ttiRequest.Slot;
	layer_present = Ul_ttiRequest.PUSCH_PDU[PDU_idx].dmrsPorts;
	int a = dmrs_sym_pos , dmrs_sym_count = 0;
	for(int i = 0 ; i < 14 ; ++i){
		if (a%4 == 3)
			l_prime += (int)(1<<(1+i));
		dmrs_sym_count += a%2;
		a /= 2;
	}

	beta_index = 0; // if it does not go to any loop, just take value '0' which means 0 db 
	// Changes done for beta_index
	if( Ul_ttiRequest.PUSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData == 1 ){
		beta_index = 0;	
	}
	else if(Ul_ttiRequest.PUSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData == 2 ){
		beta_index = 1;
	}
	else if ( (Ul_ttiRequest.PUSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData == 3) && (Ul_ttiRequest.PUSCH_PDU[PDU_idx].dmrsConfigType == 1) ){
		beta_index = 2;
	}
	//

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DMRS<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9) + (uint64_t)((uint64_t)dmrs_type<<18) + (uint64_t)((uint64_t)n_SCID<<19) + (uint64_t)((uint64_t)layer_present<<20);
	fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DMRS<<56) + (uint64_t)scrambling_ID + (uint64_t)((uint64_t)dmrs_sym_pos<<16) ;
	fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DMRS<<56) + (uint64_t)l_prime + (uint64_t)((uint64_t)n_sf<<14) + (uint64_t)((uint64_t)beta_index<<23);
	fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	fprintf(fp, "PDU index : %d \n", PDU_idx);
	fprintf(fp, "Starting RB is : %d \n", start_RB);
	fprintf(fp, "PRB length is : %d \n", num_RB);
	fprintf(fp, "DMRS configuration type is : %d \n", dmrs_type);
	fprintf(fp, "n_scid is : %d \n", n_SCID);
	fprintf(fp, "The layers which are present are : 0X%04X \n", layer_present);
	fprintf(fp, "DMRS scrambling ID is : %d \n", scrambling_ID);
	fprintf(fp, "DMRS symbols are : 0X%04X \n", dmrs_sym_pos);
	fprintf(fp, "The value of l_prime is : 0X%04X \n", l_prime);
	fprintf(fp, "Slot number in the frame is : %d \n", n_sf);
	fprintf(fp, "Beta Index is : %d \n", beta_index);

	// Update the reserved REs
	int G;
	if (dmrs_type == 1)
		// Type 1 config
		G = num_RB*dmrs_sym_count*(Ul_ttiRequest.PUSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData)*6;
	else
		// Type 2 config
		G = num_RB*dmrs_sym_count*(Ul_ttiRequest.PUSCH_PDU[PDU_idx].numDmrsCdmGrpsNoData)*4;

	Ul_tti_intermediate.TB_prop[PDU_idx].reserved_RE = G*Ul_ttiRequest.PUSCH_PDU[PDU_idx].nrOfLayers;
	fprintf(fp, "The reserved REs for TB %d by DMRS is %d\n", PDU_idx, Ul_tti_intermediate.TB_prop[PDU_idx].reserved_RE);

}





/*
Function: generate_ch_eq_config
Author: Aniruddh V

	Function to generate the configs for the channel estimation and equalization block;
	The HW module assumes that all users have the same number of DMRS occasions (only 1 additional position is supported).

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest 

	Output (to file):
		Total number of DMRS REs (first occasion), Total number of DMRS REs (first additional position)
		Total number of PUSCH REs uptil the next DMRS occasion (repeated for each additional position)
		This function assumes that SRS will be present only after the last DMRS occasion

*/

void generate_ch_eq_config(){
	int num_dmrs_loc[4] = {0,0,0,0};
	int num_pusch_loc[5] = {0,0,0,0,0};
	int dmrs_loc_common[4] = {14,14,14,14};

	for (int i = 0; i < Ul_tti_intermediate.nPUSCH; ++i){
		int a = (int)(Ul_ttiRequest.PUSCH_PDU[i].ulDmrsSymbPos);
		int dmrs_index = 0;
		int symb_index = 0;

		while(a>0){
			if (a%2 == 1){
				if (dmrs_loc_common[dmrs_index] > symb_index)
					dmrs_loc_common[dmrs_index] = symb_index;
				++dmrs_index;
				a /= 4;
				symb_index += 2;
			}
			else{
				a /= 2;
				symb_index += 1;
			}
		}
	}

	for (int i = 0; i < Ul_tti_intermediate.nPUSCH; ++i){
		PUSCH_PDU_t cur_pusch = Ul_ttiRequest.PUSCH_PDU[i];

		int a = (int)(cur_pusch.ulDmrsSymbPos >> cur_pusch.StartSymbolIndex);
		int loc_num = 0, num_data = 0;
		int num_dmrs_layers = Ul_tti_intermediate.BWP_prop[Ul_tti_intermediate.TB_prop[i].BWP_idx].numLayers;

		int dmrs_common_index = 0;
		for (int j = cur_pusch.StartSymbolIndex; j < cur_pusch.StartSymbolIndex+cur_pusch.NrOfSymbols; ++j){
			if (j >= dmrs_loc_common[dmrs_common_index]){
				num_pusch_loc[dmrs_common_index] += num_data*cur_pusch.rbSize;
				num_data = 0;
				++dmrs_common_index;
			}

			if (a & (1<<j)){
				if (cur_pusch.dmrsConfigType == 0)
					num_data += 12-6*cur_pusch.numDmrsCdmGrpsNoData;
				else
					num_data += 12-4*cur_pusch.numDmrsCdmGrpsNoData;
			}
			else
				num_data += 12;
		}
		num_pusch_loc[dmrs_common_index] += num_data*cur_pusch.rbSize;

		while (a > 0){
			if (a%2 == 0){
				a /= 2;
				continue;
			}

			if (a%4 == 3){
				if (cur_pusch.dmrsConfigType == 0)
					num_dmrs_loc[loc_num] += 2*6*(1+(int)(num_dmrs_layers>2))*cur_pusch.rbSize;
				else
					num_dmrs_loc[loc_num] += 2*4*(1+(int)(num_dmrs_layers>4))*cur_pusch.rbSize;
				a /= 4;
				++loc_num;
			}
			else{
				if (cur_pusch.dmrsConfigType == 0)
					num_dmrs_loc[loc_num] += 6*(1+(int)(num_dmrs_layers>2))*cur_pusch.rbSize;
				else
					num_dmrs_loc[loc_num] += 4*(1+(int)(num_dmrs_layers>4))*cur_pusch.rbSize;
				a /= 2;
				++loc_num;
			}
		}
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CH_EQ<<56) + (uint64_t)num_dmrs_loc[0] + (uint64_t)((uint64_t)(num_pusch_loc[0]+num_pusch_loc[1])<<16);
	fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CH_EQ<<56) + (uint64_t)num_dmrs_loc[1] + (uint64_t)((uint64_t)num_pusch_loc[2]<<16);
	fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CH_EQ<<56) + (uint64_t)num_dmrs_loc[2] + (uint64_t)((uint64_t)num_pusch_loc[3]<<16);
	fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);


	fprintf(fp, "Number of DMRS location at the first occasion : %d \n", num_dmrs_loc[0]);
	fprintf(fp, "Number of DMRS location at the second occasion : %d \n", num_dmrs_loc[1]);
	fprintf(fp, "Number of DMRS location at the third occasion : %d \n", num_dmrs_loc[2]);
	fprintf(fp, "Number of PUSCH location TILL the second DMRS : %d \n", num_pusch_loc[0]+num_pusch_loc[1]);
	fprintf(fp, "Number of PUSCH location TILL the third DMRS : %d \n", num_pusch_loc[2]);
	fprintf(fp, "Number of PUSCH location TILL the fourth DMRS : %d \n", num_pusch_loc[3]);

}





/*
Function: generate_UE_interlerleave_config
Author: Aniruddh V

	Function to generate the configs for the UE interleaver and layer demapper.
	REs from every OFDM symbol is written into the BRAM in a discontinuous manner to group all users together.
	The REs are later read from the BRAM in the codeblock order (whichever codeblock is completed first)
	As the configs are common for each layer, the layer demapper configs mention which BRAM has the data for which user.

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		Total number of DMRS REs (first occasion), Total number of DMRS REs (first additional position)
		Total number of PUSCH REs uptil the next DMRS occasion (repeated for each additional position)
		This function assumes that SRS will NOT be present

*/

void generate_UE_interleave_config(){
	int user_idx[MaxUE], user_start_addr_write[MaxUE], user_cur_write[MaxUE], num_idx=0, num_writes=0, start_addr=0;
	int user_BWP_mapping[MaxUE], layers_occupied_per_BWP[MaxUE], num_layers_per_BWP;

	for (int i = 0; i < Ul_tti_intermediate.nPUSCH; ++i){
		int cur_idx = sorted_PUSCH_idx[i];
		if (Ul_ttiRequest.PUSCH_PDU[cur_idx].dmrsPorts %2 == 1){
			user_start_addr_write[num_idx] = start_addr;
			start_addr += Ul_tti_intermediate.TB_prop[cur_idx].G / (Ul_tti_intermediate.TB_prop[cur_idx].numLayers*Ul_tti_intermediate.TB_prop[cur_idx].qamModOrder);
			user_idx[num_idx] = cur_idx;
			num_writes += Ul_ttiRequest.PUSCH_PDU[cur_idx].NrOfSymbols;
			++num_idx;

			user_BWP_mapping[i] = num_idx-1;
			layers_occupied_per_BWP[num_idx-1] = Ul_tti_intermediate.TB_prop[cur_idx].numLayers;
		}
		else{
			user_BWP_mapping[i] = num_idx-1;
			layers_occupied_per_BWP[num_idx-1] += Ul_tti_intermediate.TB_prop[cur_idx].numLayers;
		}
	}

	// WRITE configs
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_UE_INTERLEAVE_WRITE<<56) + (uint64_t)num_writes;
	fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Number of write configs are : %d\n", num_writes);

	for (int i = 0; i < num_idx; ++i){
		user_cur_write[i] = user_start_addr_write[i];
	}
	int num_writes_per_symb[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	num_writes = 0;
	for (int l = 0; l < 14; ++l){
		for (int i = 0; i < num_idx; ++i){
			PUSCH_PDU_t cur_pusch = Ul_ttiRequest.PUSCH_PDU[user_idx[i]];

			if (l < cur_pusch.StartSymbolIndex || l > cur_pusch.StartSymbolIndex + cur_pusch.NrOfSymbols -1){
				continue;
			}

			start_addr = user_cur_write[i];
			int num_addr = 12*cur_pusch.rbSize;

			// Reduce the pusch location if DMRS is present (similar logic must be followed for SRS)
			if ((int)(cur_pusch.ulDmrsSymbPos >> l)%2 == 1){
				if (cur_pusch.dmrsConfigType == 0){
					num_addr -= 6*cur_pusch.numDmrsCdmGrpsNoData*cur_pusch.rbSize;
				}
				else{
					num_addr -= 4*cur_pusch.numDmrsCdmGrpsNoData*cur_pusch.rbSize;
				}
			}

			num_writes_per_symb[l] += num_addr;
			user_cur_write[i] = start_addr + num_addr;
			++num_writes;

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_UE_INTERLEAVE_WRITE<<56) + (uint64_t)(start_addr+num_addr) + (uint64_t)((uint64_t)start_addr<<16);
			fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			fprintf(fp, "Start address for write number %d is : %d\n", num_writes, start_addr);
			fprintf(fp, "End address for write number %d is : %d\n", num_writes, start_addr+num_addr);
			fprintf(fp, "OFDM Symbol number is : %d\n", l);
			fprintf(fp, "BWP/User number is : %d\n", i);
		}
	}

	// READ and Layer Demapping configs
	int Layer_num_reads[8] = {0,0,0,0,0,0,0,0};
	int num_read_addr = HW_OP_idx;
	HW_OP_idx += 4;

	for (int i = 0; i < num_idx; ++i){
		user_cur_write[i] = user_start_addr_write[i];
	}
	for (int i = 0; i < Ul_tti_intermediate.nCB; ++i){
		int cur_idx = sorted_CB_idx[i];
		int layer_info = CB_prop[cur_idx].layer_info;
		int layer1_data = layer_info%2;
		int cur_BWP = user_BWP_mapping[CB_prop[cur_idx].TB_idx];

		int read_addr, num_addr, write_counter;

		read_addr = user_cur_write[cur_BWP];
		num_addr = CB_prop[cur_idx].E_per_layer;
		user_cur_write[cur_BWP] += num_addr;

		write_counter = 0;
		for (int k = 0; k <= CB_prop[cur_idx].cb_end_time; ++k){
			write_counter += num_writes_per_symb[k];
		}

		for (int j = 0; j < 4; ++j){
			if (layer_info%2 == 1){
				Layer_num_reads[j]++;

				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)(ADDRESS_RX_UE_INTERLEAVE_READ_L1+j)<<56) + (uint64_t)(num_addr) + (uint64_t)((uint64_t)read_addr<<16);
				fprintf(fp, "\nHardware packet 1 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)(ADDRESS_RX_UE_INTERLEAVE_READ_L1+j)<<56) + (uint64_t)(write_counter);
				fprintf(fp, "Hardware packet 2 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				fprintf(fp, "Start address for read for CB number %d in layer %d is : %d\n", i, j, read_addr);
				fprintf(fp, "Number address for read for CB number %d in layer %d is : %d\n", i, j, num_addr);
				fprintf(fp, "Write counter for read for CB number %d in layer %d is : %d\n", i, j, write_counter);
				fprintf(fp, "Last OFDM symbol for CB number %d in layer %d is : %d\n", i, j, CB_prop[cur_idx].cb_end_time);
				fprintf(fp, "CB number %d of user %d at cb_in_tb %d\n", i, CB_prop[cur_idx].TB_idx, CB_prop[cur_idx].CB_in_TB);
			}
			layer_info /= 2;
		}
		if (layer1_data == 1){ 		// Remove empty data from empty layers for the CB which occupies the first layer
			for (int j = layers_occupied_per_BWP[cur_BWP]; j < 4; ++j){
				Layer_num_reads[j]++;

				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)(ADDRESS_RX_UE_INTERLEAVE_READ_L1+j)<<56) + (uint64_t)(num_addr) + (uint64_t)((uint64_t)read_addr<<16);
				fprintf(fp, "\nHardware packet 1 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)(ADDRESS_RX_UE_INTERLEAVE_READ_L1+j)<<56) + (uint64_t)(write_counter);
				fprintf(fp, "Hardware packet 2 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				fprintf(fp, "Start address for DISCARD read for CB number %d in layer %d is : %d\n", i, j, read_addr);
				fprintf(fp, "Number address for DISCARD read for CB number %d in layer %d is : %d\n", i, j, num_addr);
				fprintf(fp, "Write counter for DISCARD read for CB number %d in layer %d is : %d\n", i, j, write_counter);
			}
		}

		// Layer demap config
		int layer_map[8] = {15,15,15,15,15,15,15,15};
		int user_layer_map[8] = {0,0,0,0,0,0,0,0};
		layer_info = CB_prop[cur_idx].layer_info;
		for (int j = 0; j < 8; ++j){
			if (layer_info%2 == 1){
				layer_map[j] = 1;
				user_layer_map[j] = CB_prop[cur_idx].TB_idx;
			}
			if (layer1_data == 1 && j>layers_occupied_per_BWP[cur_BWP]-1){
				layer_map[j] = 0;
			}
			layer_info /= 2;
		}

		HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_LAYER_MAPPER<<56);
		for (int j = 0; j < 8; ++j){
			HW_OP[HW_OP_idx] = HW_OP[HW_OP_idx] + (uint64_t)((uint64_t)layer_map[j]<<4*j);
		}
		HW_OP_idx++;
		fprintf(fp, "\nHardware packet 1 for layer-demap is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_LAYER_MAPPER<<56);
		for (int j = 0; j < 4; ++j){
			HW_OP[HW_OP_idx] = HW_OP[HW_OP_idx] + (uint64_t)((uint64_t)user_layer_map[j]<<6*j);
		}
		HW_OP_idx++;
		fprintf(fp, "Hardware packet 2 for layer-demap is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_LAYER_MAPPER<<56);
		for (int j = 0; j < 4; ++j){
			HW_OP[HW_OP_idx] = HW_OP[HW_OP_idx] + (uint64_t)((uint64_t)user_layer_map[4+j]<<6*j);
		}
		HW_OP_idx++;
		fprintf(fp, "Hardware packet 3 for layer-demap is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		for (int j = 0; j < 8; ++j){
			fprintf(fp, "Layer Map for CB number %d in layer %d is : %d\n", i, j, layer_map[j]);
		}
		for (int j = 0; j < 8; ++j){
			fprintf(fp, "User ID for CB number %d in layer %d is : %d\n", i, j, user_layer_map[j]);
		}
	}

	for (int i = 0; i < 4; ++i){
		HW_OP[num_read_addr+i] = (uint64_t)((uint64_t)(ADDRESS_RX_UE_INTERLEAVE_READ_L1+i)<<56) + (uint64_t)Layer_num_reads[i];
		fprintf(fp, "\nHardware packet for read config is : 0X%lX\n", HW_OP[num_read_addr+i]);
		fprintf(fp, "Total number of read configs for layer %d are : %d\n", i, Layer_num_reads[i]);
	}

}





/*
Function: generate_CBCRC_config
Author: Aniruddh V

	Function to generate the configs for the CB CRC checking

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		CB size with the polynomial type for each CB

*/

void generate_CBCRC_config(){

	for (int i = 0; i < Ul_tti_intermediate.nCB; ++i){
		int idx = sorted_CB_idx[i];
		int is_crc = (Ul_tti_intermediate.TB_prop[CB_prop[idx].TB_idx].num_CB > 1)?1:3;
		int l1_r = (CB_prop[sorted_CB_idx[i]].cb_size+24*(int)(is_crc==1)) %128;
		int l1_k = (CB_prop[sorted_CB_idx[i]].cb_size+24*(int)(is_crc==1)) /128;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_CBCRC<<56) + (uint64_t)l1_r + (uint64_t)((uint64_t)l1_k<<8) + (uint64_t)((uint64_t)is_crc<<24);
		fprintf(fp, "\nHardware packet for CB CRC for CB %d is : 0X%lX\n", sorted_CB_idx[i], HW_OP[HW_OP_idx-1]);
		fprintf(fp, "l1_r is : %d\n", l1_r);
		fprintf(fp, "l1_k is : %d\n", l1_k);
		fprintf(fp, "l1_poly_type is : %d\n", is_crc);
	}
}





/*
Function: generate_TBCRC_config
Author: Aniruddh V

	Function to generate the configs for the TB CRC checking

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		TB size with the polynomial type for each TB

*/

void generate_TBCRC_config(){

	for (int i = 0; i < Ul_tti_intermediate.nPUSCH; ++i){
		int l1_r = Ul_tti_intermediate.TB_prop[sorted_PUSCH_idx[i]].TBcrc %128;
		int l1_k = Ul_tti_intermediate.TB_prop[sorted_PUSCH_idx[i]].TBcrc /128;
		int l1_poly_type = ((Ul_tti_intermediate.TB_prop[sorted_PUSCH_idx[i]].TBcrc > 3824+24)?0:2);
		int l1_user_no= Ul_tti_intermediate.TB_prop[sorted_PUSCH_idx[i]].user_id;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_TBCRC<<56) + (uint64_t)l1_r + (uint64_t)((uint64_t)l1_k<<8) + (uint64_t)((uint64_t)l1_poly_type<<24)+(uint64_t)((uint64_t)l1_user_no<<27);
		fprintf(fp, "\nHardware packet for TB CRC for user %d is : 0X%lX\n", sorted_PUSCH_idx[i], HW_OP[HW_OP_idx-1]);

		fprintf(fp, "l1_r is : %d\n", l1_r);
		fprintf(fp, "l1_k is : %d\n", l1_k);
		fprintf(fp, "l1_poly_type is : %d\n", l1_poly_type);
		fprintf(fp, "l1_user_no is : %d\n", l1_user_no);
	}
}


void generate_pucch_pseudorandom_seq(int cinit){


	//initialize x1 and x2 to zeros
	for (int i = 0; i < 31; i++){
		pucch_x1[i] = 0;
		pucch_x2[i] = ( ((uint32_t)cinit & (uint32_t)(1<<i)) != (uint32_t)0 );
	}
	pucch_x1[0] = 1;

	for (int n = 0;  n < 4000-31; n++){
		pucch_x1[n + 31] = (pucch_x1[n + 3] + pucch_x1[n])%2;
    		pucch_x2[n + 31] = (pucch_x2[n + 3] + pucch_x2[n + 2] + pucch_x2[n + 1] + pucch_x2[n])%2;
	}

	pucch_pr_seq_precomputed = 1;
}


/*
Function: generate_pucchF0_config
Author: Aniruddh V

	Function to generate the configs for PUCCH F0 data

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		Sequence indices and data length
*/

void generate_pucchF0_config(){
	int i = 0;
	while (i < Ul_tti_intermediate.nPUCCH){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];
		if (i==0 && (pucch_pr_seq_precomputed == 0)){
			generate_pucch_pseudorandom_seq(cur_pucch.hoppingId);
		}
		if (cur_pucch.FormatType != 0){
			++i;
			continue;
		}



		int u = cur_pucch.hoppingId%30;
		HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0<<56);
		for (int j = 0; j < 12; ++j){
			HW_OP[HW_OP_idx] = HW_OP[HW_OP_idx] + (uint64_t)((uint64_t)(((low_PAPR_table_12[u][j]+7)/2)%4)<<(2*j));
		}
		++HW_OP_idx;
		fprintf(fp, "\nHardware packet1 for PUCCHF0 for PDU idx %d is : 0X%lX\n", sorted_PUCCH_idx[i], HW_OP[HW_OP_idx-1]);
		int num_users, num_symb, M11, M12, user0_format, user1_format, user0_id, user1_id;
		printf("i: %d and nPUCCH: %d\n", i, Ul_tti_intermediate.nPUCCH);

		if (i < Ul_tti_intermediate.nPUCCH - 1){ // if not the last PDU
			PUCCH_PDU_t next_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i+1]];
			if (next_pucch.FormatType == 0 && next_pucch.StartSymbolIndex == cur_pucch.StartSymbolIndex && next_pucch.BWPStart+next_pucch.prbStart == cur_pucch.BWPStart+cur_pucch.prbStart){
				// 2 users
				printf("2 users detected\n");
				printf("Slot: %d, CS1: %d, CS2: %d\n", Ul_ttiRequest.Slot, cur_pucch.InitialCyclicShift, next_pucch.InitialCyclicShift);
				num_users = 2;

				M11 = cur_pucch.InitialCyclicShift;

				int tmp_pr_index = (8*(14*Ul_ttiRequest.Slot+cur_pucch.StartSymbolIndex))+1600;
				for (int j = 0; j < 8; ++j){
					M11 += (int)(1<<j) * ((pucch_x1[tmp_pr_index+j]+pucch_x2[tmp_pr_index+j])%2);
				}

				M12 = M11 - cur_pucch.InitialCyclicShift + next_pucch.InitialCyclicShift;
				M11 = M11%12;

				M12 = M12%12;
				printf("M11 is: %d, M12 is: %d\n", M11, M12);
				user0_format = cur_pucch.SRFlag*3 + cur_pucch.BitLenHarq - 1;
				user1_format = next_pucch.SRFlag*3 + next_pucch.BitLenHarq - 1;
				user0_id = cur_pucch.RNTI;
				user1_id = next_pucch.RNTI;

				i += 2;
			}
			else{
				// 1 user
				num_users = 1;

				M11 = cur_pucch.InitialCyclicShift;

				int tmp_pr_index = (8*(14*Ul_ttiRequest.Slot+cur_pucch.StartSymbolIndex))+1600;
				for (int j = 0; j < 8; ++j){
					M11 += (int)(1<<j) * ((pucch_x1[tmp_pr_index+j]+pucch_x2[tmp_pr_index+j])%2);
				}
				M11 = M11%12;

				M12 = 0;

				user0_format = cur_pucch.SRFlag*3 + cur_pucch.BitLenHarq - 1;
				user1_format = 0;
				user0_id = cur_pucch.RNTI;
				user1_id = 0;

				i += 1;
			}
		}
		else{

			// 1 user
			num_users = 1;

			M11 = cur_pucch.InitialCyclicShift;

			int tmp_pr_index = (8*(14*Ul_ttiRequest.Slot+cur_pucch.StartSymbolIndex))+1600;
			for (int j = 0; j < 8; ++j){
				M11 += (int)(1<<j) * ((pucch_x1[tmp_pr_index+j]+pucch_x2[tmp_pr_index+j])%2);
			}
			M11 = M11%12;

			M12 = 0;

			user0_format = cur_pucch.SRFlag*3 + cur_pucch.BitLenHarq - 1;
			user1_format = 0;
			user0_id = cur_pucch.RNTI;
			user1_id = 0;

			i += 1;
		}
		num_symb = cur_pucch.NrOfSymbols;					// cur_pucch.NrOfSymbols;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0<<56) + (uint64_t)(num_users-1) + (uint64_t)((uint64_t)(num_symb-1)<<1) + (uint64_t)((uint64_t)M11<<2) + (uint64_t)((uint64_t)M12<<6) + (uint64_t)((uint64_t)user0_format<<10) + (uint64_t)((uint64_t)user1_format<<13) + (uint64_t)((uint64_t)user0_id<<16) + (uint64_t)((uint64_t)user1_id<<21);
		fprintf(fp, "Hardware packet2 for PUCCHF0 for PDU idx %d is : 0X%lX\n", sorted_PUCCH_idx[i], HW_OP[HW_OP_idx-1]);

		fprintf(fp, "Number of users is : %d\n", num_users);
		fprintf(fp, "Number of symbols is : %d\n", num_symb);
		fprintf(fp, "M11 is : %d\n", M11);
		fprintf(fp, "M12 is : %d\n", M12);
		fprintf(fp, "SR/HARQ format for user0 is : %d\n", user0_format);
		fprintf(fp, "SR/HARQ format for user1 is : %d\n", user1_format);
		fprintf(fp, "ID for user0 is : %d\n", user0_id);
		fprintf(fp, "ID for user1 is : %d\n", user1_id);
	}
}




/*
Function: generate_pucchF0_UE_interlerleave_config
Author: Aniruddh V

	Function to generate the configs for the UE interleaver for PUCCHF0.
	REs from every OFDM symbol is written into the BRAM in a discontinuous manner to group all users together.
	The REs are later read from the BRAM in continuous manner

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):

*/

void generate_pucchF0_UE_interleave_config(){
	int num_writes=0, start_addr=0, i, num_users=0, user_cur_write[MaxUE], num_addr;

	// WRITE configs
	int cur_addr = HW_OP_idx++;
	for (i = 0; i < Ul_tti_intermediate.nPUCCH; ++i)
		user_cur_write[i] = 24*i;

	int num_writes_per_symb[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int l = 0; l < 14; ++l){
		i = 0;
		while(i < Ul_tti_intermediate.nPUCCH){
			PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_DMRS_idx[i]];
			if (cur_pucch.FormatType != 0){
				++i;
				continue;
			}

			if (l < cur_pucch.StartSymbolIndex || l > cur_pucch.StartSymbolIndex + cur_pucch.NrOfSymbols -1){
				++i;
				continue;
			}

			start_addr = user_cur_write[i];
			num_addr = 12;
			num_writes_per_symb[l] += num_addr;
			user_cur_write[i] = start_addr + num_addr;
			++num_writes;

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0_INTERLEAVE_WRITE<<56) + (uint64_t)(start_addr+num_addr) + (uint64_t)((uint64_t)start_addr<<16);
			fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			fprintf(fp, "Start address for write number %d is : %d\n", num_writes, start_addr);
			fprintf(fp, "End address for write number %d is : %d\n", num_writes, start_addr+num_addr);
			fprintf(fp, "OFDM Symbol number is : %d\n", l);

			if (i == Ul_tti_intermediate.nPUCCH -1)
				break;

			PUCCH_PDU_t next_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i+1]];
			if (next_pucch.FormatType == 0 && next_pucch.StartSymbolIndex == cur_pucch.StartSymbolIndex && next_pucch.BWPStart+next_pucch.prbStart == cur_pucch.BWPStart+cur_pucch.prbStart)
				i += 2;
			else
				i += 1;
		}
	}

	HW_OP[cur_addr] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0_INTERLEAVE_WRITE<<56) + (uint64_t)num_writes;
	fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[cur_addr]);
	fprintf(fp, "Number of write configs are : %d\n", num_writes);

	// READ configs
	cur_addr = HW_OP_idx++;
	i = 0;
	while(i < Ul_tti_intermediate.nPUCCH){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];
		if (cur_pucch.FormatType != 0){
			++i;
			continue;
		}
		int read_addr, num_addr, write_counter;
		++num_users;

		num_addr = 12*cur_pucch.NrOfSymbols;
		read_addr = user_cur_write[i] - num_addr;

		write_counter = 0;
		for (int k = 0; k < cur_pucch.StartSymbolIndex+cur_pucch.NrOfSymbols; ++k)
			write_counter += num_writes_per_symb[k];

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0_INTERLEAVE_READ<<56) + (uint64_t)(num_addr) + (uint64_t)((uint64_t)read_addr<<16);
		fprintf(fp, "\nHardware packet 1 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0_INTERLEAVE_READ<<56) + (uint64_t)(write_counter);
		fprintf(fp, "Hardware packet 2 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "Start address for read for PUCCH PDU %d is : %d\n", i, read_addr);
		fprintf(fp, "Number address for read for PUCCH PDU number %d is : %d\n", i, num_addr);
		fprintf(fp, "Write counter for read for PUCCH PDU number %d is : %d\n", i, write_counter);

		if (i == Ul_tti_intermediate.nPUCCH -1)
			break;

		PUCCH_PDU_t next_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i+1]];
		if (next_pucch.FormatType == 0 && next_pucch.StartSymbolIndex == cur_pucch.StartSymbolIndex && next_pucch.BWPStart+next_pucch.prbStart == cur_pucch.BWPStart+cur_pucch.prbStart)
			i += 2;
		else
			i += 1;
	}

	HW_OP[cur_addr] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF0_INTERLEAVE_READ<<56) + (uint64_t)num_users;
	fprintf(fp, "\nHardware packet for read config is : 0X%lX\n", HW_OP[cur_addr]);
	fprintf(fp, "Number of read configs are : %d\n", num_users);
}


/*
Function: generate_pucchF2_UE_interlerleave_config
Author: Aniruddh V

	Function to generate the configs for the UE interleaver for PUCCHF2.
	REs from every OFDM symbol is written into the BRAM in a discontinuous manner to group all users together.
	The REs are later read from the BRAM in continuous manner

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):

*/
/*
void generate_pucchF2_UE_interleave_config_old(){
	int num_writes=0, start_addr=0, i, num_users=0, user_cur_write[MaxUE], num_addr;

	// WRITE configs
	int cur_addr = HW_OP_idx++;
	for (i = 0; i < Ul_tti_intermediate.nPUCCH; ++i)
		user_cur_write[i] = 32*i; //32 locations for each user. We think this is a pointer to how much you have filled in the array.

	int num_writes_per_symb[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	for (int l = 0; l < 14; ++l){
		i = 0;
		while(i < Ul_tti_intermediate.nPUCCH){
			PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_DMRS_idx[i]];
			if (cur_pucch.FormatType != 2){
				++i;
				continue;
			}

			if (l < cur_pucch.StartSymbolIndex || l > cur_pucch.StartSymbolIndex + cur_pucch.NrOfSymbols -1){
				++i;
				continue;
			}

			start_addr = user_cur_write[i];
			num_addr = cur_pucch.prbSize;
			num_writes_per_symb[l] += num_addr;
			user_cur_write[i] = start_addr + num_addr;
			++num_writes;

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_WRITE<<56) + (uint64_t)(start_addr+num_addr) + (uint64_t)((uint64_t)start_addr<<16);
			fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			fprintf(fp, "Start address for write number %d is : %d\n", num_writes, start_addr);
			fprintf(fp, "End address for write number %d is : %d\n", num_writes, start_addr+num_addr);
			fprintf(fp, "OFDM Symbol number is : %d\n", l);

			++i;
		}
	}

	HW_OP[cur_addr] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_WRITE<<56) + (uint64_t)num_writes;
	fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[cur_addr]);
	fprintf(fp, "Number of write configs are : %d\n", num_writes);

	// READ configs
	cur_addr = HW_OP_idx++;
	i = 0;
	while(i < Ul_tti_intermediate.nPUCCH){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];
		if (cur_pucch.FormatType != 2){
			++i;
			continue;
		}
		int read_addr, num_addr, write_counter;
		++num_users;

		num_addr = 12*cur_pucch.prbSize*cur_pucch.NrOfSymbols;
		read_addr = user_cur_write[i] - num_addr;

		write_counter = 0;
		for (int k = 0; k < cur_pucch.StartSymbolIndex+cur_pucch.NrOfSymbols; ++k)
			write_counter += num_writes_per_symb[k];

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_READ<<56) + (uint64_t)(num_addr) + (uint64_t)((uint64_t)read_addr<<16);
		fprintf(fp, "\nHardware packet 1 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_READ<<56) + (uint64_t)(write_counter);
		fprintf(fp, "Hardware packet 2 for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "Start address for read for PUCCH PDU %d is : %d\n", i, read_addr);
		fprintf(fp, "Number address for read for PUCCH PDU number %d is : %d\n", i, num_addr);
		fprintf(fp, "Write counter for read for PUCCH PDU number %d is : %d\n", i, write_counter);

		++i;
	}

	HW_OP[cur_addr] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_READ<<56) + (uint64_t)num_users;
	fprintf(fp, "\nHardware packet for read config is : 0X%lX\n", HW_OP[cur_addr]);
	fprintf(fp, "Number of read configs are : %d\n", num_users);
}
*/


void generate_pucchF2_UE_interleave_config(){

    int userWriteAddr[Ul_tti_intermediate.nPUCCH][2];// two symbols
    int userWriteUntil[Ul_tti_intermediate.nPUCCH][2];// two symbols

    int read_addr[Ul_tti_intermediate.nPUCCH];
    int read_num_addr[Ul_tti_intermediate.nPUCCH];

    int tot_write_config = 0;
    int totUserRBs = 0; // total RBs for all users
    int symbCounter[Ul_tti_intermediate.nPUCCH]; // will increment after writing each symbol 0 to 1 to 2
    int pucch_f2indices[Ul_tti_intermediate.nPUCCH];
    int pucch_f2_count = 0;

    int decode_mode;

	for(int i = 0; i < Ul_tti_intermediate.nPUCCH; i++){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];
		if(cur_pucch.FormatType != 2){
			continue;
		}
		pucch_f2indices[pucch_f2_count++] = sorted_PUCCH_idx[i];

		/***********intialize***************/
        userWriteAddr[i][0] = -1; // symb 1
        userWriteAddr[i][1] = -1; // symb 2
        userWriteUntil[i][0] = -1;
        userWriteUntil[i][1] = -1;
        /***/
        symbCounter[i] = 0;
        /*************************/
        tot_write_config = tot_write_config + cur_pucch.NrOfSymbols;

        userWriteAddr[i][0] = totUserRBs; // assuming index start from 0
        userWriteUntil[i][0] = userWriteAddr[i][0] + cur_pucch.prbSize;
        if(cur_pucch.NrOfSymbols == 2){
            userWriteAddr[i][1] = userWriteUntil[i][0];
            userWriteUntil[i][1] = userWriteAddr[i][1] + cur_pucch.prbSize;
        }
        totUserRBs = totUserRBs+cur_pucch.NrOfSymbols*cur_pucch.prbSize;
	}
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_WRITE<<56) + (uint64_t)(tot_write_config);
	fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Number of write configs are : %d\n", tot_write_config);

	int write_config_index = 0;
    int read_config_index = 0;
    int num_addr_written = 0;
    int write_threshold[Ul_tti_intermediate.nPUCCH];

    for(int symbIdx = 0; symbIdx < 14; symbIdx++){
        for(int i=0; i < pucch_f2_count; ++i){
        	PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[pucch_f2indices[i]];
            if((cur_pucch.StartSymbolIndex <= symbIdx) && (symbIdx < (cur_pucch.StartSymbolIndex+cur_pucch.NrOfSymbols))){// If this symbol contains i'th user
                write_config_index = write_config_index + 1;
                printf("%d) write_addr->write_until: %d -> %d \n", write_config_index, userWriteAddr[i][symbCounter[i]], userWriteUntil[i][symbCounter[i]]);
                HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_WRITE<<56) + (uint64_t)(userWriteUntil[i][symbCounter[i]]) + (uint64_t)((uint64_t)userWriteAddr[i][symbCounter[i]]<<16);
				fprintf(fp, "\nHardware packet for write config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
				fprintf(fp, "Write until : %d\n", userWriteUntil[i][symbCounter[i]]);
				fprintf(fp, "write address: %d\n", userWriteAddr[i][symbCounter[i]]);
                num_addr_written += cur_pucch.prbSize;
                symbCounter[i] = symbCounter[i]+1;
                if(symbCounter[i] == cur_pucch.NrOfSymbols){// ith user writing is finished
                    read_addr[read_config_index] = userWriteAddr[i][0];
                    read_num_addr[read_config_index] = cur_pucch.prbSize*cur_pucch.NrOfSymbols;
                    write_threshold[read_config_index] = num_addr_written;
                    // read_config_map[read_config_index] = i;
                    read_config_index++;
                }
            }
        }
    }

    // error checking
    // if(write_config_index != tot_write_config){
    //     printf("Error: write_config_index = %d, tot_write_config = %d are different \n",write_config_index,tot_write_config);
    // }
    // if(read_config_index != Ul_tti_intermediate.nPUCCH){
    //     printf("Error: read_config_index = %d, num_user = %d are different \n",read_config_index, Ul_tti_intermediate.nPUCCH);
    // }
    HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_READ<<56) + (uint64_t)(pucch_f2_count);
    fprintf(fp, "\nHardware packet for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Number of read configs are : %d\n", pucch_f2_count);


    for(int i=0; i < pucch_f2_count; ++i ){
        printf("%d) read_addr->read_num_addr: %d -> %d \n", i, read_addr[i], read_num_addr[i]);
        HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_READ<<56) + (uint64_t)(read_num_addr[i]) + (uint64_t)((uint64_t)read_addr[i]<<16);
        fprintf(fp, "\nHardware packet for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "Read number of addresses : %d\n", read_num_addr[i]);
		fprintf(fp, "Read address : %d\n", read_addr[i]);

		if(Ul_ttiRequest.PUCCH_PDU[pucch_f2indices[i]].SRFlag + Ul_ttiRequest.PUCCH_PDU[pucch_f2indices[i]].BitLenHarq + Ul_ttiRequest.PUCCH_PDU[pucch_f2indices[i]].BitLenCsiPart1 + Ul_ttiRequest.PUCCH_PDU[pucch_f2indices[i]].BitLenCsiPart2 >=12){
			decode_mode = 0;
		}
		else{
			decode_mode = 1;
		}

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_INTERLEAVE_READ<<56) + (uint64_t)(write_threshold[i]) + (uint64_t)((uint64_t)i<<16) + (uint64_t)((uint64_t)decode_mode<<23);
		fprintf(fp, "\nHardware packet for read config is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "Write threshold : %d\n", write_threshold[i]);
		fprintf(fp, "User number: %d\n", i);
		fprintf(fp, "Decode mode : %d\n", decode_mode);
    }


    // for(int i=0; i < Ul_tti_intermediate.nPUCCH; ++i ){
    //     printf("%d) write_threshold: %d \n", i, write_threshold[i]);
    // }
}



/*
Function: generate_pucchF2_descrambler
Author: Aniruddh V

	Function to generate the descrambler configs for PUCCH F2 data

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		Sequence indices and data length

*/

void generate_pucchF2_descrambler(){
	int NID, NRNTI, total_LLR=0, num_users=0;
	int sorted_pdus[MaxUE];

	for (int i = 0; i < Ul_tti_intermediate.nPUCCH; ++i){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];

		if (cur_pucch.FormatType != 2)
			continue;

		sorted_pdus[num_users] = sorted_PUCCH_idx[i];
		++num_users;
		total_LLR += 8*cur_pucch.prbSize*cur_pucch.NrOfSymbols*2;

		// NID = cur_pucch.dataScramblingId;
		// NRNTI = cur_pucch.RNTI;

		// HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DESCRAMBLER<<56) + (uint64_t)NID + (uint64_t)((uint64_t)NRNTI<<16);
		// fprintf(fp, "\nHardware packet for user %d is : 0X%lX\n", num_users, HW_OP[HW_OP_idx-1]);
		// fprintf(fp, "NID for user number %d is : %d\n", num_users, NID);
		// fprintf(fp, "NRNTI for user number %d is : %d\n", num_users, NRNTI);
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DESCRAMBLER<<56) + (uint64_t)num_users + (uint64_t)((uint64_t)total_LLR<<16);
	fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx - 1]);
	fprintf(fp, "Total users are : %d\n", num_users);
	fprintf(fp, "Total LLRs are : %d\n", total_LLR);

	for (int i = 0; i < num_users; ++i){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_pdus[i]];

		NID = cur_pucch.dataScramblingId;
		NRNTI = cur_pucch.RNTI;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DESCRAMBLER<<56) + (uint64_t)NID + (uint64_t)((uint64_t)NRNTI<<16);
		fprintf(fp, "\nHardware packet for user %d is : 0X%lX\n", i, HW_OP[HW_OP_idx-1]);
		fprintf(fp, "NID for user number %d is : %d\n", i, NID);
		fprintf(fp, "NRNTI for user number %d is : %d\n", i, NRNTI);
	}
}


/*
Function: generate_pucchF2_config
Author: Anil. Y

	Function to generate the configs for PUCCH F2 data encoding dmrs.
	Pi-by-2-bpsk is NOT applicable for FORMAT2
	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		Sequence indices and data length

*/

void generate_pucchF2_config(){
	int A, A1, A2, Ap, Kr, N, E, Er, CRC, n, n1, n2, K, K_index, augment=1;
	int I_seg, C, E_uci[2], Qm=2, a, b, num_uci_msgs, ceil_logE;
	int I_BIL = 1;
	int BA_table[512], UCI_payload_len, Lcrc, E_tot, num_CB;
	int n_min = 5, n_max = 10, I_IL=0, n_PC, n_wm_PC;
	int Rmax = 1;

	for (int j = 0; j < Ul_tti_intermediate.nPUCCH; ++j){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[j]];

		if (cur_pucch.FormatType != 2)
			continue;

		UCI_payload_len = cur_pucch.SRFlag + cur_pucch.BitLenHarq + cur_pucch.BitLenCsiPart1 + cur_pucch.BitLenCsiPart2;
		E_tot = 8*cur_pucch.prbSize*cur_pucch.NrOfSymbols*Qm;
		// 2 is QPSK mod, 8 - no of REs per RB for UCI data

		if (cur_pucch.BitLenCsiPart2>0){
			num_uci_msgs = 2;
			A1 = cur_pucch.SRFlag + cur_pucch.BitLenHarq + cur_pucch.BitLenCsiPart1;
			A2 = cur_pucch.BitLenCsiPart2;
		}
		else{
			num_uci_msgs = 1;
			A1 = cur_pucch.SRFlag + cur_pucch.BitLenHarq + cur_pucch.BitLenCsiPart1;
			A2 = 0;
		}
		printf("SR Flag: %d, Bit len HARQ: %d, Bit len CSI1: %d, A1: %d\n", cur_pucch.SRFlag, cur_pucch.BitLenHarq, cur_pucch.BitLenCsiPart1, A1);

		for (int i=0; i<num_uci_msgs; i++){
			if(i==0){ // message 1
				A = A1;
			}
			else{
				A = A2; // message 2
			}

			//CRC lengths
			if (A>=12){
				if (A>=20){
					Lcrc = 11;
					CRC = 1;
				}
				else{
					Lcrc = 6;
					CRC = 0;
					}
			}
			else{
				Lcrc = 0;
			}



			if (cur_pucch.BitLenCsiPart2 == 0){
				E_uci[i] = E_tot;
			}
			else{
				if(i==0){ // message 1
					a = E_tot;
					b = ceil((double)(A + Lcrc)/(double)(Rmax*Qm))*Qm;

					E_uci[i] = a < b ? a : b;
				}
				else{ // message 2

					E_uci[i] = E_tot - E_uci[i-1];
				}
			}


			if(A>=12){

				if((A>=360 && E_uci[i]>=1088) || (A>=1013)){
					I_seg = 1;
					C = 2; // Two codeblocks
				}
				else{
					I_seg = 0; // One codeblock
					C = 1;
				}

				for(int r=0; r<C; r++){ // for each codeblock
					Ap = (ceil((double)A/(double)C))*C;
					Kr = Ap + Lcrc;
					//Ap - Kr - N (polar) - Er (rate matching) - bit lengths
					Er = E_uci[i]/C;
					ceil_logE = ceil(log2(Er));
					if (Er <= (9.0/8.0)*pow(2, ceil_logE-1) && 16*Kr < 9*Er){
						n1 = ceil_logE - 1;
					}
					else{
						n1 = ceil_logE;
					}

					// R_min = 1/8;

					n2 = ceil(log2(8*Kr));

					n = n1 < n2 ? n1 : n2;

					n = n < n_max ? n : n_max;

					n = n > n_min ? n : n_min;

					N = pow(2, n);

					// if(Kr > 255){
					// 	printf("Value of Kr should be less than 255, Kr is %d\n", Kr);
					// }

					HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2<<56) + (uint64_t)(Kr-Lcrc) + (uint64_t)((uint64_t)N<<10) + (uint64_t)((uint64_t)Er<<20) + (uint64_t)((uint64_t)CRC<<30) + (uint64_t)((uint64_t)augment<<31);
					fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
					printf("A: %d, Kr: %d, Lcrc: %d\n", A, Kr, Lcrc);
					fprintf(fp, "K is : %d \n", Kr-Lcrc);
					fprintf(fp, "N is : %d \n", N);
					fprintf(fp, "E is : %d \n", Er);
					fprintf(fp, "CRC is : %d \n", CRC);
					fprintf(fp, "Augment is : %d \n", augment);


					if(Kr>=18 && Kr<=25){

						n_PC = 3;
						if(Er-Kr>189){
							n_wm_PC = 1;
						}
						else{
							n_wm_PC = 0;
						}
					}

					else{ // Kr >30
						n_PC = 0;
						n_wm_PC = 0;
					}

					//////////////////////////////////////////////////////////
					//----- BA table using xilinx c code --------------------
					flg_t imap[1024];
					idx_t para[2];
				    getIMAP(Er, A, Lcrc, n_PC, n_wm_PC, imap, para);

				    bat_t BA_out[64];
					getBAOUT(imap, para, BA_out);
					for (int m = 0; m < ceil(N/16); ++m){
						HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2<<56) + (uint64_t) BA_out[m];
						fprintf(fp, "Hardware packet%d of BA Table is : 0X%lX\n", m+1, HW_OP[HW_OP_idx-1]);
					}
					//////////////////////////////////////////////////////////

//					// BA table
//					// Generate polar encoding table entries
//					K_index = 0;
//					int count1 = 0, count2 = 0, count3 = 0, count4 = 0;
//					printf("N: %d n_PC: %d n_wm_PC: %d\n", N, n_PC, n_wm_PC);
//					printf("N-(Kr+n_PC): %d, N-(Kr+n_PC-n_wm_PC): %d, N-(Lcrc): %d\n", N-(Kr+n_PC), N-(Kr+n_PC-n_wm_PC), N-(Lcrc));
//
//					for (int k = 0; k < 1024; ++k){
//						if (polar_reliability_table_UL[k] >= N)
//							continue;
//
//						++K_index;
//
//						// printf("K_index: %d\n", K_index);
//						if (K_index <= N-(Kr+n_PC)){//frozen
//							count1+= 1;
//							BA_table[polar_reliability_table_UL[k]] = 0;
//						}
//						else if (K_index <= N-(Kr+n_PC-n_wm_PC)){ // parity
//							// printf("Parity - polar index: %d\n", polar_reliability_table_UL[k]+1);
//							// BA_table[polar_reliability_table_UL[k]] = 0;
//							BA_table[polar_reliability_table_UL[k]] = 3;
//							count2+= 1;
//						}
//						else if (K_index <= N-(Lcrc)){ // Info
//							// printf("Info - polar index: %d\n", polar_reliability_table_UL[k]+1);
//							BA_table[polar_reliability_table_UL[k]] = 2;
//							count3+= 1;
//						}
//						else{ // CRC
//							// printf("CRC - polar index: %d\n", polar_reliability_table_UL[k]+1);
//							BA_table[polar_reliability_table_UL[k]] = 1;
//							count4+= 1;
//						}
//					}
//					printf("count 1: %d, count 2: %d, count 3: %d, count 4: %d\n", count1, count2, count3, count4);
//
//					// Printing 16 packets of 64 bits each. Each packet has 32 occupied bits  + address.
//					HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2<<56);
//					for (int m = 0; m < N; ++m){
//						// printf("m: %d, m mod 16: %d, Bit shift amount: %d\n", m, m%16, 2*(m%16));
//						HW_OP[HW_OP_idx] += (uint64_t)((uint64_t)BA_table[m]<<(2*(m%16)));
//						if (m%16 == 15){
//							++HW_OP_idx;
//							fprintf(fp, "Hardware packet%d is : 0X%lX\n", 3+m/16, HW_OP[HW_OP_idx-1]);
//							HW_OP[HW_OP_idx] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2<<56);
//						}
//					}

				}// codeblock loop
			} // end A>=12 if

			else{ // small blocks A<12
				K = A;
				if(A==1){
					N = 2;
				}
				else if(A==2){
					N = 6;
				}
				else{
					N = 32;
				}


				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCH_DEC_SHORT_LENGTH<<56) + (uint64_t)K;
				fprintf(fp, "\nHardware packet is: 0X%lX\n", HW_OP[HW_OP_idx-1]);
				fprintf(fp, "K is : %d \n", K);

				HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCH_RR_SHORT_LENGTH<<56) + (uint64_t)E_uci[i] + (uint64_t)((uint64_t)N<<11);
				fprintf(fp, "\nHardware packet is: 0X%lX\n", HW_OP[HW_OP_idx-1]);
				fprintf(fp, "E is : %d \n", E_uci[i]);
				fprintf(fp, "N is: %d\n", N);

			}
		}
	}
}

/*
Function: generate_pucchF2_dmrs
Author: Aniruddh V

	Function to generate the configs for PUCCH F2 dmrs

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		Sequence indices and data length

*/

void generate_pucchF2_dmrs(){
	int slot = Ul_ttiRequest.Slot;
	int scramblingID, dmrs_present, start_RB, num_RB, num_users=0;

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DMRS<<56) + (uint64_t)slot;
	fprintf(fp, "\nHardware packet is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Slot number is : %d\n", slot);

	for (int i = 0; i < Ul_tti_intermediate.nPUCCH; ++i){
		PUCCH_PDU_t cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[i]];

		if (cur_pucch.FormatType != 2)
			continue;

		++num_users;
		scramblingID = cur_pucch.DmrsScramblingId;
		dmrs_present = (int)((2*cur_pucch.NrOfSymbols-1)<<cur_pucch.StartSymbolIndex);
		start_RB = cur_pucch.BWPStart + cur_pucch.prbStart;
		num_RB = cur_pucch.prbSize;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DMRS<<56) + (uint64_t)scramblingID + (uint64_t)((uint64_t)dmrs_present<<16);
		fprintf(fp, "\nHardware packet for user %d is : 0X%lX\n", num_users, HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DMRS<<56) + (uint64_t)start_RB + (uint64_t)((uint64_t)num_RB<<9);
		fprintf(fp, "Hardware packet for user %d is : 0X%lX\n", num_users, HW_OP[HW_OP_idx-1]);

		fprintf(fp, "DMRS Scrambling ID for user %d is : %d\n", num_users, scramblingID);
		fprintf(fp, "DMRS locations for user %d is : %d\n", num_users, dmrs_present);
		fprintf(fp, "Start RB for user %d is : %d\n", num_users, start_RB);
		fprintf(fp, "Number of RBs for user %d is : %d\n", num_users, num_RB);
	}

	for (int i = num_users; i < MaxUE; ++i){
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DMRS<<56);
		fprintf(fp, "\nHardware packet for user %d is : 0X%lX\n", i+1, HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PUCCHF2_DMRS<<56);
		fprintf(fp, "Hardware packet for user %d is : 0X%lX\n", i+1, HW_OP[HW_OP_idx-1]);
	}
}

/*
Function: generate_UL_ORAN_config
Author: Aniruddh V

	Function to generate configurations for ORAN TX C-plane and U-plane.
	One section for each MU-MIMO user group(PUSCH, DMRS, PTRS), one section for PRACH
	(TBD later) Separate section for PUCCH, SRS
	Requires the presence of the Ul_ttiRequest and Ul_tti_intermediate variables.

	Input:
		None

	Output (to file):
	C-plane requires application header for each section header.
	U-plane requires application header for each OFDM symbol.
	Configs are repeated AS-IS for every layer (8-times)
	Section header, Application header, Section extension header (possibly eCPRI and ethernet headers)

*/

void generate_UL_ORAN_config(){

	int Application_SectionType = 1;
	int dataDir = 0;				// UL
	int payloadVer = 1;
	int filterIndex = 0;
	int FrameID = Ul_ttiRequest.SFN%256;
	int subframeID = Ul_ttiRequest.Slot/2;
	int slotID = Ul_ttiRequest.Slot%2;

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
	uint64_t Mux_config_packet = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<52) + (uint64_t)((uint64_t)0<<51);
	int Mux_Config_Address = HW_OP_idx;
	HW_OP_idx += 2;

	// C-plane configs
	fprintf(fp, "=====================================================C-plane configs=====================================================================\n");

	int n_ORAN_PUSCH_PDU = 0;
	int start_loc_PUSCH = 14*MAX_PRB_BW;
	PUSCH_PDU_t cur_pusch;
	if (Ul_tti_intermediate.nPUSCH > 0){
		cur_pusch = Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[0]];
		start_loc_PUSCH = MAX_PRB_BW*cur_pusch.StartSymbolIndex + cur_pusch.BWPStart + cur_pusch.rbStart;
	}
	int n_ORAN_PUCCH_PDU = 0;
	int start_loc_PUCCH = 14*MAX_PRB_BW;
	PUCCH_PDU_t cur_pucch;
	if (Ul_tti_intermediate.nPUCCH > 0){
		cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[0]];
		start_loc_PUCCH = MAX_PRB_BW*cur_pucch.StartSymbolIndex + cur_pucch.BWPStart;
	}
	int n_ORAN_PRACH_PDU = 0;
	int start_loc_PRACH = 14*MAX_PRB_BW;
	PRACH_PDU_t cur_PRACH;
	if (Ul_tti_intermediate.nPRACH > 0){
		cur_PRACH = Ul_ttiRequest.PRACH_PDU[sorted_PRACH_idx[0]];
		start_loc_PRACH = 150;									// CONFIG_REQUEST.START
	}

	int n_ORAN_section = 0;
	int start_prb = 0;
	while (start_loc_PUSCH < 14*MAX_PRB_BW || start_loc_PRACH < 14*MAX_PRB_BW || start_loc_PUCCH < 14*MAX_PRB_BW){
		
		// Sections for PUSCH
		if (start_loc_PUSCH < start_loc_PRACH && start_loc_PUSCH < start_loc_PUCCH){
			int startSymbolID = cur_pusch.StartSymbolIndex;
			int sectionID = n_ORAN_section;
			int start_RB = cur_pusch.BWPStart + cur_pusch.rbStart;
			int num_RB = cur_pusch.rbSize;
			int reMask = (int)0b111111111111;
			int numSym = cur_pusch.NrOfSymbols;
			int numBundlePRB = 4;										// cur_pusch.RxPrecoding_Beamforming.numPRGs;

			int num_MUMIMO_UE = 1;
			RxPrecoding_Beamforming_t cur_RxPrecoding_Beamforming[4];
			int dmrsPorts[8];
			for (int i = 0; i < 4; ++i){
				if (n_ORAN_PUSCH_PDU+i >= Ul_tti_intermediate.nPUSCH){
					num_MUMIMO_UE = i;
					break;
				}
				PUSCH_PDU_t cur_pusch = Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[n_ORAN_PUSCH_PDU+i]];
				if (start_RB != cur_pusch.BWPStart + cur_pusch.rbStart || numSym != cur_pusch.NrOfSymbols){
					num_MUMIMO_UE = i;
					break;
				}
				cur_RxPrecoding_Beamforming[i] = Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[n_ORAN_PUSCH_PDU+i]].RxPrecoding_Beamforming;
				dmrsPorts[i] = Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[n_ORAN_PUSCH_PDU+i]].dmrsPorts;
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
						fprintf(fp, "\nHardware packet 1 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						HW_OP[HW_OP_idx++] = Application_Header_packet2;
						fprintf(fp, "Hardware packet 2 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						fprintf(fp, "Section type is : %d \n", Application_SectionType);
						fprintf(fp, "Data Direction is : %d \n", dataDir);
						fprintf(fp, "Frame number is : %d \n", FrameID);
						fprintf(fp, "Subframe number is : %d \n", subframeID);
						fprintf(fp, "Slot number is : %d \n", slotID);
						fprintf(fp, "Starting symbol is : %d \n", startSymbolID);
						fprintf(fp, "Number of sections are : %d \n", numSections);
						fprintf(fp, "Compression header is : %d \n", udCompressionHeader);

						// Section header
						int beamID = 1;
						int RTCid = (int)((1+available_ports[k])<<12);

						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)sectionID<<31) + (uint64_t)((uint64_t)start_RB<<21) + (uint64_t)((uint64_t)ORAN_sections[n_ORAN_section-1].num_freq<<13) + (uint64_t)((uint64_t)reMask<<1);
						fprintf(fp, "\nHardware packet 1 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
						HW_OP[HW_OP_idx++] = Section_Header_packet2 + (uint64_t)((uint64_t)numSym<<39) + (uint64_t)((uint64_t)beamID<<24) + (uint64_t)((uint64_t)RTCid<<7);
						fprintf(fp, "Hardware packet 2 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						fprintf(fp, "Section type is : %d \n", Section_SectionType);
						fprintf(fp, "Section ID is : %d \n", sectionID);
						fprintf(fp, "RB is : %d \n", rb);
						fprintf(fp, "Symbol Increment is : %d \n", symInc);
						fprintf(fp, "Start PRB is : %d \n", start_RB);
						fprintf(fp, "Number of PRB are : %d \n", num_RB);
						fprintf(fp, "RE mask is : 0X%04X \n", reMask);
						fprintf(fp, "Number of symbols are : %d \n", numSym);
						fprintf(fp, "Beam ID is : %d \n", beamID);
						fprintf(fp, "RTC ID is : 0X%x \n", RTCid);

						// Section Extension header
						for (int l = 0; l < num_beams; ++l){
							beamID = cur_RxPrecoding_Beamforming[j].NumOfPRGs[l+i/4].beamIdx[k];
							HW_OP[HW_OP_idx++] = SectionExtension_Header_packet1 + (uint64_t)((uint64_t)numBundlePRB<<16);
							fprintf(fp, "\nHardware packet %d of Section Extension Header is : 0X%lX\n", 2*l+1, HW_OP[HW_OP_idx-1]);
							HW_OP[HW_OP_idx++] = SectionExtension_Header_packet2 + (uint64_t)((uint64_t)beamID<<20);
							fprintf(fp, "Hardware packet %d of Section Extension Header is : 0X%lX\n", 2*l+2, HW_OP[HW_OP_idx-1]);
						}

						fprintf(fp, "Extension type is : %d \n", extensionType);
						fprintf(fp, "ef is : %d \n", ef);
						fprintf(fp, "extLen is : %d \n", extLen);
						fprintf(fp, "disableBFW is : %d \n", disableBFW);
						fprintf(fp, "RAD is : %d \n", RAD);
						fprintf(fp, "Number of PRBs in a bundle are : %d \n", numBundlePRB);
						fprintf(fp, "BFW compression header is : 0X%04X \n", BFWcompressionHeader);
						fprintf(fp, "BFW CompParam is : %d \n", BFWcompressionParameter);
						for (int l = 0; l < num_beams; ++l){
							beamID = cur_RxPrecoding_Beamforming[j].NumOfPRGs[l+i/4].beamIdx[k];
							fprintf(fp, "Beam%d ID is : 0X%04X \n", 1+l, beamID);
						}
					}
				}
			}

			n_ORAN_PUSCH_PDU += num_MUMIMO_UE;
			start_prb += cur_pusch.rbSize;
			if (n_ORAN_PUSCH_PDU == Ul_tti_intermediate.nPUSCH){
				start_loc_PUSCH = 14*MAX_PRB_BW;
			}
			else{
				cur_pusch = Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[n_ORAN_PUSCH_PDU]];
				start_loc_PUSCH = MAX_PRB_BW*cur_pusch.StartSymbolIndex + cur_pusch.BWPStart + cur_pusch.rbStart;
			}
		}

		// Sections for PUCCH
		else if (start_loc_PUCCH < start_loc_PRACH && start_loc_PUCCH < start_loc_PUSCH){
			/* code */
			n_ORAN_section++;
			n_ORAN_PUCCH_PDU++;

			start_prb += cur_pucch.BWPSize;
			if (n_ORAN_PUCCH_PDU == Ul_tti_intermediate.nPUCCH){
				start_loc_PUCCH = 14*MAX_PRB_BW;
			}
			else{
				cur_pucch = Ul_ttiRequest.PUCCH_PDU[sorted_PUCCH_idx[n_ORAN_PUCCH_PDU]];
				start_loc_PUCCH = MAX_PRB_BW*cur_pucch.StartSymbolIndex + cur_pucch.BWPStart;
			}
		}

		//Sections for PRACH
		else if (start_loc_PRACH < start_loc_PUSCH && start_loc_PRACH < start_loc_PUCCH){

			int startSymbolID = cur_PRACH.prachStartSymbol;
			int sectionID = n_ORAN_section;
			int start_RB = start_loc_PRACH;
			int num_RB = 12;
			int reMask = (int)0b111111111111;
			int numSym = cur_PRACH.numRa;
			int numBundlePRB = 4;							// cur_SSB.RxPrecoding_Beamforming.numPRGs;
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
			fprintf(fp, "\nHardware packet 1 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = Application_Header_packet2;
			fprintf(fp, "Hardware packet 2 of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "PDU type is : PRACH \n");
			fprintf(fp, "Section type is : %d \n", Application_SectionType);
			fprintf(fp, "Data Direction is : %d \n", dataDir);
			fprintf(fp, "Frame number is : %d \n", FrameID);
			fprintf(fp, "Subframe number is : %d \n", subframeID);
			fprintf(fp, "Slot number is : %d \n", slotID);
			fprintf(fp, "Starting symbol is : %d \n", startSymbolID);
			fprintf(fp, "Number of sections are : %d \n", numSections);
			fprintf(fp, "Compression header is : %d \n", udCompressionHeader);

			// Section header
			HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)sectionID<<31) + (uint64_t)((uint64_t)start_RB<<21) + (uint64_t)((uint64_t)num_RB<<13) + (uint64_t)((uint64_t)reMask<<1);
			fprintf(fp, "\nHardware packet 1 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = Section_Header_packet2 + (uint64_t)((uint64_t)numSym<<39) + (uint64_t)((uint64_t)beamID<<24) + (uint64_t)((uint64_t)RTCid<<7);
			fprintf(fp, "Hardware packet 2 of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "Section type is : %d \n", Section_SectionType);
			fprintf(fp, "Section ID is : %d \n", sectionID);
			fprintf(fp, "RB is : %d \n", rb);
			fprintf(fp, "Symbol Increment is : %d \n", symInc);
			fprintf(fp, "Start PRB is : %d \n", start_RB);
			fprintf(fp, "Number of PRB are : %d \n", num_RB);
			fprintf(fp, "RE mask is : 0X%04X \n", reMask);
			fprintf(fp, "Number of symbols are : %d \n", numSym);
			fprintf(fp, "Beam ID is : %d \n", beamID);
			fprintf(fp, "RTC ID is : %d \n", RTCid);

			// Section Extension header
			for (int i = 0; i < 3; ++i){
				beamID = cur_PRACH.RxPrecoding_Beamforming.NumOfPRGs[i].beamIdx[0];
				HW_OP[HW_OP_idx++] = SectionExtension_Header_packet1 + (uint64_t)((uint64_t)numBundlePRB<<16);
				fprintf(fp, "\nHardware packet %d of Section Extension Header is : 0X%lX\n", 2*i+1, HW_OP[HW_OP_idx-1]);
				HW_OP[HW_OP_idx++] = SectionExtension_Header_packet2 + (uint64_t)((uint64_t)beamID<<20);
				fprintf(fp, "Hardware packet %d of Section Extension Header is : 0X%lX\n", 2*i+2, HW_OP[HW_OP_idx-1]);
			}

			fprintf(fp, "Extension type is : %d \n", extensionType);
			fprintf(fp, "ef is : %d \n", ef);
			fprintf(fp, "extLen is : %d \n", extLen);
			fprintf(fp, "disableBFW is : %d \n", disableBFW);
			fprintf(fp, "RAD is : %d \n", RAD);
			fprintf(fp, "Number of PRBs in a bundle are : %d \n", numBundlePRB);
			fprintf(fp, "BFW compression header is : 0X%04X \n", BFWcompressionHeader);
			fprintf(fp, "BFW CompParam is : %d \n", BFWcompressionParameter);
			for (int i = 0; i < 3; ++i){
				beamID = cur_PRACH.RxPrecoding_Beamforming.NumOfPRGs[i].beamIdx[0];
				fprintf(fp, "Beam%d ID is : 0X%04X \n", 1+i, beamID);
			}

			n_ORAN_PRACH_PDU++;
			start_prb += 12;
			start_loc_PRACH = 14*MAX_PRB_BW;				// ASSUME ONLY ONE SECTION FOR THE SSB RESOURCE ACROSS ALL SYMBOLS IN THE SLOT
		}
	}

	// Mux config packets
	HW_OP[Mux_Config_Address] = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<52) + (uint64_t)((uint64_t)0<<51) + (uint64_t)((uint64_t)n_ORAN_section<<39) + (uint64_t)((uint64_t)n_ORAN_section<<27) + (uint64_t)((uint64_t)n_ORAN_section<<15) + (uint64_t)((uint64_t)n_ORAN_section<<3);
	fprintf(fp, "\nHardware packet1 of Mux config is : 0X%lX\n", HW_OP[Mux_Config_Address]);
	HW_OP[Mux_Config_Address+1] = (uint64_t)((uint64_t)ADDRESS_ORAN_C_PLANE<<56) + (uint64_t)((uint64_t)0<<52) + (uint64_t)((uint64_t)1<<51) + (uint64_t)((uint64_t)n_ORAN_section<<39) + (uint64_t)((uint64_t)n_ORAN_section<<27) + (uint64_t)((uint64_t)n_ORAN_section<<15) + (uint64_t)((uint64_t)n_ORAN_section<<3);
	fprintf(fp, "Hardware packet2 of Mux config is : 0X%lX\n", HW_OP[Mux_Config_Address+1]);

	for (int i = 0; i < MAX_ORAN_UL_LAYERS; ++i){
		fprintf(fp, "Number of sections in layer %d is %d \n", 1+i, n_ORAN_section);
	}

	// U-plane configs
	fprintf(fp, "=============================================================U-plane configs=============================================================================\n");

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
		for (int l = 0; l < MAX_ORAN_UL_LAYERS; ++l){
			int startSymbolID = i+1;
			int n_ORAN_empty_section = 4095;
			int start_prb = 0;
			int num_prb = MAX_PRB_BW;
			int sectionID = 0;

			// Application Header
			HW_OP[HW_OP_idx++] = Application_Header_packet1 + (uint64_t)((uint64_t)startSymbolID<<20);
			fprintf(fp, "\nHardware packet of Application Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "Data direction is : %d \n", dataDir);
			fprintf(fp, "Payload version is : %d \n", payloadVer);
			fprintf(fp, "Filter index is : %d \n", filterIndex);
			fprintf(fp, "Frame ID is : %d \n", FrameID);
			fprintf(fp, "Subframe ID is : %d \n", subframeID);
			fprintf(fp, "Slot ID is : %d \n", slotID);
			fprintf(fp, "Start symbol is : %d \n", startSymbolID);
			fprintf(fp, "Layer id is %d\n", 1+l);

			// Section Header
			for (int k = 0; k < n_ORAN_section; ++k){
				int j = sorted_section_idx[k];
				if ((i - ORAN_sections[j].start_symb) < 0 || (i - ORAN_sections[j].start_symb) >= ORAN_sections[j].num_symb){
					continue;
				}

				if (start_prb != ORAN_sections[j].start_freq){
					if (ORAN_sections[j].start_freq - start_prb > 255){
						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)255<<20);
						fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section--);
						fprintf(fp, "rb is : %d \n", rb);
						fprintf(fp, "symInc is : %d \n", symInc);
						fprintf(fp, "Start prb is : %d \n", start_prb);
						fprintf(fp, "Number of prb are : %d \n", 255);
						fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);

						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)(start_prb+255)<<28) + (uint64_t)((uint64_t)(ORAN_sections[j].start_freq - start_prb - 255)<<20);
						fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section--);
						fprintf(fp, "rb is : %d \n", rb);
						fprintf(fp, "symInc is : %d \n", symInc);
						fprintf(fp, "Start prb is : %d \n", start_prb+255);
						fprintf(fp, "Number of prb are : %d \n", ORAN_sections[j].start_freq - start_prb - 255);
						fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
					}
					else{
						HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)(ORAN_sections[j].start_freq - start_prb)<<20);
						fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

						fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section--);
						fprintf(fp, "rb is : %d \n", rb);
						fprintf(fp, "symInc is : %d \n", symInc);
						fprintf(fp, "Start prb is : %d \n", start_prb);
						fprintf(fp, "Number of prb are : %d \n", ORAN_sections[j].start_freq - start_prb);
						fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
					}
				}
				sectionID = j;
				num_prb = ORAN_sections[j].num_freq;
				start_prb = ORAN_sections[j].start_freq + num_prb;

				HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)sectionID<<40) + (uint64_t)((uint64_t)ORAN_sections[j].start_freq<<28) + (uint64_t)((uint64_t)num_prb<<20);
				fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

				fprintf(fp, "Section ID is : %d \n", sectionID);
				fprintf(fp, "rb is : %d \n", rb);
				fprintf(fp, "symInc is : %d \n", symInc);
				fprintf(fp, "Start prb is : %d \n", ORAN_sections[j].start_freq);
				fprintf(fp, "Number of prb are : %d \n", num_prb);
				fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
			}
			if (start_prb != MAX_PRB_BW){
				if (MAX_PRB_BW - start_prb > 255){
					HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)255<<20);
					fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

					fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section--);
					fprintf(fp, "rb is : %d \n", rb);
					fprintf(fp, "symInc is : %d \n", symInc);
					fprintf(fp, "Start prb is : %d \n", start_prb);
					fprintf(fp, "Number of prb are : %d \n", 255);
					fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);

					HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)(start_prb+255)<<28) + (uint64_t)((uint64_t)(MAX_PRB_BW - start_prb - 255)<<20);
					fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

					fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section--);
					fprintf(fp, "rb is : %d \n", rb);
					fprintf(fp, "symInc is : %d \n", symInc);
					fprintf(fp, "Start prb is : %d \n", start_prb+255);
					fprintf(fp, "Number of prb are : %d \n", MAX_PRB_BW - start_prb - 255);
					fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
				}
				else{
					HW_OP[HW_OP_idx++] = Section_Header_packet1 + (uint64_t)((uint64_t)n_ORAN_empty_section<<40) + (uint64_t)((uint64_t)start_prb<<28) + (uint64_t)((uint64_t)(MAX_PRB_BW - start_prb)<<20);
					fprintf(fp, "\nHardware packet of Section Header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

					fprintf(fp, "Section ID is : %d \n", n_ORAN_empty_section--);
					fprintf(fp, "rb is : %d \n", rb);
					fprintf(fp, "symInc is : %d \n", symInc);
					fprintf(fp, "Start prb is : %d \n", start_prb);
					fprintf(fp, "Number of prb are : %d \n", MAX_PRB_BW - start_prb);
					fprintf(fp, "UD compression header is : 0X%04X \n", udCompressionHeader);
				}
			}
		}
	}
}

/*
Function: generate_RX_Data_indication
Author: Kanish B

	Function to generate RX_Data.indication messages

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file):
		Header (for each slot): number_of_api_msgs, FAPI_HANDLE, Msg_ID, Msg_len, SFN, Slot, Number of PDUs
		For each PDU (PUSCH): 	Handle, RNTI, HarqID, PDU Length (TB size)
		*Remaining configs to be provided by RX chain

*/

void generate_RX_Data_indication(){
	int no_of_api_msgs, FAPI_handle, Msg_ID, Msg_len, SFN, Slot, nPUSCH;

	no_of_api_msgs = 1;
	FAPI_handle = 0;
	Msg_ID = 0x85;
	Msg_len = 0;
	SFN = Ul_ttiRequest.SFN;
	Slot = Ul_ttiRequest.Slot;
	nPUSCH = Ul_tti_intermediate.nPUSCH;
	Msg_len = 6; //sfn(2), slot(2), Num_pdus(2)

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)no_of_api_msgs + (uint64_t)((uint64_t)FAPI_handle<<8) + (uint64_t)((uint64_t)Msg_ID<<16);
	fprintf(fp, "\nHardware packet 1 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	for (int i = 0; i< nPUSCH; i++){
		Msg_len += 15 + Ul_ttiRequest.PUSCH_PDU[i].puschData.TBSize;
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)Msg_len;
	fprintf(fp, "Hardware packet 2 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)SFN + (uint64_t)((uint64_t)Slot<<16);
	fprintf(fp, "Hardware packet 3 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)nPUSCH;
	fprintf(fp, "Hardware packet 4 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	fprintf(fp, "no_of_api_msgs is : %d \n", no_of_api_msgs);
	fprintf(fp, "nFAPI_handle is : %d \n", FAPI_handle);
	fprintf(fp, "Msg_ID is : %d \n", Msg_ID);
	fprintf(fp, "Msg_Len is : %d \n", Msg_len);
	fprintf(fp, "SFN is : %d \n", SFN);
	fprintf(fp, "Slot is : %d \n", Slot);
	fprintf(fp, "nPUSCH is : %d \n", nPUSCH);

	int handle, RNTI, HarqID, PDU_length;

	for (int i = 0; i < nPUSCH; ++i){
		handle = Ul_ttiRequest.PUSCH_PDU[i].Handle;
		RNTI = Ul_ttiRequest.PUSCH_PDU[i].RNTI;
		HarqID = Ul_ttiRequest.PUSCH_PDU[i].puschData.harqProcessID;
		PDU_length = Ul_ttiRequest.PUSCH_PDU[i].puschData.TBSize;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)handle;
		fprintf(fp, "\nHardware packet 1 for PDU is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)RNTI + (uint64_t)((uint64_t)HarqID<<16);
		fprintf(fp, "Hardware packet 2 for PDU is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_Data_indication<<56) + (uint64_t)PDU_length;
		fprintf(fp, "Hardware packet 3 for PDU is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

		fprintf(fp, "Handle is : %d \n", handle);
		fprintf(fp, "RNTI is : %d \n", RNTI);
		fprintf(fp, "HarqID is : %d \n", HarqID);
		fprintf(fp, "PDU_length is : %d \n", PDU_length);

	}	

}


void generate_RACH_indication(){

	int no_of_api_msgs, SFN, Slot, FAPI_handle, Msg_ID, Msg_len, nPRACH, num_PDUs;
	no_of_api_msgs = 1;
	FAPI_handle = 0;
	Msg_ID = RACH_INDICATION;
	Msg_len = 0;
	SFN = Ul_ttiRequest.SFN;
	Slot = Ul_ttiRequest.Slot;
	nPRACH = Ul_tti_intermediate.nPRACH; // nPRACH - in frequency

	num_PDUs = 0;
	int prach_duration[nPRACH];
	for (int i = 0; i < nPRACH; i++){
		switch(Ul_ttiRequest.PRACH_PDU[i].prachFormat){
			case 0: prach_duration[i] = 2; //A1
			break;
			case 1: prach_duration[i] = 4; //A2
			break;
			case 2: prach_duration[i] = 6; //A3
			break;
			case 3: prach_duration[i] = 2; //B1
			break;
			case 4: prach_duration[i] = 4; //B2
			break;
			case 5: prach_duration[i] = 6; //B3
			break;
			case 6: prach_duration[i] = 12; //B4
			break;
			case 7: prach_duration[i] = 2; //C0
			break;
			case 8: prach_duration[i] = 6; //C2
			break;
			default: prach_duration[i] = 2; //Other values are unsupported
			break;
		}
		num_PDUs += Ul_ttiRequest.PRACH_PDU[i].NumPrachOcas; // Number of PRACH PDUs (Time)
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RACH_indication<<56) + (uint64_t)no_of_api_msgs + (uint64_t)((uint64_t)FAPI_handle<<8) + (uint64_t)((uint64_t)Msg_ID<<16);
	fprintf(fp, "\nHardware packet 1 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RACH_indication<<56) + (uint64_t)Msg_len;
	fprintf(fp, "Hardware packet 2 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RACH_indication<<56) + (uint64_t)SFN + (uint64_t)((uint64_t)Slot<<16);
	fprintf(fp, "Hardware packet 3 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RACH_indication<<56) + (uint64_t)num_PDUs; // Number of PDUs
	fprintf(fp, "Hardware packet 4 for header is : 0X%lX\n", HW_OP[HW_OP_idx-1]);



	printf("Number of PRACH freq occasions: %d\n", nPRACH);
	printf("Number of PRACH time occasions: %d\n", num_PDUs);
	// printf("System Frame Number: %d\n", SFN);
	// printf("Slot: %d\n", Slot);

	fprintf(fp, "no_of_api_msgs is : %d \n", no_of_api_msgs);
	fprintf(fp, "nFAPI_handle is : %d \n", FAPI_handle);
	fprintf(fp, "Msg_ID is : %d \n", Msg_ID);
	fprintf(fp, "Msg_Len is : %d \n", Msg_len);
	fprintf(fp, "SFN is : %d \n", SFN);
	fprintf(fp, "Slot is : %d \n", Slot);
	fprintf(fp, "Num PDUs is : %d \n", num_PDUs);





	int cell_id, symb_index, slot_index, freq_index;
	// symb_index0 = Ul_ttiRequest.PRACH_PDU[0].prachStartSymbol;
	for (int i = 0; i< nPRACH; i++){



		// printf("PRACH duration: %d\n", prach_duration);

		cell_id = Ul_ttiRequest.PRACH_PDU[i].physCellID;
		slot_index = Slot;
		freq_index = Ul_ttiRequest.PRACH_PDU[i].numRa;

		for (int j = 0; j<  Ul_ttiRequest.PRACH_PDU[i].NumPrachOcas; j++){


			symb_index = Ul_ttiRequest.PRACH_PDU[i].prachStartSymbol + j*prach_duration[i];


			// printf("cell ID %d\n", Ul_ttiRequest.PRACH_PDU[i].physCellID);
			// printf("Symb ID %d\n", Ul_ttiRequest.PRACH_PDU[i].prachStartSymbol);


			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RACH_indication<<56) + (uint64_t)cell_id + (uint64_t)((uint64_t)symb_index<<16) + (uint64_t)((uint64_t)slot_index<<24);
			fprintf(fp, "\nHardware packet 1 for PDU is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RACH_indication<<56) + (uint64_t)freq_index;
			fprintf(fp, "Hardware packet 2 for PDU is : 0X%lX\n", HW_OP[HW_OP_idx-1]);

			fprintf(fp, "Phy Cell ID is : %d \n", cell_id);
			fprintf(fp, "Symb Index is : %d \n", symb_index);
			fprintf(fp, "Slot Index is : %d \n", slot_index);
			fprintf(fp, "Freq Index is : %d \n", freq_index);
		}
	}
	// printf("%d\n", HW_OP_idx);
}

void generate_PRACH_PDU_length(){
	int index, tdoccasions;
	for(index =0;index < Ul_tti_intermediate.nPRACH; index++){
		tdoccasions = Ul_ttiRequest.PRACH_PDU[index].NumPrachOcas;
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_PRACH_PDU_LENGTH<<56) + (uint64_t)tdoccasions;
		fprintf(fp, "Hardware packet for PRACH PDU length is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "Number of time domain occasions are : %d \n", tdoccasions);
		//fprintf(fp, "User no. is  : %d \n", index);
	}
}

void generate_UCI_indication(){

	int no_of_api_msgs,FAPI_handle, Msg_ID, Msg_len = 6, SFN, Slot, nUCIs, i, pucchformat, PDU_type, PDU_format_type, PDU_bitmap;
	int RNTI, UCI_handle, BitLenHarq, BitLenCsiPart1, BitLenCsiPart2, BitLenSR[Ul_ttiRequest.nULCCH];
	int SRflag[Ul_ttiRequest.nULCCH], HARQflag[Ul_ttiRequest.nULCCH], CsiPart1flag[Ul_ttiRequest.nULCCH], CsiPart2flag[Ul_ttiRequest.nULCCH];
	int PDU_Size[Ul_ttiRequest.nULCCH];

	no_of_api_msgs = 1;
	FAPI_handle = 0;
	Msg_ID = 0x87;


	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)no_of_api_msgs + (uint64_t)((uint64_t)FAPI_handle<<8) + (uint64_t)((uint64_t)Msg_ID<<16);
	fprintf(fp, "\nHardware packet 1 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "No of API msgs %d\n", no_of_api_msgs);
	fprintf(fp, "FAPI Handle %d\n", FAPI_handle);
	fprintf(fp, "Message ID %d\n", Msg_ID);


	nUCIs = Ul_ttiRequest.nULCCH;

	// for PDU size and message length
	for(i=0; i<nUCIs; i++){
		pucchformat = Ul_ttiRequest.PUCCH_PDU[i].FormatType;
		if(pucchformat == 0 || pucchformat == 1){
			SRflag[i] = Ul_ttiRequest.PUCCH_PDU[i].SRFlag;
		}
		else{
			SRflag[i] = 0;
			BitLenSR[i] = 0;
			// Currently not supported
		}

		if(Ul_ttiRequest.PUCCH_PDU[i].BitLenHarq>0){
			HARQflag[i] = 1;
		}
		else{
			HARQflag[i] = 0;
		}

		if(Ul_ttiRequest.PUCCH_PDU[i].BitLenCsiPart1>0){
			CsiPart1flag[i] = 1;
		}
		else{
			CsiPart1flag[i] = 0;
		}

		if(Ul_ttiRequest.PUCCH_PDU[i].BitLenCsiPart2>0){
			CsiPart2flag[i] = 1;
		}
		else{
			CsiPart2flag[i] = 0;
		}


		if(pucchformat == 0 || pucchformat == 1){
			PDU_Size[i] = 17 + 2*SRflag[i] + (2 + Ul_ttiRequest.PUCCH_PDU[i].BitLenHarq)*HARQflag[i];
		}
		else if(pucchformat == 2 || pucchformat == 3 || pucchformat == 4){
			PDU_Size[i] = 17 + (2+ceil((double)BitLenSR[i]/8.0))*SRflag[i] +
						(3+ceil((double)Ul_ttiRequest.PUCCH_PDU[i].BitLenHarq/8.0))*HARQflag[i] +
						(3+ceil((double)Ul_ttiRequest.PUCCH_PDU[i].BitLenCsiPart1/8.0))*CsiPart1flag[i] +
						(3+ceil((double)Ul_ttiRequest.PUCCH_PDU[i].BitLenCsiPart2/8.0))*CsiPart2flag[i];
		}

		//message length will be added here

		Msg_len += PDU_Size[i];

	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)Msg_len;
	fprintf(fp, "\nHardware packet 2 for PDU is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Message Length %d\n", Msg_len);

	SFN = Ul_ttiRequest.SFN;
	Slot = Ul_ttiRequest.Slot;


	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)SFN + (uint64_t)((uint64_t)Slot<<16);
	fprintf(fp, "\nHardware packet 3 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "SFN %d\n", SFN);
	fprintf(fp, "Slot %d\n", Slot);

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)nUCIs;
	fprintf(fp, "\nHardware packet 4 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Num UCIs %d\n", nUCIs);

	for (i=0; i<nUCIs; i++){

		pucchformat = Ul_ttiRequest.PUCCH_PDU[i].FormatType;
		if(pucchformat == 0 || pucchformat == 1){
			PDU_type = 1;
		} 
		else if(pucchformat == 2 || pucchformat == 3 || pucchformat == 4){
			PDU_type = 2;
		}

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)PDU_type + (uint64_t)((uint64_t)PDU_Size[i]<<16);
		fprintf(fp, "\nHardware packet 5 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "PDU Type %d\n", PDU_type);
		fprintf(fp, "PDU Size %d\n", PDU_Size[i]);

		PDU_format_type = Ul_ttiRequest.PUCCH_PDU[i].FormatType;   // how to find if PUSCH carries UCI?
		// PDU_Size = Ul_ttiRequest.ULPDU.PDUSize; // what should be the index?

		// SRflag = Ul_ttiRequest.PUCCH_PDU[i].SRFlag;
		BitLenHarq = Ul_ttiRequest.PUCCH_PDU[i].BitLenHarq;
		BitLenCsiPart1 = Ul_ttiRequest.PUCCH_PDU[i].BitLenCsiPart1;
		BitLenCsiPart2 = Ul_ttiRequest.PUCCH_PDU[i].BitLenCsiPart2;

		PDU_bitmap = Ul_ttiRequest.PUCCH_PDU[i].SRFlag;
		if (BitLenHarq > 0){
			PDU_bitmap += 2;
		}
		if(PDU_format_type == 2 || PDU_format_type == 3 || PDU_format_type == 4){
			if(BitLenCsiPart1 > 0){
				PDU_bitmap += 4;
			}
			if(BitLenCsiPart2 > 0){
				PDU_bitmap += 8;
			}			
		}


		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)PDU_bitmap;
		fprintf(fp, "\nHardware packet 6 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "PDU Bit Map %d\n", PDU_bitmap);

		UCI_handle = Ul_ttiRequest.PUCCH_PDU[i].Handle;

		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)UCI_handle;
		fprintf(fp, "\nHardware packet 7 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "UCI Handle %d\n", UCI_handle);


		RNTI = Ul_ttiRequest.PUCCH_PDU[i].RNTI;
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_UCI_indication<<56) + (uint64_t)RNTI + (uint64_t)((uint64_t)PDU_format_type<<16);
		fprintf(fp, "\nHardware packet 8 for UCI is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		fprintf(fp, "UCI Handle %d\n", RNTI);
		fprintf(fp, "PUCCH Format %d\n", PDU_format_type);


		/*if(PDU_format_type == 0 || PDU_format_type == 1){
			if (BitLenHarq > 0)
		}
		if(PDU_format_type == 2 || PDU_format_type == 3 || PDU_format_type == 4){
			
		}*/

		
	} // nUCIs loop
} // function loop


/*
Function: generate_srs_grid_config
Author: Vishnu P

	Function to generate the configs for the SRS Grid
	Reference : TS 38.211, 6.4.1.4.3

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file): num_users, start_rb, num_rb, k_bar_tc_1, k_bar_tc_2, ktc, sym_map, num_ports


*/
void generate_srs_grid_config(){
	int start_rb, num_rb, k_bar_tc_1, k_bar_tc_2, ktc, num_ports;
	int n_RRC, b, m_SRS_b, N_b, n_b, sumFreqPos_RB, n_SRS_cs_max, N_symb_SRS;
	int num_srs_configs = Ul_tti_intermediate.nSRS;
	// Number of srs = num_users[4:0]
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_GRID<<56) + (uint64_t)num_srs_configs;
	fprintf(fp, "\nHardware packet for SRS grid, number of users is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "num_users is : %d\n", num_srs_configs);

	for (int i = 0; i < num_srs_configs; ++i){
		uint64_t hw_packet64 = 0;
		SRS_PDU_t cur_srs = Ul_ttiRequest.SRS_PDU[sorted_SRS_idx[i]];
		// k_bar_tc[20:18], ktc[23:21], sym_map[29:24]

		/* start_rb into [8:0] */
		// B_SRS = cur_srs.bandwidthIndex
		n_RRC = cur_srs.frequencyPosition;
		// compute the frequency position term which depends upon n_RRC (in RB units)
		sumFreqPos_RB = 0;
		// Check if Frequency hopping enabled or not
		if( cur_srs.frequencyHopping < cur_srs.bandwidthIndex){ // b_hop < B_SRS
			// Frequency hopping disabled
			// TODO: Not supported now, need to add code here
			printf("Error SRS config param: frequencyHopping -- Frequency hopping is unsupported but present in fapi param \n");
		}
		else{
			// Frequency hopping disabled
			for(b = 0; b <= cur_srs.bandwidthIndex; ++b){
				m_SRS_b = srs_BW_config[cur_srs.configIndex][b].m;
				N_b = srs_BW_config[cur_srs.configIndex][b].N;
				n_b = ((int)((double)n_RRC*4/(double)m_SRS_b));
				n_b = n_b%N_b;
				sumFreqPos_RB = sumFreqPos_RB + m_SRS_b * n_b;
			}
		}
		start_rb = cur_srs.frequencyShift + sumFreqPos_RB;
		hw_packet64 = hw_packet64 + (uint64_t)start_rb;

		/* num_rb into [17:9] */
		num_rb = srs_BW_config[cur_srs.configIndex][cur_srs.bandwidthIndex].m; // m_SRS_b
		hw_packet64 = hw_packet64 + ((uint64_t)num_rb<<9);

		/* k_bar_tc_1 into [19:18] */
		k_bar_tc_1 = cur_srs.combOffset;
		hw_packet64 = hw_packet64 + ((uint64_t)k_bar_tc_1<<18);

		/* ktc from combSize into [22]*/
		if(1 == cur_srs.combSize){
			ktc = 4;
			n_SRS_cs_max = 12;
			hw_packet64 = hw_packet64 + ((uint64_t)1<<22);
		}
		else{ // if cur_srs.combSize == 0
			ktc = 2;
			n_SRS_cs_max = 8;
		}

		/* k_bat_tc_2 into [21:20]*/
		k_bar_tc_2 = k_bar_tc_1; // will be modified for n_srs_cs >= (n_SRS_cs_max/2) and num_ports == 4
		/* num_ports (N_ap_SRS) into [31:29]*/
		if(2==cur_srs.numAntPorts){
			num_ports = 4;
			if(cur_srs.cyclicShift >= (n_SRS_cs_max/2)){
				k_bar_tc_2 = (k_bar_tc_2 + (ktc/2))%ktc; // modified
			}
		}
		else if(1==cur_srs.numAntPorts){
			num_ports = 2;
		}
		else{
			num_ports = 1;
		}
		hw_packet64 = hw_packet64 + ((uint64_t)k_bar_tc_2<<20);
		hw_packet64 = hw_packet64 + ((uint64_t)num_ports<<29);

		/* sym_map into [28:23]*/
		if(cur_srs.numSymbols == 2){
			N_symb_SRS = 4;
		}
		else if(cur_srs.numSymbols == 1){
			N_symb_SRS = 2;
		}
		else{
			N_symb_SRS = 1;
		}
		int sym_map[6]={0,0,0,0,0,0};
		uint64_t sym_map64 = 0;
		if(cur_srs.timeStartPosition>7){
			for(int l_dash=0; l_dash < N_symb_SRS; ++l_dash){
				sym_map[cur_srs.timeStartPosition-8+l_dash]=1;
				sym_map64 = sym_map64 + ((uint64_t)1<<(cur_srs.timeStartPosition-8+l_dash+23));
			}
		}
		else{
			// Not supported or error
			printf("Error SRS config param: timeStartPosition -- timeStartPosition < 8 is not supported or invalid \n");
		}
		hw_packet64 = hw_packet64 + sym_map64;
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_GRID<<56) + hw_packet64;

		fprintf(fp, "\nHardware packet for SRS grid for user %d is : 0X%lX\n", sorted_SRS_idx[i], HW_OP[HW_OP_idx-1]);
		fprintf(fp, "start_rb[8:0] is : %d\n", start_rb);
		fprintf(fp, "num_rb[17:9] is : %d\n", num_rb);
		fprintf(fp, "k_bar_tc_1[19:18] is : %d\n", k_bar_tc_1);
		fprintf(fp, "k_bar_tc_2[21:20] is : %d\n", k_bar_tc_2);
		fprintf(fp, "ktc[22] is : %d\n", ktc);
		fprintf(fp, "sym_map[28:23] is : %d%d%d%d%d%d\n", sym_map[5],sym_map[4],sym_map[3],sym_map[2],sym_map[1],sym_map[0]);
		fprintf(fp, "num_ports[31:29] is : %d\n", num_ports);
	}
}

/*
Function: generate_srs_seq_config
Author: Vishnu P

	Function to generate the configs for the SRS sequence generation
	Reference : TS 38.211, 6.4.1.4.2

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file): num_users, alpha_dash_0, alpha_dash_1, alpha_dash_2, alpha_dash_3, (Nzc, q) or phi_table_entries
*/
void generate_srs_seq_config(){
	int num_srs_configs = Ul_tti_intermediate.nSRS;
	int n_SRS_cs_max, N_ap_SRS, n_srs_cs, n_srs_cs_dash, n_srs_cs_i, m_SRS_b, M_zc;
	uint64_t hw_packet64;
	// Number of srs = num_users[4:0]
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_SEQ<<56) + (uint64_t)num_srs_configs;
	fprintf(fp, "\nHardware packet for SRS generation,  number of users is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "num_users is : %d\n",num_srs_configs);

	for (int i = 0; i < num_srs_configs; ++i){
		SRS_PDU_t cur_srs = Ul_ttiRequest.SRS_PDU[sorted_SRS_idx[i]];
		m_SRS_b = srs_BW_config[cur_srs.configIndex][cur_srs.bandwidthIndex].m;

		int alpha_dash[4]={0,0,0,0};
		double alpha_dash_tmp[4] = {0.0,0.0,0.0,0.0};

		// Need n_srs_cs, n_SRS_cs_max, N_ap_SRS,
		n_srs_cs = cur_srs.cyclicShift;
		if(1 == cur_srs.combSize){ // K_TC = 4
			n_SRS_cs_max = 12;
			M_zc = m_SRS_b * 3; // length of srs sequence
		}
		else{ // if cur_srs.combSize == 0 (K_TC = 2)
			n_SRS_cs_max = 8;
			M_zc = m_SRS_b * 6; // length of srs sequence
		}
		if(2==cur_srs.numAntPorts){
			// N_ap_SRS = 4;
			n_srs_cs_dash = n_SRS_cs_max / 4;
		}
		else if(1==cur_srs.numAntPorts){
			// N_ap_SRS = 2;
			n_srs_cs_dash = n_SRS_cs_max / 2;
		}
		else{
			// N_ap_SRS = 1;
			n_srs_cs_dash = n_SRS_cs_max;
		}
		for(int port_idx=0;port_idx<4;++port_idx){
			n_srs_cs_i = (n_srs_cs+n_srs_cs_dash*port_idx)%n_SRS_cs_max;
			alpha_dash_tmp[port_idx] = (double)(2.0*n_srs_cs_i)/(double)n_SRS_cs_max;
			// convert to fixed point ap_fixed(24,2) format
			// 2^22 = 4194304
			alpha_dash[port_idx]=(int)(alpha_dash_tmp[port_idx]*4194304);
		}

		/* alpha_dash_0[23:0], alpha_dash_1[47:24] */
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_SEQ<<56) + ((uint64_t)alpha_dash[1]<<24)+((uint64_t)alpha_dash[0]);
		fprintf(fp, "\nHardware packet 1 for SRS generation for user %d is : 0X%lX\n", sorted_SRS_idx[i], HW_OP[HW_OP_idx-1]);
		fprintf(fp, "alpha_dash_0[8:0] is : %f\n", alpha_dash_tmp[0]);
		fprintf(fp, "alpha_dash_1[47:24] is : %f\n", alpha_dash_tmp[1]);

		/* alpha_dash_2[23:0], alpha_dash_3[47:24] */
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_SEQ<<56) + ((uint64_t)alpha_dash[3]<<24)+((uint64_t)alpha_dash[2]);
		fprintf(fp, "\nHardware packet 2 for SRS generation for user %d is : 0X%lX\n", sorted_SRS_idx[i], HW_OP[HW_OP_idx-1]);
		fprintf(fp, "alpha_dash_2[8:0] is : %f\n", alpha_dash_tmp[2]);
		fprintf(fp, "alpha_dash_3[47:24] is : %f\n", alpha_dash_tmp[3]);

		/* Nzc,q or phi_table_entry */
		hw_packet64 = 0;
		int u, Nzc, q;
		if( 0 == cur_srs.groupOrSequenceHopping){
			u = cur_srs.sequenceId % 30;
			if(M_zc >= 36){ // Nzc,q
				// find prime number Nzc such that Nzc < M_zc

				//-- code to find prime number less than M_zc - start --//
				/*Nzc = M_zc-1, isFactor, n_i;
				while(1){
					isFactor = 0;
					for(n_i=2;n_i<=(Nzc/2);++n_i){
						if( 0 == (Nzc % n_i)){
							isFactor = 1;
							break;
						}

					}
					if(0 == isFactor){
						break;
					}
					else{
						Nzc = Nzc - 1;
					}
				}*/
				//-- code to find prime number less than M_zc - end --//

				// Alternatively take Nzc from lookup table (Otherwise uncomment above code)
				if(1 == cur_srs.combSize){ // K_TC = 4
					Nzc = srs_Nzc_table_K_TC4[cur_srs.configIndex][cur_srs.bandwidthIndex];
				}
				else{ // K_TC = 2
					Nzc = srs_Nzc_table_K_TC2[cur_srs.configIndex][cur_srs.bandwidthIndex];
				}

				/* Nzc[10:0] */
				hw_packet64 = hw_packet64 + ((uint64_t)Nzc);

				/* q[31:16] */
				q = (int)((((double)Nzc*(u+1))/31.0)+0.5);
				hw_packet64 = hw_packet64 + ((uint64_t)q<<16);
			}
			else{
				if(M_zc == 24){ // phi_table_entries
					for(int n_i=0; n_i<24; ++n_i){
						hw_packet64 = hw_packet64 + ((uint64_t)srs_phi_table_M_zc24[u][n_i]<<(n_i*2));
					}
				}
				else if(M_zc == 12){ // phi_table_entries
					for(int n_i=0; n_i<12; ++n_i){
						hw_packet64 = hw_packet64 + ((uint64_t)srs_phi_table_M_zc12[u][n_i]<<(n_i*2));
					}
				}
				else{
					printf("Error SRS config : Invalid SRS sequence length, M_zc = %d \n", M_zc);
				}
			}
		}
		else{
			// TODO: groupOrSequenceHopping not supported now, need to code here for the support
			printf("Error SRS config param: groupOrSequenceHopping -- Group and sequence hopping are unsupported but present in fapi param \n");
		}
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_SEQ<<56) + hw_packet64;
		fprintf(fp, "\nHardware packet 3 for SRS generation for user %d is : 0X%lX\n", sorted_SRS_idx[i], HW_OP[HW_OP_idx-1]);
		if(M_zc >= 36){
			fprintf(fp, "Nzc[10:0] is : %d\n", Nzc);
			fprintf(fp, "q[31:16] is : %d\n", q);
		}
		else if(M_zc == 24){ // phi_table_entries
			fprintf(fp, "phi_table_entries are :");
			for(int n_i=0; n_i<24; ++n_i){
				hw_packet64 = hw_packet64 + ((uint64_t)srs_phi_table_M_zc24[u][n_i]<<(n_i*2));
				fprintf(fp, " %d", srs_phi_table_M_zc24[u][n_i]);
			}
			fprintf(fp, "\n");
		}
		else if(M_zc == 12){ // phi_table_entries
			fprintf(fp, "phi_table_entries are :");
			for(int n_i=0; n_i<12; ++n_i){
				hw_packet64 = hw_packet64 + ((uint64_t)srs_phi_table_M_zc12[u][n_i]<<(n_i*2));
				fprintf(fp, " %d", srs_phi_table_M_zc12[u][n_i]);
			}
			fprintf(fp, "\n");
		}
	}
}

/*
Function: generate_srs_fapi_out_config
Author: Vishnu P

	Function to generate the configs for the SRS SNR Fapi out module

	Inputs:
		None. Requires the presence of the variable Ul_ttiRequest and Ul_tti_intermediate_t.

	Output (to file): num_users, num_symbols, num_rb
*/
void generate_srs_fapi_out_config(){
	int num_srs_configs = Ul_tti_intermediate.nSRS;
	int num_symbols, num_rb;
	uint64_t hw_packet64;
	// Number of srs = num_users[4:0]
	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_FAPI_OUT<<56) + (uint64_t)num_srs_configs;
	fprintf(fp, "\nHardware packet for SRS SNR FAPI Out,  number of users is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "num_users is : %d\n",num_srs_configs);

	for (int i = 0; i < num_srs_configs; ++i){
		SRS_PDU_t cur_srs = Ul_ttiRequest.SRS_PDU[sorted_SRS_idx[i]];
		num_symbols = (int)(1<<cur_srs.numSymbols);
		num_rb = srs_BW_config[cur_srs.configIndex][cur_srs.bandwidthIndex].m;
		// num_symbols[2:0]
		// num_rb[11:3]
		HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_SRS_FAPI_OUT<<56) + ((uint64_t)num_symbols) +((uint64_t)num_rb<<3);
		fprintf(fp, "\nHardware packet for user %d is : 0X%lX\n", i, HW_OP[HW_OP_idx-1]);
		fprintf(fp, "num_symbols is : %d\n",num_symbols);
		fprintf(fp, "num_rb is : %d\n",num_rb);
	}
}



void generate_HARQ_config_user(){

	int n_id, rnti_user, p_id , tb_size, tb_type;
	int total_users_per_slot = Ul_tti_intermediate.nPUSCH;
	int total_cb_per_slot = Ul_tti_intermediate.nCB;

	printf("Number of users per slot %d\n",total_users_per_slot);
	printf("Number of CBs per slot %d\n",total_cb_per_slot);



	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_HARQ<<56)+(uint64_t)((uint64_t)total_users_per_slot)+(uint64_t)((uint64_t)total_cb_per_slot<<8);
	fprintf(fp, "\nHardware packet 1 for HARQ is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "Total users per slot %d\n", total_users_per_slot);
	fprintf(fp, "Total codeblocks per slot %d\n", total_cb_per_slot);

	for(int i=0; i < MaxUE; i++){
		if(i<total_users_per_slot){

			n_id = (int)Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[i]].dataScramblingId;
			rnti_user = (int)Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[i]].RNTI;

			tb_size = (int)Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[i]].puschData.TBSize*8;
			if (tb_size <= 1048576){
					tb_type = 0;
				}
				else{
					tb_type = 1;
				}

			p_id = Ul_ttiRequest.PUSCH_PDU[sorted_PUSCH_idx[i]].puschData.harqProcessID;


			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_HARQ<<56) + (uint64_t)rnti_user + (uint64_t)((uint64_t)p_id<<16) + (uint64_t)((uint64_t)n_id<<20) + (uint64_t)((uint64_t)tb_type);
			fprintf(fp, "Hardware packet 2 for HARQ is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			fprintf(fp, "rnti_user : %d \n",  rnti_user);
			fprintf(fp, "n_id_user : %d \n",  n_id);
			fprintf(fp, "p_id_user : %d \n",  p_id);
			//fprintf(fp, "tb_size%d : %d \n",  tb_size[i]);
			fprintf(fp, "tb_type: %d \n",  tb_type);
		}
		else{
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_HARQ<<56);
			fprintf(fp, "Hardware packet 2 for HARQ is: 0X%lX\n", HW_OP[HW_OP_idx-1]);
		}
	}

}

void generate_HARQ_config_cb(int cb_idx){


	int rvid, cb_user, cb_size, CB_size, cb_type,cb_in_tb;

	cb_user = CB_prop[cb_idx].TB_idx; // corresponding user index
	cb_size = CB_prop[cb_idx].cb_size; // check.
	cb_in_tb = CB_prop[cb_idx].CB_in_TB; // cb index in that particular user.


	rvid = (int)Ul_tti_intermediate.TB_prop[cb_user].rvIndex;

	if(cb_size <= 16000){
		cb_type = 0;
	}
	else{
		cb_type =1;
	}

	HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_HARQ<<56) + (uint64_t)cb_user + (uint64_t)((uint64_t)cb_size<<5) + (uint64_t)((uint64_t)rvid<<20) + (uint64_t)((uint64_t)cb_type<<22) + (uint64_t)((uint64_t)cb_in_tb<<23);
	fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
	fprintf(fp, "rvid_user : %d \n",  rvid);
	fprintf(fp, "cb_user : %d \n",  cb_user);
	fprintf(fp, "cb_size : %d \n",  cb_size);
	fprintf(fp, "cb_type : %d \n", cb_type);
	fprintf(fp, "cb_no : %d\n", cb_in_tb);
}


void ul_config(){
	HW_OP_idx = 0;
	int i;
	fprintf(fp, "\n ====================== Begin Packets for SFN = %d, Slot = %d ================  \n \n", Ul_ttiRequest.SFN, Ul_ttiRequest.Slot);
	printf("\n ====================== Begin Packets for SFN = %d, Slot = %d ================  \n \n", Ul_ttiRequest.SFN, Ul_ttiRequest.Slot);
	fprintf(fp, "Total PDU count = %d \n", Ul_ttiRequest.nPDUs);
	fprintf(fp, "PUSCH PDU count = %d \n", Ul_tti_intermediate.nPUSCH);
	fprintf(fp, "SRS PDU count = %d \n", Ul_tti_intermediate.nSRS);
	fprintf(fp, "PUCCH PDU count = %d \n", Ul_tti_intermediate.nPUCCH);
	fprintf(fp, "PRACH PDU count = %d \n", Ul_tti_intermediate.nPRACH);

	//Calling the sorting function
	sort_PDU_UL();

	// Calling the RE demultiplexing function
	fprintf(fp, "\n\n===================RE demapper configurations===================\n\n");
	generate_RE_demapper_config();
	printf("Number of hardware packets after RE demapper is : %d\n", HW_OP_idx);

	// Generate Time Domain Occasions for PRACH
	if(Ul_tti_intermediate.nPRACH != 0){
		fprintf(fp, "\n\n===================PRACH PDU length===================\n\n");
		generate_PRACH_PDU_length();
	}

	// Generate configs for SRS
	if(Ul_tti_intermediate.nSRS > 0){
		fprintf(fp, "\n\n=====================SRS configurations======================\n");
		generate_srs_grid_config();
		generate_srs_seq_config();
		generate_srs_fapi_out_config();
		printf("Number of hardware packets after SRS configs : %d\n", HW_OP_idx);
	}

	// PUCCH configurations
	if(Ul_tti_intermediate.nPUCCH > 0){
		int pucch_f2_count = 0, pucch_f0_count = 0;

		for(int ii=0; ii<Ul_tti_intermediate.nPUCCH; ii++){
			if(Ul_ttiRequest.PUCCH_PDU[ii].FormatType == 0){
				pucch_f0_count += 1;
			}
			else if(Ul_ttiRequest.PUCCH_PDU[ii].FormatType == 2){
				pucch_f2_count += 1;
			}
		}

		fprintf(fp, "\n\n=====================PUCCH configurations=======================\n");
		if(pucch_f0_count > 0){
			fprintf(fp, "\n\n=====================PUCCH F0 interleaver configurations=======================\n");
			generate_pucchF0_UE_interleave_config();
			fprintf(fp, "\n\n=====================PUCCH F0 configurations=======================\n");
			generate_pucchF0_config();
		}
		if(pucch_f2_count > 0){
			fprintf(fp, "\n\n=====================PUCCH F2 interleaver configurations=======================\n");
			generate_pucchF2_UE_interleave_config();
			fprintf(fp, "\n\n=====================PUCCH F2 DMRS configurations=======================\n");
			generate_pucchF2_dmrs();
			fprintf(fp, "\n\n=====================PUCCH F2 descrambler configurations=======================\n");
			generate_pucchF2_descrambler();
			fprintf(fp, "\n\n=====================PUCCH F2 configurations=======================\n");
			generate_pucchF2_config();
			printf("Number of hardware packets after PUCCH configs : %d\n", HW_OP_idx);
		}
	}

	// Generate configs for DMRS
	if(Ul_tti_intermediate.nPUSCH > 0){
		fprintf(fp, "\n\n=====================DMRS configurations=======================\n");
		for (i = 0; i < Ul_tti_intermediate.nPUSCH; ++i)
			generate_dmrs_config_UL(sorted_DMRS_idx[i]);
		for (; i < MaxUE; ++i){
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DMRS<<56);
			fprintf(fp, "\nHardware packet1 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DMRS<<56);
			fprintf(fp, "Hardware packet2 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
			HW_OP[HW_OP_idx++] = (uint64_t)((uint64_t)ADDRESS_RX_DMRS<<56);
			fprintf(fp, "Hardware packet3 is : 0X%lX\n", HW_OP[HW_OP_idx-1]);
		}
		printf("Number of hardware packets after DMRS configs : %d\n", HW_OP_idx);
	}

	// Call the IP config gen functions
	if(Ul_tti_intermediate.nPUSCH > 0){
		fprintf(fp, "\n\n=====================CBS configurations========================\n");
		cbs_configs_UL();
		printf("Number of hardware packets after CBS ordering : %d\n", HW_OP_idx);
	}

	// Generate configs for channel equalization and estimation modules
	if(Ul_tti_intermediate.nPUSCH > 0){
		fprintf(fp, "\n\n=====================Channel Estimation configurations=======================\n");
		generate_ch_eq_config();
		printf("Number of hardware packets after Channel Equalization configs : %d\n", HW_OP_idx);
	}

	if(Ul_tti_intermediate.nPUSCH > 0){
		fprintf(fp, "\n\n====================PUSCH configurations=====================\n\n");
		/******** merge harq changes after hardware modification ******
		// Harq
		generate_HARQ_config_user();
		*******************/

		for (int i = 0; i < Ul_tti_intermediate.nCB; ++i){
			int CB_index_test = sorted_CB_idx[i];
			int TB_index_test = CB_prop[CB_index_test].TB_idx;
			int PUSCH_pdu_index_test = Ul_tti_intermediate.TB_prop[TB_index_test].user_id;

			fprintf(fp, "CB index: %d \n\n", CB_index_test);
			fprintf(fp, "PUSCH PDU index: %d \n", PUSCH_pdu_index_test);
			fprintf(fp, "UE index: %d \n", TB_index_test);
			fprintf(fp, "CB index in TB: %d \n\n", CB_prop[CB_index_test].CB_in_TB);
			fprintf(fp, "=======================\n");
			// Testing the generate_ldpc_config_UL function
			fprintf(fp, "Configuration for LDPC module- \n");
			generate_ldpc_config_UL(PUSCH_pdu_index_test, TB_index_test, CB_prop[CB_index_test].CB_in_TB, CB_index_test);
			fprintf(fp, "=======================\n");
			// Testing the generate_filler_bit_remover_config function
			fprintf(fp, "Configuration for filler bit remover module- \n");
			generate_filler_bit_remover_config(TB_index_test);
			fprintf(fp, "=======================\n");
			// Testing the generate_depuncturing_config function
			fprintf(fp, "Configuration for depuncturing module- \n");
			generate_depuncturing_config(TB_index_test, CB_prop[CB_index_test].CB_in_TB);
			fprintf(fp, "=======================\n");
			// Testing the generate_rate_dematching_config function
			fprintf(fp, "Configuration for rate dematching module- \n");
			generate_rate_dematching_config(TB_index_test, CB_prop[CB_index_test].CB_in_TB);
			fprintf(fp, "=======================\n");
			// Testing the generate_deinterleaver_config function
			fprintf(fp, "Configuration for deinterleaver module- \n");
			generate_deinterleaver_config(TB_index_test, CB_prop[CB_index_test].CB_in_TB);
			fprintf(fp, "=======================\n");
			fprintf(fp,"Configurations for HARQ module- \n");
			printf("\n cb index = %d,user number is %d, cb size = %d, cb in user is %d\n",i,CB_prop[i].TB_idx,CB_prop[i].cb_size, CB_prop[i].CB_in_TB);
			/******** merge harq changes after hardware modification ******
			generate_HARQ_config_cb(CB_index_test);
			********/
			fprintf(fp, "=========================================================\n\n");
		}
		// Generate configs for UE interelaver
		fprintf(fp, "\n\n=====================User Interleaver configurations=======================\n");
		generate_UE_interleave_config();
		printf("Number of hardware packets after UE interleaver configs : %d\n", HW_OP_idx);

		fprintf(fp, "\n\n=====================Remaining PUSCH configurations=======================\n");
		fprintf(fp, "Configuration for descrambler module- \n");
		generate_descrambler_config();
		fprintf(fp, "=======================\n");

		fprintf(fp, "Configuration for LLR gen module- \n");
		generate_LLR_gen_config();
		fprintf(fp, "=======================\n");

		fprintf(fp, "\n\n=====================TB and CB CRC configurations=======================\n");
		generate_CBCRC_config();
		generate_TBCRC_config();
		printf("Number of hardware packets after TB and CB CRC check configs : %d\n", HW_OP_idx);
	}



        /********* commented ORAN configs now for testing, add it later ************/
	//fprintf(fp, "\n\n=================================================ORAN configurations===================================================\n\n");
	// generate_UL_ORAN_config();
	//printf("Number of hardware packets after ORAN : %d\n", HW_OP_idx);
        /*********************************************/

	if(Ul_tti_intermediate.nPUSCH > 0 ){
		fprintf(fp, "\n\n=================================================RX_Data_indication===================================================\n\n");
		generate_RX_Data_indication();
		printf("Number of hardware packets after RX_Data_indication : %d\n", HW_OP_idx);
	}

	if(Ul_tti_intermediate.nPRACH > 0){
		fprintf(fp, "\n\n=================================================RACH_indication===================================================\n\n");
		generate_RACH_indication();
		printf("Number of hardware packets after RACH_indication : %d\n", HW_OP_idx);
	}

	if(Ul_tti_intermediate.nPUCCH > 0){
		fprintf(fp, "\n\n=================================================UCI_indication===================================================\n\n");
		generate_UCI_indication();
		printf("Number of hardware packets after UCI_indication : %d\n", HW_OP_idx);
	}

	printf("\n ====================== End Packets for SFN = %d, Slot = %d ================  \n \n", Ul_ttiRequest.SFN, Ul_ttiRequest.Slot);
	fprintf(fp, "\n ====================== End Packets for SFN = %d, Slot = %d ================  \n \n", Ul_ttiRequest.SFN, Ul_ttiRequest.Slot);

	// Write hardware packets to hardware packets only file
	for (int i = 0; i < HW_OP_idx; ++i){
		fprintf(fp_configsONLY, "0X%lX\n", HW_OP[i]);
	}
}
