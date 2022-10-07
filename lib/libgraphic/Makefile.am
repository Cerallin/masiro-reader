noinst_LTLIBRARIES = libgraphic.la

libgraphic_la_SOURCES = layer.cpp \
                        imagelayer.cpp \
                        textlayer.cpp \
                        style/texttypesetting.cpp

libgraphic_la_LIBADD = \
                      ../libepd37/libepd37.la \
                      ../libfont/libfont.la \
                      ../libimage/libimage.la

AM_CPPFLAGS = \
              -I$(top_srcdir)/lib/libepd37 \
              -I$(top_srcdir)/lib/libimage \
              -I$(top_srcdir)/lib/libfont

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = \
            -lCppUTest \
            ../libepd37/libepd37.la \
            ../libfont/libfont.la \
            ../libimage/libimage.la \
            ./libgraphic.la

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage
