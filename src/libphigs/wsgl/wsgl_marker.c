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

/*
 * Markers are meant to keep a constant apparent size and always face the
 * viewer, like annotation text -- not shrink with distance the way ordinary
 * WC geometry does under a perspective view. Wsgl_marker_ctx/Wsgl_marker_anchor
 * carry what wsgl_marker_vertex() needs to draw a marker's glyph offsets in
 * eye space (camera-aligned, and pre-scaled by the anchor's projective w so
 * the perspective divide the GPU applies downstream cancels out to a fixed
 * screen size) instead of in modelling coordinates.
 */
typedef struct {
  Pmatrix3 model_tran_inv;
  int      billboard;
} Wsgl_marker_ctx;

typedef struct {
  Ppoint3 eye;
  Pfloat  w;
} Wsgl_marker_anchor;

/*******************************************************************************
 * wsgl_marker_prep_anchor
 *
 * DESCR:    Project one marker's anchor point into eye space and read off
 *           the projection matrix row that produces its clip-space w, so
 *           glyph offsets can be pre-scaled by w before wsgl_marker_vertex()
 *           adds them.
 * RETURNS:  N/A
 */
static void wsgl_marker_prep_anchor(
                                    Ws *ws,
                                    Ppoint3 *pt,
                                    Wsgl_marker_anchor *anchor
                                    )
{
  Wsgl_handle wsgl = ws->render_context;

  anchor->eye.x = wsgl->model_tran[0][0]*pt->x + wsgl->model_tran[0][1]*pt->y +
                  wsgl->model_tran[0][2]*pt->z + wsgl->model_tran[0][3];
  anchor->eye.y = wsgl->model_tran[1][0]*pt->x + wsgl->model_tran[1][1]*pt->y +
                  wsgl->model_tran[1][2]*pt->z + wsgl->model_tran[1][3];
  anchor->eye.z = wsgl->model_tran[2][0]*pt->x + wsgl->model_tran[2][1]*pt->y +
                  wsgl->model_tran[2][2]*pt->z + wsgl->model_tran[2][3];
  anchor->w = wsgl->cur_struct.view_rep.map_matrix[3][0]*anchor->eye.x +
              wsgl->cur_struct.view_rep.map_matrix[3][1]*anchor->eye.y +
              wsgl->cur_struct.view_rep.map_matrix[3][2]*anchor->eye.z +
              wsgl->cur_struct.view_rep.map_matrix[3][3];
}

/*******************************************************************************
 * wsgl_marker_vertex
 *
 * DESCR:    Emit one glyph vertex at (dx, dy) from a marker's anchor point.
 *           When billboarding, (dx, dy) is added in eye space (so it stays
 *           aligned with the camera, not the object's modelling transform)
 *           after being scaled by the anchor's w, so the perspective divide
 *           applied downstream by the GPU cancels back out to a constant
 *           screen size; the result is mapped back into the current
 *           modelling coordinate system with the inverse of the modelview
 *           matrix so it can be handed to the normal transform pipeline
 *           unchanged, exactly like the vrc2wc round trip annotation text
 *           already uses for camera-facing glyphs.
 *
 *           Off (e.g. during PHIGS pick traversal, where model_tran
 *           temporarily holds something other than the true modelview --
 *           see the WS_RENDER_MODE_SELECT branch of wsgl_update_projection())
 *           it falls back to the previous, unscaled behaviour.
 * RETURNS:  N/A
 */
static void wsgl_marker_vertex(
                               Wsgl_marker_ctx *ctx,
                               Wsgl_marker_anchor *anchor,
                               Ppoint3 *base,
                               Pfloat dx,
                               Pfloat dy
                               )
{
  Ppoint3 eye_v, mc_v;

  if (!ctx->billboard) {
    glVertex3f(base->x + dx, base->y + dy, base->z);
    return;
  }
  eye_v.x = anchor->eye.x + anchor->w * dx;
  eye_v.y = anchor->eye.y + anchor->w * dy;
  eye_v.z = anchor->eye.z;
  if (!phg_tranpt3(&eye_v, ctx->model_tran_inv, &mc_v)) {
    mc_v = eye_v;
  }
  glVertex3f(mc_v.x, mc_v.y, mc_v.z);
}

/*******************************************************************************
 * wsgl_marker_line_loop
 *
 * DESCR:    Draw marker dots helper function
 * RETURNS:    N/A
 */

