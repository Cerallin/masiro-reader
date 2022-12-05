AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir) \
              -I$(top_srcdir)/lib \
              -I$(top_srcdir)/libmasiro

SUBDIRS = \
          lib/dom \
          lib/display \
          lib/fonts \
          lib/graphics \
          lib/images

################ LIBRARIES #################

lib_LTLIBRARIES = libmasiro.la

libmasiro_ladir = $(includedir)

libmasiro_la_HEADERS = libmasiro/display.h \
                       libmasiro/graphics.h \
                       libmasiro/masiro.h

libmasiro_la_SOURCES = libmasiro/masiro.cpp

libmasiro_la_LIBADD = $(top_builddir)/lib/display/libdisplay.la \
                      $(top_builddir)/lib/fonts/libfont.la \
                      $(top_builddir)/lib/images/libimage.la \
                      $(top_builddir)/lib/graphics/libgraphic.la

libmasiro_la_LDFLAGS = -version-info @version_info@

################  BINARIES  ################

bin_PROGRAMS = masiro

masiro_SOURCES = src/main.cpp

masiro_LDADD = $(top_builddir)/libmasiro.la

if FAKE_EPD
    # TODO fake epd
else
    SUBDIRS += lib/epd37
    AM_CPPFLAGS += -I$(top_srcdir)/lib/epd37
    libmasiro_la_LIBADD += $(top_builddir)/lib/epd37/libepd37.la
endif #FAKE_EPD
