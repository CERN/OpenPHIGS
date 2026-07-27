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

/*******************************************************************************
 * wsgl_text_vertex3tcs
 *
 * DESCR:    transform from text coordinate system to wc and define vertex
 * RETURNS:    N/A
 */
void wsgl_text_vertex3tcs(
                          Pmatrix3 tcs2wc,
                          Pfloat x,
                          Pfloat y,
                          Pfloat z,
                          Ppoint3 * pwcb
                          ){
  Ppoint3 point3, pwc;
  point3.x = x;
  point3.y = y;
  point3.z = z;
  phg_tranpt3(&point3, tcs2wc, &pwc);
  glVertex3f(pwc.x, pwc.y, pwc.z);
  *pwcb = pwc;
}

/*******************************************************************************
 * wsgl_text_set_align
 *
 * DESCR:    Set position of text according to alignment
 * RETURNS:  New position
 */
static void wsgl_set_text_align(
                                Ptext *text,
                                Ws_attr_st *ast,
                                Ppoint pos,
                                Ppoint *npos
                                )
{
  Phor_text_align hora;
  Pvert_text_align vera;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  size_t i, len;
  Phg_char *ch;
  Phg_font *fnt;
  char *str;

  Pfloat lx, ly;
  Pfloat dx, dy;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_space = wsgl_get_char_space(ast);
  char_ht = ast->char_ht;

  hora = ast->text_align.hor;
  vera = ast->text_align.vert;
  npos->x = pos.x;
  npos->y = pos.y;
  ly = (fnt->top - fnt->bottom)*char_ht;
  str = text->char_string;
  len = strlen(str);

  /* text length */
  lx = 0.0;
  for (i = 0; i < len; i++) {
    ch = &fnt->chars[(int) str[i]];
    lx +=  (ch->right + char_space) * char_ht * char_expan;
  }
  /* horizontal alignement */
  switch (hora) {
    case PHOR_NORM:
      dx = 0.;
      break;
    case PHOR_RIGHT:
      dx = -lx;
      break;
    case PHOR_CTR:
      dx = -lx/2.0;
      break;
    case PHOR_LEFT:
      dx = 0.0;
      break;
  default:
    dx = 0.0;
  }
  switch (vera) {
    case PVERT_NORM:
      dy = 0.0;
      break;
    case PVERT_TOP:
      dy = 0.0;
      break;
    case PVERT_BASE:
      dy = -ly;
      break;
    case PVERT_BOTTOM:
      dy = - ly;
      break;
    case PVERT_HALF:
      dy = - ly/2.0;
      break;
  default:
    dy = 0.0;
  }
  npos->x = pos.x + dx;
  npos->y = pos.y + dy;

}

/*******************************************************************************
 * wsgl_set_text_align3
 *
 * DESCR:    Set position of text according to alignment
 * RETURNS:  New position
 */
static void wsgl_set_text_align3(
                                 Ptext3 *text,
                                 Ws_attr_st *ast,
                                 Ppoint3 pos,
                                 Ppoint3 *npos
                                 )
{
  Phor_text_align hora;
  Pvert_text_align vera;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  size_t i, len;
  Phg_char *ch;
  Phg_font *fnt;
  char *str;

  Pfloat lx, ly;
  Pfloat dx, dy, dz;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_space = wsgl_get_char_space(ast);
  char_ht = ast->char_ht;

  hora = ast->text_align.hor;
  vera = ast->text_align.vert;
  npos->x = pos.x;
  npos->y = pos.y;
  npos->z = pos.z;
  ly = (fnt->top - fnt->bottom)*char_ht;
  str = text->char_string;
  len = strlen(str);

  /* text length */
  lx = 0.0;
  for (i = 0; i < len; i++) {
    ch = &fnt->chars[(int) str[i]];
    lx +=  (ch->right + char_space) * char_ht * char_expan;
  }
  /* horizontal alignement */
  dz = 0.0;
  switch (hora) {
    case PHOR_NORM:
      dx = 0.;
      break;
    case PHOR_RIGHT:
      dx = -lx;
      break;
    case PHOR_CTR:
      dx = -lx/2.0;
      break;
    case PHOR_LEFT:
      dx = 0.0;
      break;
  default:
    dx = 0.0;
  }
  switch (vera) {
    case PVERT_NORM:
      dy = 0.0;
      break;
    case PVERT_TOP:
      dy = 0.0;
      break;
    case PVERT_BASE:
      dy = -ly;
      break;
    case PVERT_BOTTOM:
      dy = - ly;
      break;
    case PVERT_HALF:
      dy = - ly/2.0;
      break;
  default:
    dy = 0.0;
  }
  npos->x = pos.x + dx;
  npos->y = pos.y + dy;
  npos->z = pos.z + dz;

}

