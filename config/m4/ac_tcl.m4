# SYNOPSIS
#
#   AC_TCL([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the Tcl/TK library. If we find them, we set the Makefile
# conditional HAVE_TCL. We as set TCL_LIBS and TCL_CFLAGS
# 
# To allow users to build there own copy of Tcl/Tk, we also define
# BUILD_TCL
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

AC_DEFUN([AC_TCL],
[
# Guard against running twice
if test "x$done_tcl" = "x"; then
AC_HANDLE_WITH_ARG([tcl], [tcl], [Tcl/Tk], $2, $3, $1)
if test "x$want_tcl" = "xyes"; then
        AC_MSG_CHECKING([for Tcl library])
        succeeded=no
        if test "$ac_tcl_path" != ""; then
            TCL_LIBS=['`source '"$ac_tcl_path/lib/tclConfig.sh; source $ac_tcl_path/lib/tkConfig.sh;"'echo $$TK_LIBS $$TCL_LIB_SPEC $$TK_LIBS $$TCL_LIBS`'] 
            TCL_CFLAGS=['`source '"$ac_tcl_path/lib/tclConfig.sh; source $ac_tcl_path/lib/tkConfig.sh;"'echo $$TK_INCLUDE_SPEC $$TCL_INCLUDE_SPEC`'] 
	    if test "$build_tcl" != "yes"; then
               tcl_version=`source $ac_tcl_path/lib/tclConfig.sh; echo $TCL_VERSION`
            fi
            succeeded=yes
        else
	  for ac_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /opt/afids_support /usr /usr/local /opt /opt/local /sw ; do
	    if test -e "$ac_path_tmp/lib/tclConfig.sh"; then
              ac_tcl_path="$ac_path_tmp"
              TCL_LIBS=['`source '"$ac_path_tmp/lib/tclConfig.sh; source $ac_path_tmp/lib/tkConfig.sh;"'echo $$TK_LIBS $$TCL_LIB_SPEC $$TK_LIBS $$TCL_LIBS`'] 
              TCL_CFLAGS=['`source '"$ac_path_tmp/lib/tclConfig.sh; source $ac_path_tmp/lib/tkConfig.sh;"'echo $$TK_INCLUDE_SPEC $$TCL_INCLUDE_SPEC`'] 
	      tcl_version=`source $ac_path_tmp/lib/tclConfig.sh; echo $TCL_VERSION`
	      succeeded=yes
              break
            elif test -e "$ac_path_tmp/lib64/tclConfig.sh"; then
              ac_tcl_path="$ac_path_tmp"
              TCL_LIBS=['`source '"$ac_path_tmp/lib64/tclConfig.sh; source $ac_path_tmp/lib64/tkConfig.sh;"'echo $$TK_LIBS $$TCL_LIB_SPEC $$TK_LIBS $$TCL_LIBS`'] 
              TCL_CFLAGS=['`source '"$ac_path_tmp/lib64/tclConfig.sh; source $ac_path_tmp/lib64/tkConfig.sh;"'echo $$TK_INCLUDE_SPEC $$TCL_INCLUDE_SPEC`'] 
	      tcl_version=`source $ac_path_tmp/lib64/tclConfig.sh; echo $TCL_VERSION`
              succeeded=yes
              break
            fi
          done
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_tcl" == "xyes" ; then
            build_tcl="yes"
            ac_tcl_path="\${prefix}"
            TCL_LIBS=['`source '"$ac_tcl_path/lib/tclConfig.sh; source $ac_tcl_path/lib/tkConfig.sh;"'echo $$TK_LIBS $$TCL_LIB_SPEC $$TK_LIBS $$TCL_LIBS`'] 
            TCL_CFLAGS=['`source '"$ac_tcl_path/lib/tclConfig.sh; source $ac_tcl_path/lib/tkConfig.sh;"'echo $$TK_INCLUDE_SPEC $$TCL_INCLUDE_SPEC`'] 
            succeeded=yes
        fi
        if test "$build_tcl" = "yes"; then
             tcl_version="8.6"
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(TCL_LIBS)
                AC_SUBST(TCL_CFLAGS)
                AC_SUBST(tcl_version)
                AC_DEFINE(HAVE_TCL,,[Defined if we have Tcl])
                have_tcl="yes"
        fi
fi

# Make sure we have the X11 headers required to build tcl/tk
if test "$build_tcl" = "yes"; then
   AC_CHECK_HEADER([X11/Xlib.h], [],
   [ 
   have_tcl=no
   build_tcl=no
   if test "$1" = "required"; then
      AC_MSG_ERROR([Could not find X11/Xlib.h, which is required to build the required TCL/TK. Either install this header, or install TCL/TK so we do not need to build it.])
   fi
   ])
fi

AM_CONDITIONAL([HAVE_TCL], [test "$have_tcl" = "yes"])
AM_CONDITIONAL([BUILD_TCL], [test "$build_tcl" = "yes"])

AC_CHECK_FOUND([tcl], [tcl],[Tcl/Tk],$1,$2)

done_tcl="yes"
fi
])
