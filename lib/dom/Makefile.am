noinst_LTLIBRARIES = libdom.la

AM_CPPFLAGS = -I$(top_srcdir)/lib

libdom_la_SOURCES = \
                    style/margin.cpp \
                    style/padding.cpp \
                    style/position.cpp \
                    style/textalign.cpp \
                    style/textindent.cpp \
                    treenode.cpp \
                    domnode.cpp

libdom_ladir = $(includedir)/dom

libdom_la_HEADERS = \
                    style/margin.h \
                    style/padding.h \
                    style/position.h \
                    style/textalign.h \
                    style/textindent.h \
                    treenode.h \
                    domnode.h

libdom_la_LIBADD = @LDFLAGS@

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -ldom -lCppUTest
