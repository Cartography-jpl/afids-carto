# SYNOPSIS
#
#   AC_JPEG([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the JPEG libraries. If we find them, we set the Makefile
# conditional HAVE_JPEG. 
# 
# To allow users to build there own copy of JPEG, we also define
# BUILD_JPEG
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


AC_DEFUN([AC_JPEG],
[
# Guard against running twice
if test "x$done_jpeg" = "x"; then
AC_HANDLE_WITH_ARG([jpeg], [jpeg], [JPEG library], $2, $3, $1)
if test "x$want_jpeg" = "xyes"; then
        AC_MSG_CHECKING([for JPEG library])
        succeeded=no
        if test "$ac_jpeg_path" != ""; then
            succeeded=yes
	    JPEG_PREFIX="$ac_jpeg_path"
        else
            for ac_jpeg_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /opt/afids_support /usr /usr/local /opt /opt/local /sw ; do
                  if test -e "$ac_jpeg_path_tmp/include/jpeglib.h" && test -r "$ac_jpeg_path_tmp/include/jpeglib.h"; then
                    succeeded=yes
	            JPEG_PREFIX="$ac_jpeg_path_tmp"
                    break;
                  fi
            done
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_jpeg" == "xyes" ; then
            build_jpeg="yes"
            ac_jpeg_path="\${prefix}"
	    JPEG_PREFIX="$ac_jpeg_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(JPEG_PREFIX)
                AC_DEFINE(HAVE_JPEG,,[Defined if we have JPEG])
                have_jpeg="yes"
        fi
fi
AM_CONDITIONAL([HAVE_JPEG], [test "$have_jpeg" = "yes"])
AM_CONDITIONAL([BUILD_JPEG], [test "$build_jpeg" = "yes"])

AC_CHECK_FOUND([jpeg], [jpeg],[JPEG library],$1,$2)

done_jpeg="yes"
fi
])
