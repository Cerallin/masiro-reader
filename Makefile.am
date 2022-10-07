AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
          lib/libepd37 \
          lib/libfont \
          lib/libimage \
          lib/libdom \
          lib/libgraphic

bin_PROGRAMS = masiro

masiro_SOURCES = src/main.cpp

AM_CPPFLAGS = \
              -I$(top_srcdir)/lib/libepd37 \
              -I$(top_srcdir)/lib/libfont \
              -I$(top_srcdir)/lib/libimage \
              -I$(top_srcdir)/lib/libgraphic

masiro_LDADD = \
              $(top_builddir)/lib/libepd37/libepd37.la \
              $(top_builddir)/lib/libfont/libfont.la \
              $(top_builddir)/lib/libimage/libimage.la \
              $(top_builddir)/lib/libgraphic/libgraphic.la
