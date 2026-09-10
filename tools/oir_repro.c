/*
 * Repro (and workaround exploration) for a suspected NVIDIA driver bug
 * affecting order independent rendering (`%oir` in phigs.def): a GL_R32UI
 * image cleared through the normal texture-update path and bound to an
 * image unit is not seen correctly by imageLoad() in a separately linked
 * GLSL program built with plain glCreateProgram()/glAttachShader()/
 * glLinkProgram() (the program's image-unit uniform never touched by
 * glUniform1i/glProgramUniform1i -- it relies purely on the
 * `layout(binding = 0)` qualifier).
 *
 * Expected (and what Mesa does): the fragment shader's imageLoad() sees the
 * same sentinel value that was just uploaded, confirmed independently by a
 * CPU-side glGetTexImage() readback of the same texture object.
 *
 * Observed on at least one NVIDIA driver (580.178.04, RTX 3060, GL 4.6
 * compatibility context obtained through the legacy glXCreateContext(), i.e.
 * NOT glXCreateContextAttribsARB): the CPU-side readback correctly shows the
 * sentinel everywhere, but the shader's own imageLoad() of the exact same
 * texel reads back 0 -- as if no texture were bound to the image unit at all.
 *
 * This file has two purposes:
 *   1. `./oir_repro` (mode "image", the default) reproduces the bug in the
 *      exact shape OpenPHIGS's wsgl_oir.c uses it, with flags to vary clear
 *      method / sync / draw style / image unit (see below) -- all of which
 *      were tried live and made no difference.
 *   2. `./oir_repro <mode>` tries a specific workaround candidate instead:
 *        ssbo       - head pointer as a std430 SSBO instead of a uimage2D,
 *                     plain write (glMapBuffer) then plain read
 *        ssboatomic - the same, but pass 1 writes via atomicExchange() (as
 *                     appendFragment() needs to) instead of a plain write,
 *                     and pass 2 is a *separately linked* program, matching
 *                     the real two-pass wsgl_oir.c shape exactly
 *        listbuf    - the uimageBuffer/GL_TEXTURE_BUFFER approach
 *                     list_buffer already uses, tested in isolation, to see
 *                     whether it has the same problem as the uimage2D head
 *                     pointer or is already fine being buffer-backed
 *        uniform    - explicit glUniform1i on the image unit, on top of "image"
 *        core       - a core-profile context (glXCreateContextAttribsARB)
 *                     instead of the legacy compatibility context, with the
 *                     shaders and vertex setup ported to match
 *        sso        - separable shader objects (glCreateShaderProgramv +
 *                     a program pipeline) instead of glCreateProgram/
 *                     glAttachShader/glLinkProgram
 *      Each prints PASS/FAIL exactly like the baseline, so a workaround
 *      candidate that flips FAIL to PASS is a real fix to port back into
 *      wsgl_oir.c / the fs430 shaders.
 *
 * Results on the NVIDIA RTX 3060 above: image/uniform/core/sso all FAIL;
 * ssbo/ssboatomic/listbuf all PASS. Conclusion: only head_pointer_image
 * needs to move from a uimage2D to an SSBO; list_buffer is already using a
 * storage kind (uimageBuffer) that is unaffected.
 *
 * Build:   gcc oir_repro.c -o oir_repro -lX11 -lGL -lepoxy
 * Run:     ./oir_repro                        (mode "image", baseline)
 *          ./oir_repro cleartex vbo finish unit6   (flags modify mode "image")
 *          ./oir_repro ssbo
 *          ./oir_repro ssboatomic
 *          ./oir_repro listbuf
 *          ./oir_repro uniform
 *          ./oir_repro core
 *          ./oir_repro sso
 *
 * Exit status: 0 = PASS, 1 = FAIL (bug reproduces), 2 = setup error.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>

#define WIDTH  64
#define HEIGHT 64
/* distinctive, non-trivial sentinel: not all-0s, not all-1s, easy to spot */
#define SENTINEL 0x00123456u

static int opt_cleartex = 0;
static int opt_finish   = 0;
static int opt_vbo      = 0;
static int image_unit   = 0;
static const char * mode = "image";

static Display * dpy;
static Window win;

static void die(const char * msg)
{
  fprintf(stderr, "FATAL: %s\n", msg);
  exit(2);
}

