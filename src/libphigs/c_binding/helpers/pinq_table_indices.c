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
 * \file pinq_table_indices.c
 * \brief Get table indices from workstation helper function
 */
void pinq_table_indices(
                               Phg_args_rep_type type,
                               Pint ws_id,
                               Pint num_elems_appl_list,
                               Pint start_ind,
                               Pint *err_ind,
                               Pint_list *def_line_ind,
                               Pint *num_elems_impl_list
                               )
{
  Ws_handle wsh;
  Phg_ret ret;

  wsh = PHG_WSID(ws_id);
  if (type == PHG_ARGS_VIEWREP) {
    (*wsh->inq_view_indices)(wsh, &ret);
  }
  else {
    (*wsh->inq_bundle_indices)(wsh, type, &ret);
  }

  if (ret.err) {
    *err_ind = ret.err;
  }
  else {
    *err_ind = 0;
    *num_elems_impl_list = ret.data.int_list.num_ints;
    if (ret.data.int_list.num_ints > 0) {
      if (start_ind < 0 || start_ind >= ret.data.int_list.num_ints) {
        *err_ind = ERR2201;
      }
      else if (num_elems_appl_list > 0) {
        def_line_ind->num_ints =
          PHG_MIN(num_elems_appl_list,
                  ret.data.int_list.num_ints - start_ind);
        memcpy (def_line_ind->ints,
                &ret.data.int_list.ints[start_ind],
                def_line_ind->num_ints * sizeof(Pint));
      }
      else if (num_elems_appl_list < 0) {
        *err_ind = ERRN153;
      }
    }
  }
}

