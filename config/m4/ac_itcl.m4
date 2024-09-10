# SYNOPSIS
#
#   AC_ITCL([required])
#
# DESCRIPTION
#
#  This tests for the itcl extension of tcl. This test is closely tied
#  to the ac_tcl.m4 test.

AC_DEFUN([AC_ITCL],
[
if test "x$build_tcl" = "xno"; then
    AC_MSG_CHECKING([for Iticl extension])
    succeeded=no
    if test "$ac_tcl_path" != ""; then
       if test -e "$ac_tcl_path/lib/itcl4.0.4/itcl.tcl"; then
          ITCL_PREFIX="$ac_tcl_path/lib/itcl4.0.4"
          succeeded=yes
       elif test -e "$ac_tcl_path/lib/itcl4.1.0/itcl.tcl"; then
          ITCL_PREFIX="$ac_tcl_path/lib/itcl4.1.0"
          succeeded=yes
       elif test -e "$ac_tcl_path/lib/itcl4.2.2/itcl.tcl"; then
          ITCL_PREFIX="$ac_tcl_path/lib/itcl4.2.2"
          succeeded=yes
       elif test -e "$ac_tcl_path/lib/tcl8.5/itcl3.4/itcl.tcl"; then
          ITCL_PREFIX="$ac_tcl_path/lib/tcl8.5/itcl3.4"
          succeeded=yes
       elif test -e "$ac_tcl_path/lib/itcl3.4/itcl.tcl"; then
          ITCL_PREFIX="$ac_tcl_path/lib/itcl3.4"
          succeeded=yes
       fi
    fi
    if test "$succeeded" != "yes"; then
	  for ac_path_tmp in $prefix $CONDA_PREFIX $THIRDPARTY /opt/afids /usr /usr/local /opt /opt/local /sw ; do
            if test -e "$ac_path_tmp/lib/itcl4.0.4/itcl.tcl"; then
                ITCL_PREFIX="$ac_path_tmp/lib/itcl4.0.4"
                succeeded=yes
            elif test -e "$ac_path_tmp/lib/itcl4.1.0/itcl.tcl"; then
                ITCL_PREFIX="$ac_path_tmp/lib/itcl4.1.0"
                succeeded=yes
            elif test -e "$ac_path_tmp/lib/itcl4.2.2/itcl.tcl"; then
                ITCL_PREFIX="$ac_path_tmp/lib/itcl4.2.2"
                succeeded=yes
            elif test -e "$ac_path_tmp/lib/tcl8.5/itcl3.4/itcl.tcl"; then
                ITCL_PREFIX="$ac_path_tmp/lib/tcl8.5/itcl3.4"
                succeeded=yes
		break
            elif test -e "$ac_path_tmp/lib/itcl3.4/itcl.tcl"; then
                ITCL_PREFIX="$ac_path_tmp/lib/itcl3.4"
                succeeded=yes
		break
            fi
          done
    fi
    if test "$succeeded" != "yes" ; then
         have_itcl=no
         AC_MSG_RESULT([no])
    else
        have_itcl=yes
        AC_MSG_RESULT([yes])
    fi
    if test "$1" = "required"; then
        if test "$succeeded" != "yes" ; then
           AC_MSG_ERROR([
The required itcl Tcl extension is requred by AFIDS. You can
specify --with-tcl=build if you want to build your own local copy
of this extension.])
        fi
    fi
else
    have_itcl=yes
    ITCL_PREFIX="\${AFIDSTOP}/lib/itcl4.0.4"
    succeeded=yes
fi
AC_SUBST(ITCL_PREFIX)
AM_CONDITIONAL([HAVE_ITCL], [test "$have_itcl" = "yes"])
])
