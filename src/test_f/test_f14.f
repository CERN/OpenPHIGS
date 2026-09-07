C *****************************************************************************
C * DO NOT REMOVE COPYRIGHT NOTICES OR THIS HEADER
C *
C * This file is part of Open PHIGS
C * Copyright (C) 2026 CERN for the benefit of the OPAL Collaboration
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

      SUBROUTINE VZERO(RX, NUM)
      INTEGER NUM
      REAL RX(*)
      DO I=1, NUM
        RX(I) = 0.0
      END DO
      END

      SUBROUTINE INITCOLS(KWIDGR)
      INTEGER KWIDGR, I
      REAL GRRED(0:15),GRGREE(0:15),GRBLUE(0:15)

*
*  Set default colours
*
      CALL VZERO(GRRED(0),16)
      GRRED(1)=1.
      GRRED(2)=1.
      GRRED(5)=1.
      GRRED(6)=1.
      GRRED(8)=0.5
      GRRED(11)=0.5
      GRRED(12)=0.5
      GRRED(13)=1.
      GRRED(14)=1.
      GRRED(15)=1.
      CALL VZERO(GRGREE(0),16)
      GRGREE(1)=1.
      GRGREE(3)=1.
      GRGREE(5)=1.
      GRGREE(7)=1.
      GRGREE(8)=0.5
      GRGREE(9)=0.5
      GRGREE(10)=1.
      GRGREE(12)=1.
      GRGREE(14)=0.5
      GRGREE(15)=0.7
      CALL VZERO(GRBLUE(0),16)
      GRBLUE(1)=1.
      GRBLUE(4)=1.
      GRBLUE(6)=1.
      GRBLUE(7)=1.
      GRBLUE(8)=0.5
      GRBLUE(9)=1.
      GRBLUE(10)=1.
      GRBLUE(11)=1.
      GRBLUE(13)=0.5
      DO 10 I=0,15
         CALL GRSCR(KWIDGR,I,GRRED(I),GRGREE(I),GRBLUE(I))
  10  CONTINUE
      END

CDECK  ID>, GRSCR.
      SUBROUTINE GRSCR(IWKID,ICOL,RED,GREEN,BLUE)
*
* Interface to old/new fortran bindings of Gphigs
*
*. AUTHOR    :  ?
*. CREATED   :  ?
*. LAST MOD  : 13-May-23
*.
*. Modification Log.
*. 13-May-23 U.Schwickerath Add flag for openphigs

      INTEGER IWKID,ICOL,ITYPE,IER,III
      REAL    RED,BLUE,GREEN,RGB(3)
      RGB(1)=RED
      RGB(2)=GREEN
      RGB(3)=BLUE
      CALL PSCR(IWKID,ICOL,3,RGB)
      END

CDECK  ID>, CLTOU.
      SUBROUTINE CLTOU (CHV)
C
C CERN PROGLIB# M432    CLTOU           .VERSION KERNFOR  4.21  890323
C ORIG. 11/02/86 A. PETRILLI
C NEW    9/02/89 JZ, for speed
C
C-    Convert character string CHV from lower to upper case.

      CHARACTER    CHV*(*)



      DO 19  JJ=1,LEN(CHV)
          J = ICHAR(CHV(JJ:JJ))
          IF (J.LT.97)       GO TO 19
          IF (J.GE.123)      GO TO 19
          CHV(JJ:JJ) = CHAR(J-32)
   19 CONTINUE
      END

CDECK, ID>, GRMTOOPH.
      INTEGER FUNCTION GRMTOOPH(I)
C     map interior style to correct openphigs settings
C     original settings   PHOLLO=0 , PSOLID=1 , PPATTR=2 , PHATCH=3 , PISEMP=4)
C     openphigs settings:  PISEMP=0 PHOLLO=1, PSOLID=2, PHATCH=3, PPATTR=4
      INTEGER MAP(0:4)
      DATA MAP /1, 2, 4, 3, 0 /
      IF (I.GE.0 .AND. I.LE.4) THEN
         GRMTOOPH = MAP(I)
      ELSE
         GRMTOOPH = 2
      ENDIF
      RETURN
      END

CDECK  ID>, GRLOCT.
      SUBROUTINE GRLOCT(TOFIND,SRCHIN,ITABSZ,INDEX)
*.
*...GRLOCT   Find the position of a string in a string array (Locate)
*.
*. INPUT     : CHARACTER*(*) TOFIND      STRING TO LOOK FOR
*. INPUT     : CHARACTER*(*) SRCHIN(*)   ARRAY IN WHICH TO SEARCH
*. INPUT     : INTEGER       ITABSZ      SIZE OF THE ARRAY SRCHIN
*.
*. SEQUENCE  : DECLAR RCREP.
*. CALLS     : CLTOU  REPORT
*. CALLED    : GRSERE GRAMP
*.
*. REPORT CONDITIONS
*.
*. AUTHOR    : David Ward
*. VERSION   : 2.01
*. CREATED   : 14-Apr-1994
*. LAST MOD  :
*.
*. Modification Log.
*.
*.**********************************************************************
*.
*
*. OUTPUT    : INTEGER       INDEX       INDEX OF THE STRING IN THE ARRA
*.
*. SEQUENCE  :
*. CALLS     :
*. CALLED    :
*.
*. REPORT CONDITIONS
*.
*. AUTHOR    : J. Le Mouel
*. VERSION   : 1.00
*. CREATED   :   6-Apr-89
*. LAST MOD  :  29-May-89
*.
*. Modification Log.
*. 29-May-89   J.LeMouel     Case insensitive
*. 26-Apr-89   D.R.Ward      Error message if something wrong
*.
*.**********************************************************************
*.
      IMPLICIT NONE
      CHARACTER*132 CHREP
      COMMON/RCREP/CHREP
      INTEGER INDEX,ITABSZ,I
      CHARACTER*(*) SRCHIN(*),TOFIND
      CHARACTER*15  TESRCH,TEFIND
      TEFIND = TOFIND
      CALL CLTOU(TEFIND)
      INDEX = 0
      DO 10 I =  1, ITABSZ
         TESRCH = SRCHIN(I)
         CALL CLTOU(TESRCH)
         IF (TESRCH .EQ. TEFIND) THEN
            INDEX = I
            GOTO 20
         ENDIF
   10 CONTINUE
      CHREP='String not found .... '//TOFIND
C      CALL REPORT('GRLOCT',1,'W')
   20 RETURN
      END
CDECK  ID>, GRPAD.
      SUBROUTINE GRPAD(NAME,NEWLEN)
*.
*...GRPAD      Add blanks at the end of the string until it is NEWLEN charac
*.
*. INPUT     : CHARACTER*(*) NAME        STRING TO GRPAD
*. OUTPUT    : CHRACTER*(*)  NAME        PADED STRING
*.
*. SEQUENCE  :
*. CALLS     :
*. CALLED    : GRAMP
*.
*. REPORT CONDITIONS
*.
*. AUTHOR    : J. Le Mouel
*. VERSION   : 1.00
*. CREATED   :   6-Apr-89
*. LAST MOD  :   6-Apr-89
*.
*. Modification Log.
*.  5-May-89   J.LeMouel Add the NEWLEN parameter
*.
*.**********************************************************************
*.
      CHARACTER*(*) NAME
      INTEGER LENGTH,I,NEWLEN
      LENGTH = LEN(NAME)
      IF (LENGTH .LT. NEWLEN) THEN
         LENGTH = NEWLEN - LENGTH
         DO 10 I = 1,LENGTH
            NAME = NAME // ' '
   10    CONTINUE
      ENDIF
      RETURN
      END

CDECK  ID>, GRAMP.
      SUBROUTINE GRAMP
*.
*...GRAMP    This routine contains the whole Attributes Management Package.
*.           The routine in itself does nothing; the active parts are called
*.           through the ENTRY points; these are grouped in the same routine
*.           in order for them to share the same datas (the style definition
*.           arrays defined in the REPRESDAT sequence), avoiding the use of
*.           COMMON BLOCKS and BLOCK DATAS.
*.
*. SEQUENCE  : DECLAR GRATTR GRCOLO GRDATA PENUM  REPRES
*. CALLS     : GRLOCT GRPAD  PQSTRS PSCHH  PSCHSP PSCHXP PSICI  PSIS
*. CALLS     : PSISI  PSLN   PSLWSC PSMK   PSMKSC PSPLCI PSPMCI PSTXCI
*. CALLS     : PSTXFN PSTXPR
*. CALLED    : <various>
*.
*. REPORT CONDITIONS
*.
*. AUTHOR    : J. Le Mouel
*. VERSION   : 1.00
*. CREATED   :   6-Apr-89
*. LAST MOD  : 13-May-23
*.
*. Modification Log.
*. 21-Jul-26   U.Schwickerath   Configure back face filling as well
*. 13-May-23   U.Schwickerath   Changes for openphigs
*. 15-Jul-93   D.R.Ward         Try to fix marker sizes in GOSIP/GPHIGS
*. 21-Apr-92   J.Banks          Remove IOFF clash
*. 26-Mar-92   J.Banks          Always use GPhigs software text
*.                              add 100 to colours for gosip
*. 22-Jan-91   J.Banks          Scale marker size for gosip
*. 14-Apr-89   J.LeMouel        Changes to use Individual Attributes
*.
*.**********************************************************************
*.
*
      IMPLICIT NONE
      INTEGER LTYPE,ICOL,IERR,INDEX,MTYPE,IFONT,IPREC,INTS,ISTYL
      INTEGER DINDEX,NAMLEN,ISTOP,LUN
      INTEGER PLMAX,PMMAX,TXMAX,FAMAX
      INTEGER GRMTOOPH
      REAL WIDTH,SZSF,CHXP,CHSP,TXCH
      CHARACTER*(15) NAME,NAME2
      CHARACTER*(*) TENAME,TTYPE
      CHARACTER*(4) TEABRE,ABRE
      CHARACTER*7   PLTYPE(4),PMTYPE(5),PRECIS(0:2),FAISTY(0:3)
C
C         Archive state
C
C   archive closed , archive opened
      INTEGER  PARCL  , PAROP
      PARAMETER (PARCL = 0 , PAROP  = 1)
C
C         Attribute identifier
C
      INTEGER PLN , PLWSC , PPLCI , PMK , PMKSC , PPMCI, PTXFN , PTXPR ,
     +PCHXP , PCHSP , PTXCI , PIS , PISI , PICI , PEDFG , PEDTY ,
     +PEWSC , PEDCI

      PARAMETER (PLN= 0, PLWSC= 1, PPLCI= 2, PMK= 3, PMKSC= 4, PPMCI= 5,
     +PTXFN= 6, PTXPR= 7, PCHXP= 8, PCHSP= 9, PTXCI=10, PIS =11, PISI
     +=12, PICI =13, PEDFG=14, PEDTY=15, PEWSC=16, PEDCI=17)
C
C         Aspect source flag
C
C   bundled individual
      INTEGER  PBUNDL       , PINDIV
      PARAMETER (PBUNDL=0     , PINDIV =1)
C
C         Clear control flag
C
C   conditionally   always
      INTEGER  PCONDI  , PALWAY
      PARAMETER (PCONDI=0 , PALWAY = 1)
C
C         Clipping indicator
C
C   noclip    clip
      INTEGER  PNCLIP  , PCLIP
      PARAMETER (PNCLIP=0 , PCLIP  = 1)
C
C         Colour available
C
C   monochrome   colour
      INTEGER  PMONOC  , PCOLOR
      PARAMETER (PMONOC=0 , PCOLOR = 1)
