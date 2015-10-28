AC_INIT([libxml++], [2.40.1],
        [https://bugzilla.gnome.org/enter_bug.cgi?product=libxml%2B%2B],
        [libxml++], [http://libxmlplusplus.sourceforge.net/])
AC_PREREQ([2.59])

AC_CONFIG_SRCDIR([libxml++/libxml++.h])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_HEADERS([config.h libxml++config.h])

AM_INIT_AUTOMAKE([1.9 -Wno-portability dist-bzip2 no-define nostdinc subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])
AM_MAINTAINER_MODE([disable])
AC_ARG_VAR([ACLOCAL_FLAGS], [aclocal flags, e.g. -I <macro dir>])

MM_PREREQ([0.9.8])
MM_INIT_MODULE([libxml++-2.6])

# Copy the mm-common .pl scripts into docs/,
# and use them from there,
# so we can dist them to avoid a tarball-build dependency.
MM_CONFIG_DOCTOOL_DIR([docs])

# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([LIBXMLXX_SO_VERSION], [2:7:0])

AC_PROG_CXX
MM_AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

LT_PREREQ([2.2.6])
LT_INIT([dlopen win32-dll disable-static])

AC_SUBST([LIBXMLXX_MODULES], ['libxml-2.0 >= 2.7.7 glibmm-2.4 >= 2.32.0'])
PKG_CHECK_MODULES([LIBXMLXX], [$LIBXMLXX_MODULES])

AC_LANG([C++])
AC_CHECK_HEADERS([string list map], [], [AC_MSG_ERROR([required headers not found])])
LIBXMLXX_CXX_HAS_EXCEPTION_PTR

MM_ARG_ENABLE_DOCUMENTATION
MM_ARG_WITH_TAGFILE_DOC([libstdc++.tag], [mm-common-libstdc++])
MM_ARG_WITH_TAGFILE_DOC([libsigc++-2.0.tag], [sigc++-2.0])
MM_ARG_WITH_TAGFILE_DOC([glibmm-2.4.tag], [glibmm-2.4])

# Evaluate the --enable-warnings=level option.
MM_ARG_ENABLE_WARNINGS([LIBXMLXX_WXXFLAGS],
        [-Wall],
        [-pedantic -Wall -Wextra -Wformat-security -Wshadow -Wno-long-long],
        [G GLIBMM SIGCXX])

# Offer the ability to omit some API from the library.
MM_ARG_DISABLE_DEPRECATED_API([LIBXMLXX])

AC_PATH_PROG([XMLLINT], [xmllint], [xmllint])
AC_ARG_VAR([XMLLINT], [path to xmllint utility])
AC_PATH_PROGS([DB2LATEX], [db2latex docbook2latex], [db2latex])
AC_ARG_VAR([DB2LATEX], [path to db2latex utility])
MM_CHECK_PERL()

# This is just defined to avoid breaking old code:
AC_DEFINE([LIBXMLCPP_EXCEPTIONS_ENABLED],[1], [This is always set. This is only for backwards compatibility.])

AC_CONFIG_FILES([Makefile
        libxml++/Makefile
        examples/Makefile
        tests/Makefile
        docs/Makefile
        docs/reference/Doxyfile
        MSVC_Net2010/libxml++/libxml++.rc
        libxml++-2.6.pc])

# Copy the generated configuration headers into the MSVC project directories.
AC_CONFIG_COMMANDS([MSVC_Net2010/libxml++/libxml++config.h],
        [cp -f libxml++config.h MSVC_Net2010/libxml++/libxml++config.h])

AC_OUTPUT
