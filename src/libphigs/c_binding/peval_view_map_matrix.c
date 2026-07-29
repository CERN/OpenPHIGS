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
 * \file peval_view_map_matrix.c
 * \brief Generate view mapping matrix
 */
void peval_view_map_matrix(
                           Pview_map *map,            /* view mapping */
                           Pint *error_ind,           /* OUT error indicator */
                           Pmatrix m                  /* OUT view mapping matrix */
                           )
{
  /* 1. Translate window's lower-left-corner to 0,0.
   * 2. Scale size of window to size of viewport.
   * 3. Translate 0,0 to viewport's lower-left-corner.
   *
   * Matrices are:
   * 1:  1 0 -win->xmin   2:  scale.x 0       0   3:  1 0  vp->xmin
   *     0 1 -win->ymin        0      scale.y 0       0 1  vp->ymin
   *     0 0   1               0      0       1       0 0   1
   */

  float sx, sy;              /* scale factors: len(vp) / len(win) */
  Plimit *win = &map->win;
  Plimit *vp = &map->proj_vp;

  ERR_SET_CUR_FUNC(PHG_ERH, Pfn_INQUIRY);

  if (PSL_SYS_STATE(PHG_PSL) != PSYS_ST_PHOP) {
    *error_ind = ERR2;
  } else if ( !(win->x_min < win->x_max) || !(win->y_min < win->y_max)) {
    *error_ind = ERR151;
  } else if ( !(vp->x_min < vp->x_max) || !(vp->y_min < vp->y_max)) {
    *error_ind = ERR152;
  } else if ( !PHG_IN_RANGE( PDT_NPC_XMIN, PDT_NPC_XMAX, vp->x_min)
              || !PHG_IN_RANGE( PDT_NPC_XMIN, PDT_NPC_XMAX, vp->x_max)
              || !PHG_IN_RANGE( PDT_NPC_YMIN, PDT_NPC_YMAX, vp->y_min)
              || !PHG_IN_RANGE( PDT_NPC_YMIN, PDT_NPC_YMAX, vp->y_max)) {
    *error_ind = ERR155;
  } else {
    *error_ind = 0;
    sx = (vp->x_max - vp->x_min) / (win->x_max - win->x_min);
    sy = (vp->y_max - vp->y_min) / (win->y_max - win->y_min);
    m[0][0] = sx;
    m[0][1] = 0.0;
    m[0][2] = sx * (-win->x_min) + vp->x_min;

    m[1][0] = 0.0;
    m[1][1] = sy;
    m[1][2] = sy * (-win->y_min) + vp->y_min;

    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;
  }
}
