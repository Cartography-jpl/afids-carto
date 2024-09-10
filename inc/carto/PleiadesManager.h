#ifndef PLEIADESMANAGER
#define PLEIADESMANAGER

#include <stdio.h>
#include "carto/ImageUtils.h"

#define PLEIADES_N_BANDS          5
#define PLEIADES_MSI_BANDS        4

                                // MIN/MAX WAVELENGTH
#define PLDS_BLUE            0   // 0.43/0.55 micron
#define PLDS_GREEN           1   // 0.50/0.62 micron
#define PLDS_RED             2   // 0.59/0.71 micron
#define PLDS_NIR             3   // 0.74/0.94 micron
#define PLDS_PAN             4   // 0.47/0.83 micron

#define PLDS_IS_PAN          1
#define PLDS_IS_NOT_PAN      0

#define PLDS_BAND1     0
#define PLDS_BAND2     1
#define PLDS_BAND3     2
#define PLDS_BAND4     3
#define PLDS_BAND_PAN  4

#define PLDS_UNIT_SET      1
#define PLDS_UNIT_NOT_SET  0
#define PLDS_META_NOT_SET  0
#define PLDS_META_GAIN_SET  1
#define PLDS_META_BIAS_SET 2
#define PLDS_META_ALL_SET  3

#define PLDS_METABUF_SIZE 10000

// PLDS ERROR CODES

#define PLDS_UNINITIALIZED  -999
#define PLDS_NO_DATA          -2
#define PLDS_NO_METAFILE      -1
#define PLDS_SUCCESS           1

/***************************************************************************/
/* band constants solar spectral irradiances bands 1 thru 4 + pan          */
/***************************************************************************/
static const double PLDS_ESUN[PLEIADES_N_BANDS] =
                       /*BAND1      BAND2      BAND3      BAND4      PANBAND*/
                        {1915,      1830,      1594,      1060,      1549};

typedef struct
{
   VICAR_IMAGE* images[PLEIADES_N_BANDS];
   unsigned char units_set[PLEIADES_N_BANDS];
   double **rad_buffs;
   double **ref_buffs;
   double **radLookupTable;
   double **refLookupTable;
   int curr_line_in_rad_buffs[PLEIADES_N_BANDS];
   int curr_line_in_ref_buffs[PLEIADES_N_BANDS];

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

   double pan_year;
   double pan_month;
   double pan_day;
   double pan_hh;
   double pan_mm;
   double pan_ssdd;
   double pan_solarElevation;
   double pan_solarZenithAngle;
   double pan_solarZenithAngleInRadians;
   double pan_solarDist;

   double gain[PLEIADES_N_BANDS];
   double bias[PLEIADES_N_BANDS];

   /* flags set if calibration factor and bandwidth are set from metadata */
   /* PLDS_META_NOT_SET  - non are set */
   /* PLDS_META_CAL_SET  - calibration factor was set */
   /* PLDS_META_BWID_SET - bandwidth was set */
   /* PLDS_META_ALL_SET  - both calibration factor and bandwidth were set */
   unsigned char metaFlags[PLEIADES_N_BANDS];
}PLDS_MANAGER;

/***************************************************************************/
// PLDS_getPLDSManager: returns a PLDS_MANAGER struct
//
// input:
// ======
// + vi
//    - VICAR_IMAGE array
//    - ordered in accordance with PLDS_BAND#
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
// + PLDS_MANAGER struct
/***************************************************************************/
PLDS_MANAGER* PLDS_getPLDSManager(VICAR_IMAGE *vi[PLEIADES_N_BANDS], char* metafname, char* panMetaFname);

/***************************************************************************/
// PLDS_deletePLDSManager: deletes a PLDS_MANAGER struct and frees buffers
//                       and closes files
//
// input:
// ======
// + **plds
//    - PLDS_MANAGER struct to free
/***************************************************************************/
void PLDS_deletePLDSManager(PLDS_MANAGER **plds);

/***************************************************************************/
// PLDS_setTOARadianceLine: calculates the radiance from raw dn image
//
// input:
// ======
// + *plds
//    - PLDS_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate radiance for
// + line
//    - line to calculate radiance for
//
// output:
// =======
// + plds->rad_buffs[band]
//    - contains the radiance data inside the rad_buffs
// + status
//    - PLDS_SUCCESS
/***************************************************************************/
int PLDS_setTOARadianceLine(PLDS_MANAGER *plds, int band, int line);

/***************************************************************************/
// PLDS_createTOARadianceImage: creates a radiance image for specified band
//
// input:
// ======
// + *plds
//    - PLDS_MANAGER struct with the band raw dn file set
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
int PLDS_createTOARadianceImage(PLDS_MANAGER *plds, int outInst, int band);

/***************************************************************************/
// PLDS_setTOAReflectanceLine: calculates the reflectance from raw dn image
//
// input:
// ======
// + *plds
//    - PLDS_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate reflectance for
// + line
//    - line to calculate reflectance for
//
// output:
// =======
// + plds->ref_buffs[band]
//    - contains the reflectance data inside the rad_buffs
// + status
//    - PLDS_SUCCESS
/***************************************************************************/
int PLDS_setTOAReflectanceLine(PLDS_MANAGER *plds, int band, int line);

/***************************************************************************/
// PLDS_createTOAReflectanceImage: creates a reflectance image for specified band
//
// input:
// ======
// + *plds
//    - PLDS_MANAGER struct with the band raw dn file set
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
int PLDS_createTOAReflectanceImage(PLDS_MANAGER *plds, int outInst, int band);

/***************************************************************************/
// PLDS_print: prints out given plds struct to screen
//
// input:
// ======
// + *plds
//    - PLDS_MANAGER struct with the band raw dn file set
//
// output:
// =======
// + prints out to screen
/***************************************************************************/
void PLDS_print(PLDS_MANAGER *plds);

#endif
