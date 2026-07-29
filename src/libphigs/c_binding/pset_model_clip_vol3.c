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

#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "private/cb_internal.h"

/**
 * \file pset_model_clip_vol3.c
 * \brief Creates a new element - set clipping volume 3
 */
void  pset_model_clip_vol3 (
                            Pint op,
                            Phalf_space_list3 spacelist
                            )
{
  Phg_args_add_el args;
  Pint * data;
  Phalf_space3 * selement;
  int i;
  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_model_clip_vol3)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else if ( op < 0 || op > 2) {
      /* the value of op is not important as this is anyway not used */
      ERR_REPORT(PHG_ERH, ERR112);
    }
    else {
      args.el_type = PELEM_MODEL_CLIP_VOL3;
      args.el_size = 2*sizeof(Pint)+spacelist.num_half_spaces*sizeof(Phalf_space3);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data =(Pint *) args.el_data;
        data[0] = op;
        data[1] = spacelist.num_half_spaces;
        selement = (Phalf_space3 *)&data[2];
        for (i=0; i<spacelist.num_half_spaces; i++){
          memcpy(&selement[i], &spacelist.half_spaces[i], sizeof(Phalf_space3));
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}


/***********
   Extensions outside the standards
 **********/

