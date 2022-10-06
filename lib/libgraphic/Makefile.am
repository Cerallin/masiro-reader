noinst_LTLIBRARIES = libgraphic.la

libgraphic_la_SOURCES = layer.cpp \
						imagelayer.cpp \
						textlayer.cpp \
						style/texttypesetting.cpp

libgraphic_la_LIBADD = @LDFLAGS@

AM_CPPFLAGS = -I../libepd37 -I../libimage -I../libfont

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -lgraphic -limage -lepd37 -lfont -lCppUTest -L../libepd37 -L../libimage -L../libfont

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage
