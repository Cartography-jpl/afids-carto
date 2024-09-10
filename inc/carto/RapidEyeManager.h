#ifndef RAPIDEYEMANAGER
#define RAPIDEYEMANAGER

#include <stdio.h>
#include "carto/ImageUtils.h"

                                    // BANDWIDTHS
#define RAPIDEYE_BLUE_IR      0     // 0.44  - 0.51  micron 
#define RAPIDEYE_GREEN_IR     1     // 0.52  - 0.59  micron
#define RAPIDEYE_RED_IR       2     // 0.63  - 0.685 micron
#define RAPIDEYE_RED_EDGE_IR  3     // 0.69  - 0.73  micron
#define RAPIDEYE_NEAR_IR      4     // 0.76  - 0.85  micron

#define RAPIDEYE_BAND1       0
#define RAPIDEYE_BAND2       1
#define RAPIDEYE_BAND3       2
#define RAPIDEYE_BAND4       3
#define RAPIDEYE_BAND5       4
#define RAPIDEYE_N_BANDS     5
#define RAPIDEYE_N_REF_BANDS 5

#define RAPIDEYE_BAND_SET        1
#define RAPIDEYE_BAND_NOT_SET    0

#define RAPIDEYE_METABUF_SIZE 30000000
// RAPIDEYE ERROR CODES

#define RAPIDEYE_GAIN_INIT        -999
#define RAPIDEYE_ELEV_INIT        -999

#define RAPIDEYE_INVALID_BAND     -6
#define RAPIDEYE_GAIN_NOT_SET_ERR -5
#define RAPIDEYE_NO_DATE_DATA     -4
#define RAPIDEYE_NO_ELEV_DATA     -3
#define RAPIDEYE_NO_METAFILE      -2
#define RAPIDEYE_UNIT_NOT_SET_ERR -1
#define RAPIDEYE_SUCCESS           1

/***************************************************************************/
/* band constants exoatmospheric irradiances bands 1 thru 5                */
/* DATA ACQUIRED FROM http://www.rapideye.de/home/products/index.html      */
/*                    under Frequently Asked Questions link                */
/***************************************************************************/
/*                             BANDS =   1       2       3       4     5   */
static const double RAPIDEYE_ESUN[6] = {1997.8, 1836.5, 1560.4, 1395, 1124.4};

typedef struct
{
   VICAR_IMAGE* images[RAPIDEYE_N_BANDS];
   unsigned char units_set[RAPIDEYE_N_BANDS];
   double dist;
   double gain[RAPIDEYE_N_BANDS];
   double elevation;
   double **rad_buffs;
   double **ref_buffs;
   int curr_line_in_rad_buffs[RAPIDEYE_N_BANDS];
   int curr_line_in_ref_buffs[RAPIDEYE_N_REF_BANDS];
}RAPIDEYE_MANAGER;

/***************************************************************************/
// getRapidEyeManager: returns a RAPIDEYE_MANAGER struct
//
// input:
// ======
// + VICAR_IMAGE
//    - VICAR_IMAGE structs for raw dn files
//    - ordered in accordance with RAPIDEYE_BAND#
//    - uninitialized bands set to NULL
// + metafile
//    - opened file containing metadata
//
// output:
// =======
// + RAPIDEYE_MANAGER struct
/***************************************************************************/
RAPIDEYE_MANAGER* RE_getRapidEyeManager(VICAR_IMAGE *vi[RAPIDEYE_N_BANDS], FILE *metafile);

/***************************************************************************/
// deleteRapidEyeManager: deletes a RAPIDEYE_MANAGER struct and frees buffers
//                       and closes files
//
// input:
// ======
// + **rem
//    - RAPIDEYE_MANAGER struct to free
/***************************************************************************/
void RE_deleteRapidEyeManager(RAPIDEYE_MANAGER **rem);

/***************************************************************************/
// getRadianceLine: calculates the radiance from raw dn image
//
// input:
// ======
// + *rem
//    - RAPIDEYE_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate radiance for
// + line
//    - line to calculate radiance for
//
// output:
// =======
// + rem->rad_buffs[band]
//    - contains the radiance data inside the rad_buffs
/***************************************************************************/
int RE_getRadianceLine(RAPIDEYE_MANAGER *rem, int band, int line);

/***************************************************************************/
// createRadianceImage: creates a radiance image for specified band
//
// input:
// ======
// + *rem
//    - RAPIDEYE_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate radiance for
//
// output:
// =======
// + *vi
//    - VICAR_IMAGE struct containing output file
/***************************************************************************/
void RE_createRadianceImage(RAPIDEYE_MANAGER *rem, VICAR_IMAGE *vi, int band);

/***************************************************************************/
// getReflectanceLine: calculates the reflectance from raw dn image
//
// input:
// ======
// + *rem
//    - RAPIDEYE_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate reflectance for
// + line
//    - line to calculate reflectance for
//
// output:
// =======
// + rem->ref_buffs[band]
//    - contains the reflectance data inside the rad_buffs
/***************************************************************************/
int RE_getReflectanceLine(RAPIDEYE_MANAGER *rem, int band, int line);

/***************************************************************************/
// createReflectanceImage: creates a reflectance image for specified band
//
// input:
// ======
// + *rem
//    - RAPIDEYE_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate reflectance for
//
// output:
// =======
// + *vi
//    - VICAR_IMAGE struct containing output file
/***************************************************************************/
void RE_createReflectanceImage(RAPIDEYE_MANAGER *rem, VICAR_IMAGE *vi, int band);

/***************************************************************************/
// getBTempLine: calculates the brightness temp from raw dn image
//
// input:
// ======
// + *rem
//    - RAPIDEYE_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate brightness temp for
// + line
//    - line to calculate brightness temp for
//
// output:
// =======
// + rem->b_temp_buffs[band]
//    - contains the brightness temp data inside the rad_buffs
/***************************************************************************/
int RE_getBTempLine(RAPIDEYE_MANAGER *rem, int band, int line);

/***************************************************************************/
// createBTempImage: creates a brightness temp image for specified band
//
// input:
// ======
// + *rem
//    - RAPIDEYE_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate brightness temp for
//
// output:
// =======
// + *vi
//    - VICAR_IMAGE struct containing output file
/***************************************************************************/
void RE_createBTempImage(RAPIDEYE_MANAGER *rem, VICAR_IMAGE *vi, int band);

#endif