C
C         Composition type
C
C    preconcatenate   postconcatenate   replace
      INTEGER   PCPRE  ,  PCPOST    , PCREPL
      PARAMETER ( PCPRE=0 ,  PCPOST=1    , PCREPL=2 )
C
C         Conflict resolution
C
C   maintain , abandon    , update
      INTEGER  PCRMNT  , PCRABA    , PCRUPD
      PARAMETER (PCRMNT=0 , PCRABA =1    , PCRUPD=2)
C
C         Deferral mode
C
C   ASAP    , BNIG     , BNIL  , ASTI     , WAIT
      INTEGER  PASAP    , PBNIG    , PBNIL  , PASTI    , PWAITD
      PARAMETER (PASAP =0  , PBNIG =1 , PBNIL =2 , PASTI =3 , PWAITD=4)
C
C         Device coordinate units
C
C   metres      other
      INTEGER  PMETRE    , POTHU
      PARAMETER (PMETRE=0  , POTHU  =1)
C
C         Display surface empty
C
C   notempty  , empty
      INTEGER  PNEMPT    , PEMPTY
      PARAMETER (PNEMPT=0  , PEMPTY =1)
C
C         Dynamic modification
C
C   IRG      IMM    CBS
      INTEGER  PIRG    , PIMM  , PCBS
      PARAMETER (PIRG  =0  , PIMM   =1  , PCBS  =2)
C
C         Echo switch
C
C   noecho       echo
      INTEGER  PNECHO     , PECHO
      PARAMETER (PNECHO=0   , PECHO  =1)
C
C       Edit mode
C
C    insert      replace
      INTEGER   PINSRT    , PREPLC
      PARAMETER ( PINSRT=0  , PREPLC=1 )
C
C       Element Type
C
      INTEGER PEALL , PENIL , PEPL3 , PEPL , PEPM3 , PEPM , PETX3 ,
     +PETX , PEATR3 , PEATR , PEFA3 , PEFA , PEFAS3 , PEFAS , PECA3 ,
     +PECA , PEGDP3 , PEGDP , PEPLI , PEPMI , PETXI , PEII , PEEDI ,
     +PELN , PELWSC , PEPLCI , PEMK , PEMKSC , PEPMCI , PETXFN ,
     +PETXPR , PECHXP , PECHSP , PETXCI , PECHH , PECHUP

      INTEGER PETXP , PETXAL , PEATCH , PEATCU , PEATP , PEATAL ,
     +PEANST , PEIS , PEISI , PEICI , PEEDFG , PEEDT , PEEWSC , PEEDCI ,
     +PEPA , PEPRPV , PEPARF , PEADS , PERES , PEIASF , PEHRID ,
     +PELMT3 , PELMT , PEGMT3 , PEGMT , PEMCV3 , PEMCV , PEMCLI ,
     +PERMCL , PEVWI , PEEXST , PELB , PEAP , PEGSE , PEPKID
C
      PARAMETER ( PEALL = 00, PENIL = 01, PEPL3 = 02, PEPL = 03, PEPM3 =
     +04, PEPM = 05, PETX3 = 06, PETX = 07, PEATR3= 08, PEATR = 09,
     +PEFA3 = 10, PEFA = 11, PEFAS3= 12, PEFAS = 13, PECA3 = 14, PECA =
     +15, PEGDP3= 16, PEGDP = 17, PEPLI = 18, PEPMI = 19, PETXI = 20,
     +PEII = 21, PEEDI = 22, PELN = 23, PELWSC= 24, PEPLCI= 25, PEMK =
     +26, PEMKSC= 27, PEPMCI= 28, PETXFN= 29, PETXPR= 30, PECHXP= 31,
     +PECHSP= 32, PETXCI= 33, PECHH = 34, PECHUP=35)

      PARAMETER ( PETXP = 36, PETXAL= 37, PEATCH= 38, PEATCU= 39, PEATP
     += 40, PEATAL= 41, PEANST= 42, PEIS = 43, PEISI = 44, PEICI = 45,
     +PEEDFG= 46, PEEDT = 47, PEEWSC= 48, PEEDCI= 49, PEPA = 50, PEPRPV=
     +51, PEPARF= 52, PEADS = 53, PERES = 54, PEIASF= 55, PEHRID= 56,
     +PELMT3= 57, PELMT = 58, PEGMT3= 59, PEGMT = 60, PEMCV3= 61, PEMCV
     += 62, PEMCLI= 63, PERMCL= 64, PEVWI = 65, PEEXST= 66, PELB = 67,
     +PEAP = 68, PEGSE = 69, PEPKID=70)
C
C       Search success indicator
C
C   failure    , success
      INTEGER  PFAIL     , PSUCC
      PARAMETER (PFAIL =0  , PSUCC =1 )
C
C       Input device status
C
C   none     , ok      , nopick  , nochoice
      INTEGER  PNONE     , POK     , PNPICK  , PNCHOI
      PARAMETER (PNONE =0  , POK  =1 , PNPICK=2 , PNCHOI= 2)
C
C       Interior style
C
C    hollow   , solid    , pattern  , hatch    , empty
      INTEGER   PHOLLO   , PSOLID   , PPATTR  , PHATCH   , PISEMP
      PARAMETER ( PHOLLO=0 , PSOLID=1 , PPATTR=2 , PHATCH=3 , PISEMP=4)
C
C       Input class
C
C   none   ,locator ,stroke  ,valuator,choice  ,pick ,
C   string
      INTEGER PNCLAS,PLOCAT,PSTROK,PVALUA,PCHOIC,PPICK,PSTRIN
      PARAMETER (PNCLAS=0,PLOCAT=1,PSTROK=2,PVALUA=3,PCHOIC=4,PPICK=5,
     +           PSTRIN=6)
      INTEGER PEXPOS, PRSIZE, PENWIN, PEXWIN
      PARAMETER (PEXPOS=10,PRSIZE=11,PENWIN=12,PEXWIN=13)
C
C       Modification mode
C
C   NIVE     , UWOR     , UQUM
      INTEGER  PNIVE     , PUWOR    , PUQUM
      PARAMETER (PNIVE =0  , PUWOR =1 , PUQUM =2)
C
C      Off/on switch for edge flag and error handling mode
C
C   off       on
      INTEGER  POFF     , PON      ,PACT
      PARAMETER (POFF =0  , PON   =1 ,PACT   =2)
C
C       Open - Structure Status
C
C    none       open
      INTEGER   PNONST    , POPNST
      PARAMETER ( PNONST=0  , POPNST=1 )
C
C       Operating mode
C
C   request    , sample , event
      INTEGER  PREQU     , PSAMPL , PEVENT
      PARAMETER (PREQU =0  , PSAMPL=1 , PEVENT=2)
C
C       Path order
C
C   top first  , bottom first
      INTEGER  PPOTOP     , PPOBOT
      PARAMETER (PPOTOP =0  , PPOBOT=1)
C
C       Presence of invalid values
C
C   absent     , present
      INTEGER  PABSNT     , PPRSNT
      PARAMETER (PABSNT=0   , PPRSNT=1)
C
C       Reference handling flag
C
C   delete     , keep
      INTEGER  PDELE     , PKEEP
      PARAMETER (PDELE =0  , PKEEP =1)
C
C       regeneration flag
C
C   postpone   , perform
      INTEGER  PPOSTP     , PPERFO
      PARAMETER (PPOSTP =0  , PPERFO=1)
C
C       relative input priority
C
C   higher     , lower
      INTEGER  PHIGHR     , PLOWER
      PARAMETER (PHIGHR =0  , PLOWER=1)
C
C       search direction
C
C   backward   , forward
      INTEGER  PBWD     , PFWD
      PARAMETER (PBWD =0  , PFWD  =1)
C
C       simultaneous events flag
C
C   nomore     , more
      INTEGER  PNMORE     , PMORE
      PARAMETER (PNMORE =0  , PMORE =1)
C
C       state of visual representation
C
C   correct    , deferred , simulated
      INTEGER  PVROK     , PVRDFR , PVRSIM
      PARAMETER (PVROK =0  , PVRDFR=1 , PVRSIM=2)
C
C       structure network source
C
C   CSS     , archive file
      INTEGER  PCSS     , PARCHV
      PARAMETER (PCSS =0  , PARCHV=1)
C
C       Structure State Value
C
C    STCL      STOP
      INTEGER   PSTCL    , PSTOP
      PARAMETER ( PSTCL=0  , PSTOP=1 )
C
C       structure status indicator
C
C   non-existent, empty  , non-empty
      INTEGER  PSNOEX      , PSEMPT  , PSNEMP
      PARAMETER (PSNOEX =0   , PSEMPT=1  , PSNEMP=2)
C
C
C        System State Value
C
C    PHCL      PHOP
      INTEGER   PPHCL    , PPHOP
      PARAMETER ( PPHCL=0  , PPHOP=1 )
C
C       text alignment horizontal
C
C   normal     , left , center  , right
      INTEGER  PAHNOR     , PALEFT , PACENT  , PARITE
      PARAMETER (PAHNOR =0  , PALEFT=1 , PACENT=2, PARITE=3)
C
C       text alignment vertical
C
C   normal   ,top    ,cap    ,half    ,base    ,bottom
      INTEGER  PAVNOR   ,PATOP   ,PACAP  ,PAHALF  ,PABASE  ,PABOTT
      PARAMETER (PAVNOR =0,PATOP =1,PACAP=2,PAHALF=3,PABASE=4,PABOTT=5)
C
C       text path
C
C   right     , left , up   , down
      INTEGER  PRIGHT     , PLEFT , PUP   , PDOWN
      PARAMETER (PRIGHT =0  , PLEFT =1 , PUP=2   , PDOWN=3)
C
C       text precision
C
C   string     , character , stroke
      INTEGER  PSTRP     , PCHARP , PSTRKP
      PARAMETER (PSTRP =0  , PCHARP=1 , PSTRKP=2)
C
C       type of returned values
C
C   set     , realized
      INTEGER  PSET     , PREALI
      PARAMETER (PSET =0  , PREALI=1)
C
C       update state
C
C   notpending , pending
      INTEGER  PNPEND     , PPEND
      PARAMETER (PNPEND =0  , PPEND =1)
C
C       vector/raster/other type
C
C   vector     , raster , other
      INTEGER  PVECTR     , PRASTR , POTHWK
      PARAMETER (PVECTR =0  , PRASTR=1 , POTHWK=2)
C
C    viewtype
C
C   parallel   , perspective
      INTEGER  PPARL     , PPERS
      PARAMETER (PPARL =0  , PPERS =1)
C
C       workstation category
C
C   OUTPUT     , INPUT , OUTIN   , MO   , MI
      INTEGER  POUTPT     , PINPUT , POUTIN  , PMO   , PMI
      PARAMETER (POUTPT =0  , PINPUT=1 , POUTIN=2, PMO=3 , PMI=4)
C
C   workstation dependency indicator
C
C   workstation independent, workstation dependent ,
      INTEGER  PWKI   , PWKD
      PARAMETER (PWKI = 0  , PWKD=1)
C
C       workstation state
C
C   closed     , open
      INTEGER  PWKCL     , PWKOP
      PARAMETER (PWKCL =0  , PWKOP =1)
C
C       list of GDP
C
C     attributes polyline  , polymarker, text  , interior , edge
      INTEGER  PPLBND    , PPMBND    , PTXBND  , PINBND   , PEDBND
      PARAMETER (PPLBND =0 , PPMBND=1  , PTXBND=2, PINBND=3 , PEDBND=4)
