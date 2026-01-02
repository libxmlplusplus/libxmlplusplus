# NMake Makefile portion for displaying config info

all-build-info:
	@echo.
	@echo ----------
	@echo Build info
	@echo ---------
	@echo Build Type: $(CFG)

help:
	@echo.
	@echo ============================
	@echo Building libxml++ Using NMake
	@echo ============================
	@echo nmake /f Makefile.vc CFG=[release^|debug] ^<option1=xxx option2=xxx^>
	@echo.
	@echo Where:
	@echo ------
	@echo CFG: Required, use CFG=release for an optimized build and CFG=debug
	@echo for a debug build.  PDB files are generated for all builds.
	@echo.
	@echo -----
	@echo A few options are supported here, namely:
	@echo.
	@echo USE_COMPAT_LIBS: Use this if you want to use 'vc150' in your DLL and .lib filenames,
	@echo instead of using the Visual Studio toolset version.
	@echo.
	@echo USE_MESON_LIBS: Use DLLs and LIBs of C++ dependencies that are built with Meson,
	@echo as applicable.
	@echo.
	@echo PERL: Full path to PERL interpreter, if not already in PATH, needed to generate
	@echo libxml++config.h and libxml++.rc if not already present in .\libxml++.
	@echo.
	@echo PREFIX: Base directory to look for dependencies and location for where the
	@echo build results are copied, default is in ..\..\$$(short_vs_ver)\\$$(platform)
	@echo.
	@echo BASE_INCLUDEDIR: Base include directory to look for dependenct headers,
	@echo such as libxml2, default is in $$(PREFIX)\\include.
	@echo.
	@echo BASE_LIBDIR: Base directory to look for dependent libraries, such as libxml2,
	@echo default is in $$(PREFIX)\\lib, and libxml2's dependent libraries if libxml2
	@echo is a static build.
	@echo.
	@echo [DEP]_INCLUDEDIR: Directory to look for headers for [DEP], default is in
	@echo $$(BASE_INCLUDEDIR)\\include. [DEP] includes LIBXML2; the 'libxml2'
	@echo subdirectory of $$(LIBXML2_INCLUDEDIR) will be looked for automatically
	@echo.
	@echo [DEP]_INCLUDEDIR: Directory to look for .lib's for [DEP], default is in
	@echo $$(BASE_INCLUDEDIR)\\lib. [DEP] includes LIBXML2.
	@echo.
	@echo ======
	@echo A 'clean' target is supported to remove all generated files, intermediate
	@echo object files and binaries for the specified configuration.
	@echo.
	@echo An 'install' target is supported to copy the build (DLLs, utility programs,
	@echo LIBs, along with the header files) to appropriate locations under $$(PREFIX).
	@echo.
	@echo A 'tests' target is supported to build the test programs.
	@echo ======
	@echo.
