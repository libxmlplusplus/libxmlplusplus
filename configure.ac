## This file is part of libxml++.

AC_INIT([libxml++], [5.2.0],
        [https://github.com/libxmlplusplus/libxmlplusplus/issues],
        [libxml++], [https://libxmlplusplus.github.io/libxmlplusplus/])
AC_PREREQ([2.59])

AC_CONFIG_SRCDIR([libxml++/libxml++.h])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_HEADERS([config.h libxml++config.h])

AM_INIT_AUTOMAKE([1.9 -Wno-portability dist-bzip2 no-define nostdinc subdir-objects foreign])
# Support silent build rules.
# Disable by either passing --disable-silent-rules to configure or passing V=1 to make.
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE([disable])
AC_ARG_VAR([ACLOCAL_FLAGS], [aclocal flags, e.g. -I <macro dir>])

MM_PREREQ([0.9.10])
MM_INIT_MODULE([libxml++-5.0])

# Tell mm-common-prepare to copy the mm-common .pl scripts
# and some other files into docs/, and use them from there,
# so we can dist them to avoid a tarball-build dependency.
MM_CONFIG_DOCTOOL_DIR([docs])

# Library version (so version) [current:revision:age]
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([LIBXMLXX_SO_VERSION], [1:0:0])

AC_PROG_CXX
MM_AX_CXX_COMPILE_STDCXX([17], [noext], [mandatory])

LT_PREREQ([2.2.6])
LT_INIT([dlopen win32-dll disable-static])

AC_SUBST([LIBXMLXX_MODULES], ['libxml-2.0 >= 2.7.7'])
AC_SUBST([LIBXML2_LIB_NO_PKGCONFIG], [''])
AC_SUBST([MSVC_TOOLSET_VER], [''])
AC_SUBST(DOXYGEN_HAVE_DOT, [YES])
AM_SUBST_NOTMAKE(DOXYGEN_HAVE_DOT)
PKG_CHECK_MODULES([LIBXMLXX], [$LIBXMLXX_MODULES])

AC_LANG([C++])
AC_CHECK_HEADERS([string list map], [], [AC_MSG_ERROR([required headers not found])])
LIBXMLXX_CXX_HAS_EXCEPTION_PTR

MM_ARG_ENABLE_DOCUMENTATION
MM_ARG_WITH_TAGFILE_DOC([libstdc++.tag], [mm-common-libstdc++])

# Evaluate the --enable-warnings=level option.
MM_ARG_ENABLE_WARNINGS([LIBXMLXX_WXXFLAGS],
        [-Wall],
        [-pedantic -Wall -Wextra -Wformat-security -Wsuggest-override -Wshadow -Wno-long-long])

# Offer the ability to omit some API from the library.
MM_ARG_DISABLE_DEPRECATED_API([LIBXMLXX])

AC_PATH_PROG([XMLLINT], [xmllint], [xmllint])
AC_ARG_VAR([XMLLINT], [path to xmllint utility])
AC_PATH_PROGS([DB2LATEX], [db2latex docbook2latex], [db2latex])
AC_ARG_VAR([DB2LATEX], [path to db2latex utility])
MM_CHECK_PERL()

AC_CONFIG_FILES([Makefile
        libxml++/Makefile
        examples/Makefile
        tests/Makefile
        docs/Makefile
        docs/reference/Doxyfile
        MSVC_NMake/libxml++/libxml++.rc
        ${LIBXMLXX_MODULE_NAME}.pc:libxml++.pc.in])

# Copy the generated configuration headers into the MSVC project directories.
AC_CONFIG_COMMANDS([MSVC_NMake/libxml++/libxml++config.h],
        [cp -f libxml++config.h MSVC_NMake/libxml++/libxml++config.h])

AC_OUTPUT
