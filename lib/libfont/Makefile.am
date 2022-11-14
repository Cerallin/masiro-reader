noinst_LTLIBRARIES = libfont.la

libfont_la_SOURCES = font.cpp \
					 fontmetrics.cpp \
					 fontface.cpp \
					 fontfamily.cpp \
					 codepoint.cpp

libfont_la_LIBADD = @LDFLAGS@

AM_CPPFLAGS = -I$(top_srcdir)

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -lfont -lCppUTest

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage
