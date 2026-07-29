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
#include <string.h>
#include <math.h>
#include <png.h>
#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif
#include <gl2ps.h>

#include "phg.h"
#include "css.h"
#include "ws.h"
#include "private/phgP.h"
#include "private/cb_internal.h"
#include "private/cbP.h"
#include "private/wsglP.h"
#include "private/wsxP.h"
#include "phconf.h"

short int wsgl_use_shaders_settings;

/**
 * \file pinq_open_wss.c
 * \brief Get list of open workstations
 */
void pinq_open_wss(
                   Pint num_elems_appl_list,
                   Pint start_ind,
                   Pint *err_ind,
                   Pint_list *open_ws_ids,
                   Pint *num_elems_impl_list
                   )
{
  Pint ws_ids[MAX_NO_OPEN_WS];
  Pint n;

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else {
    *err_ind = 0;
    n = phg_psl_inq_wsids(PHG_PSL, ws_ids);
    open_ws_ids->num_ints = 0;
    *num_elems_impl_list = n;
    if (n > 0) {
      if (start_ind < 0 || start_ind >= n) {
        *err_ind = ERR2201;
      }
      else if (num_elems_appl_list > 0) {
        open_ws_ids->num_ints = PHG_MIN(num_elems_appl_list, n - start_ind);
        memcpy(open_ws_ids->ints, &ws_ids[start_ind],
               open_ws_ids->num_ints * sizeof(Pint));
      }
      else if (num_elems_appl_list < 0) {
        *err_ind = ERRN153;
      }
    }
  }
}

