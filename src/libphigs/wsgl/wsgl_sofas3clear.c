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
#include "private/wsgl_tessP.h"
#include "private/sofas3P.h"

/*******************************************************************************
 * priv_clear_area3_points
 *
 * DESCR:	Clear fill area with point data 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_points(
                                    Pint_list *vlist,
                                    Ppoint3 *points
                                    )
{
  Pint i, vert;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(vlist->num_ints * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, vlist->num_ints * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < vlist->num_ints; i++) {
    vert = vlist->ints[i];
    t_verts[i].pos[0] = points[vert].x;
    t_verts[i].pos[1] = points[vert].y;
    t_verts[i].pos[2] = points[vert].z;
  }
  wsgl_draw_tess_polygon(t_verts, vlist->num_ints, record_geom);
  free(t_verts);
}

/*******************************************************************************
 * priv_clear_area3_ptcolrs
 *
 * DESCR:	Clear fill area with point and colour data 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_ptcolrs(
                                     Pint_list *vlist,
                                     Pptco3 *ptcolrs
                                     )
{
  Pint i, vert;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(vlist->num_ints * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, vlist->num_ints * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < vlist->num_ints; i++) {
    vert = vlist->ints[i];
    t_verts[i].pos[0] = ptcolrs[vert].point.x;
    t_verts[i].pos[1] = ptcolrs[vert].point.y;
    t_verts[i].pos[2] = ptcolrs[vert].point.z;
  }
  wsgl_draw_tess_polygon(t_verts, vlist->num_ints, record_geom);
  free(t_verts);
}

/*******************************************************************************
 * priv_clear_area3_ptnorms
 *
 * DESCR:	Clear fill area with point and normal data 3D helper function
 * RETURNS:	N/A
 */

static void priv_clear_area3_ptnorms(
                                     Pint_list *vlist,
                                     Pptnorm3 *ptnorms
                                     )
{
  Pint i, vert;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(vlist->num_ints * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, vlist->num_ints * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < vlist->num_ints; i++) {
    vert = vlist->ints[i];
    t_verts[i].pos[0] = ptnorms[vert].point.x;
    t_verts[i].pos[1] = ptnorms[vert].point.y;
    t_verts[i].pos[2] = ptnorms[vert].point.z;
  }
  wsgl_draw_tess_polygon(t_verts, vlist->num_ints, record_geom);
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
                                       Pint_list *vlist,
                                       Pptconorm3 *ptconorms
                                       )
{
  Pint i, vert;
  Wsgl_tess_vertex *t_verts = (Wsgl_tess_vertex *)malloc(vlist->num_ints * sizeof(Wsgl_tess_vertex));
  if (!t_verts) return;
  memset(t_verts, 0, vlist->num_ints * sizeof(Wsgl_tess_vertex));

  for (i = 0; i < vlist->num_ints; i++) {
    vert = vlist->ints[i];
    t_verts[i].pos[0] = ptconorms[vert].point.x;
    t_verts[i].pos[1] = ptconorms[vert].point.y;
    t_verts[i].pos[2] = ptconorms[vert].point.z;
  }
  wsgl_draw_tess_polygon(t_verts, vlist->num_ints, record_geom);
  free(t_verts);
}

/*******************************************************************************
 * wsgl_set_of_clear_area_set3_data
 *
 * DESCR:	Clear set of fill area set with data 3D
 * RETURNS:	N/A
 */

void wsgl_set_of_clear_area_set3_data(
                                      Ws *ws,
                                      void *pdata,
                                      Ws_attr_st *ast
                                      )
{
  Pint i, j;
  Psofas3 sofas3;
  Pint num_lists;
  Pint_list vlist;

  sofas3_head(&sofas3, pdata);

  glPolygonOffset(WS_CLEAR_AREA_OFFSET, wsgl_get_edge_width(ast));
  glEnable(GL_POLYGON_OFFSET_FILL);
  wsgl_setup_background(ws);

  switch (sofas3.vflag) {
  case PVERT_COORD:
    for (i = 0; i < sofas3.num_sets; i++) {
      num_lists = sofas3_num_vlists(&sofas3);
      for (j = 0; j < num_lists; j++) {
        sofas3_next_vlist(&vlist, &sofas3);
        priv_clear_area3_points(&vlist,
                                sofas3.vdata.vertex_data.points);
      }
    }
    break;

  case PVERT_COORD_COLOUR:
    for (i = 0; i < sofas3.num_sets; i++) {
      num_lists = sofas3_num_vlists(&sofas3);
      for (j = 0; j < num_lists; j++) {
        sofas3_next_vlist(&vlist, &sofas3);
        priv_clear_area3_ptcolrs(&vlist,
                                 sofas3.vdata.vertex_data.ptcolrs);
      }
    }
    break;

  case PVERT_COORD_NORMAL:
    for (i = 0; i < sofas3.num_sets; i++) {
      num_lists = sofas3_num_vlists(&sofas3);
      for (j = 0; j < num_lists; j++) {
        sofas3_next_vlist(&vlist, &sofas3);
        priv_clear_area3_ptnorms(&vlist,
                                 sofas3.vdata.vertex_data.ptnorms);
      }
    }
    break;

  case PVERT_COORD_COLOUR_NORMAL:
    for (i = 0; i < sofas3.num_sets; i++) {
      num_lists = sofas3_num_vlists(&sofas3);
      for (j = 0; j < num_lists; j++) {
        sofas3_next_vlist(&vlist, &sofas3);
        priv_clear_area3_ptconorms(&vlist,
                                   sofas3.vdata.vertex_data.ptconorms);
      }
    }
    break;

  default:
    break;
  }

  glDisable(GL_POLYGON_OFFSET_FILL);
}
