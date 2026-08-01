/******************************************************************************

Copyright (c) 1989, 1990, 1991  X Consortium
Copyright (c) 2014 Surplus Users Ham Society

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright 1989, 1990, 1991 by Sun Microsystems, Inc.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Sun Microsystems,
not be used in advertising or publicity pertaining to distribution of
the software without specific, written prior permission.

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "phg.h"
#include "util.h"

/**
 * \file peval_view_map_matrix3.c
 * \brief Generate 3D view mapping matrix
 */
void peval_view_map_matrix3(
                            Pview_map3 *map,           /* view mapping */
                            Pint *error_ind,           /* OUT error indicator */
                            Pmatrix3 m                 /* OUT view mapping matrix */
    )
{
  /* Procedure:
      (Perspective):
   - Translate to PRP,      Tc
   - Convert to left handed coords,   Tlr
   - Shear,         H
   - Scale to canonical view volume,  S
   - Normalize perspective view volume,   Ntp
   - Scale to viewport,       Svp
   - Convert to right handed coords,  Tlr
   - Translate to viewport,     Tvp

      (Parallel):
   - Translate to view plane,     Tc
   - Shear about the view plane,    H
   - Translate back,      Tc inverse
   - Translate window to origin,    Tl
   - Scale to canonical view volume,  S
   - Scale to viewport,       Svp
   - Translate to viewport,     Tvp

   See pevalviewmappingmatrix3_debug for the matrices.
  */

  Pfloat *r;
  Ppoint3 *prp = &map->proj_ref_point;
  Plimit3 *vp = &map->proj_vp;
  Plimit3 *win = &map->win;

  /* These are ordered roughly by the number of times used, the most
   * used is first.  Those used twice or less aren't declared register.
   */
  double sz, sx, sy;
  double zf;
  double dx = vp->x_max - vp->x_min;
  double dy = vp->y_max - vp->y_min;
  double hx, hy;
  double d;
  double dz = vp->z_max - vp->z_min;
  double vvz = map->front_plane - map->back_plane;

  ERR_SET_CUR_FUNC(PHG_ERH, Pfn_INQUIRY);

  if (PSL_SYS_STATE(PHG_PSL) != PSYS_ST_PHOP) {
    *error_ind = ERR2;
  } else if ( !(win->x_min < win->x_max) || !(win->y_min < win->y_max)) {
    *error_ind = ERR151;
  } else if ( !(vp->x_min < vp->x_max) || !(vp->y_min < vp->y_max)
              || !(vp->z_min <= vp->z_max) ) {
    *error_ind = ERR152;
  } else if ( PHG_NEAR_ZERO( vvz) && vp->z_min != vp->z_max) {
    *error_ind = ERR158;
  } else if ( map->proj_type == PTYPE_PERSPECT
              && prp->z < map->front_plane && prp->z > map->back_plane ) {
    *error_ind = ERR162;
  } else if ( prp->z == map->view_plane) {
    *error_ind = ERR163;
  } else if ( map->front_plane < map->back_plane) {
    *error_ind = ERR164;
  } else if ( !PHG_IN_RANGE( PDT_NPC_XMIN, PDT_NPC_XMAX, vp->x_min)
              || !PHG_IN_RANGE( PDT_NPC_XMIN, PDT_NPC_XMAX, vp->x_max)
              || !PHG_IN_RANGE( PDT_NPC_YMIN, PDT_NPC_YMAX, vp->y_min)
              || !PHG_IN_RANGE( PDT_NPC_YMIN, PDT_NPC_YMAX, vp->y_max)
              || !PHG_IN_RANGE( PDT_NPC_ZMIN, PDT_NPC_ZMAX, vp->z_min)
              || !PHG_IN_RANGE( PDT_NPC_ZMIN, PDT_NPC_ZMAX, vp->z_max) ) {
    *error_ind = ERR155;
  } else if ( map->proj_type == PTYPE_PERSPECT) {
    *error_ind = 0;
    d = prp->z - map->view_plane;
    sz = 1.0 / (prp->z - map->back_plane);
    sx = sz * d * 2.0 / (win->x_max - win->x_min);
    sy = sz * d * 2.0 / (win->y_max - win->y_min);
    hx = (prp->x - 0.5 * (win->x_min + win->x_max)) / d;
    hy = (prp->y - 0.5 * (win->y_min + win->y_max)) / d;

    r = m[0];
    r[0] = 0.5 * dx * sx;
    r[1] = 0.0;
    r[2] = -(0.5 * dx * (sx * hx + sz) + sz * vp->x_min);
    r[3] = -(0.5 * dx * sx * (prp->x - hx * prp->z)
             - sz * prp->z * (0.5 * dx + vp->x_min));

    r = m[1];
    r[0] = 0.0;
    r[1] = 0.5 * dy * sy;
    r[2] = -(0.5 * dy * (sy * hy + sz) + sz * vp->y_min);
    r[3] = -(0.5 * dy * sy * (prp->y - hy * prp->z)
             - sz * prp->z * (0.5 * dy + vp->y_min));

    r = m[2];
    r[0] = r[1] = 0.0;
    zf = (prp->z - map->front_plane) / (prp->z - map->back_plane);
    if ( PHG_NEAR_ZERO( 1.0 - zf)) {
      r[2] = 0.0;
      r[3] = sz * prp->z * vp->z_max;
    } else {
      r[2] = sz * ((dz / (1.0 - zf)) - vp->z_max);
      r[3] = sz * prp->z * vp->z_max - (dz/(1.0-zf)) * (sz * prp->z - zf);
    }

    r = m[3];
    r[0] = r[1] = 0.0;
    r[2] = -sz;
    r[3] = sz * prp->z;
  } else {  /* parallel */
    *error_ind = 0;
    sx = dx / (win->x_max - win->x_min);
    sy = dy / (win->y_max - win->y_min);
    hx = (prp->x - 0.5 * (win->x_min + win->x_max))
      / (map->view_plane - prp->z);
    hy = (prp->y - 0.5 * (win->y_min + win->y_max))
      / (map->view_plane - prp->z);

    r = m[0];
    r[0] = sx;
    r[1] = 0.0;
    r[2] = sx * hx;
    r[3] = vp->x_min - sx * (hx * map->view_plane + win->x_min);

    r = m[1];
    r[0] = 0.0;
    r[1] = sy;
    r[2] = sy * hy;
    r[3] = vp->y_min - sy * (hy * map->view_plane + win->y_min);

    r  = m[2];
    r[0] = r[1] = 0.0;
    if ( PHG_NEAR_ZERO(vvz)) {
      r[2] = 0.0;
    }
    else {
      r[2] = dz / vvz;
    }
    r[3] = vp->z_min - r[2] * map->back_plane;

    r = m[3];
    r[0] = r[1] = r[2] = 0.0;
    r[3] = 1.0;
  }
}

