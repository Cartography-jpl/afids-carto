#ifndef __IO_FLAT
#define __IO_FLAT

int read_flat(char *iname, int *zr_adr, int *zc_adr, double **Z_adr);
int write_flat(char *oname, int zr, int zc, double *Z);
int print_flat(int zr, int zc, double *Z);

#endif
