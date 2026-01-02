Building libxml++ with Visual Studio 2017 or later
-

* You will need Visual Studio 2017 (MSVC 15.0) or later, as unfortunately, 
due to a compiler issue that forbid using `explicit` with `virtual` (and the 
likes), Visual Studio 2015 and earlier are not supported, even though only 
C++-11 is required.

### Notes on dependencies
* Install libxml2 from https://xmlsoft.org/, either via Windows binaries or
building from source, using NMake or CMake.  It is strongly recommended, if
building libxml2 from source, that Visual Studio 2015 or later is used.
* Install the latest Win32 GTK+ Development files from ftp://ftp.gnome.org/pub/GNOME/binaries/win32/gtk+/, or build GLib from source
or obtain GLib with vcpkg.
* You will need the `C++-11` versions of glibmm and libsigc++, which are
glibmm up to glibmm-2.66.x and libsigc++-2.x.
* For locating libxml2, GLib, glibmm and libsigc++ (noted as [dep] below), do one of the following:
  * Add libxml2's include path to `%INCLUDE%` and its library path to `%LIB%`, for NMake
  or Meson
  * See the following NMake and Meson sections for the dependencies listed above and their dependent libraries.
  * Or, Place the libxml2 installation like the following, and pass in `PREFIX=$(prefix)` for NMake:
```
$(prefix)\include
   |
   --giomm-2.4\*
   |
   --glibmm-2.4\*
   |
   --glib-2.0\*
   |
   --sigc++-2.0\*
   |
   --libxml2\libxml\*.h
   |
   --(headers of libraries libxml2/GLib depends on, such as ZLib, liblzma...)

$(prefix)\lib
   |
   # glib's .pc files, and glibmm/libsigc++'s .pc files if using Meson
   --pkgconfig\*.pc
   |
   --[gio|glib]mm-2.4\include\[gio|glib]mmconfig.h
   |
   --sigc++-2.0\include\sigc++config.h
   |
   --[gio|glib]mm-vc14x-[2_4|2.4].lib
   |
   --sigc++-[vc140-2_0|2.0].lib
   |
   --[gio|gobject|gmodule|glib]-2.0.lib
   |
   --libxml2.lib
   |
   --(.lib's of libraries that libxml2/GLib depends on, such as ZLib, liblzma, if libxml2 is a static build)
```

* You need libxml2's DLL and all of its dependent DLLs in `%PATH%` if linking
against a DLL build of libxml2, and the glibmm DLLs and their dependent DLLs
in `%PATH%` in order to run the tests.

* Notice that for the built DLL and .lib the Visual Studio version is no
longer `vc$(VSVER)0`, but is named like the following (Visual Studio version),
to follow what is done in other C++ libraries such as Boost (use
`-Dmsvc14x-parallel-installable=false` to the Meson options or pass
`USE_COMPAT_LIBS=1` in the NMake commandline to override, recommended only if
rebuilding items using libxml++ is inconvenient):
  * NMake:
    * 2017: `VSVER=15`, `xml++-vc141-3_0.[dll|pdb|lib]`
    * 2019: `VSVER=16`, `xml++-vc142-3_0.[dll|pdb|lib]`
    * 2022: `VSVER=17`, `xml++-vc143-3_0.[dll|pdb|lib]`
    * 2026: `VSVER=18`, `xml++-vc145-3_0.[dll|pdb|lib]`
  * Meson:
    * 2017: `VSVER=15`, `xml++-vc141-3.0-1[dll|pdb]`, `xml++-vc141-3.0.lib`
    * 2019: `VSVER=16`, `xml++-vc142-3.0-1[dll|pdb]`, `xml++-vc142-3.0.lib`
    * 2022: `VSVER=17`, `xml++-vc143-3.0-1[dll|pdb]`, `xml++-vc143-3.0.lib`
    * 2026: `VSVER=18`, `xml++-vc145-3.0-1[dll|pdb]`, `xml++-vc145-3.0.lib`


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

* For Visual Studio builds, it is also recommended that CMake is installed and can
be found in `%PATH%`, so that it can help with finding an installed libxml2, or
build libxml2 alongside with libxml++ if it is not previously installed. If
libxml2 is installed, you will most probably need to add its include directory
to `%INCLUDE%` and its library path to `%LIB%`, as described earlier, or use
`--cmake-prefix-path` to point to libxml2's installation directory or
`--pkg-config-path` to point to where your libxml2's pkg-config files can be found.

* If libxml2 is installed, you will most probably need to add its include
directory to `%INCLUDE%` and its library path to `%LIB%`, as described earlier.

* glibmm and libsigc++ should also be built with Meson, so that the pkg-config
files for them, which are required, are generated, and the locations of these
pkg-config files should be set by `--pkg-config-path=...` if `pkg-config` (or
compatible tool) cannot locate them automatically.


Cedric Gustin
01/26/2005

Armin Burgmeier
10/03/2010

Chun-wei Fan
06/12/2020, updated 07/05/2023
