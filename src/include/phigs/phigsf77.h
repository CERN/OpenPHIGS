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

C Text related
C---- horizontal text alignement
      INTEGER    PAHNOR, PALEFT, PACENT, PARITE
      PARAMETER (PAHNOR=0, PALEFT=1, PACENT=2, PARITE=3)

C---- vertical text alignement
      INTEGER   PAVNOR, PATOP, PACAP, PAHALF, PABASE, PABOTT
      PARAMETER (PAVNOR=0,PATOP=1,PACAP=2,PAHALF=3,PABASE=4,PABOTT=5)

C---- text path
      INTEGER  PRIGHT, PLEFT, PUP, PDOWN
      PARAMETER (PRIGHT=0, PLEFT=1, PUP=2, PDOWN=3)

C---- text precision
      INTEGER    PSTRP, PCHARP, PSTRKP
      PARAMETER (PSTRP=0, PCHARP=1, PSTRKP=2)

C---- line type
      INTEGER   PLSOLI, PLDASH, PLDOT, PLDASD
      PARAMETER(PLSOLI=1, PLDASH=2, PLDOT=3, PLDASD=4)

C---- marker types
      INTEGER    PPOINT, PPLUS, PAST, POMARK, PXMARK
      PARAMETER (PPOINT=1, PPLUS=2, PAST=3, POMARK=4, PXMARK=5)

C---- interior style
      INTEGER   PISEMP, PHOLLO, PSOLID,  PHATCH
      PARAMETER ( PISEMP=0, PHOLLO=1, PSOLID=2, PHATCH=3)

C---- Facet data flags
      INTEGER  PFNO, PFC, PFN, PFCN
      PARAMETER (PFNO=0, PFC=1, PFN=2, PFCN=3)

C---- Edge data flags
      INTEGER    PENO, PEVF
      PARAMETER (PENO=0, PEVF=1)

C---- Vertex data flag
      INTEGER    PCD, PCDC, PCDN, PCDCN
      PARAMETER (PCD=0, PCDC=1, PCDN=2, PCDCN=3)

C---- edit mode
      INTEGER    PINSRT, PREPLC
      PARAMETER (PINSRT=0, PREPLC=1)

C---- element types
      INTEGER PELEM_ALL
      INTEGER PENIL, PEANS, PERNS, PEFA, PEFA3, PEFAS
      INTEGER PEFAS3, PEFAS3D, PESFAS3D, PEPLI, PEPLI3
      INTEGER PEPM, PEPM3, PETX, PEII, PEICI, PEIS
      INTEGER PEBIS, PEISI, PEBISI, PELCI, PELW, PELT
      INTEGER PELI, PEMIND, PEMCI, PEMS, PEMT, PEEI
      INTEGER PEECI, PEEDW, PEET, PEEF, PETXI
      INTEGER PETXF, PETXTPR, PETXTP, PETXTA, PECH
      INTEGER PECE, PECS, PECUPV, PETXCI
      INTEGER PEIA, PELMT3, PEGMT3, PEVWI
      INTEGER PEES, PELB, PEPI, PEHID, PEICR
      INTEGER PEBICR, PELICR, PEMCR, PEECR
      INTEGER PETXCR, PELSS, PEISM, PEBISM
      INTEGER PEIRE, PEBIRE, PERPR, PEBRPR
      INTEGER PEFDM, PEFCM, PENET

      PARAMETER (PELEM_ALL=0)
      PARAMETER (PENIL=1, PEANS=2, PERNS=3, PEFA=4, PEFA3=5, PEFAS=6)
      PARAMETER (PEFAS3=7, PEFAS3D=8, PESFAS3D=9, PEPLI=10, PEPLI3=11)
      PARAMETER (PEPM=12, PEPM3=13, PETX=14, PEII=15, PEICI=16, PEIS=17)
      PARAMETER (PEBIS=18, PEISI=19, PEBISI=20, PELCI=21, PELW=22,
     + PELT=23)
      PARAMETER (PELI=24, PEMIND=25, PEMCI=26, PEMS=27, PEMT=28,
     + PEEI=29)
      PARAMETER (PEECI=30, PEEDW=31, PEET=32, PEEF=33, PETXI=34)
      PARAMETER (PETXF=35, PETXTPR=36, PETXTP=37, PETXTA=38, PECH=39)
      PARAMETER (PECE=40, PECS=41, PECUPV=42, PETXCI=43)
      PARAMETER (PEIA=44, PELMT3=45, PEGMT3=46, PEVWI=47)
      PARAMETER (PEES=48, PELB=49, PEPI=50, PEHID=51, PEICR=52)
      PARAMETER (PEBICR=53, PELICR=54, PEMCR=55, PEECR=56)
      PARAMETER (PETXCR=57, PELSS=58, PEISM=59, PEBISM=60)
      PARAMETER (PEIRE=61, PEBIRE=62, PERPR=63, PEBRPR=64)
      PARAMETER (PEFDM=65, PEFCM=66, PENET=67)

