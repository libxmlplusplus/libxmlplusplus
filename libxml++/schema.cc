/* schema.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++config.h> // LIBXMLXX_DISABLE_DEPRECATED

#ifndef LIBXMLXX_DISABLE_DEPRECATED

#include <libxml++/schema.h>

#include <libxml/tree.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>

namespace xmlpp
{
  
Schema::Schema(_xmlSchema* schema)
: impl_(schema)
, embedded_doc_(false)
{
  schema->_private = this;
}

Schema::Schema(Document* document, bool embed)
: impl_(nullptr)
, embedded_doc_(false)
{
  set_document(document, embed);
}

Schema::~Schema()
{
  release_underlying();
}

void Schema::set_document(Document* document, bool embed)
{
  release_underlying();

  bool has_created_document = false;
  if (!document)
  {
    document = new Document();
    has_created_document = true;
  }

  xmlResetLastError();
  auto context = xmlSchemaNewDocParserCtxt( document->cobj() );

  if(!context)
  {
    if (has_created_document)
      delete document;
    throw parse_error("Schema could not be parsed.\n" + format_xml_error());
  }
  
  impl_ = xmlSchemaParse(context);
  if(!impl_)
  {
    xmlSchemaFreeParserCtxt(context);
    if (has_created_document)
      delete document;
    throw parse_error("Schema could not be parsed.\n" + format_xml_error());
  }

  impl_->_private = this;
  embedded_doc_ = embed;
  xmlSchemaFreeParserCtxt(context);
}

Glib::ustring Schema::get_name() const
{
  return (impl_ && impl_->name) ? (char*)impl_->name : "";
}

Glib::ustring Schema::get_target_namespace() const
{
  return (impl_ && impl_->targetNamespace) ? (char*)impl_->targetNamespace : "";
}

Glib::ustring Schema::get_version() const
{
  return (impl_ && impl_->version) ? (char*)impl_->version : "";
}

void Schema::release_underlying()
{
  if(embedded_doc_ && impl_ && impl_->doc && impl_->doc->_private)
  {
    delete (Document*) impl_->doc->_private;
    embedded_doc_ = false;
  }

  if(impl_)
  {
    xmlSchemaFree(impl_);
    impl_ = nullptr;
  }
}

Document* Schema::get_document()
{
  if (!(impl_ && impl_->doc))
    return nullptr;

  if (!impl_->doc->_private) // Possible if *this was created with Schema(xmlSchema* schema).
    new Document(impl_->doc); // Sets impl_->doc->_private

  return (Document*) impl_->doc->_private;
}

const Document* Schema::get_document() const
{
  return const_cast<Schema*>(this)->get_document();
}

_xmlSchema* Schema::cobj()
{
  return impl_;
}

const _xmlSchema* Schema::cobj() const
{
  return impl_;
}

} //namespace xmlpp
#endif // LIBXMLXX_DISABLE_DEPRECATED