/*******************************************************************************
 * wsgl_set_anno_text_set_align
 *
 * DESCR:    Set position of text according to alignment
 * RETURNS:  New position
 */
static void wsgl_set_anno_text_align(
                                     Ptext *text,
                                     Ws_attr_st *ast,
                                     Ppoint pos,
                                     Ppoint *npos
                                     )
{
  Phor_text_align hora;
  Pvert_text_align vera;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  size_t i, len;
  Phg_char *ch;
  Phg_font *fnt;
  char *str;

  Pfloat lx, ly;
  Pfloat dx, dy;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_space = wsgl_get_char_space(ast);
  char_ht = ast->anno_char_ht;

  hora = ast->anno_text_align.hor;
  vera = ast->anno_text_align.vert;
  npos->x = pos.x;
  npos->y = pos.y;
  ly = (fnt->top - fnt->bottom)*char_ht;
  str = text->char_string;
  len = strlen(str);

  /* text length */
  lx = 0.0;
  for (i = 0; i < len; i++) {
    ch = &fnt->chars[(int) str[i]];
    lx +=  (ch->right + char_space) * char_ht * char_expan;
  }
  /* horizontal alignement */
  switch (hora) {
    case PHOR_NORM:
      dx = 0.;
      break;
    case PHOR_RIGHT:
      dx = -lx;
      break;
    case PHOR_CTR:
      dx = -lx/2.0;
      break;
    case PHOR_LEFT:
      dx = 0.0;
      break;
  default:
    dx = 0.0;
  }
  switch (vera) {
    case PVERT_NORM:
      dy = 0.0;
      break;
    case PVERT_TOP:
      dy = 0.0;
      break;
    case PVERT_BASE:
      dy = -ly;
      break;
    case PVERT_BOTTOM:
      dy = - ly;
      break;
    case PVERT_HALF:
      dy = - ly/2.0;
      break;
  default:
    dy = 0.0;
  }
  npos->x = pos.x + dx;
  npos->y = pos.y + dy;

}

/*******************************************************************************
 * wsgl_set_anno_text_set_align3
 *
 * DESCR:    Set position of text according to alignment
 * RETURNS:  New position
 */
static void wsgl_set_anno_text_align3(
                                      Ptext3 *text,
                                      Ws_attr_st *ast,
                                      Ppoint3 pos,
                                      Ppoint3 *npos
                                      )
{
  Phor_text_align hora;
  Pvert_text_align vera;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  size_t i, len;
  Phg_char *ch;
  Phg_font *fnt;
  char *str;

  Pfloat lx, ly;
  Pfloat dx, dy, dz;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_space = wsgl_get_char_space(ast);
  char_ht = ast->anno_char_ht;

  hora = ast->anno_text_align.hor;
  vera = ast->anno_text_align.vert;
  npos->x = pos.x;
  npos->y = pos.y;
  npos->z = pos.z;
  ly = (fnt->top - fnt->bottom)*char_ht;
  str = text->char_string;
  len = strlen(str);

  /* text length */
  lx = 0.0;
  for (i = 0; i < len; i++) {
    ch = &fnt->chars[(int) str[i]];
    lx +=  (ch->right + char_space) * char_ht * char_expan;
  }
  /* horizontal alignement */
  dz = 0.0;
  switch (hora) {
    case PHOR_NORM:
      dx = 0.;
      break;
    case PHOR_RIGHT:
      dx = -lx;
      break;
    case PHOR_CTR:
      dx = -lx/2.0;
      break;
    case PHOR_LEFT:
      dx = 0.0;
      break;
  default:
    dx = 0.0;
  }
  switch (vera) {
    case PVERT_NORM:
      dy = 0.0;
      break;
    case PVERT_TOP:
      dy = 0.0;
      break;
    case PVERT_BASE:
      dy = -ly;
      break;
    case PVERT_BOTTOM:
      dy = - ly;
      break;
    case PVERT_HALF:
      dy = - ly/2.0;
      break;
  default:
    dy = 0.0;
  }
  npos->x = pos.x + dx;
  npos->y = pos.y + dy;
  npos->z = pos.z + dz;

}

/*******************************************************************************
 * wsgl_text_string
 *
 * DESCR:    Draw text with string precision helper function
 * RETURNS:    N/A
 */

static void wsgl_text_string(
                             Ws *ws,
                             Ptext *text,
                             Ws_attr_st *ast
                             )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint pos, posa;
  int j, z;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  {
      GLfloat cur_color[4];
      glGetFloatv(GL_CURRENT_COLOR, cur_color);
      if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
      if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
  }
  char_ht = ast->char_ht;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  wsgl_set_text_align(text, ast, posa, &pos);

  str = text->char_string;

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          glVertex2f(pos.x + spath->points[z].x * char_ht * char_expan,
                     pos.y + spath->points[z].y * char_ht);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pos.x + spath->points[z].x * char_ht * char_expan,
                                                         pos.y + spath->points[z].y * char_ht,
                                                         0.0);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }
    pos.x += ch->right * char_ht * char_expan;
  }
}