C
C   linetype
C
C   solid     , dash , dot    , dash-dot
      INTEGER  PLSOLI     , PLDASH , PLDOT    , PLDASD
      PARAMETER (PLSOLI =1  , PLDASH=2 , PLDOT=3  , PLDASD=4)
C
C       marker type
C               "."        "+"       "*"      "o"       "x"
      INTEGER  PPOINT     , PPLUS , PAST  , POMARK  , PXMARK
      PARAMETER (PPOINT =1  , PPLUS =2 , PAST=3 , POMARK=4, PXMARK=5)
C
C       annotation style
C   unconnected, lead line using current
C         polyline attributes
      INTEGER  PUNCON     , PLDLN
      PARAMETER (PUNCON =1  , PLDLN =2)
C
C       colour model
C
C   RGB     , CIE , HSV   , HLS
C      INTEGER  PRGB     , PCIE , PHSV   , PHLS
C      PARAMETER (PRGB =1  , PCIE  =2 , PHSV=3  , PHLS=4)
      INTEGER PIND, PINDIR, PRGB, PRGBA
      PARAMETER (PIND=0, PINDIR=0, PRGB=1, PRGBA=2)

C
C       modelling clipping operator
C
C   replace      intersect
      INTEGER  PMCREP     , PMCINT
      PARAMETER (PMCREP =1  , PMCINT=2)
C
C       polyline/fill area control flag
C
C   polyline     fill area , fill area set
      INTEGER  PPLINE     , PFILLA , PFILAS
      PARAMETER (PPLINE =0  , PFILLA=1 , PFILAS=2)
C
C       Additional in the FORTRAN binding, to be used with
C       PHIGS inquiry functions that return both Current
C   and Requested values
C
C   current      requested
      INTEGER  PCURVL     , PRQSVL
      PARAMETER (PCURVL =0  , PRQSVL=1)
C
C
C        Names used for Error Handling
C
      INTEGER  EOPPH    ,ECLPH     ,EOPWK  ,ECLWK     ,ERST
      INTEGER  EUWK    ,ESDUS     ,EMSG  ,EPL3     ,EPL
      INTEGER  EPM3    ,EPM       ,ETX3  ,ETX     ,EATR3
      INTEGER  EATR    ,EFA3      ,EFA  ,EFAS3     ,EFAS
      INTEGER  ECA3    ,ECA       ,EGDP3  ,EGDP     ,ESPLI
      INTEGER  ESPMI    ,ESTXI     ,ESII  ,ESEDI     ,ESLN
      INTEGER  ESLWSC    ,ESPLCI    ,ESMK  ,ESMKSC    ,ESPMCI
      INTEGER  ESTXFN    ,ESTXPR    ,ESCHXP  ,ESCHSP    ,ESTXCI
      INTEGER  ESCHH    ,ESCHUP    ,ESTXP  ,ESTXAL    ,ESATCH
      INTEGER  ESATCU    ,ESATP     ,ESATAL  ,ESANS     ,ESIS
      INTEGER  ESISI    ,ESICI     ,ESEDFG  ,ESEDT     ,ESEWSC
      INTEGER  ESEDCI    ,ESPA      ,ESPRPV  ,ESPARF    ,EADS
      INTEGER  ERES    ,ESIASF    ,ESPLR  ,ESPMR     ,ESTXR
      INTEGER  ESIR    ,ESEDR     ,ESPAR  ,ESCR     ,ESHLFT
      INTEGER  ESIVFT    ,ESCMD     ,ESHRID  ,ESHRM     ,ESLMT3
      INTEGER  ESLMT    ,ESGMT3    ,ESGMT  ,ESMCV3    ,ESMCV
      INTEGER  ESMCLI    ,ERMCV     ,ESVWI  ,ESVWR3    ,ESVWR
      INTEGER  ESVTIP    ,ESWKW3    ,ESWKW  ,ESWKV3    ,ESWKV
      INTEGER  EOPST    ,ECLST     ,EEXST  ,ELB     ,EAP
      INTEGER  EGSE    ,ESEDM     ,ECELST  ,ESEP     ,EOSEP
      INTEGER  ESEPLB    ,EDEL      ,EDELRA  ,EDELLB    ,EEMST
      INTEGER  EDST    ,EDSN      ,EDAS  ,ECSTID    ,ECSTRF
      INTEGER  ECSTIR    ,EPOST     ,EUPOST  ,EUPAST    ,EOPARF
      INTEGER  ECLARF    ,EARST     ,EARSN  ,EARAST    ,ESCNRS
      INTEGER  ERSID    ,EREST     ,ERESN  ,ERAST     ,ERANST
      INTEGER  ERDEST    ,EDSTAR    ,EDSNAR  ,EDASAR    ,ESPKID
      INTEGER  ESPKFT    ,EINLC3    ,EINLC  ,EINSK3    ,EINSK
      INTEGER  EINVL3    ,EINVL     ,EINCH3    ,EINCH     ,EINPK3
      INTEGER  EINPK     ,EINST3    ,EINST     ,ESLCM     ,ESSKM
      INTEGER  ESVLM     ,ESCHM     ,ESPKM     ,ESSTM     ,ERQLC3
      INTEGER  ERQLC     ,ERQSK3    ,ERQSK     ,ERQVL     ,ERQCH
      INTEGER  ERQPK     ,ERQST     ,ESMLC3    ,ESMLC     ,ESMSK3
      INTEGER  ESMSK     ,ESMVL     ,ESMCH     ,ESMPK     ,ESMST
      INTEGER  EWAIT     ,EFLUSH    ,EGTLC3    ,EGTLC     ,EGTSK3
      INTEGER  EGTSK     ,EGTVL     ,EGTCH     ,EGTPK     ,EGTST
      INTEGER  EWITM     ,EGTITM    ,ERDITM    ,EIITM     ,ESERHM
      INTEGER  EUREC     ,EPREC     ,EESC
C
      PARAMETER (EOPPH =000,ECLPH =001,EOPWK =002,ECLWK =003,ERST  =004)
      PARAMETER (EUWK  =005,ESDUS =006,EMSG  =007,EPL3 =008,EPL   =009)
      PARAMETER (EPM3  =010,EPM   =011,ETX3  =012,ETX =013,EATR3 =014)
      PARAMETER (EATR  =015,EFA3  =016,EFA   =017,EFAS3 =018,EFAS  =019)
      PARAMETER (ECA3  =020,ECA   =021,EGDP3 =022,EGDP =023,ESPLI =024)
      PARAMETER (ESPMI =025,ESTXI =026,ESII  =027,ESEDI =028,ESLN  =029)
      PARAMETER (ESLWSC=030,ESPLCI=031,ESMK  =032,ESMKSC=033,ESPMCI=034)
      PARAMETER (ESTXFN=035,ESTXPR=036,ESCHXP=037,ESCHSP=038,ESTXCI=039)
      PARAMETER (ESCHH =040,ESCHUP=041,ESTXP =042,ESTXAL=043,ESATCH=044)
      PARAMETER (ESATCU=045,ESATP =046,ESATAL=047,ESANS =048,ESIS  =049)
      PARAMETER (ESISI =050,ESICI =051,ESEDFG=052,ESEDT =053,ESEWSC=054)
      PARAMETER (ESEDCI=055,ESPA  =056,ESPRPV=057,ESPARF=058,EADS  =059)
      PARAMETER (ERES  =060,ESIASF=061,ESPLR =062,ESPMR =063,ESTXR =064)
      PARAMETER (ESIR  =065,ESEDR =066,ESPAR =067,ESCR =068,ESHLFT=069)
      PARAMETER (ESIVFT=070,ESCMD =071,ESHRID=072,ESHRM =073,ESLMT3=074)
      PARAMETER (ESLMT =075,ESGMT3=076,ESGMT =077,ESMCV3=078,ESMCV =079)
      PARAMETER (ESMCLI=080,ERMCV =081,ESVWI =082,ESVWR3=083,ESVWR =084)
      PARAMETER (ESVTIP=085,ESWKW3=086,ESWKW =087,ESWKV3=088,ESWKV =089)
      PARAMETER (EOPST =090,ECLST =091,EEXST =092,ELB =093,EAP   =094)
      PARAMETER (EGSE  =095,ESEDM =096,ECELST=097,ESEP =098,EOSEP =099)
      PARAMETER (ESEPLB=100,EDEL  =101,EDELRA=102,EDELLB=103,EEMST =104)
      PARAMETER (EDST  =105,EDSN  =106,EDAS  =107,ECSTID=108,ECSTRF=109)
      PARAMETER (ECSTIR=110,EPOST =111,EUPOST=112,EUPAST=113,EOPARF=114)
      PARAMETER (ECLARF=115,EARST =116,EARSN =117,EARAST=118,ESCNRS=119)
      PARAMETER (ERSID =120,EREST =121,ERESN =122,ERAST =123,ERANST=124)
      PARAMETER (ERDEST=125,EDSTAR=126,EDSNAR=127,EDASAR=128,ESPKID=129)
      PARAMETER (ESPKFT=130,EINLC3=131,EINLC =132,EINSK3=133,EINSK =134)
      PARAMETER (EINVL3=135,EINVL =136,EINCH3=137,EINCH =138,EINPK3=139)
      PARAMETER (EINPK =140,EINST3=141,EINST =142,ESLCM =143,ESSKM =144)
      PARAMETER (ESVLM =145,ESCHM =146,ESPKM =147,ESSTM =148,ERQLC3=149)
      PARAMETER (ERQLC =150,ERQSK3=151,ERQSK =152,ERQVL =153,ERQCH =154)
      PARAMETER (ERQPK =155,ERQST =156,ESMLC3=157,ESMLC =158,ESMSK3=159)
      PARAMETER (ESMSK =160,ESMVL =161,ESMCH =162,ESMPK =163,ESMST =164)
      PARAMETER (EWAIT =165,EFLUSH=166,EGTLC3=167,EGTLC =168,EGTSK3=169)
      PARAMETER (EGTSK =170,EGTVL =171,EGTCH =172,EGTPK =173,EGTST= 174)
      PARAMETER (EWITM =175,EGTITM=176,ERDITM=177,EIITM =178,ESERHM=179)
      PARAMETER (EUREC =180,EPREC =181,EESC  =182)
C
C
C
C  INQUIRY
C
C         Structure status indicator
C
C   not exist , not empty
      INTEGER  PNOEXS    , PNOEMP
      PARAMETER (PNOEXS=0  , PNOEMP=2)
C
C
C=====================================================================
C================ EXTENTION ==========================================
C=====================================================================
C
C     Additional Names used for Extension Error Handling
C
      INTEGER    EXAMOD    ,EXGMOD    ,EXCHLG
      PARAMETER (EXAMOD=900,EXGMOD=901,EXCHLG=909)
      INTEGER    EXQMT     ,EXQMT3
      PARAMETER (EXQMT =902,EXQMT3=903)
      INTEGER    EXSTFT    ,EXRTRP
      PARAMETER (EXSTFT=904,EXRTRP=905)
      INTEGER    EXQTLA    ,EXQTMA    ,EXQTTA
      PARAMETER (EXQTLA=906,EXQTMA=907,EXQTTA=908)
C
C
C
      INTEGER    PXMCWC  ,PXWCMC
      PARAMETER (PXMCWC=0,PXWCMC=1)
C HLHSR identifiers
C
      INTEGER    PHIDEF  , PZBON   , PZBOFF
      PARAMETER (PHIDEF=0, PZBON=1 , PZBOFF=2)
