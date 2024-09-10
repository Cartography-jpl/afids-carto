# SYNOPSIS
#
#   AC_TIFF([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the tiff library. If we find them, we set the Makefile
# conditional HAVE_TIFF. We as set TIFF_CFLAGS and TIFF_LIBS
# 
# To allow users to build there own copy of tiff, we also define
# BUILD_TIFF
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

AC_DEFUN([AC_TIFF],
[
# Guard against running twice
if test "x$done_tiff" = "x"; then
AC_HANDLE_WITH_ARG([tiff], [tiff], [Tiff], $2, $3, $1)
if test "x$want_tiff" = "xyes"; then
        AC_MSG_CHECKING([for Tiff library])
        succeeded=no
        if test "$ac_tiff_path" != ""; then
	    TIFF_PREFIX="$ac_tiff_path"
            TIFF_LIBS="-L$ac_tiff_path/lib -ltiff"
            TIFF_CFLAGS="-I$ac_tiff_path/include"
            succeeded=yes
        else
	    AC_SEARCH_LIB([TIFF], [libtiff], , [tiff.h], ,
                          [libtiff], [-ltiff])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_tiff" == "xyes" ; then
            build_tiff="yes"
            ac_tiff_path="\${prefix}"
	    TIFF_PREFIX="$ac_tiff_path"
            TIFF_LIBS="-L$ac_tiff_path/lib -ltiff"
            TIFF_CFLAGS="-I$ac_tiff_path/include"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(TIFF_CFLAGS)
                AC_SUBST(TIFF_LIBS)
                AC_SUBST(TIFF_PREFIX)
                AC_DEFINE(HAVE_TIFF,,[Defined if we have Tiff])
                have_tiff="yes"
        fi
fi
AM_CONDITIONAL([HAVE_TIFF], [test "$have_tiff" = "yes"])
AM_CONDITIONAL([BUILD_TIFF], [test "$build_tiff" = "yes"])

AC_CHECK_FOUND([tiff], [tiff],[Tiff],$1,$2)
done_tiff="yes"
fi
])
