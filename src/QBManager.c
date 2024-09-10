#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <zvproto.h>
#include "carto/ImageUtils.h"
#include "carto/QBManager.h"

/******************************************************************************/
void QB_checkPreconditions(QB_MANAGER *qb, int band)
{
   if(band < QB_BAND1 || band > QB_BAND_PAN)
   {
      printf("Band number :%d is outside of range.\n", band+1);
      zabend();
   }

   if(qb->units_set[band] == QB_UNIT_NOT_SET)
   {
      printf("Image for band %d not give.\n", band+1);
      zabend();
   }

   if(qb->metaFlags[band] != QB_META_ALL_SET)
   {
      printf("Meta data for band %d unavailable.\n", band+1);
      zabend();
   }
}

/******************************************************************************/
double QB_getEarthSunDist(double year, double month, double day, double hh, double mm, double ssdd)
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
int QB_readMetaFile(QB_MANAGER *qb, char *fname, int isPan)
{
   int index, corrupted;
   char line[QB_METABUF_SIZE];
   FILE *metafile = NULL;

   //   printf("strlen: %d\n", strlen(fname));
   metafile = fopen(fname, "r");
   if(metafile == NULL)
      return QB_NO_METAFILE;

   // assume file is corrupted until we see good data at the end
   corrupted = 1;
   index = -1;
   while(fgets(line, sizeof(line), metafile) != NULL)
   {
      char *ptr;

      ptr = strstr(line, "BEGIN_GROUP = BAND_B");
      if(ptr != NULL) index = QB_BAND1;
      ptr = strstr(line, "BEGIN_GROUP = BAND_G");
      if(ptr != NULL) index = QB_BAND2;
      ptr = strstr(line, "BEGIN_GROUP = BAND_R");
      if(ptr != NULL) index = QB_BAND3;
      ptr = strstr(line, "BEGIN_GROUP = BAND_N");
      if(ptr != NULL) index = QB_BAND4;
      ptr = strstr(line, "BEGIN_GROUP = BAND_P");
      if(ptr != NULL) index = QB_BAND_PAN;
      ptr = strstr(line, "END_GROUP = BAND_");
      if(ptr != NULL) index = -1;
      //      printf("%d %s", index, line);

      // look for absolute calibration factor
      ptr = strstr(line, "absCalFactor = ");
      if(ptr != NULL)
      {
         ptr += strlen("absCalFactor = ");
         qb->absCalFactor[index] = atof(ptr);
         qb->metaFlags[index] += QB_META_CAL_SET;
         continue;
      }

      // look for bandwidth
      ptr = strstr(line, "effectiveBandwidth = ");
      if(ptr != NULL)
      {
         ptr += strlen("effectiveBandwidth = ");
         qb->effectiveBandwidth[index] = atof(ptr);
         qb->metaFlags[index] += QB_META_BWID_SET;
         continue;
      }

      // look for date
      ptr = strstr(line, "firstLineTime = ");
      if(ptr != NULL)
      {
         ptr += strlen("firstLineTime = ");
         if(!isPan)
            sscanf(ptr, "%lf-%lf-%lfT%lf:%lf:%lfZ;", &(qb->year), &(qb->month), &(qb->day), &(qb->hh), &(qb->mm), &(qb->ssdd));
         else
            sscanf(ptr, "%lf-%lf-%lfT%lf:%lf:%lfZ;", &(qb->year), &(qb->month), &(qb->day), &(qb->hh), &(qb->mm), &(qb->pan_ssdd));
         continue;
      }

      // look for elevation and set solar zenith angle
      ptr = strstr(line, "meanSunEl = ");
      if(ptr != NULL)
      {
         ptr += strlen("meanSunEl = ");
         if(!isPan)
         {
            qb->solarElevation = atof(ptr);
            qb->solarZenithAngle = 90. - qb->solarElevation;
            qb->solarZenithAngleInRadians = qb->solarZenithAngle*(M_PI/180.);
         }
         else
         {
            qb->pan_solarElevation = atof(ptr);
            qb->pan_solarZenithAngle = 90. - qb->pan_solarElevation;
            qb->pan_solarZenithAngleInRadians = qb->pan_solarZenithAngle*(M_PI/180.);
         }
         continue;
      }

      // look for good end of data
      ptr = strstr(line, "END;");
      if(ptr != NULL) corrupted = 0;
   }

   fclose(metafile);

   // return error if end of good data "END;"  was not found
   if(corrupted) return QB_NO_DATA;

   return QB_SUCCESS;
}

