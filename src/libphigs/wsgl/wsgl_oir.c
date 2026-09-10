/******************************************************************************
 *   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
 *
 *   This file is part of Open PHIGS
 *   Copyright (C) 2026 CERN
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
#include <string.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xlib.h>
#ifdef GLEW
#include <GL/glew.h>
#else
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#endif

#include "phg.h"
#include "private/phgP.h"
#include "ws.h"
#include "private/wsglP.h"

/*
 * Image units the shaders expect the two objects on. These have to agree with
 * the binding qualifiers in fs430.frag and fs430_resolve.frag.
 */
#define OIR_HEAD_POINTER_UNIT 0
#define OIR_LIST_BUFFER_UNIT  1

/*******************************************************************************
 * wsgl_oir_publish_state
 *
 * DESCR:       Hand the current order independent rendering state to the
 *              shaders. oirEnable tells the first pass whether to append at
 *              all, list_capacity tells it how much room the list has.
 *
 *              Both have to be pushed together: telling the first pass that
 *              order independent rendering is on while list_capacity is still
 *              zero makes every appendFragment() call fail, which silently
 *              turns the whole thing back into unsorted direct drawing.
 * RETURNS:     N/A
 */
static void wsgl_oir_publish_state(Ws * ws, int enabled)
{
  GLint loc;

  if (ws->shader.program <= 0) return;
  if (ws->shader.oirModeLoc >= 0){
    glProgramUniform1i(ws->shader.program, ws->shader.oirModeLoc,
                       enabled ? ws->oir.mode : 0);
  }
  loc = glGetUniformLocation(ws->shader.program, "list_capacity");
  if (loc >= 0){
    glProgramUniform1ui(ws->shader.program, loc,
                        enabled ? ws->oir.frag_list_capacity : 0u);
  }
  if (enabled && ws->shader.oir_program > 0 && ws->shader.oirMode >= 0){
    glProgramUniform1i(ws->shader.oir_program, ws->shader.oirMode,
                       ws->oir.mode);
  }
}

/*
 * wsgl_oir_wanted: is order independent rendering asked for and supported?
 *
 * The 4.20 and later fragment shaders are the ones that build fragment lists,
 * so anything older must not allocate the buffers, and must not be told that
 * order independent rendering is on.
 */
static int wsgl_oir_wanted(Ws * ws)
{
  return (ws->oir.mode > 0) && wsgl_use_shaders &&
         (wsgl_frag_shader_version >= 420);
}

/*******************************************************************************
 * wsgl_oir_ini
 *
 * DESCR:       Initialise Order Independent Rendering
 *              Called when opening the workstation.
 * RETURNS:     N/A
 * BUGS:
 */
