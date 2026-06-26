/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
*             (C) 2022-2025 CERN
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

static void priv_fill_area3_points(
                                   Pint num_vertices,
                                   Ppoint3 *points
                                   )
{
  Pint i;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(num_vertices * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, num_vertices * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < num_vertices; i++) {
    t_verts[i].pos[0] = points[i].x;
    t_verts[i].pos[1] = points[i].y;
    t_verts[i].pos[2] = points[i].z;
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom && record_geom_fill);
  free(t_verts);
}

/*******************************************************************************
 * priv_fill_area3_ptcolrs
 *
 * DESCR:	Draw fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void cb_ptcolrs(void *data) {
    Wsgl_tess_vertex *v = (Wsgl_tess_vertex *)data;
    wsgl_setup_int_colr(v->ws, v->colr_type, &v->colr, v->ast);
}

static void priv_fill_area3_ptcolrs(
                                    Ws *ws,
                                    Pint colr_type,
                                    Pint num_vertices,
                                    Pptco3 *ptcolrs,
                                    Ws_attr_st *ast
                                    )
{
  Pint i;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(num_vertices * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, num_vertices * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < num_vertices; i++) {
    t_verts[i].pos[0] = ptcolrs[i].point.x;
    t_verts[i].pos[1] = ptcolrs[i].point.y;
    t_verts[i].pos[2] = ptcolrs[i].point.z;
    t_verts[i].apply_cb = cb_ptcolrs;
    t_verts[i].ws = ws;
    t_verts[i].colr_type = colr_type;
    t_verts[i].colr = ptcolrs[i].colr;
    t_verts[i].ast = ast;
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom && record_geom_fill);
  free(t_verts);
}

/*******************************************************************************
 * priv_back_area3_ptcolrs
 *
 * DESCR:	Draw back area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void cb_back_ptcolrs(void *data) {
    Wsgl_tess_vertex *v = (Wsgl_tess_vertex *)data;
    wsgl_setup_back_int_colr(v->ws, v->colr_type, &v->colr, v->ast);
}

static void priv_back_area3_ptcolrs(
                                    Ws *ws,
                                    Pint colr_type,
                                    Pint num_vertices,
                                    Pptco3 *ptcolrs,
                                    Ws_attr_st *ast
                                    )
{
  Pint i;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(num_vertices * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, num_vertices * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < num_vertices; i++) {
    t_verts[i].pos[0] = ptcolrs[i].point.x;
    t_verts[i].pos[1] = ptcolrs[i].point.y;
    t_verts[i].pos[2] = ptcolrs[i].point.z;
    t_verts[i].apply_cb = cb_back_ptcolrs;
    t_verts[i].ws = ws;
    t_verts[i].colr_type = colr_type;
    t_verts[i].colr = ptcolrs[i].colr;
    t_verts[i].ast = ast;
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom && record_geom_fill);
  free(t_verts);
}

/*******************************************************************************
 * priv_fill_area3_ptnorms
 *
 * DESCR:	Draw fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void priv_fill_area3_ptnorms(
                                    Pint num_vertices,
                                    Pptnorm3 *ptnorms
                                    )
{
  Pint i;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(num_vertices * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, num_vertices * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < num_vertices; i++) {
    t_verts[i].pos[0] = ptnorms[i].point.x;
    t_verts[i].pos[1] = ptnorms[i].point.y;
    t_verts[i].pos[2] = ptnorms[i].point.z;
    t_verts[i].has_norm = 1;
    t_verts[i].norm[0] = ptnorms[i].norm.delta_x;
    t_verts[i].norm[1] = ptnorms[i].norm.delta_y;
    t_verts[i].norm[2] = ptnorms[i].norm.delta_z;
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom && record_geom_fill);
  free(t_verts);
}

/*******************************************************************************
 * priv_fill_area3_ptconorms
 *
 * DESCR:	Draw fill area with point, colour and normal data 3D
 *              helper function
 * RETURNS:	N/A
 */

