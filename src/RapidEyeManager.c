#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <zvproto.h>
#include "carto/ImageUtils.h"
#include "carto/RapidEyeManager.h"

/******************************************************************************/
int RE_checkRapidEyeCommonPreconditions(RAPIDEYE_MANAGER *rem, int band)
{
   if(band < 0 || band > RAPIDEYE_N_BANDS) return RAPIDEYE_INVALID_BAND;
   if(!rem->units_set[band]) return RAPIDEYE_UNIT_NOT_SET_ERR;
   if(rem->gain[band] == RAPIDEYE_GAIN_INIT)
      return RAPIDEYE_GAIN_NOT_SET_ERR;

   return RAPIDEYE_SUCCESS;
}

/******************************************************************************/
double RE_getEarthSunDist(int julDate)
{
   double eccentricity, distance;

   eccentricity = 0.016710219;
   distance = 1/((1 + eccentricity*cos((julDate - 4)*2*M_PI/365.25))/(1-pow(eccentricity, 2)));

   return distance;
}

/******************************************************************************/
int RE_getDayOfYear(int year, int month, int day)
{
   int leap;
   int leapyear[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
   int regyear[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
   int dayOfYear;

   leap = 0;
   if(!(year%400)) leap = 1;
   else if(!(year%100)) leap = 0;
   else if(!(year%4)) leap = 1;

   if(leap) dayOfYear = leapyear[month-1];
   else dayOfYear = regyear[month-1];

   dayOfYear += day;

   return dayOfYear;
}

/******************************************************************************/
int RE_fillMetadata(RAPIDEYE_MANAGER *rem, FILE *metafile)
{
   char *metabuf;
   int year, month, day, i, dayOfYear;
   long long int filesize;

   fseek(metafile, 0, SEEK_END);
   filesize = ftell(metafile);
   metabuf = (char*)malloc(filesize+1);
   if(metafile == NULL) return RAPIDEYE_NO_METAFILE;

   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
      rem->gain[i] = RAPIDEYE_GAIN_INIT;
   rem->elevation = RAPIDEYE_ELEV_INIT;
   year = month = day = 0;

   fseek(metafile, 0, SEEK_SET);
   //while(fread(metabuf, sizeof(char), filesize, metafile) != EOF)
   fread(metabuf, sizeof(char), filesize, metafile);
   {
      char *p, dum[20];

      p = strstr(metabuf, "<hma:acquisitionDate>");
      if(p != NULL)
      {
         p += strlen("<hma:acquisitionDate>");
         strncpy(dum, p, 4);
         sscanf(dum, "%d", &year);
         p += 5;
         strncpy(dum, p, 2);
         dum[2]=0;
         sscanf(dum, "%d", &month);
         p += 3;
         strncpy(dum, p, 2);
         dum[2]=0;
         sscanf(dum, "%d", &day);
      }
      p = strstr(metabuf, "<ohr:illuminationElevationAngle uom=\"deg\">");
      if(p != NULL)
      {
         char *tmp;
         int len;

         p += strlen("<ohr:illuminationElevationAngle uom=\"deg\">");
         tmp = strstr(p, "</ohr:illuminationElevationAngle>");
         len = tmp-p;
         strncpy(dum, p, len);
         dum[len] = 0;
         sscanf(dum, "%lf", &rem->elevation);
      }

      p = metabuf;
      for(i = 0; i < RAPIDEYE_N_BANDS; i++)
      {
         char *tmp;
         int len;

         p = strstr(p, "<re:radiometricScaleFactor>");
         if(p != NULL)
         {
            p += strlen("<re:radiometricScaleFactor>");
            tmp = strstr(p, "</re:radiometricScaleFactor>");
            len = tmp-p;
            strncpy(dum, p, len);
            dum[len] = 0;
            sscanf(dum, "%lf", &rem->gain[i]);
         }
      }
   }

   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
      if(rem->gain[i] == RAPIDEYE_GAIN_INIT) return RAPIDEYE_GAIN_NOT_SET_ERR;

   if(!year && !month && !year) return RAPIDEYE_NO_DATE_DATA;
   if(rem->elevation == RAPIDEYE_ELEV_INIT) return RAPIDEYE_NO_ELEV_DATA;

   dayOfYear = RE_getDayOfYear(year, month, day);
   rem->dist = RE_getEarthSunDist(dayOfYear);

   printf("\nExtracted from metafile.");
   printf("\n========================\n");
   printf("Calendar date: %d %d %d\n", year, month, day);
   printf("Elevation:     %f\n", rem->elevation);
   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
      printf("Gain %d: %lf\n", i+1, rem->gain[i]);
   printf("day of year: %d\n", dayOfYear);
   printf("Earth Sun Dist: %f\n", rem->dist);

   printf("Acquired data from metafile...\n");

   free(metabuf);
   return RAPIDEYE_SUCCESS;
}

/******************************************************************************/
void RE_setBuffers(RAPIDEYE_MANAGER **rem)
{
   int i;

   // set radiance buffers
   (*rem)->rad_buffs = (double**)malloc(sizeof(double*)*RAPIDEYE_N_BANDS);
   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
   {
      if((*rem)->units_set[i] == RAPIDEYE_BAND_SET)
      {
         (*rem)->rad_buffs[i] = (double*)calloc((*rem)->images[i]->ns, sizeof(double));
         (*rem)->curr_line_in_rad_buffs[i] = -1;
      }
      else
         (*rem)->rad_buffs[i] = NULL;
   }

   // set reflectance buffers
   (*rem)->ref_buffs = (double**)malloc(sizeof(double*)*(RAPIDEYE_N_BANDS));
   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
   {
      if((*rem)->units_set[i] == RAPIDEYE_BAND_SET)
      {
         (*rem)->ref_buffs[i] = (double*)calloc((*rem)->images[i]->ns, sizeof(double));
         (*rem)->curr_line_in_ref_buffs[i] = -1;
      }
      else
         (*rem)->ref_buffs[i] = NULL;
   }
}

/******************************************************************************/
RAPIDEYE_MANAGER* RE_getRapidEyeManager(VICAR_IMAGE *vi[RAPIDEYE_N_BANDS], FILE *metafile)
{
   int i, status;
   RAPIDEYE_MANAGER *rem;

   rem = (RAPIDEYE_MANAGER*)malloc(sizeof(RAPIDEYE_MANAGER));

   status = RE_fillMetadata(rem, metafile);
   switch(status)
   {
      case RAPIDEYE_NO_METAFILE:
         printf("Metafile not found.");
         zabend();
      case RAPIDEYE_NO_DATE_DATA:
         printf("Can not find date in metadata file.");
         zabend();
      case RAPIDEYE_GAIN_NOT_SET_ERR:
         printf("Can not find gain information in metadata file.");
         zabend();
      case RAPIDEYE_NO_ELEV_DATA:
         printf("Can not find elevation information in metadata file.");
         zabend();
   }

   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
   {
      rem->images[i] = vi[i];
      if(vi[i] != NULL)
         rem->units_set[i] = RAPIDEYE_BAND_SET;
      else
         rem->units_set[i] = RAPIDEYE_BAND_NOT_SET;
   }

   RE_setBuffers(&rem);

   return rem;
}

/******************************************************************************/
void RE_deleteRapidEyeManager(RAPIDEYE_MANAGER **rem)
{
   int i;

   for(i = 0; i < RAPIDEYE_N_BANDS; i++)
   {
      if((*rem)->units_set[i] == RAPIDEYE_BAND_SET)
      {
         if((*rem)->rad_buffs[i] != NULL)
            free((*rem)->rad_buffs[i]);
         (*rem)->rad_buffs[i] = NULL;
      }
   }
   for(i = 0; i < RAPIDEYE_N_REF_BANDS; i++)
   {
      if((*rem)->units_set[i] == RAPIDEYE_BAND_SET)
      {
         if((*rem)->ref_buffs[i] != NULL)
            free((*rem)->ref_buffs[i]);
         (*rem)->ref_buffs[i] = NULL;
      }
   }

   free((*rem)->rad_buffs);
   free((*rem)->ref_buffs);
   free((*rem));
}

/******************************************************************************/
int RE_getRadianceLine(RAPIDEYE_MANAGER *rem, int band, int line)
{
   int i, err;

   // check gain coefficient is not 0 and band is set
   err = RE_checkRapidEyeCommonPreconditions(rem, band);
   if(err != RAPIDEYE_SUCCESS) return err;
   if(rem->curr_line_in_rad_buffs[band] == line) return RAPIDEYE_SUCCESS;

   readVicarImageLine(rem->images[band], line);
   for(i = 0; i < rem->images[band]->ns; i++)
   {
      double pixel;

      pixel = rem->images[band]->buffer[i];
      if(fabs(pixel) < 10E-10) rem->rad_buffs[band][i] = 0.0;
      else rem->rad_buffs[band][i] = rem->gain[band]*pixel;
   }
   rem->curr_line_in_rad_buffs[band] = line;

   return RAPIDEYE_SUCCESS;
}

/******************************************************************************/
void RE_createRadianceImage(RAPIDEYE_MANAGER *rem, VICAR_IMAGE *vi, int band)
{
   int i;

   for(i = 0; i < vi->nl; i++)
   {
      assert(RE_getRadianceLine(rem, band, i) == RAPIDEYE_SUCCESS);
      memcpy(vi->buffer, rem->rad_buffs[band], vi->ns*sizeof(double));
      writeVicarImageLine(vi, i);
   }
}

/******************************************************************************/
int RE_getReflectanceLine(RAPIDEYE_MANAGER *rem, int band, int line)
{
   int i, err;

   // check gain coefficient is not 0 and band is set
   err = RE_checkRapidEyeCommonPreconditions(rem, band);
   if(err != RAPIDEYE_SUCCESS) return err;
   if(rem->curr_line_in_ref_buffs[band] == line) return RAPIDEYE_SUCCESS;

   RE_getRadianceLine(rem, band, line);
   for(i = 0; i < rem->images[band]->ns; i++)
   {
      double rad;

      rad = rem->rad_buffs[band][i];
      if(fabs(rad) < 10E-10) rem->ref_buffs[band][i] = 0.0;
      else rem->ref_buffs[band][i] = rem->rad_buffs[band][i]*rem->dist*M_PI/
                                    (RAPIDEYE_ESUN[band]*cos((90-rem->elevation)*M_PI/180));
   }
   rem->curr_line_in_ref_buffs[band] = line;

   return RAPIDEYE_SUCCESS;
}

/******************************************************************************/
void RE_createReflectanceImage(RAPIDEYE_MANAGER *rem, VICAR_IMAGE *vi, int band)
{
   int i;

   for(i = 0; i < vi->nl; i++)
   {
      assert(RE_getReflectanceLine(rem, band, i) == RAPIDEYE_SUCCESS);
      memcpy(vi->buffer, rem->ref_buffs[band], vi->ns*sizeof(double));
      writeVicarImageLine(vi, i);
   }
}
