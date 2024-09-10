#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <zvproto.h>
#include "carto/ImageUtils.h"
#include "carto/PleiadesManager.h"

/******************************************************************************/
void PLDS_checkPreconditions(PLDS_MANAGER *plds, int band)
{
   if(band < PLDS_BAND1 || band > PLDS_BAND_PAN)
   {
      printf("Band number :%d is outside of range.\n", band+1);
      zabend();
   }

   if(plds->units_set[band] == PLDS_UNIT_NOT_SET)
   {
      printf("Image for band %d not give.\n", band+1);
      zabend();
   }

   if(plds->metaFlags[band] != PLDS_META_ALL_SET)
   {
      printf("Meta data for band %d unavailable.", band+1);
      zabend();
   }
}

/******************************************************************************/
double PLDS_getEarthSunDist(double year, double month, double day, double hh, double mm, double ssdd)
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
int PLDS_readMetaFile(PLDS_MANAGER *plds, char *fname, int isPan)
{
   int index, corrupted, isCenter;
   char line[500];
   FILE *metafile = NULL;

   //   printf("strlen: %d\n", strlen(fname));
   metafile = fopen(fname, "r");
   if(metafile == NULL)
      return PLDS_NO_METAFILE;

   // assume file is corrupted until we see good data at the end
   isCenter = 0;
   corrupted = 1;
   index = -1;
   while(fgets(line, sizeof(line), metafile) != NULL)
   {
      char *ptr;

      ptr = strstr(line, "<BAND_ID>B0</BAND_ID>");
      if(ptr != NULL)
      {
         index = PLDS_BAND1;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>B1</BAND_ID>");
      if(ptr != NULL)
      {
         index = PLDS_BAND2;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>B2</BAND_ID>");
      if(ptr != NULL)
      {
         index = PLDS_BAND3;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>B3</BAND_ID>");
      if(ptr != NULL)
      {
         index = PLDS_BAND4;
         continue;
      }
      ptr = strstr(line, "<BAND_ID>P</BAND_ID>");
      if(ptr != NULL)
      {
         index = PLDS_BAND_PAN;
         continue;
      }
      ptr = strstr(line, "</Band_Radiance>");
      if(ptr != NULL)
      {
         index = -1;
         continue;
      }

      // look for gain
      ptr = strstr(line, "<GAIN>");
      if(ptr != NULL)
      {
         assert(index != -1);
         sscanf(ptr, "<GAIN>%lf</GAIN>", &(plds->gain[index]));
         plds->metaFlags[index] += PLDS_META_GAIN_SET;
         continue;
      }

      // look for bias
      ptr = strstr(line, "<BIAS>");
      if(ptr != NULL)
      {
         assert(index != -1);
         sscanf(ptr, "<BIAS>%lf</BIAS>", &(plds->bias[index]));
         plds->metaFlags[index] += PLDS_META_BIAS_SET;
         continue;
      }

      // look for acquisition date and time
      ptr = strstr(line, "<IMAGING_DATE>");
      if(ptr != NULL)
      {
         if(!isPan)
            sscanf(ptr, "<IMAGING_DATE>%lf-%lf-%lf</IMAGING_DATE>", &(plds->year), &(plds->month), &(plds->day));
         else
            sscanf(ptr, "<IMAGING_DATE>%lf-%lf-%lf</IMAGING_DATE>", &(plds->pan_year), &(plds->pan_month), &(plds->pan_day));
         continue;
      }
      ptr = strstr(line, "<IMAGING_TIME>");
      if(ptr != NULL)
      {
         if(!isPan)
            sscanf(ptr, "<IMAGING_TIME>%lf:%lf:%lfZ</IMAGING_TIME>", &(plds->hh), &(plds->mm), &(plds->ssdd));
         else
            sscanf(ptr, "<IMAGING_TIME>%lf:%lf:%lfZ</IMAGING_TIME>", &(plds->pan_hh), &(plds->pan_mm), &(plds->pan_ssdd));
         continue;
      }

      // look for elevation and set solar zenith angle at image center
      if(strstr(line, "<LOCATION_TYPE>Center</LOCATION_TYPE>") != NULL)
      {
         isCenter = 1;
         continue;
      }
      if(strstr(line, "<LOCATION_TYPE>Bottom Center</LOCATION_TYPE>") != NULL)
      {
         isCenter = 0;
         continue;
      }

      ptr = strstr(line, "<SUN_ELEVATION unit=\"deg\">");
      if(ptr != NULL && isCenter)
      {
         double solarElvDeg;

         sscanf(ptr, "<SUN_ELEVATION unit=\"deg\">%lf</SUN_ELEVATION>", &solarElvDeg);
         if(!isPan)
         {
            plds->solarElevation = solarElvDeg;
            plds->solarZenithAngle = 90. - plds->solarElevation;
            plds->solarZenithAngleInRadians = plds->solarZenithAngle*(M_PI/180.);
         }
         else
         {
            plds->pan_solarElevation = solarElvDeg;
            plds->pan_solarZenithAngle = 90. - plds->pan_solarElevation;
            plds->pan_solarZenithAngleInRadians = plds->pan_solarZenithAngle*(M_PI/180.);
         }
         continue;
      }

      // look for good end of data
      ptr = strstr(line, "</Dimap_Document>");
      if(ptr != NULL) corrupted = 0;
   }

   fclose(metafile);

   // return error if end of good data "END;"  was not found
   if(corrupted) return PLDS_NO_DATA;

   return PLDS_SUCCESS;
}

/******************************************************************************/
int PLDS_fillMetadata(PLDS_MANAGER *plds, char *multiMetaFname, char *panMetaFname)
{
   int status; 

   // do multi-band meta file if specified
   if(strlen(multiMetaFname) > 0)
   {
      status = PLDS_readMetaFile(plds, multiMetaFname, PLDS_IS_NOT_PAN);
      if(status != PLDS_SUCCESS) return status;
      plds->solarDist = PLDS_getEarthSunDist(plds->year, plds->month, plds->day, plds->hh, plds->mm, plds->ssdd);
   }

   // do pan meta file if specified
   if(strlen(panMetaFname) > 0)
   {
      status = PLDS_readMetaFile(plds, panMetaFname, PLDS_IS_PAN);
      if(status != PLDS_SUCCESS) return status;
      plds->pan_solarDist = PLDS_getEarthSunDist(plds->pan_year, plds->pan_month, plds->pan_day, plds->pan_hh, plds->pan_mm, plds->pan_ssdd);
   }


   return PLDS_SUCCESS;
}

/******************************************************************************/
void PLDS_setBuffers(PLDS_MANAGER **plds)
{
   int i;

   // set radiance buffers
   (*plds)->rad_buffs = (double**)malloc(sizeof(double*)*PLEIADES_N_BANDS);
   (*plds)->radLookupTable = (double**)malloc(sizeof(double*)*PLEIADES_N_BANDS);
   for(i = 0; i < PLEIADES_N_BANDS; i++)
   {
      if((*plds)->units_set[i] == PLDS_UNIT_SET && (*plds)->metaFlags[i] == PLDS_META_ALL_SET)
      {
         (*plds)->rad_buffs[i] = (double*)calloc((*plds)->images[i]->ns, sizeof(double));
         (*plds)->radLookupTable[i] = (double*)calloc(pow(2, 16), sizeof(double));
         (*plds)->curr_line_in_rad_buffs[i] = -1;
      }
      else
         (*plds)->rad_buffs[i] = NULL;
   }

   // set reflectance buffers
   (*plds)->ref_buffs = (double**)malloc(sizeof(double*)*(PLEIADES_N_BANDS));
   (*plds)->refLookupTable = (double**)malloc(sizeof(double*)*PLEIADES_N_BANDS);
   for(i = 0; i < PLEIADES_N_BANDS; i++)
   {
      if((*plds)->units_set[i] == PLDS_UNIT_SET && (*plds)->metaFlags[i] == PLDS_META_ALL_SET)
      {
         (*plds)->ref_buffs[i] = (double*)malloc((*plds)->images[i]->ns*sizeof(double));
         (*plds)->refLookupTable[i] = (double*)calloc(pow(2, 16), sizeof(double));
         (*plds)->curr_line_in_ref_buffs[i] = -1;
      }
      else
         (*plds)->ref_buffs[i] = NULL;
   }
}

/******************************************************************************/
PLDS_MANAGER* PLDS_getPLDSManager(VICAR_IMAGE *vi[PLEIADES_N_BANDS], char *multiMetaFname, char *panMetaFname)
{
   int i, status;
   PLDS_MANAGER *plds;

   plds = (PLDS_MANAGER*)malloc(sizeof(PLDS_MANAGER));

   // initialize metaflags and get metadata
   for(i = 0; i < PLEIADES_N_BANDS; i++)
      plds->metaFlags[i] = PLDS_META_NOT_SET;

   status = PLDS_fillMetadata(plds, multiMetaFname, panMetaFname);
   switch(status)
   {
      case PLDS_NO_METAFILE:
         printf("Metafile not found.");
         zabend();
      case PLDS_NO_DATA:
         printf("Error while attempting to attain metafile data.");
         zabend();
   }

   // check for input vicar images and initialize unit_set flags
   for(i = 0; i < PLEIADES_N_BANDS; i++)
   {
      plds->images[i] = vi[i];

      if(vi[i] != NULL) plds->units_set[i] = PLDS_UNIT_SET;
      else plds->units_set[i] = PLDS_UNIT_NOT_SET;
   }

   PLDS_setBuffers(&plds);

   return plds;
}

/******************************************************************************/
void PLDS_deletePLDSManager(PLDS_MANAGER **plds)
{
   int i;

   // free rad buffs and delete VICAR IMAGE structs
   for(i = 0; i < PLEIADES_N_BANDS; i++)
   {
      if((*plds)->units_set[i] == PLDS_UNIT_SET)
      {
         if((*plds)->rad_buffs[i] != NULL)
            free((*plds)->rad_buffs[i]);

         if((*plds)->radLookupTable[i] != NULL)
            free((*plds)->radLookupTable[i]);

         (*plds)->rad_buffs[i] = NULL;
         deleteAndCloseImage(&((*plds)->images[i]));
      }
   }

   // free reflectance buffers
   for(i = 0; i < PLEIADES_N_BANDS; i++)
   {
      if((*plds)->ref_buffs[i] != NULL)
         free((*plds)->ref_buffs[i]);

      if((*plds)->refLookupTable[i] != NULL)
         free((*plds)->refLookupTable[i]);
   }

   free((*plds)->rad_buffs);
   free((*plds)->ref_buffs);
   free((*plds)->radLookupTable);
   free((*plds)->refLookupTable);
   free(*plds);
}

/******************************************************************************/
double PLDS_getTOARadiance(PLDS_MANAGER *plds, int band, double dn)
{
   return (dn/plds->gain[band]) + plds->bias[band];
}

/******************************************************************************/
int PLDS_setTOARadianceLine(PLDS_MANAGER *plds, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(plds->curr_line_in_rad_buffs[band] == line) return PLDS_SUCCESS;

   vi = plds->images[band];
   readVicarImageLine(vi, line);
   for(i = 0; i < vi->ns; i++)
   {
      plds->rad_buffs[band][i] = plds->radLookupTable[band][(int)(vi->buffer[i])];
//      plds->rad_buffs[band][i] = PLDS_getTOARadiance(plds, band, vi->buffer[i]);
   }

   plds->curr_line_in_rad_buffs[band] = line;

   return PLDS_SUCCESS;
}


/******************************************************************************/
void PLDS_initRadLookupTable(PLDS_MANAGER *plds, int band)
{
   int i;

   for(i = 0; i < (int)(pow(2, 16)); i++)
      plds->radLookupTable[band][i] = PLDS_getTOARadiance(plds, band, (double)i);
}

/******************************************************************************/
int PLDS_createTOARadianceImage(PLDS_MANAGER *plds, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   PLDS_checkPreconditions(plds, band);
   inp = plds->images[band];
   status = zvselpi(outInst);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to plds reflectance buffer
   free(out->buffer);
   out->buffer = plds->rad_buffs[band];

   PLDS_initRadLookupTable(plds, band);
   for(i = 0; i < out->nl; i++)
   {
      PLDS_setTOARadianceLine(plds, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return PLDS_SUCCESS;
}

/******************************************************************************/
double PLDS_getTOAReflectance(PLDS_MANAGER *plds, int band, double radiance)
{
   double reflectance;
   
   if(band == PLDS_BAND_PAN)
      reflectance = (radiance*pow(plds->pan_solarDist, 2.0)*M_PI)/(PLDS_ESUN[band]*cos(plds->pan_solarZenithAngleInRadians));
   else
      reflectance = (radiance*pow(plds->solarDist, 2.0)*M_PI)/(PLDS_ESUN[band]*cos(plds->solarZenithAngleInRadians));

   return reflectance;
}

/******************************************************************************/
int PLDS_setTOAReflectanceLine(PLDS_MANAGER *plds, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(plds->curr_line_in_ref_buffs[band] == line) return PLDS_SUCCESS;

   vi = plds->images[band];
   readVicarImageLine(vi, line);
//   PLDS_setTOARadianceLine(plds, band, line);
   for(i = 0; i < vi->ns; i++)
   {
      plds->ref_buffs[band][i] = plds->refLookupTable[band][(int)vi->buffer[i]];
//      plds->ref_buffs[band][i] = PLDS_getTOAReflectance(plds, band, plds->rad_buffs[band][i]);
   }

   plds->curr_line_in_ref_buffs[band] = line;

   return PLDS_SUCCESS;
}

/******************************************************************************/
void PLDS_initRefLookupTable(PLDS_MANAGER *plds, int band)
{
   int i;

   for(i = 0; i < (int)(pow(2, 16)); i++)
   {
      double rad = PLDS_getTOARadiance(plds, band, (double)i);
      plds->refLookupTable[band][i] = PLDS_getTOAReflectance(plds, band, rad);
   }
}

/******************************************************************************/
int PLDS_createTOAReflectanceImage(PLDS_MANAGER *plds, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   PLDS_checkPreconditions(plds, band);
   inp = plds->images[band];
   status = zvselpi(outInst);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to plds reflectance buffer
   free(out->buffer);
   out->buffer = plds->ref_buffs[band];

   PLDS_initRefLookupTable(plds, band);
   for(i = 0; i < out->nl; i++)
   {
      PLDS_setTOAReflectanceLine(plds, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return PLDS_SUCCESS;
}

/******************************************************************************/
void PLDS_print(PLDS_MANAGER *plds)
{
   int i;

   printf("Metadata:\n");
   printf("=========\n");
   printf("datetime: %lf %lf %lf %lf %lf %lf\n", plds->year, plds->month, plds->day, plds->hh, plds->mm, plds->ssdd);
   printf("solar elevation/zenith: %lf %lf\n", plds->solarElevation, plds->solarZenithAngle);
   printf("solar distance: %.9lf\n", plds->solarDist);

   if(plds->metaFlags[PLDS_BAND_PAN] == PLDS_META_ALL_SET)
   {
      printf("pan ssdd: %lf\n", plds->pan_ssdd);
      printf("pan solar elevation/zenith: %lf %lf\n", plds->pan_solarElevation, plds->pan_solarZenithAngle);
      printf("pan solar distance: %.9lf\n", plds->pan_solarDist);
   }

   for(i = 0; i < PLEIADES_N_BANDS; i++)
   {
      printf("-----\n");
      printf("Band: %d metaflag: %d\n", i+1, plds->metaFlags[i]);
      // printf("abscalfactor: %lf\n", plds->absCalFactor[i]);
      // printf("effectiveBandwidth: %lf\n", plds->effectiveBandwidth[i]);
   }

}
