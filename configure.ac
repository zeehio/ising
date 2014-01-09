#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ising], [4.1], [https://github.com/zeehio/ising/issues])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/ising.c])
AC_CONFIG_MACRO_DIR([config/m4])
AC_CONFIG_HEADERS([config/config.h])
AC_PREFIX_DEFAULT([$PWD/install])

AM_INIT_AUTOMAKE
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset mkdir pow sqrt])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 lib/Makefile
                 scripts/Makefile
                 src/Makefile
                 tools/Makefile])
AC_OUTPUT
