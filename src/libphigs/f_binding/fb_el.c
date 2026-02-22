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
#include <string.h>
#include <stdio.h>
#include "phg.h"
#include "css.h"
#include "private/phgP.h"
#include "util/ftn.h"

/*******************************************************************************
 * ppl
 *
 * DESCR:   Creates a new element - Polyline
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(ppl)(
                    FTN_INTEGER(n),
                    FTN_REAL_ARRAY(pxa),
                    FTN_REAL_ARRAY(pya)
                    )
{
  Phg_args_add_el args;
  Pint *data;
  Pint i, num_points;
  Ppoint *points;
#ifdef DEBUG
  printf("DEBUG: PPL create poly line\n");
#endif

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_polyline)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      num_points = FTN_INTEGER_GET(n);
      args.el_type = PELEM_POLYLINE;
      args.el_size = sizeof(Pint) + sizeof(Ppoint) * num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = num_points;
        points = (Ppoint *) &data[1];
        for (i = 0; i < num_points; i++) {
          points[i].x = FTN_REAL_ARRAY_GET(pxa, i);
          points[i].y = FTN_REAL_ARRAY_GET(pya, i);
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * ppl3
 *
 * DESCR:   Creates a new element - Polyline 3
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(ppl3)(
                     FTN_INTEGER(n),
                     FTN_REAL_ARRAY(pxa),
                     FTN_REAL_ARRAY(pya),
                     FTN_REAL_ARRAY(pza)
                     )
{
  Pint num_points = FTN_INTEGER_GET(n);
  Phg_args_add_el args;
  Pint i;
  Pint  *data;
  Ppoint3 *point;

#ifdef DEBUG
  printf("DEBUG: PPL3 pset polyline3 called\n");
#endif
  if (phg_entry_check(PHG_ERH, 0, Pfn_polyline3)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_POLYLINE3;
      args.el_size = sizeof(Pint) + sizeof(Ppoint3) * num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = (Pint) num_points;
        point = (Ppoint3*) &data[1];
        for (i = 0; i < num_points; i++) {
          point[i].x = FTN_REAL_ARRAY_GET(pxa, i);
          point[i].y = FTN_REAL_ARRAY_GET(pya, i);
          point[i].z = FTN_REAL_ARRAY_GET(pza, i);
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * pspcli
 *
 * DESCR:       Creates a new element - Line Color Attribute
 * RETURNS:     N/A
 */
FTN_SUBROUTINE(psplci)(
                       FTN_INTEGER(coli)
                       )
{
  Pint colr_ind = FTN_INTEGER_GET(coli);
#ifdef DEBUG
  printf("DEBUG:  psplci set line color\n");
#endif
  pset_line_colr_ind(colr_ind);
}

/*******************************************************************************
 * psln
 *
 * DESCR:       Creates a new element - Line Type Attribute
 * RETURNS:     N/A
 */
FTN_SUBROUTINE(psln)(
                     FTN_INTEGER(ltype)
                     )
{
  Pint linetype = FTN_INTEGER_GET(ltype);
#ifdef DEBUG
  printf("DEBUG: PSLN line type attribute\n");
#endif
  pset_linetype(linetype);
}

/*******************************************************************************
 * pslwsc
 *
 * DESCR:       Creates a new element - Line Width Attribute
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pslwsc)(
                       FTN_REAL(lwidth)
                       )
{
  Phg_args_add_el args;
  Pfloat linewidth = FTN_REAL_GET(lwidth);
#ifdef DEBUG
  printf("DEBUG: PSLWSC line width attribute\n");
#endif
  pset_linewidth(linewidth);
}

/*******************************************************************************
 * pschh
 *
 * DESCR:       Creates a new element - Character height Attribute
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pschh)(
                      FTN_REAL(chh)
                      )
{
  Phg_args_add_el args;
  Pfloat char_ht;
#ifdef DEBUG
  printf("DEBUG: PSCHH character height\n");
#endif

  if (phg_entry_check(PHG_ERH, ERR5, Pfn_set_char_ht)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_CHAR_HT;
      args.el_size = sizeof(Pfloat);
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        char_ht = FTN_REAL_GET(chh);
        memcpy(args.el_data, &char_ht, args.el_size);
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * pads
 *
 * DESCR:   add names to set
 * RETURNS:   N/A
 */

