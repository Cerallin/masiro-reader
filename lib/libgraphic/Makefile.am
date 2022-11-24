noinst_LTLIBRARIES = libgraphic.la

libgraphic_la_SOURCES = layer.cpp \
                        imagelayer.cpp \
                        textlayer.cpp \
                        style/texttypesetting.cpp
libgraphic_ladir = $(includedir)/masiro
libgraphic_la_HEADERS = style.h\
                        layer.h \
                        imagelayer.h \
                        textlayer.h

libgraphic_traitsdir = $(includedir)/masiro/traits
libgraphic_traits_HEADERS = traits/layersetters.h

AM_CPPFLAGS = -I$(top_srcdir) \
              -I$(top_srcdir)/lib/libimage \
              -I$(top_srcdir)/lib/libfont

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = \
            -lCppUTest \
            ../libfont/libfont.la \
            ../libimage/libimage.la \
            ./libgraphic.la

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage

if FAKE_EPD
    # TODO fake epd
else
    AM_CPPFLAGS += -I$(top_srcdir)/lib/libepd37
    test_LDADD += ../libepd37/libepd37.la
endif #FAKE_EPD
