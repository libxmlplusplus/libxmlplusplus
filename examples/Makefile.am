## Copyright (C) 2012 The libxml++ development team
##
## This file is part of libxml++.
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
## License along with this library. If not, see <http://www.gnu.org/licenses/>.

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir) -I. $(LIBXMLXX_CFLAGS)
AM_CXXFLAGS = $(LIBXMLXX_WXXFLAGS)
LDADD = $(top_builddir)/libxml++/libxml++-$(LIBXMLXX_API_VERSION).la $(LIBXMLXX_LIBS)

check_PROGRAMS = \
  dom_build/dom_build \
  dom_parse_entities/dom_parse_entities \
  dom_parser/dom_parser \
  dom_parser_raw/dom_parser_raw \
  dom_read_write/dom_read_write \
  dom_update_namespace/dom_update_namespace \
  dom_xinclude/dom_xinclude \
  dom_xpath/dom_xpath \
  dtdvalidation/dtdvalidation \
  import_node/import_node \
  sax_exception/sax_exception \
  sax_parser/sax_parser \
  sax_parser_build_dom/sax_parser_build_dom \
  sax_parser_entities/sax_parser_entities \
  schemavalidation/schemavalidation \
  textreader/textreader

# Shell scripts that call the example programs.
check_SCRIPTS = \
  dom_build/make_check.sh \
  dom_parse_entities/make_check.sh \
  dom_parser/make_check.sh \
  dom_parser_raw/make_check.sh \
  dom_read_write/make_check.sh \
  dom_update_namespace/make_check.sh \
  dom_xinclude/make_check.sh \
  dom_xpath/make_check.sh \
  dtdvalidation/make_check.sh \
  import_node/make_check.sh \
  sax_exception/make_check.sh \
  sax_parser/make_check.sh \
  sax_parser_build_dom/make_check.sh \
  sax_parser_entities/make_check.sh \
  schemavalidation/make_check.sh \
  textreader/make_check.sh

TESTS = $(check_SCRIPTS)

xmlpp_test_util = testutilities.h testutilities.cc

dom_build_dom_build_SOURCES = \
  dom_build/main.cc
dom_parse_entities_dom_parse_entities_SOURCES = \
  dom_parse_entities/main.cc $(xmlpp_test_util)
dom_parser_dom_parser_SOURCES = \
  dom_parser/main.cc $(xmlpp_test_util)
dom_parser_raw_dom_parser_raw_SOURCES = \
  dom_parser_raw/main.cc
dom_read_write_dom_read_write_SOURCES = \
  dom_read_write/main.cc
dom_update_namespace_dom_update_namespace_SOURCES = \
  dom_update_namespace/main.cc
dom_xinclude_dom_xinclude_SOURCES = \
  dom_xinclude/main.cc
dom_xpath_dom_xpath_SOURCES = \
  dom_xpath/main.cc
dtdvalidation_dtdvalidation_SOURCES = \
  dtdvalidation/main.cc
import_node_import_node_SOURCES = \
  import_node/main.cc
sax_exception_sax_exception_SOURCES = \
  sax_exception/main.cc \
  sax_exception/myparser.cc \
  sax_exception/myparser.h
sax_parser_sax_parser_SOURCES = \
  sax_parser/main.cc \
  sax_parser/myparser.cc \
  sax_parser/myparser.h
sax_parser_build_dom_sax_parser_build_dom_SOURCES = \
  sax_parser_build_dom/main.cc \
  sax_parser_build_dom/svgparser.cc \
  sax_parser_build_dom/svgparser.h \
  sax_parser_build_dom/svgdocument.cc \
  sax_parser_build_dom/svgdocument.h \
  sax_parser_build_dom/svgelement.cc \
  sax_parser_build_dom/svgelement.h \
  sax_parser_build_dom/svgpath.h \
  sax_parser_build_dom/svggroup.h
sax_parser_entities_sax_parser_entities_SOURCES = \
  sax_parser_entities/main.cc \
  sax_parser_entities/myparser.cc \
  sax_parser_entities/myparser.h
schemavalidation_schemavalidation_SOURCES = \
  schemavalidation/main.cc
textreader_textreader_SOURCES = \
  textreader/main.cc

dist_noinst_DATA = \
  README \
  dom_parse_entities/example.xml \
  dom_parse_entities/example.dtd \
  dom_parser/example.xml \
  dom_parser/example_with_namespace.xml \
  dom_parser/example_invalid.xml \
  dom_parser/example.dtd \
  dom_parser_raw/example.xml \
  dom_parser_raw/example_invalid.xml \
  dom_parser_raw/example.dtd \
  dom_read_write/README \
  dom_read_write/example.xml \
  dom_read_write/example.dtd \
  dom_update_namespace/example1.xml \
  dom_update_namespace/example2.xml \
  dom_xinclude/example.xml \
  dom_xinclude/include1.txt \
  dom_xinclude/include2.xml \
  dom_xpath/example.xml \
  dtdvalidation/example.dtd \
  import_node/example1.xml \
  import_node/example2.xml \
  sax_exception/example.xml \
  sax_parser/example.xml \
  sax_parser_build_dom/README \
  sax_parser_build_dom/example.xml \
  sax_parser_entities/example.xml \
  schemavalidation/example.rng \
  schemavalidation/example.xml \
  schemavalidation/example.xsd \
  textreader/example.xml

# Build the shell scripts that call the example programs.
#
# Some programs can find their input file(s) only if the current directory,
# when they are executed, is the program's own source directory.
# To make these program invocations as consistent as possible, and to avoid
# having to specify parameters for the programs, most programs are executed
# from their own source directory.

# dom_read_write shall write its output file in the build directory,
# which may or may not be the same as the source directory.
# Here it's necessary to specify parameters when the input file and the output
# file are located in different directories.
dom_read_write/make_check.sh: Makefile
	echo '# Generated and used by "make check"' >$@
	echo 'dom_read_write/dom_read_write "$(srcdir)/dom_read_write/example.xml" dom_read_write/example_output.xml >/dev/null' >>$@
	chmod +x $@

script_template = cd "$(srcdir)/<!progname!>" && "$(abs_builddir)/<!progname!>/<!progname!>" >/dev/null
standard_scripts = $(filter-out dom_read_write/make_check.sh,$(check_SCRIPTS))

# All other script files are generated like so:
$(standard_scripts): Makefile
	echo '# Generated and used by "make check"' >$@
	echo '$(subst <!progname!>,$(subst /make_check.sh,,$@),$(script_template))' >>$@
	chmod +x $@

CLEANFILES = \
  dom_read_write/example_output.xml \
  $(check_SCRIPTS)
