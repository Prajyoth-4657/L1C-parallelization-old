#include "getBAUL.h"


idx_t BA_TABLE_QI[] ={
		   0,    0,    0,    0,    3,    2,    1,    0,    7,    6,    5,    3,    4,    2,    1,    0,
		  15,   14,   13,   11,    7,   12,   10,    6,    9,    5,    3,    8,    4,    2,    1,    0,
		  31,   30,   29,   27,   23,   15,   28,   22,   25,   26,   21,   14,   13,   19,   11,    7,
		  24,   20,   12,   18,   10,   17,    6,    9,    5,    3,   16,    8,    4,    2,    1,    0,
		  63,   62,   61,   59,   55,   47,   31,   60,   58,   57,   54,   53,   46,   51,   45,   30,
		  43,   29,   39,   27,   56,   23,   52,   15,   50,   44,   49,   42,   28,   41,   38,   22,
		  25,   37,   26,   35,   21,   14,   48,   13,   19,   40,   11,    7,   36,   24,   34,   20,
		  33,   12,   18,   10,   17,    6,    9,    5,    3,   32,   16,    8,    4,    2,    1,    0,
		 127,  126,  125,  123,  119,  111,   95,  124,   63,  122,  121,  118,  117,  110,  115,  109,
		  94,  107,   93,  103,   62,  120,   91,   61,  116,   87,  114,   59,  108,   79,  113,   55,
		 106,   47,   92,  105,  102,   90,   31,  101,   89,   60,   86,   99,   58,   85,   78,  112,
		  57,   83,   54,   77,   53,  104,   75,   46,   51,  100,   45,   71,   30,   88,   98,   43,
		  29,   84,   39,   97,   27,   56,   82,   76,   23,   52,   15,   73,   50,   81,   44,   70,
		  74,   49,   42,   69,   28,   41,   67,   96,   38,   22,   25,   37,   80,   26,   35,   21,
		  72,   14,   48,   13,   19,   68,   40,   11,   66,    7,   36,   24,   34,   20,   65,   33,
		  12,   18,   10,   17,    6,    9,   64,    5,    3,   32,   16,    8,    4,    2,    1,    0,
		 255,  254,  253,  247,  251,  239,  223,  252,  250,  249,  246,  191,  127,  245,  238,  243,
		 237,  222,  235,  221,  190,  248,  231,  219,  215,  189,  244,  242,  126,  236,  187,  207,
		 241,  125,  234,  183,  220,  123,  175,  233,  230,  218,  119,  159,  229,  217,  188,  214,
		 111,  227,  186,  213,   95,  206,  240,  185,  211,  182,  205,  124,  232,  181,   63,  203,
		 122,  174,  228,  179,  199,  121,  173,  118,  158,  216,  226,  171,  212,  117,  110,  157,
		 225,  167,  115,  184,  109,  210,  155,  204,   94,  209,  151,  180,  107,  202,   93,  178,
		 103,  143,   62,  201,  120,  172,  198,   91,  177,   61,  170,  116,  197,   87,  156,  114,
		 169,   59,  195,  166,  224,  108,   79,  154,  113,   55,  106,  165,  153,  150,  208,   47,
		  92,  163,  105,  102,  149,   90,  200,   31,  142,  176,  147,  101,  141,  196,   89,   60,
		  86,   99,  139,  168,   58,   85,  194,   78,  135,  112,   57,   83,   54,  164,   77,  152,
		 193,   53,  162,  104,   75,   46,  148,   51,  100,   45,  161,   71,  146,   30,  140,   88,
		  98,   43,   29,  145,  138,   84,   39,   97,   27,   56,   82,  137,   76,  134,   23,   52,
		 133,   15,   73,   50,   81,  131,   44,   70,  192,  160,   74,   49,   42,   69,   28,  144,
		  41,   67,   96,   38,  136,   22,   25,   37,   80,   26,   35,  132,   21,   72,   14,   48,
		  13,   19,  130,   68,   40,   11,   66,  129,    7,   36,   24,   34,   20,   65,   33,   12,
		 128,   18,   10,   17,    6,    9,   64,    5,    3,   32,   16,    8,    4,    2,    1,    0,
		 511,  510,  509,  507,  503,  495,  508,  479,  506,  505,  447,  501,  494,  502,  499,  493,
		 383,  478,  491,  477,  255,  504,  487,  475,  446,  500,  471,  445,  498,  382,  443,  492,
		 497,  381,  463,  490,  439,  476,  486,  489,  431,  379,  254,  474,  473,  485,  415,  483,
		 470,  444,  375,  253,  367,  247,  469,  441,  442,  462,  251,  438,  467,  351,  496,  461,
		 380,  437,  459,  378,  239,  488,  430,  484,  319,  435,  377,  455,  472,  223,  414,  427,
		 482,  373,  252,  429,  468,  366,  413,  481,  371,  250,  466,  423,  374,  440,  365,  411,
		 249,  460,  350,  246,  465,  436,  407,  191,  127,  363,  458,  245,  349,  434,  399,  457,
		 359,  238,  428,  376,  318,  454,  243,  347,  433,  237,  453,  426,  222,  317,  372,  343,
		 412,  235,  451,  425,  422,  370,  221,  315,  480,  335,  364,  190,  369,  248,  231,  410,
		 421,  311,  219,  409,  362,  464,  406,  419,  348,  215,  361,  189,  244,  303,  405,  358,
		 456,  346,  398,  242,  126,  236,  187,  357,  432,  207,  403,  397,  452,  345,  241,  316,
		 342,  125,  234,  183,  287,  355,  395,  424,  314,  220,  341,  123,  175,  233,  334,  450,
		 313,  391,  230,  368,  218,  339,  119,  333,  310,  420,  159,  229,  408,  217,  449,  188,
		 309,  214,  331,  111,  360,  302,  418,  227,  404,  186,  213,  417,  301,  307,  356,  402,
		 327,   95,  206,  240,  344,  396,  185,  401,  211,  354,  299,  286,  182,  205,  124,  232,
		 285,  295,  181,  394,  340,   63,  203,  353,  448,  122,  283,  393,  174,  390,  312,  338,
		 228,  179,  199,  121,  173,  389,  332,  118,  337,  158,  279,  271,  416,  216,  308,  387,
		 226,  330,  171,  212,  117,  110,  329,  157,  306,  326,  225,  167,  115,  184,  109,  300,
		 305,  210,  155,  325,  352,  400,  298,  204,   94,  284,  209,  151,  180,  107,  297,  392,
		 323,  202,   93,  294,  178,  103,  143,  282,   62,  336,  201,  120,  172,  198,   91,  388,
		 293,  177,  278,  281,   61,  170,  116,  197,   87,  156,  277,  114,  169,   59,  291,  275,
		 270,  195,  166,  224,  108,  269,   79,  154,  113,  328,   55,  106,  165,  153,  150,  386,
		 208,  324,  385,  267,   47,   92,  163,  296,  304,  105,  102,  149,  263,  322,  292,   90,
		 200,   31,  321,  142,  176,  147,  101,  141,  196,  290,   89,  280,   60,   86,   99,  139,
		 168,   58,  276,   85,  194,  289,   78,  135,  112,   57,   83,   54,  274,  268,  164,   77,
		 152,  193,   53,  162,  104,  273,  266,   75,   46,  148,   51,  100,   45,  161,  265,  262,
		  71,  146,   30,  140,   88,   98,   43,   29,  261,  145,  138,   84,  259,   39,   97,   27,
		  56,   82,  137,   76,  384,  134,   23,   52,  133,  320,   15,   73,   50,   81,  131,   44,
		  70,  192,  288,  160,  272,   74,   49,   42,   69,   28,  144,   41,   67,   96,   38,  264,
		 260,  136,   22,   25,   37,   80,   26,  258,   35,  132,   21,  257,   72,   14,   48,   13,
		  19,  130,   68,   40,   11,   66,  129,    7,   36,   24,   34,  256,   20,   65,   33,   12,
		 128,   18,   10,   17,    6,    9,   64,    5,    3,   32,   16,    8,    4,    2,    1,    0,
		1023, 1022, 1021, 1019, 1015, 1007, 1020,  991, 1018, 1017, 1014, 1006,  895, 1013, 1011,  959,
		1005,  990, 1003,  989,  767, 1016,  999, 1012,  987,  958,  983,  957, 1010, 1004,  955, 1009,
		 894,  975,  893, 1002,  951, 1001,  988,  511,  766,  998,  891,  943,  986,  997,  985,  887,
		 956,  765,  995,  927,  982,  981,  879,  954,  974,  763,  953,  979,  510, 1008,  759,  863,
		 950,  892, 1000,  973,  949,  509,  890,  971,  996,  942,  751,  984,  889,  507,  947,  831,
		 886,  967,  941,  764,  926,  980,  994,  939,  885,  993,  735,  878,  925,  503,  762,  883,
		 978,  935,  703,  495,  952,  877,  761,  972,  923,  977,  948,  758,  862,  875,  919,  970,
		 757,  861,  508,  969,  750,  946,  479,  888,  639,  871,  911,  830,  940,  859,  755,  966,
		 945,  749,  506,  884,  938,  965,  829,  734,  924,  855,  505,  747,  963,  937,  882,  934,
		 827,  733,  447,  992,  847,  876,  501,  921,  702,  494,  881,  760,  743,  933,  502,  918,
		 874,  922,  823,  731,  499,  860,  756,  931,  701,  873,  493,  727,  917,  870,  976,  815,
		 910,  383,  968,  478,  858,  754,  699,  491,  869,  944,  748,  638,  915,  477,  719,  909,
		 964,  255,  799,  504,  857,  854,  753,  828,  746,  695,  487,  907,  637,  867,  853,  475,
		 936,  962,  446,  732,  826,  745,  846,  500,  825,  903,  687,  932,  635,  471,  445,  742,
		 880,  498,  730,  851,  822,  382,  920,  845,  741,  443,  700,  729,  631,  492,  872,  961,
		 726,  821,  930,  497,  381,  843,  463,  916,  739,  671,  623,  490,  929,  439,  814,  819,
		 868,  752,  914,  698,  725,  839,  856,  476,  813,  718,  908,  486,  723,  866,  489,  607,
		 431,  697,  379,  811,  798,  913,  575,  717,  254,  694,  636,  474,  807,  715,  906,  797,
		 693,  865,  960,  852,  744,  634,  473,  795,  905,  485,  415,  483,  470,  444,  375,  850,
		 740,  686,  902,  824,  691,  253,  711,  633,  844,  685,  630,  901,  367,  791,  928,  728,
		 820,  849,  783,  670,  899,  738,  842,  683,  247,  469,  441,  442,  462,  251,  737,  438,
		 467,  351,  629,  841,  724,  679,  669,  496,  461,  818,  380,  437,  627,  622,  459,  378,
		 239,  488,  667,  838,  430,  484,  812,  621,  319,  817,  435,  377,  696,  722,  912,  606,
		 810,  864,  716,  837,  721,  714,  809,  796,  455,  472,  619,  835,  692,  663,  223,  414,
		 904,  427,  806,  482,  632,  713,  690,  848,  605,  373,  252,  794,  429,  710,  684,  615,
		 805,  900,  655,  468,  366,  603,  413,  574,  481,  371,  250,  793,  466,  423,  374,  689,
		 628,  440,  365,  709,  789,  803,  411,  573,  682,  249,  460,  790,  668,  599,  350,  707,
		 246,  681,  465,  571,  626,  436,  407,  782,  191,  127,  363,  620,  666,  458,  245,  349,
		 677,  434,  678,  591,  787,  399,  457,  359,  238,  625,  840,  567,  736,  665,  428,  376,
		 781,  898,  618,  675,  318,  454,  662,  243,  897,  347,  836,  816,  720,  433,  604,  617,
		 779,  808,  661,  834,  712,  804,  833,  559,  237,  453,  426,  222,  317,  775,  372,  343,
		 412,  235,  543,  614,  451,  425,  422,  613,  370,  221,  315,  480,  335,  659,  654,  364,
		 190,  369,  248,  653,  688,  231,  410,  602,  611,  802,  792,  421,  651,  601,  598,  708,
		 311,  219,  572,  597,  788,  570,  409,  590,  362,  801,  680,  464,  406,  419,  348,  647,
		 786,  215,  589,  706,  361,  676,  566,  189,  595,  244,  569,  303,  405,  358,  456,  346,
		 398,  565,  242,  126,  705,  780,  587,  624,  664,  236,  187,  357,  432,  785,  558,  674,
		 207,  403,  397,  452,  345,  563,  778,  241,  316,  342,  616,  660,  557,  125,  234,  183,
		 287,  355,  583,  673,  395,  424,  314,  220,  777,  341,  612,  658,  123,  175,  774,  555,
		 233,  334,  542,  450,  313,  391,  230,  652,  368,  218,  339,  600,  119,  333,  657,  610,
		 773,  541,  310,  420,  159,  229,  650,  551,  596,  609,  408,  217,  449,  188,  309,  214,
		 331,  111,  539,  360,  771,  649,  302,  418,  594,  896,  227,  404,  646,  186,  588,  832,
		 568,  213,  417,  301,  307,  356,  402,  800,  564,  327,   95,  206,  240,  535,  593,  645,
		 586,  344,  396,  185,  401,  211,  354,  299,  585,  286,  562,  643,  182,  205,  124,  232,
		 285,  295,  181,  556,  582,  527,  394,  340,   63,  203,  561,  353,  448,  122,  283,  393,
		 581,  554,  174,  390,  704,  312,  338,  228,  179,  784,  199,  553,  121,  173,  389,  540,
		 579,  332,  118,  672,  550,  337,  158,  279,  271,  416,  216,  308,  387,  538,  549,  226,
		 330,  776,  171,  212,  117,  110,  329,  656,  157,  772,  306,  326,  225,  167,  115,  537,
		 534,  184,  109,  300,  547,  305,  210,  155,  533,  325,  352,  608,  400,  298,  204,   94,
		 648,  284,  209,  151,  180,  107,  770,  297,  392,  323,  592,  202,  644,   93,  294,  178,
		 103,  143,  282,   62,  336,  201,  120,  172,  198,  769,  584,   91,  388,  293,  177,  526,
		 278,  281,  642,  525,  531,   61,  170,  116,  197,   87,  156,  277,  114,  560,  169,   59,
		 291,  580,  275,  523,  641,  270,  195,  552,  519,  166,  224,  578,  108,  269,   79,  154,
		 113,  548,  577,  536,  328,   55,  106,  165,  153,  150,  386,  208,  324,  546,  385,  267,
		  47,   92,  163,  296,  304,  105,  102,  149,  263,  532,  322,  292,  545,   90,  200,   31,
		 321,  530,  142,  176,  147,  101,  141,  196,  524,  529,  290,   89,  280,   60,   86,   99,
		 139,  168,   58,  522,  276,   85,  194,  289,   78,  135,  112,  521,   57,   83,   54,  518,
		 274,  268,  768,  164,   77,  152,  193,   53,  162,  104,  517,  273,  266,   75,   46,  148,
		  51,  640,  100,   45,  576,  161,  265,  262,   71,  146,   30,  140,   88,  515,   98,   43,
		  29,  261,  145,  138,   84,  259,   39,   97,   27,   56,   82,  137,   76,  384,  134,   23,
		  52,  133,  320,   15,   73,   50,   81,  131,   44,   70,  544,  192,  528,  288,  520,  160,
		 272,   74,   49,  516,   42,   69,   28,  144,   41,   67,   96,  514,   38,  264,  260,  136,
		  22,   25,   37,   80,  513,   26,  258,   35,  132,   21,  257,   72,   14,   48,   13,   19,
		 130,   68,   40,   11,  512,   66,  129,    7,   36,   24,   34,  256,   20,   65,   33,   12,
		 128,   18,   10,   17,    6,    9,   64,    5,    3,   32,   16,    8,    4,    2,    1,    0
};


