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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <phigs.h>
#include <phg.h>
#include <css.h>
#include <private/phgP.h>
#include <private/wsxP.h>
#include <util/ftn.h>

/*******************************************************************************
 * pprec
 *
 * DESCR:       pack data record
 * RETURNS:	error, length, data record
 */

FTN_SUBROUTINE(pprec)(
                      FTN_INTEGER(il),
                      FTN_INTEGER_ARRAY(ia),
                      FTN_INTEGER(rl),
                      FTN_REAL_ARRAY(ra),
                      FTN_INTEGER(sl),
                      FTN_INTEGER_ARRAY(lstri),
                      char* str,
                      FTN_INTEGER(mldr),
                      int *errind,
                      int *ldr,
                      char *datrec
                      )
{
#ifdef DEBUG
  printf("DEBUG: PPREC packing data record\n");
#endif
  Pint intl = FTN_INTEGER_GET(il);
  Pint flol = FTN_INTEGER_GET(rl);
  Pint nstr = FTN_INTEGER_GET(sl);
  Pint dima = FTN_INTEGER_GET(mldr);
  char * here = datrec;
#ifdef DEBUG
  char * final;
#endif
  int i, len, num_bytes, chars;
  int maxbytes, required;
  int * intp;

  if (dima < 1){
    printf("pprec error: dimensionality not supported %d\n", dima);
    *errind = 1;
    return;
  }
  maxbytes = 80*dima*sizeof(char);
  /* size sanity check */
  chars = 0;
  for (i=0; i<nstr; i++){
    chars += 1 + FTN_INTEGER_ARRAY_GET(lstri, i);
  }
  required =
    (intl+1)*sizeof(int)
    +(flol+1)*sizeof(float)
    +(chars+1)+sizeof(int)
    +chars*sizeof(char);
  if (required > maxbytes){
    printf("pprec error: buffer passed on is too small: Have %d bytes but need %d\n", maxbytes, required);
    *errind = 1;
    return;
  }
  /* copy the ints */
  memcpy(here, &intl, sizeof(int));
  here += sizeof(int);
  memcpy(here, (char*) ia, intl*sizeof(int));
  here += intl*sizeof(int);
  /* copy the floats */
  memcpy(here, &flol, sizeof(float));
  here += sizeof(float);
  memcpy(here, (char*) ra, flol*sizeof(float));
  here += flol*sizeof(float);
  /* strings */
  memcpy(here, (char*) &nstr, sizeof(int));
  here += sizeof(int);
  /* copy sizes first */
  memcpy(here, (char*) lstri, nstr*sizeof(int));
  here += nstr*sizeof(int);
  /* copy the strings */
  for (i=0; i<nstr; i++){
    len = FTN_INTEGER_ARRAY_GET(lstri, i);
#ifdef DEBUG
    printf("DEBUG: pprec string nr %d length %d\n", i, len);
#endif
    memcpy(here, &str[i*dima], len);
#ifdef DEBUG
    final = here;
#endif
    here += len*sizeof(char);
    *here = '\0';
#ifdef DEBUG
    printf("DEBUG: pprec final %s length %d\n", final, (int)strlen(final));
#endif
    here++;
  }
  *errind = 0;
  /* this is probably not what is expected */
  *ldr = (int)(here-datrec);
#ifdef DEBUG
  printf("DEBUG: PPREC returns %d pages\n", *ldr);
#endif
}

/*******************************************************************************
 * pinlc3
 *
 * DESCR:       initialize locator 3
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinlc3)(
                     FTN_INTEGER(wkid),
                     FTN_INTEGER(lcdnr),
                     FTN_INTEGER(iviewi),
                     FTN_REAL(ipx),
                     FTN_REAL(ipy),
                     FTN_REAL(ipz),
                     FTN_INTEGER(pet1),
                     FTN_REAL_ARRAY(evol),
                     FTN_INTEGER(ldr),
                     char* datrec
                     )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint loc_num = FTN_INTEGER_GET(lcdnr);
  Pint init_view_ind = FTN_INTEGER_GET(iviewi);
  Pint pet = FTN_INTEGER_GET(pet1);
  Ppoint3 location;
  Plimit3 lim;
  Ploc_data3 data;

#ifdef DEBUG
  printf("DEBUG: PINLC3 setup 3d locator\n");
#endif

  location.x = FTN_REAL_GET(ipx);
  location.y = FTN_REAL_GET(ipy);
  location.z = FTN_REAL_GET(ipz);
  lim.x_min = FTN_REAL_ARRAY_GET(evol, 0);
  lim.x_max = FTN_REAL_ARRAY_GET(evol, 1);
  lim.y_min = FTN_REAL_ARRAY_GET(evol, 2);
  lim.y_max = FTN_REAL_ARRAY_GET(evol, 3);
  lim.z_min = FTN_REAL_ARRAY_GET(evol, 4);
  lim.z_max = FTN_REAL_ARRAY_GET(evol, 5);

  /* FIXME here we may need some more stuff */
  data.pets.pet_r1.unused = 0;
  /* FIXME check if the data record is actually being used */
  pinit_loc3(ws_id, loc_num, init_view_ind, &location, pet, &lim, &data);

};


