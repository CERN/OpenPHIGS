/*
 * Checks whether pfill_area_set_data() (PFASD in Fortran -- what the DELPHI
 * dolphin logo, delgrasubs.f's KYDELP, actually uses for its coloured
 * regions, as opposed to plain pfill_area()) renders correctly under order
 * independent rendering. wsgl.c's dispatch for PELEM_FILL_AREA_SET_DATA is
 * structurally different from PELEM_FILL_AREA: when face-distinguishing
 * mode is on, it draws the shape TWICE -- once back-face-culled, once
 * front-face-culled -- which plain pfill_area() never does. That is the one
 * concrete, checkable difference found so far between "isolated/overlapping
 * pfill_area() calls work under OIR" (see tie_break_test.c,
 * concave_fill_test.c, overflow_tie_test.c, all PASS) and "the dolphin's
 * PFASD fills disappear under OIR".
 *
 * Draws the same concave ("arrow") shape as concave_fill_test.c, via
 * pfill_area_set_data() instead of pfill_area(), twice: once with the
 * default face-distinguishing mode, once with it explicitly turned on
 * (matching a hypothesis about what state might precede KYDELP's calls in
 * the real application).
 *
 * Build/run: see tie_break_test.c in this directory for the exact command.
 * Optional argument: "disting" to call pset_face_disting_mode(PDISTING_YES)
 * before drawing.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#ifdef GLEW
#include <GL/glew.h>
#else
#include <epoxy/gl.h>
#endif

#include "phg.h"

int main(int argc, char ** argv)
{
  int disting = 0, nohlhsr = 0, zbuff = 0, i;
  for (i = 1; i < argc; i++){
    if (!strcmp(argv[i], "disting")) disting = 1;
    if (!strcmp(argv[i], "nohlhsr")) nohlhsr = 1;
    if (!strcmp(argv[i], "zbuff")) zbuff = 1;
  }
  Ppoint3 verts[6] = {
    {0.2, 0.2, 0.0}, {0.8, 0.2, 0.0}, {0.8, 0.8, 0.0},
    {0.5, 0.5, 0.0}, {0.2, 0.8, 0.0}, {0.2, 0.2, 0.0}
  };
  Pfacet_data3 fdata;
  Pfacet_vdata_list3 vdata;
  Pgcolr green;
  GLubyte pixel[3];
  int width, height;

  green.type = PMODEL_RGB;
  green.val.general.x = 0.0; green.val.general.y = 1.0; green.val.general.z = 0.0;

  popen_phigs(NULL, 0);
  popen_struct(1);
  pset_int_style(PSTYLE_SOLID);
  pset_int_colr(&green);
  pset_back_int_colr(&green);
  if (disting){
    pset_face_disting_mode(PDISTING_YES);
    printf("pset_face_disting_mode(PDISTING_YES) called before the fill.\n");
  }
  vdata.num_vertices = 6;
  vdata.vertex_data.points = verts;
  pfill_area_set_data(PFACET_NONE, PEDGE_NONE, PVERT_COORD, PMODEL_RGB,
                      &fdata, 1, NULL, &vdata);
  pclose_struct();

  popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
  /* matches the DELPHI banner's own workstation: 2D content, no hidden
     surface removal, so GL_DEPTH_TEST ends up disabled (see wsgl.c) --
     unless "nohlhsr" is passed, to see what the untouched default is */
  if (zbuff) pset_hlhsr_mode(0, PHIGS_HLHSR_MODE_ZBUFF);
  else if (!nohlhsr) pset_hlhsr_mode(0, PHIGS_HLHSR_MODE_NONE);
  ppost_struct(0, 1, 0.0);
  pupd_ws(0, PFLAG_PERFORM);
  printf("GL_DEPTH_TEST enabled after the frame: %d\n",
         (int) glIsEnabled(GL_DEPTH_TEST));

  width = PHG_WSID(0)->ws_rect.width;
  height = PHG_WSID(0)->ws_rect.height;
  glReadBuffer(GL_FRONT); /* pupd_ws() already swapped */
  glReadPixels((int) (0.3 * width), (int) (0.3 * height), 1, 1,
               GL_RGB, GL_UNSIGNED_BYTE, pixel);

  printf("Pixel inside PFASD concave fill = (%d, %d, %d)\n",
         pixel[0], pixel[1], pixel[2]);
  if (pixel[1] > pixel[0] && pixel[1] > pixel[2]){
    printf("PASS: fill is green, as expected.\n");
  } else {
    printf("FAIL: fill is missing or wrong (expected green).\n");
  }

  pclose_ws(0);
  pclose_phigs();
  return 0;
}