*
*  Stores number of available colours (as given by PQCF)
*
      REAL GRRED(0:15),GRGREE(0:15),GRBLUE(0:15)
      COMMON /GRCOLO/NCOLI,ICOLA,NPCI,ICOLOR,GRRED,GRGREE,GRBLUE
      INTEGER NCOLI,ICOLA,NPCI,ICOLOR
*     View index numbers
      INTEGER JVWIEV,JVWIAX,JVWIHD
      PARAMETER (JVWIEV=4,JVWIAX=2,JVWIHD=0)
      INTEGER JVWIV1,JVWIV2,JVWIV3
      PARAMETER (JVWIV1=4,JVWIV2=5,JVWIV3=3)
*
*     Transformation numbers
      INTEGER JTNREV,JTNRAX,JTNRHD
      PARAMETER (JTNREV=4,JTNRAX=5,JTNRHD=3)
*
*     Workstation types.  (MetaFile O/p)
      INTEGER KWTMFO
      PARAMETER (KWTMFO=10201)
*
*     KWTYPE (screen) set by data card/KUIP
*     MFSTAT tells whether Metafile open
*     COLOUR flags whether screen in colour or not
      INTEGER KWTYPE,MFSTAT
      LOGICAL COLOUR
*     Workstation IDs  (GRope, MetaFile, DZdoc etc.)
*             KWIDGR,KWIDMF, etc.
*     Connection  IDs  (GRope, MetaFile)
*             KONIGR,KONIMF
*
      INTEGER KWTYGR,KWTYDZ,KWTYKU,
     + KWIDGR,KWIDMF,KWIDDZ,KWIDLE,KWIDDX,KWIDSI,KWIDSW,KWIDTF,
     + KONIGR,KONIMF,
     + IPPERS
      REAL DPRP
      EQUIVALENCE (KWTYGR,KWTYPE)
      COMMON/GRDATA/KWTYGR,KWTYDZ,KWTYKU,
     +KWIDGR,KWIDMF,KWIDDZ,KWIDLE,KWIDDX,KWIDSI,KWIDSW,KWIDTF,
     +KONIGR,KONIMF,
     +MFSTAT,COLOUR,IPPERS,DPRP
      INTEGER IGRLN , IGRPLC, IGRMK , IGRMKC, IGRTXF, IGRTXP, IGRTXC,
     +IGRFIS, IGRFSI, IGRFAC
      REAL    GRLWSC, GRMKSC, GRTXXP, GRTXSP, GRTXCH
      COMMON/GRATTR/IGRLN,GRLWSC,IGRPLC, IGRMK,GRMKSC,IGRMKC, IGRTXF,
     +IGRTXP,IGRTXC,GRTXXP,GRTXSP,GRTXCH, IGRFIS,IGRFSI,IGRFAC
*
*     Attributes currently set through GRAMP calls
*
      INTEGER PLSIZE,PMSIZE,TXSIZE,FASIZE
      PARAMETER (PLSIZE=100,PMSIZE=40,TXSIZE=30,FASIZE=30)
      CHARACTER*15 PLNAME(PLSIZE),PMNAME(PMSIZE),TXNAME(TXSIZE),
     +FANAME(FASIZE)
      CHARACTER*4 PLABRE(PLSIZE),PMABRE(PMSIZE),TXABRE(TXSIZE),
     +FAABRE(FASIZE)
      INTEGER I
      INTEGER PLLN(PLSIZE),PLPLCI(PLSIZE),PMMK(PMSIZE),
     +PMPMCI(PMSIZE),TXTXF(TXSIZE),TXTXP(TXSIZE),TXTXCI(TXSIZE),
     +FAFAIS(FASIZE),FAFASI(FASIZE),FAFACI(FASIZE)
      INTEGER MAXPL,MAXPM,MAXTX,MAXFA
      INTEGER PMAXPL,PMAXPM,PMAXTX,PMAXFA
      REAL PLLWSC(PLSIZE),PMMKSC(PMSIZE),TXCHXP(TXSIZE),TXCHSP(TXSIZE),
     +TXCHH(TXSIZE)
      PARAMETER (PMAXPL=92,PMAXPM=38,PMAXTX=26,PMAXFA=25)
*
* Default values
*
      INTEGER PLLN0(PLSIZE),PLPLC0(PLSIZE),PMMK0(PMSIZE),
     +PMPMC0(PMSIZE),TXTXF0(TXSIZE),TXTXP0(TXSIZE),TXTXC0(TXSIZE),
     +FAFAI0(FASIZE),FAFAS0(FASIZE),FAFAC0(FASIZE)
      REAL PLLWS0(PLSIZE),PMMKS0(PMSIZE),TXCHX0(TXSIZE),TXCHS0(TXSIZE),
     +TXCHH0(TXSIZE)
*
      DATA MAXPL,MAXPM,MAXTX,MAXFA /PMAXPL,PMAXPM,PMAXTX,PMAXFA/
C
C   DEFINE GROPE'S "Standard" POLY-LINE STYLES
C
C
C    INTERNAL NAME,    TYPE,WIDTH,COLOR INDEX
C
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I), I=1,10)
     +/
     +'AxisLine       ','AX  ',PLSOLI,1.0,1,
     +'ButtonsLine    ','BU  ',PLSOLI,1.0,1,
     +'CDRecLine      ','CDR ',PLSOLI,1.0,6,
     +'CJGeoLine      ','CJG ',PLSOLI,1.0,1,
     +'CJTrackLine    ','CJT ',PLSOLI,1.0,1,
     +'CVGeoLine      ','CVG ',PLSOLI,1.0,1,
     +'CVGeoAnodeLine ','CVGA',PLDASH,1.0,1,
     +'CVRecLine      ','CVR ',PLSOLI,1.0,2,
     +'CVRecUnmatcLine','CVRU',PLSOLI,1.0,2,
     +'CZGeoLine      ','CZG ',PLSOLI,1.0,1
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I), I=11,20)
     +/
     +'CZRecLine      ','CZR ',PLSOLI,1.0,5,
     +'EBGeoLine      ','EBG ',PLDOT ,1.0,1,
     +'EBHitLine      ','EBH ',PLSOLI,1.0,5,
     +'EEGeoLine      ','EEG ',PLDOT ,1.0,1,
     +'EEHitLine      ','EEH ',PLSOLI,1.0,5,
     +'EMRecLine      ','EMR ',PLSOLI,1.0,5,
     +'EXRecLine      ','EXR ',PLSOLI,1.0,6,
     +'MERgeoline     ','MERG',PLDOT ,1.0,1,
     +'MEHitline      ','MEHL',PLSOLI,1.0,5,
     +'TOFHitLine     ','TBH ',PLSOLI,1.0,3
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I), I=21,30)
     +/
     +'ZBoxLine       ','ZB  ',PLDASH,1.0,1,
     +'GRTreeCHadrLine','GRTC',PLSOLI,1.0,2,
     +'GRTreeElectLine','GRTE',PLSOLI,1.0,3,
     +'GRTreeMuonLine ','GRTM',PLDASH,1.0,3,
     +'GRTreeNHadrLine','GRTH',PLDASH,1.0,2,
     +'GRTreeNuLine   ','GRTN',PLDOT ,1.0,3,
     +'GRTreePhotoLine','GRTP',PLDOT ,1.0,2,
     +'GRTreeTauLine  ','GRTT',PLDASD,1.0,3,
     +'HeaderLine     ','HEAD',PLSOLI,1.0,1,
     +'HBHitLine      ','HBH ',PLSOLI,1.0,6
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I), I=31,40)
     +/
     +'HBStripsLine   ','HBS ',PLSOLI,1.0,6,
     +'2DBoxline      ','2BOX',PLSOLI,1.0,15,
     +'HEHitLine      ','HEH ',PLSOLI,1.0,6,
     +'HEStripsLine   ','HES ',PLSOLI,1.0,6,
     +'HPHitLine      ','HPH ',PLSOLI,1.0,6,
     +'HPStripsLine   ','HPS ',PLSOLI,1.0,6,
     +'MBGeoLine      ','MBG ',PLDOT ,1.0,1,
     +'MEGeoLine      ','MEG ',PLDOT ,1.0,1,
     +'MEHitLine      ','MEH ',PLSOLI,1.0,1,
     +'MEStripsLine   ','MES ',PLSOLI,1.0,7
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I), I=41,50)
     +/
     +'MUHadStrLine   ','MUH ',PLSOLI,1.0,6,
     +'MURecMEMBLine  ','MUR ',PLSOLI,1.0,2,
     +'MURecMMMergLine','MUMG',PLSOLI,1.0,2,
     +'MURecMMNoMgLine','MUNM',PLSOLI,1.0,1,
     +'ODCTBadChiLine ','ODCB',PLDOT ,1.0,7,
     +'ODCTGoodChiLine','ODCG',PLSOLI,1.0,7,
     +'ODCTMediChiLine','ODCM',PLDASH,1.0,7,
     +'ODECalLine     ','ODE ',PLSOLI,1.0,5,
     +'ODFDetLine     ','ODF ',PLSOLI,1.0,3,
     +'ODHCalBadLine  ','ODHB',PLDASH,1.0,6
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I), I=51,60)
     +/
     +'ODHCalGoodLine ','ODHG',PLSOLI,1.0,6,
     +'ODHCalMuonLine ','ODHM',PLDOT ,1.0,2,
     +'ODMuonLine     ','ODM ',PLSOLI,1.0,2,
     +'ODPresCoLine   ','ODPC',PLSOLI,1.0,4,
     +'ODPresBWLine   ','ODPB',PLDOT ,1.0,1,
     +'ODSecVrtxBWLine','ODSB',PLDOT ,1.0,1,
     +'ODSecVrtxCoLine','ODSC',PLSOLI,1.0,2,
     +'StandardLine   ','STD ',PLSOLI,1.0,1,
     +'EtoCLine1      ','ECL1',PLSOLI,1.0,2,
     +'EtoCLine2      ','ECL2',PLSOLI,1.0,5
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I),
     +I=61,70)
     +/
     +'EtoCLine3      ','ECL3',PLSOLI,1.0,3,
     +'EtoCLine4      ','ECL4',PLSOLI,1.0,4,
     +'EtoCLine5      ','ECL5',PLSOLI,1.0,6,
     +'EtoCLine6      ','ECL6',PLSOLI,1.0,7,
     +'EtoCLine7      ','ECL7',PLSOLI,1.0,1,
     +'HPRecLine      ','HPR ',PLSOLI,1.0,6,
     +'PBGeoLine      ','PBG ',PLDASH,1.0,1,
     +'PBHitLine      ','PBH ',PLSOLI,1.0,7,
     +'PBRecLine      ','PBR ',PLSOLI,1.0,2,
     +'TOFROutLine    ','TBRL',PLSOLI,1.0,3
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I),
     +I=71,80)
     +/
     +'ThrustLine     ','THR ',PLSOLI,3.0,4,
     +'PmisLine       ','PVIS',PLSOLI,3.0,5,
     +'FDetRecLine    ','FDR ',PLSOLI,1.0,3,
     +'MELgeoline     ','MELG',PLDOT ,1.0,1,
     +'TOFROutDummLine','TBDL',PLDOT ,1.0,3,
     +'HBStripEdgeLine','TBDL',PLDASH,1.0,3,
     +'HBStripDeadLine','TBDL',PLDOT ,1.0,3,
     +'SIBarrelLine   ','SIB ',PLSOLI,1.0,1,
     +'SILadderLine   ','SIL ',PLSOLI,1.0,1,
     +'SIDetectorLine ','SID ',PLSOLI,1.0,1
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I),
     +I=81,90)
     +/
     +'SIHitsLine     ','SIH ',PLSOLI,1.0,3,
     +'SIReconLine    ','SIR ',PLSOLI,1.0,2,
     +'ODSiWLine      ','ODSW',PLSOLI,1.0,3,
     +'FillBoxLine    ','FBOX',PLSOLI,1.0,1,
     +'JetsLineCone   ','JETC',PLDOT ,1.0,4,
     +'JetsLineJade   ','JETS',PLSOLI,3.0,4,
     +'IDElectronLine ','IDEL',PLSOLI,2.0,5,
     +'IDMuonLine     ','IDMU',PLSOLI,2.0,2,
     +'IDLambdaLine   ','IDLA',PLSOLI,2.0,14,
     +'IDKshortLine   ','IDKS',PLSOLI,2.0,3
     +/
      DATA (PLNAME(I),PLABRE(I),PLLN(I),PLLWSC(I),PLPLCI(I),
     +I=91,PMAXPL)
     +/
     +'IDGammaConvLine','ISGC',PLSOLI,2.0,11,
     +'CombineLine    ','COMB',PLSOLI,2.0,6
     +/
