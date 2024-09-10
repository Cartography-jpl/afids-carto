# SYNOPSIS
#
#   AC_GNUPLOT([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the gnuplot. If we find it, then we set the
# Makefile conditional HAVE_GNUPLOT. We also set GNUPLOT to the command.
#
# To allow users to build there own copy of Gnuplot, we also define
# BUILD_GNUPLOT.
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

AC_DEFUN([AC_GNUPLOT],
[
# Guard against running twice
if test "x$done_gnuplot" = "x"; then
AC_HANDLE_WITH_ARG([gnuplot], [gnuplot], [Gnuplot], $2, $3, $1)

if test "x$want_gnuplot" = "xyes"; then
        AC_MSG_CHECKING([for Gnuplot])
        succeeded=no
        if test "$build_gnuplot" == "yes"; then
            succeeded=yes
	    GNUPLOT="$ac_gnuplot_path/bin/gnuplot"
        elif test "$ac_gnuplot_path" != ""; then
            succeeded=yes
	    GNUPLOT=$ac_gnuplot_path
        else
	    if test "x$THIRDPARTY" = x ; then
              gnuplot_search_path=$PATH
            else
              gnuplot_search_path=$THIRDPARTY/bin:$PATH
            fi
            AC_PATH_PROG([GNUPLOT], [gnuplot], [], [$gnuplot_search_path])
	    if test -n "$GNUPLOT" ; then
	      GNUPLOT_ABS=`eval echo ${GNUPLOT}`
	      GNUPLOT_PREFIX=`AS_DIRNAME(["$GNUPLOT_ABS"])`
	      GNUPLOT_PREFIX=`AS_DIRNAME(["$GNUPLOT_PREFIX"])`
              ac_version=`LD_LIBRARY_PATH=$GNUPLOT_PREFIX/lib:$GNUPLOT_PREFIX/lib64 $GNUPLOT --version`
              AX_COMPARE_VERSION($ac_version, [ge], [4.4],
              [succeeded=yes],
              [AC_MSG_WARN([
We require version 4.4 of gnuplot or later, found '$ac_version'])
              ])
            fi
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_gnuplot" == "xyes" ; then
            build_gnuplot="yes"
            ac_gnuplot_path="\${prefix}"
	    GNUPLOT="$ac_gnuplot_path/bin/gnuplot"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
		GNUPLOT_ABS=`eval echo ${GNUPLOT}`
		GNUPLOT_PREFIX=`AS_DIRNAME(["$GNUPLOT_ABS"])`
		GNUPLOT_PREFIX=`AS_DIRNAME(["$GNUPLOT_PREFIX"])`
	        # If we are building, then use AFIDSTOP here instead. prefix
		# is fine in the makefile, but GNUPLOT_PREFIX is used in the
		# setup_env file.
 	        if test "$build_gnuplot" = "yes"; then
	            GNUPLOT_PREFIX="\${AFIDSTOP}"
		fi
                AC_SUBST(GNUPLOT)
                AC_SUBST(GNUPLOT_ABS)
                AC_SUBST(GNUPLOT_PREFIX)
		AC_SUBST(build_gnuplot)
                have_gnuplot="yes"
        fi
fi
AM_CONDITIONAL([HAVE_GNUPLOT], [test "$have_gnuplot" = "yes"])
AM_CONDITIONAL([BUILD_GNUPLOT], [test "$build_gnuplot" = "yes"])

AC_CHECK_FOUND([gnuplot], [gnuplot],[Gnuplot],$1,$2)

done_gnuplot="yes"
fi
])
