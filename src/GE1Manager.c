#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <zvproto.h>
#include "carto/ImageUtils.h"
#include "carto/GE1Manager.h"

/******************************************************************************/
void GE1_checkPreconditions(GE1_MANAGER *ge1, int band)
{
   if(band < GE1_BAND1 || band > GE1_BAND_PAN)
   {
      printf("Band number :%d is outside of range.\n", band+1);
      zabend();
   }

   if(ge1->units_set[band] == GE1_UNIT_NOT_SET)
   {
      printf("Image for band %d not give.\n", band+1);
      zabend();
   }

   if(ge1->metaFlags[band] != GE1_META_ALL_SET)
   {
      printf("Meta data for band %d unavailable - flag %d.\n", band+1, ge1->metaFlags[band]);
      zabend();
   }
}

/******************************************************************************/
double GE1_getEarthSunDist(double year, double month, double day, double hh, double mm, double ssdd)
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
int GE1_readMetaFile(GE1_MANAGER *ge1, char *fname)
{
   int index, corrupted;
   char line[500];
   FILE *metafile = NULL;

   //   printf("strlen: %d\n", strlen(fname));
   metafile = fopen(fname, "r");
   if(metafile == NULL)
      return GE1_NO_METAFILE;

   // assume file is corrupted until we see good data at the end
   corrupted = 1;
   index = -1;
   while(fgets(line, sizeof(line), metafile) != NULL)
   {
      char *ptr;

      ptr = strstr(line, "bandNumber = 1;");
      if(ptr != NULL) index = GE1_BAND1;
      ptr = strstr(line, "bandNumber = 2;");
      if(ptr != NULL) index = GE1_BAND2;
      ptr = strstr(line, "bandNumber = 3;");
      if(ptr != NULL) index = GE1_BAND3;
      ptr = strstr(line, "bandNumber = 4;");
      if(ptr != NULL) index = GE1_BAND4;
      ptr = strstr(line, "bandNumber = 5;");
      if(ptr != NULL) index = GE1_BAND_PAN;
      ptr = strstr(line, "END_GROUP = bandSpecificInformation;");
      if(ptr != NULL) index = -1;
      // printf("%d %s", index, line);

      // look for radiometric gain
      ptr = strstr(line, "gain = ");
      if(ptr != NULL)
      {
         ptr += strlen("gain = ");
         ge1->gain[index] = atof(ptr);
         ge1->metaFlags[index] += GE1_META_GAIN_SET;
         continue;
      }

      // look for radiometric offset
      ptr = strstr(line, "offset = ");
      if(ptr != NULL)
      {
         ptr += strlen("offset = ");
         ge1->offset[index] = atof(ptr);
         ge1->metaFlags[index] += GE1_META_OFFSET_SET;
         continue;
      }

      // look for date
      ptr = strstr(line, "firstLineAcquisitionDateTime = ");
      if(ptr != NULL)
      {
         ptr += strlen("firstLineAcquisitionDateTime = ");
         sscanf(ptr, "%lf-%lf-%lfT%lf:%lf:%lfZ;", &(ge1->year), &(ge1->month), &(ge1->day), &(ge1->hh), &(ge1->mm), &(ge1->ssdd));
         continue;
      }

      // look for elevation and set solar zenith angle
      ptr = strstr(line, "firstLineElevationAngle = ");
      if(ptr != NULL)
      {
         ptr += strlen("firstLineElevationAngle = ");
         ge1->solarElevation = atof(ptr);
         ge1->solarZenithAngle = 90. - ge1->solarElevation;
         ge1->solarZenithAngleInRadians = ge1->solarZenithAngle*(M_PI/180.);

         continue;
      }

      // look for good end of data
      ptr = strstr(line, "END;");
      if(ptr != NULL) corrupted = 0;
   }

   fclose(metafile);

   // return error if end of good data "END;"  was not found
   if(corrupted) return GE1_NO_DATA;

   return GE1_SUCCESS;
}

