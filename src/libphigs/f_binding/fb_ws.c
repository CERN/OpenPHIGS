/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2014 Surplus Users Ham Society
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
******************************************************************************
* Changes:   Copyright (C) 2022-2023 CERN
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gl2ps.h>
#include "phigs.h"
#include "phg.h"
#include "private/phgP.h"
#include "private/wsglP.h"
#include "css.h"
#include "ws.h"
#include "util/ftn.h"
#include "phconf.h"

extern short int wsgl_use_shaders_settings;
/*******************************************************************************
 * popwk
 *
 * DESCR:	Open workstation
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(popwk)(
   FTN_INTEGER(wkid),
   FTN_INTEGER(conid),
   FTN_INTEGER(wtype)
   )
{
   Wst *wst;
   Ws_handle wsh;
   Phg_args_open_ws args;
   Phg_ret ret;

   Pint ws_id = FTN_INTEGER_GET(wkid);
   Pint lun = FTN_INTEGER_GET(conid);
   Pint ws_type = FTN_INTEGER_GET(wtype);

   Phg_args_conn_info conn_id;
   Pcolr_rep rep;
   char filename[512];

   /* read default configuration file if not read yet */
   if (! config_read){
     config_read = 1;
     read_config("phigs.def");
   };
   /* save the current shader settings */
   wsgl_use_shaders_settings = wsgl_use_shaders;
   /* init filename to zero */
   bzero(filename, 512);
   conn_id.lun = lun;
   conn_id.background = 0;

#ifdef DEBUG
  printf("DEBUG: POPWK open %d\n", *wkid);
#endif

   ERR_SET_CUR_FUNC(PHG_ERH, Pfn_open_ws);

   if ((ws_id < 0) || (ws_id > MAX_NO_OPEN_WS)) {
      ERR_REPORT(PHG_ERH, ERR65);
   }
   else if (phg_psl_inq_ws_open(PHG_PSL, ws_id)) {
      ERR_REPORT(PHG_ERH, ERR53);
   }
   else if (!phg_psl_ws_free_slot(PHG_PSL)) {
      ERR_REPORT(PHG_ERH, ERR63);
   }
   else {
      wst = phg_wst_find(&PHG_WST_LIST, ws_type);

      if (wst == NULL) {
         ERR_REPORT(PHG_ERH, ERR52);
      }
      else {
         memset(&args, 0, sizeof(Phg_args_open_ws));
         args.width = config[ws_id].display_width;
         args.height = config[ws_id].display_height;
         args.hcsf = config[ws_id].hcsf;
#ifdef DEBUG
         printf("fb_ws: WSID=%d type=%d scale factor %f\n", ws_id, ws_type, args.hcsf);
#endif
         if (lun == 0) {
           args.conn_info.background = 0;
           args.conn_type = PHG_ARGS_CONN_OPEN;
         }
         else {
           if (
               ws_type == PWST_HCOPY_TRUE_TGA ||
               ws_type == PWST_HCOPY_TRUE_RGB_PNG ||
               ws_type == PWST_HCOPY_TRUE_RGBA_PNG ||
               ws_type == PWST_HCOPY_TRUE_EPS ||
               ws_type == PWST_HCOPY_TRUE_PDF ||
               ws_type == PWST_HCOPY_TRUE_SVG
               ) {
             args.conn_type = PHG_ARGS_CONN_HCOPY;
             args.width = config[ws_id].display_width*config[ws_id].hcsf;
             args.height = config[ws_id].display_height*config[ws_id].hcsf;
             memcpy(&args.conn_info, &conn_id, sizeof(Phg_args_conn_info));
           }
           else {
             args.conn_type = PHG_ARGS_CONN_DRAWABLE;
             memcpy(&args.conn_info, &conn_id, sizeof(Phg_args_conn_info));
           }
         }
         /* switch off shaders for EPS export */
         if (ws_type == PWST_HCOPY_TRUE_EPS || ws_type == PWST_HCOPY_TRUE_PDF || ws_type == PWST_HCOPY_TRUE_SVG ){
           wsgl_use_shaders_settings = wsgl_use_shaders;
           wsgl_use_shaders = 0;
         }
         args.wsid = ws_id;
         args.type = wst;
         args.erh = PHG_ERH;
         args.cssh = PHG_CSS;
         args.memory = 8192;
         args.input_q = PHG_INPUT_Q;
         args.window_name = config[ws_id].window_title;
         args.icon_name = config[ws_id].window_icon;
         args.x = config[ws_id].xpos;
         args.y = config[ws_id].ypos;
         args.border_width =  config[ws_id].border_width;
         args.limits = config[ws_id].vpos;

         /* Open workstation */
         PHG_WSID(ws_id) = (*wst->desc_tbl.phigs_dt.ws_open)(&args, &ret);
         if (PHG_WSID(ws_id) == NULL) {
            ERR_REPORT(PHG_ERH, ERR900);
         }
         else {
            /* Add workstation to info list */
            phg_psl_add_ws(PHG_PSL, ws_id, NULL, wst);
         }
         /* predefine some colors */
         pxset_color_map(ws_id);
         /* set background as specified in configuration file */
         pset_colr_rep(ws_id, 0, &(config[ws_id].background_color));
         /* init output file name */
         wsh = PHG_WSID(ws_id);
         if (strlen(config[ws_id].filename) == 0){
           sprintf(filename, "fort.%d", lun);
           strncpy(wsh->filename, filename, strlen(filename));
           (wsh->filename)[strlen(filename)] = '\0';
         } else {
           strncpy(wsh->filename, config[ws_id].filename, strlen(config[ws_id].filename));
           (wsh->filename)[strlen(config[ws_id].filename)] = '\0';
         }
         wsgl_clear(wsh);
      }
   }
   ERR_FLUSH(PHG_ERH);
}

