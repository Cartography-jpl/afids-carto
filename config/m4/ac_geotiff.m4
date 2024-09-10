# SYNOPSIS
#
#   AC_GEOTIFF([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the geotiff library. If we find them, we set the Makefile
# conditional HAVE_GEOTIFF. We as set GEOTIFF_CFLAGS and GEOTIFF_LIBS
# 
# To allow users to build there own copy of geotiff, we also define
# BUILD_GEOTIFF
#
# A particular package might not have the library source code, so you
# can supply the "can_build" argument as "can_build". Empty string means we
# can't build this, and various help messages etc. are adjusted for this.
#
# Not finding this library might or might not indicate an error. If you
# want an error to occur if we don't find the library, then specify
# "required". Otherwise, leave it as empty and we'll just silently
# return if we don't find the library.
# 
# If the user doesn't otherwise specify the "with" argument for this
# library, we can either have a default behavior of searching for the
# library on the system or of building our own copy of it. You can
# specify "default_build" if this should build, otherwise we just look
# for this on the system.

AC_DEFUN([AC_GEOTIFF],
[
# Guard against running twice
if test "x$done_geotiff" = "x"; then
AC_HANDLE_WITH_ARG([geotiff], [geotiff], [Geotiff], $2, $3, $1)
if test "x$want_geotiff" = "xyes"; then
        AC_MSG_CHECKING([for Geotiff library])
        succeeded=no
        if test "$ac_geotiff_path" != ""; then
	    GEOTIFF_PREFIX="$ac_geotiff_path"
            GEOTIFF_LIBS="-L$ac_geotiff_path/lib -lgeotiff -ltiff -ljpeg"
            GEOTIFF_CFLAGS="-I$ac_geotiff_path/include"
            succeeded=yes
        else
	    AC_SEARCH_LIB([GEOTIFF], [libgeotiff], , [xtiffio.h], ,
                          [libgeotiff], [-lgeotiff])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_geotiff" == "xyes" ; then
            build_geotiff="yes"
            ac_geotiff_path="\${prefix}"
	    GEOTIFF_PREFIX="$ac_geotiff_path"
            GEOTIFF_LIBS="-L$ac_geotiff_path/lib -lgeotiff -ltiff -ljpeg"
            GEOTIFF_CFLAGS="-I$ac_geotiff_path/include"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(GEOTIFF_CFLAGS)
                AC_SUBST(GEOTIFF_LIBS)
                AC_SUBST(GEOTIFF_PREFIX)
                AC_DEFINE(HAVE_GEOTIFF,,[Defined if we have Geotiff])
                have_geotiff="yes"
        fi
fi
AM_CONDITIONAL([HAVE_GEOTIFF], [test "$have_geotiff" = "yes"])
AM_CONDITIONAL([BUILD_GEOTIFF], [test "$build_geotiff" = "yes"])

AC_CHECK_FOUND([geotiff], [geotiff],[Geotiff],$1,$2)
done_geotiff="yes"
fi
])