/******************************************************************************/
int GE1_fillMetadata(GE1_MANAGER *ge1, char *metaFname)
{
   int status; 


   status = GE1_readMetaFile(ge1, metaFname);
   if(status != GE1_SUCCESS) return status;
   ge1->solarDist = GE1_getEarthSunDist(ge1->year, ge1->month, ge1->day, ge1->hh, ge1->mm, ge1->ssdd);

   return GE1_SUCCESS;
}

/******************************************************************************/
void GE1_setBuffers(GE1_MANAGER **ge1)
{
   int i;

   // set radiance buffers
   (*ge1)->rad_buffs = (double**)malloc(sizeof(double*)*GE1_N_BANDS);
   for(i = 0; i < GE1_N_BANDS; i++)
   {
      if((*ge1)->units_set[i] == GE1_UNIT_SET && (*ge1)->metaFlags[i] == GE1_META_ALL_SET)
      {
         (*ge1)->rad_buffs[i] = (double*)calloc((*ge1)->images[i]->ns, sizeof(double));
         (*ge1)->curr_line_in_rad_buffs[i] = -1;
      }
      else
         (*ge1)->rad_buffs[i] = NULL;
   }

   // set reflectance buffers
   (*ge1)->ref_buffs = (double**)malloc(sizeof(double*)*(GE1_N_BANDS));
   for(i = 0; i < GE1_N_BANDS; i++)
   {
      if((*ge1)->units_set[i] == GE1_UNIT_SET && (*ge1)->metaFlags[i] == GE1_META_ALL_SET)
      {
         (*ge1)->ref_buffs[i] = (double*)calloc((*ge1)->images[i]->ns, sizeof(double));
         (*ge1)->curr_line_in_ref_buffs[i] = -1;
      }
      else
         (*ge1)->ref_buffs[i] = NULL;
   }
}

/******************************************************************************/
GE1_MANAGER* GE1_getGE1Manager(VICAR_IMAGE *vi[GE1_N_BANDS], char *metaFname)
{
   int i, status;
   GE1_MANAGER *ge1;

   ge1 = (GE1_MANAGER*)malloc(sizeof(GE1_MANAGER));

   // initialize metaflags and get metadata
   for(i = 0; i < GE1_N_BANDS; i++)
      ge1->metaFlags[i] = GE1_META_NOT_SET;

   status = GE1_fillMetadata(ge1, metaFname);
   switch(status)
   {
      case GE1_NO_METAFILE:
         printf("Metafile not found.");
         zabend();
      case GE1_NO_DATA:
         printf("Error while attempting to attain metafile data.");
         zabend();
   }

   // check for input vicar images and initialize unit_set flags
   for(i = 0; i < GE1_N_BANDS; i++)
   {
      ge1->images[i] = vi[i];

      if(vi[i] != NULL) ge1->units_set[i] = GE1_UNIT_SET;
      else ge1->units_set[i] = GE1_UNIT_NOT_SET;
   }

   GE1_setBuffers(&ge1);

   return ge1;
}

/******************************************************************************/
void GE1_deleteGE1Manager(GE1_MANAGER **ge1)
{
   int i;

   // free rad buffs and delete VICAR IMAGE structs
   for(i = 0; i < GE1_N_BANDS; i++)
   {
      if((*ge1)->units_set[i] == GE1_UNIT_SET)
      {
         if((*ge1)->rad_buffs[i] != NULL)
            free((*ge1)->rad_buffs[i]);
         (*ge1)->rad_buffs[i] = NULL;
         deleteAndCloseImage(&((*ge1)->images[i]));
      }
   }

   // free reflectance buffers
   for(i = 0; i < GE1_N_BANDS; i++)
   {
      if((*ge1)->ref_buffs[i] != NULL)
         free((*ge1)->ref_buffs[i]);
   }

   free((*ge1)->rad_buffs);
   free((*ge1)->ref_buffs);
   free(*ge1);
}

