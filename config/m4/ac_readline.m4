# SYNOPSIS
#
#   AC_READLINE([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the READLINE libraries. If we find them, we set the Makefile
# conditional HAVE_READLINE. 
# 
# To allow users to build there own copy of READLINE, we also define
# BUILD_READLINE

AC_DEFUN([AC_READLINE],
[
# Guard against running twice
if test "x$done_readline" = "x"; then
AC_HANDLE_WITH_ARG([readline], [readline], [Readline], $2, $3, $1)
if test "x$want_readline" = "xyes"; then
        AC_MSG_CHECKING([for readline library])
        succeeded=no
        if test "$ac_readline_path" != ""; then
            READLINE_CFLAGS="-I$ac_readline_path/include"
            READLINE_LIBS="-R$ac_readline_path/lib -L$ac_readline_path/lib -lreadline"
	    READLINE_PREFIX="$ac_readline_path"
            succeeded=yes
        else
	    AC_SEARCH_LIB([READLINE], [readline], , [readline/readline.h], ,
                          [libreadline], [-lreadline])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_readline" == "xyes" ; then
            build_readline="yes"
            ac_readline_path="\${prefix}"
            READLINE_CFLAGS="-I$ac_readline_path/include"
            READLINE_LIBS="-R$ac_readline_path/lib -L$ac_readline_path/lib -lreadline"
	    READLINE_PREFIX="$ac_readline_path"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(READLINE_CFLAGS)
                AC_SUBST(READLINE_LIBS)
                AC_SUBST(READLINE_PREFIX)
                AC_DEFINE(HAVE_READLINE,,[Defined if we have readline])
                have_readline="yes"
        fi
fi
AM_CONDITIONAL([HAVE_READLINE], [test "$have_readline" = "yes"])
AM_CONDITIONAL([BUILD_READLINE], [test "$build_readline" = "yes"])

AC_CHECK_FOUND([readline], [readline],[Readline],$1,$2)
done_readline="yes"
fi
])