FTN_SUBROUTINE(pads)(
                     FTN_INTEGER(n),
                     FTN_INTEGER_ARRAY(nameset)
                     )
{
  Pint num_names = FTN_INTEGER_GET(n);
  Pint arr[num_names];
  int i;
#ifdef DEBUG
  printf("DEBUG: adding %d names\n", num_names);
#endif
  for (i=0; i<num_names; i++){
    arr[i] =  FTN_INTEGER_ARRAY_GET(nameset, i);
  }
  Pint_list list = {num_names, arr};
  padd_names_set(&list);
}

/*******************************************************************************
 * psvwi
 *
 * DESCR:   set view index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psvwi)(
                      FTN_INTEGER(viewi)
                      )
{
  Pint view_index = FTN_INTEGER_GET(viewi);
#ifdef DEBUG
  printf("DEBUG: PSVWI view ind set to  %d\n", view_index);
#endif
  pset_view_ind(view_index);
}

/*******************************************************************************
 * psici
 *
 * DESCR:   set interior colour index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psici)(
                      FTN_INTEGER(coli)
                      )
{
  Pint colr_ind = FTN_INTEGER_GET(coli);
#ifdef DEBUG
  printf("DEBUG: pset interior color index set to %d\n", colr_ind);
#endif
  pset_int_colr_ind(colr_ind);
}

/*******************************************************************************
 * psis
 *
 * DESCR:   set interior style
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psis)(
                     FTN_INTEGER(ints)
                     )
{
  Pint_style interior_style = (Pint_style) FTN_INTEGER_GET(ints);
#ifdef DEBUG
  printf("DEBUG: PSIS interior style called to %d\n", (int)interior_style);
#endif
  pset_int_style(interior_style);
}

/*******************************************************************************
 * pschup
 *
 * DESCR:   set character up vector
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pschup)(
                       FTN_REAL(chux),
                       FTN_REAL(chuy)
                       )
{
#ifdef DEBUG
  printf("DEBUG: PSCHUP pset char up vector called\n");
#endif
  Pvec char_up_vec;
  char_up_vec.delta_x = FTN_REAL_GET(chux);
  char_up_vec.delta_y = FTN_REAL_GET(chuy);
  pset_char_up_vec(&char_up_vec);
}

/*******************************************************************************
 * psatcu
 *
 * DESCR:   set annotation text character up vector
 * RETURNS:   N/A
 */

FTN_SUBROUTINE(psatcu)(
                       FTN_REAL(atchux),
                       FTN_REAL(atchuy)
                       )
{
#ifdef DEBUG
  printf("DEBUG: PSATCU pset char up vector called\n");
#endif
  Pvec char_up_vec;
  char_up_vec.delta_x = FTN_REAL_GET(atchux);
  char_up_vec.delta_y = FTN_REAL_GET(atchuy);
  pset_anno_char_up_vec(&char_up_vec);
}

/*******************************************************************************
 * pfa
 *
 * DESCR:   fill area
 * RETURNS:   N/A
 */

