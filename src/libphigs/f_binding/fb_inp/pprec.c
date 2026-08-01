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

/**
 * \file pprec.c
 * \brief pack data record
 *
 * \return error, length, data record
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

