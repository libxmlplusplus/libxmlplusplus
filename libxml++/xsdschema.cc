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

#include <libxml++/xsdschema.h>

#include <libxml/tree.h>
#include <libxml/xmlschemas.h>

namespace
{
  // This class holds an xmlSchemaParserCtxtPtr and releases it on
  // destruction. This way, we make sure we don't leak it, not even
  // when an exception is thrown.
  class XsdSchemaParserContextHolder
  {
  public:
    XsdSchemaParserContextHolder(xmlSchemaParserCtxtPtr ctx): ctx_(ctx) {}
    ~XsdSchemaParserContextHolder() { if (ctx_) xmlSchemaFreeParserCtxt(ctx_); }

  private:
    xmlSchemaParserCtxtPtr ctx_;
  };
}

namespace xmlpp
{

struct XsdSchema::Impl
{
  Impl() : schema(nullptr), document(nullptr) {}

  _xmlSchema* schema;
  _xmlDoc* document;
};

  
XsdSchema::XsdSchema()
: pimpl_(new Impl)
{
}

XsdSchema::XsdSchema(_xmlSchema* schema)
: pimpl_(new Impl)
{
  pimpl_->schema = schema;
}

XsdSchema::XsdSchema(const Glib::ustring& filename)
: pimpl_(new Impl)
{
  parse_file(filename);
}

XsdSchema::XsdSchema(const Document* document)
: pimpl_(new Impl)
{
  parse_document(document);
}

XsdSchema::~XsdSchema()
{
  release_underlying();
}

void XsdSchema::parse_file(const Glib::ustring& filename)
{
  xmlResetLastError();
  release_underlying();
  parse_context(xmlSchemaNewParserCtxt(filename.c_str()));
}

void XsdSchema::parse_memory(const Glib::ustring& contents)
{
  xmlResetLastError();
  release_underlying();
  parse_context(xmlSchemaNewMemParserCtxt(contents.c_str(), contents.bytes()));
}

void XsdSchema::parse_document(const Document* document)
{
  if (!document)
    throw parse_error("XsdSchema::parse_document(): document must not be nullptr.");

  xmlResetLastError();
  release_underlying();

  // xmlSchemaParse() may modify the document. Take a copy.
  pimpl_->document = xmlCopyDoc(const_cast<xmlDoc*>(document->cobj()), true); // recursive copy
  if (!pimpl_->document)
    throw parse_error("XsdSchema::parse_document(): Could not copy the document.\n" + format_xml_error());

  parse_context(xmlSchemaNewDocParserCtxt(pimpl_->document));
}

void XsdSchema::parse_context(_xmlSchemaParserCtxt* context)
{
  if (!context)
    throw parse_error("XsdSchema::parse_context(): Could not create parser context.\n" + format_xml_error());
  
  XsdSchemaParserContextHolder holder(context);

  pimpl_->schema = xmlSchemaParse(context);
  if (!pimpl_->schema)
  {
    release_underlying();
    throw parse_error("XsdSchema::parse_context(): Schema could not be parsed.\n" + format_xml_error());
  }
}

_xmlSchema* XsdSchema::cobj()
{
  return pimpl_->schema;
}

const _xmlSchema* XsdSchema::cobj() const
{
  return pimpl_->schema;
}

void XsdSchema::release_underlying()
{
  if (pimpl_->schema)
  {
    xmlSchemaFree(pimpl_->schema);
    pimpl_->schema = nullptr;
  }

  if (pimpl_->document)
  {
    xmlFreeDoc(pimpl_->document);
    pimpl_->document = nullptr;
  }
}

} //namespace xmlpp