C---- light source stuff
      INTEGER    PAMB, PDIRE, PPOSI, PSPOT
      PARAMETER (PAMB=1, PDIRE=2, PPOSI=3, PSPOT=4)

C---- operating mode
      INTEGER    PREQU, PSAMPL, PEVENT
      PARAMETER (PREQU=0, PSAMPL=1, PEVENT=2)

C---- Echo switch
      INTEGER    PNECHO, PECHO
      PARAMETER (PNECHO=0, PECHO=1)

C----- Input classes
      INTEGER PNCLAS,PLOCAT,PSTROK,PVALUA,PCHOIC,PPICK,PSTRIN
      PARAMETER (PNCLAS=0,PLOCAT=1,PSTROK=2,PVALUA=3,PCHOIC=4,PPICK=5,
     +           PSTRIN=6)
      INTEGER PEXPOS, PRSIZE, PENWIN, PEXWIN
      PARAMETER (PEXPOS=10,PRSIZE=11,PENWIN=12,PEXWIN=13)

C---- Colour model, Color type
      INTEGER PIND, PINDIR, PRGB, PRGBA
      PARAMETER (PIND=0, PINDIR=0, PRGB=1, PRGBA=2)

C---- Search direction
      INTEGER PBWD, PFWD
      PARAMETER (PBWD=0, PFWD=1)

C---- Reflectance model
      INTEGER    PNORM  , PARM  , PADRM  , PADSRM
      PARAMETER (PNORM=1, PARM=2, PADRM=3, PADSRM=4)

C---- Interior shading
      INTEGER    PNOIS  , PCIS
      PARAMETER (PNOIS=1, PCIS=2)

C---- composition
      INTEGER     PCPRE,  PCPOST, PCREPL
      PARAMETER ( PCPRE=0, PCPOST=1, PCREPL=2 )

C---- control flags
      INTEGER    PCURVL, PRQSVL
      INTEGER    PCONDI, PALWAY
      INTEGER    PPOTOP, PPOBOT
      PARAMETER (PCURVL=0, PRQSVL=1)
      PARAMETER (PCONDI=0, PALWAY=1)
      PARAMETER (PPOTOP=0, PPOBOT=1)

C---- clipping related
      INTEGER    PMCREP, PMCINT
      PARAMETER (PMCREP=1, PMCINT=2)
      INTEGER    PNCLIP, PCLIP
      PARAMETER (PNCLIP=0, PCLIP=1)

C---- structure status inquiry
      INTEGER    PNOEXS,   PEXIST, PNOEMP
      PARAMETER (PNOEXS=0, PEXIST=1, PNOEMP=2)

C---- picking related
      INTEGER    PNONE, POK, PNPICK, PNCHOI
      PARAMETER (PNONE=0, POK=1, PNPICK=2, PNCHOI= 2)

C---- view type parallel or perspective
      INTEGER    PPARL, PPERS
      PARAMETER (PPARL=0, PPERS=1)

C---- search success return codes
      INTEGER    PFAIL, PSUCC
      PARAMETER (PFAIL=0, PSUCC=1 )

C---- choice input
      INTEGER    POFF, PON, PACT
      PARAMETER (POFF=0, PON=1, PACT=2)
