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
GLint clipping_ind, num_clip_planes;
GLint plane0, point0;
GLint plane1, point1;
GLint shading_mode;
GLint vAmbient, vDiffuse, vSpecular, vPositional;
GLint ModelViewMatrix, ProjectionMatrix;
GLint alpha_channel;
GLint lightSource0, lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0;
GLint lightSource1, lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1;
GLint lightSource2, lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2;
GLint lightSource3, lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3;
GLint lightSource4, lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4;
GLint lightSource5, lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5;
GLint lightSource6, lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6;

static const char* vertex_shader_text_130 =
"#version 130\n"
"in vec4 vColor;\n"
"out vec4 Color;\n"
"out vec4 Normal;\n"
"out float gl_ClipDistance[6];\n"
"uniform mat4 ModelViewMatrix;\n"
"uniform mat4 ProjectionMatrix;\n"
"uniform int num_clip_planes;\n"
"uniform int clipping_ind;\n"
"uniform vec4 plane0;\n"
"uniform vec4 point0;\n"
"uniform vec4 plane1;\n"
"uniform vec4 point1;\n"
"float distance;\n"
"void main()\n"
"{\n"
"  Color = vColor;\n"
"  Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));\n"
"  gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;\n"
"  if (clipping_ind > 0) {\n"
"    if (num_clip_planes == 1) {\n"
"      gl_ClipDistance[0] = dot(gl_Vertex-point0, plane0);\n"
"    } else if (num_clip_planes == 2) {\n"
"      gl_ClipDistance[0] = dot(gl_Vertex-point0, plane0);\n"
"      gl_ClipDistance[1] = -dot(gl_Vertex-point1, plane1);\n"
"    } else {\n"
"      gl_ClipDistance[0] = 1.0;\n"
"      gl_ClipDistance[1] = 1.0;\n"
"    }\n"
"  } else {\n"
"    gl_ClipDistance[0] = 1.0;\n"
"    gl_ClipDistance[1] = 1.0;\n"
"  }\n"
"}\n";

