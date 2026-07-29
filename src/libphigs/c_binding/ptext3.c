/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
*             (C) 2022-2023 CERN
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
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cb_internal.h"

/**
 * \file ptext3.c
 * \brief Creates a new element - Text
 */
void ptext3(
            Ppoint3 *text_pos,
            Pvec3 plane[2],
            char *char_string
            )
{
  Phg_args_add_el args;
  Ppoint3 *data;
  Pvec3 * data1;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_text)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_TEXT3;
      args.el_size = sizeof(Ppoint3) + +2*sizeof(Pvec3) + strlen(char_string) + 1;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Ppoint3 *) args.el_data;
        memcpy(data, text_pos, sizeof(Ppoint3));
        data1 = (Pvec3*) &data[1];
        memcpy(data1, &plane[0], 2*sizeof(Pvec3));
        strcpy((char *) &data1[2], char_string);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

