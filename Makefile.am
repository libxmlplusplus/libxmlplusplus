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

include $(srcdir)/MSVC_NMake/filelist.am

dist_noinst_DATA = \
  $(addprefix MSVC_NMake/,$(msvc_nmake_data)) \
  README.md

dist_noinst_SCRIPTS = autogen.sh

DISTCLEANFILES = MSVC_NMake/libxml++/libxml++config.h

# Distribute files needed when building libxml++ with Meson.
EXTRA_DIST = \
  meson.build \
  meson_options.txt \
  libxml++config.h.meson \
  MSVC_NMake/libxml++/meson.build \
  docs/manual/can_use_xmllint.xml \
  docs/manual/insert_example_code.py \
  docs/manual/meson.build \
  docs/reference/meson.build \
  examples/meson.build \
  libxml++/meson.build \
  subprojects/libxml2_cmake.wrap \
  subprojects/mm-common.wrap \
  tests/meson.build \
  tools/build_scripts/tutorial-custom-cmd.py \
  tools/conf_tests/have_exception_ptr.cc \
  untracked/README

# Optional: auto-generate the ChangeLog file from the git log on make dist
include $(top_srcdir)/build/dist-changelog.am