/*******************************************************************************
 * wsgl_text_string3
 *
 * DESCR:    Draw text with string precision helper function
 * RETURNS:    N/A
 */

static void wsgl_text_string3(
                              Ws *ws,
                              Ptext3 *text,
                              Ws_attr_st *ast,
                              Pmatrix3 tmatrix
                              )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint3 posa, pos;
  int j, z;
  Ppoint3 pwc;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  {
      GLfloat cur_color[4];
      glGetFloatv(GL_CURRENT_COLOR, cur_color);
      if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
      if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
  }
  char_ht = ast->char_ht;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  posa.z = text->pos.z;
  str = text->char_string;
  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);

  wsgl_set_text_align3(text, ast, posa, &pos);
  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          wsgl_text_vertex3tcs(tmatrix,
                               pos.x + spath->points[z].x * char_ht * char_expan,
                               pos.y + spath->points[z].y * char_ht,
                               pos.z, &pwc);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pwc.x, pwc.y, pwc.z);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }
    pos.x += ch->right * char_ht * char_expan;
  }
}

/*******************************************************************************
 * wsgl_anno_text_string3
 *
 * DESCR:    Draw text with string precision helper function
 * RETURNS:    N/A
 */

static void wsgl_anno_text_string3(
                                   Ws *ws,
                                   Ptext3 *text,
                                   Ws_attr_st *ast,
                                   Pmatrix3 vrc2wc
                                   )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint3 pos, posa;
  int j, z;
  Ppoint3 pwc;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  {
      GLfloat cur_color[4];
      glGetFloatv(GL_CURRENT_COLOR, cur_color);
      if (wsgl_current_transparency_pass == 0 && cur_color[3] < 1.0f) return;
      if (wsgl_current_transparency_pass == 1 && cur_color[3] >= 1.0f) return;
  }
  char_ht = ast->anno_char_ht;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  posa.z = text->pos.z;
  wsgl_set_anno_text_align3(text, ast, posa, &pos);

  str = text->char_string;
  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          wsgl_text_vertex3tcs(vrc2wc,
                               pos.x + spath->points[z].x * char_ht * char_expan,
                               pos.y + spath->points[z].y * char_ht,
                               pos.z, &pwc);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pwc.x, pwc.y, pwc.z);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }
    pos.x += ch->right * char_ht * char_expan;
  }
}

/*******************************************************************************
 * wsgl_text_char
 *
 * DESCR:    Draw text with character precision helper function
 * RETURNS:    N/A
 */

static void wsgl_text_char(
   Ws *ws,
   Ptext *text,
   Ws_attr_st *ast
                           )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  Pfloat height;
  Ptext_path text_path;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint pos, posa;
  int j, z;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_ht = ast->char_ht;
  char_space = wsgl_get_char_space(ast);
  text_path = ast->text_path;
  height = fnt->top - fnt->bottom;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  wsgl_set_text_align(text, ast, posa, &pos);
  str = text->char_string;

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          glVertex2f(pos.x + spath->points[z].x * char_ht * char_expan,
                     pos.y + spath->points[z].y * char_ht);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pos.x + spath->points[z].x * char_ht * char_expan,
                                                         pos.y + spath->points[z].y * char_ht, 0.);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }

    switch (text_path) {
    case PPATH_RIGHT:
      pos.x += (ch->right + char_space) * char_ht * char_expan;
      break;

    case PPATH_LEFT:
      pos.x -= (ch->right + char_space) * char_ht * char_expan;
      break;

    case PPATH_UP:
      pos.y += (height + char_space) * char_ht;
      break;

    case PPATH_DOWN:
      pos.y -= (height + char_space) * char_ht;
      break;
    }
  }
}

/*******************************************************************************
 * wsgl_text_char3
 *
 * DESCR:    Draw text with character precision helper function
 * RETURNS:    N/A
 */

