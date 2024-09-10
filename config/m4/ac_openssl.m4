# SYNOPSIS
#
#   AC_OPENSSL([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the OPENSSL libraries. If we find them, we set the Makefile
# conditional HAVE_OPENSSL. We as set OPENSSL_CFLAGS and OPENSSL_LIBS
# 
# To allow users to build there own copy of OPENSSL, we also define
# BUILD_OPENSSL.
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


AC_DEFUN([AC_OPENSSL],
[
# Guard against running twice
if test "x$done_openssl" = "x"; then
AC_HANDLE_WITH_ARG([openssl], [openssl], [Openssl], $2, $3, $1)
if test "x$want_openssl" = "xyes"; then
        AC_MSG_CHECKING([for OPENSSL library])
        succeeded=no
        if test "$ac_openssl_path" != ""; then
	    OPENSSL_HOME=$ac_openssl_path
            OPENSSL_LIBS="-L$ac_openssl_path/lib -L$ac_openssl_path/lib64 -lssl -lcrypto"
            OPENSSL_CFLAGS="-I$ac_openssl_path/include"
            succeeded=yes
        else
	    AC_SEARCH_LIB([OPENSSL], [openssl], , [openssl/aes.h], ,
	                  [libssl], [-lssl -lcrypto])
	    OPENSSL_HOME=$OPENSSL_PREFIX
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_openssl" == "xyes" ; then
            build_openssl="yes"
            ac_openssl_path="\${prefix}"
	    OPENSSL_HOME=$ac_openssl_path
            OPENSSL_LIBS="-L$ac_openssl_path/lib -L$ac_openssl_path/lib64 -lssl -lcrypto"
            OPENSSL_CFLAGS="-I$ac_openssl_path/include"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(OPENSSL_CFLAGS)
                AC_SUBST(OPENSSL_LIBS)
                AC_SUBST(OPENSSL_HOME)
                AC_DEFINE(HAVE_OPENSSL,,[Defined if we have OPENSSL])
                have_openssl="yes"
        fi
fi
AM_CONDITIONAL([HAVE_OPENSSL], [test "$have_openssl" = "yes"])
AM_CONDITIONAL([BUILD_OPENSSL], [test "$build_openssl" = "yes"])

AC_CHECK_FOUND([openssl], [openssl],[Openssl],$1,$2)
done_openssl="yes"
fi
])