/*******************************************************************************
 * ppost
 *
 * DESCR:	Post structure
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(ppost)(
   FTN_INTEGER(wkid),
   FTN_INTEGER(strid),
   FTN_REAL(priort)
   )
{
   int status;
   Ws_handle wsh;
   Css_handle cssh;
   Struct_handle structp;

#ifdef DEBUG
  printf("DEBUG: PPOST structure to %d\n", *wkid);
#endif

   Pint ws_id = FTN_INTEGER_GET(wkid);
   Pint struct_id = FTN_INTEGER_GET(strid);
   Pfloat priority = FTN_REAL_GET()priort;

   if (phg_ws_open(ws_id, Pfn_post_struct) != NULL) {
      wsh = PHG_WSID(ws_id);
      cssh = wsh->out_ws.model.b.cssh;
      structp = phg_css_post(cssh, struct_id, wsh, &status);
      if (structp != NULL) {
         (*wsh->post)(wsh, structp, priority, !status);
      }
   }
}

/*******************************************************************************
 * pscr
 *
 * DESCR:       Set colour representation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pscr)(
   FTN_INTEGER(wkid),
   FTN_INTEGER(ci),
   FTN_INTEGER(nccs),
   FTN_REAL_ARRAY(cspec)
   )
{
   Pint ws_id = FTN_INTEGER_GET(wkid);
   Pint ind = FTN_INTEGER_GET(ci);
   Pint ncc = FTN_INTEGER_GET(nccs);
   Pcolr_rep rep;
#ifdef DEBUG
   printf("DEBUG: PSCR workstation color representation %d\n", *wkid);
#endif
   if (ncc == 3) {
     rep.rgb.red   = FTN_REAL_ARRAY_GET(cspec, 0);
     rep.rgb.green = FTN_REAL_ARRAY_GET(cspec, 1);
     rep.rgb.blue  = FTN_REAL_ARRAY_GET(cspec, 2);
   }
   else {
     rep.rgb.red = rep.rgb.green = rep.rgb.blue = FTN_REAL_ARRAY_GET(cspec, 0);
   }
   pset_colr_rep(ws_id, ind, &rep);
}

/*******************************************************************************
 * pupost
 *
 * DESCR:       Unpost structure
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pupost)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(strid)
)
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint struct_id = FTN_INTEGER_GET(strid);
#ifdef DEBUG
  printf("DEBUG: PUPOST Unpost structure %d\n", *strid);
#endif
  punpost_struct(ws_id, struct_id);
}

/*******************************************************************************
 * pupast
 *
 * DESCR:       Unpost all structures
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pupast)(
		       FTN_INTEGER(wkid)
		       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
#ifdef DEBUG
  printf("DEBUG: PUPAST Unpost all structures \n");
#endif
  punpost_all_structs(ws_id);
}

/*******************************************************************************
 * prst
 *
 * DESCR:       Redraw all structures
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(prst)(
                     FTN_INTEGER(wkid),
                     FTN_INTEGER(cofl)
                     )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pctrl_flag ctrl_flag =  FTN_INTEGER_GET(cofl);
#ifdef DEBUG
  printf("DEBUG: PRST redraw all structures %d\n", ctrl_flag);
#endif
  predraw_all_structs(ws_id, ctrl_flag);
}

/*******************************************************************************
 * pswkw
 *
 * DESCR:       Set workstation window
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pswkw)(
		      FTN_INTEGER(wkid),
		      FTN_REAL(xmin),
		      FTN_REAL(xmax),
		      FTN_REAL(ymin),
		      FTN_REAL(ymax)
		      )
{
  Pint wk_id = FTN_INTEGER_GET(wkid);
#ifdef DEBUG
  printf("DEBUG: set window NPC limits for %d\n", wk_id);
#endif
  Plimit lim;
  lim.x_min = FTN_REAL_GET(xmin);
  lim.x_max = FTN_REAL_GET(xmax);
  lim.y_min = FTN_REAL_GET(ymin);
  lim.y_max = FTN_REAL_GET(ymax);
  pset_ws_win(wk_id, &lim);
}

/*******************************************************************************
 * pswkw3
 *
 * DESCR:       Set workstation window 3
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pswkw3)(
                       FTN_INTEGER(wkid),
                       FTN_REAL_ARRAY(wkwn)
                       )
{
  Pint wk_id = FTN_INTEGER_GET(wkid);
#ifdef DEBUG
  printf("DEBUG: set window NPC limits for %d\n", wk_id);
#endif
  Plimit3 lim;
  lim.x_min = FTN_REAL_ARRAY_GET(wkwn, 0);
  lim.x_max = FTN_REAL_ARRAY_GET(wkwn, 1);
  lim.y_min = FTN_REAL_ARRAY_GET(wkwn, 2);
  lim.y_max = FTN_REAL_ARRAY_GET(wkwn, 3);
  lim.z_min = FTN_REAL_ARRAY_GET(wkwn, 4);
  lim.z_max = FTN_REAL_ARRAY_GET(wkwn, 5);
  pset_ws_win3(wk_id, &lim);
}

/*******************************************************************************
 * pclwk
 *
 * DESCR:       Close workstation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pclwk)(
		       FTN_INTEGER(wkid)
		       )
{
#ifdef DEBUG
  printf("DEBUG: close work station %d\n", FTN_INTEGER_GET(wkid));
#endif
  pclose_ws(FTN_INTEGER_GET(wkid));
}

/*******************************************************************************
 * psvwr3
 *
 * DESCR:       Set view representation 3
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psvwr3)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(viewi),
		       Pfloat* vwormti,
		       Pfloat* vwmpmti,
		       FTN_REAL_ARRAY(vwcplm),
		       FTN_INTEGER(xclipi),
		       FTN_INTEGER(bclipi),
		       FTN_INTEGER(fclipi)
		       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint vrep = FTN_INTEGER_GET(viewi);
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Phg_args_rep_data rep;
  Pint xcli = FTN_INTEGER_GET(xclipi);
  Pint bcli = FTN_INTEGER_GET(bclipi);
  Pint fcli = FTN_INTEGER_GET(fclipi);
  Pmatrix3 vwormt;
  Pmatrix3 vwmpmt;
  Ws_handle wsh;
  Phg_ret ret;
  int i ,j;
#ifdef DEBUG
  printf("DEBUG: psvwr3 changing view %d\n", vrep);
#endif
  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    printf("FIXME: Error in psvwr3\n");
  }
  else if ((wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id)) == NULL) {
    printf("Error ERR54 in psvwr3\n");
  }
  else {
    dt = &wsinfo->wstype->desc_tbl.phigs_dt;
    if (dt->ws_category == PCAT_MI) {
      printf("Error ERR57 in psvwr3\n");
    }
    wsh = PHG_WSID(ws_id);
    rep.index = vrep;
    for (i=0; i<4; i++){
      for (j=0; j<4; j++){
	rep.bundl.viewrep.ori_matrix[i][j] = (Pfloat)vwormti[i+4*j];
	rep.bundl.viewrep.map_matrix[i][j] = (Pfloat)vwmpmti[i+4*j];
      }
    }
    rep.bundl.viewrep.clip_limit.x_min = FTN_REAL_ARRAY_GET(vwcplm, 0);
    rep.bundl.viewrep.clip_limit.x_max = FTN_REAL_ARRAY_GET(vwcplm, 1);
    rep.bundl.viewrep.clip_limit.y_min = FTN_REAL_ARRAY_GET(vwcplm, 2);
    rep.bundl.viewrep.clip_limit.y_max = FTN_REAL_ARRAY_GET(vwcplm, 3);
    rep.bundl.viewrep.clip_limit.z_min = FTN_REAL_ARRAY_GET(vwcplm, 4);
    rep.bundl.viewrep.clip_limit.z_max = FTN_REAL_ARRAY_GET(vwcplm, 5);
    rep.bundl.viewrep.xy_clip = xcli;
    rep.bundl.viewrep.back_clip = bcli;
    rep.bundl.viewrep.front_clip = fcli;

    (*wsh->set_rep)(wsh, PHG_ARGS_VIEWREP, &rep);
#ifdef DEBUG
    printf("ORI:\n");
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][0], rep.bundl.viewrep.ori_matrix[1][0], rep.bundl.viewrep.ori_matrix[2][0], rep.bundl.viewrep.ori_matrix[3][0]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][1], rep.bundl.viewrep.ori_matrix[1][1], rep.bundl.viewrep.ori_matrix[2][1], rep.bundl.viewrep.ori_matrix[3][1]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][2], rep.bundl.viewrep.ori_matrix[1][2], rep.bundl.viewrep.ori_matrix[2][2], rep.bundl.viewrep.ori_matrix[3][2]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.ori_matrix[0][3], rep.bundl.viewrep.ori_matrix[1][3], rep.bundl.viewrep.ori_matrix[2][3], rep.bundl.viewrep.ori_matrix[3][3]);
    printf("REP:\n");
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][0], rep.bundl.viewrep.map_matrix[1][0], rep.bundl.viewrep.map_matrix[2][0], rep.bundl.viewrep.map_matrix[3][0]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][1], rep.bundl.viewrep.map_matrix[1][1], rep.bundl.viewrep.map_matrix[2][1], rep.bundl.viewrep.map_matrix[3][1]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][2], rep.bundl.viewrep.map_matrix[1][2], rep.bundl.viewrep.map_matrix[2][2], rep.bundl.viewrep.map_matrix[3][2]);
    printf("    %f %f %f %f\n",  rep.bundl.viewrep.map_matrix[0][3], rep.bundl.viewrep.map_matrix[1][3], rep.bundl.viewrep.map_matrix[2][3], rep.bundl.viewrep.map_matrix[3][3]);
#endif
  }
}

/*******************************************************************************
 * pqhrm
 *
 * DESCR:       Inquire hlhsr mode
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pqhrm)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(err_ind),
		      FTN_INTEGER(hupd),
		      FTN_INTEGER(chrm),
		      FTN_INTEGER(rhrm)
		      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Ws_handle wsh;
  Phg_ret ret;
  wsh = PHG_WSID(ws_id);
  (*wsh->inq_hlhsr_mode)(wsh, &ret);
  *err_ind = ret.err;
  if (ret.err == 0){
    switch (ret.data.hlhsr_mode.state){
    case PUPD_NOT_PEND:
      *hupd = 0;
      break;
    case PUPD_PEND:
      *hupd = 1;
      break;
    default:
      printf("PQHRM ERROR: update state not recognised.\n");
      *err_ind = 1;
      return;
    }
    *chrm = ret.data.hlhsr_mode.cur_mode;
    *rhrm = ret.data.hlhsr_mode.req_mode;
    *err_ind = 0;
#ifdef DEBUG
    printf("DEBUG: PQHRM: current %d requested %d\n", *chrm, *rhrm);
#endif
  }
}

/*******************************************************************************
 * pqvwr
 *
 * DESCR:       Inquire view representation
 * RETURNS:     error index, number of entries, Nth element of defined view indices
 */