/*******************************************************************************
 * pspkm
 *
 * DESCR:       set pick mode
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pspkm)(
              FTN_INTEGER(wkid),
              FTN_INTEGER(pkdnr),
              FTN_INTEGER(mode),
              FTN_INTEGER(esw)
              )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_num = FTN_INTEGER_GET(pkdnr);
  Pint opmode = FTN_INTEGER_GET(mode);
  Pint echoswitch = FTN_INTEGER_GET(esw);

#ifdef DEBUG
  printf("DEBUG: PSPKM set picking mode for window %d\n", ws_id);
#endif
  pset_pick_mode(ws_id, pick_num, opmode, echoswitch);
}

/*******************************************************************************
 * pwait
 *
 * DESCR:       await event
 * RETURNS:	work station, class, number
 */

FTN_SUBROUTINE(pwait)(
              FTN_REAL(tout),
              Pint *wkid,
              Pin_class *icl,
              Pint *idnr
              )
{
  Pfloat timeout = FTN_REAL_GET(tout);
  pawait_event(timeout, wkid, icl, idnr);
}

/*******************************************************************************
 * pslcm
 *
 * DESCR:       set locator mode
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pslcm)(
              FTN_INTEGER(wkid),
              FTN_INTEGER(lcdnr),
              FTN_INTEGER(mode),
              FTN_INTEGER(echo)
              )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint dev_id = FTN_INTEGER_GET(lcdnr);
  Pint mode1 = FTN_INTEGER_GET(mode);
  Pint echo1 = FTN_INTEGER_GET(echo);
  pset_loc_mode(ws_id, dev_id, mode1, echo1);
}

/*******************************************************************************
 * pinlc
 *
 * DESCR:       initialize locator
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinlc)(
                     FTN_INTEGER(wkid),
                     FTN_INTEGER(lcdnr),
                     FTN_INTEGER(iviewi),
                     FTN_REAL(ipx),
                     FTN_REAL(ipy),
                     FTN_INTEGER(pet1),
                     FTN_REAL(xmin),
                     FTN_REAL(xmax),
                     FTN_REAL(ymin),
                     FTN_REAL(ymax),
                     FTN_INTEGER(ldr),
                     char* datrec
                     )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint loc_num = FTN_INTEGER_GET(lcdnr);
  Pint init_view_ind = FTN_INTEGER_GET(iviewi);
  Pint pet = FTN_INTEGER_GET(pet1);
  Ppoint3 location;
  Plimit3 lim;
  Ploc_data3 data;

#ifdef DEBUG
  printf("DEBUG: PSCHSP setup 3d locator\n");
#endif

  location.x = FTN_REAL_GET(ipx);
  location.y = FTN_REAL_GET(ipy);
  location.z = 0.0;
  lim.x_min = FTN_REAL_GET(xmin);
  lim.x_max = FTN_REAL_GET(xmax);
  lim.y_min = FTN_REAL_GET(ymin);
  lim.y_max = FTN_REAL_GET(ymax);
  lim.z_min = 0.0;
  lim.z_max = 0.0;

  /* only echo mode 1 used */
  data.pets.pet_r1.unused = 0;
  /* FIXME check if the data is being used */
  pinit_loc3(ws_id, loc_num, init_view_ind, &location, pet, &lim, &data);

};

