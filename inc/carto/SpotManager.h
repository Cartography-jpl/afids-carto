#ifndef SPOTMANAGER
#define SPOTMANAGER

#include <stdio.h>
#include "carto/ImageUtils.h"

#define SPOT_N_BANDS          5
#define SPOT_MSI_BANDS        4

                                // MIN/MAX WAVELENGTH
#define SPOT_BLUE            0   // 0.45/0.52 micron
#define SPOT_GREEN           1   // 0.53/0.59 micron
#define SPOT_RED             2   // 0.62/0.69 micron
#define SPOT_NIR             3   // 0.76/0.89 micron
#define SPOT_PAN             4   // 0.45/0.75 micron

#define SPOT_IS_PAN          1
#define SPOT_IS_NOT_PAN      0

#define SPOT_BAND1     0
#define SPOT_BAND2     1
#define SPOT_BAND3     2
#define SPOT_BAND4     3
#define SPOT_BAND_PAN  4

#define SPOT_UNIT_SET      1
#define SPOT_UNIT_NOT_SET  0
#define SPOT_META_NOT_SET  0
#define SPOT_META_GAIN_SET  1
#define SPOT_META_BIAS_SET 2
#define SPOT_META_ALL_SET  3

#define SPOT_METABUF_SIZE 10000

// SPOT ERROR CODES

#define SPOT_UNINITIALIZED  -999
#define SPOT_NO_DATA          -2
#define SPOT_NO_METAFILE      -1
#define SPOT_SUCCESS           1

/***************************************************************************/
/* band constants solar spectral irradiances bands 1 thru 4 + pan          */
/***************************************************************************/
static const double SPOT_ESUN[SPOT_N_BANDS] =
                       /*BAND1      BAND2        BAND3        BAND4        PANBAND*/
                      {1982.671954, 1826.087443, 1540.494123, 1094.747446, 1706.514896};

typedef struct
{
   VICAR_IMAGE* images[SPOT_N_BANDS];
   unsigned char units_set[SPOT_N_BANDS];
   double **rad_buffs;
   double **ref_buffs;
   double **radLookupTable;
   double **refLookupTable;
   int curr_line_in_rad_buffs[SPOT_N_BANDS];
   int curr_line_in_ref_buffs[SPOT_N_BANDS];

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

   double gain[SPOT_N_BANDS];
   double bias[SPOT_N_BANDS];

   /* flags set if calibration factor and bandwidth are set from metadata */
   /* SPOT_META_NOT_SET  - non are set */
   /* SPOT_META_CAL_SET  - calibration factor was set */
   /* SPOT_META_BWID_SET - bandwidth was set */
   /* SPOT_META_ALL_SET  - both calibration factor and bandwidth were set */
   unsigned char metaFlags[SPOT_N_BANDS];
}SPOT_MANAGER;

/***************************************************************************/
// SPOT_getSPOTManager: returns a SPOT_MANAGER struct
//
// input:
// ======
// + vi
//    - VICAR_IMAGE array
//    - ordered in accordance with SPOT_BAND#
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
// + SPOT_MANAGER struct
/***************************************************************************/
SPOT_MANAGER* SPOT_getSPOTManager(VICAR_IMAGE *vi[SPOT_N_BANDS], char* metafname, char* panMetaFname);

/***************************************************************************/
// SPOT_deleteSPOTManager: deletes a SPOT_MANAGER struct and frees buffers
//                       and closes files
//
// input:
// ======
// + **spot
//    - SPOT_MANAGER struct to free
/***************************************************************************/
void SPOT_deleteSPOTManager(SPOT_MANAGER **spot);

/***************************************************************************/
// SPOT_setTOARadianceLine: calculates the radiance from raw dn image
//
// input:
// ======
// + *spot
//    - SPOT_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate radiance for
// + line
//    - line to calculate radiance for
//
// output:
// =======
// + spot->rad_buffs[band]
//    - contains the radiance data inside the rad_buffs
// + status
//    - SPOT_SUCCESS
/***************************************************************************/
int SPOT_setTOARadianceLine(SPOT_MANAGER *spot, int band, int line);

/***************************************************************************/
// SPOT_createTOARadianceImage: creates a radiance image for specified band
//
// input:
// ======
// + *spot
//    - SPOT_MANAGER struct with the band raw dn file set
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
int SPOT_createTOARadianceImage(SPOT_MANAGER *spot, int outInst, int band);

/***************************************************************************/
// SPOT_setTOAReflectanceLine: calculates the reflectance from raw dn image
//
// input:
// ======
// + *spot
//    - SPOT_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate reflectance for
// + line
//    - line to calculate reflectance for
//
// output:
// =======
// + spot->ref_buffs[band]
//    - contains the reflectance data inside the rad_buffs
// + status
//    - SPOT_SUCCESS
/***************************************************************************/
int SPOT_setTOAReflectanceLine(SPOT_MANAGER *spot, int band, int line);

/***************************************************************************/
// SPOT_createTOAReflectanceImage: creates a reflectance image for specified band
//
// input:
// ======
// + *spot
//    - SPOT_MANAGER struct with the band raw dn file set
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
int SPOT_createTOAReflectanceImage(SPOT_MANAGER *spot, int outInst, int band);

/***************************************************************************/
// SPOT_print: prints out given spot struct to screen
//
// input:
// ======
// + *spot
//    - SPOT_MANAGER struct with the band raw dn file set
//
// output:
// =======
// + prints out to screen
/***************************************************************************/
void SPOT_print(SPOT_MANAGER *spot);

#endif
