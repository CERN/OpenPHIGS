/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022-2023 CERN
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
******************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <util/ftn.h>

#ifndef  MAX_ARRAY_SIZE
#define  MAX_ARRAY_SIZE 400
#endif

/*******************************************************************************
 * pslss
 *
 * DESCR:       set light source state
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pslss)(
                      FTN_INTEGER(nacti),
                      FTN_INTEGER_ARRAY(acti),
                      FTN_INTEGER(ndeacti),
                      FTN_INTEGER_ARRAY(deacti)
                      )
{
  Pint n_active = FTN_INTEGER_GET(nacti);
  Pint n_deactive = FTN_INTEGER_GET(ndeacti);
  Pint_list active;
  Pint_list deactive;
  Pint i;
  Pint aarr[n_active];
  Pint darr[n_deactive];
  for (i=0; i<n_active; i++){
    aarr[i] = FTN_INTEGER_ARRAY_GET(acti, i);
  }
  for (i=0; i<n_deactive; i++){
    darr[i] = FTN_INTEGER_ARRAY_GET(deacti, i);
  }
#ifdef DEBUG
  printf("DEBUG: set light source status elem 0 are active %d decactive %d\n", acti[0], deacti[0]);
#endif
  active.num_ints = n_active;
  active.ints = &aarr[0];
  deactive.num_ints = n_deactive;
  deactive.ints = &darr[0];
  pset_light_src_state(&active, &deactive);
}

/*******************************************************************************
 * psism
 *
 * DESCR:       set interior shading method
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psism)(
                      FTN_INTEGER(ism1)
                      ){
  Pint ism = FTN_INTEGER_GET(ism1);
  pset_int_shad_meth(ism);
}

/*******************************************************************************
 * psbism
 *
 * DESCR:       set back interior shading method
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psbism)(
                       FTN_INTEGER(ism1)
                       ){
  Pint ism = FTN_INTEGER_GET(ism1);
  pset_back_int_shad_meth(ism);
}

/*******************************************************************************
 * psrfeq
 *
 * DESCR:       set reflectance equation
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psrfeq)(
                       FTN_INTEGER(refl_model1)
                       ){
  Pint refl_model = FTN_INTEGER_GET(refl_model1);
  pset_refl_eqn(refl_model);
}

/*******************************************************************************
 * psbrfm
 *
 * DESCR:       set back reflectance equation
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psbrfm)(
                       FTN_INTEGER(refl_model1)
                       ){
  Pint refl_model = FTN_INTEGER_GET(refl_model1);
  pset_back_refl_eqn(refl_model);
}

/*******************************************************************************
 * psrfp
 *
 * DESCR:       set reflectance properties
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psrfp)(
                      FTN_INTEGER(refl_type1),
                      FTN_INTEGER(refl_len1),
                      char* datarec
                      ){
  Pint refl_type = FTN_INTEGER_GET(refl_type1);
  int refl_len = FTN_INTEGER_GET(refl_len1);
  int* here = (int*)datarec;
  float* fp;
  Pfloat farr[4];
  Prefl_props refl_properties;
  int num_ints = here[0];
  if (refl_type == 1) {
    int col_type = here[1];
    int ncc = here[2];
    int index = here[3];
    refl_properties.specular_colr.type = col_type;
    if (col_type == PINDIRECT){
      refl_properties.specular_colr.val.ind = index;
    }
    fp = (float*) &here[5];
    refl_properties.ambient_coef = fp[0];
    refl_properties.diffuse_coef = fp[1];
    refl_properties.specular_coef = fp[2];
    refl_properties.specular_exp = fp[3];
#ifdef DEBUG
    printf("PSRFP: Reflectance %f %f %f %f",
           refl_properties.ambient_coef,
           refl_properties.diffuse_coef,
           refl_properties.specular_coef,
           refl_properties.specular_exp);
#endif
    if (col_type ==  PMODEL_RGB){
      refl_properties.specular_colr.val.general.x = fp[4];
      refl_properties.specular_colr.val.general.y = fp[5];
      refl_properties.specular_colr.val.general.z = fp[6];
    }
    pset_refl_props(&refl_properties);
  } else {
    printf("PSRFP: unknown reflection type. Ignorning function.\n");
  }
}

/*******************************************************************************
 * pfas3d
 *
 * DESCR:       fill area set 3
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pfas3d)(
                       FTN_INTEGER(fflag1),
                       FTN_INTEGER(eflag1),
                       FTN_INTEGER(vflag1),
                       FTN_INTEGER(colr_type1),
                       FTN_INTEGER(ncc1),
                       FTN_INTEGER(fcoli1),
                       FTN_REAL_ARRAY(fcolr),
                       FTN_REAL(fnx),
                       FTN_REAL(fny),
                       FTN_REAL(fnz),
                       FTN_INTEGER(fdlen),
                       FTN_REAL_ARRAY(fdata1),
                       FTN_INTEGER(npl),
                       FTN_INTEGER_ARRAY(ixa),
                       FTN_INTEGER_ARRAY(edata),
                       FTN_REAL_ARRAY(pxa),
                       FTN_REAL_ARRAY(pya),
                       FTN_REAL_ARRAY(pza),
                       FTN_INTEGER_ARRAY(vcoli),
                       FTN_REAL_ARRAY(vcolr),
                       FTN_REAL_ARRAY(vnxa),
                       FTN_REAL_ARRAY(vnya),
                       FTN_REAL_ARRAY(vnza),
                       FTN_INTEGER(vdn),
                       FTN_REAL_ARRAY(vdata1)
                       )
{
  Pint fflag = FTN_INTEGER_GET(fflag1);
  Pint eflag = FTN_INTEGER_GET(eflag1);
  Pint vflag = FTN_INTEGER_GET(vflag1);
  Pint colr_type = FTN_INTEGER_GET(colr_type1);
  Pint ncc = FTN_INTEGER_GET(ncc1);
  Pint nfa = FTN_INTEGER_GET(npl);

  Phg_args_add_el args;
  Pint i, j;
  unsigned facet_size, vertex_size;
  Pint *data;
  char *tp;
  Pint num_vertices;
  Ppoint3 vbuffer[MAX_ARRAY_SIZE];
  Pptco3   cbuffer[MAX_ARRAY_SIZE];
  Pptnorm3 nbuffer[MAX_ARRAY_SIZE];
  Pptconorm3 cnbuffer[MAX_ARRAY_SIZE];

  Pcoval coval;
  Pvec3 vec3;
  Pconorm3 conorm;

#ifdef DEBUG
  printf("DEBUG: PFAS3D called. NFA is set to %d\n", nfa);
#endif
  if (phg_entry_check(PHG_ERH, 0, Pfn_fill_area_set3_data)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      switch (fflag) {
      case PFACET_COLOUR:
        facet_size = sizeof(Pcoval);
        break;

      case PFACET_NORMAL:
        facet_size = sizeof(Pvec3);
        break;

      case PFACET_COLOUR_NORMAL:
        facet_size = sizeof(Pconorm3);
        break;

      default:
        facet_size = 0;
        break;
      }
      switch (vflag) {
      case PVERT_COORD:
        vertex_size = sizeof(Ppoint3);
        break;

      case PVERT_COORD_COLOUR:
        vertex_size = sizeof(Pptco3);
        break;

      case PVERT_COORD_NORMAL:
        vertex_size = sizeof(Pptnorm3);
        break;

      case PVERT_COORD_COLOUR_NORMAL:
        vertex_size = sizeof(Pptconorm3);
        break;

      default:
        vertex_size = 0;
        break;
      }
      args.el_type = PELEM_FILL_AREA_SET3_DATA;
      args.el_size = 5 * sizeof(Pint) + facet_size;

      if (eflag == PEDGE_VISIBILITY) {
        for (i = 0; i < nfa; i++) {
          args.el_size += sizeof(Pint);   /* Pint num_edges */
          args.el_size += ixa[i] * sizeof(Pedge_flag);
        }
      }

      for (i = 0; i < nfa; i++) {
        args.el_size += sizeof(Pint);      /* Pint num_vertices */
        args.el_size += ixa[i] * vertex_size;
      }

      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = fflag;
        data = &data[1];
        data[0] = eflag;
        data = &data[1];
        data[0] = vflag;
        data = &data[1];
        data[0] = colr_type;
        tp = (char *) &data[1];
        switch(fflag) {
        case PFACET_COLOUR:
          coval.ind = FTN_INTEGER_GET(fcoli1);
          memcpy(tp, &coval, sizeof(Pcoval));
          tp += sizeof(Pcoval);
          break;

        case PFACET_NORMAL:
          vec3.delta_x = FTN_REAL_ARRAY_GET(fcolr, 0);
          vec3.delta_y = FTN_REAL_ARRAY_GET(fcolr, 1);
          vec3.delta_z = FTN_REAL_ARRAY_GET(fcolr, 2);
          memcpy(tp, &vec3, sizeof(Pvec3));
          tp += sizeof(Pvec3);
          break;

        case PFACET_COLOUR_NORMAL:
          conorm.colr.ind = FTN_INTEGER_GET(fcoli1);
          conorm.norm.delta_x = FTN_REAL_GET(fnx);
          conorm.norm.delta_y = FTN_REAL_GET(fny);
          conorm.norm.delta_z = FTN_REAL_GET(fnz);
          memcpy(tp, &conorm, sizeof(Pconorm3));
          tp += sizeof(Pconorm3);
          break;

        default:
          break;
        }

        data = (Pint *) tp;
        data[0] = nfa;
        tp = (char *) &data[1];

        if (eflag == PEDGE_VISIBILITY) {
          for (i = 0; i < nfa; i++) {
            num_vertices = ixa[i];
            data = (Pint *) tp;
            data[0] = num_vertices;
            tp = (char *) &data[1];
            memcpy(tp, &edata[0],
                   sizeof(Pedge_flag) * num_vertices);
            tp += sizeof(Pedge_flag) * num_vertices;
          }
        }
        for (i = 0; i < nfa; i++) {
          num_vertices = ixa[i];
          data = (Pint *) tp;
          data[0] = num_vertices;
          tp = (char *) &data[1];

          switch (vflag) {
          case PVERT_COORD:
            for (j=0; j<num_vertices; j++){
              vbuffer[j].x = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              vbuffer[j].y = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              vbuffer[j].z = FTN_REAL_ARRAY_GET(pza, j+num_vertices*i);
            }
            memcpy(tp, &vbuffer[0],
                   num_vertices * sizeof(Ppoint3));
            tp += num_vertices * sizeof(Ppoint3);
            break;

          case PVERT_COORD_COLOUR:
            for (j=0; j<num_vertices; j++){
              cbuffer[j].colr.ind = FTN_INTEGER_ARRAY_GET(vcoli, j+num_vertices*i);
              cbuffer[j].point.x  = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              cbuffer[j].point.y  = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              cbuffer[j].point.z  = FTN_REAL_ARRAY_GET(pza, j+num_vertices*i);
            }
            memcpy(tp, &cbuffer[0],
                   num_vertices * sizeof(Pptco3));
            tp += num_vertices * sizeof(Pptco3);
            break;

          case PVERT_COORD_NORMAL:
            for (j=0; j<num_vertices; j++){
              nbuffer[j].point.x = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              nbuffer[j].point.y = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              nbuffer[j].point.z = FTN_REAL_ARRAY_GET(pza, j+num_vertices*i);
              nbuffer[j].norm.delta_x = FTN_REAL_ARRAY_GET(vnxa, j+num_vertices*i);
              nbuffer[j].norm.delta_y = FTN_REAL_ARRAY_GET(vnya, j+num_vertices*i);
              nbuffer[j].norm.delta_z = FTN_REAL_ARRAY_GET(vnza, j+num_vertices*i);
            }
            memcpy(tp, &nbuffer[0],
                   num_vertices * sizeof(Pptnorm3));
            tp += num_vertices * sizeof(Pptnorm3);
            break;

          case PVERT_COORD_COLOUR_NORMAL:
            for (j=0; j<num_vertices; j++){
              cnbuffer[j].colr.ind    = FTN_INTEGER_ARRAY_GET(vcoli, j+num_vertices*i);
              cnbuffer[j].point.x = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              cnbuffer[j].point.y = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              cnbuffer[j].point.z = FTN_REAL_ARRAY_GET(pza, j+num_vertices*i);

              cnbuffer[j].norm.delta_x = FTN_REAL_ARRAY_GET(vnxa, j+num_vertices*i);
              cnbuffer[j].norm.delta_y = FTN_REAL_ARRAY_GET(vnya, j+num_vertices*i);
              cnbuffer[j].norm.delta_z = FTN_REAL_ARRAY_GET(vnza, j+num_vertices*i);
            }
            memcpy(tp, &cnbuffer[0],
                   num_vertices * sizeof(Pptconorm3));
            tp += num_vertices * sizeof(Pptconorm3);
            break;

          default:
            break;
          }
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * pfas3d
 *
 * DESCR:       fill area set
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pfasd)(
                      FTN_INTEGER(fflag1),
                      FTN_INTEGER(eflag1),
                      FTN_INTEGER(vflag1),
                      FTN_INTEGER(colr_type1),
                      FTN_INTEGER(ncc1),
                      FTN_INTEGER(fcoli1),
                      FTN_REAL_ARRAY(fcolr),
                      FTN_REAL(fnx),
                      FTN_REAL(fny),
                      FTN_REAL(fnz), //Facet normal data(MC)
                      FTN_INTEGER(fdlen),
                      FTN_REAL_ARRAY(fdata), // fdata[fdlen]
                      FTN_INTEGER(npl), // Number of points list
                      FTN_INTEGER_ARRAY(ixa), // ixa[npl] end indices for point lists
                      FTN_INTEGER_ARRAY(edata), // edge data
                      FTN_REAL_ARRAY(pxa),
                      FTN_REAL_ARRAY(pya),
                      FTN_INTEGER_ARRAY(vcoli),
                      FTN_REAL_ARRAY(vcolr),
                      FTN_REAL_ARRAY(vnxa),
                      FTN_REAL_ARRAY(vnya),
                      FTN_REAL_ARRAY(vnza),
                      FTN_INTEGER(vdn),
                      FTN_REAL_ARRAY(vdata)
                      )
{
  Pint fflag = FTN_INTEGER_GET(fflag1);
  Pint eflag = FTN_INTEGER_GET(eflag1);
  Pint vflag = FTN_INTEGER_GET(vflag1);
  Pint colr_type = FTN_INTEGER_GET(colr_type1);
  Pint ncc = FTN_INTEGER_GET(ncc1); //Number of components of colour value FCOLR (NCC)
  Pint nfa = FTN_INTEGER_GET(npl);

  Phg_args_add_el args;
  Pint i, j;
  unsigned facet_size, vertex_size;
  Pint *data;
  char *tp;
  Pint num_vertices;
  Ppoint3 vbuffer[MAX_ARRAY_SIZE];
  Pptco3   cbuffer[MAX_ARRAY_SIZE];
  Pptnorm3 nbuffer[MAX_ARRAY_SIZE];
  Pptconorm3 cnbuffer[MAX_ARRAY_SIZE];

  Pcoval coval;
  Pvec3 vec3;
  Pconorm3 conorm;

#ifdef DEBUG
  printf("DEBUG: PFASD called. NFA is set to %d\n", nfa);
#endif

  if (phg_entry_check(PHG_ERH, 0, Pfn_fill_area_set3_data)) {
    if (PSL_STRUCT_STATE(PHG_PSL) != PSTRUCT_ST_STOP) {
      ERR_REPORT(PHG_ERH, ERR5);
    }
    else {
      switch (fflag) {
      case PFACET_COLOUR:
        facet_size = sizeof(Pcoval);
        break;

      case PFACET_NORMAL:
        facet_size = sizeof(Pvec3);
        break;

      case PFACET_COLOUR_NORMAL:
        facet_size = sizeof(Pconorm3);
        break;

      default:
        facet_size = 0;
        break;
      }
      switch (vflag) {
      case PVERT_COORD:
        vertex_size = sizeof(Ppoint3);
        break;

      case PVERT_COORD_COLOUR:
        vertex_size = sizeof(Pptco3);
        break;

      case PVERT_COORD_NORMAL:
        vertex_size = sizeof(Pptnorm3);
        break;

      case PVERT_COORD_COLOUR_NORMAL:
        vertex_size = sizeof(Pptconorm3);
        break;

      default:
        vertex_size = 0;
        break;
      }
      args.el_type = PELEM_FILL_AREA_SET3_DATA;
      args.el_size = 5 * sizeof(Pint) + facet_size;

      if (eflag == PEDGE_VISIBILITY) {
        for (i = 0; i < nfa; i++) {
          args.el_size += sizeof(Pint);   /* Pint num_edges */
          args.el_size += ixa[i] * sizeof(Pedge_flag);
        }
      }

      for (i = 0; i < nfa; i++) {
        args.el_size += sizeof(Pint);      /* Pint num_vertices */
        args.el_size += ixa[i] * vertex_size;
      }

      if (!PHG_SCRATCH_SPACE(&PHG_SCRATCH, args.el_size)) {
        ERR_REPORT(PHG_ERH, ERR900);
      }
      else {
        args.el_data = PHG_SCRATCH.buf;
        data = (Pint *) args.el_data;
        data[0] = fflag;
        data = &data[1];
        data[0] = eflag;
        data = &data[1];
        data[0] = vflag;
        data = &data[1];
        data[0] = colr_type;
        tp = (char *) &data[1];
        switch(fflag) {
        case PFACET_COLOUR:
          coval.ind = FTN_INTEGER_GET(fcoli1);//Facet colour index
          memcpy(tp, &coval, sizeof(Pcoval));
          tp += sizeof(Pcoval);
          break;

        case PFACET_NORMAL:
          vec3.delta_x = FTN_REAL_ARRAY_GET(fcolr, 0);//Facet colour values
          vec3.delta_y = FTN_REAL_ARRAY_GET(fcolr, 1);
          vec3.delta_z = FTN_REAL_ARRAY_GET(fcolr, 2);
          memcpy(tp, &vec3, sizeof(Pvec3));
          tp += sizeof(Pvec3);
          break;

        case PFACET_COLOUR_NORMAL:
          conorm.colr.ind = FTN_INTEGER_GET(fcoli1);
          conorm.norm.delta_x = FTN_REAL_GET(fnx);
          conorm.norm.delta_y = FTN_REAL_GET(fny);
          conorm.norm.delta_z = FTN_REAL_GET(fnz);
          memcpy(tp, &conorm, sizeof(Pconorm3));
          tp += sizeof(Pconorm3);
          break;

        default:
          break;
        }

        data = (Pint *) tp;
        data[0] = nfa;
        tp = (char *) &data[1];
        if (eflag == PEDGE_VISIBILITY) {
          for (i = 0; i < nfa; i++) {
            num_vertices = ixa[i];
            data = (Pint *) tp;
            data[0] = num_vertices;
            tp = (char *) &data[1];
            memcpy(tp, &edata[0],
                   sizeof(Pedge_flag) * num_vertices);
            tp += sizeof(Pedge_flag) * num_vertices;
          }
        }
        for (i = 0; i < nfa; i++) {
          num_vertices = ixa[i];
          data = (Pint *) tp;
          data[0] = num_vertices;
          tp = (char *) &data[1];

          switch (vflag) {
          case PVERT_COORD:
            for (j=0; j<num_vertices; j++){
              vbuffer[j].x = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              vbuffer[j].y = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              vbuffer[j].z = 0.0;
            }
            memcpy(tp, &vbuffer[0],
                   num_vertices * sizeof(Ppoint3));
            tp += num_vertices * sizeof(Ppoint3);
            break;

          case PVERT_COORD_COLOUR:
            for (j=0; j<num_vertices; j++){
              cbuffer[j].colr.ind = FTN_INTEGER_ARRAY_GET(vcoli, j+num_vertices*i);
              cbuffer[j].point.x  = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              cbuffer[j].point.y  = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              cbuffer[j].point.z  = 0.0;
            }
            memcpy(tp, &cbuffer[0],
                   num_vertices * sizeof(Pptco3));
            tp += num_vertices * sizeof(Pptco3);
            break;

          case PVERT_COORD_NORMAL:
            for (j=0; j<num_vertices; j++){
              nbuffer[j].point.x = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              nbuffer[j].point.y = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              nbuffer[j].point.z = 0.0;
              nbuffer[j].norm.delta_x = FTN_REAL_ARRAY_GET(vnxa, j+num_vertices*i);
              nbuffer[j].norm.delta_y = FTN_REAL_ARRAY_GET(vnya, j+num_vertices*i);
              nbuffer[j].norm.delta_z = FTN_REAL_ARRAY_GET(vnza, j+num_vertices*i);
            }
            memcpy(tp, &nbuffer[0],
                   num_vertices * sizeof(Pptnorm3));
            tp += num_vertices * sizeof(Pptnorm3);
            break;

          case PVERT_COORD_COLOUR_NORMAL:
            for (j=0; j<num_vertices; j++){
              cnbuffer[j].colr.ind    = FTN_INTEGER_ARRAY_GET(vcoli, j+num_vertices*i);
              cnbuffer[j].point.x = FTN_REAL_ARRAY_GET(pxa, j+num_vertices*i);
              cnbuffer[j].point.y = FTN_REAL_ARRAY_GET(pya, j+num_vertices*i);
              cnbuffer[j].point.z = 0.0;
              cnbuffer[j].norm.delta_x = FTN_REAL_ARRAY_GET(vnxa, j+num_vertices*i);
              cnbuffer[j].norm.delta_y = FTN_REAL_ARRAY_GET(vnya, j+num_vertices*i);
              cnbuffer[j].norm.delta_z = FTN_REAL_ARRAY_GET(vnza, j+num_vertices*i);
            }
            memcpy(tp, &cnbuffer[0],
                   num_vertices * sizeof(Pptconorm3));
            tp += num_vertices * sizeof(Pptconorm3);
            break;

          default:
            break;
          }
        }
        phg_add_el(PHG_CSS, &args);
      }
    }
  }
}

