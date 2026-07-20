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
#include <stdlib.h>
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
#include "private/wsglP.h"


#define ImageWidth 64
#define ImageHeight 64
static GLubyte checkImage[ImageWidth][ImageHeight][3];
static GLubyte randomImage[ImageWidth][ImageHeight][3];
#define NTEXTURES 2
static unsigned int texture[NTEXTURES];
extern GLint applyTexture;

/*******************************************************************************
 * CreateRandomImage
 *
 * DESCR:      Generate random pattern
 * RETURNS:    N/A
 */
void CreateRandomImage(void)
{
  int i, j, k, r, c;
  for (i = 0; i < ImageWidth; i++) {
    for (j = 0; j < ImageHeight; j++) {
      c = (unsigned int)(100.0*rand()/RAND_MAX+0.5);
      for (k=0; k<3;k++){
	if (c >= 50){
	  randomImage[i][j][k] = (GLubyte) 255;
	} else {
	  for (k=0; k<3;k++){
	    randomImage[i][j][k] = (GLubyte) 0;
	  }
	}
      }
    }
  }
}

/*******************************************************************************
 * CreateCheckImage
 *
 * DESCR:      Generate check pattern
 * RETURNS:    N/A
 */
void CreateCheckImage(void)
{
  int i, j, r, c;
  for (i = 0; i < ImageWidth; i++) {
    for (j = 0; j < ImageHeight; j++) {
      c = ((((i&0x8)==0)^((j&0x8))==0))*255;
      // FIXME: use take actual fill color ?
      checkImage[i][j][0] = (GLubyte) c;
      checkImage[i][j][1] = (GLubyte) c;
      checkImage[i][j][2] = (GLubyte) c;
    }
  }
}

/*******************************************************************************
 * wsgl_setup_random
 *
 * DESCR:      Setup check pattern
 * RETURNS:    N/A
 */
void wsgl_setup_random(){
  CreateRandomImage();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*******************************************************************************
 * wsgl_setup_check
 *
 * DESCR:      Setup check pattern
 * RETURNS:    N/A
 */
void wsgl_setup_check(){
  CreateCheckImage();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

/*******************************************************************************
 * wsgl_setup_patterns
 *
 * DESCR:      Setup patterns
 * RETURNS:    N/A
 */
void wsgl_setup_patterns(){
  int i;
  /* initialize textures */
  for (i=0; i<NTEXTURES; i++){
    texture[i] = 0;
  }
  glGenTextures(NTEXTURES, &texture[0]);
  for (i=0; i<NTEXTURES; i++){
    glBindTexture(GL_TEXTURE_2D, texture[i]);
  }
  wsgl_setup_check();
  wsgl_setup_random();
}

/*******************************************************************************
 * wsgl_select_pattern
 *
 * DESCR:      Set pattern
 * RETURNS:    N/A
 */
void wsgl_select_pattern(Ws * ws, unsigned short num){
  glActiveTexture(GL_TEXTURE0);
  switch (num) {
  case 1:
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth,
		 ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		 &checkImage[0][0][0]);
    break;
  case 2:
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth,
		 ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		 &randomImage[0][0][0]);
    break;
  default:
    break;
  }
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  if (wsgl_use_shaders){
    GLint loc = glGetUniformLocation(ws->program, "currentTexture");
    if (loc == -1) {
      fprintf(stderr, "currentTexture uniform not found (optimized out or wrong name)\n");
    }
    glUniform1i(loc, 0);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
      fprintf(stderr, "glUniform1i failed: 0x%x\n", err);
    }
  }
}

/*******************************************************************************
 * wsgl_cleanup_patterns
 *
 * DESCR:      Setup patterns
 * RETURNS:    N/A
 */
void wsgl_cleanup_patterns(){
  int i;
  for (i=0; i<NTEXTURES; i++){
    if (texture[i] != 0){
      glDeleteTextures(1, &texture[i]);
      texture[i] = 0;
    }
  }
}
