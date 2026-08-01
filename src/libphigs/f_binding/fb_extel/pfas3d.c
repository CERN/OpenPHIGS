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

/**
 * \file pfas3d.c
 * \brief fill area set
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

