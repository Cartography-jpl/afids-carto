#ifndef QBMANAGER
#define QBMANAGER

#include <stdio.h>
#include "carto/ImageUtils.h"

#define QB_N_BANDS          5
#define QB_MSI_BANDS        4

                               // WAVELENGTH
#define QB_BLUE            0   // 0.450-0.520 micron
#define QB_GREEN           1   // 0.520-0.600 micron
#define QB_RED             2   // 0.630-0.690 micron
#define QB_NIR             3   // 0.760-0.900 micron
#define QB_PAN             4   // 0.450-0.900 micron

#define QB_IS_PAN          1
#define QB_IS_NOT_PAN      0

#define QB_BAND1     0
#define QB_BAND2     1
#define QB_BAND3     2
#define QB_BAND4     3
#define QB_BAND_PAN  4

#define QB_UNIT_SET      1
#define QB_UNIT_NOT_SET  0
#define QB_META_NOT_SET  0
#define QB_META_CAL_SET  1
#define QB_META_BWID_SET 2
#define QB_META_ALL_SET  3

#define QB_METABUF_SIZE 10000

// QB ERROR CODES

#define QB_UNINITIALIZED  -999
#define QB_NO_DATA          -2
#define QB_NO_METAFILE      -1
#define QB_SUCCESS           1

/***************************************************************************/
/* band constants solar spectral irradiances bands 1 thru 8 + pan          */
/***************************************************************************/
static const double QB_ESUN[QB_N_BANDS] =
                       /*BAND1    BAND2    BAND3    BAND4    PANBAND*/
                        {1924.59, 1843.08, 1574.77, 1113.71, 1381.79};

typedef struct
{
   VICAR_IMAGE* images[QB_N_BANDS];
   unsigned char units_set[QB_N_BANDS];
   double **rad_buffs;
   double **ref_buffs;
   int curr_line_in_rad_buffs[QB_N_BANDS];
   int curr_line_in_ref_buffs[QB_N_BANDS];

   /* Metadata */
   double year;
   double month;
   double day;
   double hh;
   double mm;
   double ssdd;
   double solarElevation;
   double solarZenithAngle;
   double solarZenithAngleInRadians;
   double solarDist;

   double pan_ssdd;
   double pan_solarElevation;
   double pan_solarZenithAngle;
   double pan_solarZenithAngleInRadians;
   double pan_solarDist;

   double absCalFactor[QB_N_BANDS];
   double effectiveBandwidth[QB_N_BANDS];

   /* flags set if calibration factor and bandwidth are set from metadata */
   /* QB_META_NOT_SET  - non are set */
   /* QB_META_CAL_SET  - calibration factor was set */
   /* QB_META_BWID_SET - bandwidth was set */
   /* QB_META_ALL_SET  - both calibration factor and bandwidth were set */
   unsigned char metaFlags[QB_N_BANDS];
}QB_MANAGER;

/***************************************************************************/
// QB_getQBManager: returns a QB_MANAGER struct
//
// input:
// ======
// + vi
//    - VICAR_IMAGE array
//    - ordered in accordance with QB_BAND#
//    - !! uninitialized units must be set to NULL !!
// + metafname
//    - path and filename of .IMD metafile for multi bands
//    - pass in NULL if not doing multi bands
// + panMetaFname
//    - path and filename of .IMD metafile for pan band
//    - pass in NULL if not doing pan band
//
// output:
// =======
// + QB_MANAGER struct
/***************************************************************************/
QB_MANAGER* QB_getQBManager(VICAR_IMAGE *vi[QB_N_BANDS], char* metafname, char* panMetaFname);

/***************************************************************************/
// QB_deleteQBManager: deletes a QB_MANAGER struct and frees buffers
//                       and closes files
//
// input:
// ======
// + **qb
//    - QB_MANAGER struct to free
/***************************************************************************/
void QB_deleteQBManager(QB_MANAGER **qb);

/***************************************************************************/
// QB_setTOARadianceLine: calculates the radiance from raw dn image
//
// input:
// ======
// + *qb
//    - QB_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate radiance for
// + line
//    - line to calculate radiance for
//
// output:
// =======
// + qbm->rad_buffs[band]
//    - contains the radiance data inside the rad_buffs
// + status
//    - QB_SUCCESS
/***************************************************************************/
int QB_setTOARadianceLine(QB_MANAGER *qb, int band, int line);

/***************************************************************************/
// QB_createTOARadianceImage: creates a radiance image for specified band
//
// input:
// ======
// + *qb
//    - QB_MANAGER struct with the band raw dn file set
// + outInst
//    - in out argument, the out instance of output file
// + band
//    - band to calculate radiance for
//
// output:
// =======
// + *vi
//    - VICAR_IMAGE struct containing output file
/***************************************************************************/
int QB_createTOARadianceImage(QB_MANAGER *qb, int outInst, int band);

/***************************************************************************/
// QB_setTOAReflectanceLine: calculates the reflectance from raw dn image
//
// input:
// ======
// + *qb
//    - QB_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate reflectance for
// + line
//    - line to calculate reflectance for
//
// output:
// =======
// + qbm->ref_buffs[band]
//    - contains the reflectance data inside the rad_buffs
// + status
//    - QB_SUCCESS
/***************************************************************************/
int QB_setTOAReflectanceLine(QB_MANAGER *qb, int band, int line);

/***************************************************************************/
// QB_createTOAReflectanceImage: creates a reflectance image for specified band
//
// input:
// ======
// + *qb
//    - QB_MANAGER struct with the band raw dn file set
// + outInst
//    - int out argument, the out instance of the output file
// + band
//    - band to calculate reflectance for
//
// output:
// =======
// + *vi
//    - VICAR_IMAGE struct containing output file
/***************************************************************************/
int QB_createTOAReflectanceImage(QB_MANAGER *qb, int outInst, int band);

/***************************************************************************/
// QB_print: prints out given qb struct to screen
//
// input:
// ======
// + *qb
//    - QB_MANAGER struct with the band raw dn file set
//
// output:
// =======
// + prints out to screen
/***************************************************************************/
void QB_print(QB_MANAGER *qb);

#endif
