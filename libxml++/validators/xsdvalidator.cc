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

#include "libxml++/validators/xsdvalidator.h"
#include "libxml++/xsdschema.h"

#include <libxml/xmlschemas.h>

namespace xmlpp
{

struct XsdValidator::Impl
{
  Impl() noexcept = default;

  XsdSchema *schema{nullptr};
  bool is_schema_owner{false};
  _xmlSchemaValidCtxt *context{nullptr};
};


XsdValidator::XsdValidator()
: pimpl_(new Impl)
{
}

XsdValidator::XsdValidator(const std::string& filename)
: pimpl_(new Impl)
{
  parse_file(filename);
}

XsdValidator::XsdValidator(const Document* document)
: pimpl_(new Impl)
{
  parse_document(document);
}

XsdValidator::XsdValidator(XsdSchema* schema, bool take_ownership)
: pimpl_(new Impl)
{
  set_schema(schema, take_ownership);
}

XsdValidator::~XsdValidator()
{
  release_underlying();
}

void XsdValidator::parse_file(const std::string& filename)
{
  set_schema(new XsdSchema(filename), true);
}

void XsdValidator::parse_memory(const ustring& contents)
{
  std::unique_ptr<XsdSchema> schema(new XsdSchema());
  schema->parse_memory(contents);
  set_schema(schema.release(), true);
}

void XsdValidator::parse_document(const Document* document)
{
  set_schema(new XsdSchema(document), true);
}

void XsdValidator::set_schema(XsdSchema* schema, bool take_ownership)
{
  release_underlying();
  pimpl_->schema = schema;
  pimpl_->is_schema_owner = take_ownership;
}

void XsdValidator::release_underlying()
{
  if (pimpl_->context)
  {
    xmlSchemaFreeValidCtxt(pimpl_->context);
    pimpl_->context = nullptr;
  }

  if (pimpl_->schema)
  {
    if (pimpl_->is_schema_owner)
      delete pimpl_->schema;
    pimpl_->schema = nullptr;
  }

  SchemaValidatorBase::release_underlying();
}

XsdSchema* XsdValidator::get_schema() noexcept
{
  return pimpl_->schema;
}

const XsdSchema* XsdValidator::get_schema() const noexcept
{
  return pimpl_->schema;
}

XsdValidator::operator bool() const noexcept
{
  return pimpl_->schema && pimpl_->schema->cobj();
}


void XsdValidator::initialize_context()
{
  xmlSchemaSetValidErrors(pimpl_->context, &callback_validity_error, &callback_validity_warning, this);
  SchemaValidatorBase::initialize_context();
}


void XsdValidator::validate(const Document* document)
{
  if (!document)
    throw internal_error("XsdValidator::validate(): document must not be nullptr.");

  if (!*this)
    throw internal_error("XsdValidator::validate(): Must have a schema to validate document");

  // A context is required at this stage only
  if (!pimpl_->context)
    pimpl_->context = xmlSchemaNewValidCtxt(pimpl_->schema->cobj());

  if (!pimpl_->context)
    throw internal_error("XsdValidator::validate(): Could not create validating context");

  xmlResetLastError();
  initialize_context();

  const int res = xmlSchemaValidateDoc(pimpl_->context, const_cast<xmlDoc*>(document->cobj()));
  if (res != 0)
  {
    check_for_exception();

    auto error_str = format_xml_error();
    if (error_str.empty())
      error_str = "Error code from xmlSchemaValidateDoc(): " + std::to_string(res);
    throw validity_error("Document failed XSD schema validation.\n" + error_str);
  }
}

void XsdValidator::validate(const std::string& filename)
{
  if (!*this)
    throw internal_error("XsdValidator::validate(): Must have a schema to validate file.");

  // A context is required at this stage only
  if (!pimpl_->context)
    pimpl_->context = xmlSchemaNewValidCtxt(pimpl_->schema->cobj());

  if (!pimpl_->context)
    throw internal_error("XsdValidator::validate(): Could not create validating context");

  xmlResetLastError();
  initialize_context();

  const int res = xmlSchemaValidateFile(pimpl_->context, filename.c_str(), 0);
  if (res != 0)
  {
    check_for_exception();

    auto error_str = format_xml_error();
    if (error_str.empty())
      error_str = "Error code from xmlSchemaValidateFile(): " + std::to_string(res);
    throw validity_error("XML file failed XSD schema validation.\n" + error_str);
  }
}

} // namespace xmlpp
