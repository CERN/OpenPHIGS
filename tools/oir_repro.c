/*
 * Minimal repro: an R32UI image cleared through the ordinary texture-update
 * path (a PBO-backed glTexImage2D, exactly as an "A-buffer" / per-pixel
 * linked-list OIT implementation clears its head-pointer texture every
 * frame) is bound to image unit 0 and read back with imageLoad() in a
 * fragment shader belonging to a *separately linked* GLSL program (built
 * with plain glCreateProgram()/glAttachShader()/glLinkProgram(), the
 * program's image-unit uniform never touched by glUniform1i/glProgramUniform1i
 * — it relies purely on the `layout(binding = 0)` qualifier).
 *
 * Expected (and what Mesa does): the fragment shader's imageLoad() sees the
 * same 0xFFFFFFFF sentinel that was just uploaded, confirmed independently
 * by a CPU-side glGetTexImage() readback of the same texture object.
 *
 * Observed on at least one NVIDIA driver (580.178.04, RTX 3060, GL 4.6
 * compatibility context obtained through the legacy glXCreateContext(),
 * i.e. NOT glXCreateContextAttribsARB): the CPU-side readback correctly
 * shows 0xFFFFFFFF everywhere, but the shader's own imageLoad() of the
 * exact same texel reads back 0 -- as if no texture were bound to the image
 * unit at all -- regardless of:
 *   - clearing via glTexImage2D+PBO vs. glClearTexImage
 *   - an extra glFinish() forcing full CPU/GPU sync before the draw
 *   - moving the image unit away from 0 (tried unit 6) to rule out any
 *     aliasing with a texture unit of the same index
 *   - drawing the test quad with a VBO instead of immediate-mode glBegin/glEnd
 *
 * This program repeats that isolation in a single, dependency-light file so
 * it can be filed as an NVIDIA driver bug report or run on other vendors to
 * compare. Exit status is 0 on PASS (shader agrees with the CPU readback), 1
 * on FAIL (mismatch -- the bug), 2 on setup error.
 *
 * Build:   gcc oir_repro.c -o oir_repro -lX11 -lGL -lepoxy
 * Run:     ./oir_repro           (clears via glTexImage2D+PBO, immediate-mode quad)
 *          ./oir_repro cleartex  (clears via glClearTexImage instead)
 *          ./oir_repro vbo       (draws the quad via a VBO instead of glBegin/glEnd)
 *          ./oir_repro finish    (adds a glFinish() before the read)
 *          ./oir_repro unit6     (uses image unit 6 instead of 0)
 * Flags combine, e.g. `./oir_repro cleartex finish unit6`.
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

static const char * vs_src =
  "#version 430 compatibility\n"
  "void main() { gl_Position = gl_Vertex; }\n";

/* NOTE: the layout(binding=...) slot is filled in at runtime from image_unit,
   via the printf-style template below. */
static const char * fs_src_template =
  "#version 430 compatibility\n"
  "layout (binding = %d, r32ui) coherent uniform uimage2D img;\n"
  "void main() {\n"
  "  uint v = imageLoad(img, ivec2(5, 5)).x;\n"
  "  float r = float((v >> 16u) & 0xFFu) / 255.0;\n"
  "  float g = float((v >> 8u)  & 0xFFu) / 255.0;\n"
  "  float b = float(v & 0xFFu) / 255.0;\n"
  "  gl_FragColor = vec4(r, g, b, 1.0);\n"
  "}\n";

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