/*******************************************************************************
 * pspkft
 *
 * DESCR:       set pick filter
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pspkft)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(pkdnr),
		       FTN_INTEGER(isn),
		       Pint *is,
		       FTN_INTEGER(esn),
		       Pint *es
		       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_dev =  FTN_INTEGER_GET(pkdnr);
  Pint isn1 = FTN_INTEGER_GET(isn);
  Pint esn1 = FTN_INTEGER_GET(esn);

  Pfilter filter;

  Pint incl_set[isn1+1];
  Pint excl_set[esn1+1];
  memcpy(&incl_set, is, isn1*sizeof(int));
  memcpy(&excl_set, es, esn1*sizeof(int));

  filter.incl_set.num_ints = isn1;
  filter.excl_set.num_ints = esn1;
  if (isn == 0) incl_set[0] = 0;
  if (esn == 0) excl_set[0] = 0;
  filter.incl_set.ints = &incl_set[0];
  filter.excl_set.ints = &excl_set[0];

  pset_pick_filter(ws_id, pick_dev, &filter);
}

/*******************************************************************************
 * pshlft
 *
 * DESCR:       set highlighting filter
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pshlft)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(isn),
		       Pint *is,
		       FTN_INTEGER(esn),
		       Pfloat *es
		       )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint isn1 = FTN_INTEGER_GET(isn);
  Pint esn1 = FTN_INTEGER_GET(esn);

  Pfilter filter;

  Pint incl_set[isn1+1];
  Pint excl_set[esn1+1];
  memcpy(&incl_set, is, isn1*sizeof(int));
  memcpy(&excl_set, es, esn1*sizeof(int));

  filter.incl_set.num_ints = isn1;
  filter.excl_set.num_ints = esn1;
  if (isn == 0) incl_set[0] = 0;
  if (esn == 0) excl_set[0] = 0;
  filter.incl_set.ints = &incl_set[0];
  filter.excl_set.ints = &excl_set[0];

  pset_highl_filter(ws_id, &filter);
}

/*******************************************************************************
 * pinpk
 *
 * DESCR:       initialize pick
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinpk)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(pkdnr),
		      FTN_INTEGER(istat),
		      FTN_INTEGER(ippd),
		      FTN_INTEGER_ARRAY(pp),
		      FTN_INTEGER(pet1),
		      FTN_REAL(xmin),
		      FTN_REAL(xmax),
		      FTN_REAL(ymin),
		      FTN_REAL(ymax),
		      FTN_INTEGER(ndr1),
		      char* datrec,
		      FTN_INTEGER(ppordr)
              )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_num = FTN_INTEGER_GET(pkdnr);
  Pin_status init_status = FTN_INTEGER_GET(istat);
  Pint pet = FTN_INTEGER_GET(pet1);
  Ppick_path init_pick;
  Ppick_path_elem elem;
  Plimit3 echo_area;
  Ppath_order order = FTN_INTEGER_GET(ppordr);
  Pint ndr = FTN_INTEGER_GET(ndr1);
  Ppick_data data;

  elem.struct_id = FTN_INTEGER_ARRAY_GET(pp, 0);
  elem.pick_id = FTN_INTEGER_ARRAY_GET(pp, 1);
  elem.elem_pos = FTN_INTEGER_ARRAY_GET(pp, 2);

  init_pick.depth = FTN_INTEGER_GET(ippd);
  init_pick.path_list = &elem;
  echo_area.x_min = FTN_REAL_GET(xmin);
  echo_area.x_max = FTN_REAL_GET(xmax);
  echo_area.y_min = FTN_REAL_GET(ymin);
  echo_area.y_max = FTN_REAL_GET(ymax);
  echo_area.z_min = 0.0;
  echo_area.z_max = 1.0;

  /* only echo mode 1 */
  data.pets.pet_r1.unused = 0;
  /* FIXME check if we properly copy stuff from the data record */
  pinit_pick3(ws_id, pick_num, init_status, &init_pick, pet, &echo_area, &data, order);
}

/*******************************************************************************
 * pgtpk
 *
 * DESCR:       get pick
 * RETURNS:	state, depth, path
 */

FTN_SUBROUTINE(pgtpk)(
              FTN_INTEGER(ippd),
              Pin_status *stat,
              Pint *ppd,
              Pint *pp
              )
{
  Pint depth = FTN_INTEGER_GET(ippd);
  Ppick_path_elem  path_list[10];
  Ppick_path pick = {0, path_list};
  Pin_status status;
  int i;

  pget_pick(depth, &status, &pick);
  *ppd = pick.depth;
  *stat = (int)status;
#ifdef DEBUG
  printf("pgtpk: depth %d, stat %d, pickdepth %d\n", depth, status, pick.depth);
#endif
  for (i=0; i<pick.depth; i++){
    pp[3*i] = pick.path_list[i].struct_id;
    pp[3*i+1] = pick.path_list[i].pick_id;
    pp[3*i+2] = pick.path_list[i].elem_pos;
  };
}

/*******************************************************************************
 * prqpk
 *
 * DESCR:       request pick
 * RETURNS:	state, depth, path
 */

