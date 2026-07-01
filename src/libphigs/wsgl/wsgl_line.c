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

/*******************************************************************************
 * wsgl_polyline
 *
 * DESCR:	Draw lines
 * RETURNS:	N/A
 */

void wsgl_polyline(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   int i;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;
   int vertex_indices[MAX_VERTICES];
   int n_vertices = 0;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   wsgl_setup_line_attr(ast);
   {
       GLfloat cur_color[4];
       glGetFloatv(GL_CURRENT_COLOR, cur_color);
       if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
       if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
   }
   glBegin(GL_LINES);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex2f(point_list.points[i].x,
                 point_list.points[i].y);
      if (record_geom){
        vertex_indices[n_vertices] = wsgl_add_vertex(point_list.points[i].x,
                                                     point_list.points[i].y,
                                                     0.0);
        n_vertices ++;
      }
   }
   if (record_geom){
     wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_polyline3
 *
 * DESCR:	Draw lines 3D
 * RETURNS:	N/A
 */

void wsgl_polyline3(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   int i;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;
   int vertex_indices[MAX_VERTICES];
   int n_vertices = 0;

   point_list.num_points = *data;
   point_list.points = (Ppoint3 *) &data[1];

   wsgl_setup_line_attr(ast);
   {
       GLfloat cur_color[4];
       glGetFloatv(GL_CURRENT_COLOR, cur_color);
       if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
       if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
   }
   glBegin(GL_LINES);
   for (i = 0; i < point_list.num_points; i++) {
      glVertex3f(point_list.points[i].x,
                 point_list.points[i].y,
                 point_list.points[i].z);
      if (record_geom){
        vertex_indices[n_vertices] = wsgl_add_vertex(point_list.points[i].x,
                                                     point_list.points[i].y,
                                                     point_list.points[i].z);
        n_vertices ++;
      }
   }
   if (record_geom){
     wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
   }
   glEnd();
}
