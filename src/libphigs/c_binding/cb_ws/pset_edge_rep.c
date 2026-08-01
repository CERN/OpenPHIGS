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
 * \file pset_edge_rep.c
 * \brief Set workstation edge represenation
 */
void pset_edge_rep(
                   Pint ws_id,
                   Pint edge_ind,
                   Pedge_bundle *edge_bundle
                   )
{
  Ws *wsh;
  Phg_args_rep_data rep;
  Wst_phigs_dt *dt;

  dt = phg_wst_check_set_rep(Pfn_set_edge_rep,
                             ws_id,
                             edge_ind,
                             edge_bundle->colr_ind);
  if (dt != NULL) {
    wsh = PHG_WSID(ws_id);
    rep.index = edge_ind;
    memcpy(&rep.bundl.edgerep, edge_bundle, sizeof(Pedge_bundle));
    (*wsh->set_rep)(wsh, PHG_ARGS_EDGEREP, &rep);
  }
}