static void wsgl_marker_line_loop(
                                  Ws *ws,
                                  Wsgl_marker_ctx *ctx,
                                  Pint n,
                                  Ppoint_list3 *point_list,
                                  Pfloat scale
                                  )
{
  int i, j;
  Wsgl_marker_anchor anchor;
  float alpha, dalpha;

  glLineWidth(1.0);
  glDisable(GL_LINE_STIPPLE);
  dalpha = 2.0*PI/(float)n;
  glBegin(GL_LINE_LOOP);
  for (i = 0; i < point_list->num_points; i++) {
    if (ctx->billboard) wsgl_marker_prep_anchor(ws, &point_list->points[i], &anchor);
    alpha = dalpha/2.0;
    for (j = 0; j < n; j++){
      wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],
                         scale*cos(alpha), scale*sin(alpha));
      alpha += dalpha;
    }
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_marker_plus
 *
 * DESCR:    Draw marker pluses helper function
 * RETURNS:    N/A
 */

static void wsgl_marker_plus(
                             Ws *ws,
                             Wsgl_marker_ctx *ctx,
                             Ppoint_list3 *point_list,
                             Pfloat scale
                             )
{
  int i;
  Wsgl_marker_anchor anchor;
  float half_scale;

  half_scale = scale / 2.0;

  glLineWidth(1.0);
  glDisable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  for (i = 0; i < point_list->num_points; i++) {
    if (ctx->billboard) wsgl_marker_prep_anchor(ws, &point_list->points[i], &anchor);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], -half_scale, 0.0);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],  half_scale, 0.0);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], 0.0, -half_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], 0.0,  half_scale);
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_marker_asterisk
 *
 * DESCR:    Draw marker asterisks helper function
 * RETURNS:    N/A
 */

static void wsgl_marker_asterisk(
                                 Ws *ws,
                                 Wsgl_marker_ctx *ctx,
                                 Ppoint_list3 *point_list,
                                 Pfloat scale
                                 )
{
  int i;
  Wsgl_marker_anchor anchor;
  float half_scale, small_scale;

  half_scale = scale / 2.0;
  small_scale = half_scale / 1.414;

  glLineWidth(1.0);
  glDisable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  for (i = 0; i < point_list->num_points; i++) {
    if (ctx->billboard) wsgl_marker_prep_anchor(ws, &point_list->points[i], &anchor);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], -half_scale, 0.0);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],  half_scale, 0.0);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], 0.0, -half_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], 0.0,  half_scale);

    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], -small_scale,  small_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],  small_scale, -small_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], -small_scale, -small_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],  small_scale,  small_scale);
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_marker_cross
 *
 * DESCR:    Draw marker crosses helper function
 * RETURNS:    N/A
 */

static void wsgl_marker_cross(
                              Ws *ws,
                              Wsgl_marker_ctx *ctx,
                              Ppoint_list3 *point_list,
                              Pfloat scale
                              )
{
  int i;
  Wsgl_marker_anchor anchor;
  float half_scale;

  half_scale = scale / 2.0;

  glLineWidth(1.0);
  glDisable(GL_LINE_STIPPLE);
  glBegin(GL_LINES);
  for (i = 0; i < point_list->num_points; i++) {
    if (ctx->billboard) wsgl_marker_prep_anchor(ws, &point_list->points[i], &anchor);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], -half_scale,  half_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],  half_scale, -half_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i], -half_scale, -half_scale);
    wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],  half_scale,  half_scale);
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_marker_polygon
 *
 * DESCR:    Draw a polygon with n corners
 * RETURNS:    N/A
 */

static void wsgl_marker_polygon(
                                Ws *ws,
                                Wsgl_marker_ctx *ctx,
                                Pint n,
                                Ppoint_list3 *point_list,
                                Pfloat scale
                                )
{
  int i, j;
  Wsgl_marker_anchor anchor;
  float alpha, dalpha;

  glLineWidth(1.0);
  glDisable(GL_LINE_STIPPLE);
  dalpha = 2.0*PI/(float)n;
  glBegin(GL_TRIANGLE_FAN);
  for (i = 0; i < point_list->num_points; i++) {
    if (ctx->billboard) wsgl_marker_prep_anchor(ws, &point_list->points[i], &anchor);
    alpha = dalpha/2.0;
    for (j = 0; j < n; j++){
      wsgl_marker_vertex(ctx, &anchor, &point_list->points[i],
                         scale*cos(alpha), scale*sin(alpha));
      alpha += dalpha;
    }
  }
  glEnd();
}

/*******************************************************************************
 * wsgl_marker_ctx_init
 *
 * DESCR:    Build the billboarding context shared by all glyph vertices of
 *           one polymarker/polymarker3 call. Billboarding is skipped during
 *           pick traversal, where model_tran is temporarily repurposed to
 *           hold the pick matrix rather than the true modelview (see the
 *           WS_RENDER_MODE_SELECT branch of wsgl_update_projection()).
 * RETURNS:  N/A
 */
