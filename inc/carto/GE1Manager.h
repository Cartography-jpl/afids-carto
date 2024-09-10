#ifndef GE1MANAGER
#define GE1MANAGER

#include <stdio.h>
#include "carto/ImageUtils.h"

#define GE1_N_BANDS          5
#define GE1_MSI_BANDS        4

                                // CENTER WAVELENGTH **needs to be changed**
#define GE1_BLUE            1   // 0.478 micron
#define GE1_GREEN           2   // 0.546 micron
#define GE1_RED             3   // 0.659 micron
#define GE1_NIR             4   // 0.831 micron
#define GE1_PAN             5   // 0.632 micron

#define GE1_BAND1     0
#define GE1_BAND2     1
#define GE1_BAND3     2
#define GE1_BAND4     3
#define GE1_BAND_PAN  4

#define GE1_UNIT_SET        1
#define GE1_UNIT_NOT_SET    0
#define GE1_META_NOT_SET    0
#define GE1_META_GAIN_SET   1
#define GE1_META_OFFSET_SET 2
#define GE1_META_ALL_SET    3

#define GE1_METABUF_SIZE 10000

// GE1 ERROR CODES

#define GE1_UNINITIALIZED  -999
#define GE1_NO_DATA          -2
#define GE1_NO_METAFILE      -1
#define GE1_SUCCESS           1

/***************************************************************************/
/* band constants solar spectral irradiances bands 1 thru 8 + pan          */
/***************************************************************************/
                                                /*BAND1   BAND2   BAND3   BAND4   PANBAND*/
static const double GE1_ESUN[GE1_N_BANDS]      = {196,    185.3,  150.5,  103.9,  161.7};
static const double GE1_BANDWIDTH[GE1_N_BANDS] = {0.0584, 0.0646, 0.0316, 0.1012, 0.3074};

typedef struct
{
   VICAR_IMAGE* images[GE1_N_BANDS];
   unsigned char units_set[GE1_N_BANDS];
   double **rad_buffs;
   double **ref_buffs;
   int curr_line_in_rad_buffs[GE1_N_BANDS];
   int curr_line_in_ref_buffs[GE1_N_BANDS];

   /* Metadata */
   double year;
   double month;
   double day;
   double hh;
   double mm;
   double gain[GE1_N_BANDS];
   double offset[GE1_N_BANDS];

   double ssdd;
   double solarElevation;
   double solarZenithAngle;
   double solarZenithAngleInRadians;
   double solarDist;

   /* flags set if calibration factor and bandwidth are set from metadata */
   /* GE1_META_NOT_SET  - none are set */
   /* GE1_META_GAIN_SET  - scale metadata was set */
   /* GE1_META_OFFSET_SET - offset metadata was set */
   /* GE1_META_ALL_SET  - both calibration factor and bandwidth were set */
   unsigned char metaFlags[GE1_N_BANDS];
}GE1_MANAGER;

/***************************************************************************/
// GE1_getGE1Manager: returns a GE1_MANAGER struct
//
// input:
// ======
// + vi
//    - VICAR_IMAGE array
//    - ordered in accordance with GE1_BAND#
//    - !! uninitialized units must be set to NULL !!
// + metafname
//    - path and filename of .pvl metafile
//
// output:
// =======
// + GE1_MANAGER struct
/***************************************************************************/
GE1_MANAGER* GE1_getGE1Manager(VICAR_IMAGE *vi[GE1_N_BANDS], char* metafname);

/***************************************************************************/
// GE1_deleteGE1Manager: deletes a GE1_MANAGER struct and frees buffers
//                       and closes files
//
// input:
// ======
// + **ge1
//    - GE1_MANAGER struct to free
/***************************************************************************/
void GE1_deleteGE1Manager(GE1_MANAGER **ge1);

/***************************************************************************/
// GE1_setTOARadianceLine: calculates the radiance from raw dn image
//
// input:
// ======
// + *ge1
//    - GE1_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate radiance for
// + line
//    - line to calculate radiance for
//
// output:
// =======
// + ge1m->rad_buffs[band]
//    - contains the radiance data inside the rad_buffs
// + status
//    - GE1_SUCCESS
/***************************************************************************/
int GE1_setTOARadianceLine(GE1_MANAGER *ge1, int band, int line);

/***************************************************************************/
// GE1_createTOARadianceImage: creates a radiance image for specified band
//
// input:
// ======
// + *ge1
//    - GE1_MANAGER struct with the band raw dn file set
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
int GE1_createTOARadianceImage(GE1_MANAGER *ge1, int outInst, int band);

/***************************************************************************/
// GE1_setTOAReflectanceLine: calculates the reflectance from raw dn image
//
// input:
// ======
// + *ge1
//    - GE1_MANAGER struct with the band raw dn file set
// + band
//    - band to calculate reflectance for
// + line
//    - line to calculate reflectance for
//
// output:
// =======
// + ge1m->ref_buffs[band]
//    - contains the reflectance data inside the rad_buffs
// + status
//    - GE1_SUCCESS
/***************************************************************************/
int GE1_setTOAReflectanceLine(GE1_MANAGER *ge1, int band, int line);

/***************************************************************************/
// GE1_createTOAReflectanceImage: creates a reflectance image for specified band
//
// input:
// ======
// + *ge1
//    - GE1_MANAGER struct with the band raw dn file set
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
int GE1_createTOAReflectanceImage(GE1_MANAGER *ge1, int outInst, int band);

/***************************************************************************/
// GE1_print: prints out given ge1 struct to screen
//
// input:
// ======
// + *ge1
//    - GE1_MANAGER struct with the band raw dn file set
//
// output:
// =======
// + prints out to screen
/***************************************************************************/
void GE1_print(GE1_MANAGER *ge1);

#endif
