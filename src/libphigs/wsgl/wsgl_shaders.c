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
#include <signal.h>
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

/* version 1.20 vertex and fragment shaders */
#include "private/vs120.h"
#include "private/fs120.h"

/* version 1.30 vertex and fragment shaders */
#include "private/vs130.h"
#include "private/fs130.h"

/* version 4.20 vertex and fragment shaders */
#include "private/vs420.h"
#include "private/fs420.h"

/* version 4.20 order independent rendering resolve pass */
#include "private/vs420_resolve.h"
#include "private/fs420_resolve.h"

/* version 4.30 vertex and fragment shaders */
#include "private/vs430.h"
#include "private/fs430.h"

/* version 4.30 order independent rendering resolve pass */
#include "private/vs430_resolve.h"
#include "private/fs430_resolve.h"

// FIXME: this should be moved into a header file
extern void wsgl_setup_patterns();

/*
 * Table of the shader sources built into the library. Adding a version means
 * adding one line here, plus the #include above and the rule that generates
 * the header in libphigs/shaders/CMakeLists.txt. The diagnostics below pick
 * up new entries automatically.
 *
 * The sources are addressed indirectly because the generated symbols are
 * variables, not compile time constants.
 */
typedef struct {
  int version;
  const char ** vertex_source;
  const char ** fragment_source;
} Wsgl_shader_set;

static const Wsgl_shader_set wsgl_shader_sets[] = {
  { 120, &vertex_shader_text_120, &fragment_shader_text_120 },
  { 130, &vertex_shader_text_130, &fragment_shader_text_130 },
  { 420, &vertex_shader_text_420, &fragment_shader_text_420 },
  { 430, &vertex_shader_text_430, &fragment_shader_text_430 }
};

#define WSGL_NUM_SHADER_SETS \
  (sizeof(wsgl_shader_sets) / sizeof(wsgl_shader_sets[0]))

/*******************************************************************************
 * wsgl_shader_source
 *
 * DESCR:	Look up the built in source of one shader stage
 * RETURNS:	Source text, or NULL if the version is not built in
 */

static const char * wsgl_shader_source(GLenum type, int version)
{
  unsigned int i;

  for (i = 0; i < WSGL_NUM_SHADER_SETS; i++){
    if (wsgl_shader_sets[i].version == version){
      if (type == GL_VERTEX_SHADER){
        return *wsgl_shader_sets[i].vertex_source;
      } else {
        return *wsgl_shader_sets[i].fragment_source;
      }
    }
  }
  return NULL;
}

/*******************************************************************************
 * wsgl_print_shader_versions
 *
 * DESCR:	List the shader versions built into the library
 * RETURNS:	N/A
 */

static void wsgl_print_shader_versions(void)
{
  unsigned int i;

  fprintf(stderr, "[ERROR] Please use one of");
  for (i = 0; i < WSGL_NUM_SHADER_SETS; i++){
    fprintf(stderr, "%s %d",
            (i == 0) ? "" : ((i + 1 == WSGL_NUM_SHADER_SETS) ? " or" : ","),
            wsgl_shader_sets[i].version);
  }
  fprintf(stderr, "\n");
}

/*******************************************************************************
 * wsgl_glsl_version
 *
 * DESCR:	Turn the driver GLSL version string ("4.60", "1.30 Mesa ...")
 *		into the integer form used to select the shaders (460, 130)
 * RETURNS:	Version number, or 0 if it could not be parsed
 */

static int wsgl_glsl_version(const char * version_string)
{
  int major, minor;

  if (version_string == NULL) return 0;
  if (sscanf(version_string, "%d.%d", &major, &minor) != 2) return 0;
  /* the minor number is two digits: "4.3" means 430, "1.30" means 130 */
  if (minor < 10) minor *= 10;
  return major * 100 + minor;
}

/*******************************************************************************
 * wsgl_print_shader_log
 *
 * DESCR:	Print the compiler log of one shader stage. The log is fetched
 *		with the length the driver reports, so it is never truncated.
 * RETURNS:	N/A
 */

