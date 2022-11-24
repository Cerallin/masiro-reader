noinst_LTLIBRARIES = libdisplay.la

libdisplay_la_SOURCES = frame.cpp

libdisplay_ladir = $(includedir)/masiro

libdisplay_la_HEADERS = frame.h

AM_CPPFLAGS = -I$(top_srcdir) \
              -I$(top_srcdir)/lib/libfont \
              -I$(top_srcdir)/lib/libimage \
              -I$(top_srcdir)/lib/libgraphic

if FAKE_EPD
    # TODO fake epd
else
    AM_CPPFLAGS += -I$(top_srcdir)/lib/libepd37
endif #FAKE_EPD
