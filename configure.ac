#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Masiro Reader], [v0.9], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([subdir-objects])

LT_INIT

SRC_DIR=`readlink -f $srcdir`
AC_DEFINE_UNQUOTED([SRC_DIR], ["$SRC_DIR"], [source path])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Debugging code
AC_ARG_ENABLE(debug,
    AS_HELP_STRING([--enable-debug], [enable debugging, default: no]))
AS_IF([! test "x$enable_debug" = xyes ], [
    AC_DEFINE([NDEBUG], [1], [disable debugging code])
])

# Check Qt
AX_HAVE_QT

# Using fake epd
AC_ARG_WITH(fake-epd,
    AS_HELP_STRING([--with-fake-epd], [using Qt form as fake display, default: no]))
AS_IF([ test "x$with_fake_epd" = xyes ], [
    AS_IF([ ! test "x$have_qt" = xyes ], [
        AC_MSG_ERROR([Enabled fake epd without Qt installed])
    ])

    AC_DEFINE([FAKE_EPD], [1], [disable fake epd])
])
AM_CONDITIONAL([FAKE_EPD], [ test "x$with_fake_epd" = xyes ])

# Checks for libraries.
AC_CHECK_LIB([bcm2835], [bcm2835_init])
AC_CHECK_LIB([pthread], [pthread_create])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([floor gettimeofday strpbrk memset])

AC_CONFIG_FILES([Makefile
                 lib/libbtn/Makefile
                 lib/libdom/Makefile
                 lib/libdisplay/Makefile
                 lib/libepd37/Makefile
                 lib/libfont/Makefile
                 lib/libft6336/Makefile
                 lib/libimage/Makefile
                 lib/libgraphic/Makefile])

AC_OUTPUT
