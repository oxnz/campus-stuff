#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([rdpp], [1.5], [yunxinyi@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_CHECK_PROGS([CXX], [clang++ g++], [:])
if test "$CXX" = :; then
	AC_MSG_ERROR([This package needs clang++ or g++ to compile], [1])
fi
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdio.h stdlib.h string.h unistd.h dirent.h errno.h sys/types.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([regcomp strerror strstr strtoul strtoull])

AC_CONFIG_FILES([Makefile
				 src/Makefile
                 tools/Makefile])
AC_OUTPUT
