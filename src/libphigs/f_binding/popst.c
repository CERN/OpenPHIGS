/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
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

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cbP.h"
#include "util/ftn.h"

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popst)(;
extern FTN_SUBROUTINE(pclst)(;
extern FTN_SUBROUTINE(pdel)(void);
extern FTN_SUBROUTINE(pdst)(;
extern FTN_SUBROUTINE(posep)(;
extern FTN_SUBROUTINE(psep)(;
extern FTN_SUBROUTINE(pseplb)(;
extern FTN_SUBROUTINE(psedm)(;
extern FTN_SUBROUTINE(pqedm)(;
extern FTN_SUBROUTINE(pels)(;
extern FTN_SUBROUTINE(pqstst)(;
extern FTN_SUBROUTINE(pqeco)(;
extern FTN_SUBROUTINE(dumpstr)(FTN_INTEGER(strid));
extern FTN_SUBROUTINE(pcelst)(FTN_INTEGER(strid));
extern FTN_SUBROUTINE(pdas)();
extern FTN_SUBROUTINE(pqstrs)(Pint* strsta);


/**
 * \file popst.c
 * \brief Opens a structure for appending or editing.
 */
FTN_SUBROUTINE(popst)(
                      FTN_INTEGER(strid)
                      )
{
  Pint struct_id = FTN_INTEGER_GET(strid);

  if (phg_entry_check(PHG_ERH, ERR6, Pfn_open_struct)) {
    if (PSL_STRUCT_STATE(PHG_PSL) == PSTRUCT_ST_STCL) {
      if (phg_css_open_struct(PHG_CSS, struct_id) != NULL) {
        PSL_STRUCT_STATE(PHG_PSL) = PSTRUCT_ST_STOP;
        PSL_OPEN_STRUCT(PHG_PSL) = struct_id;
      }
      ERR_FLUSH(PHG_ERH);
    }
    else {
      ERR_REPORT(PHG_ERH, ERR6);
    }
  }
}

