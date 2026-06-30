C *****************************************************************************
C * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
C *
C * This file is part of Open PHIGS
C * Copyright (C) 2022-2023 CERN
C *
C * Open PHIGS is free software: you can redistribute it and/or modify
C * it under the terms of the GNU Lesser General Public License as published by
C * the Free Software Foundation, either version 2.1 of the License, or
C * (at your option) any later version.
C *
C * Open PHIGS is distributed in the hope that it will be useful,
C * but WITHOUT ANY WARRANTY; without even the implied warranty of
C * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C * GNU Lesser General Public License for more details.
C *
C * You should have received a copy of the GNU Lesser General Public License
C * along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
C *****************************************************************************

      SUBROUTINE VFILL(IX, NUM, IWHAT)
      INTEGER NUM
      INTEGER IX(*)
      INTEGER IWHAT
      DO I=1, NUM
        IX(I) = IWHAT
      END DO
      END

      SUBROUTINE INITCOLS(IWK1)
      COMMON /KXCCOL/ NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IGREY, IMAG, ICYAN, IORAN, ICOL(7),
     ,                COLR(16), COLG(16), COLB(16)
      INTEGER         NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IGREY, IMAG, ICYAN, IORAN, ICOL, ICLTB(16)
      REAL            COLR    ,COLG     ,COLB
      EQUIVALENCE    (IBLACK, ICLTB)
      REAL            FCOLR(3)
      INTEGER IWK1
      NCOL = 7
      IBLACK = 1
      COLR(1) = 0.0
      COLG(1) = 0.0
      COLB(1) = 0.0

      IWHITE = 2
      COLR(2) = 1.0
      COLG(2) = 1.0
      COLB(2) = 1.0

      IRED = 3
      COLR(3) = 1.0
      COLG(3) = 0.0
      COLB(3) = 0.0

      IGREEN = 4
      COLR(4) = 0.0
      COLG(4) = 1.0
      COLB(4) = 0.0

      IBLUE = 5
      COLR(5) = 0.0
      COLG(5) = 0.0
      COLB(5) = 1.0

      IYEL = 6
      COLR(6) = 0.0
      COLG(6) = 1.0
      COLB(6) = 0.0

      IGREY = 7
      COLR(7) = 0.7
      COLG(7) = 0.7
      COLB(7) = 0.7

      DO I=1, NCOL
         FCOLR(1) = COLR(I)
         FCOLR(2) = COLG(I)
         FCOLR(3) = COLB(I)
         CALL PSCR (IWK1, I, 3, FCOLR)
      END DO

      FCOLR(1) = 1.0
      FCOLR(2) = 1.0
      FCOLR(3) = 1.0
      CALL PSCR (IWK1, 150, 3, FCOLR)
      FCOLR(1) = 0.0
      FCOLR(2) = 0.0
      FCOLR(3) = 0.0
      CALL PSCR (IWK1, 151, 3, FCOLR)
      FCOLR(1) = 1.0
      FCOLR(2) = 1.0
      FCOLR(3) = 0.0
      CALL PSCR (IWK1, 152, 3, FCOLR)
      FCOLR(1) = 0.0
      FCOLR(2) = 1.0
      FCOLR(3) = 1.0
      CALL PSCR (IWK1, 153, 3, FCOLR)
      FCOLR(1) =  0.8
      FCOLR(2) =  0.8
      FCOLR(3) =  0.8
      CALL PSCR (IWK1, 154, 3, FCOLR)

      END

CDECK  ID>, KYDELP.
      SUBROUTINE KYDELP (XORG, YORG, SCAL)
************************************************************************
*     KYDELP                                                           *
*     AUTHOR(S): D.BERTRAND, F.CARENA             DATE:   95.01.01     *
*                                              Revised:   96.08.01  DB *
*                                                                      *
*     Function: Draw the DELPHI dolphin                                *
*                                                                      *
*     Input:  XORG      lower left origin of the drawing (X)           *
*             YORG      lower left origin of the drawing (Y)           *
*             SCAL      scale of the drawing                           *
*                                                                      *
*     Output: None                                                     *
*                                                                      *
************************************************************************
*
      IMPLICIT NONE
