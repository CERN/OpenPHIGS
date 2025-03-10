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

int max_wkid = 40;
Pophconf config[256];
int config_read = 0;

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
  Pophconf newconfig;
  int use_shaders;

  /* initialize output */
  newconfig.wkid = -1;
  for (i=0; i < max_wkid; i++){
    config[i].wkid = -1;
    strcpy(config[i].window_title, phg_default_window_name);
    strcpy(config[i].window_icon, phg_default_icon_name);
    memset(config[i].filename, 0, sizeof(config[i].filename));
    config[i].background_color.rgb.red = 0.;
    config[i].background_color.rgb.green = 0.;
    config[i].background_color.rgb.blue = 0.;
  }
  strcpy(newconfig.window_title, phg_default_window_name);
  strcpy(newconfig.window_icon, phg_default_icon_name);
  memset(newconfig.filename, 0, sizeof(newconfig.filename));
  newconfig.background_color.rgb.red = 0.;
  newconfig.background_color.rgb.green = 0.;
  newconfig.background_color.rgb.blue = 0.;

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
    while (fgets(line, maxsize, fh) != NULL){
      if (line[0] == '%'){
	/* get work station ID */
	if (sscanf(line, "%%wk %d", &wk)>0){
	  if (newconfig.wkid < 0){
	    newconfig.wkid = wk;
	    newconfig.set_window_pos = 0;
	    strcpy(newconfig.window_title, phg_default_window_name);
	    strcpy(newconfig.window_icon, phg_default_icon_name);
	  } else {
	    /* new config follows - store the parsed one */
	    if (newconfig.wkid < max_wkid){
	      memcpy(&config[newconfig.wkid], &newconfig, sizeof(Pophconf));
	      newconfig.wkid = wk;
	      newconfig.set_window_pos= 0;
	      strcpy(newconfig.window_title, phg_default_window_name);
	      strcpy(newconfig.window_icon, phg_default_icon_name);
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
	  /* FIXME this is not used so far */
	  newconfig.vpos.x_min = xmin;
	  newconfig.vpos.x_max = xmax;
	  newconfig.vpos.y_min = ymin;
	  newconfig.vpos.y_max = ymax;
	  newconfig.set_window_pos = 1;
	}
	if (sscanf(line, "%%bg %f %f %f", &red, &green, &blue) > 0){
	  printf("setting new background color for wkid %d to (%f %f %f)\n", wk, red, green, blue);
	  newconfig.background_color.rgb.red = red;
	  newconfig.background_color.rgb.green = green;
	  newconfig.background_color.rgb.blue = blue;
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
      }
    }
    fclose(fh);
  }
  if ((newconfig.wkid >= 0) && (newconfig.wkid < max_wkid)){
    memcpy(&config[newconfig.wkid], &newconfig, sizeof(Pophconf));
  }
}
