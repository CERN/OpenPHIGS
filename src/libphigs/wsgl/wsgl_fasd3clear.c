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
 * priv_clear_area3_points
 *
 * DESCR:	Clear fill area with point data 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_points(
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
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom);
  free(t_verts);
}

/*******************************************************************************
 * priv_clear_area3_ptcolrs
 *
 * DESCR:	Clear fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_ptcolrs(
                                     Pint num_vertices,
                                     Pptco3 *ptcolrs
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
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, record_geom);
  free(t_verts);
}

/*******************************************************************************
 * priv_clear_area3_ptnorms
 *
 * DESCR:	Clear fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_ptnorms(
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
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, 0);
  free(t_verts);
}

/*******************************************************************************
 * priv_clear_area3_ptconorms
 *
 * DESCR:	Clear fill area with point, colour and normal data 3D
 *              helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_ptconorms(
                                       Pint num_vertices,
                                       Pptconorm3 *ptconorms
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
  }
  wsgl_draw_tess_polygon(t_verts, num_vertices, 0);
  free(t_verts);
}

/*******************************************************************************
 * wsgl_clear_area_set3_data
 *
 * DESCR:	Clear fill area set with data 3D
 * RETURNS:	N/A
 */

void wsgl_clear_area_set3_data(
                               Ws *ws,
                               void *pdata,
                               Ws_attr_st *ast
                               )
{
  Pint i;
  Pfasd3 fasd3;
  Pedge_data_list edata;
  Pfacet_vdata_list3 vdata;

  fasd3.edata = &edata;
  fasd3.vdata = &vdata;
  fasd3_head(&fasd3, pdata);

  glPolygonOffset(WS_CLEAR_AREA_OFFSET, wsgl_get_edge_width(ast));
  glEnable(GL_POLYGON_OFFSET_FILL);
  wsgl_setup_background(ws);

  switch (fasd3.vflag) {
  case PVERT_COORD:
    for (i = 0; i < fasd3.nfa; i++) {
      priv_clear_area3_points(fasd3.vdata->num_vertices,
                              fasd3.vdata->vertex_data.points);

      /* Advance to next set of data */
      fasd3_next_vdata3(&fasd3);
      if (fasd3.eflag == PEDGE_VISIBILITY) {
        fasd3_next_edata(&fasd3);
      }
    }
    break;

  case PVERT_COORD_COLOUR:
    for (i = 0; i < fasd3.nfa; i++) {
      priv_clear_area3_ptcolrs(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptcolrs);

      /* Advance to next set of data */
      fasd3_next_vdata3(&fasd3);
      if (fasd3.eflag == PEDGE_VISIBILITY) {
        fasd3_next_edata(&fasd3);
      }
    }
    break;

  case PVERT_COORD_NORMAL:
    for (i = 0; i < fasd3.nfa; i++) {
      priv_clear_area3_ptnorms(fasd3.vdata->num_vertices,
                               fasd3.vdata->vertex_data.ptnorms);

      /* Advance to next set of data */
      fasd3_next_vdata3(&fasd3);
      if (fasd3.eflag == PEDGE_VISIBILITY) {
        fasd3_next_edata(&fasd3);
      }
    }
    break;

  case PVERT_COORD_COLOUR_NORMAL:
    for (i = 0; i < fasd3.nfa; i++) {
      priv_clear_area3_ptconorms(fasd3.vdata->num_vertices,
                                 fasd3.vdata->vertex_data.ptconorms);

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

  glDisable(GL_POLYGON_OFFSET_FILL);
}