*
      REAL     XORG, YORG
      REAL     SCAL
      INCLUDE 'phigsf77.h'
      INTEGER    PLABEL     , BLABEL     , FLABEL
      PARAMETER (PLABEL=9993, BLABEL=9994, FLABEL=9995)
      INTEGER    SLABEL     , HLABEL     , PICKAB     , HIGHLI
      PARAMETER (SLABEL=9996, HLABEL=9997, PICKAB=9998, HIGHLI=9999)
      INTEGER    PFONT      , TFONT      , EFONT      , FLCKSL
      PARAMETER (PFONT = -21, TFONT = -29, EFONT = -28, FLCKSL = 20000)
      INTEGER    TFNTPS    , EFNTPS, NMARKS, NMSIZS
      PARAMETER (TFNTPS = 3, EFNTPS = 7, NMARKS=12, NMSIZS=10)
      REAL       PTOUT      , CROTST     , PSWITH
      PARAMETER (PTOUT = 0.01, CROTST = 4., PSWITH = 2.)
      CHARACTER(LEN=*) CONFIL          , ERRFIL
      PARAMETER    (CONFIL = 'GPHIGS.def', ERRFIL = 'ERRPHIGS')
*
*      COLOR LOOK UP TABLE DEFINITION
*
      COMMON /KXCCOL/ NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IGREY,IMAG, ICYAN, IORAN, ICOL(7),
     ,                COLR(16), COLG(16), COLB(16)
      INTEGER         NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IGREY,IMAG, ICYAN, IORAN, ICOL, ICLTB(16)
      REAL            COLR    ,COLG     ,COLB
      EQUIVALENCE    (IBLACK, ICLTB)
