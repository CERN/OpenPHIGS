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
 * \file pset_light_src_state.c
 * \brief Creates a new element - Set light source state
 */
void pset_light_src_state(
                          Pint_list *activation,
                          Pint_list *deactivation
                          )
{
  Phg_args_add_el args;
  Pint n1, n2;
  Pint *data;

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_light_src_state)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_LIGHT_SRC_STATE;
      args.el_size = 2 * sizeof(Pint) +
        sizeof(Pint) * activation->num_ints +
        sizeof(Pint) * deactivation->num_ints;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        n1 = activation->num_ints;
        n2 = deactivation->num_ints;
        data[0] = n1;
        data = &data[1];
        memcpy(data, activation->ints, sizeof(Pint) * n1);
        data = &data[n1];
        data[0] = n2;
        data = &data[1];
        memcpy(data, deactivation->ints, sizeof(Pint) * n2);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

