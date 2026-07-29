/******************************************************************************
 *   Do NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
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

#include <gl2ps.h>
#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern short int wsgl_use_shaders_settings;
extern int record_geom;

/* Automatically generated prototypes */
extern FTN_SUBROUTINE(popwk)(;
extern FTN_SUBROUTINE(ppost)(;
extern FTN_SUBROUTINE(pscm)(;
extern FTN_SUBROUTINE(pscr)(;
extern FTN_SUBROUTINE(pupost)(;
extern FTN_SUBROUTINE(pupast)(;
extern FTN_SUBROUTINE(prst)(;
extern FTN_SUBROUTINE(pswkw)(;
extern FTN_SUBROUTINE(pswkw3)(;
extern FTN_SUBROUTINE(pclwk)(;
extern FTN_SUBROUTINE(psvwr3)(;
extern FTN_SUBROUTINE(pqhrm)(;
extern FTN_SUBROUTINE(pqvwr)(;
extern FTN_SUBROUTINE(pqwkt)(;
extern FTN_SUBROUTINE(pqwkt3)(;
extern FTN_SUBROUTINE(pqpost)(;
extern FTN_SUBROUTINE(pqwkpo)(;
extern FTN_SUBROUTINE(puwk)(;
extern FTN_SUBROUTINE(pspmr)(;
extern FTN_SUBROUTINE(psplr)(;
extern FTN_SUBROUTINE(psedr)(;
extern FTN_SUBROUTINE(psir)(;
extern FTN_SUBROUTINE(pstxr)(;
extern FTN_SUBROUTINE(pswkv3)(;
extern FTN_SUBROUTINE(psdus)(;
extern FTN_SUBROUTINE(pqcr)(;
extern FTN_SUBROUTINE(pqcf)(;
extern FTN_SUBROUTINE(pmsg)(;
extern FTN_SUBROUTINE(pslsr)(;
extern FTN_SUBROUTINE(pxscm)(;
extern FTN_SUBROUTINE(psfname)(;


/**
 * \file pslsr.c
 * \brief Set light source representation
 *
 * \return N/A NOTES:       Not part of the standard
 */
FTN_SUBROUTINE(pslsr)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(lsi),
                      FTN_INTEGER(lstyp),
                      FTN_INTEGER(ldr),
                      char* data
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint light_src_ind = FTN_INTEGER_GET(lsi);
  Pint type = FTN_INTEGER_GET(lstyp);
  Pint dbytes = FTN_INTEGER_GET(ldr);
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Phg_args_rep_data rep;
  Ws_handle wsh;
  int *ihere;
  float *fhere;
  int col_indx;
  int ncc;
  Pamb_light_src_rec amblight;
  Pdir_light_src_rec dirlight;
  Ppos_light_src_rec poslight;

  Plight_src_bundle light_src_rep;
#ifdef DEBUG
  printf("DEBUG: PSLSR set light source representation\n");
#endif
  ihere = (int*) data;
  /* first value is the number of integers */
  ihere = &ihere[1];
  int col_type = ihere[0];
  if (col_type == PINDIRECT){
    col_indx = ihere[1];
  } else {
    ncc = ihere[1];
  }
  /* number of ints plus 2 ints plus number of floats */
  fhere = (float*)(data + 4*sizeof(int));
  switch (type) {
  case PLIGHT_AMBIENT:
    amblight.colr.type = col_type;
    switch (col_type){
    case PINDIRECT:
      amblight.colr.val.ind = col_indx;
      break;
    case PMODEL_RGB:
      memcpy(&amblight.colr.val.general.x, &fhere[0], 3*sizeof(Pfloat));
      amblight.colr.val.general.a = 1.0;
      break;
    case PMODEL_RGBA:
      memcpy(&amblight.colr.val.general.x, &fhere[0], 4*sizeof(Pfloat));
      break;
    }
    light_src_rep.type = type;
    light_src_rep.rec.ambient = amblight;
    pset_light_src_rep(ws_id, light_src_ind, & light_src_rep);
    break;
  case PLIGHT_DIRECTIONAL:
    dirlight.colr.type = col_type;
    memcpy(&dirlight.dir.delta_x, &fhere[0], 3*sizeof(Pfloat));
    switch (col_type){
    case PINDIRECT:
      dirlight.colr.val.ind = col_indx;
      break;
    case PMODEL_RGB:
      memcpy(&dirlight.colr.val.general.x, &fhere[3], 3*sizeof(Pfloat));
      dirlight.colr.val.general.a = 1.0;
      break;
    case PMODEL_RGBA:
      memcpy(&dirlight.colr.val.general.x, &fhere[3], 4*sizeof(Pfloat));
      break;
    }
    light_src_rep.type = type;
    light_src_rep.rec.directional = dirlight;
    pset_light_src_rep(ws_id, light_src_ind, & light_src_rep);
    break;
  case PLIGHT_POSITIONAL:
    poslight.colr.type = col_type;
    memcpy(&poslight.pos.x, &fhere[0], 3*sizeof(Pfloat));
    memcpy(&poslight.coef, &fhere[3], 2*sizeof(Pfloat));
    switch (col_type){
    case PINDIRECT:
      poslight.colr.val.ind = col_indx;
      break;
    case PMODEL_RGB:
      memcpy(&poslight.colr.val.general.x, &fhere[5], 3*sizeof(Pfloat));
      poslight.colr.val.general.a = 1.0;
      break;
    case PMODEL_RGBA:
      memcpy(&poslight.colr.val.general.x, &fhere[5], 4*sizeof(Pfloat));
      break;
    }
    light_src_rep.type = type;
    light_src_rep.rec.positional = poslight;
    pset_light_src_rep(ws_id, light_src_ind, & light_src_rep);
    break;
  default:
    printf("ERROR in pslsr: light type %d not yet implemented. Ignorning function.\n", type);
    break;
  }
}

