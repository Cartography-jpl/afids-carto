# SYNOPSIS
#
#   AC_EXIV([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the exiv. If we find it, then we set the
# Makefile conditional HAVE_EXIV. We also set EXIV2 to the command.
#
# To allow users to build there own copy of Exiv, we also define
# BUILD_EXIV.
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

AC_DEFUN([AC_EXIV],
[
# Guard against running twice
if test "x$done_exiv" = "x"; then
AC_HANDLE_WITH_ARG([exiv], [exiv2], [Exiv2], $2, $3, $1)
if test "x$want_exiv" = "xyes"; then
        AC_MSG_CHECKING([for Exiv])
        succeeded=no
        if test "$ac_exiv_path" != ""; then
            succeeded=yes
	    EXIV2=$ac_exiv_path/bin/exiv2
        else
	    if test "x$THIRDPARTY" = x ; then
              exiv_search_path=$PATH
            else
              exiv_search_path=$THIRDPARTY/bin:$PATH
            fi
            AC_PATH_PROG([EXIV2], [exiv2], [], [$exiv_search_path])
	    if test -n "$EXIV2" ; then
              succeeded=yes
            fi
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_exiv" == "xyes" ; then
            build_exiv="yes"
            ac_exiv_path="\${prefix}"
	    EXIV2=$ac_exiv_path/bin/exiv2
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(EXIV2)
                have_exiv="yes"
        fi
fi
AM_CONDITIONAL([HAVE_EXIV], [test "$have_exiv" = "yes"])
AM_CONDITIONAL([BUILD_EXIV], [test "$build_exiv" = "yes"])

AC_CHECK_FOUND([exiv2], [exiv2],[Exiv2],$1,$2)
done_exiv="yes"
fi
])
