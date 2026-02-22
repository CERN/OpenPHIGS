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
#ifdef GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "util.h"
#include "private/wsxP.h"
#include "private/wsglP.h"

extern GLint lightSource0, lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0;
extern GLint lightSource1, lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1;
extern GLint lightSource2, lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2;
extern GLint lightSource3, lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3;
extern GLint lightSource4, lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4;
extern GLint lightSource5, lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5;
extern GLint lightSource6, lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6;

/*******************************************************************************
 * get_light_id
 *
 * DESCR:	Get light id helper function
 * RETURNS:	Light source idientifier
 */

static GLuint get_light_id(
   Pint ind
   )
{
   GLuint id;
   switch (ind) {
      case 1:  id = GL_LIGHT1; break;
      case 2:  id = GL_LIGHT2; break;
      case 3:  id = GL_LIGHT3; break;
      case 4:  id = GL_LIGHT4; break;
      case 5:  id = GL_LIGHT5; break;
      case 6:  id = GL_LIGHT6; break;
      case 7:  id = GL_LIGHT7; break;
      default: id = GL_LIGHT0; break;
   }

   return id;
}

/*******************************************************************************
 * setup_ambient_light
 *
 * DESCR:	Setup ambient light source helper function
 * RETURNS:	N/A
 */

static void setup_ambient_light(
   Pint ind,
   Pamb_light_src_rec *rec
   )
{
   GLfloat amb[4];
   GLuint id;

   amb[0] = rec->colr.val.general.x;
   amb[1] = rec->colr.val.general.y;
   amb[2] = rec->colr.val.general.z;
   amb[3] = 1.0;

#ifdef DEBUGL
   printf("Ambient light: %f %f %f\n", amb[0], amb[1], amb[2]);
#endif
#ifdef GLEW
   if (wsgl_use_shaders && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shader_objects){
#else
   if (wsgl_use_shaders){
#endif
#ifdef DEBUGL
     printf("Ambient light Using shaders %d\n", ind);
#endif
     switch (ind){
     case 0:
       glUniform1i(lightSource0, 0);
     case 1:
       glUniform1i(lightSource0, 1);
       glUniform1i(lightSourceTyp0, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol0, 1, amb);
       break;
     case 2:
       glUniform1i(lightSource1, 1);
       glUniform1i(lightSourceTyp1, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol1, 1, amb);
       break;
     case 3:
       glUniform1i(lightSource2, 1);
       glUniform1i(lightSourceTyp2, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol2, 1, amb);
       break;
     case 4:
       glUniform1i(lightSource3, 1);
       glUniform1i(lightSourceTyp3, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol3, 1, amb);
       break;
     case 5:
       glUniform1i(lightSource4, 1);
       glUniform1i(lightSourceTyp4, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol4, 1, amb);
       break;
     case 6:
       glUniform1i(lightSource5, 1);
       glUniform1i(lightSourceTyp5, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol5, 1, amb);
       break;
     case 7:
       glUniform1i(lightSource6, 1);
       glUniform1i(lightSourceTyp6, PLIGHT_AMBIENT);
       glUniform4fv(lightSourceCol6, 1, amb);
       break;
     default:
       printf("ERROR: Unknown ambient light source index\n");
       break;
     }
   } else {
     id = get_light_id(ind);
     glLightfv(id, GL_AMBIENT, amb);
     glEnable(id);
   }
}

/*******************************************************************************
 * setup_directional_light
 *
 * DESCR:	Setup directional light source helper function
 * RETURNS:	N/A
 */

static void setup_directional_light(
   Pint ind,
   Pdir_light_src_rec *rec
   )
{
   GLfloat dif[4];
   GLfloat pos[4];
   GLuint id;

   dif[0] = rec->colr.val.general.x;
   dif[1] = rec->colr.val.general.y;
   dif[2] = rec->colr.val.general.z;
   dif[3] = 1.0;

   pos[0] = rec->dir.delta_x;
   pos[1] = rec->dir.delta_y;
   pos[2] = rec->dir.delta_z;
   pos[3] = 1.0;

#ifdef DEBUGL
   printf("Directional light: %f %f %f @(%f, %f %f)\n",
          dif[0], dif[1], dif[2],
          pos[0], pos[1], pos[2]);
#endif
#ifdef GLEW
   if (wsgl_use_shaders && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shader_objects){
#else
   if (wsgl_use_shaders){
#endif
#ifdef DEBUGL
     printf("Directional light Using shaders %d\n", ind);
#endif
     switch (ind){
     case 0:
       glUniform1i(lightSource0, 0);
     case 1:
       glUniform1i(lightSource0, 1);
       glUniform1i(lightSourceTyp0, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol0, 1, dif);
       glUniform4fv(lightSourcePos0, 1, pos);
       break;
     case 2:
       glUniform1i(lightSource1, 1);
       glUniform1i(lightSourceTyp1, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol1, 1, dif);
       glUniform4fv(lightSourcePos1, 1, pos);
       break;
     case 3:
       glUniform1i(lightSource2, 1);
       glUniform1i(lightSourceTyp2, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol2, 1, dif);
       glUniform4fv(lightSourcePos2, 1, pos);
       break;
     case 4:
       glUniform1i(lightSource3, 1);
       glUniform1i(lightSourceTyp3, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol3, 1, dif);
       glUniform4fv(lightSourcePos3, 1, pos);
       break;
     case 5:
       glUniform1i(lightSource4, 1);
       glUniform1i(lightSourceTyp4, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol4, 1, dif);
       glUniform4fv(lightSourcePos4, 1, pos);
       break;
     case 6:
       glUniform1i(lightSource5, 1);
       glUniform1i(lightSourceTyp5, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol5, 1, dif);
       glUniform4fv(lightSourcePos5, 1, pos);
       break;
     case 7:
       glUniform1i(lightSource6, 1);
       glUniform1i(lightSourceTyp6, PLIGHT_DIRECTIONAL);
       glUniform4fv(lightSourceCol6, 1, dif);
       glUniform4fv(lightSourcePos6, 1, pos);
       break;
     default:
       printf("ERROR: Unknown directional light source index\n");
       break;
     }
   } else {
     id = get_light_id(ind);
     glLightfv(id, GL_DIFFUSE, dif);
     glLightfv(id, GL_POSITION, pos);
     glEnable(id);
   }
}

/*******************************************************************************
 * setup_positional_light
 *
 * DESCR:	Setup directional light source helper function
 * RETURNS:	N/A
 */

static void setup_positional_light(
   Pint ind,
   Ppos_light_src_rec *rec
   )
{
   GLfloat dif[4];
   GLfloat pos[4];
   GLfloat coef[4];
   GLuint id;

   dif[0] = rec->colr.val.general.x;
   dif[1] = rec->colr.val.general.y;
   dif[2] = rec->colr.val.general.z;
   dif[3] = 1.0;

   pos[0] = rec->pos.x;
   pos[1] = rec->pos.y;
   pos[2] = rec->pos.z;
   pos[3] = 1.0;

   coef[0] = rec->coef[0];
   coef[1] = rec->coef[1];
   coef[2] = 0.0;
   coef[3] = 0.0;

#ifdef DEBUGL
   printf("Positional light: %f %f %f @(%f, %f %f) with %f %f\n",
          dif[0], dif[1], dif[2],
          pos[0], pos[1], pos[2],
          coef[0], coef[1]);
#endif
#ifdef GLEW
   if (wsgl_use_shaders && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shader_objects){
#else
   if (wsgl_use_shaders){
#endif
#ifdef DEBUGL
     printf("Positional light Using shaders %d\n", ind);
#endif
     switch (ind){
     case 0:
       glUniform1i(lightSource0, 0);
     case 1:
       glUniform1i(lightSource0, 1);
       glUniform1i(lightSourceTyp0, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol0, 1, dif);
       glUniform4fv(lightSourcePos0, 1, pos);
       glUniform4fv(lightSourceCoef0, 1, coef);
       break;
     case 2:
       glUniform1i(lightSource1, 1);
       glUniform1i(lightSourceTyp1, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol1, 1, dif);
       glUniform4fv(lightSourcePos1, 1, pos);
       glUniform4fv(lightSourceCoef1, 1, coef);
       break;
     case 3:
       glUniform1i(lightSource2, 1);
       glUniform1i(lightSourceTyp2, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol2, 1, dif);
       glUniform4fv(lightSourcePos2, 1, pos);
       glUniform4fv(lightSourceCoef2, 1, coef);
       break;
     case 4:
       glUniform1i(lightSource3, 1);
       glUniform1i(lightSourceTyp3, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol3, 1, dif);
       glUniform4fv(lightSourcePos3, 1, pos);
       glUniform4fv(lightSourceCoef3, 1, coef);
       break;
     case 5:
       glUniform1i(lightSource4, 1);
       glUniform1i(lightSourceTyp4, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol4, 1, dif);
       glUniform4fv(lightSourcePos4, 1, pos);
       glUniform4fv(lightSourceCoef4, 1, coef);
       break;
     case 6:
       glUniform1i(lightSource5, 1);
       glUniform1i(lightSourceTyp5, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol5, 1, dif);
       glUniform4fv(lightSourcePos5, 1, pos);
       glUniform4fv(lightSourceCoef5, 1, coef);
       break;
     case 7:
       glUniform1i(lightSource6, 1);
       glUniform1i(lightSourceTyp6, PLIGHT_POSITIONAL);
       glUniform4fv(lightSourceCol6, 1, dif);
       glUniform4fv(lightSourcePos6, 1, pos);
       glUniform4fv(lightSourceCoef6, 1, coef);
       break;
     default:
       printf("ERROR: Unknown positional light source index\n");
       break;
     }
   } else {
     id = get_light_id(ind);
     glLightfv(id, GL_DIFFUSE, dif);
     glLightfv(id, GL_POSITION, pos);
     glEnable(id);
   }
}

/*******************************************************************************
 * wsgl_update_light_src_state
 *
 * DESCR:	Update light source state for workstation
 * RETURNS:	N/A
 */

void wsgl_update_light_src_state(
                                 Ws *ws
                                 )
{
  Pint i;
  Phg_ret ret;
  Wsgl *wsgl = ws->render_context;

  glPushMatrix();
  glLoadIdentity();

  /* Activate light sources */
  for (i = 0; i < WS_MAX_LIGHT_SRC; i++) {
    if (phg_nset_name_is_set(&wsgl->cur_struct.lightstat, i)) {
#ifdef DEBUGL
      printf("Setup light source: %d\n", i);
#endif
      (*ws->inq_representation)(ws,
                                i,
                                PINQ_REALIZED,
                                PHG_ARGS_LIGHTSRCREP,
                                &ret);
      if (ret.err == 0) {
        switch (ret.data.rep.lightsrcrep.type) {
        case PLIGHT_AMBIENT:
#ifdef DEBUGL
          printf("Configure abient light\n");
#endif
          setup_ambient_light(i, &ret.data.rep.lightsrcrep.rec.ambient);
          break;
          
        case PLIGHT_DIRECTIONAL:
#ifdef DEBUGL
          printf("Configure directional light\n");
#endif
          setup_directional_light(i, &ret.data.rep.lightsrcrep.rec.directional);
          break;
          
        case PLIGHT_POSITIONAL:
#ifdef DEBUGL
          printf("Configure positional light\n");
#endif
          setup_positional_light(i, &ret.data.rep.lightsrcrep.rec.positional);
          break;
          /* FIXME
             case PLIGHT_SPOT:
             setup_spot_light(i, &ret.data.rep.lightsrcrep.rec.spot);
             break;
          */
        default:
          break;
        }
      }
    } else {
#ifdef GLEW
      if (wsgl_use_shaders && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shader_objects){
#else
      if (wsgl_use_shaders){
#endif
        switch (i){
        case 1:
          glUniform1i(lightSource0, 0);
          break;
        case 2:
          glUniform1i(lightSource1, 0);
          break;
        case 3:
          glUniform1i(lightSource2, 0);
          break;
        case 4:
          glUniform1i(lightSource3, 0);
          break;
        case 5:
          glUniform1i(lightSource4, 0);
          break;
        case 6:
          glUniform1i(lightSource5, 0);
          break;
        case 7:
          glUniform1i(lightSource6, 0);
          break;
        }
      } else {
        glDisable(get_light_id(i));
      }
      }
    }
#ifdef GLEW
   if (!wsgl_use_shaders || !GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader || !GLEW_ARB_shader_objects) glPopMatrix();
#else
   if (!wsgl_use_shaders) glPopMatrix();
#endif
}

/*******************************************************************************
 * wsgl_set_light_src_state
 *
 * DESCR:	Set light source state for workstation
 * RETURNS:	N/A
 */

void wsgl_set_light_src_state(
   Ws *ws,
   void *pdata
   )
{
   Pint i, num_ints;
   Pint *data;
   data = (Pint *) pdata;
   Wsgl *wsgl = ws->render_context;

   num_ints = *data;
   data++;
   for (i = 0; i < num_ints; i++) {
      phg_nset_name_set(&wsgl->cur_struct.lightstat, *data);
      data++;
   }

   num_ints = *data;
   data++;
   for (i = 0; i < num_ints; i++) {
      phg_nset_name_clear(&wsgl->cur_struct.lightstat, *data);
      data++;
   }

   if (phg_nset_names_is_empty_all(&wsgl->cur_struct.lightstat)) {
      wsgl->cur_struct.lighting = FALSE;
   }
   else {
      wsgl->cur_struct.lighting = TRUE;
   }

#ifdef DEBUGL
   printf("Lighting nameset: ");
   phg_nset_print(&wsgl->cur_struct.lightstat);
   printf("Lighting is %s\n", (wsgl->cur_struct.lighting) ? "On" : "Off");
#endif

   wsgl_update_light_src_state(ws);
}