/******************************************************************************/
int QB_fillMetadata(QB_MANAGER *qb, char *multiMetaFname, char *panMetaFname)
{
   int status; 

   // do multi-band meta file if specified
   if(strlen(multiMetaFname) > 0)
   {
      status = QB_readMetaFile(qb, multiMetaFname, QB_IS_NOT_PAN);
      if(status != QB_SUCCESS) return status;
      qb->solarDist = QB_getEarthSunDist(qb->year, qb->month, qb->day, qb->hh, qb->mm, qb->ssdd);
   }

   // do pan meta file if specified
   if(strlen(panMetaFname) > 0)
   {
      status = QB_readMetaFile(qb, panMetaFname, QB_IS_PAN);
      if(status != QB_SUCCESS) return status;
      qb->pan_solarDist = QB_getEarthSunDist(qb->year, qb->month, qb->day, qb->hh, qb->mm, qb->pan_ssdd);
   }


   return QB_SUCCESS;
}

/******************************************************************************/
void QB_setBuffers(QB_MANAGER **qb)
{
   int i;

   // set radiance buffers
   (*qb)->rad_buffs = (double**)malloc(sizeof(double*)*QB_N_BANDS);
   for(i = 0; i < QB_N_BANDS; i++)
   {
      if((*qb)->units_set[i] == QB_UNIT_SET && (*qb)->metaFlags[i] == QB_META_ALL_SET)
      {
         (*qb)->rad_buffs[i] = (double*)calloc((*qb)->images[i]->ns, sizeof(double));
         (*qb)->curr_line_in_rad_buffs[i] = -1;
      }
      else
         (*qb)->rad_buffs[i] = NULL;
   }

   // set reflectance buffers
   (*qb)->ref_buffs = (double**)malloc(sizeof(double*)*(QB_N_BANDS));
   for(i = 0; i < QB_N_BANDS; i++)
   {
      if((*qb)->units_set[i] == QB_UNIT_SET && (*qb)->metaFlags[i] == QB_META_ALL_SET)
      {
         (*qb)->ref_buffs[i] = (double*)calloc((*qb)->images[i]->ns, sizeof(double));
         (*qb)->curr_line_in_ref_buffs[i] = -1;
      }
      else
         (*qb)->ref_buffs[i] = NULL;
   }
}

/******************************************************************************/
QB_MANAGER* QB_getQBManager(VICAR_IMAGE *vi[QB_N_BANDS], char *multiMetaFname, char *panMetaFname)
{
   int i, status;
   QB_MANAGER *qb;

   qb = (QB_MANAGER*)malloc(sizeof(QB_MANAGER));

   // initialize metaflags and get metadata
   for(i = 0; i < QB_N_BANDS; i++)
      qb->metaFlags[i] = QB_META_NOT_SET;

   // get metadata
   status = QB_fillMetadata(qb, multiMetaFname, panMetaFname);
   switch(status)
   {
      case QB_NO_METAFILE:
         printf("Metafile not found.");
         zabend();
      case QB_NO_DATA:
         printf("Error while attempting to attain metafile data.");
         zabend();
   }

   // check for input vicar images and initialize unit_set flags
   for(i = 0; i < QB_N_BANDS; i++)
   {
      qb->images[i] = vi[i];

      if(vi[i] != NULL) qb->units_set[i] = QB_UNIT_SET;
      else qb->units_set[i] = QB_UNIT_NOT_SET;
   }

   QB_setBuffers(&qb);

   return qb;
}

/******************************************************************************/
void QB_deleteQBManager(QB_MANAGER **qb)
{
   int i;

   // free rad buffs and delete VICAR IMAGE structs
   for(i = 0; i < QB_N_BANDS; i++)
   {
      if((*qb)->units_set[i] == QB_UNIT_SET)
      {
         if((*qb)->rad_buffs[i] != NULL)
            free((*qb)->rad_buffs[i]);
         (*qb)->rad_buffs[i] = NULL;
         deleteAndCloseImage(&((*qb)->images[i]));
      }
   }

   // free reflectance buffers
   for(i = 0; i < QB_N_BANDS; i++)
   {
      if((*qb)->ref_buffs[i] != NULL)
         free((*qb)->ref_buffs[i]);
   }

   free((*qb)->rad_buffs);
   free((*qb)->ref_buffs);
   free(*qb);
}

