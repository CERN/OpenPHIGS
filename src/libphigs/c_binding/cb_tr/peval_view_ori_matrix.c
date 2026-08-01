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
 * \file peval_view_ori_matrix.c
 * \brief Generate view orientation matrix
 */
void peval_view_ori_matrix(
                           Ppoint *vrp,               /* view reference point */
                           Pvec *vup,                 /* view up vector */
                           Pint *error_ind,           /* OUT error indicator */
                           Pmatrix m                  /* OUT view orientation matrix */
                           )
{
    /* The old basis is: e1 = < 1, 0>,  e2 = < 0, 1>
     * The new basis is: e1' = < vup.y, -vup.x> / |vup|,  e2' = vup / |vup|.
     * Therefore the transform for old to new is x' = ATx, where:

       | e1' 0 |         | 1 0 -vrp.x |
   A = |       |,    T = | 0 1 -vrp.y |
       | e2' 0 |         | 0 0    1   |
       |       |
       | -0-  1|
     */

  double s;

  ERR_SET_CUR_FUNC(PHG_ERH, Pfn_INQUIRY);

  if (PSL_SYS_STATE(PHG_PSL) != PSYS_ST_PHOP) {
    *error_ind = ERR2;
  } else if ( PHG_ZERO_MAG(s = PHG_MAG_V2(vup)) ) {
    *error_ind = ERR160;
  } else {
    *error_ind = 0;

    /* Compute the new basis, note that m[0] is e1' and m[1] is e2'. */
    s = 1.0 / s;
    m[0][0] = s * vup->delta_y;
    m[0][1] = s * -vup->delta_x;
    m[1][0] = s * vup->delta_x;
    m[1][1] = s * vup->delta_y;

    /* Add the translation */
    m[0][2] = -( m[0][0] * vrp->x + m[0][1] * vrp->y);
    m[1][2] = -( m[1][0] * vrp->x + m[1][1] * vrp->y);

    /* Homogeneous entries */
    m[2][0] = m[2][1] = 0.0;
    m[2][2] = 1.0;
  }
}

