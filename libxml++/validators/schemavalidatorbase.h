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

#ifndef __LIBXMLPP_VALIDATOR_SCHEMAVALIDATORBASE_H
#define __LIBXMLPP_VALIDATOR_SCHEMAVALIDATORBASE_H

#include <libxml++/validators/validator.h>

namespace xmlpp
{
class Document;

/** Base class for schema validators.
 *
 * @newin{2,38}
 */
class SchemaValidatorBase : public Validator
{
public:
  LIBXMLPP_API SchemaValidatorBase() noexcept;
  LIBXMLPP_API ~SchemaValidatorBase() override;

  /** Parse a schema definition file.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  LIBXMLPP_API
  void parse_file(const std::string& filename) override = 0;

  /** Parse a schema definition from a string.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param contents The schema definition as a string.
   * @throws xmlpp::parse_error
   */
  LIBXMLPP_API
  void parse_memory(const ustring& contents) override = 0;

  /** Parse a schema definition from a document.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  LIBXMLPP_API
  virtual void parse_document(const Document* document) = 0;

  /** Test whether a schema has been parsed.
   * For instance
   * @code
   * if (validator)
   *   do_something();
   * @endcode
   */
  LIBXMLPP_API
  explicit operator bool() const noexcept override = 0;

  /** Validate a document, using a previously parsed schema.
   * @param document Pointer to the document.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  LIBXMLPP_API
  void validate(const Document* document) override = 0;

  /** Validate an XML file, using a previously parsed schema.
   * @param filename The URL of the XML file.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  LIBXMLPP_API
  virtual void validate(const std::string& filename) = 0;

protected:
  LIBXMLPP_API
  void initialize_context() override;
  LIBXMLPP_API
  void release_underlying() override;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_SCHEMAVALIDATORBASE_H
