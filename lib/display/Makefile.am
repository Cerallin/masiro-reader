noinst_LTLIBRARIES = libdisplay.la

libdisplay_la_SOURCES = \
                        display.cpp \
                        frame.cpp \
                        bufferpool.cpp

libdisplay_ladir = $(includedir)/display

libdisplay_la_HEADERS = \
                        display.h \
                        frame.h \
                        bufferpool.h

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(top_srcdir)

if FAKE_EPD
    # TODO fake epd
else
    AM_CPPFLAGS += -I$(top_srcdir)/lib/libepd37
endif #FAKE_EPD
