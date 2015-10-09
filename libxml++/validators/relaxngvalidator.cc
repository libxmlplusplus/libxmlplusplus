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
 * This file was adapted from schemavalidator.cc by Fugro Intersite B.V./Tjalling Hattink
 */

#include "libxml++/validators/relaxngvalidator.h"
#include "libxml++/parsers/domparser.h"
#include "libxml++/relaxngschema.h"

#include <libxml/relaxng.h>

namespace xmlpp
{

struct RelaxNGValidator::Impl
{
  Impl() : schema(nullptr), is_schema_owner(false), context(nullptr) {}

  RelaxNGSchema* schema;
  bool is_schema_owner;
  _xmlRelaxNGValidCtxt* context;
};


RelaxNGValidator::RelaxNGValidator()
: pimpl_(new Impl)
{
}

RelaxNGValidator::RelaxNGValidator(const Glib::ustring& filename)
: pimpl_(new Impl)
{
  parse_file(filename);
}

RelaxNGValidator::RelaxNGValidator(const Document* document)
: pimpl_(new Impl)
{
  parse_document(document);
}

RelaxNGValidator::RelaxNGValidator(RelaxNGSchema* schema, bool take_ownership)
: pimpl_(new Impl)
{
  set_schema(schema, take_ownership);
}

RelaxNGValidator::~RelaxNGValidator()
{
  release_underlying();
}

void RelaxNGValidator::parse_file(const Glib::ustring& filename)
{
  set_schema(new RelaxNGSchema(filename), true);
}

void RelaxNGValidator::parse_memory(const Glib::ustring& contents)
{
  std::unique_ptr<RelaxNGSchema> schema(new RelaxNGSchema());
  schema->parse_memory(contents);
  set_schema(schema.release(), true);
}

void RelaxNGValidator::parse_document(const Document* document)
{
  set_schema(new RelaxNGSchema(document), true);
}

void RelaxNGValidator::set_schema(RelaxNGSchema* schema, bool take_ownership)
{
  release_underlying();
  pimpl_->schema = schema;
  pimpl_->is_schema_owner = take_ownership;
}

void RelaxNGValidator::release_underlying()
{
  if (pimpl_->context)
  {
    xmlRelaxNGFreeValidCtxt(pimpl_->context);
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

RelaxNGSchema* RelaxNGValidator::get_schema()
{
  return pimpl_->schema;
}

const RelaxNGSchema* RelaxNGValidator::get_schema() const
{
  return pimpl_->schema;
}

RelaxNGValidator::operator const void*() const
{
  return reinterpret_cast<const void*>(pimpl_->schema && pimpl_->schema->cobj());
}


void RelaxNGValidator::initialize_valid()
{
  xmlRelaxNGSetValidErrors(pimpl_->context, &callback_validity_error, &callback_validity_warning, this);
  SchemaValidatorBase::initialize_valid();
}

void RelaxNGValidator::validate(const Document* document)
{
  if (!document)
    throw internal_error("RelaxNGValidator::validate(): document must not be nullptr.");

  if (!*this)
    throw internal_error("RelaxNGValidator::validate(): Must have a schema to validate document");

  // A context is required at this stage only
  if (!pimpl_->context)
    pimpl_->context = xmlRelaxNGNewValidCtxt(pimpl_->schema->cobj());

  if (!pimpl_->context)
    throw internal_error("RelaxNGValidator::validate(): Could not create validating context");

  xmlResetLastError();
  initialize_valid();

  const int res = xmlRelaxNGValidateDoc(pimpl_->context, const_cast<xmlDoc*>(document->cobj()));
  if (res != 0)
  {
    check_for_exception();

    auto error_str = format_xml_error();
    if (error_str.empty())
      error_str = "Error code from xmlRelaxNGValidateDoc(): " + Glib::ustring::format(res);
    throw validity_error("Document failed RelaxNG schema validation.\n" + error_str);
  }
}

void RelaxNGValidator::validate(const Glib::ustring& filename)
{
  // There is no xmlRelaxNGValidateFile().
  DomParser parser(filename);
  validate(parser.get_document());
}

} // namespace xmlpp