FTN_SUBROUTINE(pqvwr)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(viewi),
		      FTN_INTEGER(curq),
		      FTN_INTEGER(err_ind),
		      int* vwupd,
		      Pfloat* vwormt,
		      Pfloat* vwmpmt,
		      Pfloat* vwcplm ,
		      int * xyclip,
		      int* bclip,
		      int* fclip
)
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint index = FTN_INTEGER_GET(viewi);
  Pint itype = FTN_INTEGER_GET(curq);
  Pinq_type type;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;
  int i, j;

#ifdef DEBUG
  printf("DEBUG: pqvwr request view for %d\n", ws_id);
#endif

  switch (itype) {
  case 0:
    type = PINQ_SET;
    break;
  case 1:
    type = PINQ_REALIZED;
    break;
  }
  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR3;
  }
  else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    *err_ind = ERR3;
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      *err_ind = ERR54;
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else if (index < 1) {
        *err_ind = ERR100;
      }
      else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_representation)(wsh, index, type, PHG_ARGS_VIEWREP, &ret);
        if (ret.err) {
          *err_ind = ret.err;
        } else {
          *err_ind = 0;
          if (ret.data.view_rep.update_state == PUPD_NOT_PEND){*vwupd = 0;} else {*vwupd = 1;};
          for (i=0;i<4;i++){
            for (j=0;j<4;j++){
              vwormt[4*j+i] = ret.data.rep.viewrep.ori_matrix[i][j];
              vwmpmt[4*j+i] = ret.data.rep.viewrep.map_matrix[i][j];
            }
          }
          vwcplm[0] =  ret.data.rep.viewrep.clip_limit.x_min;
          vwcplm[1] =  ret.data.rep.viewrep.clip_limit.x_max;
          vwcplm[2] =  ret.data.rep.viewrep.clip_limit.y_min;
          vwcplm[3] =  ret.data.rep.viewrep.clip_limit.y_max;
          vwcplm[4] =  ret.data.rep.viewrep.clip_limit.z_min;
          vwcplm[5] =  ret.data.rep.viewrep.clip_limit.z_max;
          if (ret.data.rep.viewrep.xy_clip)   {*xyclip = 1;} else {*xyclip = 0;};
          if (ret.data.rep.viewrep.back_clip) {*bclip = 1;}  else {*bclip = 0;};
          if (ret.data.rep.viewrep.front_clip){*fclip = 1;}  else {*fclip = 0;};
        }
      }
    }
  }
