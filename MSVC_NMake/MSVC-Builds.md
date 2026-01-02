Building libxml++ with Visual Studio 2017 or later
-

* You will need Visual Studio 2017 (MSVC 15.7.x) or later, as `C++-17` is
required and only 15.7.x or later had adequate `C++-17` support.
* Install libxml2 from https://xmlsoft.org/, either via Windows binaries or
building from source, using NMake or CMake.  It is strongly recommended, if
building libxml2 from source, that Visual Studio 2015 or later is used.
* For locating libxml2, do one of the following:
  * Add libxml2's include path to `%INCLUDE%` and its library path to `%LIB%`, for NMake
  or Meson
  * See the following NMake and Meson sections for finding the libxml2 and dependent libraries.
  * Or, Place the libxml2 installation like the following, and pass in `PREFIX=$(prefix)` for NMake:
```
$(prefix)\include
   |
   --libxml2\libxml\*.h

$(prefix)\lib
   |
   --libxml2.lib
   |
   --(.lib's of libraries that libxml2 depends on, such as ZLib, liblzma, if libxml2 is a static build)
```

* You need libxml2's DLL and all of its dependent DLLs in `%PATH%` if linking
against a DLL build of libxml2 in order to run the tests.

* Notice that for the built DLL and .lib the Visual Studio version is no
longer `vc$(VSVER)0`, but is named like the following (Visual Studio version),
to follow what is done in other C++ libraries such as Boost (use
`-Dmsvc14x-parallel-installable=false` to the Meson options or pass
`USE_COMPAT_LIBS=1` in the NMake commandline to override, recommended only if
rebuilding items using libxml++ is inconvenient):
  * NMake:
    * 2017: `VSVER=15`, `xml++-vc141-5_0.[dll|pdb|lib]`
    * 2019: `VSVER=16`, `xml++-vc142-5_0.[dll|pdb|lib]`
    * 2022: `VSVER=17`, `xml++-vc143-5_0.[dll|pdb|lib]`
    * 2026: `VSVER=18`, `xml++-vc145-5_0.[dll|pdb|lib]`
  * Meson:
    * 2017: `VSVER=15`, `xml++-vc141-5.0-1[dll|pdb]`, `xml++-vc141-5.0.lib`
    * 2019: `VSVER=16`, `xml++-vc142-5.0-1[dll|pdb]`, `xml++-vc142-5.0.lib`
    * 2022: `VSVER=17`, `xml++-vc143-5.0-1[dll|pdb]`, `xml++-vc143-5.0.lib`
    * 2026: `VSVER=18`, `xml++-vc145-5.0-1[dll|pdb]`, `xml++-vc143-5.0.lib`


### NMake Builds
* Note that `$(Platform)` refers to the following:
  * `$(Platform)`
    * AMD64/x86_64/x64: `x64`
    * ARM64/aarch64: `arm64`
    * x86/i686: `Win32`

* Run the following:
```
cd $(srcroot)\MSVC_NMake
# Run "nmake /f Makefile.vc" to see to see what options are supported by
# the NMake Makefiles
# BASE_INCLUDEDIR is by default $(PREFIX)\include, and BASE_LIBDIR is by default
# $(PREFIX)\lib. PREFIX is by default $(srcroot)\..\vs$(VSVER\$(Platform).
# $(some_dir_1) and $(some_dir_2) refer to the sample layout listed above.
# LIBXML2_INCLUDEDIR is by default $(BASE_INCLUDEDIR); the libxml2 headers will
# be searched for in $(LIBXML2_INCLUDEDIR)\libxml2.
# LIBXML2_LIBDIR is by default $(BASE_LIBDIR); if libxml2 is a static build its
# dependent libraries should be placed in $(BASE_LIBDIR)

nmake /f Makefile.vc CFG=[debug|release] [other_options]
```
* The following targets are supported (only DLL builds are supported out of the
box with the NMake Makefiles):
  * `all`, `libxmlxx_examples` (or nothing passed in): The libxml++ library as
a DLL with the example programs
  * `tests`: The libxml++ library as a DLL with the test programs

### Meson builds
Please also see [README.md](../README.md) for further details.

For Visual Studio builds, it is also recommended that CMake is installed and can
be found in `%PATH%`, so that it can help with finding an installed libxml2, or
build libxml2 alongside with libxml++ if it is not previously installed. If
libxml2 is installed, you will most probably need to add its include directory
to `%INCLUDE%` and its library path to `%LIB%`, as described earlier, or use
`--cmake-prefix-path` to point to libxml2's installation directory or
`--pkg-config-path` to point to where your libxml2's pkg-config files can be found.


Cedric Gustin
01/26/2005

Armin Burgmeier
10/03/2010

Chun-wei Fan
06/12/2020, updated 07/05/2023