/******************************************************************************/
double QB_getTOARadiance(QB_MANAGER *qb, int band, double dn)
{
   return ((qb->absCalFactor[band]*dn)/qb->effectiveBandwidth[band]);
}

/******************************************************************************/
int QB_setTOARadianceLine(QB_MANAGER *qb, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(qb->curr_line_in_rad_buffs[band] == line) return QB_SUCCESS;

   vi = qb->images[band];
   readVicarImageLine(vi, line);
   for(i = 0; i < vi->ns; i++)
      qb->rad_buffs[band][i] = QB_getTOARadiance(qb, band, vi->buffer[i]);

   return QB_SUCCESS;
}

/******************************************************************************/
int QB_createTOARadianceImage(QB_MANAGER *qb, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   QB_checkPreconditions(qb, band);
   inp = qb->images[band];
   status = zvselpi(outInst);
   assert(status == 1);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to qb reflectance buffer
   free(out->buffer);
   out->buffer = qb->rad_buffs[band];

   for(i = 0; i < out->nl; i++)
   {
      QB_setTOARadianceLine(qb, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return QB_SUCCESS;
}

/******************************************************************************/
double QB_getTOAReflectance(QB_MANAGER *qb, int band, double radiance)
{
   double reflectance;
   
   if(band == QB_BAND_PAN)
      reflectance = (radiance*pow(qb->pan_solarDist, 2.0)*M_PI)/(QB_ESUN[band]*cos(qb->pan_solarZenithAngleInRadians));
   else
      reflectance = (radiance*pow(qb->solarDist, 2.0)*M_PI)/(QB_ESUN[band]*cos(qb->solarZenithAngleInRadians));

   return reflectance;
}

/******************************************************************************/
int QB_setTOAReflectanceLine(QB_MANAGER *qb, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(qb->curr_line_in_ref_buffs[band] == line) return QB_SUCCESS;

   vi = qb->images[band];
   QB_setTOARadianceLine(qb, band, line);
   for(i = 0; i < vi->ns; i++)
      qb->ref_buffs[band][i] = QB_getTOAReflectance(qb, band, qb->rad_buffs[band][i]);

   return QB_SUCCESS;
}

/******************************************************************************/
int QB_createTOAReflectanceImage(QB_MANAGER *qb, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   QB_checkPreconditions(qb, band);
   inp = qb->images[band];
   status = zvselpi(outInst);
   assert(status == 1);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to qb reflectance buffer
   free(out->buffer);
   out->buffer = qb->ref_buffs[band];

   for(i = 0; i < out->nl; i++)
   {
      QB_setTOAReflectanceLine(qb, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return QB_SUCCESS;
}

/******************************************************************************/
void QB_print(QB_MANAGER *qb)
{
   int i;

   printf("Metadata:\n");
   printf("=========\n");
   printf("datetime: %lf %lf %lf %lf %lf %lf\n", qb->year, qb->month, qb->day, qb->hh, qb->mm, qb->ssdd);
   printf("solar elevation/zenith: %lf %lf\n", qb->solarElevation, qb->solarZenithAngle);
   printf("solar distance: %.9lf\n", qb->solarDist);

   printf("pan ssdd: %lf\n", qb->pan_ssdd);
   printf("pan solar elevation/zenith: %lf %lf\n", qb->pan_solarElevation, qb->pan_solarZenithAngle);
   printf("pan solar distance: %.9lf\n", qb->pan_solarDist);

   for(i = 0; i < QB_N_BANDS; i++)
   {
      printf("-----\n");
      printf("Band: %d metaflag: %d\n", i+1, qb->metaFlags[i]);
      printf("abscalfactor: %lf\n", qb->absCalFactor[i]);
      printf("effectiveBandwidth: %lf\n", qb->effectiveBandwidth[i]);
   }

}
