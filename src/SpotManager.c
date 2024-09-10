#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <zvproto.h>
#include "carto/ImageUtils.h"
#include "carto/SpotManager.h"

/******************************************************************************/
void SPOT_checkPreconditions(SPOT_MANAGER *spot, int band)
{
   if(band < SPOT_BAND1 || band > SPOT_BAND_PAN)
   {
      printf("Band number :%d is outside of range.\n", band+1);
      zabend();
   }

   if(spot->units_set[band] == SPOT_UNIT_NOT_SET)
   {
      printf("Image for band %d not give.\n", band+1);
      zabend();
   }

   if(spot->metaFlags[band] != SPOT_META_ALL_SET)
   {
      printf("Meta data for band %d unavailable.", band+1);
      zabend();
   }
}

/******************************************************************************/
double SPOT_getEarthSunDist(double year, double month, double day, double hh, double mm, double ssdd)
{
   double ut, JD, D, g, radg, dist;
   int A, B;

   ut = hh + mm/60.0 + ssdd/3600.0;
   if(month == 1.0 || month == 2.0)
   {
      year = year - 1;
      month = month + 12;
   }

   A = (int)(year/100);
   B = 2 - A + (int)(A/4);
   JD = (int)(365.25*(year + 4716)) + (int)(30.6001*(month + 1)) + day + ut/24.0 + B - 1524.5;
   D = JD - 2451545.0;
   g = 357.529 + 0.98560028*D;
   radg = g*(M_PI/180.);
   dist = 1.00014 - 0.01671*cos(radg) - 0.00014*cos(2*radg);

   if(dist < 0.983 || dist > 1.017)
      zmabend("ERROR: earth sun distance is outside of range\n");

   return dist;
}