static const char* fragment_shader_text_130 =
"#version 130\n"
"uniform int ShadingMode;\n"
"uniform vec4 vAmbient;\n"
"uniform vec4 vDiffuse;\n"
"uniform vec4 vSpecular;\n"
"\n"
"uniform int lightSource0;\n"
"uniform int lightSourceTyp0;\n"
"uniform vec4 lightSourceCol0;\n"
"uniform vec4 lightSourcePos0;\n"
"uniform vec4 lightSourceCoef0;\n"
"\n"
"uniform int lightSource1;\n"
"uniform int lightSourceTyp1;\n"
"uniform vec4 lightSourceCol1;\n"
"uniform vec4 lightSourcePos1;\n"
"uniform vec4 lightSourceCoef1;\n"
"\n"
"uniform int lightSource2;\n"
"uniform int lightSourceTyp2;\n"
"uniform vec4 lightSourceCol2;\n"
"uniform vec4 lightSourcePos2;\n"
"uniform vec4 lightSourceCoef2;\n"
"\n"
"uniform int lightSource3;\n"
"uniform int lightSourceTyp3;\n"
"uniform vec4 lightSourceCol3;\n"
"uniform vec4 lightSourcePos3;\n"
"uniform vec4 lightSourceCoef3;\n"
"\n"
"uniform int lightSource4;\n"
"uniform int lightSourceTyp4;\n"
"uniform vec4 lightSourceCol4;\n"
"uniform vec4 lightSourcePos4;\n"
"uniform vec4 lightSourceCoef4;\n"
"\n"
"uniform int lightSource5;\n"
"uniform int lightSourceTyp5;\n"
"uniform vec4 lightSourceCol5;\n"
"uniform vec4 lightSourcePos5;\n"
"uniform vec4 lightSourceCoef5;\n"
"\n"
"uniform int lightSource6;\n"
"uniform int lightSourceTyp6;\n"
"uniform vec4 lightSourceCol6;\n"
"uniform vec4 lightSourcePos6;\n"
"uniform vec4 lightSourceCoef6;\n"
"uniform float alpha_channel;\n"
"\n"
"in vec4 Color;\n"
"in vec4 Normal;\n"
"out vec4 FragColor;\n"
"\n"
"vec4 getLight(int type, vec4 color, vec4 pos, vec4 coef){\n"
"  vec4 light;\n"
"  float refl = 0.0;\n"
"  float angle = Normal.x*pos.x+Normal.y*pos.y+Normal.z*pos.z;\n"
"  float lennorm = sqrt(Normal.x*Normal.x+Normal.y*Normal.y+Normal.z*Normal.z);\n"
"  float lenpos = sqrt(pos.x*pos.x+pos.y*pos.y+pos.z*pos.z);\n"
"  if (lennorm == 0.0) lennorm = 1.0;\n"
"  if (lenpos == 0.0) lenpos = 1.0;\n"
"  angle = max(angle/lennorm/lenpos, 0.0);\n"
"  switch (type) {\n"
"    case 1:\n"
"      light = color * vAmbient;\n"
"      break;\n"
"    case 2:\n"
"      light = color * vDiffuse * angle;\n"
"      break;\n"
"    case 3:\n"
"      refl = coef.x * pow(angle, coef.y);\n"
"      light = vSpecular * vec4(refl, refl, refl, 1.0);\n"
"      break;\n"
"    default:\n"
"      light = vec4(0.5, 0.0, 0.0, 1.0);\n"
"      break;\n"
"  };\n"
"  return light;\n"
"}\n"
"void main()\n"
"{\n"
"  int i;\n"
"  if (ShadingMode > 0) {\n"
"    int n = 0;\n"
"    FragColor = vec4(0., 0., 0, 1.);\n"
"    for (i=0; i<7; i++){\n"
"      switch (i) {\n"
"      case 0:\n"
"        if (lightSource0 > 0){ FragColor += getLight(lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0);n += 1;};\n"
"        break;\n"
"      case 1:\n"
"        if (lightSource1 > 0){ FragColor += getLight(lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1);n += 1;};\n"
"        break;\n"
"      case 2:\n"
"        if (lightSource2 > 0){ FragColor += getLight(lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2);n += 1;};\n"
"        break;\n"
"      case 3:\n"
"        if (lightSource3 > 0){ FragColor += getLight(lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3);n += 1;};\n"
"        break;\n"
"      case 4:\n"
"        if (lightSource4 > 0){ FragColor += getLight(lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4);n += 1;};\n"
"        break;\n"
"      case 5:\n"
"        if (lightSource5 > 0){ FragColor += getLight(lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5);n += 1;};\n"
"        break;\n"
"      case 6:\n"
"        if (lightSource6 > 0){ FragColor += getLight(lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6);n += 1;};\n"
"        break;\n"
"      default:\n"
"        break;\n"
"      };\n"
"    };\n"
"    if (n > 0){\n"
"      FragColor = min(FragColor, vec4(1., 1., 1., 1.));"
"    } else { FragColor = Color;};\n"
"  } else {\n"
"    FragColor = Color;\n"
"  };\n"
"  FragColor.a = alpha_channel;\n"
"}\n";

static const char* vertex_shader_text_120 =
"#version 120\n"
"uniform mat4 ModelViewMatrix;\n"
"uniform mat4 ProjectionMatrix;\n"
"attribute vec4 vColor;\n"
"varying vec4 Color;\n"
"varying vec4 Normal;\n"
"uniform int num_clip_planes;\n"
"uniform int clipping_ind;\n"
"uniform vec4 plane0;\n"
"uniform vec4 point0;\n"
"uniform vec4 plane1;\n"
"uniform vec4 point1;\n"
"void main()\n"
"{\n"
"    Color = vColor;\n"
"    Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));\n"
"    gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;\n"
"    if ((num_clip_planes >0 ) && (clipping_ind > 0)) {\n"
"      gl_ClipVertex = transpose(ModelViewMatrix) * gl_Vertex;\n"
"    };\n"
"}\n";

