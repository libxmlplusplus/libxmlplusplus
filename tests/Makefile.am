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
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) -I. $(LIBXMLXX_CFLAGS)
AM_CXXFLAGS = $(LIBXMLXX_WXXFLAGS)
LDADD = $(top_builddir)/libxml++/libxml++-$(LIBXMLXX_API_VERSION).la $(LIBXMLXX_LIBS)

check_PROGRAMS = \
	saxparser_chunk_parsing_inconsistent_state/test \
	saxparser_parse_double_free/test \
	saxparser_parse_stream_inconsistent_state/test

TESTS = $(check_PROGRAMS)

saxparser_chunk_parsing_inconsistent_state_test_SOURCES = saxparser_chunk_parsing_inconsistent_state/main.cc
saxparser_parse_double_free_test_SOURCES = saxparser_parse_double_free/main.cc
saxparser_parse_stream_inconsistent_state_test_SOURCES = saxparser_parse_stream_inconsistent_state/main.cc