static GLuint compile(GLenum type, const char * src)
{
  GLuint sh = glCreateShader(type);
  GLint ok;
  glShaderSource(sh, 1, &src, NULL);
  glCompileShader(sh);
  glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
  if (!ok){
    char log[4096];
    glGetShaderInfoLog(sh, sizeof(log), NULL, log);
    fprintf(stderr, "shader compile failed:\n%s\nsource:\n%s\n", log, src);
    die("shader compile");
  }
  return sh;
}

static void check_gl(const char * where)
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR){
    fprintf(stderr, "[GL ERROR] %s: 0x%04x\n", where, err);
  }
}

static void report(GLuint low24)
{
  printf("Shader-side result decoded from framebuffer: 0x%06x"
         " (low 24 bits of 0x%08x expected)\n", low24, SENTINEL & 0xFFFFFFu);
  int pass = (low24 == (SENTINEL & 0xFFFFFFu));
  printf("\n%s [%s]: shader %s the value the CPU readback confirmed was"
         " actually written.\n", pass ? "PASS" : "FAIL", mode,
         pass ? "sees" : "does NOT see");
  exit(pass ? 0 : 1);
}

/* legacy (non-ARB) context: no profile bit, exactly OpenPHIGS's own setup */
static GLXContext make_legacy_context(XVisualInfo ** out_vi)
{
  int attribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };
  XVisualInfo * vi = glXChooseVisual(dpy, DefaultScreen(dpy), attribs);
  if (!vi) die("glXChooseVisual");
  GLXContext ctx = glXCreateContext(dpy, vi, NULL, True);
  if (!ctx) die("glXCreateContext");
  *out_vi = vi;
  return ctx;
}

/* core-profile context via GLX_ARB_create_context_profile */
static GLXContext make_core_context(XVisualInfo ** out_vi)
{
  int fbattribs[] = {
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_DOUBLEBUFFER, True,
    GLX_DEPTH_SIZE, 16,
    None
  };
  int nfb = 0;
  GLXFBConfig * fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), fbattribs, &nfb);
  if (!fbc || nfb == 0) die("glXChooseFBConfig");

  int ctx_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
    GLX_CONTEXT_MINOR_VERSION_ARB, 5,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };
  GLXContext ctx = glXCreateContextAttribsARB(dpy, fbc[0], NULL, True, ctx_attribs);
  if (!ctx) die("glXCreateContextAttribsARB (core profile)");
  *out_vi = glXGetVisualFromFBConfig(dpy, fbc[0]);
  if (!*out_vi) die("glXGetVisualFromFBConfig");
  return ctx;
}

static void make_window(XVisualInfo * vi)
{
  Colormap cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  XSetWindowAttributes swa;
  swa.colormap = cmap;
  swa.border_pixel = 0;
  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, WIDTH, HEIGHT, 0,
                      vi->depth, InputOutput, vi->visual,
                      CWColormap | CWBorderPixel, &swa);
  XMapWindow(dpy, win);
  XSync(dpy, False);
}