C
C   DEFINE GROPE'S "Standard" POLY-MARKER STYLES
C
C
C    INTERNAL NAME,    TYPE,SCALE,COLOR INDEX
C
      DATA (PMNAME(I),PMABRE(I),PMMK(I),PMMKSC(I),PMPMCI(I), I=1,10)
     +/
     +'CJTrkExtraPmark','CJTE',PXMARK,1.0,6,
     +'CJHitPmark     ','CJH ',PPOINT,.25,3,
     +'CJAssHitPmark  ','CJHA',PXMARK,0.2,3,
     +'CVHitAssocPmark','CVHA',POMARK,0.2,3,
     +'CVHitUnassPmark','CVHU',PXMARK,0.2,3,
     +'CVHitGhostPmark','CVHG',PAST  ,0.2,3,
     +'CXReconPmark   ','CXR ',PAST  ,0.5,3,
     +'CZHitPmark     ','CZH ',PPOINT,.25,5,
     +'MBCheatPmark   ','MBC ',POMARK,0.5,1,
     +'MBHitPmark     ','MBH ',PPLUS ,0.5,5
     +/
      DATA (PMNAME(I),PMABRE(I),PMMK(I),PMMKSC(I),PMPMCI(I),I=11,20    )
     +/
     +'MBStdPmark     ','MBS ',PXMARK,0.5,1,
     +'MECheatPmark   ','MEC ',POMARK,0.5,5,
     +'MEStdPmark     ','MES ',PXMARK,0.5,5,
     +'ODVrtxPmark    ','ODV ',PAST  ,0.5,2,
     +'StandardPmark  ','STD ',PPOINT,2.0,1,
     +'TOFHitPmark    ','TOFH',PPLUS ,0.5,3,
     +'ZBoxPMark      ','ZB  ',PPLUS ,3.0,1,
     +'TOFHitROutPMark','TBRO',PXMARK,0.5,3,
     +'PBHitPmark     ','PBH ',PPLUS ,0.25,1,
     +'MERHitROutPMark','MERH',PXMARK,0.5,5
     +/
      DATA (PMNAME(I),PMABRE(I),PMMK(I),PMMKSC(I),PMPMCI(I),I=21,30)
     +/
     +'MELHitROutPMark','MELH',PXMARK,0.5,5,
     +'MELRecROutPMark','MELR',POMARK,0.5,6,
     +'MERRecROutPMark','MERR',POMARK,0.5,6,
     +'CTHitPmark     ','CTH ',PPOINT,.25,2,
     +'HBStripsMark   ','HBS ',PXMARK,0.2,6,
     +'HEStripsMark   ','HES ',PXMARK,0.2,6,
     +'HPStripsMark   ','HPS ',PXMARK,0.2,6,
     +'MEHitPmark     ','MEH ',PXMARK,0.5,5,
     +'MBHitTakenMark ','MBT ',PPLUS ,1.0,2,
     +'HBStripEdgeMark','TBDL',PPLUS ,0.2,6
     +/
      DATA (PMNAME(I),PMABRE(I),PMMK(I),PMMKSC(I),PMPMCI(I),I=31,PMAXPM)
     +/
     +'HBStripDeadMark','TBDL',PPOINT,0.2,6,
     +'MBXHitTakenMark','MBXT',PPLUS ,0.75,2,
     +'MBBadHitMark   ','MBBH',PXMARK,0.5,5,
     +'MBBadXHitTaken ','MBBX',PXMARK,0.75,2,
     +'MBBadHitTaken  ','MBBT',PXMARK,1.0,2,
     +'SIHitMarkAssoc ','SIHA',POMARK,0.3,2,
     +'SIHitMarkAssocz','SIHZ',PAST  ,0.3,2,
     +'SIHitMarkUnass ','SIHU',PXMARK,0.2,2
     +/
C
C   DEFINE GROPE'S "Standard" TEXT STYLES
C
C
C    INTERNAL NAME, FONT,PRECISION,EXPANSION,SPACING,COLOR INDEX
C
      DATA (TXNAME(I),TXABRE(I),TXTXF(I),TXTXP(I),TXCHXP(I),TXCHSP(I),
     +TXTXCI(I),TXCHH(I),I=1,10)
     +/
     +'AxisText       ','AX  ', 5, PCHARP,1.0,0.0,1,0.4,
     +'ButtonsText    ','BU  ',-1, PCHARP,1.0,0.1,1,20.,
     +'CJTrackText    ','CJT ',-1, PCHARP,1.0,0.0,1,10.,
     +'EMRecText      ','EMR ',-1, PCHARP,1.0,0.0,5,20.,
     +'HeaderText     ','HEAD', 5, PCHARP,.68,0.0,5,20.,
     +'ODECalText     ','ODE ',-1, PCHARP,1.0,0.0,5,10.,
     +'ODHCalText     ','ODH ',-1, PCHARP,1.0,0.0,6,20.,
     +'TriggerText    ','TR  ', 5, PCHARP,0.7,0.0,6,30.,
     +'2DBoxAxes      ','2DA ', 5, PCHARP,0.7,0.0,5,20.,
     +'DZDISPText     ','DZDI', 6, PSTRKP,1.0,0.0,4,0.4
     +/
      DATA (TXNAME(I),TXABRE(I),TXTXF(I),TXTXP(I),TXCHXP(I),TXCHSP(I),
     +TXTXCI(I),TXCHH(I),I=11,20)
     +/
     +'ODCTrkText     ','ODT ',-1, PCHARP,1.0,0.0,7,10.,
     +'ODMUonText     ','ODM ',-1, PCHARP,1.0,0.0,2,10.,
     +'CDRecText      ','CDR ',-1, PCHARP,1.0,0.0,7,10.,
     +'FDetRecText    ','FDR ',-1, PCHARP,1.0,0.0,5,20.,
     +'PrintTerseText ','PTT ',-1, PCHARP,0.68,0.1,1,20.,
     +'ThrustText     ','THR ',-1, PCHARP,1.0,0.0,4,20.,
     +'PmisText       ','PMIS',-1, PCHARP,1.0,0.0,5,20.,
     +'EEHitsText     ','EEH ',-1, PCHARP,1.0,0.0,5,1.5,
     +'TBText         ','TBT ',-1, PCHARP,1.0,0.0,3,8.,
     +'TreeText       ','TRT ',-1, PCHARP,1.0,0.0,7,10.
     +/
      DATA (TXNAME(I),TXABRE(I),TXTXF(I),TXTXP(I),TXCHXP(I),TXCHSP(I),
     +TXTXCI(I),TXCHH(I),I=21,PMAXTX)
     +/
     +'ODSiWText      ','ODSW',-1, PCHARP,1.0,0.0,3,20.,
     +'StatusText     ','STAT', 5, PCHARP,0.8,0.0,1,20.,
     +'FileNameText   ','FILE',-1, PCHARP,0.8,0.0,1,15.,
     +'ScaleText      ','SCAL', 5, PCHARP,0.8,0.0,1,15.,
     +'MBGeoText      ','MBG ',-1, PCHARP,1.0,0.0,1,8.,
     +'BigHeaderText  ','HEAB',-9, PCHARP,.88,0.0,5,25.
     +/
C
C   DEFINE GROPE'S "Standard" FILL-AREA STYLES
C
C
C    INTERNAL NAME, INTERIOR STYLE, STYLE INDEX, COLOR INDEX
C
      DATA (FANAME(I),FAABRE(I),FAFAIS(I),FAFASI(I),FAFACI(I), I=1,10)/
     +'ButtonsFArea   ','BU  ',PHOLLO,1,1,
     +'EEGoodBlock    ','EEGB',PHOLLO,1,1,
     +'EEDeadBlock    ','EEDB',PSOLID,1,1,
     +'EESickBlock    ','EESB',PHOLLO,1,1,
     +'HeaderBox      ','HEAD',PSOLID,1,4,
     +'LogoBackground ','LOGB',PSOLID,1,4,
     +'Logo_O         ','LOGO',PSOLID,1,6,
     +'Logo_P         ','LOGP',PSOLID,1,6,
     +'Logo_A         ','LOGA',PSOLID,1,6,
     +'Logo_L         ','LOGL',PSOLID,1,6/
      DATA (FANAME(I),FAABRE(I),FAFAIS(I),FAFASI(I),FAFACI(I), I=11,
     +20)/
     +'PriVtxFillArea ','PVTX',PSOLID,1,2,
     +'SecVtxFillArea ','SVTX',PSOLID,1,5,
     +'Background     ','BACK',PHOLLO,1,0,
     +'CVBackground   ','CVBG',PSOLID,1,15,
     +'CJBackground   ','CJBG',PSOLID,1,14,
     +'CZBackground   ','CZBG',PSOLID,1,15,
     +'EBBackground   ','EBBG',PSOLID,1, 2,
     +'EEBackground   ','EEBG',PSOLID,1, 2,
     +'MBBackground   ','MBBG',PSOLID,1,11,
     +'MEBackground   ','MEBG',PSOLID,1,11/
      DATA (FANAME(I),FAABRE(I),FAFAIS(I),FAFASI(I),FAFACI(I), I=21,
     +PMAXFA)/
     +'HBBackground   ','HBBG',PSOLID,1, 3,
     +'HEBackground   ','HEBG',PSOLID,1, 3,
     +'HPBackground   ','HPBG',PSOLID,1, 3,
     +'FDBackground   ','FDBG',PSOLID,1,13,
     +'SWBackground   ','SWBG',PSOLID,1,13/
*
      DATA PLTYPE/'SOLID','DASH','DOT','DASHDOT'/
      DATA PMTYPE/'POINT','PLUS','AST','OMARK','XMARK'/
      DATA PRECIS/'STRING','CHARACT','STROKE'/
      DATA FAISTY/'HOLLOW','SOLID','PATTERN','HATCH'/
      RETURN
      ENTRY GRAMPI
