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
 * This file was adapted from schemavalidator.h by Fugro Intersite B.V./Tjalling Hattink
 */

#ifndef __LIBXMLPP_VALIDATOR_RELAXNGVALIDATOR_H
#define __LIBXMLPP_VALIDATOR_RELAXNGVALIDATOR_H

#include <libxml++/validators/schemavalidatorbase.h>
#include <memory> // std::unique_ptr

namespace Glib
{
class ustring;
}

namespace xmlpp
{
class Document;
class RelaxNGSchema;

/** RelaxNG schema validator.
 * RelaxNG = REgular LAnguage for XML Next Generation
 *
 * @newin{2,38}
 */
class RelaxNGValidator : public SchemaValidatorBase
{
public:
  RelaxNGValidator();

  /** Create a validator and parse a schema definition file.
   * The schema must be defined with XML syntax (.rng file). The compact syntax
   * (.rnc file) is not supported.
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  explicit RelaxNGValidator(const std::string& filename);

  /** Create a validator and parse a schema definition document.
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  explicit RelaxNGValidator(const Document* document);

  /** Create a validator.
   * @param schema A pointer to the schema to use when validating XML documents.
   * @param take_ownership If <tt>true</tt>, the validator takes ownership of
   *        the schema. The caller must not delete it.<br>
   *        If <tt>false</tt>, the validator does not take ownership of the schema.
   *        The caller must guarantee that the schema exists as long as the
   *        validator keeps a pointer to it. The caller is responsible for
   *        deleting the schema when it's no longer needed.
   */
  explicit RelaxNGValidator(RelaxNGSchema* schema, bool take_ownership);

  ~RelaxNGValidator() override;

  /** Parse a schema definition file.
   * The schema must be defined with XML syntax (.rng file). The compact syntax
   * (.rnc file) is not supported.
   *
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  void parse_file(const std::string& filename) override;

  /** Parse a schema definition from a string.
   * The schema must be defined with XML syntax. The compact syntax is not supported.
   *
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param contents The schema definition as a string.
   * @throws xmlpp::parse_error
   */
  void parse_memory(const Glib::ustring& contents) override;

  /** Parse a schema definition from a document.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  void parse_document(const Document* document) override;

  /** Set a schema.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param schema A pointer to the schema to use when validating XML documents.
   * @param take_ownership If <tt>true</tt>, the validator takes ownership of
   *        the schema. The caller must not delete it.<br>
   *        If <tt>false</tt>, the validator does not take ownership of the schema.
   *        The caller must guarantee that the schema exists as long as the
   *        validator keeps a pointer to it. The caller is responsible for
   *        deleting the schema when it's no longer needed.
   */
  void set_schema(RelaxNGSchema* schema, bool take_ownership);

  /** Test whether a schema has been parsed.
   * For instance
   * @code
   * if (relaxng_validator)
   *   do_something();
   * @endcode
   */
  explicit operator bool() const noexcept override;

  /** Get the schema.
   * @returns A pointer to the schema, or <tt>nullptr</tt>.
   */
  RelaxNGSchema* get_schema() noexcept;

  /** Get the schema.
   * @returns A pointer to the schema, or <tt>nullptr</tt>.
   */
  const RelaxNGSchema* get_schema() const noexcept;

  /** Validate a document, using a previously parsed schema.
   * @param document Pointer to the document.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  void validate(const Document* document) override;

  /** Validate an XML file, using a previously parsed schema.
   * @param filename The URL of the XML file.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  void validate(const std::string& filename) override;

protected:
  void initialize_context() override;
  void release_underlying() override;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_RELAXNGVALIDATOR_H
