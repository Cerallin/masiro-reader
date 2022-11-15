AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
          lib/libepd37 \
          lib/libfont \
          lib/libimage \
          lib/libdom \
          lib/libgraphic

################ LIBRARIES #################

lib_LTLIBRARIES = libmasiro.la

libmasiro_la_SOURCES = libmasiro/text.cpp

libmasiro_la_LIBADD = $(top_builddir)/lib/libgraphic/libgraphic.la

################  BINARIES  ################

bin_PROGRAMS = masiro

masiro_SOURCES = src/main.cpp

AM_CPPFLAGS = \
              -I$(top_srcdir)/lib/libepd37 \
              -I$(top_srcdir)/lib/libfont \
              -I$(top_srcdir)/lib/libimage \
              -I$(top_srcdir)/lib/libgraphic

masiro_LDADD = $(top_builddir)/libmasiro.la
