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

#ifndef __LIBXMLPP_VALIDATOR_XSDVALIDATOR_H
#define __LIBXMLPP_VALIDATOR_XSDVALIDATOR_H

#include <libxml++/validators/schemavalidatorbase.h>
#include <memory> // std::auto_ptr

namespace Glib
{
class ustring;
}

namespace xmlpp
{
class Document;
class XsdSchema;

/** XSD schema validator.
 * XSD = XML %Schema Definition, a.k.a. XML %Schema or W3C XML %Schema
 *
 * @newin{2,38}
 */
class XsdValidator : public SchemaValidatorBase
{
public:
  XsdValidator();

  /** Create a validator and parse a schema definition file.
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  explicit XsdValidator(const Glib::ustring& filename);

  /** Create a validator and parse a schema definition document.
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  explicit XsdValidator(const Document* document);

  /** Create a validator.
   * @param schema A pointer to the schema to use when validating XML documents.
   * @param take_ownership If <tt>true</tt>, the validator takes ownership of
   *        the schema. The caller must not delete it.<br>
   *        If <tt>false</tt>, the validator does not take ownership of the schema.
   *        The caller must guarantee that the schema exists as long as the
   *        validator keeps a pointer to it. The caller is responsible for
   *        deleting the schema when it's no longer needed.
   */
  explicit XsdValidator(XsdSchema* schema, bool take_ownership);

  ~XsdValidator() override;

  //TODO: Remove virtuals when we can break ABI,
  //or really put these in the base class.

  /** Parse a schema definition file.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param filename The URL of the schema.
   * @throws xmlpp::parse_error
   */
  virtual void parse_file(const Glib::ustring& filename);

  /** Parse a schema definition from a string.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param contents The schema definition as a string.
   * @throws xmlpp::parse_error
   */
  virtual void parse_memory(const Glib::ustring& contents);

  /** Parse a schema definition from a document.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param document A preparsed document tree, containing the schema definition.
   * @throws xmlpp::parse_error
   */
  virtual void parse_document(const Document* document);

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
  void set_schema(XsdSchema* schema, bool take_ownership);

  /** Test whether a schema has been parsed.
   * For instance
   * @code
   * if (xsd_validator)
   *   do_something();
   * @endcode
   */
  virtual operator BoolExpr() const;

  /** Get the schema.
   * @returns A pointer to the schema, or <tt>nullptr</tt>.
   */
  XsdSchema* get_schema();

  /** Get the schema.
   * @returns A pointer to the schema, or <tt>nullptr</tt>.
   */
  const XsdSchema* get_schema() const;

  /** Validate a document, using a previously parsed schema.
   * @param document Pointer to the document.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  virtual void validate(const Document* document);

  /** Validate an XML file, using a previously parsed schema.
   * @param filename The URL of the XML file.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  virtual void validate(const Glib::ustring& filename);

protected:
  void initialize_valid() override;
  void release_underlying() override;

private:
  struct Impl;
  std::auto_ptr<Impl> pimpl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_XSDVALIDATOR_H