/******************************************************************************/
int SPOT_readMetaFile(SPOT_MANAGER *spot, char *fname, int isPan)
{
   int index, corrupted, isCenter, readFlag;
   char line[500];
   FILE *metafile = NULL;

   //   printf("strlen: %d\n", strlen(fname));
   metafile = fopen(fname, "r");
   if(metafile == NULL)
      return SPOT_NO_METAFILE;

   // assume file is corrupted until we see good data at the end
   isCenter = 0;
   readFlag = 0;
   corrupted = 1;
   index = -1;
   while(fgets(line, sizeof(line), metafile) != NULL)
   {
      char *ptr;

      ptr = strstr(line, "<BAND_ID>B0</BAND_ID>");
      if(ptr != NULL && readFlag)
      {
         index = SPOT_BAND1;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>B1</BAND_ID>");
      if(ptr != NULL && readFlag)
      {
         index = SPOT_BAND2;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>B2</BAND_ID>");
      if(ptr != NULL && readFlag)
      {
         index = SPOT_BAND3;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>B3</BAND_ID>");
      if(ptr != NULL && readFlag)
      {
         index = SPOT_BAND4;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>P</BAND_ID>");
      if(ptr != NULL && readFlag)
      {
         index = SPOT_BAND_PAN;
         continue;
      }

      ptr = strstr(line, "<Band_Radiance>");
      if(ptr != NULL)
      {
         readFlag = 1;
         continue;
      }

      ptr = strstr(line, "</Band_Radiance>");
      if(ptr != NULL)
      {
         readFlag = 0;
         index = -1;
         continue;
      }

      // look for gain
      ptr = strstr(line, "<GAIN>");
      if(ptr != NULL && readFlag)
      {
         assert(index != -1);
         sscanf(ptr, "<GAIN>%lf</GAIN>", &(spot->gain[index]));
         spot->metaFlags[index] += SPOT_META_GAIN_SET;
         continue;
      }

      // look for bias
      ptr = strstr(line, "<BIAS>");
      if(ptr != NULL && readFlag)
      {
         assert(index != -1);
         sscanf(ptr, "<BIAS>%lf</BIAS>", &(spot->bias[index]));
         spot->metaFlags[index] += SPOT_META_BIAS_SET;
         continue;
      }

      // look for acquisition date and time
      ptr = strstr(line, "<IMAGING_DATE>");
      if(ptr != NULL)
      {
         if(!isPan)
            sscanf(ptr, "<IMAGING_DATE>%lf-%lf-%lf</IMAGING_DATE>", &(spot->year), &(spot->month), &(spot->day));
         else
            sscanf(ptr, "<IMAGING_DATE>%lf-%lf-%lf</IMAGING_DATE>", &(spot->pan_year), &(spot->pan_month), &(spot->pan_day));
         continue;
      }
      ptr = strstr(line, "<IMAGING_TIME>");
      if(ptr != NULL)
      {
         if(!isPan)
            sscanf(ptr, "<IMAGING_TIME>%lf:%lf:%lf</IMAGING_TIME>", &(spot->hh), &(spot->mm), &(spot->ssdd));
         else
            sscanf(ptr, "<IMAGING_TIME>%lf:%lf:%lf</IMAGING_TIME>", &(spot->pan_hh), &(spot->pan_mm), &(spot->pan_ssdd));
         continue;
      }

      // look for elevation and set solar zenith angle at image center
      if(strstr(line, "<LOCATION_TYPE>Center</LOCATION_TYPE>") != NULL)
      {
         isCenter = 1;
         continue;
      }
      if(strstr(line, "<LOCATION_TYPE>CenterRight</LOCATION_TYPE>") != NULL)
      {
         isCenter = 0;
         continue;
      }

      ptr = strstr(line, "<SUN_ELEVATION>");
      if(ptr != NULL && isCenter)
      {
         double solarElvDeg;

         sscanf(ptr, "<SUN_ELEVATION>%lf</SUN_ELEVATION>", &solarElvDeg);
         if(!isPan)
         {
            spot->solarElevation = solarElvDeg;
            spot->solarZenithAngle = 90. - spot->solarElevation;
            spot->solarZenithAngleInRadians = spot->solarZenithAngle*(M_PI/180.);
         }
         else
         {
            spot->pan_solarElevation = solarElvDeg;
            spot->pan_solarZenithAngle = 90. - spot->pan_solarElevation;
            spot->pan_solarZenithAngleInRadians = spot->pan_solarZenithAngle*(M_PI/180.);
         }
         continue;
      }

      // look for good end of data
      ptr = strstr(line, "</Dimap_Document>");
      if(ptr != NULL) corrupted = 0;
   }

   fclose(metafile);

   // return error if end of good data "END;"  was not found
   if(corrupted) return SPOT_NO_DATA;

   return SPOT_SUCCESS;
}

/******************************************************************************/
int SPOT_fillMetadata(SPOT_MANAGER *spot, char *multiMetaFname, char *panMetaFname)
{
   int status; 

   // do multi-band meta file if specified
   if(strlen(multiMetaFname) > 0)
   {
      status = SPOT_readMetaFile(spot, multiMetaFname, SPOT_IS_NOT_PAN);
      if(status != SPOT_SUCCESS) return status;
      spot->solarDist = SPOT_getEarthSunDist(spot->year, spot->month, spot->day, spot->hh, spot->mm, spot->ssdd);
   }

   // do pan meta file if specified
   if(strlen(panMetaFname) > 0)
   {
      status = SPOT_readMetaFile(spot, panMetaFname, SPOT_IS_PAN);
      if(status != SPOT_SUCCESS) return status;
      spot->pan_solarDist = SPOT_getEarthSunDist(spot->pan_year, spot->pan_month, spot->pan_day, spot->pan_hh, spot->pan_mm, spot->pan_ssdd);
   }


   return SPOT_SUCCESS;
}

/******************************************************************************/
void SPOT_setBuffers(SPOT_MANAGER **spot)
{
   int i;

   // set radiance buffers
   (*spot)->rad_buffs = (double**)malloc(sizeof(double*)*SPOT_N_BANDS);
   (*spot)->radLookupTable = (double**)malloc(sizeof(double*)*SPOT_N_BANDS);
   for(i = 0; i < SPOT_N_BANDS; i++)
   {
      if((*spot)->units_set[i] == SPOT_UNIT_SET && (*spot)->metaFlags[i] == SPOT_META_ALL_SET)
      {
         (*spot)->rad_buffs[i] = (double*)calloc((*spot)->images[i]->ns, sizeof(double));
         (*spot)->radLookupTable[i] = (double*)calloc(pow(2, 16), sizeof(double));
         (*spot)->curr_line_in_rad_buffs[i] = -1;
      }
      else
         (*spot)->rad_buffs[i] = NULL;
   }

   // set reflectance buffers
   (*spot)->ref_buffs = (double**)malloc(sizeof(double*)*(SPOT_N_BANDS));
   (*spot)->refLookupTable = (double**)malloc(sizeof(double*)*SPOT_N_BANDS);
   for(i = 0; i < SPOT_N_BANDS; i++)
   {
      if((*spot)->units_set[i] == SPOT_UNIT_SET && (*spot)->metaFlags[i] == SPOT_META_ALL_SET)
      {
         (*spot)->ref_buffs[i] = (double*)calloc((*spot)->images[i]->ns, sizeof(double));
         (*spot)->refLookupTable[i] = (double*)calloc(pow(2, 16), sizeof(double));
         (*spot)->curr_line_in_ref_buffs[i] = -1;
      }
      else
         (*spot)->ref_buffs[i] = NULL;
   }
}

/******************************************************************************/
SPOT_MANAGER* SPOT_getSPOTManager(VICAR_IMAGE *vi[SPOT_N_BANDS], char *multiMetaFname, char *panMetaFname)
{
   int i, status;
   SPOT_MANAGER *spot;

   spot = (SPOT_MANAGER*)malloc(sizeof(SPOT_MANAGER));

   // initialize metaflags and get metadata
   for(i = 0; i < SPOT_N_BANDS; i++)
      spot->metaFlags[i] = SPOT_META_NOT_SET;

   status = SPOT_fillMetadata(spot, multiMetaFname, panMetaFname);
   switch(status)
   {
      case SPOT_NO_METAFILE:
         printf("Metafile not found.");
         zabend();
      case SPOT_NO_DATA:
         printf("Error while attempting to attain metafile data.");
         zabend();
   }

   // check for input vicar images and initialize unit_set flags
   for(i = 0; i < SPOT_N_BANDS; i++)
   {
      spot->images[i] = vi[i];

      if(vi[i] != NULL) spot->units_set[i] = SPOT_UNIT_SET;
      else spot->units_set[i] = SPOT_UNIT_NOT_SET;
   }

   SPOT_setBuffers(&spot);

   return spot;
}

/******************************************************************************/
void SPOT_deleteSPOTManager(SPOT_MANAGER **spot)
{
   int i;

   // free rad buffs and delete VICAR IMAGE structs
   for(i = 0; i < SPOT_N_BANDS; i++)
   {
      if((*spot)->units_set[i] == SPOT_UNIT_SET)
      {
         if((*spot)->rad_buffs[i] != NULL)
            free((*spot)->rad_buffs[i]);

         if((*spot)->radLookupTable[i] != NULL)
            free((*spot)->radLookupTable[i]);

         (*spot)->rad_buffs[i] = NULL;
         deleteAndCloseImage(&((*spot)->images[i]));
      }
   }

   // free reflectance buffers
   for(i = 0; i < SPOT_N_BANDS; i++)
   {
      if((*spot)->ref_buffs[i] != NULL)
         free((*spot)->ref_buffs[i]);

      if((*spot)->refLookupTable[i] != NULL)
         free((*spot)->refLookupTable[i]);
   }

   free((*spot)->rad_buffs);
   free((*spot)->ref_buffs);
   free((*spot)->radLookupTable);
   free((*spot)->refLookupTable);
   free(*spot);
}

/******************************************************************************/
double SPOT_getTOARadiance(SPOT_MANAGER *spot, int band, double dn)
{
   return (dn/spot->gain[band]) + spot->bias[band];
}

/******************************************************************************/
int SPOT_setTOARadianceLine(SPOT_MANAGER *spot, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(spot->curr_line_in_rad_buffs[band] == line) return SPOT_SUCCESS;

   vi = spot->images[band];
   readVicarImageLine(vi, line);
   for(i = 0; i < vi->ns; i++)
   {
      spot->rad_buffs[band][i] = spot->radLookupTable[band][(int)(vi->buffer[i])];
//      spot->rad_buffs[band][i] = SPOT_getTOARadiance(spot, band, vi->buffer[i]);
   }

   spot->curr_line_in_rad_buffs[band] = line;

   return SPOT_SUCCESS;
}


/******************************************************************************/
void SPOT_initRadLookupTable(SPOT_MANAGER *spot, int band)
{
   int i;

   for(i = 0; i < (int)(pow(2, 16)); i++)
      spot->radLookupTable[band][i] = SPOT_getTOARadiance(spot, band, (double)i);
}

/******************************************************************************/
int SPOT_createTOARadianceImage(SPOT_MANAGER *spot, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   SPOT_checkPreconditions(spot, band);
   inp = spot->images[band];
   status = zvselpi(outInst);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to spot reflectance buffer
   free(out->buffer);
   out->buffer = spot->rad_buffs[band];

   SPOT_initRadLookupTable(spot, band);
   for(i = 0; i < out->nl; i++)
   {
      SPOT_setTOARadianceLine(spot, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return SPOT_SUCCESS;
}

/******************************************************************************/
double SPOT_getTOAReflectance(SPOT_MANAGER *spot, int band, double radiance)
{
   double reflectance;
   
   if(band == SPOT_BAND_PAN)
      reflectance = (radiance*pow(spot->pan_solarDist, 2.0)*M_PI)/(SPOT_ESUN[band]*cos(spot->pan_solarZenithAngleInRadians));
   else
      reflectance = (radiance*pow(spot->solarDist, 2.0)*M_PI)/(SPOT_ESUN[band]*cos(spot->solarZenithAngleInRadians));

   return reflectance;
}

/******************************************************************************/
int SPOT_setTOAReflectanceLine(SPOT_MANAGER *spot, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(spot->curr_line_in_ref_buffs[band] == line) return SPOT_SUCCESS;

   vi = spot->images[band];
   readVicarImageLine(vi, line);
//   SPOT_setTOARadianceLine(spot, band, line);
   for(i = 0; i < vi->ns; i++)
   {
      spot->ref_buffs[band][i] = spot->refLookupTable[band][(int)vi->buffer[i]];
//      spot->ref_buffs[band][i] = SPOT_getTOAReflectance(spot, band, spot->rad_buffs[band][i]);
   }

   spot->curr_line_in_ref_buffs[band] = line;

   return SPOT_SUCCESS;
}

/******************************************************************************/
void SPOT_initRefLookupTable(SPOT_MANAGER *spot, int band)
{
   int i;

   for(i = 0; i < (int)(pow(2, 16)); i++)
   {
      double rad = SPOT_getTOARadiance(spot, band, (double)i);
      spot->refLookupTable[band][i] = SPOT_getTOAReflectance(spot, band, rad);
   }
}

/******************************************************************************/
int SPOT_createTOAReflectanceImage(SPOT_MANAGER *spot, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   SPOT_checkPreconditions(spot, band);
   inp = spot->images[band];
   status = zvselpi(outInst);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to spot reflectance buffer
   free(out->buffer);
   out->buffer = spot->ref_buffs[band];

   SPOT_initRefLookupTable(spot, band);
   for(i = 0; i < out->nl; i++)
   {
      SPOT_setTOAReflectanceLine(spot, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return SPOT_SUCCESS;
}

/******************************************************************************/
void SPOT_print(SPOT_MANAGER *spot)
{
   int i;

   printf("Metadata:\n");
   printf("=========\n");
   printf("datetime: %lf %lf %lf %lf %lf %lf\n", spot->year, spot->month, spot->day, spot->hh, spot->mm, spot->ssdd);
   printf("solar elevation/zenith: %lf %lf\n", spot->solarElevation, spot->solarZenithAngle);
   printf("solar distance: %.9lf\n", spot->solarDist);

   printf("pan ssdd: %lf\n", spot->pan_ssdd);
   printf("pan solar elevation/zenith: %lf %lf\n", spot->pan_solarElevation, spot->pan_solarZenithAngle);
   printf("pan solar distance: %.9lf\n", spot->pan_solarDist);

   for(i = 0; i < SPOT_N_BANDS; i++)
   {
      printf("-----\n");
      printf("Band: %d metaflag: %d\n", i+1, spot->metaFlags[i]);
      printf("gain: %lf\n", spot->gain[i]);
      printf("bias: %lf\n", spot->bias[i]);
   }

}
