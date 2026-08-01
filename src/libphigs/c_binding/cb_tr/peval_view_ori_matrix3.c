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
 * \file peval_view_ori_matrix3.c
 * \brief Generate 3D view orientation matrix
 */
void peval_view_ori_matrix3(
                            Ppoint3 *vrp,              /* view reference point */
                            Pvec3 *vpn,                /* view plane normal */
                            Pvec3 *vup,                /* view up vector */
                            Pint *error_ind,           /* OUT error indicator */
                            Pmatrix3 m                 /* OUT view orientation matrix */
                            )
{
  /*  Translate to VRP then change the basis.
     *  The old basis is: e1 = < 1, 0, 0>,  e2 = < 0, 1, 0>, e3 = < 0, 0, 1>.
     * The new basis is: ("x" means cross product)
    e3' = VPN / |VPN|
    e1' = VUP x VPN / |VUP x VPN|
    e2' = e3' x e1'
     * Therefore the transform from old to new is x' = ATx, where:

       | e1' 0 |         | 1 0 0 -vrp.x |
   A = |       |,    T = | 0 1 0 -vrp.y |
       | e2' 0 |         | 0 0 1 -vrp.z |
       |       |         | 0 0 0    1   |
       | e3' 0 |
       |       |
       | -0-  1|
     */

    /* These ei's are really ei primes. */
  Pfloat *e1 = m[0], *e3 = m[2], *e2 = m[1];
  double s, mag_vpn;

  ERR_SET_CUR_FUNC(PHG_ERH, Pfn_INQUIRY);

  if (PSL_SYS_STATE(PHG_PSL) != PSYS_ST_PHOP) {
    *error_ind = ERR2;
  } else if ( PHG_ZERO_MAG(mag_vpn = PHG_MAG_V3(vpn)) ) {
    *error_ind = ERR159;
  } else if ( PHG_ZERO_MAG(PHG_MAG_V3(vup)) ) {
    *error_ind = ERR160;
  } else {
    /* e1' = VUP x VPN / |VUP x VPN|, but do the division later. */
    e1[0] = vup->delta_y * vpn->delta_z - vup->delta_z * vpn->delta_y;
    e1[1] = vup->delta_z * vpn->delta_x - vup->delta_x * vpn->delta_z;
    e1[2] = vup->delta_x * vpn->delta_y - vup->delta_y * vpn->delta_x;
    s = sqrt(e1[0] * e1[0] + e1[1] * e1[1] + e1[2] * e1[2]);

    /* Check for vup and vpn colinear (zero dot product). */
    if ( PHG_ZERO_MAG(s) ) {
      *error_ind = ERR161;
    } else {
      *error_ind = 0;
      /* Normalize e1 */
      s = 1.0 / s;
      e1[0] *= s;
      e1[1] *= s;
      e1[2] *= s;

      /* e3 = VPN / |VPN| */
      s = 1.0 / mag_vpn;
      e3[0] = s * vpn->delta_x;
      e3[1] = s * vpn->delta_y;
      e3[2] = s * vpn->delta_z;

      /* e2 = e3 x e1 */
      e2[0] = e3[1] * e1[2] - e3[2] * e1[1];
      e2[1] = e3[2] * e1[0] - e3[0] * e1[2];
      e2[2] = e3[0] * e1[1] - e3[1] * e1[0];

      /* Add the translation */
      e1[3] = -( e1[0] * vrp->x + e1[1] * vrp->y + e1[2] * vrp->z);
      e2[3] = -( e2[0] * vrp->x + e2[1] * vrp->y + e2[2] * vrp->z);
      e3[3] = -( e3[0] * vrp->x + e3[1] * vrp->y + e3[2] * vrp->z);

      /* Homogeneous entries */
      m[3][0] = m[3][1] = m[3][2] = 0.0;
      m[3][3] = 1.0;
    }
  }
}

