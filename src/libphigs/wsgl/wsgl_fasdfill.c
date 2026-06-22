/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (c) 2014 Surplus Users Ham Society
*             (c) 2022-2025 CERN
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

/* Derived from the 3d version */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"
#include "private/fasd3P.h"

/*******************************************************************************
 * priv_fill_area3_points
 *
 * DESCR:	Draw fill area with point data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area_points(
                                  Pint num_vertices,
                                  Ppoint3 *points
                                  )
{
  Pint i;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;
  int normal_indices[MAX_VERTICES];
  int n_normals = 0;

  glBegin(GL_POLYGON);
  for (i = 0; i < num_vertices; i++) {
    glVertex3f(points[i].x,
               points[i].y,
               0.0);
    if (record_geom && record_geom_fill){
      vertex_indices[n_vertices] = wsgl_add_vertex(points[i].x,
                                                   points[i].y,
                                                   0.0);
      n_vertices ++;
      normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                  current_normal.y,
                                                  current_normal.z);
      n_normals ++;
    }
  }
  if (record_geom && record_geom_fill){
    wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
  }
  glEnd();
}

/*******************************************************************************
 * priv_fill_area3_ptcolrs
 *
 * DESCR:	Draw fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area_ptcolrs(
                                   Ws *ws,
                                   Pint colr_type,
                                   Pint num_vertices,
                                   Pptco3 *ptcolrs,
                                   Ws_attr_st *ast
                                   )
{
  Pint i;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;
  int normal_indices[MAX_VERTICES];
  int n_normals = 0;

  glBegin(GL_POLYGON);
  for (i = 0; i < num_vertices; i++) {
    wsgl_setup_int_colr(ws, colr_type, &ptcolrs[i].colr, ast);
    glVertex3f(ptcolrs[i].point.x,
               ptcolrs[i].point.y,
               0.0);
    if (record_geom && record_geom_fill){
      vertex_indices[n_vertices] = wsgl_add_vertex(ptcolrs[i].point.x,
                                                   ptcolrs[i].point.y,
                                                   0.0);
      n_vertices ++;
      normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                  current_normal.y,
                                                  current_normal.z);
      n_normals ++;
    }
  }
  if (record_geom && record_geom_fill){
    wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
  }
  glEnd();
}

/*******************************************************************************
 * priv_back_area_ptcolrs
 *
 * DESCR:	Draw back area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void priv_back_area_ptcolrs(
                                   Ws *ws,
                                   Pint colr_type,
                                   Pint num_vertices,
                                   Pptco3 *ptcolrs,
                                   Ws_attr_st *ast
                                   )
{
  Pint i;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;
  int normal_indices[MAX_VERTICES];
  int n_normals = 0;

  glBegin(GL_POLYGON);
  for (i = 0; i < num_vertices; i++) {
    wsgl_setup_back_int_colr(ws, colr_type, &ptcolrs[i].colr, ast);
    glVertex3f(ptcolrs[i].point.x,
               ptcolrs[i].point.y,
               0.0);
    if (record_geom && record_geom_fill){
      vertex_indices[n_vertices] = wsgl_add_vertex(ptcolrs[i].point.x,
                                                   ptcolrs[i].point.y,
                                                   0.0);
      n_vertices ++;
      normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                  current_normal.y,
                                                  current_normal.z);
      n_normals ++;
    }
  }
  if (record_geom && record_geom_fill){
    wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
  }
  glEnd();
}

/*******************************************************************************
 * priv_fill_area_ptnorms
 *
 * DESCR:	Draw fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area_ptnorms(
                                   Pint num_vertices,
                                   Pptnorm3 *ptnorms
                                   )
{
  Pint i;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;
  int normal_indices[MAX_VERTICES];
  int n_normals = 0;

  glBegin(GL_POLYGON);
  for (i = 0; i < num_vertices; i++) {
    glNormal3f(ptnorms[i].norm.delta_x,
               ptnorms[i].norm.delta_y,
               0.0);
    wsgl_set_current_normal(ptnorms[i].norm.delta_x,
                            ptnorms[i].norm.delta_y,
                            0.0);
    glVertex3f(ptnorms[i].point.x,
               ptnorms[i].point.y,
               0.0);
    if (record_geom && record_geom_fill){
      vertex_indices[n_vertices] = wsgl_add_vertex(ptnorms[i].point.x,
                                                   ptnorms[i].point.y,
                                                   0.0);
      n_vertices ++;
      normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                  current_normal.y,
                                                  current_normal.z);
      n_normals ++;
    }
  }
  if (record_geom && record_geom_fill){
    wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
  }
  glEnd();
}

/*******************************************************************************
 * priv_fill_area_ptconorms
 *
 * DESCR:	Draw fill area with point, colour and normal data 3D
 *              helper function
 * RETURNS:	N/A
 */

