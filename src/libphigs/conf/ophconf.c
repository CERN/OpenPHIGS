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
*******************************************************************************/

/* handle configuration file */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "phconf.h"
#include "private/wsglP.h"
#include "ws.h"

int max_wkid = 100;
Pophconf config[256];
int config_read = 0;

void set_defaults(Pophconf* config){
    config->wkid = -1;
    strcpy(config->window_title, phg_default_window_name);
    strcpy(config->window_icon, phg_default_icon_name);
    memset(config->filename, 0, sizeof(config->filename));
    config->background_color.rgb.red = 0.;
    config->background_color.rgb.green = 0.;
    config->background_color.rgb.blue = 0.;
    config->display_width = DISPLAY_WIDTH;
    config->display_height = DISPLAY_HEIGHT;
    config->border_width = 1;
    config->xpos = 0;
    config->ypos = 0;
    config->vpos.x_min = 0.;
    config->vpos.x_max = 1.;
    config->vpos.y_min = 0.;
    config->vpos.y_max = 1.;
    config->set_window_pos = 1;
    config->hcsf = 1.0;
}

void init_defaults(){
  int i;
  /* set default values for all workstations */
  for (i=0; i <= max_wkid; i++){
    set_defaults(&config[i]);
  }
}

void query_settings(){
  int i;
  Pophconf* cf;
  /* set default values for all workstations */
  for (i=0; i <= max_wkid; i++){
    cf = &config[i];
    if (cf->wkid >= 0){
      printf("Workstation   ID %d:\n", cf->wkid);
      printf("  Title:         %s\n", cf->window_title);
      printf("  Background rgb %f %f %f\n",
             cf->background_color.rgb.red,
             cf->background_color.rgb.green,
             cf->background_color.rgb.blue);
      printf("  Width, height: %d %d\n", cf->display_width, cf->display_height);
      printf("  Position x, y: %d %d\n", cf->xpos, cf->ypos);
      printf("  Border width:  %d\n", cf->border_width);
      printf("  View port   :  (%f %f) (%f %f) \n",
             cf->vpos.x_min,
             cf->vpos.y_min,
             cf->vpos.x_max,
             cf->vpos.y_max
             );
      printf("  HC Scale fac : %f\n", cf->hcsf);
    }
  }
}

void read_config(char * config_file){
  /* read configuraton for works station from file */
  FILE* fh;
  int maxsize = 128;
  char line[129];
  char text[129];
  int wk;
  int i;
  float xmin,  xmax, ymin, ymax;
  float red, green, blue;
  float hcsf;
  unsigned int width, height, border;
  int xpos, ypos;
  Pophconf newconfig;
  int use_shaders;

  /* initialize output */
  newconfig.wkid = -1;
  /* print the full config at the end or not */
  int printconf = 0;

  /* defaults for updated configs */
  init_defaults();
  wsgl_use_shaders = 1;

  if (config_file == NULL){
    printf("No configuration file name defined. Using defaults instead.\n");
  } else {
    printf("Reading configuration from %s\n", config_file);
    config_read = 1;
    /* read the configuration file and filter for wkid */
    fh = fopen(config_file, "r");
    if (fh == NULL){
      printf("WARNING: Cannot open configuration file %s. Using defaults.\n", config_file);
      return;
    }
    /* set initial defaults for this configuration */
    set_defaults(&newconfig);
    while (fgets(line, maxsize, fh) != NULL){
      if (line[0] == '%'){
        /* get work station ID */
        if (sscanf(line, "%%wk %d", &wk)>0){
          printf("New workstation id %d\n", wk);
          if (wk > max_wkid){
            printf("Error: maximum workstation number exceeded: %d > %d\n", wk, max_wkid);
          }
          if (newconfig.wkid < 0){
            newconfig.wkid = wk;
            newconfig.set_window_pos = 0;
            strcpy(newconfig.window_title, phg_default_window_name);
            strcpy(newconfig.window_icon, phg_default_icon_name);
          } else {
            /* new config follows */
            if (newconfig.wkid < max_wkid){
              /* save the last parsed config */
              memcpy(&config[newconfig.wkid], &newconfig, sizeof(Pophconf));
              /* reset defaults for the next one */
              set_defaults(&newconfig);
              /* store workstation number */
              newconfig.wkid = wk;
            }
          }
        }
        if (sscanf(line, "%%wn %s", text) > 0){
          strncpy(newconfig.window_title, text, max_text);
        }
        if (sscanf(line, "%%wf %s", text) > 0){
          strncpy(newconfig.filename, text, max_text);
        }
        if (sscanf(line, "%%wi %s", text) > 0){
          strncpy(newconfig.window_icon, text, max_text);
        }
        if (sscanf(line, "%%wp %f %f %f %f", &xmin, &xmax, &ymin, &ymax) > 0){
          newconfig.vpos.x_min = xmin;
          newconfig.vpos.x_max = xmax;
          newconfig.vpos.y_min = ymin;
          newconfig.vpos.y_max = ymax;
          newconfig.set_window_pos = 1;
        }
        if (sscanf(line, "%%wg %d %d %d %d %d", &width, &height, &xpos, &ypos, &border) > 0){
          newconfig.display_width = width;
          newconfig.display_height = height;
          newconfig.xpos = xpos;
          newconfig.xpos = ypos;
          newconfig.border_width = border;
        }
        if (sscanf(line, "%%bg %f %f %f", &red, &green, &blue) > 0){
          newconfig.background_color.rgb.red = red;
          newconfig.background_color.rgb.green = green;
          newconfig.background_color.rgb.blue = blue;
        }
        if (sscanf(line, "%%hs %f", &hcsf) > 0){
          newconfig.hcsf = hcsf;
          printf("Scale factor  is: %f\n", hcsf);
        }
        if (sscanf(line, "%%gs %d", &use_shaders) > 0){
          if (use_shaders == 0){
            wsgl_use_shaders = 0;
            printf("Shaders are DISABLED by configuration\n");
          } else {
            wsgl_use_shaders = 1;
            printf("Shaders are ENABLED by configuration\n");
          }
        }
        if (sscanf(line, "%%pc %d", &printconf) > 0){
          if (printconf == 0){
            printf("Printing configuration will be suppressed\n");
          } else {
            printconf = 1;
          }
        }
      }
    }
    fclose(fh);
  }
  /* make sure we store the last one as well */
  if ((newconfig.wkid >= 0) && (newconfig.wkid <= max_wkid)){
    memcpy(&config[newconfig.wkid], &newconfig, sizeof(Pophconf));
  }
  if (printconf){
    query_settings();
  }
}
