/******************************************************************************
 *   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
 *
 *   This file is part of Open PHIGS
 *   Copyright (C) 2022-2023 CERN
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

GLint vertex_shader, fragment_shader;
GLint shading_mode;
GLint vAmbient, vDiffuse, vSpecular, vPositional;
GLint ModelViewMatrix, ProjectionMatrix;
GLint lightSource0, lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0;
GLint lightSource1, lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1;
GLint lightSource2, lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2;
GLint lightSource3, lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3;
GLint lightSource4, lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4;
GLint lightSource5, lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5;
GLint lightSource6, lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6;

/* version 1.20 vertex and fragment shaders */
#include "private/vs120.h"
#include "private/fs120.h"

/* version 1.30 vertex and fragment shaders */
#include "private/vs130.h"
#include "private/fs130.h"

/*******************************************************************************
 * wsgl_shaders
 *
 * DESCR:	Initialise shaders
 * RETURNS:	N/A
 */

void wsgl_shaders(Ws * ws){
  GLenum err;
  GLint result;
  GLchar eLog[1024] = { 0 };
  if (ws->drawable_id){
    glXMakeCurrent(ws->display, ws->drawable_id, ws->glx_context);
  }
#ifdef GLEW
#ifdef DEBUG
  printf("DEBUG: Shaders: initialising GLEW\n");
#endif
  err = glewInit();
  if (GLEW_OK != err){
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    abort();
  }
  if (! (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader && GLEW_ARB_shader_objects)) wsgl_use_shaders = 0;
#endif

   if (! wsgl_use_shaders) {
      fprintf(stderr, "WARNING: Shaders are not available or not wanted.\nSome functionality may not work as expected.\n");
    glUseProgram(0);
  } else {
    char NewerVersion[] = "1.30";
    const char * ShaderVersion = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    const char * Vendor = (const char *) glGetString(GL_VENDOR);
    const char * Renderer = (const char *) glGetString(GL_RENDERER);
    printf("INFO: Hardware Shader version is %s.\n", ShaderVersion);
    printf("INFO: Hardware Vendor: %s, card: %s\n", Vendor, Renderer);
    /*
      There is a bug somewhere when V3D driver (like on Raspberry-Pi) are used.
      Rendering works fine but then the program crashes with a segfault when the OpenGL window is clicked.
      For now, we switch off the use of shaders if this driver is detected.
    */
    if (0 == strncmp(Renderer,"V3D", 3)){
      printf("WARNING: Detected V3D driver.\n");
      printf("WARNING: Because of a bug please switch off shaders via the configuration file\n");
    }
    if (strcmp(ShaderVersion, NewerVersion) < 0 ){
      printf("[WARNING] Shader version is %s\n", ShaderVersion);
      printf("Detected NVIDIA card.\n");
    } else {
      if (strcmp(Vendor, "NVIDIA Corporation") == 0){
        printf("Detected NVIDIA card.\n");
      } else if (strcmp(Vendor, "Intel") == 0) {
        printf("Detected Intel card.\n");
      } else {
        printf("Unknown vendor card.\n");
        printf("Using default shaders version 1.20\n");
      }
    }
    printf("[INFO] Using shader version %d for vertex shader\n", wsgl_vert_shader_version);
    printf("[INFO] Using shader version %d for fragment shader\n", wsgl_frag_shader_version);
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    switch (wsgl_vert_shader_version){
    case 120:
      glShaderSource(vertex_shader, 1, &vertex_shader_text_120, NULL);
      break;
    case 130:
      glShaderSource(vertex_shader, 1, &vertex_shader_text_130, NULL);
      break;
    default:
      printf("[ERROR] Unsupported vertex shader version %d\n", wsgl_vert_shader_version);
      printf("[ERROR] Please use one of 120 or 130\n");
      abort();
      break;
    }
    switch (wsgl_frag_shader_version){
    case 120:
      glShaderSource(fragment_shader, 1, &fragment_shader_text_120, NULL);
      break;
    case 130:
      glShaderSource(fragment_shader, 1, &fragment_shader_text_130, NULL);
      break;
    default:
      printf("[ERROR]Unsupported fragment shader version %d\n", wsgl_frag_shader_version);
      printf("[ERROR] Please use one of 120 or 130\n");
      abort();
      break;
    }

    // compile vertex shader
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
    if (!result){
      glGetShaderInfoLog(vertex_shader, 1024, NULL, eLog);
      fprintf(stderr, "Error compiling the vertex shader: '%s'\n", eLog);
      abort();
    }
    // compile fragment shader
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
    if (!result){
      glGetShaderInfoLog(fragment_shader, 1024, NULL, eLog);
      fprintf(stderr, "Error compiling the fragment shader: '%s'\n", eLog);
      abort();
    }
    ws->program = glCreateProgram();
    glAttachShader(ws->program, vertex_shader);
    glAttachShader(ws->program, fragment_shader);
    glLinkProgram(ws->program);
    glUseProgram(ws->program);
    // define static vColor as index 1
    glBindAttribLocation(ws->program, vCOLOR, "vColor");
    // lighting parameters
    vAmbient = glGetUniformLocation(ws->program, "vAmbient");
    vDiffuse = glGetUniformLocation(ws->program, "vDiffuse");
    vSpecular = glGetUniformLocation(ws->program, "vSpecular");
    vPositional = glGetUniformLocation(ws->program, "vPositional");
    // set some default color
    glVertexAttrib4f(vCOLOR, 0.5, 0.5, 0.5, 1.0);
    // shading mode
    shading_mode = glGetUniformLocation(ws->program, "ShadingMode");
    // light sources
    lightSource0    = glGetUniformLocation(ws->program, "lightSource0");
    lightSourceTyp0 = glGetUniformLocation(ws->program, "lightSourceTyp0");
    lightSourceCol0 = glGetUniformLocation(ws->program, "lightSourceCol0");
    lightSourcePos0 = glGetUniformLocation(ws->program, "lightSourcePos0");
    lightSourceCoef0 = glGetUniformLocation(ws->program, "lightSourceCoef0");
    //
    lightSource1    = glGetUniformLocation(ws->program, "lightSource1");
    lightSourceTyp1 = glGetUniformLocation(ws->program, "lightSourceTyp1");
    lightSourceCol1 = glGetUniformLocation(ws->program, "lightSourceCol1");
    lightSourcePos1 = glGetUniformLocation(ws->program, "lightSourcePos1");
    lightSourceCoef1 = glGetUniformLocation(ws->program, "lightSourceCoef1");
    //
    lightSource2    = glGetUniformLocation(ws->program, "lightSource2");
    lightSourceTyp2 = glGetUniformLocation(ws->program, "lightSourceTyp2");
    lightSourceCol2 = glGetUniformLocation(ws->program, "lightSourceCol2");
    lightSourcePos2 = glGetUniformLocation(ws->program, "lightSourcePos2");
    lightSourceCoef2 = glGetUniformLocation(ws->program, "lightSourceCoef2");
    //
    lightSource3    = glGetUniformLocation(ws->program, "lightSource3");
    lightSourceTyp3 = glGetUniformLocation(ws->program, "lightSourceTyp3");
    lightSourceCol3 = glGetUniformLocation(ws->program, "lightSourceCol3");
    lightSourcePos3 = glGetUniformLocation(ws->program, "lightSourcePos3");
    lightSourceCoef3 = glGetUniformLocation(ws->program, "lightSourceCoef3");
    //
    lightSource4    = glGetUniformLocation(ws->program, "lightSource4");
    lightSourceTyp4 = glGetUniformLocation(ws->program, "lightSourceTyp4");
    lightSourceCol4 = glGetUniformLocation(ws->program, "lightSourceCol4");
    lightSourcePos4 = glGetUniformLocation(ws->program, "lightSourcePos4");
    lightSourceCoef4 = glGetUniformLocation(ws->program, "lightSourceCoef4");
    //
    lightSource5    = glGetUniformLocation(ws->program, "lightSource5");
    lightSourceTyp5 = glGetUniformLocation(ws->program, "lightSourceTyp5");
    lightSourceCol5 = glGetUniformLocation(ws->program, "lightSourceCol5");
    lightSourcePos5 = glGetUniformLocation(ws->program, "lightSourcePos5");
    lightSourceCoef5 = glGetUniformLocation(ws->program, "lightSourceCoef5");
    //
    lightSource6    = glGetUniformLocation(ws->program, "lightSource6");
    lightSourceTyp6 = glGetUniformLocation(ws->program, "lightSourceTyp6");
    lightSourceCol6 = glGetUniformLocation(ws->program, "lightSourceCol6");
    lightSourcePos6 = glGetUniformLocation(ws->program, "lightSourcePos6");
    lightSourceCoef6 = glGetUniformLocation(ws->program, "lightSourceCoef6");
    // projection matrices
    ModelViewMatrix = glGetUniformLocation(ws->program, "ModelViewMatrix");
    ProjectionMatrix = glGetUniformLocation(ws->program, "ProjectionMatrix");
  }
}