static void wsgl_marker_ctx_init(
                                 Ws *ws,
                                 Wsgl_marker_ctx *ctx
                                 )
{
  Wsgl_handle wsgl = ws->render_context;

  ctx->billboard = (wsgl->render_mode != WS_RENDER_MODE_SELECT);
  if (ctx->billboard) {
    phg_mat_copy(ctx->model_tran_inv, wsgl->model_tran);
    phg_mat_inv(ctx->model_tran_inv);
  }
}

/*******************************************************************************
 * wsgl_polymarker
 *
 * DESCR:    Draw markers
 * RETURNS:    N/A
 */

void wsgl_polymarker(
                     Ws *ws,
                     void *pdata,
                     Ws_attr_st *ast
                     )
{
  Pint type;
  Pfloat size;
  int i;
  Ppoint_list src_list;
  Ppoint_list3 point_list;
  Pint *data = (Pint *) pdata;
  GLint polygonMode[2];
  Wsgl_marker_ctx ctx;

  src_list.num_points = *data;
  src_list.points = (Ppoint *) &data[1];

  if (!PHG_SCRATCH_SPACE(&ws->scratch, src_list.num_points * sizeof(Ppoint3))) {
    ERR_REPORT(ws->erh, ERR900);
    return;
  }
  point_list.num_points = src_list.num_points;
  point_list.points = (Ppoint3 *) ws->scratch.buf;
  for (i = 0; i < src_list.num_points; i++) {
    point_list.points[i].x = src_list.points[i].x;
    point_list.points[i].y = src_list.points[i].y;
    point_list.points[i].z = 0.0;
  }

  wsgl_marker_ctx_init(ws, &ctx);
  wsgl_setup_marker_attr(ws, ast, &type, &size);
  glGetIntegerv(GL_POLYGON_MODE, polygonMode);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  switch (type) {
  case PMARKER_DOT:
    wsgl_marker_polygon(ws, &ctx, 40, &point_list, size);
    break;

  case PMARKER_PLUS:
    wsgl_marker_plus(ws, &ctx, &point_list, size);
    break;

  case PMARKER_ASTERISK:
    wsgl_marker_asterisk(ws, &ctx, &point_list, size);
    break;

  case PMARKER_CROSS:
    wsgl_marker_cross(ws, &ctx, &point_list, size);
    break;

  case PMARKER_CIRCLE:
    wsgl_marker_line_loop(ws, &ctx, 40, &point_list, size);
    break;

  case PMARKER_TRIANG:
    wsgl_marker_polygon(ws, &ctx, 3, &point_list, size);
    break;

  case PMARKER_SQUARE:
    wsgl_marker_polygon(ws, &ctx, 4, &point_list, size);
    break;

  case PMARKER_PENTAGON:
    wsgl_marker_polygon(ws, &ctx, 5, &point_list, size);
    break;

  case PMARKER_HEXAGON:
    wsgl_marker_polygon(ws, &ctx, 6, &point_list, size);
    break;
  }
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
}

/*******************************************************************************
 * wsgl_polymarker3
 *
 * DESCR:    Draw markers 3D
 * RETURNS:    N/A
 */

void wsgl_polymarker3(
                      Ws *ws,
                      void *pdata,
                      Ws_attr_st *ast
                      )
{
  Pint type;
  Pfloat size;
  Ppoint_list3 point_list;
  Pint *data = (Pint *) pdata;
  GLint polygonMode[2];
  Wsgl_marker_ctx ctx;

  point_list.num_points = *data;
  point_list.points = (Ppoint3 *) &data[1];

  glGetIntegerv(GL_POLYGON_MODE, polygonMode);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  wsgl_setup_line_attr(ws, ast);
  wsgl_marker_ctx_init(ws, &ctx);
  wsgl_setup_marker_attr(ws, ast, &type, &size);
  switch (type) {
  case PMARKER_DOT:
    wsgl_marker_polygon(ws, &ctx, 40, &point_list, size);
    break;

  case PMARKER_PLUS:
    wsgl_marker_plus(ws, &ctx, &point_list, size);
    break;

  case PMARKER_ASTERISK:
    wsgl_marker_asterisk(ws, &ctx, &point_list, size);
    break;

  case PMARKER_CROSS:
    wsgl_marker_cross(ws, &ctx, &point_list, size);
    break;

  case PMARKER_CIRCLE:
    wsgl_marker_line_loop(ws, &ctx, 40, &point_list, size);
    break;

  case PMARKER_TRIANG:
    wsgl_marker_polygon(ws, &ctx, 3, &point_list, size);
    break;

  case PMARKER_SQUARE:
    wsgl_marker_polygon(ws, &ctx, 4, &point_list, size);
    break;

  case PMARKER_PENTAGON:
    wsgl_marker_polygon(ws, &ctx, 5, &point_list, size);
    break;

  case PMARKER_HEXAGON:
    wsgl_marker_polygon(ws, &ctx, 6, &point_list, size);
    break;
  }
  glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
}
