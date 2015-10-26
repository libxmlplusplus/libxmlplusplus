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

include $(top_srcdir)/libxml++/filelist.am

book_name = $(LIBXMLXX_MODULE_NAME)
doc_input = $(addprefix $(top_srcdir)/libxml++/,$(h_sources_public))

# Sets dist_noinst_DATA, DISTCLEANFILES and MAINTAINERCLEANFILES
include $(top_srcdir)/build/doc-reference.am

manual/libxml++.xml: manual/libxml++_without_code.xml manual/insert_example_code.pl
	$(AM_V_GEN)$(PERL) -- manual/insert_example_code.pl ../examples $< >$@

manualdir = $(docdir)/manual
manual_htmldir = $(manualdir)/html
manual_html_files = $(wildcard manual/html/*.html)

manual/html/index.html: manual/libxml++.xml manual/docbook-customisation.xsl
	$(AM_V_at)rm -fr manual/html
	$(AM_V_at)$(MKDIR_P) manual/html
	$(AM_V_GEN)$(XSLTPROC) -o manual/html/ --xinclude --catalogs \
		$(top_srcdir)/docs/manual/docbook-customisation.xsl $<

manual/libxml++.pdf: manual/libxml++.xml
	$(AM_V_GEN)$(DB2LATEX) -o $@ -O manual -tpdf $<

validate_original_manual: manual/libxml++_without_code.xml
	$(XMLLINT) --xinclude --postvalid --noout $<

validate_manual: manual/libxml++.xml
	$(XMLLINT) --xinclude --postvalid --noout $<

all-local: manual/html/index.html

dist_noinst_DATA += \
	manual/docbook-customisation.xsl \
	$(manual_html_files) \
	manual/libxml++.xml \
	manual/libxml++_without_code.xml

dist_noinst_SCRIPTS = \
	manual/insert_example_code.pl

MAINTAINERCLEANFILES += manual/html/*.html

# Make it easy to upload to the sourceforge web site.
# The manual and API reference are hosted on developer.gnome.org via tarball releases.
web_path_project = /home/groups/l/li/libxmlplusplus/htdocs/
rsync_args = -vz --rsh ssh --recursive --cvs-exclude --exclude="**1.0**" --exclude=".gitignore" --exclude="Makefile" --exclude="Makefile.in" --exclude=".deps" --exclude=".libs"

## TODO: When I moved this code from libxml++/Makefile.am to libxml++/docs/Makefile.am,
## I changed the relative paths accordingly (docs/index.html -> index.html,
## examples -> ../examples). I don't know if that changes the location where the files
## are stored at sourceforge.net. If necessary, change again before you use 'make post-html'.
## Why are the examples copied to sourceforge? / 2015-09-11 Kjell Ahlstedt
post-html: index.html
	rsync $(rsync_args) -r index.html $$USER,libxmlplusplus@web.sourceforge.net:$(web_path_project)
	rsync $(rsync_args) -r ../examples $$USER,libxmlplusplus@web.sourceforge.net:$(web_path_project)
