# SYNOPSIS
#
#   AC_HDF4([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the HDF 4 libraries. If we find them, we set the Makefile
# conditional HAVE_HDF4. We as set HDF4_CPPFLAGS and HDF4_LDFLAGS
# 
# To allow users to build there own copy of HDF4, we also define
# BUILD_HDF4
#
# To allow users to build there own copy of VICAR RTL, we also define
# BUILD_VICAR_RTL.
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


AC_DEFUN([AC_HDF4],
[
# Guard against running twice
if test "x$done_hdf4" = "x"; then
AC_HANDLE_WITH_ARG([hdf4], [hdf4], [HDF 4], $2, $3, $1)
if test "x$want_hdf4" = "xyes"; then
        AC_MSG_CHECKING([for HDF4 library])
        succeeded=no
        if test "$ac_hdf4_path" != ""; then
            HDF4HOME=$ac_hdf4_path
            HDF4_LIBS="-L$ac_hdf4_path/lib -lmfhdf -ldf"
            HDF4_CFLAGS="-I$ac_hdf4_path/include"
            succeeded=yes
        else
            # Hdf headers go into a couple of places depending on how
            # they were installed. First check the various include directories,
            # and then check include/hdf directories
	    AC_SEARCH_LIB([HDF4], [hdf4], , [hdf.h], ,
                          [libmfhdf], [-lmfhdf -ldf])
            if test "$succeeded" != "yes" ; then
	       AC_SEARCH_LIB([HDF4], [hdf4], [hdf/], [hdf.h], [hdf/],
 	                     [libmfhdf], [-lmfhdf -ldf])
            fi
            HDF4HOME=$HDF4_PREFIX
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_hdf4" == "xyes" ; then
            build_hdf4="yes"
            ac_hdf4_path="\${prefix}"
            HDF4HOME=$ac_hdf4_path
            HDF4_LIBS="-L$ac_hdf4_path/lib -lmfhdf -ldf"
            HDF4_CFLAGS="-I$ac_hdf4_path/include"
            succeeded=yes
        fi

        if test "$succeeded" = "yes" ; then
          # Check for szlib. If found, hdf still might not use it, but we add it to the link
          # line in case it needs it
          AC_SEARCH_LIB([SZLIB], [szlib], , [szlib.h], , [libsz], [-lsz])
	  HDF4_LIBS="$HDF4_LIBS $SZLIB_LIBS"
          # Search overwrites succeeded. We want to succeed if we find szlib or not, so go
          # ahead and reset this
          succeeded="yes"
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(HDF4_CFLAGS)
                AC_SUBST(HDF4HOME)
                AC_SUBST(HDF4_LIBS)
                AC_DEFINE(HAVE_HDF4,,[Defined if we have HDF4])
                have_hdf4="yes"
        fi
fi
AM_CONDITIONAL([HAVE_HDF4], [test "$have_hdf4" = "yes"])
AM_CONDITIONAL([BUILD_HDF4], [test "$build_hdf4" = "yes"])

AC_CHECK_FOUND([hdf4], [hdf4],[HDF 4],$1,$2)

done_hdf4="yes"
fi
])
