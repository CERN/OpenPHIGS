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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(ppl)(;
extern FTN_SUBROUTINE(ppl3)(;
extern FTN_SUBROUTINE(psplci)(;
extern FTN_SUBROUTINE(psln)(;
extern FTN_SUBROUTINE(pslwsc)(;
extern FTN_SUBROUTINE(pschh)(;
extern FTN_SUBROUTINE(pads)(;
extern FTN_SUBROUTINE(psvwi)(;
extern FTN_SUBROUTINE(psici)(;
extern FTN_SUBROUTINE(psis)(;
extern FTN_SUBROUTINE(pschup)(;
extern FTN_SUBROUTINE(psatcu)(;
extern FTN_SUBROUTINE(pfa)(;
extern FTN_SUBROUTINE(pfa3)(;
extern FTN_SUBROUTINE(plb)(;
extern FTN_SUBROUTINE(psedfg)(;
extern FTN_SUBROUTINE(ppm)(;
extern FTN_SUBROUTINE(ppm3)(;
extern FTN_SUBROUTINE(psewsc)(;
extern FTN_SUBROUTINE(psmksc)(;
extern FTN_SUBROUTINE(pspmi)(;
extern FTN_SUBROUTINE(pspmci)(;
extern FTN_SUBROUTINE(psmk)(;
extern FTN_SUBROUTINE(pexst)(;
extern FTN_SUBROUTINE(psgmt3)(;
extern FTN_SUBROUTINE(pshrm)(;
extern FTN_SUBROUTINE(pshrid)(;
extern FTN_SUBROUTINE(pspkid)(;
extern FTN_SUBROUTINE(pschsp)(;
extern FTN_SUBROUTINE(pschxp)(;
extern FTN_SUBROUTINE(psedi)(;
extern FTN_SUBROUTINE(psiasf)(;
extern FTN_SUBROUTINE(psedci)(;
extern FTN_SUBROUTINE(psedt)(;


/**
 * \file pschh.c
 * \brief Creates a new element - Character height Attribute
 */

FTN_SUBROUTINE(pschh)(
                      FTN_REAL(chh)
                      )
{
  Phg_args_add_el args;
  Pfloat char_ht;
#ifdef DEBUG
  printf("DEBUG: PSCHH character height\n");
#endif

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_ht)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_CHAR_HT;
      args.el_size = sizeof(Pfloat);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        char_ht = FTN_REAL_GET(chh);
        memcpy(args.el_data, &char_ht, args.el_size);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

