#=================================================================
# We pull the version information out into its own file, so it can get include
# seperately in AFIDS.
#=================================================================

AC_DEFUN([VICAR_RTL_VER],[

# *********************************************************************
# If you change the version number here, you should also change this in:
# 1. configure.ac for the vicar_rtl source tree.
# 2. vicar-rtl.spec and vicar-rtl-el5.spec Redhat RPM specs.
# *********************************************************************

# This is the version
vicar_rtl_version=1.10
# This is the closely related library version. You can read the gory
# details about this in the libtool documentation, or 
# https://www.flameeyes.eu/autotools-mythbuster/libtool/version.html. 
# Most of the time this is just the previous verison with "." changed to
# ":", and a ":0" added at the end
vicar_rtl_library_version=1:10:0

AC_SUBST([VICAR_RTL_VERSION], [${vicar_rtl_version}])
AC_SUBST([VICAR_RTL_LIBARY_VERSION], [${vicar_rtl_library_version}])
])
