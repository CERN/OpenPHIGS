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

      SUBROUTINE VFILLR(X, NUM, WHAT)
      INTEGER NUM
      REAL X(*)
      REAL WHAT
      DO I=1, NUM
        X(I) = WHAT
      END DO
      END

      SUBROUTINE VFILLI(IX, NUM, IWHAT)
      INTEGER NUM
      INTEGER IX(*)
      INTEGER IWHAT
      DO I=1, NUM
        IX(I) = IWHAT
      END DO
      END

CDECK  ID>, KYSABL.
      SUBROUTINE KYSABL(iwk)
************************************************************************
*     KYSABL                                                           *
*                                                                      *
*     Author: D.Bertrand, F.Carena                Date:   95.01.01     *
*                                                 Revised 95.05.19     *
*                                                                      *
*     Function:   Draw a "sablier"                                     *
*                                                                      *
*     Input:      None                                                 *
*                                                                      *
*     Output:     None                                                 *
*                                                                      *
************************************************************************
*
      IMPLICIT NONE
*
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
     ,                IMAG, ICYAN, IORAN, ICOL(7),
     ,                COLR(12), COLG(12), COLB(12)
      INTEGER         NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IMAG, ICYAN, IORAN, ICOL, ICLTB(16)
      REAL            COLR    ,COLG     ,COLB
      EQUIVALENCE    (IBLACK, ICLTB)
