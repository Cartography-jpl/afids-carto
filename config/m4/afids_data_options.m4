# This are some options to control where the various links for afids data
# is made.

AC_DEFUN([AFIDS_DATA_OPTIONS],
[
#=================================================================
# Directory where various datasets are found. This gets links 
# created when we install

have_massive_dataset="yes"
AC_ARG_WITH([afids-test-data],
AS_HELP_STRING([--with-afids-test-data@<:@=DIR@:>@], [most of the time when you want to change the afids test data dir, you also want to point to the test version of cib01, nevada and srtm. This sets all of these at once.]), 
[ ac_cib01_dir_default="$withval/cib01"
  ac_srtml2_dir_default="$withval/srtmL2_filled"
  ac_mars_kernel_dir_default="$withval/mars_kernel"
  ac_nevada_dir_default="$withval/nevada"
  ac_afids_test_data_dir_default="$withval/test_data"
  have_massive_dataset="no"
], 
[ ac_cib01_dir_default="/raid22/cib01-2009"
  ac_srtml2_dir_default="/raid25/SRTM_2014_update/srtm_v3_dem_L2"
  ac_mars_kernel_dir_default="/raid26/mars_kernel"
  ac_nevada_dir_default="/raid22/nevada"
  ac_afids_test_data_dir_default="/raid3/test_data"
  ac_pommos_planetdem_dir_default="/bigdata/PommDelivery/install/pomm_data/planet_dem"
  ac_pommos_testcases_dir_default="/bigdata/PommDelivery/install/pomm_data/testcases"
] )

AC_ARG_WITH([cib01],
AS_HELP_STRING([--with-cib01@<:@=DIR@:>@], [give directory where CIB01 data can be found]), [ ac_cib01_dir="$withval" ], [ ac_cib01_dir="$ac_cib01_dir_default"] )
AC_SUBST([cib01dir], ["$ac_cib01_dir"])

AC_ARG_WITH([cib05],
AS_HELP_STRING([--with-cib05@<:@=DIR@:>@], [give directory where CIB05 data can be found]), [ ac_cib05_dir="$withval" ], [ ac_cib05_dir="/raid22/cib05-2009"] )
AC_SUBST([cib05dir], ["$ac_cib05_dir"])

AC_ARG_WITH([landsat],
AS_HELP_STRING([--with-landsat@<:@=DIR@:>@], [give directory where LANDSAT data can be found]), [ ac_landsat_dir="$withval" ], [ ac_landsat_dir="/raid22/band3_VICAR"] )
AC_SUBST([landsatdir], ["$ac_landsat_dir"])

AC_ARG_WITH([nevada],
AS_HELP_STRING([--with-nevada@<:@=DIR@:>@], [give directory where Nevada data can be found, used for port gaston DEM and DOQ data]), [ ac_nevada_dir="$withval" ], [ ac_nevada_dir="$ac_nevada_dir_default"] )
AC_SUBST([nevadadir], ["$ac_nevada_dir"])

AC_ARG_WITH([srtm-l2],
AS_HELP_STRING([--with-srtm-l2@<:@=DIR@:>@], [give directory where SRTM L2 data can be found]), [ ac_srtml2_dir="$withval" ], [ ac_srtml2_dir="$ac_srtml2_dir_default"] )
AC_SUBST([srtml2dir], ["$ac_srtml2_dir"])

AC_ARG_WITH([mars-kernels],
AS_HELP_STRING([--with-mars-kernels@<:@=DIR@:>@], [give directory where Mars kernels can be found]), [ ac_mars_kernel_dir="$withval" ], [ ac_mars_kernel_dir="$ac_mars_kernel_dir_default"] )
AC_SUBST([mars_kerneldir], ["$ac_mars_kernel_dir"])

AC_ARG_WITH([srtm-l1],
AS_HELP_STRING([--with-srtm-l1@<:@=DIR@:>@], [give directory where SRTM L1 data can be found]), [ ac_srtml1_dir="$withval" ], [ ac_srtml1_dir="/raid25/SRTM_2014_update/srtm_v3_dem_L1"] )
AC_SUBST([srtml1dir], ["$ac_srtml1_dir"])

AC_ARG_WITH([adrg],
AS_HELP_STRING([--with-adrg@<:@=DIR@:>@], [give directory where ADRG data can be found]), [ ac_adrg_dir="$withval" ], [ ac_adrg_dir="/raid7/adrg_final_maps_gtif/temp_tiles"] )
AC_SUBST([adrgdir], ["$ac_adrg_dir"])

AC_ARG_WITH([vmap1],
AS_HELP_STRING([--with-vmap1@<:@=DIR@:>@], [give directory where VMAP1 data can be found]), [ ac_vmap1_dir="$withval" ], [ ac_vmap1_dir="/raid14/TAHAT/data/tdps_vmap1_ossim"] )
AC_SUBST([vmap1dir], ["$ac_vmap1_dir"])

AC_ARG_WITH([pommos_planetdem],
AS_HELP_STRING([--with-pommos-planetdem@<:@=DIR@:>@], [give directory where planet dem data can be found]), [ ac_pommos_planetdem_dir="$withval" ], [ ac_pommos_planetdem_dir="$ac_pommos_planetdem_dir_default"] )
AC_SUBST([pommos_planetdemdir], ["$ac_pommos_planetdem_dir"])

AC_ARG_WITH([pommos_testcases],
AS_HELP_STRING([--with-pommos-testcases@<:@=DIR@:>@], [give directory where planet dem data can be found]), [ ac_pommos_testcases_dir="$withval" ], [ ac_pommos_testcases_dir="$ac_pommos_testcases_dir_default"] )
AC_SUBST([pommos_testcasesdir], ["$ac_pommos_testcases_dir"])

AC_ARG_WITH([afids-test-data-dir],
AS_HELP_STRING([--with-afids-test-data-dir@<:@=DIR@:>@], 
[give directory where AFIDS test data can be found (default is /raid3/test_data). See also --with-afids-test-data, which might be option you want instead of this one.]), 
[ ac_afids_test_data_dir="$withval" 
  have_massive_dataset="no"
], 
[ ac_afids_test_data_dir="$ac_afids_test_data_dir_default"] )
AC_SUBST([afidstestdatadir], ["$ac_afids_test_data_dir"])
AM_CONDITIONAL([HAVE_MASSIVE_DATASET], [test x$have_massive_dataset = xyes])
])

