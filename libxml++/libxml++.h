/* libxml++.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLCPP_H
#define __LIBXMLCPP_H

/** @mainpage libxml++ Reference Manual
 *
 * @section description Description
 *
 * libxml++ is a C++ wrapper for the <a href="http://xmlsoft.org/">libxml2</a> XML parser and builder library. It presents a
 * simple C++-like API that can achieve common tasks with less code.
 *
 * See also the <a href="http://developer.gnome.org/libxml++-tutorial/stable/">libxml++ Tutorial</a> and the <a href="http://libxmlplusplus.sourceforge.net/">libxml++ website</a>.
 *
 * @section features Features
 *
 * - xmlpp::DomParser: A DOM-style parser.
 * - xmlpp::SaxParser: A SAX-style parser.
 * - xmlpp::TextReader: An XmlTextReader-style parser.
 * - A hierarchy of xmlpp::Node classes.
 *
 * @section basics Basic Usage
 *
 * Include the libxml++ header:
 * @code
 * #include <libxml++/libxml++.h>
 * @endcode
 * (You may include individual headers, such as libxml++/document.h instead.)
 *
 * If your source file is @c program.cc, you can compile it with:
 * @code
 * g++ program.cc -o program `pkg-config --cflags --libs libxml++-3.0`
 * @endcode
 * If your version of g++ is not C++11-compliant be default,
 * add the @c -std=c++11 or @c -std=c++0x option.
 *
 * Alternatively, if using autoconf, use the following in @c configure.ac:
 * @code
 * PKG_CHECK_MODULES([LIBXMLXX], [libxml++-3.0])
 * @endcode
 * Then use the generated @c LIBXMLXX_CFLAGS and @c LIBXMLXX_LIBS variables in
 * the project @c Makefile.am files. For example:
 * @code
 * program_CPPFLAGS = $(LIBXMLXX_CFLAGS)
 * program_LDADD = $(LIBXMLXX_LIBS)
 * @endcode
 */
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/exceptions/parse_error.h>
#include <libxml++/parsers/domparser.h>
#include <libxml++/parsers/saxparser.h>
#include <libxml++/parsers/textreader.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/cdatanode.h>
#include <libxml++/nodes/commentnode.h>
#include <libxml++/nodes/element.h>
#include <libxml++/nodes/entitydeclaration.h>
#include <libxml++/nodes/entityreference.h>
#include <libxml++/nodes/processinginstructionnode.h>
#include <libxml++/nodes/textnode.h>
#include <libxml++/nodes/xincludeend.h>
#include <libxml++/nodes/xincludestart.h>
#include <libxml++/attribute.h>
#include <libxml++/attributedeclaration.h>
#include <libxml++/attributenode.h>
#include <libxml++/document.h>
#include <libxml++/relaxngschema.h>
#include <libxml++/xsdschema.h>
#include <libxml++/validators/validator.h>
#include <libxml++/validators/dtdvalidator.h>
#include <libxml++/validators/relaxngvalidator.h>
#include <libxml++/validators/xsdvalidator.h>

#endif //__LIBXMLCPP_H
