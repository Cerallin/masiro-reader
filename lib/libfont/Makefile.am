noinst_LTLIBRARIES = libfont.la

libfont_la_SOURCES = font.cpp \
					 fontface.cpp \
					 codepoint.cpp

libfont_la_LIBADD = @LDFLAGS@

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -lfont -lCppUTest

# test_CXXFLAGS = -fprofile-arcs -ftest-coverage