static void wsgl_text_char3(
                            Ws *ws,
                            Ptext3 *text,
                            Ws_attr_st *ast,
                            Pmatrix3 tmatrix
                            )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  Pfloat height;
  Ptext_path text_path;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint3 posa, pos;
  int j, z;
  Ppoint3 pwc;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_ht = ast->char_ht;
  char_space = wsgl_get_char_space(ast);
  text_path = ast->text_path;
  height = fnt->top - fnt->bottom;
  posa.x = text->pos.x;
  posa.y = text->pos.y;
  posa.z = text->pos.z;
  wsgl_set_text_align3(text, ast, posa, &pos);
  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);

  str = text->char_string;

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          wsgl_text_vertex3tcs(
                               tmatrix,
                               pos.x + spath->points[z].x * char_ht * char_expan,
                               pos.y + spath->points[z].y * char_ht,
                               pos.z, &pwc);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pwc.x, pwc.y, pwc.z);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }

    switch (text_path) {
    case PPATH_RIGHT:
      pos.x += (ch->right + char_space) * char_ht * char_expan;
      break;

    case PPATH_LEFT:
      pos.x -= (ch->right + char_space) * char_ht * char_expan;
      break;

    case PPATH_UP:
      pos.y += (height + char_space) * char_ht;
      break;

    case PPATH_DOWN:
      pos.y -= (height + char_space) * char_ht;
      break;
    }
  }
}

/*******************************************************************************
 * wsgl_anno_text_char3
 *
 * DESCR:    Draw text with character precision helper function
 * RETURNS:    N/A
 */

static void wsgl_anno_text_char3(
                                 Ws *ws,
                                 Ptext3 *text,
                                 Ws_attr_st *ast,
                                 Pmatrix3 vrc2wc
                                 )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  Pfloat height;
  Ptext_path text_path;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint3 pos, posa;
  int j, z;
  Ppoint3 pwc;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_ht = ast->anno_char_ht;
  char_space = wsgl_get_char_space(ast);
  text_path = ast->anno_text_path;
  height = fnt->top - fnt->bottom;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  posa.z = text->pos.z;
  wsgl_set_anno_text_align3(text, ast, posa, &pos);
  str = text->char_string;
  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          wsgl_text_vertex3tcs(
                               vrc2wc,
                               pos.x + spath->points[z].x * char_ht * char_expan,
                               pos.y + spath->points[z].y * char_ht,
                               pos.z, &pwc);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pwc.x, pwc.y, pwc.z);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }

    switch (text_path) {
    case PPATH_RIGHT:
      pos.x += (ch->right + char_space) * char_ht * char_expan;
      break;

    case PPATH_LEFT:
      pos.x -= (ch->right + char_space) * char_ht * char_expan;
      break;

    case PPATH_UP:
      pos.y += (height + char_space) * char_ht;
      break;

    case PPATH_DOWN:
      pos.y -= (height + char_space) * char_ht;
      break;
    }
  }
}

/*******************************************************************************
 * wsgl_text_stroke
 *
 * DESCR:    Draw text with stroke precision helper function
 * RETURNS:    N/A
 */

static void wsgl_text_stroke(
                             Ws *ws,
                             Ptext *text,
                             Ws_attr_st *ast
                             )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  Pfloat height;
  Ptext_path text_path;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint pt, pos, posa;
  int j, z;
  Pvec *up;
  Pvec right;
  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_ht = ast->char_ht;
  char_space = wsgl_get_char_space(ast);
  text_path = ast->text_path;
  height = fnt->top - fnt->bottom;
  up = &ast->char_up_vec;

  right.delta_x =  up->delta_y;
  right.delta_y = -up->delta_x;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  wsgl_set_text_align(text, ast, posa, &pos);
  str = text->char_string;

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          pt.x = spath->points[z].x * right.delta_x +
            spath->points[z].y * right.delta_y;
          pt.y = spath->points[z].x * up->delta_x +
            spath->points[z].y * up->delta_y;
          glVertex2f(pos.x + pt.x * char_ht * char_expan,
                     pos.y + pt.y * char_ht);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pos.x + pt.x * char_ht * char_expan,
                                                         pos.y + pt.y * char_ht, 0.);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }

    switch (text_path) {
    case PPATH_RIGHT:
      pos.x += (ch->right + char_space) *
        right.delta_x * char_ht * char_expan;
      pos.y += (ch->right + char_space) *
        up->delta_x * char_ht * char_expan;
      break;

    case PPATH_LEFT:
      pos.x -= (ch->right + char_space) *
        right.delta_x * char_ht * char_expan;
      pos.y -= (ch->right + char_space) *
        up->delta_x * char_ht * char_expan;
      break;

    case PPATH_UP:
      pos.x += (height + char_space) * right.delta_y * char_ht;
      pos.y += (height + char_space) * up->delta_y * char_ht;
      break;

    case PPATH_DOWN:
      pos.x -= (height + char_space) * right.delta_y * char_ht;
      pos.y -= (height + char_space) * up->delta_y * char_ht;
      break;
    }
  }
}

/*******************************************************************************
 * wsgl_text_stroke3
 *
 * DESCR:    Draw text with stroke precision helper function
 * RETURNS:    N/A
 */

