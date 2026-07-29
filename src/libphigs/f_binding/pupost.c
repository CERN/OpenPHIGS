/******************************************************************************
 *   Do NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
 *
 *   This file is part of Open PHIGS
 *   Copyright (C) 2014 Surplus Users Ham Society
 *
 *   Open PHIGS is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   Open PHIGS is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 * Changes:   Copyright (C) 2022-2023 CERN
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gl2ps.h>
#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern short int wsgl_use_shaders_settings;
extern int record_geom;

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popwk)(;
extern FTN_SUBROUTINE(ppost)(;
extern FTN_SUBROUTINE(pscm)(;
extern FTN_SUBROUTINE(pscr)(;
extern FTN_SUBROUTINE(pupost)(;
extern FTN_SUBROUTINE(pupast)(;
extern FTN_SUBROUTINE(prst)(;
extern FTN_SUBROUTINE(pswkw)(;
extern FTN_SUBROUTINE(pswkw3)(;
extern FTN_SUBROUTINE(pclwk)(;
extern FTN_SUBROUTINE(psvwr3)(;
extern FTN_SUBROUTINE(pqhrm)(;
extern FTN_SUBROUTINE(pqvwr)(;
extern FTN_SUBROUTINE(pqwkt)(;
extern FTN_SUBROUTINE(pqwkt3)(;
extern FTN_SUBROUTINE(pqpost)(;
extern FTN_SUBROUTINE(pqwkpo)(;
extern FTN_SUBROUTINE(puwk)(;
extern FTN_SUBROUTINE(pspmr)(;
extern FTN_SUBROUTINE(psplr)(;
extern FTN_SUBROUTINE(psedr)(;
extern FTN_SUBROUTINE(psir)(;
extern FTN_SUBROUTINE(pstxr)(;
extern FTN_SUBROUTINE(pswkv3)(;
extern FTN_SUBROUTINE(psdus)(;
extern FTN_SUBROUTINE(pqcr)(;
extern FTN_SUBROUTINE(pqcf)(;
extern FTN_SUBROUTINE(pmsg)(;
extern FTN_SUBROUTINE(pslsr)(;
extern FTN_SUBROUTINE(pxscm)(;
extern FTN_SUBROUTINE(psfname)(;


/**
 * \file pupost.c
 * \brief Unpost structure
 */

FTN_SUBROUTINE(pupost)(
                       FTN_INTEGER(wkid),
                       FTN_INTEGER(strid)
                       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint struct_id = FTN_INTEGER_GET(strid);
#ifdef DEBUG
  printf("DEBUG: PUPOST Unpost structure %d\n", *strid);
#endif
  punpost_struct(ws_id, struct_id);
}