*
      INTEGER    NPT1      , NPT2      , NPT3      , NPT4
      PARAMETER (NPT1 = 334, NPT2 =  28, NPT3 =  44, NPT4 =  28)
      INTEGER    NPT5      , NPT6      , NPT7      , NPT8
      PARAMETER (NPT5 = 169, NPT6 = 207, NPT7 =   2, NPT8 =   2)
      INTEGER    NPT9      , NPT10     , NPT11     , NPT12
      PARAMETER (NPT9 =   2, NPT10=   2, NPT11=   5, NPT12=   5)
      INTEGER    NPT13     , NPT14
      PARAMETER (NPT13=   5, NPT14=   5)
      INTEGER    NPTMX
      PARAMETER (NPTMX= 334)
      REAL       OFFSET      , NORM
      PARAMETER (OFFSET=2048., NORM=1.0/4096.)
      REAL       FACT             , SCALMAX
      PARAMETER (FACT = 730./2232., SCALMAX = 5.37*FACT)
      REAL       X0           , Y0
      PARAMETER (X0= 670.*NORM, Y0= 1324.*NORM)
      INTEGER    I,
     ,           X1 (NPT1 ), X2 (NPT2 ), X3 (NPT3 ), X4 (NPT4 ),
     ,           X5 (NPT5 ), X6 (NPT6 ), X7 (NPT7 ), X8 (NPT8 ),
     ,           X9 (NPT9 ), X10(NPT10), X11(NPT11), X12(NPT12),
     ,           X13(NPT13), X14(NPT14),
     ,           Y1 (NPT1 ), Y2 (NPT2 ), Y3 (NPT3 ), Y4 (NPT4 ),
     ,           Y5 (NPT5 ), Y6 (NPT6 ), Y7 (NPT7 ), Y8 (NPT8 ),
     ,           Y9 (NPT9 ), Y10(NPT10), Y11(NPT11), Y12(NPT12),
     ,           Y13(NPT13), Y14(NPT14)
      INTEGER    VCOLI(NPTMX), EDATA(NPTMX)
      LOGICAL    NKPRT
      REAL       SCALE, XG(NPTMX), YG(NPTMX), VNXY(NPTMX), CDOL(3)
      DATA X1   /
     , -984, -984, -984, -988, -996,-1004,-1014,-1020,-1022,-1024,-1016,
     , -998, -980, -960, -946, -930, -918, -906, -896, -894, -890, -888,
     , -888, -884, -888, -894, -912, -924, -940, -950, -962, -974, -990,
     ,-1004,-1010,-1022,-1044,-1082,-1114,-1148,-1192,-1230,-1264,-1292,
     ,-1322,-1338,-1360,-1370,-1384,-1386,-1380,-1364,-1348,-1322,-1294,
     ,-1268,-1240,-1216,-1194,-1170,-1140,-1164,-1180,-1206,-1190,-1174,
     ,-1156,-1140,-1120,-1098,-1090,-1112,-1126,-1138,-1138,-1132,-1118,
     ,-1102,-1084,-1058,-1040,-1016, -972, -928, -882, -838, -800, -754,
     , -706, -646, -606, -568, -542, -516, -486, -452, -424, -404, -392,
     , -416, -438, -464, -482, -504, -520, -490, -446, -406, -374, -340,
     , -304, -282, -246, -204, -162, -114,  -84,  -34,   14,   62,  114,
     ,  160,  204,  230,  250,  252,  234,  192,  148,  118,   86,   60,
     ,   38,   16,   -4,  -20,  -26,   -6,   26,   64,  108,  152,  182,
     ,  218,  236,  254,  268,  278,  296,  308,  322,  328,  330,  328,
     ,  320,  310,  298,  288,  264,  232,  204,  178,  146,  118,   92,
     ,   70,   44,   18,  -10,  -24,  -38,  -40,  -24,   -8,   18,   66,
     ,  108,  140,  172,  210,  250,  290,  370,  368,  406,  438,  468,
     ,  494,  500,  508,  512,  508,  504,  494,  480,  464,  454,  440,
     ,  428,  436,  454,  492,  520,  554,  588,  616,  642,  666,  686,
     ,  696,  692,  682,  674,  666,  650,  628,  606,  586,  586,  602,
     ,  628,  648,  674,  702,  738,  764,  792,  818,  846,  860,  880,
     ,  902,  918,  934,  940,  944,  940,  936,  930,  920,  906,  886,
     ,  860,  826,  796,  748,  690,  634,  588,  532,  488,  438,  390,
     ,  340,  292,  246,  204,  168,  128,  104,   72,   36,    8,  -22,
     ,  -56, -104, -142, -188, -232, -276, -324, -368, -400, -424, -444,
     , -458, -476, -480, -478, -476, -470, -456, -442, -432, -420, -412,
     , -410, -396, -380, -374, -364, -356, -354, -356, -362, -376, -392,
     , -414, -450, -510, -548, -596, -626, -658, -678, -704, -746, -776,
     , -794, -818, -840, -850, -862, -880, -900, -922, -948, -970,-1002,
     ,-1026,-1056,-1074,-1082,-1080,-1070,-1058,-1048,-1042,-1034,-1030,
     ,-1030,-1026,-1028,-1030/
      DATA Y1   /
     ,  586,  564,  538,  518,  494,  476,  456,  432,  408,  396,  388,
     ,  380,  380,  390,  400,  418,  440,  462,  486,  516,  536,  558,
     ,  578,  602,  620,  640,  646,  648,  642,  632,  618,  606,  588,
     ,  578,  572,  564,  550,  534,  522,  520,  524,  536,  556,  576,
     ,  600,  624,  652,  674,  694,  710,  726,  746,  754,  768,  766,
     ,  764,  760,  750,  742,  736,  722,  732,  742,  750,  764,  768,
     ,  774,  774,  772,  764,  760,  766,  774,  780,  796,  828,  864,
     ,  894,  922,  960,  986, 1010, 1042, 1056, 1064, 1072, 1076, 1074,
     , 1070, 1060, 1052, 1038, 1028, 1018, 1006,  990,  974,  964,  958,
     ,  972,  980,  992, 1002, 1014, 1022, 1020, 1022, 1026, 1032, 1040,
     , 1052, 1060, 1078, 1106, 1134, 1158, 1172, 1190, 1206, 1214, 1218,
     , 1220, 1222, 1222, 1222, 1210, 1190, 1160, 1124, 1088, 1058, 1026,
     ,  994,  954,  922,  902,  878,  864,  846,  826,  798,  762,  732,
     ,  704,  684,  654,  626,  608,  576,  534,  504,  448,  390,  350,
     ,  314,  280,  248,  214,  174,  134,  104,   68,   28,   -2,  -36,
     ,  -66, -104, -144, -182, -214, -254, -280, -312, -330, -340, -340,
     , -326, -318, -306, -292, -274, -254, -206, -212, -188, -160, -132,
     , -100,  -68,  -36,    4,   38,   80,  120,  158,  190,  216,  242,
     ,  256,  266,  260,  228,  202,  166,  130,   94,   66,   24,  -20,
     ,  -68, -112, -158, -194, -212, -228, -242, -254, -262, -264, -254,
     , -248, -240, -230, -226, -238, -248, -260, -280, -296, -314, -336,
     , -360, -390, -426, -464, -510, -562, -600, -628, -638, -608, -558,
     , -532, -502, -482, -450, -426, -416, -414, -414, -418, -428, -444,
     , -462, -490, -508, -532, -556, -588, -614, -638, -664, -684, -698,
     , -712, -720, -724, -722, -710, -694, -664, -626, -590, -552, -518,
     , -476, -426, -380, -326, -286, -242, -208, -170, -130,  -96,  -78,
     ,  -54,  -14,   24,   70,  114,  150,  190,  228,  260,  280,  304,
     ,  318,  322,  326,  330,  334,  344,  348,  356,  364,  380,  384,
     ,  390,  398,  404,  406,  392,  372,  354,  338,  322,  308,  300,
     ,  304,  308,  330,  362,  382,  412,  434,  454,  474,  492,  508,
     ,  526,  540,  546,  558/
      DATA X2   /
     , -970, -970, -964, -946, -934, -918, -906, -896, -896, -902, -910,
     , -920, -930, -944, -952, -964, -978, -992,-1002,-1012,-1010,-1012,
     ,-1008,-1002, -994, -986, -978, -970/
      DATA Y2   /
     ,  481,  498,  518,  532,  544,  550,  548,  538,  524,  508,  486,
     ,  460,  444,  424,  416,  404,  396,  392,  396,  402,  418,  434,
     ,  448,  462,  478,  488,  492,  481/
      DATA X3   /
     , -962, -956, -942, -930, -920, -918, -918, -926, -940, -952, -960,
     , -972, -978, -980, -980, -978, -976, -978, -978, -978, -978, -970,
     , -958, -944, -928, -914, -902, -892, -888, -888, -892, -896, -898,
     , -900, -902, -906, -914, -926, -934, -940, -950, -958, -964, -962/
      DATA Y3   /
     ,  908,  902,  892,  878,  866,  858,  846,  840,  836,  842,  848,
     ,  858,  872,  888,  896,  904,  906,  914,  922,  936,  948,  956,
     ,  964,  966,  962,  954,  942,  926,  910,  898,  884,  886,  894,
     ,  904,  914,  922,  928,  936,  940,  938,  934,  928,  918,  908/
      DATA X4   /
     , -898, -894, -896, -908, -920, -938, -946, -948, -952, -948, -942,
     , -930, -914, -898, -886, -878, -870, -870, -858, -850, -840, -850,
     , -860, -870, -878, -884, -890, -898/
      DATA Y4   /
     ,  647,  654,  658,  660,  666,  672,  678,  688,  698,  708,  714,
     ,  718,  718,  716,  708,  698,  688,  678,  666,  662,  664,  660,
     ,  660,  662,  662,  660,  654,  647/
      DATA X5   /
     , -566, -534, -504, -464, -438, -402, -362, -332, -306, -268, -230,
     , -188, -162, -130,  -98,  -54,  -28,    2,   26,   62,   92,  120,
     ,  138,  166,  198,  218,  246,  264,  270,  272,  274,  274,  272,
     ,  262,  254,  242,  220,  192,  168,  146,  126,  108,   98,   84,
     ,   72,   58,   44,   32,   18,   -2,  -20,  -40,  -62,  -78,  -92,
     , -104, -118, -130, -144, -154, -156, -160, -164, -172, -174, -174,
     , -168, -156, -128,  -88,  -40,    8,   48,   90,  134,  194,  248,
     ,  300,  324,  370,  410,  448,  472,  502,  506,  510,  514,  516,
     ,  514,  506,  484,  452,  422,  390,  358,  324,  286,  252,  216,
     ,  184,  144,  108,   66,   28,   -2,  -32,  -56,  -98, -132, -172,
     , -206, -230, -256, -278, -298, -316, -330, -342, -342, -334, -330,
     , -318, -310, -294, -282, -272, -260, -254, -242, -230, -212, -196,
     , -180, -168, -152, -140, -124, -104,  -84,  -60,  -38,  -26,  -20,
     ,  -34,  -64,  -90, -124, -160, -194, -230, -274, -310, -348, -382,
     , -414, -454, -486, -516, -540, -570, -572, -576, -576, -580, -580,
     , -578, -576, -568, -566/
      DATA Y5   /
     ,  555,  556,  560,  562,  564,  562,  562,  562,  560,  556,  550,
     ,  544,  538,  530,  524,  512,  506,  496,  490,  480,  476,  470,
     ,  466,  460,  456,  450,  438,  420,  406,  394,  372,  350,  330,
     ,  306,  278,  262,  244,  218,  190,  170,  146,  124,  110,   96,
     ,   78,   60,   42,   30,   10,  -12,  -36,  -58,  -86, -106, -124,
     , -146, -170, -192, -216, -240, -242, -266, -290, -322, -342, -364,
     , -386, -398, -404, -390, -380, -372, -362, -352, -338, -324, -312,
     , -300, -292, -282, -270, -264, -256, -258, -276, -286, -310, -330,
     , -352, -362, -370, -382, -392, -402, -414, -422, -436, -446, -456,
     , -468, -480, -498, -512, -522, -530, -538, -546, -560, -566, -574,
     , -574, -574, -562, -550, -530, -510, -486, -458, -422, -386, -350,
     , -312, -274, -238, -206, -174, -144, -126, -100,  -76,  -42,  -14,
     ,   20,   46,   72,   94,  126,  152,  184,  212,  248,  266,  280,
     ,  290,  300,  310,  330,  338,  352,  354,  362,  364,  374,  370,
     ,  376,  376,  378,  376,  380,  388,  408,  428,  448,  472,  496,
     ,  516,  530,  544,  555/
      DATA X6   /
     , -579, -568, -548, -520, -490, -464, -438, -406, -380, -346, -314,
     , -280, -244, -214, -182, -142, -100,  -52,   -8,   48,   98,  148,
     ,  196,  236,  280,  310,  310,  326,  348,  362,  374,  386,  386,
     ,  392,  390,  388,  382,  368,  358,  346,  336,  322,  304,  290,
     ,  282,  266,  254,  238,  224,  210,  184,  168,  152,  134,  120,
     ,   96,   76,   56,   32,    6,  -20,  -46,  -68,  -98, -124, -150,
     , -186, -214, -260, -294, -336, -368, -408, -442, -486, -536, -588,
     , -624, -658, -700, -732, -766, -792, -816, -838, -852, -866, -876,
     , -880, -884, -890, -892, -894, -888, -886, -886, -884, -882, -882,
     , -872, -864, -856, -846, -838, -830, -830, -836, -842, -844, -844,
     , -848, -846, -842, -842, -836, -832, -828, -820, -812, -800, -790,
     , -774, -752, -726, -698, -666, -638, -592, -542, -498, -460, -426,
     , -398, -362, -318, -284, -248, -212, -176, -152, -124,  -98,  -72,
     ,  -50,  -20,    4,   26,   52,   72,   94,  118,  140,  166,  190,
     ,  204,  230,  240,  256,  274,  288,  298,  308,  316,  322,  326,
     ,  330,  330,  326,  326,  320,  308,  290,  280,  258,  236,  198,
     ,  168,  130,   86,   44,    0,  -42,  -72, -106, -144, -174, -206,
     , -226, -246, -260, -284, -316, -348, -380, -412, -440, -458, -478,
     , -490, -504, -514, -522, -534, -548, -560, -574, -579/
      DATA Y6   /
     , 1045, 1072, 1100, 1134, 1162, 1192, 1222, 1250, 1278, 1308, 1330,
     , 1358, 1386, 1404, 1422, 1446, 1468, 1484, 1494, 1502, 1506, 1496,
     , 1482, 1466, 1434, 1404, 1398, 1368, 1328, 1282, 1248, 1182, 1142,
     , 1102, 1058, 1006,  938,  890,  842,  794,  754,  712,  664,  632,
     ,  606,  576,  550,  518,  486,  460,  422,  390,  364,  332,  306,
     ,  274,  248,  224,  194,  158,  130,  100,   74,   44,   16,  -10,
     ,  -40,  -66,  -96, -118, -142, -158, -174, -186, -196, -200, -202,
     , -196, -190, -172, -158, -136, -104,  -74,  -36,   -4,   30,   68,
     ,  104,  138,  170,  200,  232,  266,  298,  318,  342,  356,  370,
     ,  382,  396,  404,  410,  404,  400,  388,  364,  332,  304,  278,
     ,  244,  218,  196,  180,  154,  132,  116,   94,   74,   48,   22,
     ,    4,  -16,  -42,  -58,  -74,  -86,  -92,  -94,  -86,  -76,  -68,
     ,  -60,  -36,  -10,   14,   36,   64,   96,  118,  144,  170,  204,
     ,  228,  260,  290,  318,  350,  382,  412,  444,  476,  524,  568,
     ,  604,  646,  684,  720,  770,  816,  848,  886,  924,  962,  996,
     , 1026, 1058, 1096, 1134, 1174, 1214, 1264, 1290, 1328, 1354, 1388,
     , 1404, 1412, 1418, 1418, 1410, 1398, 1390, 1376, 1358, 1342, 1324,
     , 1312, 1300, 1288, 1270, 1244, 1216, 1186, 1160, 1132, 1114, 1092,
     , 1074, 1058, 1042, 1030, 1026, 1032, 1038, 1044, 1045/
      DATA X7   / -822, -642/
      DATA Y7   /  106,  262/
      DATA X8   / -480, -450/
      DATA Y8   /  -80,  210/
      DATA X9   /  326,   78/
      DATA Y9   / 1078,  952/
      DATA X10   / -246, -236/
      DATA Y10   / 1300, 1174/
      DATA X11   / -654, -578, -624, -640, -654/
      DATA Y11   /  281,  316,  250,  262,  281/
      DATA X12   / -479, -444, -426, -448, -479/
      DATA Y12   /  218,  292,  214,  214,  218/
      DATA X13   /   65,  -14,   86,   78,   65/
      DATA Y13   /  969,  904,  926,  952,  969/
      DATA X14   / -258, -232, -218, -236, -258/
      DATA Y14   / 1167, 1096, 1168, 1174, 1167/
      DATA VNXY  / NPTMX*0.0/
      DATA EDATA / NPTMX*1  /
      DATA NKPRT / .TRUE. /