*
*     Initialize - store values in default arrays.
*
      DO 100 I=1,PLSIZE
         PLLN0(I)=PLLN(I)
         PLPLC0(I)=PLPLCI(I)
         PLLWS0(I)=PLLWSC(I)
 100  CONTINUE
      DO 110 I=1,PMSIZE
         PMMK0(I)=PMMK(I)
         PMPMC0(I)=PMPMCI(I)
         PMMKS0(I)=PMMKSC(I)
 110  CONTINUE
      DO 120 I=1,TXSIZE
         TXTXF0(I)=TXTXF(I)
         TXTXP0(I)=TXTXP(I)
         TXTXC0(I)=TXTXCI(I)
         TXCHX0(I)=TXCHXP(I)
         TXCHS0(I)=TXCHSP(I)
         TXCHH0(I)=TXCHH(I)
 120  CONTINUE
      DO 130 I=1,FASIZE
         FAFAI0(I)=FAFAIS(I)
         FAFAS0(I)=FAFASI(I)
         FAFAC0(I)=FAFACI(I)
 130  CONTINUE
      RETURN
*
      ENTRY GRAMPP(LUN)
*
*     Print (to LUN) divergences from defaults
*
         DO 300 I=1,PLSIZE
            IF(PLLN(I).NE.PLLN0(I) .OR. PLPLCI(I).NE.PLPLC0(I) .OR.
     +         PLLWSC(I).NE.PLLWS0(I)) THEN
               WRITE(LUN,301) PLNAME(I),PLTYPE(PLLN(I))
     +               ,PLLWSC(I),PLPLCI(I)
 301           FORMAT(' set/line ',A,1X,A,1X,F8.1,I4)
            ENDIF
 300     CONTINUE
         DO 310 I=1,PMSIZE
            IF(PMMK(I).NE.PMMK0(I) .OR. PMPMCI(I).NE.PMPMC0(I) .OR.
     +         PMMKSC(I).NE.PMMKS0(I)) THEN
               WRITE(LUN,311) PMNAME(I),PMTYPE(PMMK(I))
     +               ,PMMKSC(I),PMPMCI(I)
 311           FORMAT(' set/mark ',A,1X,A,1X,F8.1,I4)
            ENDIF
 310     CONTINUE
         DO 320 I=1,TXSIZE
            IF(TXTXF(I).NE.TXTXF0(I) .OR. TXTXP(I).NE.TXTXP0(I) .OR.
     +         TXTXCI(I).NE.TXTXC0(I) .OR. TXCHXP(I).NE.TXCHX0(I) .OR.
     +         TXCHSP(I).NE.TXCHS0(I) .OR. TXCHH(I).NE.TXCHH0(I)) THEN
               WRITE(LUN,321) TXNAME(I),TXTXF(I),PRECIS(TXTXP(I)),
     +                        TXCHXP(I),TXCHSP(I),TXTXCI(I),TXCHH(I)
 321           FORMAT(' set/text ',A,1X,I4,1X,A,1X,2F8.2,I4,F8.2)
            ENDIF
 320     CONTINUE
         DO 330 I=1,FASIZE
            IF(FAFAIS(I).NE.FAFAI0(I) .OR. FAFASI(I).NE.FAFAS0(I) .OR.
     +         FAFACI(I).NE.FAFAC0(I)) THEN
               WRITE(LUN,331) FANAME(I),FAISTY(FAFAIS(I))
     +               ,FAFASI(I),FAFACI(I)
 331           FORMAT(' set/filla ',A,1X,A,1X,2I4)
            ENDIF
 330     CONTINUE
*
      RETURN
*
      ENTRY GRAMPR(TENAME,TTYPE)
*
*     Reset values from default arrays.
*
      NAME=TENAME
      CALL CLTOU(NAME)
      CALL GRPAD(NAME,15)
      IF(TTYPE(1:1).EQ.'L') THEN
         DO 200 I=1,PLSIZE
            NAME2=PLNAME(I)
            CALL CLTOU(NAME2)
            IF(NAME(1:1).NE.'*' .AND. NAME(1:4).NE.PLABRE(I)
     +                         .AND. NAME(1:15).NE.NAME2) GO TO 200
            PLLN(I)=PLLN0(I)
            PLPLCI(I)=PLPLC0(I)
            PLLWSC(I)=PLLWS0(I)
 200     CONTINUE
      ELSEIF(TTYPE(1:1).EQ.'M') THEN
         DO 210 I=1,PMSIZE
            NAME2=PMNAME(I)
            CALL CLTOU(NAME2)
            IF(NAME(1:1).NE.'*' .AND. NAME(1:4).NE.PMABRE(I)
     +                         .AND. NAME(1:15).NE.NAME2) GO TO 210
            PMMK(I)=PMMK0(I)
            PMPMCI(I)=PMPMC0(I)
            PMMKSC(I)=PMMKS0(I)
 210     CONTINUE
      ELSEIF(TTYPE(1:1).EQ.'T') THEN
         DO 220 I=1,TXSIZE
            NAME2=TXNAME(I)
            CALL CLTOU(NAME2)
            IF(NAME(1:1).NE.'*' .AND. NAME(1:4).NE.TXABRE(I)
     +                         .AND. NAME(1:15).NE.NAME2) GO TO 220
            TXTXF(I)=TXTXF0(I)
            TXTXP(I)=TXTXP0(I)
            TXTXCI(I)=TXTXC0(I)
            TXCHXP(I)=TXCHX0(I)
            TXCHSP(I)=TXCHS0(I)
            TXCHH(I)=TXCHH0(I)
 220     CONTINUE
      ELSEIF(TTYPE(1:1).EQ.'F') THEN
         DO 230 I=1,FASIZE
            NAME2=FANAME(I)
            CALL CLTOU(NAME2)
            IF(NAME(1:1).NE.'*' .AND. NAME(1:4).NE.FAABRE(I)
     +                         .AND. NAME(1:15).NE.NAME2) GO TO 230
            FAFAIS(I)=FAFAI0(I)
            FAFASI(I)=FAFAS0(I)
            FAFACI(I)=FAFAC0(I)
 230     CONTINUE
      ENDIF
*
      RETURN
C
C Set PolyLine Representation (To change the types defined in REPRESDAT. or
C to add new types)
C
      ENTRY GRSPLR(TENAME,LTYPE,WIDTH,ICOL,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,PLABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,PLNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0 .OR. INDEX.GT.MAXPL) THEN
         IERR = 1
      ELSE
         PLLN(INDEX) = LTYPE
         PLLWSC(INDEX) = WIDTH
         PLPLCI(INDEX) = ICOL
      ENDIF
      RETURN
C
C Set PolyMarker Representation (To change the types defined in REPRESDAT)
C
      ENTRY GRSPMR(TENAME,MTYPE,SZSF,ICOL,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,PMABRE,MAXPM,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,PMNAME,MAXPM,INDEX)
      ENDIF
      IF (INDEX .EQ. 0 .OR. INDEX.GT.MAXPM) THEN
         IERR = 1
      ELSE
         PMMK(INDEX) = MTYPE
         PMMKSC(INDEX) = SZSF
         PMPMCI(INDEX) = ICOL
      ENDIF
      RETURN
C
C Set Text Representation (To change the types defined in REPRESDAT)
C
      ENTRY GRSTXR(TENAME,IFONT,IPREC,CHXP,CHSP,ICOL,TXCH,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,TXABRE,MAXTX,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,TXNAME,MAXTX,INDEX)
      ENDIF
      IF (INDEX .EQ. 0 .OR. INDEX.GT.MAXTX) THEN
         IERR = 1
      ELSE
         TXTXF(INDEX) = IFONT
         TXTXP(INDEX) = IPREC
         TXCHXP(INDEX) = CHXP
         TXCHSP(INDEX) = CHSP
         TXTXCI(INDEX) = ICOL
         TXCHH(INDEX) = TXCH
      ENDIF
      RETURN
C
C Set Fill Area Representation (To change the types defined in REPRESDAT)
C
      ENTRY GRSFAR(TENAME,INTS,ISTYL,ICOL,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,FAABRE,MAXFA,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,FANAME,MAXFA,INDEX)
      ENDIF
      IF (INDEX .EQ. 0 .OR. INDEX.GT.MAXFA) THEN
         IERR = 1
      ELSE
         FAFAIS(INDEX) = INTS
         FAFASI(INDEX) = ISTYL
         FAFACI(INDEX) = ICOL
      ENDIF
      RETURN