#ifdef DEBUG
  if (*err_ind == 0){
    printf("VWORMT:");
    printf("    %f %f %f %f\n", vwormt[0],vwormt[1],vwormt[2],vwormt[3]);
    printf("    %f %f %f %f\n", vwormt[4],vwormt[5],vwormt[6],vwormt[7]);
    printf("    %f %f %f %f\n", vwormt[8],vwormt[9],vwormt[10],vwormt[11]);
    printf("    %f %f %f %f\n", vwormt[12],vwormt[13],vwormt[14],vwormt[15]);
    printf("VWMPMT:");
    printf("    %f %f %f %f\n", vwmpmt[0],vwmpmt[1],vwmpmt[2],vwmpmt[3]);
    printf("    %f %f %f %f\n", vwmpmt[4],vwmpmt[5],vwmpmt[6],vwmpmt[7]);
    printf("    %f %f %f %f\n", vwmpmt[8],vwmpmt[9],vwmpmt[10],vwmpmt[11]);
    printf("    %f %f %f %f\n", vwmpmt[12],vwmpmt[13],vwmpmt[14],vwmpmt[15]);
  } else {
    printf("Error in pqvwr! %d\n", *err_ind);
  }
#endif
}

/*******************************************************************************
 * pqwkt
 *
 * DESCR:       Inquire workstation transformation 3
 * RETURNS:     error index, update state,
 *              requested window in NPC, current window in NPC,
 *              requested window in DC, current window in DC
 */