/*******************************************************************************
 * psfcm
 *
 * DESCR:       set facet culling mode
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psfcm)(
                      FTN_INTEGER(imode)
                      )
{
  Pint cul_mode = FTN_INTEGER_GET(imode);
  pset_face_cull_mode(cul_mode);
}

/*******************************************************************************
 * psfdm
 *
 * DESCR:       set facet distinguishing mode
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psfdm)(
                      FTN_INTEGER(mode)
                      )
{
  Pint dist_mode = FTN_INTEGER_GET(mode);
  pset_face_disting_mode(dist_mode);
}

/*******************************************************************************
 * psalch
 *
 * DESCR:       set alpha channel
 * RETURNS:   N/A
 * NOTE: Extemsion
 */
FTN_SUBROUTINE(psalch)(
                       FTN_REAL(alpha_channel)
                       )
{
  Pfloat alpha = FTN_REAL_GET(alpha_channel);
  pset_alpha_channel(alpha);
}

/*******************************************************************************
 * psbis
 *
 * DESCR:       set back interior style
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psbis)(
                      FTN_INTEGER(istyle)
                      )
{
  Pint_style interior_style = (Pint_style) FTN_INTEGER_GET(istyle);
#ifdef DEBUG
  printf("DEBUG: PBIS interior style called to %d\n", (int)interior_style);
#endif
  pset_back_int_style(interior_style);
}

/*******************************************************************************
 * psbic
 *
 * DESCR:       set back interior color
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psbic)(
                      FTN_INTEGER(ityp),
                      FTN_INTEGER(incc),
                      FTN_INTEGER(icol),
                      FTN_REAL_ARRAY(rcolr)
                      )
{
  Pint colr_typ = FTN_INTEGER_GET(ityp);
  Pint colr_ind = FTN_INTEGER_GET(icol);
  Pint ncc = FTN_INTEGER_GET(incc);
  Pgcolr colr;
#ifdef DEBUG
  printf("DEBUG: pset interior color index set to %d\n", colr_ind);
#endif
  if (colr_typ == PINDIRECT){
    colr.type = PINDIRECT;
    colr.val.ind = colr_ind;
  } else {
    colr.type = PMODEL_RGB;
    if (ncc == 3){
      colr.val.general.x = FTN_REAL_ARRAY_GET(rcolr, 0);
      colr.val.general.y = FTN_REAL_ARRAY_GET(rcolr, 1);
      colr.val.general.z = FTN_REAL_ARRAY_GET(rcolr, 2);
    }
    else {
      printf("PSBCI: not enough color values provided. Ignoring function.\n");
    }
  }
  pset_back_int_colr(&colr);
}

/*******************************************************************************
 * psii
 *
 * DESCR:       set interior index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psii)(
                     FTN_INTEGER(ii)
                     )
{
  Pint iindex = FTN_INTEGER_GET(ii);
  pset_int_ind(iindex);
}

/*******************************************************************************
 * pres
 *
 * DESCR:       remove name set
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(pres)(
                     FTN_INTEGER(num),
                     FTN_INTEGER_ARRAY(set)
                     )
{
  int n;
  Pint_list names;
  names.num_ints = FTN_INTEGER_GET(num);
  names.ints = &FTN_INTEGER_ARRAY_GET(set, 0);
  premove_names_set(&names);
}

/*******************************************************************************
 * psisi
 *
 * DESCR:       set interior style index
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psisi)(
                      FTN_INTEGER(ii)
                      )
{
  Pint iindex = FTN_INTEGER_GET(ii);
  pset_int_style_ind(iindex);
}
/*******************************************************************************
 * psmcli
 *
 * DESCR:       set modelling clipping indicator
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psmcli)(
                       FTN_INTEGER(mclipi)
                       )
{
  Pint clipi = FTN_INTEGER_GET(mclipi);
  pset_model_clip_ind(clipi);
}

/*******************************************************************************
 * psmcv3
 *
 * DESCR:       set modelling clipping volume 3
 * RETURNS:   N/A
 */
FTN_SUBROUTINE(psmcv3)(
                       FTN_INTEGER(op),
                       FTN_INTEGER(nhalfs),
                       FTN_REAL_ARRAY(halfsp)
                       )
{
  Pint iop = FTN_INTEGER_GET(op);
  Pint num = FTN_INTEGER_GET(nhalfs);
  Phalf_space_list3 spacelist;
  Phalf_space3 list[num];
  int i;

  spacelist.num_half_spaces = num;
  for (i=0; i<num; i++){
    list[i].point.x = FTN_REAL_ARRAY_GET(halfsp, 0 + i*6);
    list[i].point.y = FTN_REAL_ARRAY_GET(halfsp, 1 + i*6);
    list[i].point.z = FTN_REAL_ARRAY_GET(halfsp, 2 + i*6);
    list[i].norm.delta_x = FTN_REAL_ARRAY_GET(halfsp, 3 + i*6);
    list[i].norm.delta_y = FTN_REAL_ARRAY_GET(halfsp, 4 + i*6);
    list[i].norm.delta_z = FTN_REAL_ARRAY_GET(halfsp, 5 + i*6);
  }
  spacelist.half_spaces = &list[0];
  pset_model_clip_vol3(iop, spacelist);
}
