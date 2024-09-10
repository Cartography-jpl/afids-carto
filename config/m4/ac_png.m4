# SYNOPSIS
#
#   AC_PNG([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the png library. If we find them, we set the Makefile
# conditional HAVE_PNG. We as set PNG_CFLAGS and PNG_LIBS
# 
# To allow users to build there own copy of png, we also define
# BUILD_PNG
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

AC_DEFUN([AC_PNG],
[
# Guard against running twice
if test "x$done_png" = "x"; then
AC_HANDLE_WITH_ARG([png], [png], [PNG], $2, $3, $1)
if test "x$want_png" = "xyes"; then
        AC_MSG_CHECKING([for Png library])
        succeeded=no
        if test "$ac_png_path" != ""; then
	    PNG_PREFIX="$ac_png_path"
            PNG_LIBS="-L$ac_png_path/lib -lpng"
            PNG_CFLAGS="-I$ac_png_path/include"
            succeeded=yes
        else
	    AC_SEARCH_LIB([PNG], [libpng], , [png.h], ,
                          [libpng], [-lpng])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_png" == "xyes" ; then
            build_png="yes"
            ac_png_path="\${prefix}"
	    PNG_PREFIX="$ac_png_path"
            PNG_LIBS="-L$ac_png_path/lib -lpng"
            PNG_CFLAGS="-I$ac_png_path/include"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(PNG_CFLAGS)
                AC_SUBST(PNG_LIBS)
                AC_SUBST(PNG_PREFIX)
                AC_DEFINE(HAVE_PNG,,[Defined if we have Png])
                have_png="yes"
        fi
fi
AM_CONDITIONAL([HAVE_PNG], [test "$have_png" = "yes"])
AM_CONDITIONAL([BUILD_PNG], [test "$build_png" = "yes"])

AC_CHECK_FOUND([png], [png],[PNG],$1,$2)
done_png="yes"
fi
])
