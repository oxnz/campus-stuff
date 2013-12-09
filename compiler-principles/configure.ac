#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([microcc], [0.1], [yunxinyi@gmail.com])
AC_CONFIG_SRCDIR([src/microcc.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror])
# Checks for programs.
AC_CHECK_PROGS([CXX], [clang++ g++], [:])
if test "$CXX" = :; then
	AC_MSG_ERROR([This package needs clang++ or g++ to compile], [1])
fi
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([Makefile
				 src/Makefile
				 tools/Makefile
				 exp/Makefile
                 test/Makefile])
AC_OUTPUT
