noinst_LTLIBRARIES = libdom.la

libdom_la_SOURCES = \
                    style/margin.cpp \
                    style/padding.cpp \
                    style/position.cpp \
                    style/textalign.cpp \
                    style/textindent.cpp \
                    treenode.cpp \
                    domnode.cpp

libdom_la_LIBADD = @LDFLAGS@

TESTS = test

check_PROGRAMS = test

test_SOURCES = tests/test.cpp

test_LDADD = -ldom -lCppUTest