FTN_SUBROUTINE(prqpk)(
              FTN_INTEGER(wkid),
              FTN_INTEGER(pkdnr),
              FTN_INTEGER(ippd),
              Pint *stat,
              Pint *ppd,
              Pint *pp
              )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_num = FTN_INTEGER_GET(pkdnr);
  Pint depth = FTN_INTEGER_GET(ippd);
  Ppick_path_elem  path_list[10];
  Ppick_path pick = {0, path_list};
  Pin_status status;
  Phg_ret ret;
  int i;
  preq_pick(ws_id, pick_num, depth, &status, &pick);
  *stat = (int)status;
  if ( status == PIN_STATUS_OK) {
    *ppd = pick.depth;
    for (i=0; i<pick.depth; i++){
      pp[3*i] = pick.path_list[i].struct_id;
      pp[3*i+1] = pick.path_list[i].pick_id;
      pp[3*i+2] = pick.path_list[i].elem_pos;
    }
  }
}

/*******************************************************************************
 * psmpk
 *
 * DESCR:       sample pick
 * RETURNS:	status, depth and path
 */

FTN_SUBROUTINE(psmpk)(
              FTN_INTEGER(wkid),
              FTN_INTEGER(pkdnr),
              FTN_INTEGER(ippd),
              Pin_status *stat,
              Pint *ppd,
              Pint *pp
              )
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint pick_num = FTN_INTEGER_GET(pkdnr);
  Pint depth = FTN_INTEGER_GET(ippd);
  Pin_status status;
  Ppick_path_elem  path_list[10];
  Ppick_path pick = {0, path_list};
  int i;

  psample_pick(ws_id, pick_num, depth, &status, &pick);
  *stat = (int)status;
  if ( status == PIN_STATUS_OK) {
    *ppd = pick.depth;
#ifdef DEBUG
     printf("prmpk: got depth %d, stat %d, pickdepth %d\n", depth, status, pick.depth);
#endif
    for (i=0; i<pick.depth; i++){
      pp[3*i] = pick.path_list[i].struct_id;
      pp[3*i+1] = pick.path_list[i].pick_id;
      pp[3*i+2] = pick.path_list[i].elem_pos;
    }
  }
}

/*******************************************************************************
 * psmlc
 *
 * DESCR:       sample locator
 * RETURNS:	view index, current point in WC
 */

FTN_SUBROUTINE(psmlc)(
              FTN_INTEGER(wkid),
              FTN_INTEGER(lcdnr),
              int *viewi,
              float *lpx,
              float *lpy
              )
{
  Pint    ws_id = FTN_INTEGER_GET(wkid);
  Pint    loc_dev = FTN_INTEGER_GET(lcdnr);
  Ppoint locpos;
  psample_loc(ws_id, loc_dev, viewi, &locpos);
  *lpx = locpos.x;
  *lpy = locpos.y;
}

/*******************************************************************************
 * psmlc3
 *
 * DESCR:       sample locator 3
 * RETURNS:	view index, current point in WC
 */

FTN_SUBROUTINE(psmlc3)(
              FTN_INTEGER(wkid),
              FTN_INTEGER(lcdnr),
              int *viewi,
              float *lpx,
              float *lpy,
              float *lpz
              )
{
  Pint    ws_id = FTN_INTEGER_GET(wkid);
  Pint    loc_dev = FTN_INTEGER_GET(lcdnr);
  Ppoint3 locpos;
  psample_loc3(ws_id, loc_dev, viewi, &locpos);
  *lpx = locpos.x;
  *lpy = locpos.y;
  *lpz = locpos.z;
}

/*******************************************************************************
 * prqlc3
 *
 * DESCR:       request locator 3
 * RETURNS:	status, view index, point in WC
 */

FTN_SUBROUTINE(prqlc3)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(lcdnr),
		       Pint* stat,
		       Pint* viewi,
		       Pfloat *px,
		       Pfloat *py,
		       Pfloat *pz) {
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint loc_dev = FTN_INTEGER_GET(lcdnr);
  Ppoint3 pos;
  preq_loc3(ws_id, loc_dev, (Pin_status*)stat, viewi, &pos);
  *px = pos.x;
  *py = pos.y;
  *pz = pos.z;
}

/*******************************************************************************
 * psmst
 *
 * DESCR:       sample string
 * RETURNS:	string length, string
 */

FTN_SUBROUTINE(psmst)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(stdnr),
		      int* lostr,
		      char* str
		      ){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint string_dev = FTN_INTEGER_GET(stdnr);
  psample_string(ws_id, string_dev, str);
  *lostr = strlen(str);
}