static void wsgl_text_stroke3(
                              Ws *ws,
                              Ptext3 *text,
                              Ws_attr_st *ast,
                              Pmatrix3 tmatrix
                              )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  Pfloat height;
  Ptext_path text_path;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint3 posa, pos;
  Ppoint pt;
  int j, z;
  Pvec *up;
  Pvec right;
  Ppoint3 pwc;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_ht = ast->char_ht;
  char_space = wsgl_get_char_space(ast);
  text_path = ast->text_path;
  height = fnt->top - fnt->bottom;
  up = &ast->char_up_vec;

  right.delta_x =  up->delta_y;
  right.delta_y = -up->delta_x;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  posa.z = text->pos.z;
  wsgl_set_text_align3(text, ast, posa, &pos);

  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);
  str = text->char_string;

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          pt.x = spath->points[z].x * right.delta_x +
            spath->points[z].y * right.delta_y;
          pt.y = spath->points[z].x * up->delta_x +
            spath->points[z].y * up->delta_y;
          wsgl_text_vertex3tcs(tmatrix,
                               pos.x + pt.x * char_ht * char_expan,
                               pos.y + pt.y * char_ht,
                               pos.y, &pwc);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pwc.x, pwc.y, pwc.z);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }

    switch (text_path) {
    case PPATH_RIGHT:
      pos.x += (ch->right + char_space) *
        right.delta_x * char_ht * char_expan;
      pos.y += (ch->right + char_space) *
        up->delta_x * char_ht * char_expan;
      break;

    case PPATH_LEFT:
      pos.x -= (ch->right + char_space) *
        right.delta_x * char_ht * char_expan;
      pos.y -= (ch->right + char_space) *
        up->delta_x * char_ht * char_expan;
      break;

    case PPATH_UP:
      pos.x += (height + char_space) * right.delta_y * char_ht;
      pos.y += (height + char_space) * up->delta_y * char_ht;
      break;

    case PPATH_DOWN:
      pos.x -= (height + char_space) * right.delta_y * char_ht;
      pos.y -= (height + char_space) * up->delta_y * char_ht;
      break;
    }
  }
}

/*******************************************************************************
 * wsgl_anno_text_stroke3
 *
 * DESCR:    Draw text with stroke precision helper function
 * RETURNS:    N/A
 */

static void wsgl_anno_text_stroke3(
                                   Ws *ws,
                                   Ptext3 *text,
                                   Ws_attr_st *ast,
                                   Pmatrix3 vrc2wc
                                   )
{
  Phg_font *fnt;
  Pfloat char_ht;
  Pfloat char_expan;
  Pfloat char_space;
  Pfloat height;
  Ptext_path text_path;
  char *str;
  size_t i, len;
  Phg_char *ch;
  Ppoint_list *spath;
  Ppoint pt;
  Ppoint3 pos, posa;
  int j, z;
  Pvec *up;
  Pvec right;
  Ppoint3 pwc;

  int vertex_indices[MAX_VERTICES];
  int n_vertices = 0;

  wsgl_setup_text_attr(ast, &fnt, &char_expan);
  char_ht = ast->anno_char_ht;
  char_space = wsgl_get_char_space(ast);
  text_path = ast->anno_text_path;
  height = fnt->top - fnt->bottom;
  up = &ast->anno_char_up_vec;

  right.delta_x =  up->delta_y;
  right.delta_y = -up->delta_x;

  posa.x = text->pos.x;
  posa.y = text->pos.y;
  posa.z = text->pos.z;
  wsgl_set_anno_text_align3(text, ast, posa, &pos);
  str = text->char_string;
  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);

  len = strlen(str);
  for (i = 0; i < len; i++) {

    ch = &fnt->chars[(int) str[i]];
    if (ch->num_paths > 0) {

      for (j = 0, spath = ch->paths;
           j < ch->num_paths;
           j++, spath++) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
        glBegin(GL_LINE_STRIP);
        for(z = 0; z < spath->num_points; z++) {
          pt.x = spath->points[z].x * right.delta_x +
            spath->points[z].y * right.delta_y;
          pt.y = spath->points[z].x * up->delta_x +
            spath->points[z].y * up->delta_y;
          wsgl_text_vertex3tcs(vrc2wc,
                               pos.x + pt.x * char_ht * char_expan,
                               pos.y + pt.y * char_ht,
                               pos.z, &pwc);
          if (record_geom){
            vertex_indices[n_vertices] = wsgl_add_vertex(pwc.x, pwc.y, pwc.z);
            n_vertices ++;
          }
        }
        if (record_geom){
          wsgl_add_geometry(GEOM_LINE, vertex_indices, NULL, n_vertices);
        }
        glEnd();
      }
    }

    switch (text_path) {
    case PPATH_RIGHT:
      pos.x += (ch->right + char_space) *
        right.delta_x * char_ht * char_expan;
      pos.y += (ch->right + char_space) *
        up->delta_x * char_ht * char_expan;
      break;

    case PPATH_LEFT:
      pos.x -= (ch->right + char_space) *
        right.delta_x * char_ht * char_expan;
      pos.y -= (ch->right + char_space) *
        up->delta_x * char_ht * char_expan;
      break;

    case PPATH_UP:
      pos.x += (height + char_space) * right.delta_y * char_ht;
      pos.y += (height + char_space) * up->delta_y * char_ht;
      break;

    case PPATH_DOWN:
      pos.x -= (height + char_space) * right.delta_y * char_ht;
      pos.y -= (height + char_space) * up->delta_y * char_ht;
      break;
    }
  }
}

