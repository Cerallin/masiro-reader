noinst_LTLIBRARIES = libgraphic.la

AM_CPPFLAGS = -I$(top_srcdir)/lib \
              -I$(top_srcdir)

libgraphic_la_SOURCES = layer.cpp \
                        imagelayer.cpp \
                        textlayer.cpp \
                        style/texttypesetting.cpp

libgraphic_ladir = $(includedir)/graphics

libgraphic_la_HEADERS = style.h\
                        layer.h \
                        imagelayer.h \
                        textlayer.h

libgraphic_traitsdir = $(includedir)/graphics/traits

libgraphic_traits_HEADERS = traits/layersetters.h

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = \
            -lCppUTest \
            ../fonts/libfont.la \
            ../images/libimage.la \
            ./libgraphic.la

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage

if FAKE_EPD
    # TODO fake epd
else
    AM_CPPFLAGS += -I$(top_srcdir)/lib/libepd37
    test_LDADD += ../epd37/libepd37.la
endif #FAKE_EPD