grw_t BA_TABLE_GR[] ={
		   0,    0,    0,    0,    0,    1,    1,    2,    0,    1,    1,    2,    1,    2,    2,    3,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   0,    1,    1,    2,    1,    2,    2,    3,    1,    2,    2,    3,    2,    3,    3,    4,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   1,    2,    2,    3,    2,    3,    3,    4,    2,    3,    3,    4,    3,    4,    4,    5,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   2,    3,    3,    4,    3,    4,    4,    5,    3,    4,    4,    5,    4,    5,    5,    6,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   3,    4,    4,    5,    4,    5,    5,    6,    4,    5,    5,    6,    5,    6,    6,    7,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   4,    5,    5,    6,    5,    6,    6,    7,    5,    6,    6,    7,    6,    7,    7,    8,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   5,    6,    6,    7,    6,    7,    7,    8,    6,    7,    7,    8,    7,    8,    8,    9,
		   6,    7,    7,    8,    7,    8,    8,    9,    7,    8,    8,    9,    8,    9,    9,   10
};


#define nmin 5
#define nmax 10

char ceillog2(unsigned short x){
	unsigned short xx = (x<<1)-1;
	char c;
	for(c=11;c>=0;c--){
		if ((xx>>11)&1) break;
		else xx=xx<<1;
	}
	return(c);
}


