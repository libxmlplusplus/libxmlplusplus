/* schemavalidator.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson,
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_VALIDATOR_SCHEMAVALIDATOR_H
#define __LIBXMLPP_VALIDATOR_SCHEMAVALIDATOR_H

#include <libxml++/validators/validator.h>
#include <libxml++/schema.h>
#include <libxml++/document.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlSchemaParserCtxt;
  struct _xmlSchemaValidCtxt;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS4

namespace xmlpp {

/** XML Schema Validator.
 *
 * @newin{2,24}
 */
class SchemaValidator : public Validator
{
public:
  SchemaValidator();

  /** Create a validator and parse a schema definition file immediately.
   * @param file The URL of the schema.
   * @throws xmlpp::parse_error
   */
  explicit SchemaValidator(const Glib::ustring& file);

  /** Create a validator and parse a schema definition document immediately.
   * @param document A preparsed document tree, containing the schema definition.
   * @note The document may be modified during the parsing process.
   * @throws xmlpp::parse_error
   */
  explicit SchemaValidator(Document& document);

  /** Create a schema validator.
   * @param schema A pointer to the XML schema to use when validating XML documents.
   *        The validator does not take ownership of the schema. The caller must
   *        guarantee that the schema exists as long as the validator keeps a
   *        pointer to it. The caller is responsible for deleting the schema
   *        when it's no longer needed.
   */
  explicit SchemaValidator(Schema* schema);

  virtual ~SchemaValidator();

  /** Parse a schema definition file.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param file The URL of the schema.
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
   * @note The document may be modified during the parsing process.
   * @throws xmlpp::parse_error
   */
  virtual void parse_document(Document& document);

  /** Set a schema.
   * If the validator already contains a schema, that schema is released
   * (deleted if the validator owns the schema).
   * @param schema A pointer to the XML schema to use when validating XML documents.
   *        The validator does not take ownership of the schema. The caller must
   *        guarantee that the schema exists as long as the validator keeps a
   *        pointer to it. The caller is responsible for deleting the schema
   *        when it's no longer needed.
   */
  virtual void set_schema(Schema* schema);

  /** Test whether a schema has been parsed.
   */
  operator bool() const;

  /** Get the parsed schema.
   * @returns A pointer to the parsed schema, or <tt>0</tt>.
   */
  Schema* get_schema();

  /** Get the parsed schema.
   * @returns A pointer to the parsed schema, or <tt>0</tt>.
   */
  const Schema* get_schema() const;

  /** Validate a document, using a previously parsed schema.
   * @param doc Pointer to the document.
   * @returns Whether the document is valid.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  bool validate(const Document* doc);

  /** Validate an XML file, using a previously parsed schema.
   * @param file The URI of the XML file.
   * @returns Whether the document is valid.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  bool validate(const Glib::ustring& file);

protected:
  virtual void initialize_valid();
  void parse_context(_xmlSchemaParserCtxt* context);
  virtual void release_underlying();

  Schema* schema_;
  bool embbeded_shema_; //TODO Correct mis-spelling at the next API/ABI break.
  _xmlSchemaValidCtxt* ctxt_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_SCHEMAVALIDATOR_H

