bin_PROGRAMS = client

client_SOURCES = main.cpp \
				 graphic/layer.cpp \
				 graphic/paragraph.cpp \
				 graphic/text.cpp

client_CPPFLAGS = -g -O0 -I$(top_builddir)/include/

client_LDFLAGS = -lm -lpthread

client_LDADD = \
               $(top_builddir)/lib/libdom.la \
               $(top_builddir)/lib/libepd37.la \
               $(top_builddir)/lib/libfont.la \
               $(top_builddir)/lib/libimage.la