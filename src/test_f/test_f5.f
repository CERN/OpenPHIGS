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

      SUBROUTINE INITCOLS(IWKMK)
      COMMON /KXCCOL/ NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IMAG, ICYAN, IORAN, ICOL(7),
     ,                COLR(16), COLG(16), COLB(16)
      INTEGER         NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IMAG, ICYAN, IORAN, ICOL
      REAL            COLR    ,COLG     ,COLB
      REAL            FCOLR(3)
      INTEGER IWKMK

      NCOL = 6
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
      COLB(4) = 1.0

      IBLUE = 5
      COLR(5) = 0.0
      COLG(5) = 0.0
      COLB(5) = 1.0

      IYEL = 6
      COLR(6) = 0.0
      COLG(6) = 1.0
      COLB(6) = 0.0

      DO I=1, NCOL
         FCOLR(1) = COLR(I)
         FCOLR(2) = COLG(I)
         FCOLR(3) = COLB(I)
         CALL PSCR (IWKMK, I, 3, FCOLR)
      END DO
C define more colors
C      CALL PXSCM(IWKMK)

      END

      PROGRAM MARKERSELECT

C      Include PHIGS enumeration file
      INCLUDE 'phigsf77.h'

      INTEGER    NMARKS, NMSIZS
      PARAMETER (NMARKS=8, NMSIZS=8)

      INTEGER MARKER, ISIZE, MARKRS(NMARKS)
      DATA MARKRS/1, 2, 3, 4, 5, 6, 7, 8/
      INTEGER     PICKAB
      PARAMETER  (PICKAB=98)

      COMMON /KXCCOL/ NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IMAG, ICYAN, IORAN, ICOL(7),
     ,                COLR(16), COLG(16), COLB(16)
      INTEGER         NCOL, IBLACK, IWHIT, IRED, IGREEN, IBLUE, IYEL,
     ,                IMAG, ICYAN, IORAN, ICOL
      INTEGER STRID, TUS, IWK, ICL, IDNR, ITNR
      REAL RX, RY, STATUS
      REAL WKWN(6),CW(4),RV(4),CV(4),CSPEC(3)
      CHARACTER(LEN=80) DATREC
      INTEGER MAXPCK
      PARAMETER (MAXPCK=5)
      INTEGER IPPD,STAT,PPD,PP(3, MAXPCK)
      REAL PTOUT
      PARAMETER (PTOUT=1.0)
      INTEGER ICONDI
      INTEGER NVWGEN
      PARAMETER (NVWGEN=0)
      INTEGER SELMRK
      INTEGER EDITMO

C     Init selected marker: none
      SELMRK = 0
C     Open PHIGS and a workstation
      IWKMK = 4
      CALL POPPH(0, 1)
      CALL POPWK(IWKMK, 0, 3)
C query work station
      CALL PQWKT(IWKMK,IERR,TUS,WKWN,CW,RV,CV)
      RX = RV (2)
      RY = RV (4)
* Initialize a pick
      CALL PINPK (IWKMK,1,POK,MAXPCK,PP,1,0.,RX,0.,RY,0,DATREC,PPOTOP)
* Initialize pick filter
      CALL PSPKFT (IWKMK,1,1,PICKAB,0,0)
*     Set Pick mode
      CALL PSPKM (IWKMK,1,PEVENT,PNECHO)
* set dimensions
      WKWN(1) = 0.
      WKWN(2) = RX / AMAX1 (RX,RY)
      WKWN(3) = 0.
      WKWN(4) = RY / AMAX1 (RX,RY)
      WKWN(5) = 0.
      WKWN(6) = 1.
* Set workstation window
      CALL PSWKW3 (IWKMK,WKWN)
* Init colors
      CALL INITCOLS(IWKMK)
      DY = 1./(FLOAT(NMARKS)+1.)
      DX = 1./(FLOAT(NMSIZS)+1.)
      DO I = 1, NMARKS
         MARKER = MARKRS(I)
         print*,"Defining marker:", i, marker
         Y = 1. - FLOAT(I) * DY
         DO ISIZE = 1, NMSIZS
            X = 0.05 + FLOAT(ISIZE-1) * DX
            STRID=IMKST0+(I-1)*NMSIZS+ISIZE
            CALL POPST(STRID)
            CALL PSPMI(1)
            CALL PSPMCI(2)
            CALL PSMK (MARKER)
            CALL PSMKSC (FLOAT(ISIZE)/400.)
            CALL PADS (1, PICKAB)
            CALL PSPKID(400*MARKER+ISIZE)
            CALL PPM (1, X, Y)
            CALL PSVWI (NVWGEN)
            CALL PCLST(STRID)
         ENDDO
      ENDDO

*     Create structures for Polymarker Selection
      CALL PSPMR(IWKMK, 1, 0, 1., 2)
C     Post structure to workstation
      DO I = 1, NMARKS
         DO ISIZE = 1, NMSIZS
            CALL PPOST (IWKMK,IMKST0+(I-1)*NMSIZS+ISIZE,1.)
         ENDDO
      ENDDO

C     Busy loop
      DO WHILE (1 .GT. 0)
         CALL PWAIT (PTOUT, IWK, ICL, IDNR)
         IF ((ICL.EQ.PPICK).AND.(IWK.EQ.IWKMK)) THEN
            CALL PGTPK (MAXPCK,STATUS,PPD,PP)
            IF (STATUS.EQ.PNPICK.OR.STATUS.EQ.PNONE) THEN
               PRINT*,"Nothing selected"
            ELSE
               CALL PQEDM (IERR,EDITMO)
               IF (IERR.NE.0) THEN
                  PRINT*, "ERROR Opening structure"
                  GOTO  99
               ENDIF
               CALL PSEDM (PREPLC)
               IF (SELMRK.NE.0) THEN
                  CALL POPST (SELMRK)
                  CALL PSEP(2)
                  CALL PSPMCI(2)
                  CALL PCLST(SELMRK)
               ENDIF
               CALL POPST(PP(1,1))
               CALL PSEP(2)
               CALL PSPMCI(3)
               CALL PCLST(PP(1,1))
               SELMRK=PP(1,1)
               CALL PSEDM (EDITMO)
               CALL PUWK (IWKMK,1)
               MKIND = PP(2,1) / 400
               MKSIZ = PP(2,1) - 400 * MKIND
               PRINT*, "Selected marker ", mkind, " with size ", mksiz
            ENDIF
 99         CALL PUWK(IWKMK,1)
            CALL PRST(IWKMK, ICONDI)

         ENDIF
      END DO

      STOP
      END
