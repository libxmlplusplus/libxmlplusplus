SUBDIRS = parsers exceptions nodes io

INCLUDES = -I$(top_srcdir) @LIBXML_CFLAGS@

DEFS=@DEFS@ -DLIBXMLPP_COMPILATION

h_sources_public = libxml++.h attribute.h dtd.h document.h noncopyable.h keepblanks.h api_export.h
h_sources_private = 
cc_sources = attribute.cc dtd.cc document.cc noncopyable.cc keepblanks.cc
cc_sources_private =

lib_LTLIBRARIES = libxml++-1.0.la
libxml___1_0_la_LDFLAGS = @WIN32_LDFLAGS@ -version-info $(GENERIC_LIBRARY_VERSION)
libxml___1_0_la_LIBADD = parsers/libparsers.la \
                         exceptions/libexceptions.la \
                         nodes/libnodes.la \
						 io/libio.la \
						 @LIBXML_LIBS@ \
						 @WIN32_LIBADD@
libxml___1_0_la_SOURCES = $(cc_sources) $(h_sources_public) $(h_sources_private) $(cc_sources_private)

# Install the headers:
library_includedir=$(includedir)/libxml++-1.0/libxml++
library_include_HEADERS = $(h_sources_public)