*
*      GENERAL CONTROL INFORMATIONS
*
*      SEGMENT OFFSETS
*           NSGARI :  INVARIANT MASS ARROW (40 ARE USED)
*           NSGARJ :  JET ARROW (40 ARE USED)
*           NSGARM :  MISSING MOMENTUM ARROW (4 ARE USED)
*           NSGAXE :  ZOOM AXIS SYSTEM (3 ARE USED)
*           NSGBEA :  BEAM AXIS (4 ARE USED)
*           NSGCTB :  DRAWING CONTROL TABLE (28 ARE USED)
*           NSGDET :  DETECTOR  (200 ARE USED)
*           NSGDIA :  DIALS (25 ARE USED)
*           NSGEVT :  TRACKS AND OBJECTS (10000 ARE USED)
*           NSGHST :  CALORIMETERS HISTOGRAM (2D ONLY; 1 IS USED)
*           NSGPIL :  PILOT INFORMATION CONTROL SEGMENT (1 IS USED)
*           NSGPLD :  DETECTOR COLOR PALETTE (2 ARE USED)
*           NSGPLE :  ENERGY/COLOR PALETTE (1 IS USED)
*           NSGPLM :  MASS/COLOR PALETTE (1 IS USED)
*           NSGSAB :  SABLIER (1 IS USED)
*           NSGSEP :  SPLIT SCREEN SEPARATION (1 IS USED)
*           NSGTIT :  GENERAL TITLE (2 ARE USED)
*           NSGTMP :  TEMPORARY SEGMENT (2 ARE USED)
*           NSGTSP :  SPLIT SCREEN TITLE (5 ARE USED)
*           NSGTXT :  TEXT SEGMENT (4 ARE USED)
*           NSGWIN :  INFORMATION WINDOWS SEGMENTS (700 ARE USED)
*
*      VIEW NUMBERS
*
*           NVWGEN :  TITLES, SPLIT SCREEN, PALETTES ...
*           NVWEVT :  TRACKS  1   IS THE WHOLE SCREEN
*                             1:2 ARE THE 2 SPLIT SCREENS
*                             1:4 ARE THE 4 SPLIT SCREENS
*           NVWSFA :  AXIS FOR SOFTWARE ZOOM
*           NVWSFZ :  SOFTWARE ZOOM
*
*      NORMALISATION TRANSFORMATION NUMBERS
*
*               1  :  xyz       Full screen
*             2:3  :          2 split screens
*             4:7  :          4 split screens
*               8  :  R/Th/Ph   Full screen
*             9:10 :          2 split screens
*            11:14 :          4 split screens
*           NTRSFA :  Axis for software zoom
*           NTRSFZ :  Software zoom (3D)
*
      INTEGER NVWGEN, NVWEVT, NVWSFZ, MVWSCR, MSKSEG
      PARAMETER (NVWGEN=0,NVWEVT=1,NVWSFZ=6,MVWSCR=7,MSKSEG=2047)
      INTEGER IVWSCR(MVWSCR), MIDSEG(4), NTRSFA, NTRSFZ, NVWCSY
      PARAMETER (NTRSFA=15,NTRSFZ=16,NVWCSY=17)
      DATA IVWSCR / 11, 21, 22, 41, 42, 43, 44 /
      DATA MIDSEG / 0, 2048, 4096, 6144 /
      INTEGER NSGTSP, NSGTIT, NSGSAB, NSGPLE, NSGPLM, NSGPLD, NSGCTB
      INTEGER NSGSEP, NSGDET, NSGHST, NSGAXE, NSGTXT, NSGARM, NSGBEA
      INTEGER NSGARJ, NSGTMP, NSGWIN, NSGARI, NSGBSP, NSGPVX, NSGDIP
      INTEGER NSGSCL, NSGCSY, NSGBTG, IMKST0
      PARAMETER (NSGDET=10000,NSGTMP=19998)
      PARAMETER (NSGTIT=20000,NSGTSP=20005,NSGSAB=20010,NSGPLE=20011)
      PARAMETER (NSGPLM=20012,NSGPLD=20013,NSGHST=20015,NSGAXE=20016)
      PARAMETER (NSGCTB=20020,NSGTXT=20050,NSGBEA=20055,NSGSEP=20100)
      PARAMETER (NSGARI=20200,NSGARJ=20300,NSGARM=20400,NSGBSP=20500)
      PARAMETER (NSGPVX=20600,NSGBTG=20680,NSGSCL=20690,NSGCSY=20695)
      PARAMETER (NSGDIP=20700,NSGWIN=21000,IMKST0=30000)
      REAL ZOOM(2)
      DATA ZOOM /1.0, 20000./
      LOGICAL SEGENE, SEGMAS, SEGSAB, COPICK, MISVIS, UGOREP, KINVIS
     +,       LABVIS, TXTVIS, SFTZOM, ZOOMAL, BEAXIS, XYPLOT, DELDET
     +,       IVASAV, JETSAV, MISSAV, ENESAV, MASSAV, SEGSPE, JETVIS
     +,       IVAVIS, FILDET, FWESOL, BKESOL, CONTRT, BTGVIS, BTGSAV
     +,       BSPVIS, BSPSAV, PVXVIS, PVXSAV, DIPVIS, DIPSAV, LIGHTS
     +,       DETCUT, SCAVIS, SCASAV, CSYVIS, CSYSAV, SHODCI
      COMMON /PHIGSG/ SEGENE, SEGMAS, SEGSAB, COPICK, MISVIS, UGOREP
     +,               KINVIS, LABVIS, TXTVIS, SFTZOM, ZOOMAL, BEAXIS
     +,               XYPLOT, DELDET, IVASAV, JETSAV, MISSAV, ENESAV
     +,               MASSAV, SEGSPE, JETVIS, IVAVIS, BTGVIS, BTGSAV
     +,               FILDET, FWESOL, BKESOL, CONTRT, BSPVIS, BSPSAV
     +,               PVXVIS, PVXSAV, DIPVIS, DIPSAV, LIGHTS, DETCUT
     +,               SCAVIS, SCASAV, CSYVIS, CSYSAV, SHODCI
      LOGICAL SEGDET, SEGJET, SEGMIS, SEGIVA, SEGBSP, SEGPVX, SEGDIP
      LOGICAL SEGBTG
      INTEGER NWNDVW, NCURVW, IVAWND, MISWND, IPROJT, SJETSG, IATJET
      INTEGER IATIVA
      COMMON /PHIGVW/ NWNDVW, NCURVW, IVAWND(10), MISWND, IPROJT(4)
     +,               SEGDET(4), SEGJET(4), SEGMIS(4), SEGIVA(4)
     +,               SJETSG(20), IATJET(10), IATIVA(10), SEGBSP(4)
     +,               SEGPVX(4), SEGDIP(4), SEGBTG(4)
      INTEGER SIVASG(10)
      EQUIVALENCE (SJETSG(11),SIVASG)
*
*      UNITS DEFINITION
*
      INTEGER         LI, LO, LSTPI, LSTPO, LG, LERR, LUNDET
      COMMON /KXCLUN/ LI, LO, LSTPI, LSTPO, LG, LERR, LUNDET
*
*      GKS WORKSTATION DEFINITION PARAMETERS
*
      INTEGER         IWKTYP, IWK1, IWKMK, IWKWIN, KCONID, SELMRK
      LOGICAL         WKMKOP
      COMMON /KXCWRK/ IWKTYP, IWK1, IWKMK, IWKWIN, KCONID, SELMRK
     +,               WKMKOP
