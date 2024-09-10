# SYNOPSIS
#
#   AC_PROJ([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the proj library. If we find them, we set the Makefile
# conditional HAVE_PROJ. We as set PROJ_CFLAGS and PROJ_LIBS
# 
# To allow users to build there own copy of proj, we also define
# BUILD_PROJ

AC_DEFUN([AC_PROJ],
[
# Guard against running twice
if test "x$done_proj" = "x"; then
AC_HANDLE_WITH_ARG([proj], [proj], [proj], $2, $3, $1)
if test "x$want_proj" = "xyes"; then
        AC_MSG_CHECKING([for Proj library])
        succeeded=no
        if test "$ac_proj_path" != ""; then
            PROJ_LIBS="-L$ac_proj_path/lib -lproj"
            PROJ_CFLAGS="-I$ac_proj_path/include"
	    PROJ_PREFIX="$ac_proj_path"
            succeeded=yes
        else
	    AC_SEARCH_LIB([PROJ], [proj], , [proj.h], ,
	                  [libproj], [-lproj])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_proj" == "xyes" ; then
            build_proj="yes"
            ac_proj_path="\${prefix}"
            PROJ_LIBS="-L$ac_proj_path/lib -lproj"
            PROJ_CFLAGS="-I$ac_proj_path/include"
	    PROJ_PREFIX="$ac_proj_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(PROJ_CFLAGS)
                AC_SUBST(PROJ_LIBS)
                AC_SUBST(PROJ_PREFIX)
                AC_DEFINE(HAVE_PROJ,,[Defined if we have Proj])
                have_proj="yes"
        fi
fi
AM_CONDITIONAL([HAVE_PROJ], [test "$have_proj" = "yes"])
AM_CONDITIONAL([BUILD_PROJ], [test "$build_proj" = "yes"])

AC_CHECK_FOUND([proj], [proj],[proj],$1,$2)
done_proj="yes"
fi
])