/*******************************************************************************
 * wsgl_text
 *
 * DESCR:    Draw text
 * RETURNS:    N/A
 */

void wsgl_text(
               Ws *ws,
               void *tdata,
               Ws_attr_st *ast
               )
{
  Ptext_prec prec;
  Ptext text;
  Ppoint *pos = (Ppoint *) tdata;

  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);
  memcpy(&text.pos, pos, sizeof(Ppoint));
  text.char_string = (char *) &pos[1];
  prec = wsgl_get_text_prec(ast);
  switch (prec) {
  case PREC_STRING:
    wsgl_text_string(ws, &text, ast);
    break;

  case PREC_CHAR:
    wsgl_text_char(ws, &text, ast);
    break;

  case PREC_STROKE:
    wsgl_text_stroke(ws, &text, ast);
    break;
  }
}

/*******************************************************************************
 * wsgl_text3
 *
 * DESCR:    Draw text
 * RETURNS:    N/A
 */

void wsgl_text3(
                Ws *ws,
                void *tdata,
                Ws_attr_st *ast
                )
{
  Ptext_prec prec;
  Ptext3 text3;
  Ptext text;
  Ppoint3 *pos = (Ppoint3 *) tdata;
  Pvec3 plane[2];
  Pvec3 *planep;
  Pmatrix3 tlc2wctmp, wc2tlctmp, work;
  Pmatrix3 tlc2wc, wc2tlc, romatlc, romatlci, tmatrix, trm, trmi;
  Pvec3 px, py, pz;
  Pfloat lx, ly, lz;
  Pvec up;
  Pvec3 upvec3, tlcvec3;
  Pfloat length, alpha;
  Ppoint3 tlcpos, wcpos;

  glLineStipple(2, 0xffff);
  glDisable(GL_LINE_STIPPLE);
  memcpy(&wcpos, pos, sizeof(Ppoint3));
  planep = (Pvec3 *)&pos[1];
  memcpy(&plane[0], planep, 2*sizeof(Pvec3));
  text3.char_string = (char *) &planep[2];

  /* direction vector x */
  px.delta_x = plane[0].delta_x;
  px.delta_y = plane[0].delta_y;
  px.delta_z = plane[0].delta_z;
  /* normalise */
  phg_vector_normalize(&px);

  /* direction vector y */
  py.delta_x = plane[1].delta_x;
  py.delta_y = plane[1].delta_y;
  py.delta_z = plane[1].delta_z;
  /* normalise */
  phg_vector_normalize(&py);

  /* direction vector z as cross product of the above */
  phg_vector_cross_prod(&pz, &px, &py);
  phg_vector_normalize(&pz);

  /* define transformation matrix */
  tlc2wctmp[0][0] = px.delta_x;
  tlc2wctmp[0][1] = px.delta_y;
  tlc2wctmp[0][2] = px.delta_z;
  tlc2wctmp[0][3] = 0.0;

  tlc2wctmp[1][0] = py.delta_x;
  tlc2wctmp[1][1] = py.delta_y;
  tlc2wctmp[1][2] = py.delta_z;
  tlc2wctmp[1][3] = 0.0;

  tlc2wctmp[2][0] = pz.delta_x;
  tlc2wctmp[2][1] = pz.delta_y;
  tlc2wctmp[2][2] = pz.delta_z;
  tlc2wctmp[2][3] = 0.0;

  tlc2wctmp[3][0] = 0.0;
  tlc2wctmp[3][1] = 0.0;
  tlc2wctmp[3][2] = 0.0;
  tlc2wctmp[3][3] = 1.0;

#ifdef DEBUG
  printf("initial tcs -> wc\n");
  phg_mat_print(tlc2wctmp);
#endif

  /* invert */
  phg_mat_copy(wc2tlctmp, tlc2wctmp);
  phg_mat_inv(wc2tlctmp);
  //inverse(wc2tlctmp);
#ifdef DEBUG
  printf("inverted initial wc->tlc:\n");
  phg_mat_print(wc2tlctmp);
#endif
  /* transform text position into new coordinate system */
  phg_tranpt3(&wcpos, wc2tlctmp, &text3.pos);
#ifdef DEBUG
  printf("Initial TLC Text position is %f %f %f \n", text3.pos.x, text3.pos.y, text3.pos.z);
#endif
  /* Finally][we need to rotate in TLC to include the orientation */
  /* in TLC coordinates UP should point to (0,1,0) */
  up = ast->char_up_vec;
  upvec3.delta_x = up.delta_x;
  upvec3.delta_y = up.delta_y;
  upvec3.delta_z = 0;
  /* natural direction for up is this */
  tlcvec3.delta_x = 0.0;
  tlcvec3.delta_y = 1.0;
  tlcvec3.delta_z = 0.0;
  /* normalise */
  phg_vector_normalize(&upvec3);
  phg_vector_normalize(&tlcvec3);
  length = phg_vector_length(&tlcvec3);
  if (length > 0.0) {
    alpha = -acos(phg_vector_dot_prod(&tlcvec3, &upvec3));
  } else {
    alpha = 0.0;
  };
  /* turn the system around the give point so that up becomes (0, 1, 0);
     translation matrix in tlc system for point where text starts */
  trm[0][0] = 1.0;
  trm[1][0] = 0.0;
  trm[2][0] = 0.0;
  trm[3][0] = -text3.pos.x;

  trm[0][1] = 0.0;
  trm[1][1] = 1.0;
  trm[2][1] = 0.0;
  trm[3][1] = -text3.pos.y;

  trm[0][2] = 0.0;
  trm[1][2] = 0.0;
  trm[2][2] = 0.0;
  trm[3][2] = -text3.pos.z;

  trm[0][3] = 0.0;
  trm[1][3] = 0.0;
  trm[2][3] = 0.0;
  trm[3][3] = 1.0;
#ifdef DEBUG
  printf("TRM:\n");
  phg_mat_print(trm);
#endif

  trmi[0][0] = 1.0;
  trmi[1][0] = 0.0;
  trmi[2][0] = 0.0;
  trmi[3][0] = text3.pos.x;

  trmi[0][1] = 0.0;
  trmi[1][1] = 1.0;
  trmi[2][1] = 0.0;
  trmi[3][1] = text3.pos.y;

  trmi[0][2] = 0.0;
  trmi[1][2] = 0.0;
  trmi[2][2] = 0.0;
  trmi[3][2] = text3.pos.z;

  trmi[0][3] = 0.0;
  trmi[1][3] = 0.0;
  trmi[2][3] = 0.0;
  trmi[3][3] = 1.0;

  /* rotate by alpha around x and y of the given point in the tlc system */
  romatlc[0][0] = cos(alpha);
  romatlc[1][0] = sin(alpha);
  romatlc[2][0] = 0.0;
  romatlc[3][0] = 0.0;

  romatlc[0][1] = -sin(alpha);
  romatlc[1][1] = cos(alpha);
  romatlc[2][1] = 0.0;
  romatlc[3][1] = 0.0;

  romatlc[0][2] = 0.0;
  romatlc[1][2] = 0.0;
  romatlc[2][2] = 1.0;
  romatlc[3][2] = 0.0;

  romatlc[0][3] = 0.0;
  romatlc[1][3] = 0.0;
  romatlc[2][3] = 0.0;
  romatlc[3][3] = 1.0;
  /* inverse of this */
  romatlci[0][0] = cos(alpha);
  romatlci[1][0] = -sin(alpha);
  romatlci[2][0] = 0.0;
  romatlci[3][0] = 0.0;

  romatlci[0][1] = sin(alpha);
  romatlci[1][1] = cos(alpha);
  romatlci[2][1] = 0.0;
  romatlci[3][1] = 0.0;

  romatlci[0][2] = 0.0;
  romatlci[1][2] = 0.0;
  romatlci[2][2] = 1.0;
  romatlci[3][2] = 0.0;

  romatlci[0][3] = 0.0;
  romatlci[1][3] = 0.0;
  romatlci[2][3] = 0.0;
  romatlci[3][3] = 1.0;

#ifdef DEBUG
  printf("The following should be unity:\n");
  phg_mat_mul(work, romatlci, romatlc);
  phg_mat_print(work);
#endif
  /* we need the inverse of
     (trm^-1 * rotation_matrix * trm * wc2tlctmp) ⁻1
     = tlc2wctmp * trmi * romatlci * trm
  */
  phg_mat_identity(trmi);
  phg_mat_identity(trm);
  phg_mat_mul(tmatrix, romatlci, trmi);
  phg_mat_mul(work, trm, tmatrix);
  phg_mat_mul(tlc2wc, tlc2wctmp, work);
#ifdef DEBUG
  printf("Final translation matrix text to world coordinates is:\n");
  phg_mat_print(tlc2wc);
  printf("TLC Text position is %f %f %f \n", text3.pos.x, text3.pos.y, text3.pos.z);
#endif

  prec = wsgl_get_text_prec(ast);
  switch (prec) {
  case PREC_STRING:
    wsgl_text_string3(ws, &text3, ast, tlc2wc);
    break;

  case PREC_CHAR:
    wsgl_text_char3(ws, &text3, ast, tlc2wc);
    break;

  case PREC_STROKE:
    wsgl_text_stroke3(ws, &text3, ast, tlc2wc);
    break;
  }
}

