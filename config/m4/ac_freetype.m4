# SYNOPSIS
#
#   AC_FREETYPE([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the FREETYPE libraries. If we find them, we set the Makefile
# conditional HAVE_FREETYPE. We as set FREETYPE_CFLAGS and FREETYPE_LIBS
# 
# To allow users to build there own copy of FREETYPE, we also define
# BUILD_FREETYPE

AC_DEFUN([AC_FREETYPE],
[
# Guard against running twice
if test "x$done_freetype" = "x"; then
AC_HANDLE_WITH_ARG([freetype], [freetype], [Freetype], $2, $3, $1)
if test "x$want_freetype" = "xyes"; then
        AC_MSG_CHECKING([for FREETYPE library])
        succeeded=no
        if test "$ac_freetype_path" != ""; then
            FREETYPE_LIBS="-L$ac_freetype_path/lib -lfreetype"
            FREETYPE_CFLAGS="-I$ac_freetype_path/include"
            FREETYPE_PREFIX="$ac_freetype_path"
            succeeded=yes
        else
	    AC_SEARCH_LIB([FREETYPE], [freetype2], [freetype2/],
	                  [freetype/freetype.h], , [libfreetype], 
			  [-lfreetype])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_freetype" == "xyes" ; then
            build_freetype="yes"
            ac_freetype_path="\${prefix}"
            FREETYPE_LIBS="-L$ac_freetype_path/lib -lfreetype"
            FREETYPE_CFLAGS="-I$ac_freetype_path/include"
            FREETYPE_PREFIX="$ac_freetype_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(FREETYPE_CFLAGS)
                AC_SUBST(FREETYPE_LIBS)
                AC_SUBST(FREETYPE_PREFIX)
                AC_DEFINE(HAVE_FREETYPE,,[Defined if we have FREETYPE])
                have_freetype="yes"
        fi
fi
AM_CONDITIONAL([HAVE_FREETYPE], [test "$have_freetype" = "yes"])
AM_CONDITIONAL([BUILD_FREETYPE], [test "$build_freetype" = "yes"])

AC_CHECK_FOUND([freetype], [freetype],[Freetype],$1,$2)

done_freetype="yes"
fi
])
