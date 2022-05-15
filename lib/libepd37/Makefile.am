noinst_LTLIBRARIES = libepd37.la

libepd37_la_SOURCES = epd2in13b.cpp \
					 epdif.cpp \
					 epdpaint.cpp

libepd37_la_LIBADD = @LDFLAGS@

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -lepd37 -lCppUTest