/*******************************************************************************
 * psstm
 *
 * DESCR:       set string mode
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(psstm)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(stdnr),
		      FTN_INTEGER(mode),
		      FTN_INTEGER(esw)
		      ){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint string_dev = FTN_INTEGER_GET(stdnr);
  Pint opmode = FTN_INTEGER_GET(mode);
  Pint echo = FTN_INTEGER_GET(esw);
  pset_string_mode(ws_id, string_dev, opmode, echo);
}

/*******************************************************************************
 * pinst
 *
 * DESCR:       initialize string
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinst)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(stdnr),
		      FTN_INTEGER(lstr),
		      char *istr,
		      FTN_INTEGER(ipet),
		      FTN_REAL(xmin),
		      FTN_REAL(xmax),
		      FTN_REAL(ymin),
		      FTN_REAL(ymax),
		      FTN_INTEGER(ldr),
		      char* datrec){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint string_dev = FTN_INTEGER_GET(stdnr);
  Pint ilen = FTN_INTEGER_GET(lstr);
  Pint pet = FTN_INTEGER_GET(ipet);
  char * init_string = istr;
  Plimit area;
  Pstring_data data;
  int * here;

  char * buffer = (char*)malloc((ilen+1)*sizeof(char));
  if (buffer != NULL) {
    strncpy(buffer, istr, ilen*sizeof(char));
    buffer[ilen] = '\0';
    area.x_min = FTN_REAL_GET(xmin);
    area.x_max = FTN_REAL_GET(xmax);
    area.y_min = FTN_REAL_GET(ymin);
    area.y_max = FTN_REAL_GET(ymax);
    /* decode input data */
    here = (int*)&datrec[0];
    if (here[0] == 2){
      data.buffer_size = here[1];
      data.init_pos = here[2];

      /* only one echo mode */
      data.pets.pet_r1.unused = 0;
      pinit_string(ws_id, string_dev, buffer, pet, &area, &data);
    } else {
      printf("ERROR in pinst: wrong number of integers in data record\n");
    }
    free(buffer);
  }
}

/*******************************************************************************
 * pinst3
 *
 * DESCR:       initialize string 3
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinst3)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(stdnr),
		      FTN_INTEGER(lstr),
		      char*istr,
		      FTN_INTEGER(ipet),
		      FTN_REAL_ARRAY(evol),
		      FTN_INTEGER(ldr),
		      char* datrec){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint string_dev = FTN_INTEGER_GET(stdnr);
  Pint ilen = FTN_INTEGER_GET(lstr);
  Pint pet = FTN_INTEGER_GET(ipet);
  char * init_string = istr;
  Plimit3 area;
  Pstring_data data;
  int * here;
  char * buffer = (char*)malloc((ilen+1)*sizeof(char));
  if (buffer != NULL) {
    strncpy(buffer, istr, ilen*sizeof(char));
    buffer[ilen] = '\0';
    area.x_min = FTN_REAL_ARRAY_GET(evol, 0);
    area.x_max = FTN_REAL_ARRAY_GET(evol, 1);
    area.y_min = FTN_REAL_ARRAY_GET(evol, 2);
    area.y_max = FTN_REAL_ARRAY_GET(evol, 3);
    area.z_min = FTN_REAL_ARRAY_GET(evol, 4);
    area.z_max = FTN_REAL_ARRAY_GET(evol, 5);
    /* decode input data */
    here = (int*)&datrec[0];
    if (here[0] == 2){
      data.buffer_size = here[0];
      data.init_pos = here[1];
      /* only one echo mode */
      data.pets.pet_r1.unused = 0;
      pinit_string3(ws_id, string_dev, buffer, pet, &area, &data);
    } else {
      printf("ERROR in pinst: wrong number of integers in data record\n");
    }
    free(buffer);
  }
}

/*******************************************************************************
 * prqst
 *
 * DESCR:       request string
 * RETURNS:	status, length, string
 */

FTN_SUBROUTINE(prqst)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(stdnr),
		      int * stat,
		      int * length,
		      char * string)
{
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint string_dev = FTN_INTEGER_GET(stdnr);
  preq_string(ws_id, string_dev, (Pin_status*)stat, string);
  *length = strlen(string);
}