C
C inQuire PolyLine Representation.
C
      ENTRY GRQPLR(TENAME,LTYPE,WIDTH,ICOL,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,PLABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,PLNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         LTYPE = PLLN(INDEX)
         WIDTH = PLLWSC(INDEX)
         ICOL = PLPLCI(INDEX)
      ENDIF
      RETURN
C
C inQuire PolyMarker Representation.
C
      ENTRY GRQPMR(TENAME,MTYPE,SZSF,ICOL,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,PMABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,PMNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         MTYPE = PMMK(INDEX)
         SZSF = PMMKSC(INDEX)
         ICOL = PMPMCI(INDEX)
      ENDIF
      RETURN
C
C inQuire Text Representation.
C
      ENTRY GRQTXR(TENAME,IFONT,IPREC,CHXP,CHSP,ICOL,TXCH,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,TXABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,TXNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         IFONT = TXTXF(INDEX)
         IPREC = TXTXP(INDEX)
         CHXP = TXCHXP(INDEX)
         CHSP = TXCHSP(INDEX)
         ICOL = TXTXCI(INDEX)
         TXCH = TXCHH(INDEX)
      ENDIF
      RETURN
C
C inQuire Fill Area Representation.
C
      ENTRY GRQFAR(TENAME,INTS,ISTYL,ICOL,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,FAABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,FANAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         INTS = FAFAIS(INDEX)
         ISTYL = FAFASI(INDEX)
         ICOL = FAFACI(INDEX)
      ENDIF
      RETURN
C
C Set PolyLine Type.
C
      ENTRY GRSPLT(TENAME,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,PLABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,PLNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         IGRLN=PLLN(INDEX)
         GRLWSC=PLLWSC(INDEX)
         IGRPLC=1
         IF(PLPLCI(INDEX).EQ.0) IGRPLC=0
         IF(COLOUR) IGRPLC=PLPLCI(INDEX)
         CALL PQSTRS(ISTOP)
         IF(ISTOP.EQ.1) THEN
            CALL PSLWSC(GRLWSC)
            CALL PSLN(IGRLN)
            CALL PSPLCI(IGRPLC)
         ENDIF
      ENDIF
      RETURN
C
C Set PolyMarker Type.
C
      ENTRY GRSPMT(TENAME,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,PMABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,PMNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         IGRMK=-PMMK(INDEX)
         GRMKSC=5.0*PMMKSC(INDEX)
         IGRMKC=1
         IF(PMPMCI(INDEX).EQ.0) IGRMKC=0
         IF(COLOUR) IGRMKC=PMPMCI(INDEX)
         CALL PQSTRS(ISTOP)
         IF(ISTOP.EQ.1) THEN
            CALL PSMK(IGRMK)
            CALL PSMKSC(GRMKSC)
            CALL PSPMCI(IGRMKC)
         ENDIF
      ENDIF
      RETURN
C
C Set Text Type.
C
      ENTRY GRSTXT(TENAME,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,TXABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,TXNAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         IGRTXF=TXTXF(INDEX)
         IGRTXP=TXTXP(INDEX)
         IGRTXC=1
         IF(TXTXCI(INDEX).EQ.0) IGRTXC=0
         IF(COLOUR) IGRTXC=TXTXCI(INDEX)
         GRTXXP=TXCHXP(INDEX)
         GRTXSP=TXCHSP(INDEX)
         GRTXCH=TXCHH(INDEX)
         CALL PQSTRS(ISTOP)
         IF(ISTOP.EQ.1) THEN
            CALL PSTXFN(1)
            CALL PSTXPR(IGRTXP)
            CALL PSTXCI(IGRTXC)
            CALL PSCHXP(GRTXXP)
            CALL PSCHSP(GRTXSP)
            CALL PSCHH(GRTXCH)
         ENDIF
      ENDIF
      RETURN
C
C Set Fill Area Type.
C
      ENTRY GRSFAT(TENAME,IERR)
C
C
      IERR=0
      NAMLEN = LEN(TENAME)
      IF (NAMLEN .LE. 4) THEN
         ABRE = TENAME
         CALL GRPAD(ABRE,4)
         CALL GRLOCT(ABRE,FAABRE,MAXPL,INDEX)
      ELSE
         NAME = TENAME
         CALL GRPAD(NAME,15)
         CALL GRLOCT(NAME,FANAME,MAXPL,INDEX)
      ENDIF
      IF (INDEX .EQ. 0) THEN
         IERR = 1
      ELSE
         IGRFIS=FAFAIS(INDEX)
         IGRFSI=FAFASI(INDEX)
         IGRFAC=1
         IF(FAFACI(INDEX).EQ.0) IGRFAC=0
         IF(COLOUR) IGRFAC=FAFACI(INDEX)
         CALL PQSTRS(ISTOP)
         IF(ISTOP.EQ.1) THEN
            CALL PSIS(GRMTOOPH(IGRFIS))
            CALL PSBIS(GRMTOOPH(IGRFIS))
            CALL PSISI(IGRFSI)
            CALL PSICI(IGRFAC)
            CALL PSBICI(IGRFAC)
            print*, IGRFIS, "->", GRMTOOPH(IGRFIS)
            print*, IGRFSI, IGRFAC
         ENDIF
      ENDIF
      RETURN
C
C And now, a lot of little 'subroutines' (in fact ENTRYs) to query
C informations about the setup.
C
C inQuire Maximum PolyLine style presently defined
      ENTRY GRQMPL(PLMAX)
      PLMAX = MAXPL
      RETURN
C inQuire Maximum PolyMarker style presently defined
      ENTRY GRQMPM(PMMAX)
      PMMAX = MAXPM
      RETURN
C inQuire Maximum Text style presently defined
      ENTRY GRQMTX(TXMAX)
      TXMAX = MAXTX
      RETURN
C inQuire Maximum FillArea style presently defined
      ENTRY GRQMFA(FAMAX)
      FAMAX = MAXFA
      RETURN
C inQuire PolyLine style Name (given the index number of the style)
      ENTRY GRQPLN(DINDEX,TENAME,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXPL)) THEN
         IERR = 1
      ELSE
         TENAME = PLNAME(DINDEX)
      ENDIF
      RETURN
C inQuire PolyMarker style Name (given the index number of the style)
      ENTRY GRQPMN(DINDEX,TENAME,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXPM)) THEN
         IERR = 1
      ELSE
         TENAME = PMNAME(DINDEX)
      ENDIF
      RETURN
C inQuire Text style Name (given the index number of the style)
      ENTRY GRQTXN(DINDEX,TENAME,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXTX)) THEN
         IERR = 1
      ELSE
         TENAME = TXNAME(DINDEX)
      ENDIF
      RETURN
C inQuire FillArea style Name (given the index number of the style)
      ENTRY GRQFAN(DINDEX,TENAME,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXFA)) THEN
         IERR = 1
      ELSE
         TENAME = FANAME(DINDEX)
      ENDIF
      RETURN
C inQuire PolyLine style Abreviation (given the index number of the style)
      ENTRY GRQPLA(DINDEX,TEABRE,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXPL)) THEN
         IERR = 1
      ELSE
         TEABRE = PLABRE(DINDEX)
      ENDIF
      RETURN
C inQuire PolyMarker style Abreviation (given the index number of the style)
      ENTRY GRQPMA(DINDEX,TEABRE,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXPM)) THEN
         IERR = 1
      ELSE
         TEABRE = PMABRE(DINDEX)
      ENDIF
      RETURN
C inQuire Text style Abreviation (given the index number of the style)
      ENTRY GRQTXA(DINDEX,TEABRE,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXTX)) THEN
         IERR = 1
      ELSE
         TEABRE = TXABRE(DINDEX)
      ENDIF
      RETURN
C inQuire FillArea style Abreviation (given the index number of the style)
      ENTRY GRQFAA(DINDEX,TEABRE,IERR)
      IERR = 0
      IF ((DINDEX .LE. 0) .OR. (DINDEX .GT. MAXFA)) THEN
         IERR = 1
      ELSE
         TEABRE = FAABRE(DINDEX)
      ENDIF
      RETURN