//--------------------------------------------------
// Compute interleaved address
//--------------------------------------------------
char getNidx(cnt_t E, idx_t K){

		char c  = ceillog2(E);
		char n1 = (((K<<4)<(9*E)) && ((E<<4)<=(9<<c)))?c-1:c;
		char n2 = ceillog2(K)+3;

		char n = (n1<n2)? n1:n2;
		n =  (n<nmax)? n:nmax;
		n =  (n>nmin)? n:nmin;

		return(n);
}

//--------------------------------------------------
// Compute interleaved address
//--------------------------------------------------
cnt_t J(idx_t Nidx, cnt_t x){

		char numrow = Nidx-5;
		char colid = x>>numrow;
		char colty = colid>>3;
		char coltx = colid&0x7;
		char coloffset = 	(colty==1)?     coltx :
							(colty==2)? -(7-coltx):
							(coltx==3)?          1:
							(coltx==4)?         -1:0;
		cnt_t xx = x + ((cnt_t)coloffset<<numrow);

		return(xx);
}


//--------------------------------------------------
// Compute Q_I
//--------------------------------------------------
void getIMAP(cnt_t E, idx_t A, idx_t L, idx_t Npc, idx_t Npc_wm, flg_t imap[1024], idx_t para[2]){

	cnt_t id;
	cnt_t K=A+L;
	char  Nidx=getNidx(E, K);
	cnt_t N=(1<<Nidx);
	cnt_t Ka=K+Npc;
	cnt_t Kb=K+Npc_wm;
	//char i;
	cnt_t icnt=0;
	flg_t isSet0 = ((E>>Nidx)>0);
	flg_t isSet1 = ((unsigned short) K<<4)>((unsigned short)E*7);
	flg_t isSet2 = ((unsigned short)E<<2)<((unsigned short)3<<Nidx);
	char setID = isSet0? 0 : isSet1? 1 : isSet2? 2 : 3;
	cnt_t Z = (setID&1)? (((cnt_t)3<<(Nidx-2))-(E>>1)) : (((cnt_t)9<<(Nidx-4))-(E>>2));
	grw_t gmin=11;
	cnt_t wmloc=0;

	// pass the parameters to next stage
	para[0]=A;
	para[1]=Nidx;

	//---------------------------------------
	// Search for all valid entries
	//---------------------------------------
	for(id=0;id<N;id++){

		// flag whether frozen bit
		cnt_t bitloc  = BA_TABLE_QI[N|id];
		cnt_t Jbitloc = J(Nidx, bitloc);

		// set frozen bit flag
		flg_t isF = (icnt==Ka)? 1: (setID==0)?            0 :
		            (setID==1)? (Jbitloc>=E) : ((bitloc<Z)||(Jbitloc<(N-E)));

		// set info / CRC bit
		flg_t isIC = (icnt<Kb)? 1:0;

		// get gweight
		grw_t g = BA_TABLE_GR[N|bitloc];

		if((g<gmin)&&(icnt<K)&&(isF==0)){
			gmin=g;
			wmloc=bitloc;
		}

		// update imap and increment icnt
		imap[bitloc]=isF? FLAG_FRZ: isIC? FLAG_INF:FLAG_PCS;
		icnt+=isF?0:1;
	}
	//---------------------------------------

	// set pc_wm bit
	if(Npc_wm>0) imap[wmloc]=FLAG_PCS; // force it to be
}

//--------------------------------------------------
// Compute BA Output
//--------------------------------------------------
char getBAOUT(flg_t imap[1024], idx_t para[2], bat_t BA_o[64]){

	idx_t m;
	bat_t this_ba=0;
	cnt_t id;
	char Nidx=para[1]&0xf;
	idx_t A = para[0];
	cnt_t N=(1<<Nidx);
	char  i;


	//---------------------------------------
	// Loop for all N bits
	//---------------------------------------
	m=0;
	for(i=(N>>4);i<64;i++) BA_o[i]=0;

	for(id=0;id<N;id++){
		// flag whether frozen bit
		flg_t flg = imap[id]; // 0->frozen, 1->info+crc, 3->pc
		// flag whether is a crc location at the end
		flg_t isC = ((flg==FLAG_INF)&&(m>=A));
		// update ba
		this_ba = (this_ba>>2)|((isC? FLAG_CRC: flg)<<30);
		// increment m
		m+=(flg==FLAG_INF)? 1:0;

		// write out the data
		if((id&0xf)==0xf){
			BA_o[id>>4]=this_ba;
			//printf("%d, this_ba=%ld\n",id>>4,this_ba);
		}
	}

	return(Nidx);
}

