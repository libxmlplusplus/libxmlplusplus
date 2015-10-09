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
 * This file was adapted from schema.cc by Fugro Intersite B.V./Tjalling Hattink
 */

#include <libxml++/relaxngschema.h>

#include <libxml/tree.h>
#include <libxml/relaxng.h>

namespace
{
  // This class holds an xmlRelaxNGParserCtxtPtr and releases it on
  // destruction. This way, we make sure we don't leak it, not even
  // when an exception is thrown.
  class RelaxNGSchemaParserContextHolder
  {
  public:
    RelaxNGSchemaParserContextHolder(xmlRelaxNGParserCtxtPtr ctx): ctx_(ctx) {}
    ~RelaxNGSchemaParserContextHolder() { if (ctx_) xmlRelaxNGFreeParserCtxt(ctx_); }

  private:
    xmlRelaxNGParserCtxtPtr ctx_;
  };
}

namespace xmlpp
{

struct RelaxNGSchema::Impl
{
  Impl() : schema(nullptr) {}

  _xmlRelaxNG* schema;
};

  
RelaxNGSchema::RelaxNGSchema()
: pimpl_(new Impl)
{
}

RelaxNGSchema::RelaxNGSchema(_xmlRelaxNG* schema)
: pimpl_(new Impl)
{
  pimpl_->schema = schema;
}

RelaxNGSchema::RelaxNGSchema(const Glib::ustring& filename)
: pimpl_(new Impl)
{
  parse_file(filename);
}

RelaxNGSchema::RelaxNGSchema(const Document* document)
: pimpl_(new Impl)
{
  parse_document(document);
}

RelaxNGSchema::~RelaxNGSchema()
{
  release_underlying();
}

void RelaxNGSchema::parse_file(const Glib::ustring& filename)
{
  parse_context(xmlRelaxNGNewParserCtxt(filename.c_str()));
}

void RelaxNGSchema::parse_memory(const Glib::ustring& contents)
{
  parse_context(xmlRelaxNGNewMemParserCtxt(contents.c_str(), contents.bytes()));
}

void RelaxNGSchema::parse_document(const Document* document)
{
  if (!document)
    throw parse_error("RelaxNGSchema::parse_document(): document must not be nullptr.");

  // xmlRelaxNGNewDocParserCtxt() takes a copy of the xmlDoc.
  parse_context(xmlRelaxNGNewDocParserCtxt(const_cast<xmlDoc*>(document->cobj())));
}

void RelaxNGSchema::parse_context(_xmlRelaxNGParserCtxt* context)
{
  xmlResetLastError();
  release_underlying();

  if (!context)
    throw parse_error("RelaxNGSchema::parse_context(): Could not create parser context.\n" + format_xml_error());
  
  RelaxNGSchemaParserContextHolder holder(context);

  pimpl_->schema = xmlRelaxNGParse(context);
  if (!pimpl_->schema)
    throw parse_error("RelaxNGSchema::parse_context(): Schema could not be parsed.\n" + format_xml_error());
}

_xmlRelaxNG* RelaxNGSchema::cobj()
{
  return pimpl_->schema;
}

const _xmlRelaxNG* RelaxNGSchema::cobj() const
{
  return pimpl_->schema;
}

void RelaxNGSchema::release_underlying()
{
  if (pimpl_->schema)
  {
    xmlRelaxNGFree(pimpl_->schema);
    pimpl_->schema = nullptr;
  }
}

} //namespace xmlpp
