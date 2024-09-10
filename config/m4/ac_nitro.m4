# SYNOPSIS
#
#   AC_NITRO([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the Nitro library. If we find them, we set the Makefile
# conditional HAVE_NITRO. We as set NITRO_CFLAGS and NITRO_LIBS
# 
# To allow users to build there own copy of Nitro, we also define
# BUILD_NITRO
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

AC_DEFUN([AC_NITRO],
[
# Guard against running twice
if test "x$done_nitro" = "x"; then
AC_HANDLE_WITH_ARG([nitro], [nitro], [Nitro], $2, $3, $1)

if test "x$want_nitro" = "xyes"; then
        AC_MSG_CHECKING([for Nitro library])
        succeeded=no
        if test "$ac_nitro_path" != ""; then
            NITRO_LIBS="-R$ac_nitro_path/lib -L$ac_nitro_path/lib -lnitf-cpp -lnitf-c"
            NITRO_CFLAGS="-I$ac_nitro_path/include/nitro"
            succeeded=yes
        else
	    AC_SEARCH_LIB([NITRO], [nitro], [nitro/import/], [nitf.h], ,
                          [libnitf-c], [-lnitf-cpp -lnitf-c])
	    NITRO_CFLAGS="-I$NITRO_PREFIX/include/nitro"
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_nitro" == "xyes" ; then
            build_nitro="yes"
            ac_nitro_path="\${prefix}"
            NITRO_LIBS="-R$ac_nitro_path/lib -L$ac_nitro_path/lib -lnitf-cpp -lnitf-c"
            NITRO_CFLAGS="-I$ac_nitro_path/include/nitro"
            succeeded=yes
        fi
        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
# Need to get libdl stuff shortly
       	        LIBS_SAVE="$LIBS"
		LIBS=""
		AC_SEARCH_LIBS([dlopen], [dl dld], [], [
		  AC_MSG_ERROR([unable to find the dlopen() function])
		  ])
		NITRO_DLLIB="$LIBS"
	        NITRO_LIBS="$NITRO_LIBS \$(PTHREAD_LIBS) $LIBS"
		LIBS="$LIBS_SAVE"
		NITRO_CFLAGS="$NITRO_CFLAGS -D_REENTRANT -D__POSIX \$(PTHREAD_CFLAGS)"
                AC_MSG_RESULT([yes])
                AC_SUBST(NITRO_CFLAGS)
                AC_SUBST(NITRO_DLLIB)
                AC_SUBST(NITRO_LIBS)
                AC_DEFINE(HAVE_NITRO,,[Defined if we have Nitro])
                have_nitro="yes"
        fi
fi
AM_CONDITIONAL([HAVE_NITRO], [test "$have_nitro" = "yes"])
AM_CONDITIONAL([BUILD_NITRO], [test "$build_nitro" = "yes"])

AC_CHECK_FOUND([nitro], [nitro],[Nitro],$1,$2)

done_nitro="yes"
fi
])