static void print_context_info(void)
{
  printf("GL_VENDOR:   %s\n", (const char *) glGetString(GL_VENDOR));
  printf("GL_RENDERER: %s\n", (const char *) glGetString(GL_RENDERER));
  printf("GL_VERSION:  %s\n", (const char *) glGetString(GL_VERSION));
  printf("GLSL:        %s\n", (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("mode: %s  (cleartex=%d finish=%d vbo=%d image_unit=%d)\n\n",
         mode, opt_cleartex, opt_finish, opt_vbo, image_unit);
}

/* ---- shared texture setup/clear/ground-truth-check, used by "image",
   "uniform" and "sso" modes (everything that keeps the uimage2D approach) ---- */
static GLuint make_and_clear_r32ui_texture(void)
{
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, WIDTH, HEIGHT, 0,
               GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  check_gl("initial glTexImage2D");

  if (opt_cleartex){
    GLuint sentinel = SENTINEL;
    glClearTexImage(tex, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &sentinel);
    check_gl("glClearTexImage");
  } else {
    size_t n_pixels = (size_t) WIDTH * HEIGHT;
    GLuint pbo;
    glGenBuffers(1, &pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, n_pixels * sizeof(GLuint), NULL, GL_STATIC_DRAW);
    GLuint * data = (GLuint *) glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    if (!data) die("glMapBuffer");
    for (size_t i = 0; i < n_pixels; i++) data[i] = SENTINEL;
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, WIDTH, HEIGHT, 0,
                 GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    check_gl("PBO-backed glTexImage2D clear");
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glDeleteBuffers(1, &pbo);
  }

  GLuint cpu_check[WIDTH * HEIGHT];
  glBindTexture(GL_TEXTURE_2D, tex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, cpu_check);
  check_gl("glGetTexImage ground truth");
  GLuint cpu_value = cpu_check[5 * WIDTH + 5];
  printf("CPU-side glGetTexImage at (5,5): 0x%08x  (expected 0x%08x)  %s\n",
         cpu_value, SENTINEL, cpu_value == SENTINEL ? "OK" : "MISMATCH");
  return tex;
}

static void draw_fullscreen_quad_legacy(void)
{
  if (opt_vbo){
    GLfloat quad[] = {
      -1.0f, -1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
       1.0f,  1.0f, 0.0f, 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
    };
    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(4, GL_FLOAT, 0, 0);
    glDrawArrays(GL_QUADS, 0, 4);
  } else {
    glBegin(GL_QUADS);
      glVertex4f(-1.0f, -1.0f, 0.0f, 1.0f);
      glVertex4f( 1.0f, -1.0f, 0.0f, 1.0f);
      glVertex4f( 1.0f,  1.0f, 0.0f, 1.0f);
      glVertex4f(-1.0f,  1.0f, 0.0f, 1.0f);
    glEnd();
  }
}

static GLuint read_low24_from_fb(void)
{
  GLubyte pixel[4];
  check_gl("draw");
  glFinish();
  glReadPixels(10, 10, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
  check_gl("glReadPixels");
  return ((GLuint) pixel[0] << 16) | ((GLuint) pixel[1] << 8) | pixel[2];
}

/* ======================================================================
 * mode "image": the baseline repro, matching wsgl_oir.c as closely as
 * possible (legacy context, layout(binding=N) only, plain link).
 * ====================================================================== */
static void run_mode_image(int explicit_uniform)
{
  static const char * vs_src =
    "#version 430 compatibility\n"
    "void main() { gl_Position = gl_Vertex; }\n";
  static const char * fs_src_template =
    "#version 430 compatibility\n"
    "layout (binding = %d, r32ui) coherent uniform uimage2D img;\n"
    "void main() {\n"
    "  uint v = imageLoad(img, ivec2(5, 5)).x;\n"
    "  gl_FragColor = vec4(float((v>>16u)&0xFFu)/255.0,\n"
    "                      float((v>>8u)&0xFFu)/255.0,\n"
    "                      float(v&0xFFu)/255.0, 1.0);\n"
    "}\n";

  XVisualInfo * vi;
  GLXContext ctx = make_legacy_context(&vi);
  make_window(vi);
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");
  print_context_info();

  GLuint tex = make_and_clear_r32ui_texture();
  glBindImageTexture(image_unit, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
  check_gl("glBindImageTexture");
  glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  if (opt_finish) glFinish();

  GLint bound_name = -1;
  glGetIntegeri_v(GL_IMAGE_BINDING_NAME, image_unit, &bound_name);
  printf("GL_IMAGE_BINDING_NAME at unit %d: %d  (expected %u)  %s\n\n",
         image_unit, bound_name, tex, (GLuint) bound_name == tex ? "OK" : "MISMATCH");

  char fs_src[1024];
  snprintf(fs_src, sizeof(fs_src), fs_src_template, image_unit);
  GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
  GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glLinkProgram(prog);
  GLint linked;
  glGetProgramiv(prog, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(prog, sizeof(log), NULL, log);
    fprintf(stderr, "link failed:\n%s\n", log);
    die("link");
  }
  glUseProgram(prog);

  if (explicit_uniform){
    GLint loc = glGetUniformLocation(prog, "img");
    printf("explicit glUniform1i(\"img\", %d) at location %d\n", image_unit, loc);
    if (loc >= 0) glUniform1i(loc, image_unit);
  }
  check_gl("glUseProgram / glUniform1i");

  glViewport(0, 0, WIDTH, HEIGHT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  draw_fullscreen_quad_legacy();
  report(read_low24_from_fb());
}

/* ======================================================================
 * mode "ssbo": head pointer as a std430 SSBO instead of a uimage2D.
 * ====================================================================== */
static void run_mode_ssbo(void)
{
  static const char * vs_src =
    "#version 430 compatibility\n"
    "void main() { gl_Position = gl_Vertex; }\n";
  static const char * fs_src =
    "#version 430 compatibility\n"
    "layout (std430, binding = 0) buffer HeadPointers { uint heads[]; };\n"
    "void main() {\n"
    "  uint v = heads[5 * 64 + 5];\n"
    "  gl_FragColor = vec4(float((v>>16u)&0xFFu)/255.0,\n"
    "                      float((v>>8u)&0xFFu)/255.0,\n"
    "                      float(v&0xFFu)/255.0, 1.0);\n"
    "}\n";

  XVisualInfo * vi;
  GLXContext ctx = make_legacy_context(&vi);
  make_window(vi);
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");
  print_context_info();

  size_t n_pixels = (size_t) WIDTH * HEIGHT;
  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, n_pixels * sizeof(GLuint), NULL, GL_STATIC_DRAW);
  if (opt_cleartex){
    /* the efficient, GPU-side fill a real per-frame reset would use */
    GLuint sentinel = SENTINEL;
    glClearBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, 0,
                         n_pixels * sizeof(GLuint),
                         GL_RED_INTEGER, GL_UNSIGNED_INT, &sentinel);
    check_gl("glClearBufferSubData");
  } else {
    GLuint * data = (GLuint *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    if (!data) die("glMapBuffer (SSBO)");
    for (size_t i = 0; i < n_pixels; i++) data[i] = SENTINEL;
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    check_gl("SSBO fill");
  }

  GLuint cpu_check[WIDTH * HEIGHT];
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, n_pixels * sizeof(GLuint), cpu_check);
  GLuint cpu_value = cpu_check[5 * WIDTH + 5];
  printf("CPU-side glGetBufferSubData at (5,5): 0x%08x  (expected 0x%08x)  %s\n",
         cpu_value, SENTINEL, cpu_value == SENTINEL ? "OK" : "MISMATCH");

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
  glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
  if (opt_finish) glFinish();
  check_gl("glBindBufferBase / glMemoryBarrier");

  GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
  GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glLinkProgram(prog);
  GLint linked;
  glGetProgramiv(prog, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(prog, sizeof(log), NULL, log);
    fprintf(stderr, "link failed:\n%s\n", log);
    die("link");
  }
  glUseProgram(prog);
  check_gl("glUseProgram");

  glViewport(0, 0, WIDTH, HEIGHT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  draw_fullscreen_quad_legacy();
  report(read_low24_from_fb());
}

/* ======================================================================
 * mode "ssboatomic": the full two-pass shape wsgl_oir.c actually needs --
 * pass 1 (one program) atomicExchange()s a value into an SSBO cell from the
 * fragment shader, pass 2 (a *separately linked* program, like the real
 * resolve pass) plain-reads that same cell. "ssbo" above only proved a
 * plain SSBO write-then-read works; this proves the atomic write path
 * appendFragment() needs also survives the cross-program boundary.
 * ====================================================================== */
static void run_mode_ssboatomic(void)
{
  static const char * vs_src =
    "#version 430 compatibility\n"
    "void main() { gl_Position = gl_Vertex; }\n";
  /* pass 1: every fragment atomicExchange()s SENTINEL into its own cell,
     mirroring appendFragment()'s atomicExchange on the head pointer. */
  static const char * fs_pass1_src =
    "#version 430 compatibility\n"
    "layout (std430, binding = 0) coherent buffer HeadPointers { uint heads[]; };\n"
    "void main() {\n"
    "  ivec2 c = ivec2(gl_FragCoord.xy);\n"
    "  uint idx = uint(c.y) * 64u + uint(c.x);\n"
    "  uint old = atomicExchange(heads[idx], 0x00123456u);\n"
    "  gl_FragColor = vec4(0.0, 0.0, 0.0, float(old) * 0.0 + 1.0);\n" /* keep 'old' live */
    "}\n";
  /* pass 2: a *different*, separately linked program, plain-reads it back */
  static const char * fs_pass2_src =
    "#version 430 compatibility\n"
    "layout (std430, binding = 0) readonly buffer HeadPointers { uint heads[]; };\n"
    "void main() {\n"
    "  uint v = heads[5u * 64u + 5u];\n"
    "  gl_FragColor = vec4(float((v>>16u)&0xFFu)/255.0,\n"
    "                      float((v>>8u)&0xFFu)/255.0,\n"
    "                      float(v&0xFFu)/255.0, 1.0);\n"
    "}\n";

  XVisualInfo * vi;
  GLXContext ctx = make_legacy_context(&vi);
  make_window(vi);
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");
  print_context_info();

  size_t n_pixels = (size_t) WIDTH * HEIGHT;
  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, n_pixels * sizeof(GLuint), NULL, GL_DYNAMIC_COPY);
  GLuint list_end = 0xFFFFFFFFu;
  glClearBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, 0,
                       n_pixels * sizeof(GLuint), GL_RED_INTEGER, GL_UNSIGNED_INT, &list_end);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
  glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
  check_gl("ssboatomic: initial clear");

  /* pass 1: build and run the atomic-write program */
  GLuint vs1 = compile(GL_VERTEX_SHADER, vs_src);
  GLuint fs1 = compile(GL_FRAGMENT_SHADER, fs_pass1_src);
  GLuint prog1 = glCreateProgram();
  glAttachShader(prog1, vs1);
  glAttachShader(prog1, fs1);
  glLinkProgram(prog1);
  GLint linked;
  glGetProgramiv(prog1, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(prog1, sizeof(log), NULL, log);
    fprintf(stderr, "pass1 link failed:\n%s\n", log);
    die("link pass1");
  }
  glUseProgram(prog1);
  glViewport(0, 0, WIDTH, HEIGHT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  draw_fullscreen_quad_legacy();
  check_gl("ssboatomic: pass1 draw");
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);
  if (opt_finish) glFinish();

  GLuint cpu_check[WIDTH * HEIGHT];
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, n_pixels * sizeof(GLuint), cpu_check);
  GLuint cpu_value = cpu_check[5 * WIDTH + 5];
  printf("CPU-side glGetBufferSubData after pass1's atomicExchange: 0x%08x"
         " (expected 0x%08x)  %s\n", cpu_value, SENTINEL,
         cpu_value == SENTINEL ? "OK" : "MISMATCH");

  /* pass 2: a separately linked program plain-reads the same cell */
  GLuint vs2 = compile(GL_VERTEX_SHADER, vs_src);
  GLuint fs2 = compile(GL_FRAGMENT_SHADER, fs_pass2_src);
  GLuint prog2 = glCreateProgram();
  glAttachShader(prog2, vs2);
  glAttachShader(prog2, fs2);
  glLinkProgram(prog2);
  glGetProgramiv(prog2, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(prog2, sizeof(log), NULL, log);
    fprintf(stderr, "pass2 link failed:\n%s\n", log);
    die("link pass2");
  }
  glUseProgram(prog2);
  check_gl("ssboatomic: pass2 glUseProgram");
  draw_fullscreen_quad_legacy();
  report(read_low24_from_fb());
}

/* ======================================================================
 * mode "listbuf": exercises the *other* image OpenPHIGS's OIR path uses --
 * a uimageBuffer wrapping a GL_TEXTURE_BUFFER, exactly like list_buffer in
 * fs430.frag/fs430_resolve.frag -- to see whether that one has the same
 * problem as the uimage2D head pointer, or whether being buffer-backed
 * already dodges it (the way the plain SSBO in "ssbo" mode does).
 * ====================================================================== */
static void run_mode_listbuf(void)
{
  static const char * vs_src =
    "#version 430 compatibility\n"
    "void main() { gl_Position = gl_Vertex; }\n";
  static const char * fs_src_template =
    "#version 430 compatibility\n"
    "layout (binding = %d, rgba32ui) coherent uniform uimageBuffer buf;\n"
    "void main() {\n"
    "  uint v = imageLoad(buf, 5).y;\n" /* .y mirrors how list_buffer stores colour */
    "  gl_FragColor = vec4(float((v>>16u)&0xFFu)/255.0,\n"
    "                      float((v>>8u)&0xFFu)/255.0,\n"
    "                      float(v&0xFFu)/255.0, 1.0);\n"
    "}\n";

  XVisualInfo * vi;
  GLXContext ctx = make_legacy_context(&vi);
  make_window(vi);
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");
  print_context_info();

  /* a texture buffer, RGBA32UI, mirroring frag_storage_buffer/texture */
  size_t n_entries = (size_t) WIDTH * HEIGHT;
  GLuint buf, tbo;
  glGenBuffers(1, &buf);
  glBindBuffer(GL_TEXTURE_BUFFER, buf);
  glBufferData(GL_TEXTURE_BUFFER, n_entries * 4 * sizeof(GLuint), NULL, GL_STATIC_DRAW);
  GLuint * data = (GLuint *) glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);
  if (!data) die("glMapBuffer (texture buffer)");
  for (size_t i = 0; i < n_entries; i++){
    data[i * 4 + 0] = 0;
    data[i * 4 + 1] = SENTINEL; /* .y, where fs430.frag packs the colour */
    data[i * 4 + 2] = 0;
    data[i * 4 + 3] = 0;
  }
  glUnmapBuffer(GL_TEXTURE_BUFFER);
  check_gl("texture buffer fill");

  glGenTextures(1, &tbo);
  glBindTexture(GL_TEXTURE_BUFFER, tbo);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, buf);
  check_gl("glTexBuffer");

  GLuint cpu_check[WIDTH * HEIGHT * 4];
  glGetBufferSubData(GL_TEXTURE_BUFFER, 0, n_entries * 4 * sizeof(GLuint), cpu_check);
  GLuint cpu_value = cpu_check[5 * 4 + 1];
  printf("CPU-side glGetBufferSubData, entry 5 .y: 0x%08x  (expected 0x%08x)  %s\n",
         cpu_value, SENTINEL, cpu_value == SENTINEL ? "OK" : "MISMATCH");

  glBindImageTexture(image_unit, tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32UI);
  check_gl("glBindImageTexture");
  glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  if (opt_finish) glFinish();

  char fs_src[1024];
  snprintf(fs_src, sizeof(fs_src), fs_src_template, image_unit);
  GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
  GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glLinkProgram(prog);
  GLint linked;
  glGetProgramiv(prog, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(prog, sizeof(log), NULL, log);
    fprintf(stderr, "link failed:\n%s\n", log);
    die("link");
  }
  glUseProgram(prog);
  check_gl("glUseProgram");

  glViewport(0, 0, WIDTH, HEIGHT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  draw_fullscreen_quad_legacy();
  report(read_low24_from_fb());
}