static void cb_ptconorms(void *data) {
    Wsgl_tess_vertex *v = (Wsgl_tess_vertex *)data;
    wsgl_setup_int_colr(v->ws, v->colr_type, &v->colr, v->ast);
}

static void priv_fill_area3_ptconorms(
                                      Ws *ws,
                                      Pint colr_type,
                                      Pint num_vertices,
                                      Pptconorm3 *ptconorms,
                                      Ws_attr_st *ast
                                      )
{
  Pint i;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(num_vertices * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, num_vertices * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < num_vertices; i++) {
    t_verts[i].pos[0] = ptconorms[i].point.x;
    t_verts[i].pos[1] = ptconorms[i].point.y;
    t_verts[i].pos[2] = ptconorms[i].point.z;
    t_verts[i].has_norm = 1;
    t_verts[i].norm[0] = ptconorms[i].norm.delta_x;
    t_verts[i].norm[1] = ptconorms[i].norm.delta_y;
    t_verts[i].norm[2] = ptconorms[i].norm.delta_z;
    t_verts[i].apply_cb = cb_ptconorms;
    t_verts[i].ws = ws;
    t_verts[i].colr_type = colr_type;
    t_verts[i].colr = ptconorms[i].colr;
    t_verts[i].ast = ast;
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom && record_geom_fill);
  free(t_verts);
}

/*******************************************************************************
 * priv_back_area3_ptconorms
 *
 * DESCR:	Draw back area with point, colour and normal data 3D
 *              helper function
 * RETURNS:	N/A
 */

static void cb_back_ptconorms(void *data) {
    Wsgl_tess_vertex *v = (Wsgl_tess_vertex *)data;
    wsgl_setup_back_int_colr(v->ws, v->colr_type, &v->colr, v->ast);
}

static void priv_back_area3_ptconorms(
                                      Ws *ws,
                                      Pint colr_type,
                                      Pint num_vertices,
                                      Pptconorm3 *ptconorms,
                                      Ws_attr_st *ast
                                      )
{
  Pint i;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(num_vertices * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, num_vertices * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < num_vertices; i++) {
    t_verts[i].pos[0] = ptconorms[i].point.x;
    t_verts[i].pos[1] = ptconorms[i].point.y;
    t_verts[i].pos[2] = ptconorms[i].point.z;
    t_verts[i].has_norm = 1;
    t_verts[i].norm[0] = ptconorms[i].norm.delta_x;
    t_verts[i].norm[1] = ptconorms[i].norm.delta_y;
    t_verts[i].norm[2] = ptconorms[i].norm.delta_z;
    t_verts[i].apply_cb = cb_back_ptconorms;
    t_verts[i].ws = ws;
    t_verts[i].colr_type = colr_type;
    t_verts[i].colr = ptconorms[i].colr;
    t_verts[i].ast = ast;
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom && record_geom_fill);
  free(t_verts);
}

/*******************************************************************************
 * wsgl_fill_area_set3_data_front
 *
 * DESCR:	Draw fill area set with data 3D front faces
 * RETURNS:	N/A
 */