/*******************************************************************************
 * pinch3
 *
 * DESCR:       initialize choice 3
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinch3)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(chdnr),
		       FTN_INTEGER(istat),
		       FTN_INTEGER(ichnr),
		       FTN_INTEGER(ipet),
		       FTN_REAL_ARRAY(evol),
		       FTN_INTEGER(ldr),
		       char * datrec)
{
# define MAX_PROMPTS 32
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint choice_dev = FTN_INTEGER_GET(chdnr);
  Pin_status init_status = (Pin_status)FTN_INTEGER_GET(istat);
  Pint init_choice = FTN_INTEGER_GET(ichnr);
  Pint pet = FTN_INTEGER_GET(ipet);
  int num_prompts;
  Ppr_switch prompts[MAX_PROMPTS];
  int i;
  int nstrings, charlen[MAX_PROMPTS+1];
  char *strings, *buffer;
  char *str[MAX_PROMPTS+1];

  int *ip;

  Plimit3 echo_volume;
  Pchoice_data3 choice_data_rec;
  echo_volume.x_min = FTN_REAL_ARRAY_GET(evol, 0);
  echo_volume.x_max = FTN_REAL_ARRAY_GET(evol, 1);
  echo_volume.y_min = FTN_REAL_ARRAY_GET(evol, 2);
  echo_volume.y_max = FTN_REAL_ARRAY_GET(evol, 3);
  echo_volume.z_min = FTN_REAL_ARRAY_GET(evol, 4);
  echo_volume.z_max = FTN_REAL_ARRAY_GET(evol, 5);

  ip = (int*)&datrec[0];
  num_prompts = ip[0];
  ip = &ip[1];
  for (i=0; i<num_prompts;i++){
    prompts[i] = (Ppr_switch)ip[i];
  }
  /* skip number of floats at ip[num_prompts] */
  if (ip[num_prompts] != 0){
    printf("Error in npinch3: unexpected number of floats %d\n", ip[num_prompts]);
  };
  ip = (int*)&ip[num_prompts+1];
  nstrings = ip[0];
  ip = &ip[1];
  /* copy over the length of the strings */
  memcpy(&charlen[0], (int*)&ip[0], nstrings*sizeof(int));
  strings = (char *) &ip[nstrings];
  /* FIXME: we need to release these strings later on */
  for (i=0; i<nstrings; i++){
    buffer = (char*) malloc((charlen[i]+1)*sizeof(char));
    if (buffer != NULL){
      strncpy(buffer, strings, charlen[i]*sizeof(char));
      buffer[strlen(strings)] = '\0';
    }
    str[i] = buffer;
#ifdef DEBUG
    printf("DEBUG pinch3: string nr %d %s length %d expected %d\n", i, str[i], (int) strlen(str[i]), charlen[i]);
#endif
    strings += 1 + strlen(strings);
  }
#ifdef DEBUG
  printf("DEBUG pinch3: got %d strings\n", nstrings);
  for (i=0; i<nstrings; i++){
    printf("DEBUG pinch3 Nr.: %d: Content: \"%s\" length %d\n", i, str[i], (int)strlen(str[i]));
  }
#endif
  switch (pet) {
  case 1:
  case -1:
    break;
  case 2:
  case-2:
    choice_data_rec.pets.pet_r2.num_prompts = num_prompts;
    choice_data_rec.pets.pet_r2.prompts = &prompts[0];
    break;
    /* first string is the title */
  case 3:
  case -3:
  case 4:
  case -4:
    choice_data_rec.pets.pet_r3.num_strings = num_prompts+1;
    choice_data_rec.pets.pet_r3.strings = str;
  }
  pinit_choice3(ws_id, choice_dev, init_status, init_choice, pet, &echo_volume, &choice_data_rec);
}

/*******************************************************************************
 * pschm
 *
 * DESCR:       set choice mode
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pschm)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(chdnr),
		      FTN_INTEGER(mode),
		      FTN_INTEGER(esw)
		      ){
  Pint ws_id =  FTN_INTEGER_GET(wkid);
  Pint choice_dev = FTN_INTEGER_GET(chdnr);
  Pint imode = FTN_INTEGER_GET(mode);
  Pint iesw = FTN_INTEGER_GET(esw);
  pset_choice_mode(ws_id, choice_dev, imode, iesw);
}

/*******************************************************************************
 * pgtch
 *
 * DESCR:       get choice
 * RETURNS:	status, choice number
 */

FTN_SUBROUTINE(pgtch)(
                      Pint* stat,
                      Pint* chnr) {
  Pin_status status = 0;
  Pint choice = 0;
  pget_choice(&status, &choice);
  *stat = (Pint)status;
  *chnr = choice;
}

/*******************************************************************************
 * prqch
 *
 * DESCR:       request choice
 * RETURNS:	status, choice number
 */

FTN_SUBROUTINE(prqch)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(chdnr),
		      Pint *stat,
		      Pint *chnr
		      ) {
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint choice_dev = FTN_INTEGER_GET(chdnr);
  preq_choice(ws_id, choice_dev, (Pin_status*)stat, chnr);
}

