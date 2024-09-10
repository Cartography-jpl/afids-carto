AC_INIT(carto, 1.05)
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([config/m4])
AFIDS_COMMON
# This set up all the source directories. Source for this is found in
# config/m4/afids_source_directory.m4 so we can include this in multiple
# configuration files.
CARTO_SOURCE_DIRECTORY

AC_CONFIG_FILES([carto/carto.pc])

#=================================================================
# Need to find VICAR RTL library
AC_VICAR_RTL(required, no_build, default_search)

#=================================================================
# Various thirdparty libraries.

AC_GSL(required, cannot_build, default_search)

# Always build Carto
AM_CONDITIONAL([BUILD_CARTO], [true])
AM_CONDITIONAL([BUILD_AFIDS], [false])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

#======================================================================
# Give a nice log message describing what has been configured, and which
# optional libraries are going to be used.
#======================================================================

LOC_MSG()
LOC_MSG([VICAR is now configured])
LOC_MSG()
LOC_MSG([  Installation directory:        ${prefix}])
LOC_MSG([  Build debug version:           ${enable_debug}])
LOC_MSG([  Fortran compiler:              ${F77} ${AM_FFLAGS}])
LOC_MSG([  C compiler:                    ${CC} ${CFLAGS}])
LOC_MSG([  CXX compiler:                  ${CXX} ${CXXFLAGS}])
