/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2022 CERN
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
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PHCONF_H
#define PHCONF_H
#include "phigs.h"

  static  int max_text = 30;
  typedef struct {
    int wkid; /* workstation ID */
    char window_title[30]; /* Window title */
    char window_icon[30];
    char filename[512];   /* Output filename for hard copies */
    int  set_window_pos;
    Plimit vpos;
    Pcolr_rep background_color_rgb;
    Pcolr_rep background_color_rgba;
    unsigned int display_width;
    unsigned int display_height;
    unsigned int border_width;
    int xpos, ypos;
    float hcsf; /* hard copy scale factor */
  } Pophconf;

  /* configuration file name */
  extern int config_read;
  extern int max_wkid;
  extern Pophconf config[256];

  /* read configuration file */
  void read_config(char * config_file);

#endif

#ifdef __cplusplus
}
#endif
