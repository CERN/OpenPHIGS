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
 * \file pset_light_src_rep.c
 * \brief Set light source for workstation
 */
void pset_light_src_rep(
                        Pint ws_id,
                        Pint light_src_ind,
                        Plight_src_bundle *light_src_rep
                        )
{
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Phg_args_rep_data rep;
  Ws_handle wsh;

  wsinfo = phg_ws_open(ws_id, Pfn_set_light_src_rep);
  if (wsinfo != NULL) {
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
      ERR_REPORT(PHG_ERH, ERR59);
    }
    else if (light_src_ind < 1) {
      ERR_REPORT(PHG_ERH, ERR129);
    }
    else if ((light_src_rep->type == PLIGHT_SPOT) &&
             ((light_src_rep->rec.spot.angle < 0) ||
              (light_src_rep->rec.spot.angle > M_PI))) {
      ERR_REPORT(PHG_ERH, ERR132);
    }
    else {
      wsh = PHG_WSID(ws_id);
      rep.index = light_src_ind;
      memcpy(&rep.bundl.lightsrcrep,
             light_src_rep,
             sizeof(Plight_src_bundle));
      (*wsh->set_rep)(wsh, PHG_ARGS_LIGHTSRCREP, &rep);
    }
  }
}

