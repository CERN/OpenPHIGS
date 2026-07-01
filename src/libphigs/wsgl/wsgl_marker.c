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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"

#define PI 3.1415926535897932384626433832795

/*******************************************************************************
 * wsgl_marker_line_loop
 *
 * DESCR:	Draw marker dots helper function
 * RETURNS:	N/A
 */

static void wsgl_marker_line_loop(
  Pint n,
  Ppoint_list *point_list,
  Pfloat scale
  )
{
  int i, j;

  float alpha, dalpha;
  glLineWidth(1.0);
  glDisable(GL_LINE_STIPPLE);
  dalpha = 2.0*PI/(float)n;
  glBegin(GL_LINE_LOOP);
  for (i = 0; i < point_list->num_points; i++) {
    alpha = dalpha/2.0;
    for (j = 0; j < n; j++){
      glVertex2f(point_list->points[i].x + scale*cos(alpha),
		 point_list->points[i].y + scale*sin(alpha));
      alpha += dalpha;
    }
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_marker_plus
 *
 * DESCR:	Draw marker pluses helper function
 * RETURNS:	N/A
 */

static void wsgl_marker_plus(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_marker_asterisk
 *
 * DESCR:	Draw marker asterisks helper function
 * RETURNS:	N/A
 */

static void wsgl_marker_asterisk(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale, small_scale;

   half_scale = scale / 2.0;
   small_scale = half_scale / 1.414;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x,
                 point_list->points[i].y + half_scale);

      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y + small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x - small_scale,
                 point_list->points[i].y - small_scale);
      glVertex2f(point_list->points[i].x + small_scale,
                 point_list->points[i].y + small_scale);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_marker_cross
 *
 * DESCR:	Draw marker crosses helper function
 * RETURNS:	N/A
 */

static void wsgl_marker_cross(
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i;
   float half_scale;

   half_scale = scale / 2.0;

   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   glBegin(GL_LINES);
   for (i = 0; i < point_list->num_points; i++) {
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y + half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x - half_scale,
                 point_list->points[i].y - half_scale);
      glVertex2f(point_list->points[i].x + half_scale,
                 point_list->points[i].y + half_scale);
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_marker_polygon
 *
 * DESCR:	Draw a polygon with n corners
 * RETURNS:	N/A
 */

static void wsgl_marker_polygon(
   Pint n,
   Ppoint_list *point_list,
   Pfloat scale
   )
{
   int i, j;
   float alpha, dalpha;
   glLineWidth(1.0);
   glDisable(GL_LINE_STIPPLE);
   dalpha = 2.0*PI/(float)n;
   glBegin(GL_TRIANGLE_FAN);
   for (i = 0; i < point_list->num_points; i++) {
     alpha = dalpha/2.0;
     for (j = 0; j < n; j++){
       glVertex2f(point_list->points[i].x + scale*cos(alpha),
		  point_list->points[i].y + scale*sin(alpha));
       alpha += dalpha;
     }
   }
   glEnd();
}

/*******************************************************************************
 * wsgl_polymarker
 *
 * DESCR:	Draw markers
 * RETURNS:	N/A
 */

void wsgl_polymarker(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint type;
   Pfloat size;
   Ppoint_list point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint *) &data[1];

   wsgl_setup_marker_attr(ast, &type, &size);
   {
       GLfloat cur_color[4];
       glGetFloatv(GL_CURRENT_COLOR, cur_color);
       if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
       if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
   }
   switch (type) {
      case PMARKER_DOT:
	wsgl_marker_polygon(40, &point_list, size);
	break;

      case PMARKER_PLUS:
	wsgl_marker_plus(&point_list, size);
	break;

      case PMARKER_ASTERISK:
	wsgl_marker_asterisk(&point_list, size);
	break;

      case PMARKER_CROSS:
	wsgl_marker_cross(&point_list, size);
	break;

      case PMARKER_CIRCLE:
	wsgl_marker_line_loop(40, &point_list, size);
	break;

      case PMARKER_TRIANG:
        wsgl_marker_polygon(3, &point_list, size);
	break;

      case PMARKER_SQUARE:
        wsgl_marker_polygon(4, &point_list, size);
	break;

      case PMARKER_PENTAGON:
        wsgl_marker_polygon(5, &point_list, size);
	break;

      case PMARKER_HEXAGON:
        wsgl_marker_polygon(6, &point_list, size);
	break;
   }
}

/*******************************************************************************
 * wsgl_polymarker3
 *
 * DESCR:	Draw markers 3D
 * RETURNS:	N/A
 */

void wsgl_polymarker3(
   Ws *ws,
   void *pdata,
   Ws_attr_st *ast
   )
{
   Pint type;
   Pfloat size;
   int i;
   Ppoint_list plist;
   Ppoint_list3 point_list;
   Pint *data = (Pint *) pdata;

   point_list.num_points = *data;
   point_list.points = (Ppoint3 *) &data[1];

   wsgl_setup_line_attr(ast);

   if (PHG_SCRATCH_SPACE(&ws->scratch,
                         point_list.num_points * sizeof(Ppoint))) {
      plist.num_points = point_list.num_points;
      plist.points = (Ppoint *) ws->scratch.buf;

      for (i = 0; i < point_list.num_points; i++) {
         plist.points[i].x = point_list.points[i].x;
         plist.points[i].y = point_list.points[i].y;
      }

      wsgl_setup_marker_attr(ast, &type, &size);
      {
          GLfloat cur_color[4];
          glGetFloatv(GL_CURRENT_COLOR, cur_color);
          if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
          if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
      }
      switch (type) {
      case PMARKER_DOT:
	wsgl_marker_polygon(40, &plist, size);
	break;

      case PMARKER_PLUS:
	wsgl_marker_plus(&plist, size);
	break;

      case PMARKER_ASTERISK:
	wsgl_marker_asterisk(&plist, size);
	break;

      case PMARKER_CROSS:
	wsgl_marker_cross(&plist, size);
	break;

      case PMARKER_CIRCLE:
	wsgl_marker_line_loop(40, &plist, size);
	break;

      case PMARKER_TRIANG:
        wsgl_marker_polygon(3, &plist, size);
	break;

      case PMARKER_SQUARE:
        wsgl_marker_polygon(4, &plist, size);
	break;

      case PMARKER_PENTAGON:
        wsgl_marker_polygon(5, &plist, size);
	break;

      case PMARKER_HEXAGON:
        wsgl_marker_polygon(6, &plist, size);
	break;
      }
   }
   else {
      ERR_REPORT(ws->erh, ERR900);
   }
}
