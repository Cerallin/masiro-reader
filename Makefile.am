AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
          lib/libdom \
          lib/libdisplay \
          lib/libfont \
          lib/libgraphic \
          lib/libimage

################ LIBRARIES #################

lib_LTLIBRARIES = libmasiro.la

libmasiro_la_SOURCES = libmasiro/text.cpp

libmasiro_la_LIBADD = $(top_builddir)/lib/libdisplay/libdisplay.la \
                      $(top_builddir)/lib/libfont/libfont.la \
                      $(top_builddir)/lib/libimage/libimage.la \
                      $(top_builddir)/lib/libgraphic/libgraphic.la

################  BINARIES  ################

bin_PROGRAMS = masiro

masiro_SOURCES = src/main.cpp

masiro_LDADD = $(top_builddir)/libmasiro.la

AM_CPPFLAGS = -I$(top_srcdir)/lib/libdisplay \
              -I$(top_srcdir)/lib/libfont \
              -I$(top_srcdir)/lib/libimage \
              -I$(top_srcdir)/lib/libgraphic

if FAKE_EPD
    # TODO fake epd
else
    SUBDIRS += lib/libepd37
    AM_CPPFLAGS += -I$(top_srcdir)/lib/libepd37
    libmasiro_la_LIBADD += $(top_builddir)/lib/libepd37/libepd37.la
endif #FAKE_EPD
