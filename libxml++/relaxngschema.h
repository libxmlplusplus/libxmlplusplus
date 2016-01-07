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
 *
 * This file was adapted from schema.h by Fugro Intersite B.V./Tjalling Hattink
 */

#ifndef __LIBXMLPP_RELAXNGSCHEMA_H
#define __LIBXMLPP_RELAXNGSCHEMA_H

#include <libxml++/schemabase.h>
#include <libxml++/document.h>
#include <memory> // std::unique_ptr

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlRelaxNG;
  struct _xmlRelaxNGParserCtxt;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Represents a RelaxNG schema for validating XML files.
 * RelaxNG = REgular LAnguage for XML Next Generation
 *
 * @newin{2,38}
 */
class RelaxNGSchema : public SchemaBase
{
public:
  RelaxNGSchema();

  /** Create a schema from the underlying libxml schema element.
   * @param schema A pointer to the libxml schema element. The RelaxNGSchema takes
   *               ownership of the _xmlRelaxNG. The caller must not deallocate it.
   */
  explicit RelaxNGSchema(_xmlRelaxNG* schema);

  /** Create a schema from a schema definition file.
   * The schema must be defined with XML syntax (.rng file). The compact syntax
   * (.rnc file) is not supported.
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  explicit RelaxNGSchema(const std::string& filename);

  /** Create a schema from an XML document.
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  explicit RelaxNGSchema(const Document* document);

  ~RelaxNGSchema() override;

  /** Parse a schema definition file.
   * The schema must be defined with XML syntax (.rng file). The compact syntax
   * (.rnc file) is not supported.
   *
   * If another schema has been parsed before, that schema is replaced by the new one.
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  void parse_file(const std::string& filename) override;

  /** Parse a schema definition from a string.
   * The schema must be defined with XML syntax. The compact syntax is not supported.
   *
   * If another schema has been parsed before, that schema is replaced by the new one.
   * @param contents The schema definition as a string.
   * @throws xmlpp::parse_error
   */
  void parse_memory(const Glib::ustring& contents) override;

  /** Parse a schema definition from a document.
   * If another schema has been parsed before, that schema is replaced by the new one.
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  void parse_document(const Document* document) override;

  /** Access the underlying libxml implementation. */
  _xmlRelaxNG* cobj() noexcept;

  /** Access the underlying libxml implementation. */
  const _xmlRelaxNG* cobj() const noexcept;

protected:
  void release_underlying();
  void parse_context(_xmlRelaxNGParserCtxt* context);

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_RELAXNGSCHEMA_H
