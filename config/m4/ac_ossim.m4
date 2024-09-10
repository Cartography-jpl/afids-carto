# SYNOPSIS
#
#   AC_OSSIM([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the OSSIM library. If we find it, then we set the
# Makefile conditional HAVE_OSSIM. We also set OSSIM_CFLAGS and
# OSSIM_LIBS
#
# To allow users to build there own copy of OSSIM, we also define
# BUILD_OSSIM.
#
# You should supply the library version to look for (e.g., "1")

AC_DEFUN([AC_OSSIM],
[
# Guard against running twice
if test "x$done_ossim" = "x"; then
AC_HANDLE_WITH_ARG([ossim], [ossim], [OSSIM library], $2, $3, $1)

if test "x$want_ossim" = "xyes"; then
        AC_MSG_CHECKING([for OSSIM library])
        succeeded=no
        if test "$ac_ossim_path" != ""; then
            OSSIM_LIBS="-L$ac_ossim_path/lib -lossim"
            OSSIM_CFLAGS="-I$ac_ossim_path/include/ossim"
            succeeded=yes
        else
	    AC_SEARCH_LIB([OSSIM], [ossim], , [ossim/ossimConfig.h], ,
                          [libossim], [-lossim])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_ossim" == "xyes" ; then
            build_ossim="yes"
            ac_ossim_path="\${prefix}"
            OSSIM_LIBS="-L$ac_ossim_path/lib -lossim"
            OSSIM_CFLAGS="-I$ac_ossim_path/include/ossim"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(OSSIM_CFLAGS)
                AC_SUBST(OSSIM_LIBS)
                have_ossim="yes"
        fi
fi
AM_CONDITIONAL([HAVE_OSSIM], [test "$have_ossim" = "yes"])
AM_CONDITIONAL([BUILD_OSSIM], [test "$build_ossim" = "yes"])

AC_CHECK_FOUND([ossim], [ossim],[OSSIM library],$1,$2)
done_ossim="yes"
fi
])
