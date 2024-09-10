# SYNOPSIS
#
#   AC_AFIDS_PYTHON([required])
#
# DESCRIPTION
#
# This looks for the AFIDS python programs. If we find it, then we set the
# Makefile conditional HAVE_AFIDS_PYTHON. We also set AFIDS_PYTHON_PREFIX to 
# the base directory.
#
# Not finding this library might or might not indicate an error. If you
# want an error to occur if we don't find the library, then specify
# "required". Otherwise, leave it as empty and we'll just silently
# return if we don't find the library.

AC_DEFUN([AC_AFIDS_PYTHON],
[
# Guard against running twice
if test "x$done_afids_python" = "x"; then
AC_HANDLE_WITH_ARG([afids_python], [afids-python], [AFIDS Python], [cannot_build], [default_search], $1)
if test "x$want_afids_python" = "xyes"; then
        AC_MSG_CHECKING([for AFIDS Python programs])
        succeeded=no
        if test "$build_afids_python" == "yes"; then
	    AFIDS_PYTHON_PREFIX="$ac_afids_python_path"
            succeeded=yes
        elif test "$ac_afids_python_path" != ""; then
	    AFIDS_PYTHON_PREFIX="$ac_afids_python_path"
            succeeded=yes
        else
            for ac_afids_python_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /opt/afids ; do
                  if test -e "$ac_afids_python_path_tmp/bin/sba" && test -r "$ac_afids_python_path_tmp/bin/sba"; then
                      AFIDS_PYTHON_PREFIX="$ac_afids_python_path_tmp"
                      succeeded=yes
                      break;
                  fi
            done
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_afids_python" == "xyes" ; then
            build_afids_python="yes"
            ac_afids_python_path="\${prefix}"
	    AFIDS_PYTHON_PREFIX="$ac_afids_python_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(AFIDS_PYTHON_PREFIX)
                have_afids_python="yes"
        fi
fi

AC_CHECK_FOUND([afids_python], [afids-python],[AFIDS Python],$1,[cannot_build])
done_afids_python="yes"
fi
])
