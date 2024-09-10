# SYNOPSIS
#
#   AC_VICAR([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the VICAR p2 programs. If we find it, then we set the
# Makefile conditional HAVE_VICAR. We also set VICAR_PREFIX to the 
# base directory.
#
# To allow users to build there own copy of VICAR, we also define
# BUILD_VICAR.
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

AC_DEFUN([AC_VICAR],
[
# Guard against running twice
if test "x$done_vicar" = "x"; then
AC_HANDLE_WITH_ARG([vicar], [vicar], [VICAR], $2, $3, $1)

if test "x$want_vicar" = "xyes"; then
        AC_MSG_CHECKING([for VICAR programs])
        succeeded=no
        if test "$build_vicar" == "yes"; then
	    VICAR_PREFIX="$ac_vicar_path"
            succeeded=yes
        elif test "$ac_vicar_path" != ""; then
	    VICAR_PREFIX="$ac_vicar_path"
            succeeded=yes
        else
            for ac_vicar_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /opt/afids_support ; do
                  if test -e "$ac_vicar_path_tmp/vicar/tae/bin/taetm" && test -r "$ac_vicar_path_tmp/vicar/tae/bin/taetm"; then
                      VICAR_PREFIX="$ac_vicar_path_tmp"
                      succeeded=yes
                      break;
                  fi
            done
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_vicar" == "xyes" ; then
            build_vicar="yes"
            ac_vicar_path="\${prefix}"
	    VICAR_PREFIX="$ac_vicar_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
		VICAR_PREFIX_ABS=`eval echo ${VICAR_PREFIX}`
                AC_SUBST(VICAR_PREFIX)
                AC_SUBST(VICAR_PREFIX_ABS)
                have_vicar="yes"
        fi
fi

# Make sure we have the ncurses headers required to build vicar
if test "$build_vicar" = "yes"; then
   AC_CHECK_HEADER([term.h], [],
   [ 
   have_vicar=no
   build_vicar=no
   if test "$1" = "required"; then
      AC_MSG_ERROR([Could not find term.h, which is required to build the required Vicar. This is found in ncurses_devel on Redhat])
   fi
   ])
fi

AM_CONDITIONAL([BUILD_VICAR], [test "$build_vicar" = "yes"])
AM_CONDITIONAL([HAVE_VICAR], [test "$have_vicar" = "yes"])
AC_CHECK_FOUND([vicar], [vicar],[VICAR],$1,$2)
done_vicar="yes"
fi
])
