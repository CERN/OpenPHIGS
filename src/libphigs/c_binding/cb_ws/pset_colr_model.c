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

/**
 * \file pset_colr_model.c
 * \brief Set workstation colour model
 */
void pset_colr_model(
                     Pint ws_id,
                     Pint model
                     )
{
  Ws *wsh;
  Pint original_model;
  wsh = PHG_WSID(ws_id);
  original_model = wsh->current_colour_model;
  switch (model){
  case PINDIRECT:
    wsh->current_colour_model = PINDIRECT;
    break;
  case PMODEL_RGB:
    wsh->current_colour_model = PMODEL_RGB;
    break;
  case PMODEL_RGBA:
    wsh->current_colour_model = PMODEL_RGBA;
    break;
  default:
    wsh->current_colour_model = wsh->type->desc_tbl.phigs_dt.out_dt.default_colour_model;
    printf("WARNING: pset_colr_model: Unknown color model, using default\n");
    break;
  }
  /* if the model has changed we should update the background */
  if (original_model != model){
    if (wsh->current_colour_model == PMODEL_RGBA){
      pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgba));
    } else {
      pset_colr_rep(ws_id, 0, &(config[ws_id].background_color_rgb));
    }
  }

}