FTN_SUBROUTINE(pqwkt)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(err_ind),
		      FTN_INTEGER(tus),
		      Pfloat*rwindo,
		      Pfloat*cwindo,
		      Pfloat*rviewp,
		      Pfloat*cviewp
)
{
  Pint ws_id = FTN_INTEGER_GET(wkid);

  Pinq_type type;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;

#ifdef DEBUG
  printf("DEBUG: pqwkt called\n");
#endif

  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR3;
  }
  else if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    *err_ind = ERR3;
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      *err_ind = ERR54;
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS  ||
            dt->ws_category == PCAT_PDF  ||
            dt->ws_category == PCAT_SVG  ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else {
        wsh = PHG_WSID(ws_id);
        (*wsh->inq_disp_update_state)(wsh, &ret);
        if (ret.err) {
          *err_ind = ret.err;
        } else {
          Wsb_output_ws	*owsb = &wsh->out_ws.model.b;
          if (owsb->ws_window_pending || owsb->ws_viewport_pending) {*tus = 1;} else {*tus = 0;};
          rwindo[0] = owsb->req_ws_window.x_min;
          rwindo[1] = owsb->req_ws_window.x_max;
          rwindo[2] = owsb->req_ws_window.y_min;
          rwindo[3] = owsb->req_ws_window.y_max;

          cwindo[0] = owsb->ws_window.x_min;
          cwindo[1] = owsb->ws_window.x_max;
          cwindo[2] = owsb->ws_window.y_min;
          cwindo[3] = owsb->ws_window.y_max;

          rviewp[0] = owsb->req_ws_viewport.x_min;
          rviewp[1] = owsb->req_ws_viewport.x_max;
          rviewp[2] = owsb->req_ws_viewport.y_min;
          rviewp[3] = owsb->req_ws_viewport.y_max;

          cviewp[0] = owsb->ws_viewport.x_min;
          cviewp[1] = owsb->ws_viewport.x_max;
          cviewp[2] = owsb->ws_viewport.y_min;
          cviewp[3] = owsb->ws_viewport.y_max;
          *err_ind = 0;
        }
      }
    }
  }
}

/*******************************************************************************
 * pqwkt3
 *
 * DESCR:       Inquire workstation transformation 3
 * RETURNS:     error index, update status,
 *              requested window in NPC, current window in NPC,
 *              requested viewport in DC, current viewport in DC,
 */

FTN_SUBROUTINE(pqwkt3)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(err_ind),
		      FTN_INTEGER(tus),
		      Pfloat* rwindo,
		      Pfloat* cwindo,
		      Pfloat* rviewp,
		      Pfloat* cviewp
)
{
  Pint ws_id = FTN_INTEGER_GET(wkid);

  Pinq_type type;
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Phg_ret ret;
#ifdef DEBUG
  printf("DEBUG: pqwkt3 called\n");
#endif

  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    *err_ind = ERR3;
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      *err_ind = ERR54;
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else {
        wsh = PHG_WSID(ws_id);
        Wsb_output_ws	*owsb = &wsh->out_ws.model.b;
        if (owsb->ws_window_pending || owsb->ws_viewport_pending) {*tus = 1;} else {*tus = 0;};
        rwindo[0] = owsb->req_ws_window.x_min;
        rwindo[1] = owsb->req_ws_window.x_max;
        rwindo[2] = owsb->req_ws_window.y_min;
        rwindo[3] = owsb->req_ws_window.y_max;
        rwindo[4] = owsb->req_ws_window.z_min;
        rwindo[5] = owsb->req_ws_window.z_max;

        cwindo[0] = owsb->ws_window.x_min;
        cwindo[1] = owsb->ws_window.x_max;
        cwindo[2] = owsb->ws_window.y_min;
        cwindo[3] = owsb->ws_window.y_max;
        cwindo[4] = owsb->ws_window.z_min;
        cwindo[5] = owsb->ws_window.z_max;

        rviewp[0] = owsb->req_ws_viewport.x_min;
        rviewp[1] = owsb->req_ws_viewport.x_max;
        rviewp[2] = owsb->req_ws_viewport.y_min;
        rviewp[3] = owsb->req_ws_viewport.y_max;
        rviewp[4] = owsb->req_ws_viewport.z_min;
        rviewp[5] = owsb->req_ws_viewport.z_max;

        cviewp[0] = owsb->ws_viewport.x_min;
        cviewp[1] = owsb->ws_viewport.x_max;
        cviewp[2] = owsb->ws_viewport.y_min;
        cviewp[3] = owsb->ws_viewport.y_max;
        cviewp[4] = owsb->ws_viewport.z_min;
        cviewp[5] = owsb->ws_viewport.z_max;
        *err_ind = 0;
      }
    }
  }
}

/*******************************************************************************
 * pqpost
 *
 * DESCR:       Inquire posted structures
 * RETURNS:     error index, HLHRS mode update status,
 *              current HLHRS mode, requested HLHRS mode
 */

FTN_SUBROUTINE(pqpost)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(n),
		       FTN_INTEGER(err_ind),
		       FTN_INTEGER(number),
		       FTN_INTEGER(strid),
		       FTN_REAL(priort)
		       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint num = FTN_INTEGER_GET(n);
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Ws_handle wsh;
  Ws_post_str * current;
  Wsb_output_ws *owsb;
  Ws_posted_structs posted;
  int nposted, str_id;
  float prio;

#ifdef DEBUG
  printf("DEBUG: PQPOST inquire work station \n");
