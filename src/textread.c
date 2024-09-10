/*******************************************************************************

  Title:    textread
  Author:   Mike Burl
  Date:     2005/04/20 
  Function: Read all characters from a file into a txt string.

  NOTE:     Although the number of bytes could be recovered from strlen,
            we return it explicitly since we know it and strlen
            would require an extra pass through the string.

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include "carto/burl.h"
#include "carto/qmalloc.h"
#include "carto/textread.h"

int textread(char *fname, char **txt_adr, long int *n_bytes_adr)

{
  FILE  *fp;
  long  n_bytes, i;
  char  *txt;
  char  infunc[] = "textread";

  fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr, "ERROR (%s): fopen failed on file |%s|\n", infunc, fname);
    return(ERR);
  }

  fseek(fp, 0L, SEEK_END);
  n_bytes = ftell(fp);
  *n_bytes_adr = n_bytes;
  rewind(fp);

  txt = (char *) qmalloc(n_bytes+1, sizeof(char), 1, infunc, "txt");
  *txt_adr = txt;

  for (i = 0; i < n_bytes; i++) {
    txt[i] = (char) fgetc(fp);  
    if (feof(fp)  != 0) {
      fprintf(stderr, "ERROR (%s): Attempt to read past EOF in file %s, i = %ld, n_bytes = %ld\n", infunc, fname, i, n_bytes);
      return(ERR);
    }
  }
  txt[n_bytes] = '\0';

  fclose(fp);
  return(OK);
}
