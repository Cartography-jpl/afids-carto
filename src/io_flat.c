/*******************************************************************************

Title:    io_flat.c
Author:   Mike Burl
Date:     Oct 23, 2002
Function: Read a flat file into a double array (similar to Matlab's load function).

History:  

          20050127 (MCB) - Changed filename to io_flat.c

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "carto/imageio_return_values.h"
#include "carto/io_flat.h"
#include "carto/count_lines.h"
#include "carto/tokenize.h"
#include "carto/qmalloc.h"

/**************************************/
/* GLOBAL DECLARATIONS                */
/**************************************/

#define MAXLINE 65536

/**************************************/
/* read_flat                          */
/**************************************/


int read_flat(char *iname, int *zr_adr, int *zc_adr, double **Z_adr)

{
  FILE           *fp;
  int            zr, zc=0;
  double         *Z=NULL;
  char           buffer[MAXLINE];
  int            i, j;
  int            n_tokens;
  int            *token_beg, *token_end;
  char           *token;
  char           infunc[] = "read_flat";

/*---------------------------------------------------------------------------------*/
  count_lines(iname, 2, &zr, &fp); /* Determine the number of lines */
  for (i = 0; i < zr; i++) {
    if (fgets(buffer, MAXLINE, fp) == NULL) { /* Read each line */
      fprintf(stderr, "ERROR (%s): fgets failed on line %d of %d\n", infunc, i+1, zr);
      fclose(fp);
      return(ERR);
    }

    /* Parse numbers within line */
    tokenize(buffer, " ", &n_tokens, &token_beg, &token_end);
    if (i == 0) {
      zc = n_tokens;
      Z = qmalloc(zr*zc, sizeof(double), 1, infunc, "Z");
    }
    else if (n_tokens != zc) {
      fprintf(stderr, "ERROR (%s): n_tokens(line %d) = %d; n_tokens(line 1) = %d\n", infunc, i+1, n_tokens, zc);
      fclose(fp);
      free((void *) token_beg);
      free((void *) token_end);
      free((void *) Z);
      fclose(fp);
      return(ERR);
    }

    for (j = 0; j < zc; j++) {
      token = qmalloc((token_end[j]-token_beg[j]+2), sizeof(char), 0, infunc, "token");
      extract_token(token, buffer, token_beg, token_end, j);
      Z[i*zc+j] = (double) atof(token);
      free((void *) token);
    }
    free((void *) token_beg);
    free((void *) token_end);
  }

  *zr_adr = zr;
  *zc_adr = zc;
  *Z_adr = Z;
  fclose(fp);
  return(OK);
}


/**************************************/
/* write_flat                           */
/**************************************/


int write_flat(char *oname, int zr, int zc, double *Z)
{
  FILE   *fp;
  int    i, j;
  char   infunc[] = "write_flat";

  fp = fopen(oname, "w");
  if (fp == NULL) {
    fprintf(stderr, "ERROR (%s): open failed on file %s\n", infunc, oname);
    return(ERR);
  }

  for (i = 0; i < zr; i++) {
    for (j = 0; j < zc; j++) {
      if (j == 0) {
        fprintf(fp, "%.15f", Z[i*zc+j]);
      }
      else {
        fprintf(fp, " %.15f", Z[i*zc+j]);      
      }
    }
    fprintf(fp, "\n");
  }

  return(OK);
}

/**************************************/
/* print_flat                           */
/**************************************/


int print_flat(int zr, int zc, double *Z)
{
  int    i, j;
  /*  char   infunc[] = "print_flat"; */

  for (i = 0; i < zr; i++) {
    for (j = 0; j < zc; j++) {
      if (j == 0) {
        printf("%.15f", Z[i*zc+j]);
      }
      else {
        printf(" %.15f", Z[i*zc+j]);      
      }
    }
    printf("\n");
  }

  return(OK);
}