/*******************************************************************************
 * wsgl_anno_text_rel3
 *
 * DESCR:    Draw annotation text
 * RETURNS:    N/A
 */

void wsgl_anno_text_rel3(
                         Ws *ws,
                         void *data,
                         Ws_attr_st *ast,
                         Pmatrix3  wc2vrc
                         )
{
  Ptext_prec prec;
  Ptext3 text;
  Ppoint3 ref_point;
  Pvec3 offset;
  void *tdata = data;
  Ppoint3 vrc;
  Pmatrix3  vrc2wc;

  /* invert to transform from VRC to WC */
  phg_mat_copy(vrc2wc, wc2vrc);
  phg_mat_inv(vrc2wc);
  memcpy(&ref_point, tdata, sizeof(Ppoint3));
  tdata += sizeof(Ppoint3);
  memcpy(&offset, tdata, sizeof(Pvec3));
  tdata += sizeof(Pvec3);
  /* calculate text position in VRC */
  if (phg_tranpt3(&ref_point, wc2vrc, &vrc)) {
    text.pos.x = vrc.x+offset.delta_x;
    text.pos.y = vrc.y+offset.delta_y;
    text.pos.z = vrc.z+offset.delta_z;
  } else {
    text.pos.x = ref_point.x+offset.delta_x;
    text.pos.y = ref_point.y+offset.delta_y;
    text.pos.z = ref_point.z+offset.delta_z;
  }

  text.char_string = (char *) tdata;
  prec = wsgl_get_text_prec(ast);
  switch (prec) {
  case PREC_STRING:
    wsgl_anno_text_string3(ws, &text, ast, vrc2wc);
    break;

  case PREC_CHAR:
    wsgl_anno_text_char3(ws, &text, ast, vrc2wc);
    break;

  case PREC_STROKE:
    wsgl_anno_text_stroke3(ws, &text, ast, vrc2wc);
    break;
  }
}