static const char* fragment_shader_text_120 =
"#version 120\n"
"uniform int ShadingMode;\n"
"uniform vec4 vAmbient;\n"
"uniform vec4 vDiffuse;\n"
"uniform vec4 vSpecular;\n"
"\n"
"uniform int lightSource0;\n"
"uniform int lightSourceTyp0;\n"
"uniform vec4 lightSourceCol0;\n"
"uniform vec4 lightSourcePos0;\n"
"uniform vec4 lightSourceCoef0;\n"
"\n"
"uniform int lightSource1;\n"
"uniform int lightSourceTyp1;\n"
"uniform vec4 lightSourceCol1;\n"
"uniform vec4 lightSourcePos1;\n"
"uniform vec4 lightSourceCoef1;\n"
"\n"
"uniform int lightSource2;\n"
"uniform int lightSourceTyp2;\n"
"uniform vec4 lightSourceCol2;\n"
"uniform vec4 lightSourcePos2;\n"
"uniform vec4 lightSourceCoef2;\n"
"\n"
"uniform int lightSource3;\n"
"uniform int lightSourceTyp3;\n"
"uniform vec4 lightSourceCol3;\n"
"uniform vec4 lightSourcePos3;\n"
"uniform vec4 lightSourceCoef3;\n"
"\n"
"uniform int lightSource4;\n"
"uniform int lightSourceTyp4;\n"
"uniform vec4 lightSourceCol4;\n"
"uniform vec4 lightSourcePos4;\n"
"uniform vec4 lightSourceCoef4;\n"
"\n"
"uniform int lightSource5;\n"
"uniform int lightSourceTyp5;\n"
"uniform vec4 lightSourceCol5;\n"
"uniform vec4 lightSourcePos5;\n"
"uniform vec4 lightSourceCoef5;\n"
"\n"
"uniform int lightSource6;\n"
"uniform int lightSourceTyp6;\n"
"uniform vec4 lightSourceCol6;\n"
"uniform vec4 lightSourcePos6;\n"
"uniform vec4 lightSourceCoef6;\n"
"uniform float alpha_channel;\n"
"varying vec4 Normal;\n"
"varying vec4 Color;\n"
"\n"
"vec4 getLight(int type, vec4 color, vec4 pos, vec4 coef){\n"
"  vec4 light;\n"
"  float refl = 0.0;\n"
"  float angle = Normal.x*pos.x+Normal.y*pos.y+Normal.z*pos.z;\n"
"  float lennorm = sqrt(Normal.x*Normal.x+Normal.y*Normal.y+Normal.z*Normal.z);\n"
"  float lenpos = sqrt(pos.x*pos.x+pos.y*pos.y+pos.z*pos.z);\n"
"  if (lennorm == 0.0) lennorm = 1.0;\n"
"  if (lenpos == 0.0) lenpos = 1.0;\n"
"  angle = max(angle/lennorm/lenpos, 0.0);\n"
"  light = vec4(0.5, 0.0, 0.0, 1.0);\n"
"  if (type == 1) {\n"
"      light = color * vAmbient;\n"
"  };\n"
"  if (type == 2) {\n"
"      light = color * vDiffuse * angle;\n"
"  };\n"
"  if (type == 3) {\n"
"      refl = coef.x * pow(angle, coef.y);\n"
"      light = vSpecular * vec4(refl, refl, refl, 1.0);\n"
"  };\n"
"  return light;\n"
"}\n"
"void main()\n"
"{\n"
"  int i;\n"
"  if (ShadingMode > 0) {\n"
"    int n = 0;\n"
"    gl_FragColor = vec4(0., 0., 0, 1.);\n"
"    for (i=0; i<7; i++){\n"
"      if (i==0) {\n"
"        if (lightSource0 > 0){ gl_FragColor += getLight(lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0);n += 1;};\n"
"      }"
"      if (i==1) {\n"
"        if (lightSource1 > 0){ gl_FragColor += getLight(lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1);n += 1;};\n"
"      }"
"      if (i==2) {\n"
"        if (lightSource2 > 0){ gl_FragColor += getLight(lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2);n += 1;};\n"
"      }"
"      if (i==3) {\n"
"        if (lightSource3 > 0){ gl_FragColor += getLight(lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3);n += 1;};\n"
"      }"
"      if (i==4) {\n"
"        if (lightSource4 > 0){ gl_FragColor += getLight(lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4);n += 1;};\n"
"      }"
"      if (i==5) {\n"
"        if (lightSource5 > 0){ gl_FragColor += getLight(lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5);n += 1;};\n"
"      }"
"      if (i==6) {\n"
"        if (lightSource6 > 0){ gl_FragColor += getLight(lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6);n += 1;};\n"
"      }"
"    };\n"
"    if (n > 0){\n"
"      gl_FragColor = min(gl_FragColor, vec4(1., 1., 1., 1.));"
"    } else { gl_FragColor = Color;};\n"
"  } else {\n"
"    gl_FragColor = Color;\n"
"  };\n"
"  gl_FragColor.a = alpha_channel;\n"
"}\n";

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
#endif
#ifdef GLEW
  if (! wsgl_use_shaders || !GLEW_ARB_vertex_shader ||! GLEW_ARB_fragment_shader ||! GLEW_ARB_shader_objects) {
#else
  if (! wsgl_use_shaders) {
#endif
    fprintf(stderr, "WARNING: Shaders are not available or not wanted.\nSome functionality will not work, e.g. clipping\n");
    glUseProgram(0);
  } else {
    char NewerVersion[] = "1.30";
    const char * ShaderVersion = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    const char * Vendor = (const char *) glGetString(GL_VENDOR);
    const char * Renderer = (const char *) glGetString(GL_RENDERER);
    printf("INFO: Shader version is %s.\n", ShaderVersion);
    printf("INFO Vendor: %s, card: %s\n", Vendor, Renderer);
    /*
      There is a bug somewhere when V3D driver (like on Raspberry-Pi) are used.
      Rendering works fine but then the program crashes with a segfault when the OpenGL window is clicked.
      For now, we switch off the use of shaders if this driver is detected.
     */
    if (0 == strncmp(Renderer,"V3D", 3)){
      printf("WARNING: Detected V3D driver. Because of a bug shaders will be switched off\n");
      wsgl_use_shaders = 0;
      return;
    }
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (strcmp(ShaderVersion, NewerVersion) < 0 ){
      printf("WARNING: Shader version is %s Using version 1.20 for shaders\n", ShaderVersion);
      glShaderSource(vertex_shader, 1, &vertex_shader_text_120, NULL);
      glShaderSource(fragment_shader, 1, &fragment_shader_text_120, NULL);
    } else {
      if (strcmp(Vendor, "NVIDIA Corporation") == 0){
        printf("Detected NVIDIA card. Using 1.30 for vertex and fragment shaders\n");
        glShaderSource(vertex_shader, 1, &vertex_shader_text_130, NULL);
        glShaderSource(fragment_shader, 1, &fragment_shader_text_130, NULL);
      } else if (strcmp(Vendor, "Intel") == 0) {
        printf("Detected Intel card. Using 1.20 for vertex and 1.30 for fragment shader\n");
        glShaderSource(vertex_shader, 1, &vertex_shader_text_120, NULL);
        glShaderSource(fragment_shader, 1, &fragment_shader_text_130, NULL);
      } else {
        printf("Unknown vendor card. Trying 1.30 for vertex and 1.30 for fragment shader\n");
        printf("Please report any problems.\n");
        glShaderSource(vertex_shader, 1, &vertex_shader_text_130, NULL);
        glShaderSource(fragment_shader, 1, &fragment_shader_text_130, NULL);
      }
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
    alpha_channel = glGetUniformLocation(ws->program, "alpha_channel");
    glUniform1f(alpha_channel, 1.0);
    // init clipping
    num_clip_planes = glGetUniformLocation(ws->program, "num_clip_planes");
    clipping_ind = glGetUniformLocation(ws->program, "clipping_ind");
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glUniform1i(clipping_ind, 0);
    glUniform1i(num_clip_planes, 2);
    plane0 = glGetUniformLocation(ws->program, "plane0");
    point0 = glGetUniformLocation(ws->program, "point0");
    plane1 = glGetUniformLocation(ws->program, "plane1");
    point1 = glGetUniformLocation(ws->program, "point1");
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
