/*
 * Checks pfill_area_set_data() with PINDIRECT-typed per-vertex colours
 * (colr_type = PINDIRECT = 0), exactly matching how the DELPHI dolphin logo
 * (delgrasubs.f's KYDELP) calls PFASD: e.g.
 *   CALL PFASD(PFNO, PEVF, PCD, 0, 0, 152, ..., VCOLI, ...)
 * where VCOLI is filled with small integer colour-TABLE indices (VFILL
 * (VCOLI, NPT6, IGREEN)), not literal RGB floats, and the 4th argument (0)
 * is the colr_type = PINDIRECT.
 *
 * wsgl_set_colr() in wsgl_attr.c handles PINDIRECT with a bare glIndexi(),
 * which does not touch the vCOLOR vertex attribute fs420/fs430.frag's
 * fragment shader actually reads -- worth checking directly whether that
 * leaves the fill in some fixed/stale colour, or genuinely invisible, and
 * whether that differs with OIR on vs off.
 *
 * Sets up a colour table entry (index 5) to bright green via
 * pset_colr_rep(), then fills a quad using PFASD with a PINDIRECT vertex
 * colour of index 5, and reports the centre pixel.
 *
 * Build/run: see tie_break_test.c in this directory for the exact command.
 */
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#ifdef GLEW
#include <GL/glew.h>
#else
#include <epoxy/gl.h>
#endif

#include "phg.h"

int main(void)
{
  Ppoint3 verts[4] = {
    {0.2, 0.2, 0.0}, {0.8, 0.2, 0.0}, {0.8, 0.8, 0.0}, {0.2, 0.8, 0.0}
  };
  Pptco3 vcolrs[4];
  Pfacet_data3 fdata;
  Pfacet_vdata_list3 vdata;
  Pcolr_rep col_rep;
  GLubyte pixel[3];
  int width, height, i;

  popen_phigs(NULL, 0);
  popen_struct(1);
  pset_int_style(PSTYLE_SOLID);
  for (i = 0; i < 4; i++){
    vcolrs[i].point = verts[i];
    vcolrs[i].colr.ind = 5; /* colour table index 5, set up below */
  }
  vdata.num_vertices = 4;
  vdata.vertex_data.ptcolrs = vcolrs;
  pfill_area_set_data(PFACET_NONE, PEDGE_NONE, PVERT_COORD_COLOUR, PINDIRECT,
                      &fdata, 1, NULL, &vdata);
  pclose_struct();

  popen_ws(0, NULL, PWST_OUTPUT_TRUE_DB);
  col_rep.rgb.red = 0.0;
  col_rep.rgb.green = 1.0;
  col_rep.rgb.blue = 0.0;
  pset_colr_rep(0, 5, &col_rep); /* colour table index 5 = bright green */
  ppost_struct(0, 1, 0.0);
  pupd_ws(0, PFLAG_PERFORM);

  width = PHG_WSID(0)->ws_rect.width;
  height = PHG_WSID(0)->ws_rect.height;
  glReadBuffer(GL_FRONT); /* pupd_ws() already swapped */
  glReadPixels(width / 2, height / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

  printf("Centre pixel (PINDIRECT vertex colour, index 5 = green) = (%d, %d, %d)\n",
         pixel[0], pixel[1], pixel[2]);
  if (pixel[1] > pixel[0] && pixel[1] > pixel[2]){
    printf("PASS: fill is green, as expected.\n");
  } else {
    printf("FAIL: fill is not green (got %d,%d,%d) -- PINDIRECT colour not"
           " resolved correctly.\n", pixel[0], pixel[1], pixel[2]);
  }

  pclose_ws(0);
  pclose_phigs();
  return 0;
}
