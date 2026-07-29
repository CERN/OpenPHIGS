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
 * \file inq_filter.c
 * \brief Get workstation filter
 */
void inq_filter(
                       Phg_args_flt_type type,
                       Pint ws_id,
                       struct _Pstore *store,
                       Pint *err_ind,
                       Pfilter **filter
                       )
{
  Phg_ret ret;
  Phg_ret_filter *pf = &ret.data.filter;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  int size;
  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR3;
  }
  else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    *err_ind = ERR3;
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      *err_ind = ERR54;
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OBJ ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_filter)(wsh, type, &ret);
        if (ret.err) {
          *err_ind = ret.err;
        }
        else {
          *err_ind = 0;
          size = (pf->incl.num_ints + pf->excl.num_ints) * sizeof(Pint);
          if (phg_cb_resize_store(store, size, err_ind)) {
            *filter = &store->data.filter;
            (*filter)->incl_set.num_ints = pf->incl.num_ints;
            (*filter)->excl_set.num_ints = pf->excl.num_ints;
            (*filter)->incl_set.ints = (Pint *) store->buf;
            (*filter)->excl_set.ints =
              &(*filter)->incl_set.ints[(*filter)->incl_set.num_ints];
            if (pf->incl.num_ints > 0) {
              memcpy((*filter)->incl_set.ints, pf->incl.ints,
                     (*filter)->incl_set.num_ints * sizeof(Pint));
            }
            if (pf->excl.num_ints > 0) {
              memcpy((*filter)->excl_set.ints, pf->excl.ints,
                     (*filter)->excl_set.num_ints * sizeof(Pint));
            }
          }
        }
      }
    }
  }
}

