#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Masiro Reader], [v0.9], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([subdir-objects])

LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([bcm2835], [bcm2835_init])
AC_CHECK_LIB([pthread], [pthread_create])
# AC_CHECK_LIB([CppUTest], [CommandLineTestRunner])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([floor gettimeofday strpbrk])

AC_CONFIG_FILES([Makefile
                 lib/libbtn/Makefile
                 lib/libdom/Makefile
                 lib/libepd37/Makefile
                 lib/libfont/Makefile
                 lib/libft6336/Makefile
                 lib/libimage/Makefile
                 src/Makefile])

AC_OUTPUT
