# SYNOPSIS
#
#   AC_AFIDS_XVD([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the AFIDS XVD programs. If we find it, then we set the
# Makefile conditional HAVE_AFIDS_XVD. We also set AFIDS_XVD_PREFIX to the 
# base directory.
#
# To allow users to build there own copy of AFIDS_XVD, we also define
# BUILD_AFIDS_XVD.
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
#
# If we have something missing from building xvd, we set 
# afids_xvd_build_error to yes and afids_xvd_build_error_message to
# the text that should be displayed. This can be used to formulate a nice
# error message at the end of configure (see configure.ac in base directory
# for example.
AC_DEFUN([AC_AFIDS_XVD],
[
# Guard against running twice
if test "x$done_afids_xvd" = "x"; then
AC_HANDLE_WITH_ARG([afids_xvd], [afids-xvd], [AFIDS XVD], $2, $3, $1)
afids_xvd_build_error="no"
if test "x$want_afids_xvd" = "xyes"; then
        AC_MSG_CHECKING([for AFIDS_XVD programs])
        succeeded=no
        if test "$build_afids_xvd" == "yes"; then
	    AFIDS_XVD_PREFIX="$ac_afids_xvd_path"
            succeeded=yes
        elif test "$ac_afids_xvd_path" != ""; then
	    AFIDS_XVD_PREFIX="$ac_afids_xvd_path"
            succeeded=yes
        else
            for ac_afids_xvd_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /opt/afids ; do
                  if test -e "$ac_afids_xvd_path_tmp/bin/xvd" && test -r "$ac_afids_xvd_path_tmp/bin/xvd"; then
                      AFIDS_XVD_PREFIX="$ac_afids_xvd_path_tmp"
                      succeeded=yes
                      break;
                  fi
            done
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_afids_xvd" == "xyes" ; then
            build_afids_xvd="yes"
            ac_afids_xvd_path="\${prefix}"
	    AFIDS_XVD_PREFIX="$ac_afids_xvd_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
		AFIDS_XVD_PREFIX_ABS=`eval echo ${AFIDS_XVD_PREFIX}`
                AC_SUBST(AFIDS_XVD_PREFIX)
                AC_SUBST(AFIDS_XVD_PREFIX_ABS)
                have_afids_xvd="yes"
        fi
fi
if test "$build_afids_xvd" = "yes"; then
  if test "$with_motif" = "no"; then
    build_afids_xvd="no"
    have_afids_xvd="no"
    afids_xvd_build_error="yes"
    afids_xvd_build_error_message="
============================================================================
=== Motif wasn't found on the system or was disabled, so not building xvd ==
============================================================================
"
  else
    save_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $include_motif"
    AC_CHECK_HEADERS([X11/Xlib.h Xm/Xm.h X11/xpm.h], [], 
    [
    build_afids_xvd="no"
    have_afids_xvd="no"
    afids_xvd_build_error="yes"
    afids_xvd_build_error_message="
=============================================================================
=== One of the required X11 headers wasn't found on the system, so not     ==
=== building xvd. We require X11/Xlib.h, Xm/Xm.h, X11/xpm.h (libX11-devel, ==
=== openmotif-devel, libXpm-devel on RedHat).                              ==
=============================================================================
"
    break
    ])
    CPPFLAGS="$save_CPPFLAGS"
  fi
fi
AM_CONDITIONAL([BUILD_AFIDS_XVD], [test "$build_afids_xvd" = "yes"])
AM_CONDITIONAL([HAVE_AFIDS_XVD], [test "$have_afids_xvd" = "yes"])

AC_CHECK_FOUND([afids_xvd], [afids-xvd],[AFIDS XVD],$1,$2)
done_afids_xvd="yes"
fi
])