static void wsgl_print_shader_log(GLuint shader, const char * what)
{
  GLint length = 0;
  GLchar * log;

  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  /* an empty log is reported either as 0 or as 1 (the terminating NUL) */
  if (length <= 1) return;
  log = (GLchar *) malloc((size_t) length);
  if (log == NULL) return;
  glGetShaderInfoLog(shader, length, NULL, log);
  fprintf(stderr, "----- %s shader compiler log -----\n%s\n", what, log);
  free(log);
}

/*******************************************************************************
 * wsgl_print_program_log
 *
 * DESCR:	Print the linker log of a shader program
 * RETURNS:	N/A
 */

static void wsgl_print_program_log(GLuint program)
{
  GLint length = 0;
  GLchar * log;

  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  if (length <= 1) return;
  log = (GLchar *) malloc((size_t) length);
  if (log == NULL) return;
  glGetProgramInfoLog(program, length, NULL, log);
  fprintf(stderr, "----- shader program linker log -----\n%s\n", log);
  free(log);
}

/*******************************************************************************
 * wsgl_print_shader_source
 *
 * DESCR:	Echo a shader source with line numbers, so that the line
 *		numbers the GLSL compiler reports can be looked up directly.
 *		The sources are generated into headers, so the numbering here
 *		matches the .vert / .frag file it was generated from.
 * RETURNS:	N/A
 */

static void wsgl_print_shader_source(const char * source)
{
  const char * line = source;
  const char * eol;
  int number = 1;

  if (source == NULL) return;
  fprintf(stderr, "----- shader source -----\n");
  while (*line != '\0'){
    eol = strchr(line, '\n');
    if (eol != NULL){
      fprintf(stderr, "%4d | %.*s\n", number, (int) (eol - line), line);
      line = eol + 1;
    } else {
      fprintf(stderr, "%4d | %s\n", number, line);
      break;
    }
    number++;
  }
  fprintf(stderr, "-------------------------\n");
}

/*******************************************************************************
 * wsgl_compile_shader
 *
 * DESCR:	Load and compile one shader stage. On failure the driver log
 *		and the numbered source are printed before giving up.
 * RETURNS:	N/A
 */

static void wsgl_compile_shader(GLuint shader, GLenum type, int version)
{
  const char * what = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
  const char * source = wsgl_shader_source(type, version);
  GLint result = GL_FALSE;

  if (source == NULL){
    fprintf(stderr, "[ERROR] Unsupported %s shader version %d\n",
            what, version);
    wsgl_print_shader_versions();
    abort();
  }
  printf("[INFO] Using shader version %d for %s shader\n", version, what);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (!result){
    fprintf(stderr, "[ERROR] Compilation of the %s shader failed"
            " (requested version %d)\n", what, version);
    wsgl_print_shader_log(shader, what);
    wsgl_print_shader_source(source);
    abort();
  }
  /* it compiled, but the driver may still have warnings worth seeing */
  wsgl_print_shader_log(shader, what);
}

/*******************************************************************************
 * wsgl_build_program
 *
 * DESCR:	Compile and link one program from a pair of shader sources.
 *		Used for the order independent rendering resolve pass, which
 *		is not part of the version table because it is not something
 *		the user selects.
 * RETURNS:	Program name, or 0 on failure
 */

static GLint wsgl_build_program(const char * vertex_source,
                                const char * fragment_source,
                                const char * name)
{
  GLuint vs, fs;
  GLint program, result = GL_FALSE, linked = GL_FALSE;

  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
  if (!result){
    fprintf(stderr, "[ERROR] Compilation of the %s vertex shader failed\n", name);
    wsgl_print_shader_log(vs, name);
    wsgl_print_shader_source(vertex_source);
    return 0;
  }
  wsgl_print_shader_log(vs, name);

  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_source, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
  if (!result){
    fprintf(stderr, "[ERROR] Compilation of the %s fragment shader failed\n", name);
    wsgl_print_shader_log(fs, name);
    wsgl_print_shader_source(fragment_source);
    return 0;
  }
  wsgl_print_shader_log(fs, name);

  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked){
    fprintf(stderr, "[ERROR] Linking the %s program failed\n", name);
    wsgl_print_program_log(program);
    return 0;
  }
  wsgl_print_program_log(program);
  return program;
}