C
C
C PolyLine Name to Abreviation convertion
C
      ENTRY GRPLNA(TENAME,TEABRE,IERR)
      NAME = TENAME
      TEABRE = ' '
      IERR = 1
      CALL GRPAD(NAME,15)
      CALL GRLOCT(NAME,PLNAME,MAXPL,INDEX)
      IF (INDEX .NE. 0) THEN
         TEABRE = PLABRE(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C PolyMarker Name to Abreviation convertion
C
      ENTRY GRPMNA(TENAME,TEABRE,IERR)
      NAME = TENAME
      TEABRE = ' '
      IERR = 1
      CALL GRPAD(NAME,15)
      CALL GRLOCT(NAME,PMNAME,MAXPM,INDEX)
      IF (INDEX .NE. 0) THEN
         TEABRE = PMABRE(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C Text Name to Abreviation convertion
C
      ENTRY GRTXNA(TENAME,TEABRE,IERR)
      NAME = TENAME
      TEABRE = ' '
      IERR = 1
      CALL GRPAD(NAME,15)
      CALL GRLOCT(NAME,TXNAME,MAXTX,INDEX)
      IF (INDEX .NE. 0) THEN
         TEABRE = TXABRE(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C FillArea Name to Abreviation convertion
C
      ENTRY GRFANA(TENAME,TEABRE,IERR)
      NAME = TENAME
      TEABRE = ' '
      IERR = 1
      CALL GRPAD(NAME,15)
      CALL GRLOCT(NAME,FANAME,MAXFA,INDEX)
      IF (INDEX .NE. 0) THEN
         TEABRE = FAABRE(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C PolyLine Abreviation to Name convertion
C
      ENTRY GRPLAN(TEABRE,TENAME,IERR)
      ABRE = TEABRE
      TENAME = ' '
      IERR = 1
      CALL GRPAD(ABRE,4)
      CALL GRLOCT(ABRE,PLABRE,MAXPL,INDEX)
      IF (INDEX .NE. 0) THEN
         TENAME = PLNAME(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C PolyMarker Abreviation to Name convertion
C
      ENTRY GRPMAN(TEABRE,TENAME,IERR)
      ABRE = TEABRE
      TENAME = ' '
      IERR = 1
      CALL GRPAD(ABRE,4)
      CALL GRLOCT(ABRE,PMABRE,MAXPM,INDEX)
      IF (INDEX .NE. 0) THEN
         TENAME = PMNAME(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C Text Abreviation to Name convertion
C
      ENTRY GRTXAN(TEABRE,TENAME,IERR)
      ABRE = TEABRE
      TENAME = ' '
      IERR = 1
      CALL GRPAD(ABRE,4)
      CALL GRLOCT(ABRE,TXABRE,MAXTX,INDEX)
      IF (INDEX .NE. 0) THEN
         TENAME = TXNAME(INDEX)
         IERR = 0
      ENDIF
      RETURN
C
C FillArea Abreviation to Name convertion
C
      ENTRY GRFAAN(TEABRE,TENAME,IERR)
      ABRE = TEABRE
      TENAME = ' '
      IERR = 1
      CALL GRPAD(ABRE,4)
      CALL GRLOCT(ABRE,FAABRE,MAXFA,INDEX)
      IF (INDEX .NE. 0) THEN
         TENAME = FANAME(INDEX)
         IERR = 0
      ENDIF
      RETURN
      END

CDECK  ID>, GRLOGO.
      SUBROUTINE GRLOGO(XCENT,YCENT,HHSIZE)
*.
*...GRLOGO       Crude first attempt at OPAL logo.
*.               (Better version anticipated from CERN soon)
*.
*. INPUT     : XCENT  x position of logo
*. INPUT     : YCENT  y position of logo
*. INPUT     : HHSIZE size of logo;  negative => HIGZ
*.
*. SEQUENCE  : DECLAR GRATTR GRDATA
*. CALLS     : GRSFAT IFA    IPL    ISFACI ISFAIS ISLWSC ISPLCI PFA
*. CALLS     : PPL    PSLWSC
*. CALLED    : GRHEAD GRDEDX GRTOF  GRLEGO SIROLL SWLEGO SWDISP
*.
*. REPORT CONDITIONS
*.
*. AUTHOR    : D.R.Ward
*. VERSION   : 0.00
*. CREATED   : 14-Jul-89
*. LAST MOD  : 21-Jul-26
*.
*. Modification Log.
*. 21-Jul-26   U. Schwickerath ensure line style is defined
*.  2-Dec-93   D.R.Ward      HIGZ version
*. 14-Jul-89   D.R.Ward      Zeroth version
*.
*.**********************************************************************
*.
      IMPLICIT NONE
      INTEGER IGRLN , IGRPLC, IGRMK , IGRMKC, IGRTXF, IGRTXP, IGRTXC,
     +IGRFIS, IGRFSI, IGRFAC
      REAL    GRLWSC, GRMKSC, GRTXXP, GRTXSP, GRTXCH
      COMMON/GRATTR/IGRLN,GRLWSC,IGRPLC, IGRMK,GRMKSC,IGRMKC, IGRTXF,
     +IGRTXP,IGRTXC,GRTXXP,GRTXSP,GRTXCH, IGRFIS,IGRFSI,IGRFAC
*
*     Attributes currently set through GRAMP calls
*
*     View index numbers
      INTEGER JVWIEV,JVWIAX,JVWIHD
      PARAMETER (JVWIEV=4,JVWIAX=2,JVWIHD=0)
      INTEGER JVWIV1,JVWIV2,JVWIV3
      PARAMETER (JVWIV1=4,JVWIV2=5,JVWIV3=3)
*
*     Transformation numbers
      INTEGER JTNREV,JTNRAX,JTNRHD
      PARAMETER (JTNREV=4,JTNRAX=5,JTNRHD=3)
*
*     Workstation types.  (MetaFile O/p)
      INTEGER KWTMFO
      PARAMETER (KWTMFO=10201)
*
*     KWTYPE (screen) set by data card/KUIP
*     MFSTAT tells whether Metafile open
*     COLOUR flags whether screen in colour or not
      INTEGER KWTYPE,MFSTAT
      LOGICAL COLOUR
*     Workstation IDs  (GRope, MetaFile, DZdoc etc.)
*             KWIDGR,KWIDMF, etc.
*     Connection  IDs  (GRope, MetaFile)
*             KONIGR,KONIMF
*
      INTEGER KWTYGR,KWTYDZ,KWTYKU,
     + KWIDGR,KWIDMF,KWIDDZ,KWIDLE,KWIDDX,KWIDSI,KWIDSW,KWIDTF,
     + KONIGR,KONIMF,
     + IPPERS
      REAL DPRP
      EQUIVALENCE (KWTYGR,KWTYPE)
      COMMON/GRDATA/KWTYGR,KWTYDZ,KWTYKU,
     +KWIDGR,KWIDMF,KWIDDZ,KWIDLE,KWIDDX,KWIDSI,KWIDSW,KWIDTF,
     +KONIGR,KONIMF,
     +MFSTAT,COLOUR,IPPERS,DPRP
      INTEGER IERROR
      REAL XCENT,YCENT,HSIZE,HHSIZE
      INTEGER II1
      PARAMETER (II1= 33)
      REAL XX1(II1)
      REAL YY1(II1)
      INTEGER II2
      PARAMETER (II2= 21)
      REAL XX2(II2)
      REAL YY2(II2)
      INTEGER II3
      PARAMETER (II3= 34)
      REAL XX3(II3)
      REAL YY3(II3)
      INTEGER II4
      PARAMETER (II4= 23)
      REAL XX4(II4)
      REAL YY4(II4)
      INTEGER II5
      PARAMETER (II5= 35)
      REAL XX5(II5)
      REAL YY5(II5)
      INTEGER II6
      PARAMETER (II6= 20)
      REAL XX6(II6)
      REAL YY6(II6)
      INTEGER II7
      PARAMETER (II7= 35)
      REAL XX7(II7)
      REAL YY7(II7)
      INTEGER II8
      PARAMETER (II8= 30)
      REAL XX8(II8)
      REAL YY8(II8)
      REAL X(100),Y(100)
      INTEGER I
      DATA XX1/ 0.1773, 0.1934, 0.2129, 0.2364, 0.2628, 0.2882, 0.3267,
     +0.3575, 0.3888, 0.4308, 0.4464, 0.4654, 0.4733, 0.4816, 0.4860,
     +0.4840, 0.4840, 0.4781, 0.4713, 0.4645, 0.4537, 0.4435, 0.2784,
     +0.2676, 0.2574, 0.2501, 0.2354, 0.2237, 0.2090, 0.1895, 0.1802,
     +0.1758, 0.1758/




      DATA YY1/ 0.4635, 0.5436, 0.5895, 0.6291, 0.6628, 0.6886, 0.7209,
     +0.7414, 0.7570, 0.7722, 0.7766, 0.7726, 0.7653, 0.7560, 0.7473,
     +0.7209, 0.2335, 0.2198, 0.2081, 0.2002, 0.1958, 0.1919, 0.1944,
     +0.1968, 0.2022, 0.2066, 0.2300, 0.2486, 0.2784, 0.3311, 0.3707,
     +0.4142, 0.4571/




      DATA XX2/ 0.2525, 0.2501, 0.2584, 0.2764, 0.3072, 0.3419, 0.3702,
     +0.3902, 0.4098, 0.4215, 0.4269, 0.4274, 0.4220, 0.4117, 0.3893,
     +0.3595, 0.3248, 0.2984, 0.2818, 0.2545, 0.2505/




      DATA YY2/ 0.4425, 0.4821, 0.5216, 0.5524, 0.5709, 0.5661, 0.5490,
     +0.5270, 0.4923, 0.4547, 0.4230, 0.3907, 0.3643, 0.3414, 0.3145,
     +0.3043, 0.3150, 0.3370, 0.3570, 0.4264, 0.4625/




      DATA XX3/ 0.4938, 0.5001, 0.5133, 0.5294, 0.5621, 0.5924, 0.6252,
     +0.6691, 0.7038, 0.7331, 0.7512, 0.7565, 0.7585, 0.7585, 0.7541,
     +0.7497, 0.7429, 0.7355, 0.7282, 0.6857, 0.5880, 0.5739, 0.5651,
     +0.5610, 0.5610, 0.5578, 0.5519, 0.5455, 0.5372, 0.5074, 0.4933,
     +0.4894, 0.4900, 0.4900/




      DATA YY3/ 0.7492, 0.7673, 0.7805, 0.7849, 0.7853, 0.7814, 0.7731,
     +0.7575, 0.7394, 0.7199, 0.7072, 0.6940, 0.6818, 0.4571, 0.4498,
     +0.4440, 0.4386, 0.4327, 0.4293, 0.4283, 0.4288, 0.4205, 0.4078,
     +0.3951, 0.0933, 0.0884, 0.0830, 0.0801, 0.0772, 0.0767, 0.0835,
     +0.0923, 0.1040, 0.7512/




      DATA XX4/ 0.5705, 0.5690, 0.5724, 0.5812, 0.5939, 0.6139, 0.6359,
     +0.6603, 0.6735, 0.6828, 0.6896, 0.6901, 0.6842, 0.6720, 0.6554,
     +0.6388, 0.6144, 0.5998, 0.5846, 0.5748, 0.5705, 0.5690, 0.5729/




      DATA YY4/ 0.6042, 0.6266, 0.6520, 0.6740, 0.6877, 0.6930, 0.6882,
     +0.6691, 0.6501, 0.6281, 0.6007, 0.5680, 0.5426, 0.5236, 0.5128,
     +0.5114, 0.5216, 0.5353, 0.5563, 0.5822, 0.6007, 0.6325, 0.6535/




      DATA XX5/ 0.5719, 0.5744, 0.5792, 0.5866, 0.5939, 0.6066, 0.7238,
     +0.7350, 0.7448, 0.7521, 0.7560, 0.7551, 0.7551, 0.7526, 0.7453,
     +0.7155, 0.7048, 0.6930, 0.6867, 0.6833, 0.6842, 0.6808, 0.6769,
     +0.6681, 0.6501, 0.6452, 0.6432, 0.6418, 0.6403, 0.6354, 0.5841,
     +0.5773, 0.5719, 0.5714, 0.5714/




      DATA YY5/ 0.3873, 0.3966, 0.4063, 0.4122, 0.4176, 0.4210, 0.4181,
     +0.4156, 0.4093, 0.4000, 0.3893, 0.3463, 0.1621, 0.1543, 0.1465,
     +0.1284, 0.1265, 0.1314, 0.1382, 0.1470, 0.1807, 0.1900, 0.1939,
     +0.1949, 0.1915, 0.1880, 0.1744, 0.1109, 0.1035, 0.0982, 0.0840,
     +0.0879, 0.0957, 0.1031, 0.3849/




      DATA XX6/ 0.6325, 0.6315, 0.6339, 0.6388, 0.6452, 0.6579, 0.6691,
     +0.6764, 0.6847, 0.6891, 0.6916, 0.6901, 0.6867, 0.6769, 0.6676,
     +0.6525, 0.6427, 0.6374, 0.6335, 0.6325/



      DATA YY6/ 0.3145, 0.3243, 0.3404, 0.3497, 0.3560, 0.3570, 0.3512,
     +0.3448, 0.3331, 0.3219, 0.3009, 0.2842, 0.2740, 0.2662, 0.2637,
     +0.2701, 0.2799, 0.2916, 0.3043, 0.3370/



      DATA XX7/ 0.7707, 0.7780, 0.7853, 0.7937, 0.8063, 0.8142, 0.8264,
     +0.8371, 0.8415, 0.8440, 0.8435, 0.8396, 0.8357, 0.8317, 0.8288,
     +0.8269, 0.8278, 0.8391, 0.8527, 0.8635, 0.8669, 0.8708, 0.8728,
     +0.8694, 0.8615, 0.8527, 0.8454, 0.8293, 0.8200, 0.8063, 0.7834,
     +0.7707, 0.7683, 0.7707, 0.7707/




      DATA YY7/ 0.6335, 0.6466, 0.6505, 0.6525, 0.6481, 0.6408, 0.6242,
     +0.6056, 0.5910, 0.5734, 0.5509, 0.5192, 0.4845, 0.4420, 0.4137,
     +0.3976, 0.3795, 0.3722, 0.3702, 0.3668, 0.3624, 0.3560, 0.3458,
     +0.3292, 0.3092, 0.2872, 0.2711, 0.2510, 0.2427, 0.2418, 0.2491,
     +0.2672, 0.2730, 0.3687, 0.6335/




      DATA XX8/ 0.1431, 0.1656, 0.2178, 0.2701, 0.3487, 0.4313, 0.5241,
     +0.6071, 0.6955, 0.7849, 0.8479, 0.9045, 0.9197, 0.9050, 0.8527,
     +0.7971, 0.7150, 0.6442, 0.5631, 0.4938, 0.4186, 0.3565, 0.3067,
     +0.2574, 0.2237, 0.1905, 0.1631, 0.1451, 0.1372, 0.1431/




      DATA YY8/ 0.3624, 0.2823, 0.1880, 0.1338, 0.0869, 0.0537, 0.0425,
     +0.0508, 0.0767, 0.1338, 0.2017, 0.3184, 0.4278, 0.5309, 0.6398,
     +0.7082, 0.7692, 0.8024, 0.8195, 0.8205, 0.8054, 0.7795, 0.7556,
     +0.7111, 0.6755, 0.6247, 0.5651, 0.4967, 0.4137, 0.3624/




*
      COLOUR=.TRUE.
      HSIZE=ABS(HHSIZE)
      DO 10 I=1,II8
         X(I)=(XX8(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY8(I)-.4325)*HSIZE/.40+YCENT
   10 CONTINUE
      CALL PSLWSC(3.)
      CALL PSLN(1)
      IF(COLOUR) THEN
         CALL GRSFAT('LogoBackground ',IERROR)
         CALL PFA(II8,X,Y)
      ENDIF
      CALL PPL(II8,X,Y)
      CALL PSLWSC(1.)
      DO 20 I=1,II1
         X(I)=(XX1(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY1(I)-.4325)*HSIZE/.40+YCENT
   20 CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('Logo_O         ',IERROR)
         CALL PFA(II1,X,Y)
      ENDIF
      CALL PPL(II1,X,Y)
      DO 30 I=1,II2
         X(I)=(XX2(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY2(I)-.4325)*HSIZE/.40+YCENT
   30 CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('LogoBackground ',IERROR)
         CALL PFA(II2,X,Y)
      ENDIF
      CALL PPL(II2,X,Y)
      DO 40 I=1,II3
         X(I)=(XX3(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY3(I)-.4325)*HSIZE/.40+YCENT
   40 CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('Logo_P         ',IERROR)
         CALL PFA(II3,X,Y)
      ENDIF
      CALL PPL(II3,X,Y)
      DO 50 I=1,II4
         X(I)=(XX4(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY4(I)-.4325)*HSIZE/.40+YCENT
   50 CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('LogoBackground ',IERROR)
         CALL PFA(II4,X,Y)
      ENDIF
      CALL PPL(II4,X,Y)
      DO 60 I=1,II5
         X(I)=(XX5(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY5(I)-.4325)*HSIZE/.40+YCENT
   60 CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('Logo_A         ',IERROR)
         CALL PFA(II5,X,Y)
      ENDIF
      CALL PPL(II5,X,Y)
      DO 70 I=1,II6
         X(I)=(XX6(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY6(I)-.4325)*HSIZE/.40+YCENT
 70   CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('LogoBackground ',IERROR)
         CALL PFA(II6,X,Y)
      ENDIF
      CALL PPL(II6,X,Y)
      DO 80 I=1,II7
         X(I)=(XX7(I)-.5304)*HSIZE/.40+XCENT
         Y(I)=(YY7(I)-.4325)*HSIZE/.40+YCENT
 80   CONTINUE
      IF(COLOUR) THEN
         CALL GRSFAT('Logo_L         ',IERROR)
         CALL PFA(II7,X,Y)
      ENDIF
      CALL PPL(II7,X,Y)
      END

      PROGRAM OPALLOGO

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
      CALL GRLOGO(0.5, 0.5, 0.3)
C
C     Close structure
      CALL PCLST

C      Post structure to workstation
      CALL PPOST(IWK, 0, 0.0)
      READ(*,*)
      CALL PCLWK(IWK)
      CALL PCLPH()      
      STOP
      END