/*******************************************************************************
 * wsgl_anno_text_rel
 *
 * DESCR:    Draw annotation text
 * RETURNS:    N/A
 */

void wsgl_anno_text_rel(
                        Ws *ws,
                        void *data,
                        Ws_attr_st *ast,
                        Pmatrix3 wc2vrc
                        )
{
  Ptext_prec prec;
  Ptext3 text;
  Ppoint ref_point;
  Ppoint3 ref;
  Pvec offset;
  void *tdata = data;
  Ppoint3 vrc;
  Pmatrix3 vrc2wc;

  /* invert to transform from VRC to WC */
  phg_mat_copy(vrc2wc, wc2vrc);
  phg_mat_inv(vrc2wc);
  memcpy(&ref_point, tdata, sizeof(Ppoint));
  tdata += sizeof(Ppoint);
  memcpy(&offset, tdata, sizeof(Pvec));
  tdata += sizeof(Pvec);
  /* calculate text position in WC */
  ref.x = ref_point.x;
  ref.y = ref_point.y;
  ref.z = 0;
  if (phg_tranpt3(&ref, wc2vrc, &vrc)){
    text.pos.x = vrc.x + offset.delta_x;
    text.pos.y = vrc.y + offset.delta_y;
    text.pos.z = 0.0;
  } else {
    text.pos.x = ref_point.x + offset.delta_x;
    text.pos.y = ref_point.y + offset.delta_y;
    text.pos.z = 0.0;
  }

  text.char_string = (char *) tdata;
  prec = wsgl_get_text_prec(ast);
  switch (prec) {
  case PREC_STRING:
    wsgl_anno_text_string3(ws, &text, ast, vrc2wc);
    break;

  case PREC_CHAR:
    wsgl_anno_text_char3(ws, &text, ast, vrc2wc);
    break;

  case PREC_STROKE:
    wsgl_anno_text_stroke3(ws, &text, ast, vrc2wc);
    break;
  }
}
