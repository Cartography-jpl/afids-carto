# SYNOPSIS
#
#   AC_SQLITE3([required], [can_build], [default_build])
#
# DESCRIPTION
#
# This looks for the SQLITE3 libraries. If we find them, we set the Makefile
# conditional HAVE_SQLITE3. 
# 
# To allow users to build there own copy of SQLITE3, we also define
# BUILD_SQLITE3

AC_DEFUN([AC_SQLITE3],
[
# Guard against running twice
if test "x$done_sqlite3" = "x"; then
AC_HANDLE_WITH_ARG([sqlite3], [sqlite3], [Sqlite3], $2, $3, $1)
if test "x$want_sqlite3" = "xyes"; then
        AC_MSG_CHECKING([for sqlite3 library])
        succeeded=no
        if test "$ac_sqlite3_path" != ""; then
            SQLITE3_CFLAGS="-I$ac_sqlite3_path/include"
            SQLITE3_LIBS="-R$ac_sqlite3_path/lib -L$ac_sqlite3_path/lib -lsqlite3"
            succeeded=yes
        else
	    AC_SEARCH_LIB([SQLITE3], [sqlite3], , [sqlite3.h], ,
                          [libsqlite3], [-lsqlite3])
        fi
	if test "$succeeded" != "yes" -a "x$build_needed_sqlite3" == "xyes" ; then
            build_sqlite3="yes"
            ac_splite3_path="\${prefix}"
            SQLITE3_CFLAGS="-I$ac_sqlite3_path/include"
            SQLITE3_LIBS="-R$ac_sqlite3_path/lib -L$ac_sqlite3_path/lib -lsqlite3"
            succeeded=yes
        fi

        if test "$succeeded" != "yes" ; then
                AC_MSG_RESULT([no])
        else
                AC_MSG_RESULT([yes])
                AC_SUBST(SQLITE3_CFLAGS)
                AC_SUBST(SQLITE3_LIBS)
                AC_DEFINE(HAVE_SQLITE3,,[Defined if we have SQLITE3])
                have_sqlite3="yes"
        fi
fi
AM_CONDITIONAL([HAVE_SQLITE3], [test "$have_sqlite3" = "yes"])
AM_CONDITIONAL([BUILD_SQLITE3], [test "$build_sqlite3" = "yes"])

AC_CHECK_FOUND([sqlite3], [sqlite3],[Sqlite3],$1,$2)
done_sqlite3="yes"
fi
])
