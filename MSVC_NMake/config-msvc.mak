# NMake Makefile portion for enabling features for Windows builds

# These are the base minimum libraries required for building libxml++.
!ifndef BASE_INCLUDEDIR
BASE_INCLUDEDIR = $(PREFIX)\include
!endif
!ifndef BASE_LIBDIR
BASE_LIBDIR = $(PREFIX)\lib
!endif

# Please do not change anything beneath this line unless maintaining the NMake Makefiles
LIBXMLXX_MAJOR_VERSION = 5
LIBXMLXX_MINOR_VERSION = 0

OUTDIR = vs$(VSVER)\$(CFG)\$(PLAT)
DEPS_MKFILE = deps-vs$(VSVER)-$(PLAT)-$(CFG).mak

!if "$(CFG)" == "debug" || "$(CFG)" == "Debug"
DEBUG_SUFFIX = -d
!else
DEBUG_SUFFIX =
!endif

# Gather up dependencies for their include directories and lib/bin dirs.
!if [for %t in (LIBXML2) do @(echo !ifndef %t_INCLUDEDIR>>$(DEPS_MKFILE) & echo %t_INCLUDEDIR=^$^(BASE_INCLUDEDIR^)>>$(DEPS_MKFILE) & echo !endif>>$(DEPS_MKFILE))]
!endif
!if [for %t in (LIBXML2) do @(echo !ifndef %t_LIBDIR>>$(DEPS_MKFILE) & echo %t_LIBDIR=^$^(BASE_LIBDIR^)>>$(DEPS_MKFILE) & echo !endif>>$(DEPS_MKFILE))]
!endif

!include $(DEPS_MKFILE)

!if [del /f/q $(DEPS_MKFILE)]
!endif

DEPS_INCLUDES = /I$(LIBXML2_INCLUDEDIR)\libxml2 /I$(BASE_INCLUDEDIR)

LIBXMLXX_BASE_CFLAGS =	\
	/wd4244 /wd4101

LIBXMLXX_EXTRA_INCLUDES =	\
	/I$(INCLUDEDIR)\libxml2	\
	/I$(INCLUDEDIR)

LIBXMLXX_INCLUDES =	\
	/I.\libxml++ /I..	\
	$(DEPS_INCLUDES)

LIBXMLXX_CFLAGS = /DLIBXMLPP_BUILD $(LIBXMLXX_BASE_CFLAGS)
LIBXMLXX_EX_CFLAGS = $(LIBXMLXX_BASE_CFLAGS)

# We build xml++-vc$(VSVER_LIB)-$(LIBXMLXX_MAJOR_VERSION)_$(LIBXMLXX_MINOR_VERSION).dll or
#          xml++-vc$(VSVER_LIB)-d-$(LIBXMLXX_MAJOR_VERSION)_$(LIBXMLXX_MINOR_VERSION).dll at least

!if "$(USE_COMPAT_LIBS)" != ""
VSVER_LIB = $(VSVER)0
MSVC_VSVER_LIB =
!else
VSVER_LIB = $(PDBVER)$(VSVER_SUFFIX)
MSVC_VSVER_LIB = -vc$(VSVER_LIB)
!endif

!ifdef USE_MESON_LIBS
LIBXMLXX_LIBNAME = xml++$(MSVC_VSVER_LIB)-$(LIBXMLXX_MAJOR_VERSION).$(LIBXMLXX_MINOR_VERSION)
LIBXMLXX_DLLNAME = $(LIBXMLXX_LIBNAME)-1
!else
LIBXMLXX_LIBNAME = xml++-vc$(VSVER_LIB)$(DEBUG_SUFFIX)-$(LIBXMLXX_MAJOR_VERSION)_$(LIBXMLXX_MINOR_VERSION)
LIBXMLXX_DLLNAME = $(LIBXMLXX_LIBNAME)
!endif

LIBXMLXX_DLL = $(OUTDIR)\$(LIBXMLXX_DLLNAME).dll
LIBXMLXX_LIB = $(OUTDIR)\$(LIBXMLXX_LIBNAME).lib

LIBXML2_LIBS = libxml2.lib

DEP_LDFLAGS = /libpath:$(BASE_LIBDIR) $(LIBXML2_LIBS)
!if "$(LIBXML2_LIBDIR)" != "$(BASE_LIBDIR)"
DEP_LDFLAGS = /libpath:$(LIBXML2_LIBDIR) $(DEP_LDFLAGS)
!endif
