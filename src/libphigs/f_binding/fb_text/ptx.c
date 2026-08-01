/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022-2023 CERN
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
#include "util/ftn.h"

/**
 * \file ptx.c
 * \brief Text
 */
FTN_SUBROUTINE(ptx)(
                    FTN_REAL(px),
                    FTN_REAL(py),
                    FTN_CHARACTER(chars)
                    )
{
  Phg_args_add_el args;
  Pint len;
  Ppoint text_pos;
  Ppoint *data;
  char *char_string;
#ifdef DEBUG
  printf("DEBUG: text\n");
#endif

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_text)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      len = FTN_CHARACTER_LEN(chars);
      args.el_type = PELEM_TEXT;
      args.el_size = sizeof(Ppoint) + len + 1;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Ppoint *) args.el_data;
        text_pos.x = FTN_REAL_GET(px);
        text_pos.y = FTN_REAL_GET(py);
        memcpy(data, &text_pos, sizeof(Ppoint));
        char_string = (char *) &data[1];
        strncpy(char_string, FTN_CHARACTER_GET(chars), len);
        char_string[len] = '\0';
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