static void priv_fill_area_ptconorms(
                                     Ws *ws,
                                     Pint colr_type,
                                     Pint num_vertices,
                                     Pptconorm3 *ptconorms,
                                     Ws_attr_st *ast
                                     )
{
  Pint i;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;
  int normal_indices[MAX_VERTICES];
  int n_normals = 0;

  glBegin(GL_POLYGON);
  for (i = 0; i < num_vertices; i++) {
    wsgl_setup_int_colr(ws, colr_type, &ptconorms[i].colr, ast);
    glNormal3f(ptconorms[i].norm.delta_x,
               ptconorms[i].norm.delta_y,
               0.0);
    wsgl_set_current_normal(ptconorms[i].norm.delta_x,
                            ptconorms[i].norm.delta_y,
                            0.0);
    glVertex3f(ptconorms[i].point.x,
               ptconorms[i].point.y,
               0.0);
    if (record_geom && record_geom_fill){
      vertex_indices[n_vertices] = wsgl_add_vertex(ptconorms[i].point.x,
                                                   ptconorms[i].point.y,
                                                   0.0);
      n_vertices ++;
      normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                  current_normal.y,
                                                  current_normal.z);
      n_normals ++;
    }
  }
  if (record_geom && record_geom_fill){
    wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
  }
 glEnd();
}

/*******************************************************************************
 * priv_back_area_ptconorms
 *
 * DESCR:	Draw back area with point, colour and normal data 3D
 *              helper function
 * RETURNS:	N/A
 */

