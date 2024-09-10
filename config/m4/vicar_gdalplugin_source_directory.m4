# Stuff for vicar GDAL plugin
# We have the top directory for vicar_rtl as a variable. This allows the
# full AFIDS system to include this in a subdirectory.
AC_DEFUN([VICAR_GDALPLUGIN_SOURCE_DIRECTORY],[
AC_SUBST([srcvicargdal], [${vicar_gdalplugin_topdir}/src])
AC_SUBST([gdalplugindir], [\${prefix}/lib/gdalplugins])
])