#endif
  nposted = 0;
  str_id = 0;
  prio = 0.0;
  if (PSL_WS_STATE(PHG_PSL) != PWS_ST_WSOP) {
    *err_ind = ERR3;
  }
  else {
    wsinfo = phg_psl_get_ws_info(PHG_PSL, ws_id);
    if (wsinfo == NULL) {
      *err_ind = ERR54;
    }
    else {
      dt = &wsinfo->wstype->desc_tbl.phigs_dt;
      if (!(dt->ws_category == PCAT_OUT ||
            dt->ws_category == PCAT_TGA ||
            dt->ws_category == PCAT_PNG ||
            dt->ws_category == PCAT_PNGA ||
            dt->ws_category == PCAT_EPS ||
            dt->ws_category == PCAT_PDF ||
            dt->ws_category == PCAT_SVG ||
            dt->ws_category == PCAT_OUTIN ||
            dt->ws_category == PCAT_MO)) {
        *err_ind = ERR59;
      }
      else {
        wsh = PHG_WSID(ws_id);
        owsb = &wsh->out_ws.model.b;
        Ws_posted_structs posted = owsb->posted;
        current = &posted.highest;
        while (current != NULL) {
          if (current->structh != NULL) {
            nposted += 1;
            prio = current->disp_pri;
            str_id = current->structh->struct_id;
            if (nposted == num) {
              *strid = str_id;
              *priort = prio;
            }
          }
          current = current->lower;
        }
        *err_ind = 0;
        *number = nposted;
#ifdef DEBUG
        printf("PQPOST: returning number %d, strid %d and prio %f\n", nposted, *strid, *priort);
#endif
      }
    }
  }
}

/*******************************************************************************
 * pqwkpo
 *
 * DESCR:       Inquire set member of workstations to which posted
 * RETURNS:     error index, number of workstations to which the structure is posted,
 *              Nth member of set of workstations to which the structure is posted
 */

FTN_SUBROUTINE(pqwkpo)(
		       FTN_INTEGER(strid),
		       FTN_INTEGER(n),
		       FTN_INTEGER(err_ind),
		       FTN_INTEGER(ol),
		       FTN_INTEGER(wkid)
		       )
{
  /*
This needs to loop over all work stations and all their posted structures, and if the given structure is found,
remember that WKID and the number of matches we had. Then we return the WKID of the N'th match in wkid and the number of matches in ol
FIXME: this one does not seem to find anything for some reason.

  */
  Pint struct_id = FTN_INTEGER_GET(strid);
  Ws_handle wsh;
  Pint num = FTN_INTEGER_GET(n);
  Pint matches, nwk, ws_id;
  Pint wkids[99];
  Ws_post_str * current;
  Wsb_output_ws *ows;
  Ws_posted_structs posted;
#ifdef DEBUG
  printf("DEBUG: PQWKPO inquire work station \n");
#endif
  if (!phg_entry_check(PHG_ERH, 0, Pfn_INQUIRY)) {
    *err_ind = ERR2;
  }
  else
    {
      nwk = PHG_WST_LIST.count;
      matches = 0;
      *err_ind = 1;
      for (ws_id = 0; ws_id<nwk; ws_id++){
        wsh = PHG_WSID(ws_id);
	if (wsh != NULL){
	  ows = &wsh->out_ws.model.b;
	  posted = ows->posted;
	  current =  &posted.highest;
	  while (current != NULL) {
	    if (current->structh != NULL) {
	      if (current->structh->struct_id == struct_id) {
		wkids[matches] = ws_id;
		matches += 1;
	      }
	    }
	    current = current->lower;
	  }
	}
      }
      if (matches>0 && num<=matches){
	*err_ind = 0;
	*ol = matches;
	*wkid = wkids[num-1];
#ifdef DEBUG
	printf("PQWKPO: Found %d matches. Returning %d\n", *ol, *wkid);
#endif
      } else {
	*err_ind = ERR201;
	*ol = 0;
	*wkid = 0;
#ifdef DEBUG
	printf("PQWKPO: No matches found.\n");
#endif
      }
    }
}