static void priv_back_area_ptconorms(
                                     Ws *ws,
                                     Pint colr_type,
                                     Pint num_vertices,
                                     Pptconorm3 *ptconorms,
                                     Ws_attr_st *ast
                                     )
{
  Pint i;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;
  int normal_indices[MAX_VERTICES];
  int n_normals = 0;

  glBegin(GL_POLYGON);
  for (i = 0; i < num_vertices; i++) {
    wsgl_setup_back_int_colr(ws, colr_type, &ptconorms[i].colr, ast);
    glNormal3f(ptconorms[i].norm.delta_x,
               ptconorms[i].norm.delta_y,
               0.0);
    wsgl_set_current_normal(ptconorms[i].norm.delta_x,
                            ptconorms[i].norm.delta_y,
                            0.0);
    glVertex3f(ptconorms[i].point.x,
               ptconorms[i].point.y,
               0.0);
    if (record_geom && record_geom_fill){
      vertex_indices[n_vertices] = wsgl_add_vertex(ptconorms[i].point.x,
                                                   ptconorms[i].point.y,
                                                   0.0);
      n_vertices ++;
      normal_indices[n_normals] = wsgl_add_normal(current_normal.x,
                                                  current_normal.y,
                                                  current_normal.z);
      n_normals ++;
    }
  }
  if (record_geom && record_geom_fill){
    wsgl_add_geometry(GEOM_FACE, vertex_indices, normal_indices, n_vertices);
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_fill_area_set_data_front
 *
 * DESCR:	Draw fill area set with data 3D front faces
 * RETURNS:	N/A
 */

void wsgl_fill_area_set_data_front(
                                   Ws *ws,
                                   void *pdata,
                                   Ws_attr_st *ast
                                   )
{
  Pint i;
  Pfasd3 fasd3;
  Pedge_data_list edata;
  Pfacet_vdata_list3 vdata;
  Pcoval colr;
  Pvec3 norm;

  fasd3.edata = &edata;
  fasd3.vdata = &vdata;
  fasd3_head(&fasd3, pdata);

  glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
  glEnable(GL_POLYGON_OFFSET_FILL);
  glEnable(GL_POLYGON_OFFSET_LINE);
  wsgl_setup_int_attr_nocol(ws, ast);

  switch (fasd3.vflag) {
  case PVERT_COORD:
    if (fasd3.fflag == PFACET_COLOUR_NORMAL) {
      wsgl_setup_int_colr(ws,
                          fasd3.colr_type,
                          &fasd3.fdata.conorm.colr,
                          ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else if (fasd3.fflag == PFACET_NORMAL) {
      wsgl_colr_from_gcolr(&colr, wsgl_get_int_colr(ast), ws->current_colour_model);
      wsgl_setup_int_colr(ws, fasd3.colr_type, &colr, ast);
      glNormal3f(fasd3.fdata.norm.delta_x,
                 fasd3.fdata.norm.delta_y,
                 fasd3.fdata.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else if (fasd3.fflag == PFACET_COLOUR) {
      wsgl_setup_int_colr(ws,
                          fasd3.colr_type,
                          &fasd3.fdata.colr,
                          ast);
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      wsgl_colr_from_gcolr(&colr, wsgl_get_int_colr(ast), ws->current_colour_model);
      wsgl_setup_int_colr(ws, fasd3.colr_type, &colr, ast);
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    break;

  case PVERT_COORD_COLOUR:
    if (fasd3.fflag == PFACET_NORMAL) {
      glNormal3f(fasd3.fdata.norm.delta_x,
                 fasd3.fdata.norm.delta_y,
                 fasd3.fdata.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_ptcolrs(ws,
                               fasd3.colr_type,
                               fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptcolrs,
                               ast);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_ptcolrs(ws,
                               fasd3.colr_type,
                               fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptcolrs,
                               ast);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    break;

  case PVERT_COORD_NORMAL:
    if (fasd3.fflag == PFACET_COLOUR) {
      wsgl_setup_int_colr(ws,
                          fasd3.colr_type,
                          &fasd3.fdata.colr,
                          ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_ptnorms(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptnorms);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      wsgl_colr_from_gcolr(&colr, wsgl_get_int_colr(ast), ws->current_colour_model);
      wsgl_setup_int_colr(ws, fasd3.colr_type, &colr, ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_ptnorms(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptnorms);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    break;

  case PVERT_COORD_COLOUR_NORMAL:
    for (i = 0; i < fasd3.nfa; i++) {
      priv_fill_area_ptconorms(ws,
                               fasd3.colr_type,
                               fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptconorms,
                               ast);

      /* Advance to next set of data */
      fasd3_next_vdata3(&fasd3);
      if (fasd3.eflag == PEDGE_VISIBILITY) {
        fasd3_next_edata(&fasd3);
      }
    }
    break;

  default:
    break;
  }

  glDisable(GL_POLYGON_OFFSET_LINE);
  glDisable(GL_POLYGON_OFFSET_FILL);
}

/*******************************************************************************
 * wsgl_fill_area_set_data_back
 *
 * DESCR:	Draw fill area set with data 3D back faces
 * RETURNS:	N/A
 */

void wsgl_fill_area_set_data_back(
                                  Ws *ws,
                                  void *pdata,
                                  Ws_attr_st *ast
                                  )
{
  Pint i;
  Pfasd3 fasd3;
  Pedge_data_list edata;
  Pfacet_vdata_list3 vdata;
  Pcoval colr;
  Pvec3 norm;

  fasd3.edata = &edata;
  fasd3.vdata = &vdata;
  fasd3_head(&fasd3, pdata);

  glPolygonOffset(WS_FILL_AREA_OFFSET, wsgl_get_edge_width(ast));
  glEnable(GL_POLYGON_OFFSET_FILL);
  glEnable(GL_POLYGON_OFFSET_LINE);
  wsgl_setup_back_int_attr_nocol(ws, ast);

  switch (fasd3.vflag) {
  case PVERT_COORD:
    if (fasd3.fflag == PFACET_COLOUR_NORMAL) {
      wsgl_setup_back_int_colr(ws,
                               fasd3.colr_type,
                               &fasd3.fdata.conorm.colr,
                               ast);
      glNormal3f(fasd3.fdata.conorm.norm.delta_x,
                 fasd3.fdata.conorm.norm.delta_y,
                 fasd3.fdata.conorm.norm.delta_z);
      wsgl_set_current_normal(fasd3.fdata.conorm.norm.delta_x,
                              fasd3.fdata.conorm.norm.delta_y,
                              fasd3.fdata.conorm.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else if (fasd3.fflag == PFACET_NORMAL) {
      wsgl_colr_from_gcolr(&colr, wsgl_get_back_int_colr(ast), ws->current_colour_model);
      wsgl_setup_back_int_colr(ws, fasd3.colr_type, &colr, ast);
      glNormal3f(fasd3.fdata.norm.delta_x,
                 fasd3.fdata.norm.delta_y,
                 fasd3.fdata.norm.delta_z);
      wsgl_set_current_normal(fasd3.fdata.norm.delta_x,
                              fasd3.fdata.norm.delta_y,
                              fasd3.fdata.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else if (fasd3.fflag == PFACET_COLOUR) {
      wsgl_setup_back_int_colr(ws,
                               fasd3.colr_type,
                               &fasd3.fdata.colr,
                               ast);
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      wsgl_set_current_normal(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      wsgl_colr_from_gcolr(&colr, wsgl_get_back_int_colr(ast), ws->current_colour_model);
      wsgl_setup_back_int_colr(ws, fasd3.colr_type, &colr, ast);
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      wsgl_set_current_normal(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    break;

  case PVERT_COORD_COLOUR:
    if (fasd3.fflag == PFACET_NORMAL) {
      glNormal3f(fasd3.fdata.norm.delta_x,
                 fasd3.fdata.norm.delta_y,
                 fasd3.fdata.norm.delta_z);
      wsgl_set_current_normal(fasd3.fdata.norm.delta_x,
                              fasd3.fdata.norm.delta_y,
                              fasd3.fdata.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_back_area_ptcolrs(ws,
                               fasd3.colr_type,
                               fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptcolrs,
                               ast);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      wsgl_set_current_normal(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_back_area_ptcolrs(ws,
                               fasd3.colr_type,
                               fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptcolrs,
                               ast);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    break;

  case PVERT_COORD_NORMAL:
    if (fasd3.fflag == PFACET_COLOUR) {
      wsgl_setup_back_int_colr(ws,
                               fasd3.colr_type,
                               &fasd3.fdata.colr,
                               ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_ptnorms(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptnorms);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      wsgl_colr_from_gcolr(&colr, wsgl_get_back_int_colr(ast), ws->current_colour_model);
      wsgl_setup_back_int_colr(ws, fasd3.colr_type, &colr, ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area_ptnorms(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptnorms);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    break;

  case PVERT_COORD_COLOUR_NORMAL:
    for (i = 0; i < fasd3.nfa; i++) {
      priv_back_area_ptconorms(ws,
                               fasd3.colr_type,
                               fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptconorms,
                               ast);

      /* Advance to next set of data */
      fasd3_next_vdata3(&fasd3);
      if (fasd3.eflag == PEDGE_VISIBILITY) {
        fasd3_next_edata(&fasd3);
      }
    }
    break;

  default:
    break;
  }

  glDisable(GL_POLYGON_OFFSET_LINE);
  glDisable(GL_POLYGON_OFFSET_FILL);
}