/*******************************************************************************
 * pinvl3
 *
 * DESCR:       initialize valuators 3
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pinvl3)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(vldnr),
		       FTN_REAL(ival),
		       FTN_INTEGER(ipet),
		       FTN_REAL_ARRAY(evol),
		       FTN_INTEGER(ldr),
		       char* datrec) {
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint val_dev = FTN_INTEGER_GET(vldnr);
  Pfloat init_value =FTN_REAL_GET(ival);
  Pint pet = FTN_INTEGER_GET(ipet);
  Plimit3 echo_volume;
  Pval_data val_data_rec;
  Pfloat * rp;
  Pint * ip;
  char* cp;
  char* buffer;
  int i, length, l1, l2, l3, l4;
  int nstrings, charlen;
  Pint num_boxed;
  echo_volume.x_min = FTN_REAL_ARRAY_GET(evol, 0);
  echo_volume.x_max = FTN_REAL_ARRAY_GET(evol, 1);
  echo_volume.y_min = FTN_REAL_ARRAY_GET(evol, 2);
  echo_volume.y_max = FTN_REAL_ARRAY_GET(evol, 3);
  echo_volume.z_min = FTN_REAL_ARRAY_GET(evol, 4);
  echo_volume.z_max = FTN_REAL_ARRAY_GET(evol, 5);
  ip = (int*)&datrec[0]; //  number of ints;
  switch (abs(pet)){
  case 3:
    if (ip[0] != 1) printf("WARNING: Wrong number of integers for echo mode. Expected 1 but found %d\n", ip[0]);
    num_boxed = ip[1];
    ip = &ip[1];
    break;
  default:
    if (ip[0] != 0) printf("WARNING: Wrong number of integers. Expected 0 but found %d\n", ip[0]);
    num_boxed = 0;
    break;
  }
  /* How many valuator devices to be put into a single box */
  if (ip[1] != 2) printf("WARNING: Wrong number of floats. Expected 2 but found %d\n", ip[1]);
  rp = (float*)&ip[2];
  val_data_rec.low = rp[0];
  val_data_rec.high = rp[1];
  val_data_rec.num_boxed = num_boxed;
  val_data_rec.pets.pet_u1.label = NULL;
  val_data_rec.pets.pet_u1.format = NULL;
  val_data_rec.pets.pet_u1.low_label = NULL;
  val_data_rec.pets.pet_u1.high_label = NULL;
  ip = (int*)&rp[2];
  if (pet <0){
    nstrings = ip[0];
    switch (nstrings){
    case 1:
      l1 = ip[1] + 1;
      l2 = 0;
      l3 = 0;
      l4 = 0;
      break;
    case 2:
      l1 = ip[1] + 1;
      l2 = ip[2] + 1;
      l3 = 0;
      l4 = 0;
      break;
    case 3:
      l1 = ip[1] + 1;
      l2 = ip[2] + 1;
      l3 = ip[3] + 1;
      l4 = 0;
    case 4:
      l1 = ip[1] + 1;
      l2 = ip[2] + 1;
      l3 = ip[3] + 1;
      l4 = ip[4] + 1;
    }
    /* FIXME: release space afterwards */
    cp = (char *)&ip[nstrings+1];
    if (l1 > 0){
      buffer = (char*) malloc((l1+1)*sizeof(char));
      if (buffer != NULL){
	strncpy(buffer, &cp[0], l1*sizeof(char));
	buffer[l1] = '\0';
	val_data_rec.pets.pet_u1.label = buffer;
      } else {
	val_data_rec.pets.pet_u1.label = WST_DEFAULT_VALUATOR_LABEL;
      }
      cp += l1*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.label = WST_DEFAULT_VALUATOR_LABEL;
    }
    if (l2 > 0){
      buffer = (char*) malloc((l2+1)*sizeof(char));
      if (buffer != NULL){
	strncpy(buffer, &cp[0], l2*sizeof(char));
	buffer[l2] = '\0';
	val_data_rec.pets.pet_u1.format = buffer;
      } else {
	val_data_rec.pets.pet_u1.format = WST_DEFAULT_VALUATOR_FORMAT;
      }
      cp += l2*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.format = WST_DEFAULT_VALUATOR_FORMAT;
    }
    if (l3 > 0){
      buffer = (char*) malloc((l3+1)*sizeof(char));
      if (buffer != NULL){
	strncpy(buffer, &cp[0], l3*sizeof(char));
	buffer[l3] = '\0';
	val_data_rec.pets.pet_u1.low_label = buffer;
      } else {
	val_data_rec.pets.pet_u1.low_label = WST_DEFAULT_VALUATOR_LOW_LABEL;
      }
      cp += l3*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.low_label = WST_DEFAULT_VALUATOR_LOW_LABEL;
    }
    if (l4 > 0){
      buffer = (char*) malloc((l4+1)*sizeof(char));
      if (buffer != NULL){
	strncpy(buffer, &cp[0], l4*sizeof(char));
	buffer[l4] = '\0';
	val_data_rec.pets.pet_u1.high_label = buffer;
      } else {
	val_data_rec.pets.pet_u1.high_label = WST_DEFAULT_VALUATOR_HIGH_LABEL;
      }
      cp += l4*sizeof(char);
    } else {
      val_data_rec.pets.pet_u1.high_label = WST_DEFAULT_VALUATOR_HIGH_LABEL;
    }
  }
  pinit_val3(ws_id, val_dev, init_value, pet, &echo_volume, &val_data_rec);
}

