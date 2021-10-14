---
layout: default
title: libxml++
---
# libxml++

## Introduction

libxml++ is a C++ wrapper for the [libxml2](http://www.xmlsoft.org) XML parser library.

## License

Libxml++ is released under the [LGPL](http://www.gnu.org/licenses/lgpl.html#TOC1) version 2 or above

## ABI Versions

There are several ABI (Application Binary Interface) versions of libxml++.
They can be installed in parallel.

libxml++-2.6: Old ABI, not recommended for new applications. Uses Glib::ustring from the glibmm-2.4 ABI.

libxml++-3.0: Uses Glib::ustring from the glibmm-2.4 ABI, therefore has methods that handle UTF-8 characters rather than raw bytes.

libxml++-4.0: Uses Glib::ustring from the newer glibmm-2.68 ABI, therefore has methods that handle UTF-8 characters rather than raw bytes.

libxml++-5.0: Does not depend on glibmm, is not as good at handling UTF-8 strings, but has fewer dependencies.

## Download

You can download libxml++ from [here](https://download.gnome.org/sources/libxml++/).

## Required Libraries

* [libxml2](http://www.xmlsoft.org/)
* libxml++-2.6 and libxml++-3.0: [glibmm-2.4](https://developer.gnome.org/glibmm/2.64/)
* libxml++-4.0: [glibmm-2.68](https://developer.gnome.org/glibmm/2.68/)

## Documentation

The short [manual](https://developer-old.gnome.org/libxml++-tutorial/) gives an overview of the libxml++ API.
You can also browse online the [reference documentation](https://developer-old.gnome.org/libxml++/).

## Development

Libxml++ uses the GitHub [git repository](https://github.com/libxmlplusplus/libxmlplusplus).

Bugs and patches are tracked in GitHub's [issue tracker](https://github.com/libxmlplusplus/libxmlplusplus/issues).

## Contact

Please use the [libxmlplusplus-list mailing list](http://mail.gnome.org/mailman/listinfo/libxmlplusplus-list) or GitHub. Please do not just email the developers directly.
