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

/* Automatically generated prototypes */
extern void ptext(
           Ppoint *text_pos,
           char *char_string
           );
extern void ptext3(
            Ppoint3 *text_pos,
            Pvec3 plane[2],
            char *char_string
            );
extern void pset_text_font(
                    Pint font
                    );
extern void pset_text_prec(
                    Ptext_prec prec
                    );
extern void pset_text_path(
                    Ptext_path text_path
                    );
extern void pset_text_align(
                     Ptext_align *text_align
                     );
extern void pset_text_colr_ind(
                        Pint colr_ind
                        );
extern void pset_text_ind(
                   Pint text_ind
                   );
extern void panno_text_rel(
                    Ppoint *ref_point,
                    Pvec *offset,
                    char *text);
extern void panno_text_rel3(
                     Ppoint3 *ref_point,
                     Pvec3 *offset,
                     char *text);
extern void pset_anno_char_ht(
                       Pfloat height
                       );
extern void pset_anno_align(
                     Ptext_align *text_align
                     );
extern void pset_anno_path(
                    Ptext_path text_path
                    );


/**
 * \file panno_text_rel.c
 * \brief Creates a new element - annotation text relative
 */
void panno_text_rel(
                    Ppoint *ref_point,
                    Pvec *offset,
                    char *text){
  
  Phg_args_add_el args;
  char * data;
  
  if (phg_entry_check(PHG_ERH, ERR5, Pfn_anno_text_rel)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_ANNO_TEXT_REL;
      args.el_size = sizeof(Ppoint)+sizeof(Pvec)+strlen(text) + 1;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        Ppoint * refp = (Ppoint *) args.el_data;
        memcpy(refp, ref_point, sizeof(Ppoint));
        Pvec * offs = (Pvec *) &refp[1];
        memcpy(offs, offset, sizeof(Pvec));
        strcpy((char*)&offs[1], text);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