*
      SCALE = MIN (SCAL*FACT, SCALMAX)
      IF ( NKPRT )                                  THEN
        CALL PSLWSC (1.0)
        CALL PSIS   (PSOLID)
        CALL PSBIS  (PSOLID)
        CALL PSEWSC (1.0)
        CALL PSEDFG (1)
*
*     LEP circle
        CALL PSIS(PISEMP)
        CALL PSEDCI (IGREEN)
        CALL PSICI  (152)
        CALL VFILL (VCOLI, NPT6, IGREEN)
        DO   I = 1, NPT6
          XG(I) = ((FLOAT (X6 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y6 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, 152, 0., 0., 0., 0., 0, 0.,
     ,              1, NPT6, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY, VNXY,
     ,              0, 0.)
*
*     LEP arrows
        CALL PSPLCI (IGREEN)
        DO   I = 1, NPT7
          XG(I) = ((FLOAT (X7 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y7 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PPL (NPT7, XG, YG)
        DO   I = 1, NPT8
          XG(I) = ((FLOAT (X8 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y8 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PPL (NPT8, XG, YG)
        DO   I = 1, NPT9
          XG(I) = ((FLOAT (X9 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y9 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PPL (NPT9, XG, YG)
        DO   I = 1, NPT10
          XG(I) = ((FLOAT (X10 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y10 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PPL (NPT10, XG, YG)
        CALL VFILL (VCOLI, NPT11, IYEL)
        DO   I = 1, NPT11
          XG(I) = ((FLOAT (X11 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y11 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PSEDCI (IGREEN)
        CALL PSICI  (IGREEN)
        CALL PSBICI  (IGREEN)
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, IGREEN, 0., 0., 0., 0., 0,
     ,              0., 1, NPT11, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY,
     ,              VNXY, 0, 0.)
        DO   I = 1, NPT12
          XG(I) = ((FLOAT (X12 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y12 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, IGREEN, 0., 0., 0., 0., 0,
     ,              0., 1, NPT12, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY,
     ,              VNXY, 0, 0.)
        DO   I = 1, NPT13
          XG(I) = ((FLOAT (X13 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y13 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, IGREEN, 0., 0., 0., 0., 0,
     ,              0., 1, NPT13, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY,
     ,              VNXY, 0, 0.)
        DO   I = 1, NPT14
          XG(I) = ((FLOAT (X14 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y14 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, IGREEN, 0., 0., 0., 0., 0,
     ,              0., 1, NPT14, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY,
     ,              VNXY, 0, 0.)
        NKPRT = .FALSE.
      END IF
*
*     Draw the Dolphin
        CALL PSIS(PSOLID)
        CALL PSEDCI(IBLUE)
        CALL PSICI(153)
        CALL PSBICI(153)
        CALL VFILL (VCOLI, NPT1, IBLUE)
        DO   I = 1, NPT1
          XG(I) = ((FLOAT (X1 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y1 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, 153, 0., 0., 0., 0., 0, 0.,
     ,              1, NPT1, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY, VNXY,
     ,              0, 0.)
*
*     Dolphin tong
        CALL PSEDCI (IRED)
        CALL PSICI  (IRED)
        CALL PSBICI  (IRED)
        CALL VFILL (VCOLI, NPT2, IRED)
        DO   I = 1, NPT2
          XG(I) = ((FLOAT (X2 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y2 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, IRED, 0., 0., 0., 0., 0, 0.,
     ,              1, NPT2, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY, VNXY,
     ,              0, 0.)
*
*     Dolphin eye
        CALL PSEDCI (IBLACK)
        CALL PSICI  (IBLACK)
        CALL PSBICI  (IBLACK)
*        CALL PSIS(PISEMP)
        CALL VFILL (VCOLI, MAX (NPT3, NPT4), 0)
        DO   I = 1, NPT3
          XG(I) = ((FLOAT (X3 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y3 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, 0, 0., 0., 0., 0., 0, 0., 1,
     ,              NPT3, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY, VNXY,
     ,              0, 0.)
        DO   I = 1, NPT4
          XG(I) = ((FLOAT (X4 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y4 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, 0, 0., 0., 0., 0., 0, 0., 1,
     ,              NPT4, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY, VNXY,
     ,              0, 0.)
*
*     Dolphin Z
        CALL PSEDCI (IBLACK)
        CALL PSIS(PSOLID)
        CALL PSBIS(PSOLID)
        CALL PSICI  (IRED)
        CALL PSBICI  (IRED)
        CALL VFILL (VCOLI, NPT5, IRED)
        DO   I = 1, NPT5
          XG(I) = ((FLOAT (X5 (I))+OFFSET)*NORM - X0)*SCALE + XORG
          YG(I) = ((FLOAT (Y5 (I))+OFFSET)*NORM - Y0)*SCALE + YORG
        END DO
        CALL PFASD (PFNO, PEVF, PCD, 0, 0, IRED, 0., 0., 0., 0., 0, 0.,
     ,              1, NPT5, EDATA, XG, YG, VCOLI, 0., VNXY, VNXY, VNXY,
     ,              0, 0.)
        CALL PSIS(PISEMP)
      END

      PROGRAM DRAWLINE

C      Include PHIGS enumeration file
      INCLUDE 'phigsf77.h'

      INTEGER IWK
      PARAMETER (IWK=1)

C     Open PHIGS and a workstation
      CALL POPPH(0, 1)
      CALL POPWK(IWK, 0, 3)

C     Initialize colors
      CALL INITCOLS(IWK)

C      Open structure
      CALL POPST(0)
      CALL PSIASF(13, 1)
      CALL KYDELP(0.3, 0.5, 1.)
C
C     Close structure
      CALL PCLST

C      Post structure to workstation
      CALL PPOST(IWK, 0, 0.0)

C      Buisy loop
      DO WHILE (1 .GT. 0)
      END DO

      STOP
      END