/*******************************************************************************
 * puwk
 *
 * DESCR:       Update workstation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(puwk)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(regfl)
		      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pregen_flag regen_flag = FTN_INTEGER_GET(regfl);
  pupd_ws(ws_id, regen_flag);
}

/*******************************************************************************
 * pspmr
 *
 * DESCR:       Set polymarker representation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pspmr)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(pmi),
		      FTN_INTEGER(mtype),
		      FTN_REAL(mszsf),
		      FTN_INTEGER(coli)
		      ){
  Pint wk_id = FTN_INTEGER_GET(wkid);
  Pint pind  = FTN_INTEGER_GET(pmi);
  Pint ptype = FTN_INTEGER_GET(mtype);
  Pfloat size  = FTN_REAL_GET(mszsf);
  Pint col = FTN_INTEGER_GET(coli);
  Pmarker_bundle mkrep = { ptype, size, col };
  pset_marker_rep(wk_id, pind, &mkrep);
}

/*******************************************************************************
 * psplr
 *
 * DESCR:       Set polyline representation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psplr)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(pli),
		      FTN_INTEGER(ltyp),
		      FTN_REAL(lwidth),
		      FTN_INTEGER(coli)
		      ){
  Pint wk_id = FTN_INTEGER_GET(wkid);
  Pint lind  = FTN_INTEGER_GET(pli);
  Pint ltype = FTN_INTEGER_GET(ltyp);
  Pfloat lsize = FTN_REAL_GET(lwidth);
  Pint col = FTN_INTEGER_GET(coli);
  Pline_bundle mkrep = { ltype, lsize, col };
  pset_line_rep(wk_id, lind, &mkrep);
}

/*******************************************************************************
 * psedr
 *
 * DESCR:       Set edge representation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psedr)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(edi),
		      FTN_INTEGER(edflag),
		      FTN_INTEGER(edtype),
		      FTN_REAL(ewidth),
		      FTN_INTEGER(coli)
		      ){
  Pint wk_id = FTN_INTEGER_GET(wkid);
  Pint eind  = FTN_INTEGER_GET(edi);
  Pint eflag = FTN_INTEGER_GET(edflag);
  Pint etype = FTN_INTEGER_GET(edtype);
  Pfloat ewid  = FTN_REAL_GET(ewidth);
  Pint col   = FTN_INTEGER_GET(coli);
  Pedge_bundle mkrep = { eflag, etype, ewid, col };
  pset_edge_rep(wk_id, eind, &mkrep);
}

/*******************************************************************************
 * psir
 *
 * DESCR:       Set interior representation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psir)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(ii),
		      FTN_INTEGER(ints),
		      FTN_INTEGER(styli),
		      FTN_INTEGER(coli)
		      ){
  Pint wk_id = FTN_INTEGER_GET(wkid);
  Pint iii  = FTN_INTEGER_GET(ii);
  Pint iints = FTN_INTEGER_GET(ints);
  Pint istyli = FTN_INTEGER_GET(styli);
  Pint col   = FTN_INTEGER_GET(coli);
  Pint_bundle mkrep = { iints, istyli, col };
  pset_int_rep(wk_id, iii, &mkrep);
}

/*******************************************************************************
 * pstxr
 *
 * DESCR:       Set text representation
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pstxr)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(txi),
		      FTN_INTEGER(font),
		      FTN_INTEGER(prec),
		      FTN_REAL(chxp),
		      FTN_REAL(chsp),
		      FTN_INTEGER(coli)
		      ){
  Pint wk_id = FTN_INTEGER_GET(wkid);
  Pint tind  = FTN_INTEGER_GET(txi);
  Pint tfont = FTN_INTEGER_GET(font);
  Pint tprec  = FTN_INTEGER_GET(prec);
  Pfloat txp  = FTN_REAL_GET(chxp);
  Pfloat tsp  = FTN_REAL_GET(chsp);
  Pint col   = FTN_INTEGER_GET(coli);
  Ptext_bundle mkrep = { tfont, tprec, txp, tsp, col};
  pset_text_rep(wk_id, tind, &mkrep);
}

/*******************************************************************************
 * pswkv3
 *
 * DESCR:       Set workstation viewport 3
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pswkv3)(
		       FTN_INTEGER(wkid),
		       FTN_REAL_ARRAY(wkvp)
		      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Plimit3 lim3;
  lim3.x_min = FTN_REAL_ARRAY_GET(wkvp, 0);
  lim3.x_max = FTN_REAL_ARRAY_GET(wkvp, 1);
  lim3.y_min = FTN_REAL_ARRAY_GET(wkvp, 2);
  lim3.y_max = FTN_REAL_ARRAY_GET(wkvp, 3);
  lim3.z_min = FTN_REAL_ARRAY_GET(wkvp, 4);
  lim3.z_max = FTN_REAL_ARRAY_GET(wkvp, 5);
  pset_ws_vp3(ws_id, &lim3);
}

/*******************************************************************************
 * psdus
 *
 * DESCR:       Set display update state
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(psdus)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(defmod),
		      FTN_INTEGER(modmod)
		      ){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pdefer_mode def_mod = (Pdefer_mode) FTN_INTEGER_GET(defmod);
  Pmod_mode mod_mod = (Pmod_mode) FTN_INTEGER_GET(modmod);
  pset_disp_upd_st(ws_id, def_mod, mod_mod);
}

/*******************************************************************************
 * pqcr
 *
 * DESCR:       Inquire colour representation
 * RETURNS:     Error index, number of colour components, colour specs
 */

FTN_SUBROUTINE(pqcr)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(coli),
		      FTN_INTEGER(ccsbsz),
		      FTN_INTEGER(rtype),
		      int* err_ind,
		      int* ol,
		      float* cspec
		      ){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint colr_ind = FTN_INTEGER_GET(coli);
  Pinq_type type = (Pinq_type) FTN_INTEGER_GET(rtype);
  Pint buf_size = FTN_INTEGER_GET(ccsbsz);
  Pcolr_rep colr_rep;
  if (buf_size >= 3){
    pinq_colr_rep(ws_id, colr_ind, type, err_ind, &colr_rep);
    if (*err_ind == 0){
      *ol = 3;
      cspec[0] = colr_rep.rgb.red;
      cspec[1] = colr_rep.rgb.green;
      cspec[2] = colr_rep.rgb.blue;
    }
  } else {
    *err_ind = 1;
    printf("pqcr: Buffer size too small. Ignoring function.\n");
  }
}

/*******************************************************************************
 * pqcf
 *
 * DESCR:       Inquire colour facilities
 * RETURNS:     error index, number of colour indices,
 *              colour available, number of predefined colour indices, primary colours
 */

