# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Generate pre-generated resources and configuration headers (builds from GIT)
prep-git-build: pkg-ver.mak

libxml++\libxml++.rc: ..\configure.ac libxml++\libxml++.rc.in libxml++\libxml++config.h
	@if not "$(DO_REAL_GEN)" == "1" if exist pkg-ver.mak del pkg-ver.mak
	@if not exist pkg-ver.mak $(MAKE) /f Makefile.vc CFG=$(CFG) prep-git-build
	@if "$(DO_REAL_GEN)" == "1" echo Generating $@...
	@if "$(DO_REAL_GEN)" == "1" copy "$@.in" "$@"
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\@LIBXMLXX_MAJOR_VERSION\@/$(PKG_MAJOR_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\@LIBXMLXX_MINOR_VERSION\@/$(PKG_MINOR_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\@LIBXMLXX_MICRO_VERSION\@/$(PKG_MICRO_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\@PACKAGE_VERSION\@/$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION).$(PKG_MICRO_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\@PACKAGE_NAME\@/libxml++-$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" del $@.bak

# You may change LIBXMLXX_DISABLE_DEPRECATED if you know what you are doing
libxml++\libxml++config.h: ..\configure.ac ..\libxml++config.h.in
	@if not "$(DO_REAL_GEN)" == "1" if exist pkg-ver.mak del pkg-ver.mak
	@if not exist pkg-ver.mak $(MAKE) /f Makefile.vc CFG=$(CFG) prep-git-build
	@if "$(DO_REAL_GEN)" == "1" echo Generating $@...
	@if "$(DO_REAL_GEN)" == "1" copy "..\$(@F).in" "$@"
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\#undef LIBXMLXX_DISABLE_DEPRECATED/\/\* \#undef LIBXMLXX_DISABLE_DEPRECATED \*\//g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\#undef LIBXMLXX_HAVE_EXCEPTION_PTR/\#define LIBXMLXX_HAVE_EXCEPTION_PTR 1/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\#undef LIBXMLXX_MAJOR_VERSION/\#define LIBXMLXX_MAJOR_VERSION $(PKG_MAJOR_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\#undef LIBXMLXX_MINOR_VERSION/\#define LIBXMLXX_MINOR_VERSION $(PKG_MINOR_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" $(PERL) -pi.bak -e "s/\#undef LIBXMLXX_MICRO_VERSION/\#define LIBXMLXX_MICRO_VERSION $(PKG_MICRO_VERSION)/g" $@
	@if "$(DO_REAL_GEN)" == "1" del $@.bak

pkg-ver.mak: ..\configure.ac
	@echo Generating version info Makefile Snippet...
	@$(PERL) -00 -ne "print if /AC_INIT\(/" $** |	\
	$(PERL) -pe "tr/, /\n/s" |	\
	$(PERL) -ne "print if 2 .. 2" |	\
	$(PERL) -ne "print /\[(.*)\]/" > ver.txt
	@echo @echo off>pkg-ver.bat
	@echo.>>pkg-ver.bat
	@echo set /p libxmlxx_ver=^<ver.txt>>pkg-ver.bat
	@echo for /f "tokens=1,2,3 delims=." %%%%a IN ("%libxmlxx_ver%") do (echo PKG_MAJOR_VERSION=%%%%a^& echo PKG_MINOR_VERSION=%%%%b^& echo PKG_MICRO_VERSION=%%%%c)^>$@>>pkg-ver.bat
	@pkg-ver.bat
	@del ver.txt pkg-ver.bat
	$(MAKE) /f Makefile.vc CFG=$(CFG) GENERATE_VERSIONED_FILES=1 libxml++\libxml++.rc