void wsgl_oir_ini(Ws *ws){
  /*
    Until the buffers below exist the first pass must not append, so the
    shaders are told order independent rendering is off on every path that
    leaves this function early.
  */
  if (!wsgl_oir_wanted(ws)){
    wsgl_oir_publish_state(ws, 0);
    return;
  }
  Pint width = ws->ws_rect.width;
  Pint height = ws->ws_rect.height;
  size_t n_pixels = width * height;
  if (n_pixels <= 0){
    /* At the first call things may not be initialised yet. Capture this and just ignore the call */
    wsgl_oir_publish_state(ws, 0);
    return;
  }
  /*
    Called from both phg_wsx_setup_tool() and phg_wsb_open_ws(), so on the X
    path it runs twice for one workstation. Without this guard the second call
    would allocate a second set of objects and leak the first.
  */
  if (ws->oir.head_p_texture != 0) return;
  ws->oir.oir_width  = width;
  ws->oir.oir_height = height;
  glGenTextures(1, &ws->oir.head_p_texture);
  glBindTexture(GL_TEXTURE_2D, ws->oir.head_p_texture);
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_R32UI,
               width, height,
               0,
               GL_RED_INTEGER,
               GL_UNSIGNED_INT,
               NULL
               );
  glGenBuffers(1, &ws->oir.head_p_initializer);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, ws->oir.head_p_initializer);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, n_pixels* sizeof(GLuint), NULL, GL_STATIC_DRAW);
  ws->oir.data = (char*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  if (ws->oir.data == NULL){
    fprintf(stderr, "WARNING: could not map the head pointer initialiser,"
            " order independent rendering is disabled\n");
    ws->oir.head_p_texture = 0;
    ws->oir.mode = 0;
    glDeleteBuffers(1, &ws->oir.head_p_initializer);
    glDeleteTextures(1, &ws->oir.head_p_texture);
    ws->oir.head_p_initializer = 0;
    ws->oir.head_p_texture = 0;
    return;
  }
  memset(ws->oir.data, 0xFF, n_pixels*sizeof(GLuint));
  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  /* leaving this bound would turn the data pointer of every later texture
     upload in the library into an offset into this buffer */
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  glGenBuffers(1, &ws->oir.acounter_buffer);
  glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, ws->oir.acounter_buffer);
  glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

  ws->oir.frag_list_capacity = (GLuint)(ws->oir.layersPerPixel * n_pixels);
  glGenBuffers(1, &ws->oir.frag_storage_buffer);
  glBindBuffer(GL_TEXTURE_BUFFER, ws->oir.frag_storage_buffer);
  glBufferData(GL_TEXTURE_BUFFER,
               (GLsizeiptr)ws->oir.frag_list_capacity * 4 * sizeof(GLuint),
               NULL, GL_DYNAMIC_COPY);
  printf("[INFO] OIR fragment list: %u entries (%.1f MB), %d layers per pixel\n",
         ws->oir.frag_list_capacity,
         (double)ws->oir.frag_list_capacity * 4.0 * sizeof(GLuint) / (1024.0*1024.0),
         ws->oir.layersPerPixel);
  /* the shader sees the list as an image, which needs a buffer texture */
  glGenTextures(1, &ws->oir.frag_storage_texture);
  glBindTexture(GL_TEXTURE_BUFFER, ws->oir.frag_storage_texture);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, ws->oir.frag_storage_buffer);

  /* the buffers exist now, so the first pass may append into them */
  wsgl_oir_publish_state(ws, 1);
}

/*******************************************************************************
 * wsgl_oir_cleanup
 *
 * DESCR:       Cleanup Order Independent Rendering
 *              Called when closing the workstation.
 * RETURNS:     N/A
 * BUGS:
 */
void wsgl_oir_cleanup(Ws * ws){
  /* the buffers are about to go away, so stop the first pass appending */
  wsgl_oir_publish_state(ws, 0);
  if (!wsgl_oir_wanted(ws)) return;
  glDeleteTextures(1, &ws->oir.frag_storage_texture); ws->oir.frag_storage_texture = 0;
  glDeleteBuffers(1, &ws->oir.frag_storage_buffer); ws->oir.frag_storage_buffer = 0;
  ws->oir.frag_list_capacity = 0;
  glDeleteBuffers(1, &ws->oir.acounter_buffer); ws->oir.acounter_buffer = 0;
  glDeleteBuffers(1, &ws->oir.head_p_initializer);ws->oir.head_p_initializer = 0;
  ws->oir.data = NULL;
  glDeleteTextures(1, &ws->oir.head_p_texture); ws->oir.head_p_texture = 0;
}

/*******************************************************************************
 * wsgl_oir_reset
 *
 * DESCR:       Reset Order Independent Rendering
 *              Called for each new frame
 * RETURNS:     N/A
 * BUGS:
 */
