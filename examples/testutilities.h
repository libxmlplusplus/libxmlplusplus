/* Copyright (C) 2013 The libxml++ development team
 *
 * This file is part of libxml++.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <glibmm/ustring.h>
#include <iostream>

// CatchConvertError is identical to Glib::ustring, except that
// std::ostream& operator<<(std::ostream&, const CatchConvertError&)
// catches Glib::ConvertError, and prints the exception message.
//
// If a printed string contains characters that don't exist in the global
// locale's character set, an exception is thrown, but it's caught locally,
// and the program continues.
// This is particularly useful when the example programs are run by 'make check'.
// If the user-specified locale's character set does not contain all characters
// in the Glib::ustring, operator<<(std::ostream&, const Glib::ustring&) throws
// a Glib::ConvertError exception. That exception is not the result of an error
// in libxml++ or libxml2.
// See also https://bugzilla.gnome.org/show_bug.cgi?id=702136

class CatchConvertError : public Glib::ustring
{
public:
  explicit CatchConvertError(const Glib::ustring& str)
  : Glib::ustring(str)
  { }
};

std::ostream& operator<<(std::ostream& os, const CatchConvertError& utf8_string);