FTN_SUBROUTINE(pfa)(
                    FTN_INTEGER(n),
                    FTN_REAL_ARRAY(pxa),
                    FTN_REAL_ARRAY(pya)
                    )
{
#ifdef DEBUG
  printf("DEBUG: PFA pfill area called\n");
#endif
  Pint num_points = FTN_INTEGER_GET(n);
  Phg_args_add_el args;
  Pint i;
  Pint  *data;
  Ppoint *point;
  if (phg_entry_check(PHG_ERH, 0, Pfn_fill_area)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_FILL_AREA;
      args.el_size = sizeof(Pint) + sizeof(Ppoint) * num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = num_points;
        point = (Ppoint*) &data[1];
        for (i=0; i<num_points;i++){
          point[i].x = FTN_REAL_ARRAY_GET(pxa, i);
          point[i].y = FTN_REAL_ARRAY_GET(pya, i);
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * pfa
 *
 * DESCR:   fill area 3
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pfa3)(
                     FTN_INTEGER(n),
                     FTN_REAL_ARRAY(pxa),
                     FTN_REAL_ARRAY(pya),
                     FTN_REAL_ARRAY(pza)
                     )
{
#ifdef DEBUG
  printf("DEBUG: PFA3 fill area called\n");
#endif
  Pint num_points = FTN_INTEGER_GET(n);
  Phg_args_add_el args;
  Pint i;
  Pint  *data;
  Ppoint3 *point;
  if (phg_entry_check(PHG_ERH, 0, Pfn_fill_area)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      args.el_type = PELEM_FILL_AREA3;
      args.el_size = sizeof(Pint) + sizeof(Ppoint3) * num_points;
      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = num_points;
        point = (Ppoint3*) &data[1];
        for (i=0; i<num_points;i++){
          point[i].x = FTN_REAL_ARRAY_GET(pxa, i);
          point[i].y = FTN_REAL_ARRAY_GET(pya, i);
          point[i].z = FTN_REAL_ARRAY_GET(pza, i);
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * plb
 *
 * DESCR:   label
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(plb)(
                    FTN_INTEGER(label)
                    )
{
  Pint label_id = FTN_INTEGER_GET(label);
#ifdef DEBUG
  printf("DEBUG: PLB set label %d\n", label_id);
#endif
  plabel(label_id);
}

/*******************************************************************************
 * psedfg
 *
 * DESCR:   set edge flag
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psedfg)(
                       FTN_INTEGER(flag)
                       )
{
  Pint edge_flag = FTN_INTEGER_GET(flag);
#ifdef DEBUG
  printf("DEBUG: PSEDFG Set edge flag %d\n", edge_flag);
#endif
  pset_edge_flag(edge_flag);
}

/*******************************************************************************
 * ppm
 *
 * DESCR:   polymarker
 * RETURNS:   N/A
 */

FTN_SUBROUTINE(ppm)(
                    FTN_INTEGER(n),
                    FTN_REAL_ARRAY(pxa),
                    FTN_REAL_ARRAY(pya)
                    )
{
  int num_points = FTN_INTEGER_GET(n);
  Ppoint points[num_points];
  Ppoint_list ppoint_list;
  int i;
  ppoint_list.num_points = num_points;
  for (i=0; i<num_points; i++){
    points[i].x = FTN_REAL_ARRAY_GET(pxa, i);
    points[i].y = FTN_REAL_ARRAY_GET(pya, i);
#ifdef DEBUG
    printf("PPM %d pos %f %f\n", i, points[i].x, points[i].y);
#endif
  }
  ppoint_list.points = &points[0];
  ppolymarker(&ppoint_list);
}

/*******************************************************************************
 * ppm
 *
 * DESCR:   polymarker 3
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(ppm3)(
                     FTN_INTEGER(n),
                     FTN_REAL_ARRAY(pxa),
                     FTN_REAL_ARRAY(pya),
                     FTN_REAL_ARRAY(pza)
                     )
{
  int num_points = FTN_INTEGER_GET(n);
  Ppoint3 points[num_points];
  Ppoint_list3 ppoint_list;
  int i;
  ppoint_list.num_points = num_points;
  for (i=0; i<num_points; i++){
    points[i].x = FTN_REAL_ARRAY_GET(pxa, i);
    points[i].y = FTN_REAL_ARRAY_GET(pya, i);
    points[i].z = FTN_REAL_ARRAY_GET(pza, i);
  }
  ppoint_list.points = &points[0];
  ppolymarker3(&ppoint_list);
}

/*******************************************************************************
 * psewsc
 *
 * DESCR:   set edgewidth scale factor
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psewsc)(
                       FTN_REAL(ewidth)
                       )
{
  Pfloat esfr = FTN_REAL_GET(ewidth);
#ifdef DEBUG
  printf("DEBUG: PSEDWSG set edge scale factor to %f\n", esfr);
#endif
  pset_edgewidth(esfr);
}

/*******************************************************************************
 * psmksc
 *
 * DESCR:       set marker size scale factor
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psmksc)(
                       FTN_REAL(mszsf)
                       )
{
  Pfloat pmsf = FTN_REAL_GET(mszsf);
#ifdef DEBUG
  printf("DEBUG: PSMKSC set poly marker scale factor to %f\n", pmsf);
#endif
  pset_marker_size(pmsf);
}

/*******************************************************************************
 * pspmi
 *
 * DESCR:       set polymarker index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pspmi)(
                      FTN_INTEGER(pmi)
                      )
{
  Pint pindex = FTN_INTEGER_GET(pmi);
#ifdef DEBUG
  printf("DEBUG: PSPMI set marker index to %d\n", pindex);
#endif
  pset_marker_ind(pindex);

}

/*******************************************************************************
 * pspmci
 *
 * DESCR:       set polymarker colour index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pspmci)(
                       FTN_INTEGER(coli)
                       )
{
  Pint colr_ind = FTN_INTEGER_GET(coli);
#ifdef DEBUG
  printf("DEBUG: PSPMCI set marker color index to %d\n", colr_ind);
#endif
  pset_marker_colr_ind(colr_ind);

}

/*******************************************************************************
 * psmk
 *
 * DESCR:       set marker type
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psmk)(
                     FTN_INTEGER(mtype)
                     )
{
  Pint marker_type = FTN_INTEGER_GET(mtype);
#ifdef DEBUG
  printf("DEBUG: PSMK set marker type to %d\n", marker_type);
#endif
  pset_marker_type(marker_type);
}

/*******************************************************************************
 * pexst
 *
 * DESCR:       execute structure
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pexst)(
                      FTN_INTEGER(strid)
                      )
{
  Pint str_id = FTN_INTEGER_GET(strid);
#ifdef DEBUG
  printf("DEBUG: PEXST execute structure %d\n", str_id);
#endif
  pexec_struct(str_id);
}

/*******************************************************************************
 * psgmt3
 *
 * DESCR:       set global transformation 3
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psgmt3)(
                       float* xfrmt
                       )
{
  Pmatrix3 global_tran;
  int i, j;
#ifdef DEBUG
  printf("DEBUG: PSGMT3 set global transformation matrix.\n");
#endif
  for (i=0; i<4; i++){
    for (j=0; j<4; j++){
      global_tran[j][i] = (Pfloat) xfrmt[4*i+j];
    }
  }
  pset_global_tran3(global_tran);
}

/*******************************************************************************
 * pshrm
 *
 * DESCR:       set hlhsr mode
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pshrm)(
                      FTN_INTEGER(wkid),
                      FTN_INTEGER(hrm)
                      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint mode = FTN_INTEGER_GET(hrm);
#ifdef DEBUG
  printf("DEBUG: PSHRM set hlrs mode to %d.\n", mode);
#endif
  pset_hlhsr_mode(ws_id, mode);
}

/*******************************************************************************
 * pshrid
 *
 * DESCR:       set hlhsr identifier
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pshrid)(
                       FTN_INTEGER(hrid)
                       )
{
  Pint hrmode = FTN_INTEGER_GET(hrid);
#ifdef DEBUG
  printf("DEBUG: PSHRID set hlrs id to %d.\n", hrmode);
#endif
  if (hrmode == 0) {
    pset_hlhsr_id(PHIGS_HLHSR_ID_OFF);
  } else {
    pset_hlhsr_id(PHIGS_HLHSR_ID_ON);
  }
}

/*******************************************************************************
 * pspkid
 *
 * DESCR:       set pick identifier
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pspkid)(
                       FTN_INTEGER(pkid)
                       )
{
  Pint pk_id = FTN_INTEGER_GET(pkid);
  pset_pick_id(pk_id);
}

/*******************************************************************************
 * pschsp
 *
 * DESCR:       set character spacing
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pschsp)(
                       FTN_REAL(chsp)
                       )
{
  Pfloat ch_sp = FTN_REAL_GET(chsp);
  pset_char_space(ch_sp);
}

/*******************************************************************************
 * pschxp
 *
 * DESCR:       set character expansion factor
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pschxp)(
                       FTN_REAL(chxp)
                       )
{
  Pfloat ch_xp = FTN_REAL_GET(chxp);
  pset_char_expan(ch_xp);
}

/*******************************************************************************
 * psedi
 *
 * DESCR:       set edge index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psedi)(
                      FTN_INTEGER(edi)
                      )
{
  Pint edge_index = FTN_INTEGER_GET(edi);
  pset_edge_ind(edge_index);
}

/*******************************************************************************
 * psiasf
 *
 * DESCR:       set individual asf
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psiasf)(
                       FTN_INTEGER(attr),
                       FTN_INTEGER(val)
                       )
{
  Paspect attrid = (Paspect) FTN_INTEGER_GET(attr);
  Pasf asfval = (Pasf) FTN_INTEGER_GET(val);
  pset_indiv_asf(attrid, asfval);
}

/*******************************************************************************
 * psedci
 *
 * DESCR:       set edge colour index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psedci)(
                       FTN_INTEGER(coli)
                       )
{
  Pint colr_ind = FTN_INTEGER_GET(coli);
#ifdef DEBUG
  printf("DEBUG: Set edge color index %d\n", colr_ind);
#endif
  pset_edge_colr_ind(colr_ind);
}