FTN_SUBROUTINE(pqcf)(
		     FTN_INTEGER(wtype),
		     int* errind,
		     int* ncoli,
		     int* cola,
		     int* npci,
		     float* cc) {
  printf("WARNING: pqcf called for WSTYPE: %d. Returning DUMMY values\n", FTN_INTEGER_GET(wtype));
  *errind = 0;
  *ncoli = 15;
  *cola = 1;
  cc[0]=1.;
  cc[1]=0.;
  cc[2]=0.;
  cc[3]=0.;
  cc[4]=1.;
  cc[5]=0.;
  cc[6]=0.;
  cc[7]=0.;
  cc[8]=1.;
}

/*******************************************************************************
 * pmsg
 *
 * DESCR:       Message
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pmsg)(
		     FTN_INTEGER(wkid),
		     FTN_CHARACTER(msg)
		     ){
  char buffer[100];
  Pint ws_id = FTN_INTEGER_GET(wkid);
  char * message = FTN_CHARACTER_GET(msg);
  Pint length = FTN_CHARACTER_LEN(msg);
  strncpy(buffer, message, length);
  buffer[length] = '\0';
  pmessage(ws_id, buffer);
}

/*
 *
 *  extensions to the Standard
 *
*/

/*******************************************************************************
 * pslsr
 *
 * DESCR:       Set light source representation
 * RETURNS:     N/A
 * NOTES:       Not part of the standard
 */

FTN_SUBROUTINE(pslsr)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(lsi),
		       FTN_INTEGER(lstyp),
		       FTN_INTEGER(ldr),
		       char* data
		       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint light_src_ind = FTN_INTEGER_GET(lsi);
  Pint type = FTN_INTEGER_GET(lstyp);
  Pint dbytes = FTN_INTEGER_GET(ldr);
  Psl_ws_info *wsinfo;
  Wst_phigs_dt *dt;
  Phg_args_rep_data rep;
  Ws_handle wsh;
  int *ihere;
  float *fhere;
  int col_indx;
  int ncc;
  Pamb_light_src_rec amblight;
  Pdir_light_src_rec dirlight;
  Ppos_light_src_rec poslight;

  Plight_src_bundle light_src_rep;
#ifdef DEBUG
  printf("DEBUG: PSLSR set light source representation\n");
#endif
  ihere = (int*) data;
  /* first value is the number of integers */
  ihere = &ihere[1];
  int col_type = ihere[0];
  if (col_type == PINDIRECT){
    col_indx = ihere[1];
  } else {
    ncc = ihere[1];
  }
  /* number of ints plus 2 ints plus number of floats */
  fhere = (float*)(data + 4*sizeof(int));
  switch (type) {
  case PLIGHT_AMBIENT:
    amblight.colr.type = col_type;
    if (col_type == PINDIRECT){
      amblight.colr.val.ind = col_indx;
    } else {
      memcpy(&amblight.colr.val.general.x, &fhere[0], 3*sizeof(Pfloat));
    }
    light_src_rep.type = type;
    light_src_rep.rec.ambient = amblight;
    pset_light_src_rep(ws_id, light_src_ind, & light_src_rep);
    break;
  case PLIGHT_DIRECTIONAL:
    dirlight.colr.type = col_type;
    memcpy(&dirlight.dir.delta_x, &fhere[0], 3*sizeof(Pfloat));
    if (col_type == PINDIRECT){
      dirlight.colr.val.ind = col_indx;
    } else {
      memcpy(&dirlight.colr.val.general.x, &fhere[3], 3*sizeof(Pfloat));
    }
    light_src_rep.type = type;
    light_src_rep.rec.directional = dirlight;
    pset_light_src_rep(ws_id, light_src_ind, & light_src_rep);
    break;
  case PLIGHT_POSITIONAL:
    poslight.colr.type = col_type;
    memcpy(&poslight.pos.x, &fhere[0], 3*sizeof(Pfloat));
    memcpy(&poslight.coef, &fhere[3], 2*sizeof(Pfloat));
    if (col_type == PINDIRECT){
      poslight.colr.val.ind = col_indx;
    } else {
      memcpy(&poslight.colr.val.general.x, &fhere[5], 3*sizeof(Pfloat));
    }
    light_src_rep.type = type;
    light_src_rep.rec.positional = poslight;
    pset_light_src_rep(ws_id, light_src_ind, & light_src_rep);
    break;

  default:
    printf("ERROR in pslsr: light type %d not yet implemented. Ignorning function.\n", type);
    break;
  }
}

/*******************************************************************************
 * pxscm
 *
 * DESCR:       Set color map
 * RETURNS:     N/A
 */

FTN_SUBROUTINE(pxscm)(
		       FTN_INTEGER(wkid)
		      )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  pxset_color_map(ws_id);
}

/*******************************************************************************
 * psfname
 *
 * DESCR:       Set workstation output file name for hardcopy types
 * RETURNS:     N/A
 * NOTES:       extension
 */

FTN_SUBROUTINE(psfname)(
                        FTN_INTEGER(wkid),
                        FTN_CHARACTER(fname)
                        )
{
  Ws_handle wsh;
  Pint ws_id = FTN_INTEGER_GET(wkid);
  char * filename = FTN_CHARACTER_GET(NAME);
  int length = FTN_CHARACTER_LEN(fname);
  wsh = PHG_WSID(ws_id);
  if (wsh != NULL){
    strncpy(wsh->filename, filename, length);
    (wsh->filename)[length] = '\0';
  } else {
    printf("ERROR in PSFNAME: workstation %d not found.", ws_id);
  }
}