*
      INTEGER    I
      LOGICAL    START
      REAL       OFFSET        , NORM
      PARAMETER (OFFSET = 2048., NORM = 1.0/4096.)
      REAL       DROPX(5), DROPY(5)
      REAL       XRECT1(5), YRECT1(5), YRECT2(5)
      REAL       XTRIA1(5), XTRIA2(5), YTRIA1(5), YTRIA2(5)
      REAL       XSID1(4), XSID2(4), XSID3(3), XSID4(3)
      REAL       YSID1(4),           YSID3(3), ALLZS(5)

      DATA       START /.TRUE./
      DATA       XRECT1 / 0.4023, 0.5977, 0.5977, 0.4023, 0.4023/
      DATA       YRECT1 / 0.6465, 0.6465, 0.6660, 0.6660, 0.6465/
      DATA       YRECT2 / 0.3340, 0.3340, 0.3535, 0.3535, 0.3340/
      DATA       XTRIA1 / 0.4990, 0.5010, 0.5361, 0.4639, 0.4990/
      DATA       XTRIA2 / 0.4990, 0.4404, 0.5596, 0.5010, 0.4990/
      DATA       YTRIA1 / 0.5   , 0.5   , 0.5879, 0.5879, 0.5   /
      DATA       YTRIA2 / 0.4131, 0.3545, 0.3545, 0.4131, 0.4131/
      DATA       XSID1  / 0.4219, 0.4805, 0.4805, 0.4219/
      DATA       XSID2  / 0.5781, 0.5195, 0.5195, 0.5781/
      DATA       XSID3  / 0.4402, 0.4988, 0.4402/
      DATA       XSID4  / 0.5598, 0.5012, 0.5598/
      DATA       YSID1  / 0.6465, 0.5098, 0.4902, 0.3535/
      DATA       YSID3  / 0.6465, 0.5   , 0.3535/
C      DATA       DROPX  / 0.4990, 0.5010, 0.5010, 0.4990, 0.4990/
      DATA       DROPX   / 0.4990, 0.4990, 0.5010, 0.5010, 0.4990/
      DATA       ALLZS  /1.,1.,1.,1.,1./
*
      DATA COLR / 0., 1., 1., 0., 0., 1., 1., 0., 1.,  0.75, 0.5, 0./
      DATA COLG / 0., 1., 0., 1., 0., 1., 0., 1., 0.5, 1., 0.,  0.7/
      DATA COLB / 0., 1., 0., 0., 1., 0., 1., 1., 0.,  0., 1., 0.50/

      INTEGER IWK
      REAL CSPEC(4)
      IWK1 = IWK
      CALL POPST (NSGSAB)
*     Set color table
      NCOL=12
      DO 11 I = 1, NCOL
         CSPEC(1) = COLR(I)
         CSPEC(2) = COLG(I)
         CSPEC(3) = COLB(I)
         CSPEC(4) = 1.
         CALL PSCR (IWK1,I,4,CSPEC)
 11   CONTINUE
      IBLACK=1
      IWHIT=2
      IRED=3
      IGREEN=4
      IBLUE=5
      IYEL=6
      IMAG=7
      ICYAN=8

      CALL PSFCM (0)
      CALL PSVWI (NVWGEN)
      CALL PSIS (PSOLID)
      CALL PSICI (IRED)
      CALL PFA3 (5, XRECT1, YRECT1, ALLZS)
      CALL PFA3 (5, XRECT1, YRECT2, ALLZS)
*
      CALL PSPLCI (IWHIT)
      CALL PPL3 (4, XSID1, YSID1, ALLZS)
      CALL PPL3 (4, XSID2, YSID1, ALLZS)
      CALL PPL3 (3, XSID3, YSID3, ALLZS)
      CALL PPL3 (3, XSID4, YSID3, ALLZS)
*
      CALL PSICI (IYEL)
      CALL PFA3 (4, XTRIA1, YTRIA1, ALLZS)
      CALL PFA3 (4, XTRIA2, YTRIA2, ALLZS)
*
      DO 10 I = 30, 310, 40
         DROPY(5) = (FLOAT (4-I) + OFFSET) * NORM
         DROPY(4) =  DROPY(5)
         DROPY(3) = (OFFSET - FLOAT (I+3)) * NORM
         DROPY(2) =  DROPY(3)
         DROPY(1) =  DROPY(5)
         CALL PFA3 (5, DROPX, DROPY, ALLZS)
 10   CONTINUE
*
      CALL PCLST (NSGSAB)
      CALL PPOST (IWK1,NSGSAB,1.)
      CALL PRST (IWK1,PCONDI)
*
      END

      PROGRAM DRAWLINE

C      Include PHIGS enumeration file
      INCLUDE 'phigsf77.h'

      INTEGER IWK
      PARAMETER (IWK=1)

C     Open PHIGS and a workstation
      CALL POPPH(0, 1)
      CALL POPWK(IWK, 0, 3)

*  Set color model to PRGBA
      CALL PSCM(IWK, PRGBA)
* Re-Pre-define colors FIXME: maybe have a different workstation type
      CALL PXSCM(IWK)

      CALL KYSABL(IWK)

C      Buisy loop
      DO WHILE (1 .GT. 0)
      END DO

      STOP
      END
