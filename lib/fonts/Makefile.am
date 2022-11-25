noinst_LTLIBRARIES = libfont.la

AM_CPPFLAGS = -I$(top_srcdir)/lib \
			  -I$(top_srcdir)

libfont_la_SOURCES = font.cpp \
					 fontmetrics.cpp \
					 fontface.cpp \
					 fontfamily.cpp \
					 codepoint.cpp

libfont_ladir = $(includedir)/fonts

libfont_la_HEADERS = font.h \
					 fontmetrics.h \
					 fontface.h \
					 fontfamily.h \
					 codepoint.h \
					 stb_truetype.h

libfont_la_LIBADD = @LDFLAGS@

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -lfont -lCppUTest

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage
