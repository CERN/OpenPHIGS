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

#include <stdlib.h>

#include "phg.h"
#include "phgtype.h"
#include "private/arP.h"

/******************************************************************************
 * phg_swap_nil
 *
 * DESCR:       Swap empty element
 * RETURNS:     N/A
 */

static void phg_swap_nil(
   Phg_swap *swp,
   void *data
   )
{
}

/******************************************************************************
 * phg_swap_int
 *
 * DESCR:       Swap integer element
 * RETURNS:     N/A
 */

static void phg_swap_int(
   Phg_swap *swp,
   void *data
   )
{
   (*swp->conv_long)((uint32_t *) data);
}

/******************************************************************************
 * phg_swap_int2
 *
 * DESCR:       Swap dual integer element
 * RETURNS:     N/A
 */

static void phg_swap_int2(
   Phg_swap *swp,
   void *data
   )
{
   Pint *idata;

   idata = (Pint *) data;
   (*swp->conv_long)((uint32_t *) idata);
   (*swp->conv_long)((uint32_t *) &idata[1]);
}

/******************************************************************************
 * phg_swap_float
 *
 * DESCR:       Swap float element
 * RETURNS:     N/A
 */

static void phg_swap_float(
   Phg_swap *swp,
   void *data
   )
{
   (*swp->conv_float)((float *) data);
}

/******************************************************************************
 * phg_swap_float2
 *
 * DESCR:       Swap dual float element
 * RETURNS:     N/A
 */

static void phg_swap_float2(
   Phg_swap *swp,
   void *data
   )
{
   Pfloat *fdata;

   fdata = (Pfloat *) data;
   (*swp->conv_float)((float *) fdata);
   (*swp->conv_float)((float *) &fdata[1]);
}

/******************************************************************************
 * phg_swap_int_list
 *
 * DESCR:       Swap integer list element
 * RETURNS:     N/A
 */

static void phg_swap_int_list(
   Phg_swap *swp,
   void *data
   )
{
   Pint *idata;
   Pint_list int_list;
   Pint i;

   idata = (Pint *) data;
   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      int_list.num_ints = idata[0];
      (*swp->conv_long)((uint32_t *) &int_list.num_ints);
   }
   else {
      (*swp->conv_long)((uint32_t *) idata);
      int_list.num_ints = idata[0];
   }

   int_list.ints = (Pint *) &idata[1];
   for (i = 0; i < int_list.num_ints; i++) {
      (*swp->conv_long)((uint32_t *) &int_list.ints[i]);
   }
}

/******************************************************************************
 * phg_swap_point_list
 *
 * DESCR:       Swap point list element
 * RETURNS:     N/A
 */

static void phg_swap_point_list(
   Phg_swap *swp,
   void *data
   )
{
   Pint *idata;
   Ppoint_list point_list;
   Pint i;

   idata = (Pint *) data;

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      point_list.num_points = idata[0];
      (*swp->conv_long)((uint32_t *) &point_list.num_points);
   }
   else {
      (*swp->conv_long)((uint32_t *) idata);
      point_list.num_points = idata[0];
   }

   point_list.points = (Ppoint *) &idata[1];
   for (i = 0; i < point_list.num_points; i++) {
      (*swp->conv_float)((float *) &point_list.points[i].x);
      (*swp->conv_float)((float *) &point_list.points[i].y);
   }
}

/******************************************************************************
 * phg_swap_point_list3
 *
 * DESCR:       Swap point list 3D element
 * RETURNS:     N/A
 */

static void phg_swap_point_list3(
   Phg_swap *swp,
   void *data
   )
{
   Pint *idata;
   Ppoint_list3 point_list;
   Pint i;

   idata = (Pint *) data;

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      point_list.num_points = idata[0];
      (*swp->conv_long)((uint32_t *) &point_list.num_points);
   }
   else {
      (*swp->conv_long)((uint32_t *) idata);
      point_list.num_points = idata[0];
   }

   point_list.points = (Ppoint3 *) &idata[1];
   for (i = 0; i < point_list.num_points; i++) {
      (*swp->conv_float)((float *) &point_list.points[i].x);
      (*swp->conv_float)((float *) &point_list.points[i].y);
      (*swp->conv_float)((float *) &point_list.points[i].z);
   }
}

/******************************************************************************
 * phg_swap_text
 *
 * DESCR:       Swap text element
 * RETURNS:     N/A
 */

static void phg_swap_text(
   Phg_swap *swp,
   void *data
   )
{
   Ppoint *pdata;

   pdata = (Ppoint *) data;
   (*swp->conv_float)((float *) &pdata->x);
   (*swp->conv_float)((float *) &pdata->y);
}

/******************************************************************************
 * phg_swap_text3
 *
 * DESCR:       Swap text element
 * RETURNS:     N/A
 */

static void phg_swap_text3(
   Phg_swap *swp,
   void *data
   )
{
   Ppoint3 *pdata;

   pdata = (Ppoint3 *) data;
   (*swp->conv_float)((float *) &pdata->x);
   (*swp->conv_float)((float *) &pdata->y);
   (*swp->conv_float)((float *) &pdata->z);
}

/******************************************************************************
 * phg_swap_matrix3
 *
 * DESCR:       Swap matrix 3D element
 * RETURNS:     N/A
 */

static void phg_swap_matrix3(
   Phg_swap *swp,
   void *data
   )
{
   Pfloat *fdata;
   Pint i;

   fdata = (Pfloat *) data;

   for (i = 0; i < 16; i++) {
      (*swp->conv_float)((float *) fdata++);
   }
}

/******************************************************************************
 * phg_swap_local_tran3
 *
 * DESCR:       Swap local transformation 3D element
 * RETURNS:     N/A
 */

static void phg_swap_local_tran3(
   Phg_swap *swp,
   void *data
   )
{
   Pint *idata;
   Pfloat *fdata;
   Pint i;

   idata = (Pint *) data;
   (*swp->conv_long)((uint32_t *) idata);
   fdata = (Pfloat *) &idata[1];

   for (i = 0; i < 16; i++) {
      (*swp->conv_float)((float *) fdata++);
   }
}

/******************************************************************************
 * phg_swap_gcolr
 *
 * DESCR:       Swap generic colour element
 * RETURNS:     N/A
 */

static void phg_swap_gcolr(
   Phg_swap *swp,
   void *data
   )
{
   Pint *idata;
   Pfloat *fdata;
   Pgcolr gcolr;

   idata = (Pint *) data;

   if (swp->fromFormat & PHG_AR_HOST_BYTE_ORDER) {
      gcolr.type = idata[0];
      (*swp->conv_long)((uint32_t *) &gcolr.type);
   }
   else {
      (*swp->conv_long)((uint32_t *) idata);
      gcolr.type = idata[0];
   }

   if (gcolr.type == PINDIRECT) {
      (*swp->conv_long)((uint32_t *) &idata[1]);
   }
   else if (gcolr.type == PMODEL_RGB) {
      fdata = (Pfloat *) &idata[1];
      (*swp->conv_float)((float *) &fdata[0]);
      (*swp->conv_float)((float *) &fdata[1]);
      (*swp->conv_float)((float *) &fdata[2]);
   }
   else if (gcolr.type == PMODEL_RGBA) {
      fdata = (Pfloat *) &idata[1];
      (*swp->conv_float)((float *) &fdata[0]);
      (*swp->conv_float)((float *) &fdata[1]);
      (*swp->conv_float)((float *) &fdata[2]);
      (*swp->conv_float)((float *) &fdata[3]);
   }
}

/******************************************************************************
 * phg_swap_anno_text_rel
 *
 * DESCR:       Swap text element
 * RETURNS:     N/A
 */

static void phg_swap_anno_text_rel(
   Phg_swap *swp,
   void *data
   )
{
   Ppoint *pdata;
   Pvec *odata;
   pdata = (Ppoint *) data;
   (*swp->conv_float)((float *) &pdata->x);
   (*swp->conv_float)((float *) &pdata->y);
   odata = (Pvec *) &pdata[1];
   (*swp->conv_float)((float *) &odata->delta_x);
   (*swp->conv_float)((float *) &odata->delta_y);

}

/******************************************************************************
 * phg_swap_anno_text_rel
 *
 * DESCR:       Swap text element
 * RETURNS:     N/A
 */

static void phg_swap_anno_text_rel3(
   Phg_swap *swp,
   void *data
   )
{
   Ppoint3 *pdata;
   Pvec3 * odata;
   pdata = (Ppoint3 *) data;
   (*swp->conv_float)((float *) &pdata->x);
   (*swp->conv_float)((float *) &pdata->y);
   (*swp->conv_float)((float *) &pdata->z);
   odata = (Pvec3 *) &pdata[1];
   (*swp->conv_float)((float *) &odata->delta_x);
   (*swp->conv_float)((float *) &odata->delta_y);
   (*swp->conv_float)((float *) &odata->delta_z);
}