void wsgl_oir_reset(Ws * ws){
  Pint width = ws->ws_rect.width;
  Pint height = ws->ws_rect.height;
  if (!wsgl_oir_wanted(ws)) return;
  if (ws->oir.head_p_texture == 0) return;
  if (width != ws->oir.oir_width || height != ws->oir.oir_height) {
     wsgl_oir_cleanup(ws);      /* frees and zeroes the handles */
     wsgl_oir_ini(ws);          /* rebuilds at the new size */
     if (ws->oir.head_p_texture == 0) return;
  }
  /*
    Set every head pointer back to the end of list marker by uploading the
    0xFF filled buffer built in wsgl_oir_ini(). With a pixel unpack buffer
    bound the NULL below is an offset into that buffer, not a host pointer.
  */
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, ws->oir.head_p_initializer);
  glBindTexture(GL_TEXTURE_2D, ws->oir.head_p_texture);
  glTexImage2D(GL_TEXTURE_2D, 0,
               GL_R32UI,
               width, height,
               0,
               GL_RED_INTEGER,
               GL_UNSIGNED_INT,
               NULL );
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glBindImageTexture(OIR_HEAD_POINTER_UNIT,
                     ws->oir.head_p_texture,
                     0,
                     GL_FALSE,
                     0,
                     GL_READ_WRITE,
                     GL_R32UI);
  glBindImageTexture(OIR_LIST_BUFFER_UNIT,
                     ws->oir.frag_storage_texture,
                     0,
                     GL_FALSE,
                     0,
                     GL_READ_WRITE,
                     GL_RGBA32UI);
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, ws->oir.acounter_buffer);
  const GLuint zero = 0;
  glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);
  /* order the clear above against last frame's appends and this frame's */
  glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT |
                  GL_BUFFER_UPDATE_BARRIER_BIT  |
                  GL_ATOMIC_COUNTER_BARRIER_BIT |
                  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  /*
    Republish the capacity every frame. It is cheap and it means a resize,
    which reallocates the list through wsgl_oir_cleanup()/wsgl_oir_ini(),
    can never leave the first pass with a stale or zero capacity.
  */
  wsgl_oir_publish_state(ws, 1);
}

/*******************************************************************************
 * wsgl_oir_resolve
 *
 * DESCR:       Resolve Order Independent Rendering
 *              Called at the end of each frame, after all geometry has been
 *              rasterised and before the buffers are swapped. Walks the per
 *              pixel fragment lists built during the frame and blends the
 *              result over the opaque image already in the framebuffer.
 * RETURNS:     N/A
 */
void wsgl_oir_resolve(Ws * ws){
  GLboolean depth_test, blend, depth_mask;
  GLint viewport[4];

  if (!wsgl_oir_wanted(ws)) return;
  if (ws->oir.head_p_texture == 0) return;
  if (ws->shader.oir_program <= 0) return;
  /* make the appends of this frame visible to the reads below */
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT |
                  GL_TEXTURE_FETCH_BARRIER_BIT);

  depth_test = glIsEnabled(GL_DEPTH_TEST);
  blend      = glIsEnabled(GL_BLEND);
  glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask);
  glGetIntegerv(GL_VIEWPORT, viewport);

  /*
    The resolve covers the viewport with one quad, so it must not be depth
    tested against the geometry it is compositing over, and it must not
    disturb the depth buffer.
  */
  /*
    The depth test stays on: fs430_resolve.frag reports the depth of the
    nearest transparent fragment, so opaque geometry in front of a
    transparent surface still hides it. The depth buffer itself must not be
    disturbed, hence the write mask.
  */
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /*
    The lists are indexed by window coordinate and a frame may have drawn
    several views, each with its own viewport, so the resolve has to cover
    the whole head pointer image rather than whatever viewport happens to be
    current.
  */
  glViewport(0, 0, (GLsizei) ws->oir.oir_width, (GLsizei) ws->oir.oir_height);

  glUseProgram(ws->shader.oir_program);
  /*
    The quad is given in clip coordinates and vs430_resolve.vert passes it
    through unchanged, so the current matrices are irrelevant here.
  */
  glBegin(GL_QUADS);
    glVertex4f(-1.0f, -1.0f, 0.0f, 1.0f);
    glVertex4f( 1.0f, -1.0f, 0.0f, 1.0f);
    glVertex4f( 1.0f,  1.0f, 0.0f, 1.0f);
    glVertex4f(-1.0f,  1.0f, 0.0f, 1.0f);
  glEnd();

  glUseProgram(ws->shader.program);
  glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  if (!blend) glDisable(GL_BLEND);
  if (!depth_test) glDisable(GL_DEPTH_TEST);
  glDepthMask(depth_mask);
}