/******************************************************************************/
double GE1_getTOARadiance(GE1_MANAGER *ge1, int band, double dn)
{
   return ge1->gain[band]*dn + ge1->offset[band];
}

/******************************************************************************/
int GE1_setTOARadianceLine(GE1_MANAGER *ge1, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(ge1->curr_line_in_rad_buffs[band] == line) return GE1_SUCCESS;

   vi = ge1->images[band];
   readVicarImageLine(vi, line);
   for(i = 0; i < vi->ns; i++)
      ge1->rad_buffs[band][i] = GE1_getTOARadiance(ge1, band, vi->buffer[i]);

   ge1->curr_line_in_rad_buffs[band] = line;

   return GE1_SUCCESS;
}

/******************************************************************************/
int GE1_createTOARadianceImage(GE1_MANAGER *ge1, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   GE1_checkPreconditions(ge1, band);
   inp = ge1->images[band];
   status = zvselpi(outInst);
   assert(status == 1);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to ge1 reflectance buffer
   free(out->buffer);
   out->buffer = ge1->rad_buffs[band];

   for(i = 0; i < out->nl; i++)
   {
      GE1_setTOARadianceLine(ge1, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return GE1_SUCCESS;
}

/******************************************************************************/
double GE1_getTOAReflectance(GE1_MANAGER *ge1, int band, double radiance)
{
   return (radiance*pow(ge1->solarDist, 2.0)*M_PI)/(GE1_ESUN[band]*cos(ge1->solarZenithAngleInRadians));
}

/******************************************************************************/
int GE1_setTOAReflectanceLine(GE1_MANAGER *ge1, int band, int line)
{
   int i;
   VICAR_IMAGE *vi;

   if(ge1->curr_line_in_ref_buffs[band] == line) return GE1_SUCCESS;

   vi = ge1->images[band];
   GE1_setTOARadianceLine(ge1, band, line);
   for(i = 0; i < vi->ns; i++)
      ge1->ref_buffs[band][i] = GE1_getTOAReflectance(ge1, band, ge1->rad_buffs[band][i]);

   ge1->curr_line_in_ref_buffs[band] = line;

   return GE1_SUCCESS;
}

/******************************************************************************/
int GE1_createTOAReflectanceImage(GE1_MANAGER *ge1, int outInst, int band)
{
   int i, status;
   VICAR_IMAGE *inp;
   VICAR_IMAGE *out;

   GE1_checkPreconditions(ge1, band);
   inp = ge1->images[band];
   status = zvselpi(outInst);
   assert(status == 1);
   out = getVI_out("REAL", outInst, inp->nl, inp->ns);

   // free the default buffer in output VICAR_IMAGE and point it to ge1 reflectance buffer
   free(out->buffer);
   out->buffer = ge1->ref_buffs[band];

   for(i = 0; i < out->nl; i++)
   {
      GE1_setTOAReflectanceLine(ge1, band, i);
      writeVicarImageLine(out, i);
   }

   deleteAndCloseImage(&out);

   return GE1_SUCCESS;
}

/******************************************************************************/
void GE1_print(GE1_MANAGER *ge1)
{
   int i;

   printf("Metadata:\n");
   printf("=========\n");
   printf("datetime: %lf %lf %lf %lf %lf %lf\n", ge1->year, ge1->month, ge1->day, ge1->hh, ge1->mm, ge1->ssdd);
   printf("solar elevation/zenith: %lf %lf\n", ge1->solarElevation, ge1->solarZenithAngle);
   printf("solar distance: %.9lf\n", ge1->solarDist);

   for(i = 0; i < GE1_N_BANDS; i++)
   {
      printf("-----\n");
      printf("Band: %d metaflag: %d\n", i+1, ge1->metaFlags[i]);
      printf("gain: %lf\n", ge1->gain[i]);
      printf("offset: %lf\n", ge1->offset[i]);
   }

}
