# SYNOPSIS
#
#   AC_HDFEOS([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the Hdfeos library. If we find it, then we set the
# Makefile conditional HAVE_HDFEOS. We also set HDFEOS_CFLAGS and
# HDFEOS_LIBS
#
# To allow users to build there own copy of Hdfeos, we also define
# BUILD_HDFEOS.
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

AC_DEFUN([AC_HDFEOS],
[
# Guard against running twice
if test "x$done_hdfeos" = "x"; then
AC_HANDLE_WITH_ARG([hdfeos], [hdfeos], [Hdfeos library], $2, $3, $1)
if test "x$want_hdfeos" = "xyes"; then
        AC_HDF4($1, $2, default_search)
        AC_MSG_CHECKING([for Hdfeos library])
        succeeded=no
        if test "$build_hdfeos" == "yes"; then
            HDFEOS_LIBS="libhdfeos.la"
            HDFEOS_CFLAGS="-I$srcdir/hdfeos/external/HDF-EOS/v2.19/include"
            succeeded=yes
        elif test "$ac_hdfeos_path" != ""; then
            HDFEOS_LIBS="-R$ac_hdfeos_path/lib -L$ac_hdfeos_path/lib -lhdfeos -lGctp"
            HDFEOS_CFLAGS="-I$ac_hdfeos_path/include/hdfeos"
            succeeded=yes
        else
  	    if test "x$CONDA_PREFIX" != x; then
	       if test -e "$CONDA_PREFIX/include/HdfEosDef.h"; then
                  HDFEOS_LIBS="-R$CONDA_PREFIX/lib -L$CONDA_PREFIX/lib -lhdfeos -lGctp"
                  HDFEOS_CFLAGS="-I$CONDA_PREFIX/include"
                  succeeded=yes
	       fi
            fi
            if test "$succeeded" != "yes" ; then
	       AC_SEARCH_LIB([HDFEOS], [hdfeos], [hdfeos/], [HdfEosDef.h], ,
                          [libhdfeos], [-lhdfeos -lGctp])
	    fi		  
            if test "$succeeded" != "yes" ; then
 	       AC_SEARCH_LIB([HDFEOS], [hdfeos], , [HdfEosDef.h], ,
                             [libhdfeos], [-lhdfeos -lGctp])
            fi
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_hdfeos" == "xyes" ; then
            build_hdfeos="yes"
            ac_hdfeos_path="\${prefix}"
            HDFEOS_LIBS="libhdfeos.la"
            HDFEOS_CFLAGS="-I$srcdir/hdfeos/external/HDF-EOS/v2.19/include"
            succeeded=yes
        fi
	if test "$have_hdf4" == "no"; then
            succeeded=no
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                HDFEOS_CFLAGS="$HDFEOS_CFLAGS \$(HDF4_CFLAGS)"
		HDFEOS_LIBS="$HDFEOS_LIBS \$(HDF4_LIBS)"
                AC_MSG_RESULT([yes])
                AC_SUBST(HDFEOS_CFLAGS)
                AC_SUBST(HDFEOS_LIBS)
                have_hdfeos="yes"
        fi
fi
AM_CONDITIONAL([HAVE_HDFEOS], [test "$have_hdfeos" = "yes"])
AM_CONDITIONAL([BUILD_HDFEOS], [test "$build_hdfeos" = "yes"])

AC_CHECK_FOUND([hdfeos], [hdfeos],[Hdfeos library],$1,$2)

done_hdfeos="yes"
fi
])
