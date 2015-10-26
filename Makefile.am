## This file is part of libxml++.

ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}
DISTCHECK_CONFIGURE_FLAGS = --enable-warnings=fatal

if ENABLE_DOCUMENTATION
doc_subdirs = docs
else
doc_subdirs =
endif
SUBDIRS = libxml++ examples tests $(doc_subdirs)

library_configdir = $(libdir)/$(LIBXMLXX_MODULE_NAME)/include
nodist_library_config_HEADERS = libxml++config.h

pkgconfigdir = $(libdir)/pkgconfig
nodist_pkgconfig_DATA = $(LIBXMLXX_MODULE_NAME).pc

include $(srcdir)/MSVC_Net2010/filelist.am

dist_noinst_DATA = $(addprefix MSVC_Net2010/,$(msvc_net2010_data))

dist_noinst_SCRIPTS = autogen.sh

DISTCLEANFILES = MSVC_Net2010/libxml++/libxml++config.h

# Optional: auto-generate the ChangeLog file from the git log on make dist
include $(top_srcdir)/build/dist-changelog.am
