/* Copyright (C) 2014 The libxml++ development team
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

#ifndef __LIBXMLPP_SCHEMABASE_H
#define __LIBXMLPP_SCHEMABASE_H

#include <libxml++/noncopyable.h>

namespace Glib
{
class ustring;
}

namespace xmlpp
{
class Document;

/** Base class for schemas, used for validation of XML files.
 *
 * @newin{2,38}
 */
class SchemaBase : NonCopyable
{
public:
  SchemaBase();
  ~SchemaBase() override;

  /** Parse a schema definition file.
   * If another schema has been parsed before, that schema is replaced by the new one.
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  virtual void parse_file(const Glib::ustring& filename) = 0;

  /** Parse a schema definition from a string.
   * If another schema has been parsed before, that schema is replaced by the new one.
   * @param contents The schema definition as a string.
   * @throws xmlpp::parse_error
   */
  virtual void parse_memory(const Glib::ustring& contents) = 0;

  /** Parse a schema definition from a document.
   * If another schema has been parsed before, that schema is replaced by the new one.
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  virtual void parse_document(const Document* document) = 0;
};

} // namespace xmlpp

#endif //__LIBXMLPP_SCHEMABASE_H