void wsgl_fill_area_set3_data_front(
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
  Pint colr_type;
  Pgcolr *gcolr;

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
      glNormal3f(fasd3.fdata.conorm.norm.delta_x,
                 fasd3.fdata.conorm.norm.delta_y,
                 fasd3.fdata.conorm.norm.delta_z);
      wsgl_set_current_normal(fasd3.fdata.conorm.norm.delta_x,
                              fasd3.fdata.conorm.norm.delta_y,
                              fasd3.fdata.conorm.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_points(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else if (fasd3.fflag == PFACET_NORMAL) {
      colr_type = wsgl_get_int_colr(ast)->type;
      wsgl_colr_from_gcolr(&colr, wsgl_get_int_colr(ast), ws->current_colour_model);
      wsgl_setup_int_colr(ws, colr_type, &colr, ast);
      glNormal3f(fasd3.fdata.norm.delta_x,
                 fasd3.fdata.norm.delta_y,
                 fasd3.fdata.norm.delta_z);
      wsgl_set_current_normal(fasd3.fdata.norm.delta_x,
                              fasd3.fdata.norm.delta_y,
                              fasd3.fdata.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_points(fasd3.vdata->num_vertices,
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
      wsgl_set_current_normal(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_points(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      gcolr = wsgl_get_int_colr(ast);
      wsgl_colr_from_gcolr(&colr, gcolr, ws->current_colour_model);
      wsgl_setup_int_colr(ws, gcolr->type, &colr, ast);
#ifdef DEBUGA
      printf("fasd3fill: Setup color type %d: %f %f %f %f\n",
             gcolr->type,
             colr.direct.rgba.red,
             colr.direct.rgba.green,
             colr.direct.rgba.blue,
             colr.direct.rgba.alpha
      );
#endif
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      wsgl_set_current_normal(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_points(fasd3.vdata->num_vertices,
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
        priv_fill_area3_ptcolrs(ws,
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
        priv_fill_area3_ptcolrs(ws,
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
        priv_fill_area3_ptnorms(fasd3.vdata->num_vertices,
                                fasd3.vdata->vertex_data.ptnorms);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      colr_type = wsgl_get_int_colr(ast)->type;
      wsgl_colr_from_gcolr(&colr, wsgl_get_int_colr(ast), ws->current_colour_model);
      wsgl_setup_int_colr(ws, colr_type, &colr, ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_ptnorms(fasd3.vdata->num_vertices,
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
      priv_fill_area3_ptconorms(ws,
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
 * wsgl_fill_area_set3_data_back
 *
 * DESCR:	Draw fill area set with data 3D back faces
 * RETURNS:	N/A
 */

void wsgl_fill_area_set3_data_back(
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
  Pint colr_type;

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
        priv_fill_area3_points(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else if (fasd3.fflag == PFACET_NORMAL) {
      colr_type = wsgl_get_back_int_colr(ast)->type;
      wsgl_colr_from_gcolr(&colr, wsgl_get_back_int_colr(ast), ws->current_colour_model);
      wsgl_setup_back_int_colr(ws, colr_type, &colr, ast);
      glNormal3f(fasd3.fdata.norm.delta_x,
                 fasd3.fdata.norm.delta_y,
                 fasd3.fdata.norm.delta_z);
      wsgl_set_current_normal(fasd3.fdata.norm.delta_x,
                              fasd3.fdata.norm.delta_y,
                              fasd3.fdata.norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_points(fasd3.vdata->num_vertices,
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
        priv_fill_area3_points(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.points);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      colr_type = wsgl_get_back_int_colr(ast)->type;
      wsgl_colr_from_gcolr(&colr, wsgl_get_back_int_colr(ast), ws->current_colour_model);
      wsgl_setup_back_int_colr(ws, colr_type, &colr, ast);
      fasd3_normal3(&norm, &fasd3);
      glNormal3f(norm.delta_x, norm.delta_y, norm.delta_z);
      wsgl_set_current_normal(norm.delta_x, norm.delta_y, norm.delta_z);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_points(fasd3.vdata->num_vertices,
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
        priv_back_area3_ptcolrs(ws,
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
        priv_back_area3_ptcolrs(ws,
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
        priv_fill_area3_ptnorms(fasd3.vdata->num_vertices,
                                fasd3.vdata->vertex_data.ptnorms);

        /* Advance to next set of data */
        fasd3_next_vdata3(&fasd3);
        if (fasd3.eflag == PEDGE_VISIBILITY) {
          fasd3_next_edata(&fasd3);
        }
      }
    }
    else {
      colr_type = wsgl_get_back_int_colr(ast)->type;
      wsgl_colr_from_gcolr(&colr, wsgl_get_back_int_colr(ast), ws->current_colour_model);
      wsgl_setup_back_int_colr(ws, colr_type, &colr, ast);
      for (i = 0; i < fasd3.nfa; i++) {
        priv_fill_area3_ptnorms(fasd3.vdata->num_vertices,
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
      priv_back_area3_ptconorms(ws,
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