/*******************************************************************************
 * psvlm
 *
 * DESCR:       set valuator mode
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(psvlm)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(vldnr),
		      FTN_INTEGER(mode),
		      FTN_INTEGER(esw)
		      ){
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint valdev = FTN_INTEGER_GET(vldnr);
  Pint imode = FTN_INTEGER_GET(mode);
  Pint iesw = FTN_INTEGER_GET(esw);

  pset_val_mode(ws_id, valdev, imode, iesw);
}

/*******************************************************************************
 * pgtvl
 *
 * DESCR:       get valuator
 * RETURNS:	value (float)
 */

FTN_SUBROUTINE(pgtvl)(
		      float* val) {
  Pfloat valuator;
  pget_val(&valuator);
  *val = valuator;
}

/*******************************************************************************
 * prqvl
 *
 * DESCR:       request valuator
 * RETURNS:	status, value
 */

FTN_SUBROUTINE(prqvl)(
		      FTN_INTEGER(wkid),
		      FTN_INTEGER(vldnr),
		      Pint *stat,
		      Pfloat *val
		      ) {
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint val_dev = FTN_INTEGER_GET(vldnr);
  preq_valuator(ws_id, val_dev, (Pin_status*)stat, val);
}

/*******************************************************************************
 * pflush
 *
 * DESCR:       flush device events
 * RETURNS:	N/A
 */

FTN_SUBROUTINE(pflush)(
		       FTN_INTEGER(wkid),
		       FTN_INTEGER(icl),
		       FTN_INTEGER(idnr)
		       ) {
  Pint ws_id = FTN_INTEGER_GET(wkid);
  Pint i_cl = FTN_INTEGER_GET(icl);
  Pint i_dnr = FTN_INTEGER_GET(idnr);
  pflush_events(ws_id, i_cl, i_dnr);
}

/*******************************************************************************
 * pqdlc3
 *
 * DESCR:       inquire default locator device data 3
 * RETURNS:	error index, init position in WC, number of echoes, echoes, 
 *              echo volume in DC, number of elements in data record, data record
 */

FTN_SUBROUTINE(pqdlc3)(
		       FTN_INTEGER(wtype),
		       FTN_INTEGER(devno),
		       FTN_INTEGER(n),
		       FTN_INTEGER(mldr),
		       int* errind,
		       float* dpx,
		       float* dpy,
		       float* dpz,
		       int* ol,
		       int* pet,
		       float evol[6],
		       int* ldr,
		       char* datrec
		       ) {
  Wst *wst;
  Wst_input_wsdt *idt;

  Pint ws_type = FTN_INTEGER_GET(wtype);
  Pint devnum = FTN_INTEGER_GET(devno);

  *errind = 0;
  if (devnum <= WST_MAX_NUM_LOCATOR_DEVS){
    wst = phg_wst_find(&PHG_WST_LIST, ws_type);
    if (wst == NULL) {
      ERR_REPORT(PHG_ERH, ERR52);
    }
    idt = &wst->desc_tbl.phigs_dt.in_dt;
    *dpx = idt->locators[devnum].position.x;
    *dpy = idt->locators[devnum].position.y;
    *dpz = idt->locators[devnum].position.z;
    evol[0] = idt->locators[devnum].e_volume.x_min;
    evol[1] = idt->locators[devnum].e_volume.x_max;
    evol[2] = idt->locators[devnum].e_volume.y_min;
    evol[3] = idt->locators[devnum].e_volume.y_max;
    evol[4] = idt->locators[devnum].e_volume.z_min;
    evol[5] = idt->locators[devnum].e_volume.z_max;
    *pet =  idt->locators[devnum].pets[0];
  } else {
    *errind = 1;
    ERR_REPORT(PHG_ERH, ERR250);
  }
  *ldr = 0;
}
