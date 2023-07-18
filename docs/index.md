---
layout: default
title: libxml++
---
## Introduction

libxml++ is a C++ wrapper for the [libxml2](http://www.xmlsoft.org) XML parser library.

## License

Libxml++ is released under the [LGPL](https://www.gnu.org/licenses/lgpl.html) version 2 or above.

## ABI Versions

There are several ABI (Application Binary Interface) versions of libxml++.
They can be installed in parallel.

- libxml++-2.6: Old ABI, not recommended for new applications.
  Uses Glib::ustring from the glibmm-2.4 ABI.

- libxml++-3.0: Uses Glib::ustring from the glibmm-2.4 ABI,
  therefore has methods that handle UTF-8 characters rather than raw bytes.

- libxml++-4.0: Uses Glib::ustring from the newer glibmm-2.68 ABI,
  therefore has methods that handle UTF-8 characters rather than raw bytes.

- libxml++-5.0: Does not depend on glibmm, is not as good at handling UTF-8 strings,
  has fewer dependencies.

The APIs of libxml++-3.0, libxml++-4.0 and libxml++-5.0 are very similar.
The only significant difference is that libxml++-5.0 uses xmlpp::ustring
instead of Glib::ustring.

In a new application, choose between libxml++-4.0 with UTF-8 support and libxml++-5.0
with fewer dependencies. Choose libxml++-3.0 only if you want UTF-8 support but
don't have access to the newer glibmm-2.68 ABI (glibmm version 2.68.0 or higher).

## Download

You can download libxml++ from [GitHub releases](https://github.com/libxmlplusplus/libxmlplusplus/releases/)
or the [GNOME download site](https://download.gnome.org/sources/libxml++/).

## Required Libraries

* [libxml2](http://www.xmlsoft.org/)
* libxml++-2.6 and libxml++-3.0: [glibmm-2.4](https://developer-old.gnome.org/glibmm/2.66/)
* libxml++-4.0: [glibmm-2.68](https://gnome.pages.gitlab.gnome.org/glibmm/)

## Documentation

### libxml++-5.0

The short [manual](manual/html/index.html) gives an overview of the libxml++ API.
You can also browse online the [reference documentation](reference/html/index.html).

### Other ABI Versions

libxml++-2.6: [manual](https://developer-old.gnome.org/libxml++-tutorial/2.42/)
and [reference documentation](https://developer-old.gnome.org/libxml++/2.42/)

libxml++-3.0: [manual](https://developer-old.gnome.org/libxml++-tutorial/3.2/)
and [reference documentation](https://developer-old.gnome.org/libxml++/3.2/)

libxml++-4.0: [manual](https://developer-old.gnome.org/libxml++-tutorial/4.0/)
and [reference documentation](https://developer-old.gnome.org/libxml++/4.0)

This documentation is frozen on the web. It does not document the latest releases.
If you want newer documentation, you can download a tarball from
[GitHub releases](https://github.com/libxmlplusplus/libxmlplusplus/releases/)
or the [GNOME download site](https://download.gnome.org/sources/libxml++/),
extract it, and view the documentation at *untracked/docs/*.

## Development

Libxml++ uses the GitHub [git repository](https://github.com/libxmlplusplus/libxmlplusplus).

Bugs and patches are tracked in GitHub's [issue tracker](https://github.com/libxmlplusplus/libxmlplusplus/issues)
and [pull request tracker](https://github.com/libxmlplusplus/libxmlplusplus/pulls).

## Discussion

You can discuss libxml++ questions on GNOME's [Discourse](https://discourse.gnome.org)
instance, under the [Platform/Language bindings](https://discourse.gnome.org/c/platform/language-bindings)
category with a _cplusplus_ tag.

There is an old mailing list, which is now closed for new posts.
There is an archive of messages [here](https://mail.gnome.org/archives/libxmlplusplus-list/).
Please do not email the developers directly.
