INCLUDES = -I$(top_srcdir) @LIBXML_CFLAGS@

DEFS=@DEFS@ -DLIBXMLPP_COMPILATION

h_sources_public = outputbuffer.h ostreamoutputbuffer.h
h_sources_private = 
cc_sources = outputbuffer.cc ostreamoutputbuffer.cc
cc_sources_private =

noinst_LTLIBRARIES = libio.la
libio_la_SOURCES = $(cc_sources) $(h_sources_public) $(h_sources_private) $(cc_sources_private)

# Install the headers:
library_includedir=$(includedir)/libxml++-1.0/libxml++/io
library_include_HEADERS = $(h_sources_public)

