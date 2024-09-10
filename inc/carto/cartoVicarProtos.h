#ifndef CARTOVICARPROTOS_H
#define CARTOVICARPROTOS_H

/* Missing from vicar includes */

void zifmessage( char * );
void zmabend( char * );
void zprnt( int dcode,      /* format code of data to be printed   */
	    int n,          /* number of data elements to print  */
	    void *buf,      /* array of data elements to be printed*/
	    char *title);   /* string to be printed in front of data */

#include "taeconf.inp"
/* conflicts with hdf.h */
#undef FAIL

FUNCTION VOID q_init( struct PARBLK	*p,		/* PARBLK to initialize		*/
		      FUNINT		pool_size,	/* bytes allocated in p.pool	*/
		      FUNINT		mode );		/* P_ABORT or P_CONT		*/
FUNCTION CODE q_intg( struct PARBLK	*p,		/* V-block			*/
		      TEXT		name[],		/* in: variable name		*/
		      FUNINT 		count,		/* in: count of variable	*/
		      TAEINT		intg[],		/* in: value vector		*/
		      FUNINT		mode );		/* in: P_UPDATE or P_ADD	*/
FUNCTION CODE q_real( struct PARBLK	*p,		/* V-block			*/
		      TEXT		name[],		/* in: variable name		*/
		      FUNINT 		count,		/* in: count of variable	*/
		      TAEFLOAT		real[],		/* in: reals			*/
		      FUNINT		mode );		/* in: P_UPDATE or P_ADD	*/
FUNCTION CODE q_string( struct PARBLK	*p,		/* V-block			*/
			TEXT		name[],		/* in: variable name		*/
			FUNINT 		count,		/* in: count of vector		*/
			/* (0 means set count = 0)	*/
			TEXT		*vector[],	/* in: vector of string ptrs	*/
			FUNINT 		mode );		/* in: P_UPDATE or P_ADD	*/

#endif