/****************************************
 *
 * Debugging callback
 *
 ***************************************/
static void GLAPIENTRY oir_debug_cb(GLenum src, GLenum type, GLuint id,
                                    GLenum sev, GLsizei len,
                                    const GLchar *msg, const void *user)
{
   (void) src; (void) type; (void) id; (void) len; (void) user;
   fprintf(stderr, "[GL %s] %s\n",
           sev == GL_DEBUG_SEVERITY_HIGH ? "HIGH" :
           sev == GL_DEBUG_SEVERITY_MEDIUM ? "MED" : "low", msg);
   if (sev == GL_DEBUG_SEVERITY_HIGH)
     raise(SIGTRAP);          /* or raise(SIGTRAP) under gdb */

}

/*******************************************************************************
 * wsgl_shaders
 *
 * DESCR:	Initialise shaders
 * RETURNS:	N/A
 */

void wsgl_shaders(Ws * ws){
  GLenum err;
  /* local variables */
  GLint linked;
  GLint vColorLoc;
  int driver_glsl;
  
  GLint vertex_shader, fragment_shader;

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
  wsgl_setup_patterns();
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
#ifdef GL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(oir_debug_cb, NULL);
#endif
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
      printf("Shader version is %s\n", ShaderVersion);
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
    /*
      Warn up front if we are about to feed the driver a shader it cannot
      possibly accept. The compile below would fail anyway, but the driver
      message alone tends to be cryptic.
    */
    driver_glsl = wsgl_glsl_version(ShaderVersion);
    if (driver_glsl > 0){
      if (wsgl_vert_shader_version > driver_glsl){
        fprintf(stderr, "WARNING: Requested vertex shader version %d is newer"
                " than the %d supported by the driver\n",
                wsgl_vert_shader_version, driver_glsl);
      }
      if (wsgl_frag_shader_version > driver_glsl){
        fprintf(stderr, "WARNING: Requested fragment shader version %d is newer"
                " than the %d supported by the driver\n",
                wsgl_frag_shader_version, driver_glsl);
      }
    }
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    wsgl_compile_shader(vertex_shader, GL_VERTEX_SHADER,
                        wsgl_vert_shader_version);
    wsgl_compile_shader(fragment_shader, GL_FRAGMENT_SHADER,
                        wsgl_frag_shader_version);

    ws->shader.program = glCreateProgram();
    glAttachShader(ws->shader.program, vertex_shader);
    glAttachShader(ws->shader.program, fragment_shader);
    glLinkProgram(ws->shader.program);
    glGetProgramiv(ws->shader.program, GL_LINK_STATUS, &linked);
    if (!linked) {
      fprintf(stderr, "[ERROR] Linking the shader program failed"
              " (vertex version %d, fragment version %d)\n",
              wsgl_vert_shader_version, wsgl_frag_shader_version);
      wsgl_print_program_log(ws->shader.program);
      abort();
    }
    /* it linked, but the driver may still have warnings worth seeing */
    wsgl_print_program_log(ws->shader.program);
    glUseProgram(ws->shader.program);
    /* define static vColor as index 1 */
    glBindAttribLocation(ws->shader.program, vCOLOR, "vColor");
    /*
      The whole colour path drives vColor through glVertexAttrib*(vCOLOR, ...),
      so warn if the linker did not put it there. Note that the call above only
      takes effect on the next link, so this reports the location the driver
      picked by itself.
    */
    vColorLoc = glGetAttribLocation(ws->shader.program, "vColor");
    if (vColorLoc < 0){
      fprintf(stderr, "WARNING: vColor is not an active attribute of the"
              " shader program, colours will not reach the shader\n");
    } else if (vColorLoc != vCOLOR){
      fprintf(stderr, "WARNING: vColor was linked to attribute location %d,"
              " but colours are sent to location %d\n", vColorLoc, vCOLOR);
    }
    /* define default color */
    glVertexAttrib4f(vCOLOR, 0.5, 0.5, 0.5, 1.0);
    /* lighting parameters */
    ws->shader.vAmbient = glGetUniformLocation(ws->shader.program, "vAmbient");
    ws->shader.vDiffuse = glGetUniformLocation(ws->shader.program, "vDiffuse");
    ws->shader.vSpecular = glGetUniformLocation(ws->shader.program, "vSpecular");
    ws->shader.vPositional = glGetUniformLocation(ws->shader.program, "vPositional");
    /* shading mode */
    ws->shader.shading_mode = glGetUniformLocation(ws->shader.program, "ShadingMode");
    /* light sources */
    ws->shader.lightSource0     = glGetUniformLocation(ws->shader.program, "lightSource0");
    ws->shader.lightSourceTyp0  = glGetUniformLocation(ws->shader.program, "lightSourceTyp0");
    ws->shader.lightSourceCol0  = glGetUniformLocation(ws->shader.program, "lightSourceCol0");
    ws->shader.lightSourcePos0  = glGetUniformLocation(ws->shader.program, "lightSourcePos0");
    ws->shader.lightSourceCoef0 = glGetUniformLocation(ws->shader.program, "lightSourceCoef0");
    ws->shader.lightSource1     = glGetUniformLocation(ws->shader.program, "lightSource1");
    ws->shader.lightSourceTyp1  = glGetUniformLocation(ws->shader.program, "lightSourceTyp1");
    ws->shader.lightSourceCol1  = glGetUniformLocation(ws->shader.program, "lightSourceCol1");
    ws->shader.lightSourcePos1  = glGetUniformLocation(ws->shader.program, "lightSourcePos1");
    ws->shader.lightSourceCoef1 = glGetUniformLocation(ws->shader.program, "lightSourceCoef1");
    ws->shader.lightSource2     = glGetUniformLocation(ws->shader.program, "lightSource2");
    ws->shader.lightSourceTyp2  = glGetUniformLocation(ws->shader.program, "lightSourceTyp2");
    ws->shader.lightSourceCol2  = glGetUniformLocation(ws->shader.program, "lightSourceCol2");
    ws->shader.lightSourcePos2  = glGetUniformLocation(ws->shader.program, "lightSourcePos2");
    ws->shader.lightSourceCoef2 = glGetUniformLocation(ws->shader.program, "lightSourceCoef2");
    ws->shader.lightSource3     = glGetUniformLocation(ws->shader.program, "lightSource3");
    ws->shader.lightSourceTyp3  = glGetUniformLocation(ws->shader.program, "lightSourceTyp3");
    ws->shader.lightSourceCol3  = glGetUniformLocation(ws->shader.program, "lightSourceCol3");
    ws->shader.lightSourcePos3  = glGetUniformLocation(ws->shader.program, "lightSourcePos3");
    ws->shader.lightSourceCoef3 = glGetUniformLocation(ws->shader.program, "lightSourceCoef3");
    ws->shader.lightSource4     = glGetUniformLocation(ws->shader.program, "lightSource4");
    ws->shader.lightSourceTyp4  = glGetUniformLocation(ws->shader.program, "lightSourceTyp4");
    ws->shader.lightSourceCol4  = glGetUniformLocation(ws->shader.program, "lightSourceCol4");
    ws->shader.lightSourcePos4  = glGetUniformLocation(ws->shader.program, "lightSourcePos4");
    ws->shader.lightSourceCoef4 = glGetUniformLocation(ws->shader.program, "lightSourceCoef4");
    ws->shader.lightSource5     = glGetUniformLocation(ws->shader.program, "lightSource5");
    ws->shader.lightSourceTyp5  = glGetUniformLocation(ws->shader.program, "lightSourceTyp5");
    ws->shader.lightSourceCol5  = glGetUniformLocation(ws->shader.program, "lightSourceCol5");
    ws->shader.lightSourcePos5  = glGetUniformLocation(ws->shader.program, "lightSourcePos5");
    ws->shader.lightSourceCoef5 = glGetUniformLocation(ws->shader.program, "lightSourceCoef5");
    ws->shader.lightSource6     = glGetUniformLocation(ws->shader.program, "lightSource6");
    ws->shader.lightSourceTyp6  = glGetUniformLocation(ws->shader.program, "lightSourceTyp6");
    ws->shader.lightSourceCol6  = glGetUniformLocation(ws->shader.program, "lightSourceCol6");
    ws->shader.lightSourcePos6  = glGetUniformLocation(ws->shader.program, "lightSourcePos6");
    ws->shader.lightSourceCoef6 = glGetUniformLocation(ws->shader.program, "lightSourceCoef6");
    /* projection matrices */
    ws->shader.ModelViewMatrix  = glGetUniformLocation(ws->shader.program, "ModelViewMatrix");
    ws->shader.ProjectionMatrix = glGetUniformLocation(ws->shader.program, "ProjectionMatrix");
    /* Texture settings */
    ws->shader.applyTexture = glGetUniformLocation(ws->shader.program, "applyTexture");
    ws->shader.sLoc = glGetUniformLocation(ws->shader.program, "sPlane");
    ws->shader.tLoc = glGetUniformLocation(ws->shader.program, "tPlane");
    glUniform4fv( ws->shader.sLoc, 1, ws->shader.s_plane);
    glUniform4fv( ws->shader.tLoc, 1, ws->shader.t_plane);
    /* location of uniform oirMode in first pass program */
    ws->shader.oirModeLoc = -1;
    ws->shader.oirModeLoc = glGetUniformLocation(ws->shader.program, "oirEnable");
    /* set shading OIR mode in default program */
    if (ws->shader.oirModeLoc >= 0){
      glProgramUniform1i(ws->shader.program, ws->shader.oirModeLoc, ws->oir.mode);
    } else {
      printf("ERROR initialising OIR mode in 1st pass program. Aborting.");
      exit(1);
    }
    /*
      Order independent rendering needs a second program to resolve the
      per pixel fragment lists. Only the 4.30 fragment shader builds those
      lists (the head pointer needs an SSBO, which is not reliably available
      as a 4.20 extension across the hardware/drivers this runs on -- see
      the comment on wsgl_oir_wanted() in wsgl_oir.c), so for every other
      version oir_program stays zero and the rendering path is exactly what
      it always was.
    */
    ws->shader.oir_program = 0;
    ws->shader.oirMode = -1;
    if (ws->oir.mode > 0 && wsgl_frag_shader_version < 430){
      fprintf(stderr, "WARNING: order independent rendering needs shader"
              " version 430 or later (configured: %d). Continuing with OIR"
              " disabled for this workstation.\n", wsgl_frag_shader_version);
    }
    else if (ws->oir.mode > 0){
      if (wsgl_frag_shader_version == 430 ){
        ws->shader.oir_program = wsgl_build_program(vertex_shader_text_430_resolve,
                                                    fragment_shader_text_430_resolve,
                                                    "OIR resolve");
        if (ws->shader.oir_program == 0){
          fprintf(stderr, "[ERROR] Could not build the order independent"
                  " rendering resolve program\n");
          abort();
        }
        ws->shader.oirMode = glGetUniformLocation(ws->shader.oir_program, "oirMode");
        printf("[INFO] Order independent rendering mode is %d\n", ws->oir.mode);
      }
      if (ws->shader.oirMode>=0) {
        glProgramUniform1i(ws->shader.oir_program, ws->shader.oirMode, ws->oir.mode);
      } else {
        printf("ERROR initialising OIR mode. Aborting.");
        exit(1);
      }
    }
    /* the geometry program has to be the current one when we return */
    glUseProgram(ws->shader.program);
  }
}