/* ======================================================================
 * mode "core": GL 4.5 core-profile context instead of the legacy
 * compatibility context, otherwise following the "image" approach.
 * ====================================================================== */
static void run_mode_core(void)
{
  static const char * vs_src =
    "#version 430 core\n"
    "layout (location = 0) in vec4 vPos;\n"
    "void main() { gl_Position = vPos; }\n";
  static const char * fs_src_template =
    "#version 430 core\n"
    "layout (binding = %d, r32ui) coherent uniform uimage2D img;\n"
    "out vec4 fragColour;\n"
    "void main() {\n"
    "  uint v = imageLoad(img, ivec2(5, 5)).x;\n"
    "  fragColour = vec4(float((v>>16u)&0xFFu)/255.0,\n"
    "                     float((v>>8u)&0xFFu)/255.0,\n"
    "                     float(v&0xFFu)/255.0, 1.0);\n"
    "}\n";

  XVisualInfo * vi;
  GLXContext ctx = make_core_context(&vi);
  make_window(vi);
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");
  print_context_info();

  GLuint tex = make_and_clear_r32ui_texture();
  glBindImageTexture(image_unit, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
  check_gl("glBindImageTexture");
  glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  if (opt_finish) glFinish();

  GLint bound_name = -1;
  glGetIntegeri_v(GL_IMAGE_BINDING_NAME, image_unit, &bound_name);
  printf("GL_IMAGE_BINDING_NAME at unit %d: %d  (expected %u)  %s\n\n",
         image_unit, bound_name, tex, (GLuint) bound_name == tex ? "OK" : "MISMATCH");

  char fs_src[1024];
  snprintf(fs_src, sizeof(fs_src), fs_src_template, image_unit);
  GLuint vs = compile(GL_VERTEX_SHADER, vs_src);
  GLuint fs = compile(GL_FRAGMENT_SHADER, fs_src);
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glLinkProgram(prog);
  GLint linked;
  glGetProgramiv(prog, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(prog, sizeof(log), NULL, log);
    fprintf(stderr, "link failed:\n%s\n", log);
    die("link");
  }
  glUseProgram(prog);
  check_gl("glUseProgram");

  /* core profile has no immediate mode / client-state arrays: VBO + VAO only */
  GLfloat quad[] = {
    -1.0f, -1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
  };
  GLuint vbo, vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, WIDTH, HEIGHT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  report(read_low24_from_fb());
}

/* ======================================================================
 * mode "sso": separable shader objects (glCreateShaderProgramv) + a
 * program pipeline, instead of glCreateProgram/glAttachShader/glLinkProgram.
 * ====================================================================== */
static void run_mode_sso(void)
{
  static const char * vs_src =
    "#version 430 compatibility\n"
    "out gl_PerVertex { vec4 gl_Position; };\n"
    "void main() { gl_Position = gl_Vertex; }\n";
  static const char * fs_src_template =
    "#version 430 compatibility\n"
    "layout (binding = %d, r32ui) coherent uniform uimage2D img;\n"
    "void main() {\n"
    "  uint v = imageLoad(img, ivec2(5, 5)).x;\n"
    "  gl_FragColor = vec4(float((v>>16u)&0xFFu)/255.0,\n"
    "                      float((v>>8u)&0xFFu)/255.0,\n"
    "                      float(v&0xFFu)/255.0, 1.0);\n"
    "}\n";

  XVisualInfo * vi;
  GLXContext ctx = make_legacy_context(&vi);
  make_window(vi);
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");
  print_context_info();

  GLuint tex = make_and_clear_r32ui_texture();
  glBindImageTexture(image_unit, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
  check_gl("glBindImageTexture");
  glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  if (opt_finish) glFinish();

  char fs_src[1024];
  snprintf(fs_src, sizeof(fs_src), fs_src_template, image_unit);

  const char * fs_src_ptr = fs_src;
  GLuint vs_prog = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vs_src);
  GLuint fs_prog = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fs_src_ptr);
  GLint linked;
  glGetProgramiv(vs_prog, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(vs_prog, sizeof(log), NULL, log);
    fprintf(stderr, "vertex separable program failed:\n%s\n", log);
    die("link (sso vs)");
  }
  glGetProgramiv(fs_prog, GL_LINK_STATUS, &linked);
  if (!linked){
    char log[4096];
    glGetProgramInfoLog(fs_prog, sizeof(log), NULL, log);
    fprintf(stderr, "fragment separable program failed:\n%s\n", log);
    die("link (sso fs)");
  }

  GLuint pipeline;
  glGenProgramPipelines(1, &pipeline);
  glBindProgramPipeline(pipeline);
  glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vs_prog);
  glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fs_prog);
  check_gl("program pipeline setup");

  glViewport(0, 0, WIDTH, HEIGHT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  draw_fullscreen_quad_legacy();
  report(read_low24_from_fb());
}