int main(int argc, char ** argv)
{
  int i;
  for (i = 1; i < argc; i++){
    if (!strcmp(argv[i], "cleartex")) opt_cleartex = 1;
    else if (!strcmp(argv[i], "finish")) opt_finish = 1;
    else if (!strcmp(argv[i], "vbo")) opt_vbo = 1;
    else if (!strcmp(argv[i], "unit6")) image_unit = 6;
    else { fprintf(stderr, "unknown option: %s\n", argv[i]); return 2; }
  }

  /* ---- X11 + legacy (non-ARB) GLX context, matching OpenPHIGS's setup ---- */
  Display * dpy = XOpenDisplay(NULL);
  if (!dpy) die("XOpenDisplay");

  int attribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };
  XVisualInfo * vi = glXChooseVisual(dpy, DefaultScreen(dpy), attribs);
  if (!vi) die("glXChooseVisual");

  Colormap cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  XSetWindowAttributes swa;
  swa.colormap = cmap;
  swa.border_pixel = 0;
  Window win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, WIDTH, HEIGHT, 0,
                             vi->depth, InputOutput, vi->visual,
                             CWColormap | CWBorderPixel, &swa);
  XMapWindow(dpy, win);
  XSync(dpy, False);

  /* the legacy entry point: no ARB_create_context, no explicit profile bit --
     this is exactly how OpenPHIGS's phg_wsx_create_context() gets its context */
  GLXContext ctx = glXCreateContext(dpy, vi, NULL, True);
  if (!ctx) die("glXCreateContext");
  if (!glXMakeCurrent(dpy, win, ctx)) die("glXMakeCurrent");

  printf("GL_VENDOR:   %s\n", (const char *) glGetString(GL_VENDOR));
  printf("GL_RENDERER: %s\n", (const char *) glGetString(GL_RENDERER));
  printf("GL_VERSION:  %s\n", (const char *) glGetString(GL_VERSION));
  printf("GLSL:        %s\n", (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("options: cleartex=%d finish=%d vbo=%d image_unit=%d\n\n",
         opt_cleartex, opt_finish, opt_vbo, image_unit);

  /* ---- the R32UI texture, allocated the same way wsgl_oir_ini() does ---- */
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, WIDTH, HEIGHT, 0,
               GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
  check_gl("initial glTexImage2D");

  /* ---- clear it to SENTINEL, either via a PBO upload or glClearTexImage ---- */
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
    for (i = 0; i < (int) n_pixels; i++) data[i] = SENTINEL;
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, WIDTH, HEIGHT, 0,
                 GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    check_gl("PBO-backed glTexImage2D clear");
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glDeleteBuffers(1, &pbo);
  }

  /* ---- CPU-side ground truth: read the texture back directly, by name ---- */
  GLuint cpu_check[WIDTH * HEIGHT];
  glBindTexture(GL_TEXTURE_2D, tex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, cpu_check);
  check_gl("glGetTexImage ground truth");
  GLuint cpu_value = cpu_check[5 * WIDTH + 5];
  printf("CPU-side glGetTexImage at (5,5): 0x%08x  (expected 0x%08x)  %s\n",
         cpu_value, SENTINEL, cpu_value == SENTINEL ? "OK" : "MISMATCH");

  /* ---- bind as an image, exactly as wsgl_oir_reset() does ---- */
  glBindImageTexture(image_unit, tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
  check_gl("glBindImageTexture");
  glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  check_gl("glMemoryBarrier");
  if (opt_finish) glFinish();

  /* confirm the binding metadata itself is correct, independent of the shader */
  GLint bound_name = -1;
  glGetIntegeri_v(GL_IMAGE_BINDING_NAME, image_unit, &bound_name);
  printf("GL_IMAGE_BINDING_NAME at unit %d: %d  (expected %u)  %s\n\n",
         image_unit, bound_name, tex, (GLuint) bound_name == tex ? "OK" : "MISMATCH");

  /* ---- build the program the same way wsgl_build_program() does: plain
     glCreateProgram()/glAttachShader()/glLinkProgram(), no glUniform1i on
     the image uniform -- it relies solely on layout(binding=N) ---- */
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

  if (opt_vbo){
    GLfloat quad[] = {
      -1.0f, -1.0f, 0.0f, 1.0f,
       1.0f, -1.0f, 0.0f, 1.0f,
       1.0f,  1.0f, 0.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 1.0f,
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
  check_gl("draw");
  glFinish();

  GLubyte pixel[4];
  glReadPixels(10, 10, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
  check_gl("glReadPixels");

  GLuint shader_value = ((GLuint) pixel[0] << 16) | ((GLuint) pixel[1] << 8) | pixel[2];
  printf("Shader-side imageLoad() decoded from framebuffer: 0x%06x"
         " (low 24 bits of 0x%08x expected)\n", shader_value, SENTINEL & 0xFFFFFFu);

  int pass = (shader_value == (SENTINEL & 0xFFFFFFu));
  printf("\n%s: shader %s the value the CPU readback confirmed was actually"
         " written to the texture.\n",
         pass ? "PASS" : "FAIL",
         pass ? "sees" : "does NOT see");

  return pass ? 0 : 1;
}