Phg_conv phg_swap_tbl[PELEM_NUM_EL_TYPES] = {
   NULL,                           /* PELEM_ALL */
   phg_swap_nil,                   /* PELEM_NIL */
   phg_swap_int_list,              /* PELEM_ADD_NAMES_SET */
   phg_swap_int_list,              /* PELEM_REMOVE_NAMES_SET */
   phg_swap_point_list,            /* PELEM_FILL_AREA */
   phg_swap_point_list3,           /* PELEM_FILL_AREA3 */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET3 */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET3_DATA */
   phg_swap_nil,                   /* PELEM_SET_OF_FILL_AREA_SET3_DATA */
   phg_swap_point_list,            /* PELEM_POLYLINE */
   phg_swap_point_list3,           /* PELEM_POLYLINE3 */
   phg_swap_point_list,            /* PELEM_POLYMARKER */
   phg_swap_point_list3,           /* PELEM_POLYMARKER3 */
   phg_swap_text,                  /* PELEM_TEXT */
   phg_swap_int,                   /* PELEM_INT_IND */
   phg_swap_int,                   /* PELEM_INT_COLR_IND */
   phg_swap_int,                   /* PELEM_INT_STYLE */
   phg_swap_int,                   /* PELEM_BACK_INT_STYLE */
   phg_swap_int,                   /* PELEM_INT_STYLE_IND */
   phg_swap_int,                   /* PELEM_BACK_INT_STYLE_IND */
   phg_swap_int,                   /* PELEM_LINE_COLR_IND */
   phg_swap_float,                 /* PELEM_LINEWIDTH */
   phg_swap_int,                   /* PELEM_LINETYPE */
   phg_swap_int,                   /* PELEM_LINE_IND */
   phg_swap_int,                   /* PELEM_MARKER_IND */
   phg_swap_int,                   /* PELEM_MARKER_COLR_IND */
   phg_swap_float,                 /* PELEM_MARKER_SIZE */
   phg_swap_int,                   /* PELEM_MARKER_TYPE */
   phg_swap_int,                   /* PELEM_EDGE_IND */
   phg_swap_int,                   /* PELEM_EDGE_COLR_IND */
   phg_swap_float,                 /* PELEM_EDGEWIDTH */
   phg_swap_int,                   /* PELEM_EDGETYPE */
   phg_swap_int,                   /* PELEM_EDGE_FLAG */
   phg_swap_int,                   /* PELEM_TEXT_IND */
   phg_swap_int,                   /* PELEM_TEXT_FONT */
   phg_swap_int,                   /* PELEM_TEXT_PREC */
   phg_swap_int,                   /* PELEM_TEXT_PATH */
   phg_swap_int2,                  /* PELEM_TEXT_ALIGN */
   phg_swap_float,                 /* PELEM_CHAR_HT */
   phg_swap_float,                 /* PELEM_CHAR_EXPAN */
   phg_swap_float,                 /* PELEM_CHAR_SPACE */
   phg_swap_float2,                /* PELEM_CHAR_UP_VEC */
   phg_swap_int,                   /* PELEM_TEXT_COLR_IND */
   phg_swap_int2,                  /* PELEM_INDIV_ASF */
   phg_swap_local_tran3,           /* PELEM_LOCAL_MODEL_TRAN3 */
   phg_swap_matrix3,               /* PELEM_GLOBAL_MODEL_TRAN3 */
   phg_swap_int,                   /* PELEM_VIEW_IND */
   phg_swap_int,                   /* PELEM_EXEC_STRUCT */
   phg_swap_int,                   /* PELEM_LABEL */
   phg_swap_int,                   /* PELEM_PICK_ID */
   phg_swap_int,                   /* PELEM_HLHSR_ID */
   phg_swap_gcolr,                 /* PELEM_INT_COLR */
   phg_swap_gcolr,                 /* PELEM_BACK_INT_COLR */
   phg_swap_gcolr,                 /* PELEM_LINE_COLR */
   phg_swap_gcolr,                 /* PELEM_MARKER_COLR */
   phg_swap_gcolr,                 /* PELEM_EDGE_COLR */
   phg_swap_gcolr,                 /* PELEM_TEXT_COLR */
   phg_swap_nil,                   /* PELEM_LIGHT_SRC_STATE */
   phg_swap_int,                   /* PELEM_INT_SHAD_METH */
   phg_swap_int,                   /* PELEM_BACK_INT_SHAD_METH */
   phg_swap_int,                   /* PELEM_INT_REFL_EQN */
   phg_swap_int,                   /* PELEM_BACK_INT_REFL_EQN */
   phg_swap_nil,                   /* PELEM_REFL_PROPS */
   phg_swap_nil,                   /* PELEM_BACK_REFL_PROPS */
   phg_swap_int,                   /* PELEM_FACE_DISTING_MODE */
   phg_swap_int,                   /* PELEM_FACE_CULL_MODE */
   phg_swap_anno_text_rel3,        /* PELEM_ANNO_TEXT_REL3 */
   phg_swap_anno_text_rel,         /* PELEM_ANNO_TEXT_REL */
   phg_swap_float,                 /* PELEM_ANNO_CHAR_HT */
   phg_swap_float2,                /* PELEM_ANNO_CHAR_UP_VEC */
   phg_swap_int,                   /* PELEM_ANNO_PATH */
   phg_swap_int2,                  /* PELEM_ANNO_ALIGN */
   phg_swap_int,                   /* PELEM_ANNO_STYLE */
   phg_swap_nil,                   /* PELEM_MODEL_CLIP_VOL3 */
   phg_swap_int,                   /* PELEM_MODEL_CLIP_IND */
   phg_swap_nil,                   /* PELEM_FILL_AREA_SET_DATA */
   phg_swap_nil,                   /* PELEM_GSE */
   phg_swap_nil,                   /* PELEM_ALPHA_CHANNEL */
   phg_swap_text3,                 /* PELEM_TEXT3 */
   phg_swap_int                    /* PELEM_BACK_INT_COLR_IND */
};
