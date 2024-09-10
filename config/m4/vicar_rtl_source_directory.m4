#=================================================================
# A large number of source directories. We collect all this here, so we
# have one place to edit this. 
#
#=================================================================

# Stuff in VICAR_RTL library. 

# We have the top directory for vicar_rtl as a variable. This allows the
# full AFIDS system to include this in a subdirectory.
AC_DEFUN([VICAR_RTL_SOURCE_DIRECTORY],[
# Get version information
VICAR_RTL_VER
AC_SUBST([vicarrtltop], [${vicar_rtl_topdir}])
AC_SUBST([srcmath77], [${vicar_rtl_topdir}/external/math77/v5.0/fortran])
AC_SUBST([shvicinc], [${vicar_rtl_topdir}/shell_vicar/inc])
AC_SUBST([srcshvic], [${vicar_rtl_topdir}/shell_vicar/src])
AC_SUBST([rtlinc], [${vicar_rtl_topdir}/rtl/inc])
AC_SUBST([srcrtl], [${vicar_rtl_topdir}/rtl/src])
AC_SUBST([taeinc], [${vicar_rtl_topdir}/tae/inc])
AC_SUBST([srclibtae], [${vicar_rtl_topdir}/tae/src])
AC_SUBST([p1inc], [${vicar_rtl_topdir}/p1/inc])
AC_SUBST([srcp1], [${vicar_rtl_topdir}/p1/src])
AC_SUBST([p2inc], [${vicar_rtl_topdir}/p2/inc])
AC_SUBST([srcp2], [${vicar_rtl_topdir}/p2/src])
AC_SUBST([srcstae], [${vicar_rtl_topdir}/stae/src])
AC_SUBST([staeinc], [${vicar_rtl_topdir}/stae/inc])
AC_SUBST([vicarrtlincdir], [${includedir}/vicar_rtl])
])