int main(int argc, char ** argv)
{
  int i;
  for (i = 1; i < argc; i++){
    if (!strcmp(argv[i], "cleartex")) opt_cleartex = 1;
    else if (!strcmp(argv[i], "finish")) opt_finish = 1;
    else if (!strcmp(argv[i], "vbo")) opt_vbo = 1;
    else if (!strcmp(argv[i], "unit6")) image_unit = 6;
    else if (!strcmp(argv[i], "image") || !strcmp(argv[i], "ssbo") ||
             !strcmp(argv[i], "uniform") || !strcmp(argv[i], "core") ||
             !strcmp(argv[i], "sso") || !strcmp(argv[i], "listbuf") ||
             !strcmp(argv[i], "ssboatomic")) mode = argv[i];
    else { fprintf(stderr, "unknown option: %s\n", argv[i]); return 2; }
  }

  dpy = XOpenDisplay(NULL);
  if (!dpy) die("XOpenDisplay");

  if (!strcmp(mode, "image")) run_mode_image(0);
  else if (!strcmp(mode, "uniform")) run_mode_image(1);
  else if (!strcmp(mode, "ssbo")) run_mode_ssbo();
  else if (!strcmp(mode, "ssboatomic")) run_mode_ssboatomic();
  else if (!strcmp(mode, "listbuf")) run_mode_listbuf();
  else if (!strcmp(mode, "core")) run_mode_core();
  else if (!strcmp(mode, "sso")) run_mode_sso();
  else die("unknown mode");

  return 2; /* run_mode_* always exit()s via report() */
}
