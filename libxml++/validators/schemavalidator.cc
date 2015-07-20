/* dtdvalidator.cpp
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++config.h> // LIBXMLXX_DISABLE_DEPRECATED

#ifndef LIBXMLXX_DISABLE_DEPRECATED

#include "libxml++/validators/schemavalidator.h"
#include "libxml++/schema.h"

#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>

#include <sstream>
#include <iostream>

namespace
{
  // This class simply holds a xmlSchemaParserCtxtPtr and releases it on
  // destruction. This way, we make sure we don't leak it in either case,
  // even when an exception is thrown.
  class XmlSchemaParserContextHolder
  {
  public:
    XmlSchemaParserContextHolder(xmlSchemaParserCtxtPtr ptr): ptr_(ptr) {}
    ~XmlSchemaParserContextHolder() { xmlSchemaFreeParserCtxt(ptr_); }

  private:
    xmlSchemaParserCtxtPtr ptr_;
  };
}

namespace xmlpp
{

SchemaValidator::SchemaValidator()
: schema_(nullptr)
, embbeded_shema_(false)
, ctxt_(nullptr)
{
}

SchemaValidator::SchemaValidator(const Glib::ustring& file)
: schema_(nullptr)
, embbeded_shema_(false)
, ctxt_(nullptr)
{
  parse_file( file );
}

SchemaValidator::SchemaValidator(Document& document)
: schema_(nullptr)
, embbeded_shema_(false)
, ctxt_(nullptr)
{
  parse_document( document );
}

SchemaValidator::SchemaValidator(Schema* schema)
: schema_(schema)
, embbeded_shema_(false)
, ctxt_(nullptr)
{
}

SchemaValidator::~SchemaValidator()
{
  release_underlying();
  Validator::release_underlying();
}

void SchemaValidator::parse_context(_xmlSchemaParserCtxt* context)
{
  if (!context)
    throw parse_error("Could not create schema parser context\n" + format_xml_error());

  release_underlying(); // Free any existing schema.

  auto schema = xmlSchemaParse( context );
  if ( ! schema )
    throw parse_error("Schema could not be parsed\n" + format_xml_error());

  schema->_private = new Schema(schema);

  schema_ = static_cast<Schema*>(schema->_private);
  embbeded_shema_ = true;
}

void SchemaValidator::parse_file(const Glib::ustring& filename)
{
  xmlResetLastError();
  auto ctx = xmlSchemaNewParserCtxt( filename.c_str() );
  XmlSchemaParserContextHolder holder(ctx);
  parse_context( ctx );
}

void SchemaValidator::parse_memory(const Glib::ustring& contents)
{
  xmlResetLastError();
  auto ctx = xmlSchemaNewMemParserCtxt( contents.c_str(), contents.bytes() );
  XmlSchemaParserContextHolder holder(ctx);
  parse_context( ctx );
}

void SchemaValidator::parse_document(Document& document)
{
  xmlResetLastError();
  auto ctx = xmlSchemaNewDocParserCtxt( document.cobj() );
  XmlSchemaParserContextHolder holder(ctx);
  parse_context( ctx );
}

void SchemaValidator::set_schema(Schema* schema)
{
  release_underlying();
  schema_ = schema;
  embbeded_shema_ = false;
}

void SchemaValidator::release_underlying()
{
  if(ctxt_)
  {
    xmlSchemaFreeValidCtxt( ctxt_ );
    ctxt_ = nullptr;
  }
  if(schema_)
  {
    if(embbeded_shema_)
      delete schema_;
    schema_ = nullptr;
  }
}

SchemaValidator::operator bool() const
{
  return schema_ != nullptr;
}

Schema* SchemaValidator::get_schema()
{
  return schema_;
}

const Schema* SchemaValidator::get_schema() const
{
  return schema_;
}

void SchemaValidator::initialize_valid()
{
  xmlSchemaSetValidErrors(ctxt_, &callback_validity_error, &callback_validity_warning, this);
  Validator::initialize_valid();
}


bool SchemaValidator::validate(const Document* doc)
{
  if (!doc)
    throw internal_error("Document pointer cannot be 0");

  if (!schema_)
    throw internal_error("Must have a schema to validate document");

  // A context is required at this stage only
  if (!ctxt_)
    ctxt_ = xmlSchemaNewValidCtxt( schema_->cobj() );

  if(!ctxt_)
  {
    throw internal_error("Couldn't create validating context");
  }

  xmlResetLastError();
  initialize_valid();

  const int res = xmlSchemaValidateDoc( ctxt_, (xmlDoc*)doc->cobj() );

  if(res != 0)
  {
    check_for_exception();

    auto error_str = format_xml_error();
    if (error_str.empty())
      error_str = "Error code from xmlSchemaValidateDoc(): " + Glib::ustring::format(res);
    throw validity_error("Document failed schema validation\n" + error_str);
  }

  return res == 0;
}

bool SchemaValidator::validate(const Glib::ustring& file)
{
  if (file.empty())
    throw internal_error("File path must not be empty");

  if (!schema_)
    throw internal_error("Must have a schema to validate document");

  // A context is required at this stage only
  if (!ctxt_)
    ctxt_ = xmlSchemaNewValidCtxt( schema_->cobj() );

  if(!ctxt_)
  {
    throw internal_error("Couldn't create validating context");
  }

  xmlResetLastError();
  initialize_valid();

  const int res = xmlSchemaValidateFile( ctxt_, file.c_str(), 0 );

  if(res != 0)
  {
    check_for_exception();

    auto error_str = format_xml_error();
    if (error_str.empty())
      error_str = "Error code from xmlSchemaValidateFile(): " + Glib::ustring::format(res);
    throw validity_error("Document failed schema validation\n" + error_str);
  }

  return res == 0;
}

} // namespace xmlpp
#endif // LIBXMLXX_DISABLE_DEPRECATED
