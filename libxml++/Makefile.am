## Copyright (C) 2015  The libxml++ development team
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.

AUTOMAKE_OPTIONS = subdir-objects

include $(srcdir)/filelist.am

lib_LTLIBRARIES = libxml++-@LIBXMLXX_API_VERSION@.la

libxml___@LIBXMLXX_API_VERSION@_la_SOURCES = $(cc_sources) $(h_sources_public)
libxml___@LIBXMLXX_API_VERSION@_la_LDFLAGS = -no-undefined -version-info $(LIBXMLXX_SO_VERSION)
libxml___@LIBXMLXX_API_VERSION@_la_LIBADD = $(LIBXMLXX_LIBS)

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) $(LIBXMLXX_CFLAGS) -DLIBXMLPP_BUILD
AM_CXXFLAGS = $(LIBXMLXX_WXXFLAGS)

library_includedir = $(includedir)/$(LIBXMLXX_MODULE_NAME)/libxml++
nobase_library_include_HEADERS = $(h_sources_public)
