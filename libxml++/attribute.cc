/* attribute.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/attribute.h"
#include "libxml++/attributedeclaration.h"

#include <libxml/tree.h>

namespace xmlpp
{

Attribute::Attribute(xmlNode* node)
  : Node(node)
{
}

Attribute::~Attribute()
{
}

Glib::ustring Attribute::get_name() const
{
  // This will get the name also for an AttributeDeclaration. The name is in
  // the same position in xmlNode, xmlAttr and xmlAttribute.
  return cobj()->name ? (char*)cobj()->name : Glib::ustring();
}

//TODO when we can break ABI: Make get_value() virtual.
Glib::ustring Attribute::get_value() const
{
  const AttributeDeclaration* const attributeDecl =
    dynamic_cast<const AttributeDeclaration*>(this);
  if (attributeDecl) // AttributeDeclaration
    return attributeDecl->get_value();

  // AttributeNode
  xmlChar* value = nullptr;
  if (cobj()->ns && cobj()->ns->href)
    value = xmlGetNsProp(cobj()->parent, cobj()->name, cobj()->ns->href);
  else
    value = xmlGetNoNsProp(cobj()->parent, cobj()->name);

  const Glib::ustring retn = value ? (const char*)value : "";
  if (value)
    xmlFree(value);
  return retn;
}

//TODO when we can break ABI: Move set_value() to AttributeNode.
void Attribute::set_value(const Glib::ustring& value)
{
  if (dynamic_cast<const AttributeDeclaration*>(this))
    return; // Won't change the value of an AttributeDeclaration

  if (cobj()->ns)
    xmlSetNsProp(cobj()->parent, cobj()->ns, cobj()->name, (const xmlChar*)value.c_str());
  else
    xmlSetProp(cobj()->parent, cobj()->name, (const xmlChar*)value.c_str());
}

xmlAttr* Attribute::cobj()
{
  // yes, this does what it looks like: it takes an xmlNode pointer
  // and *reinterprets* it as an xmlAttr pointer
  // -stefan
  return reinterpret_cast<xmlAttr*>(Node::cobj());
}

const xmlAttr* Attribute::cobj() const
{
  // yes, this does what it looks like: it takes an xmlNode pointer
  // and *reinterprets* it as an xmlAttr pointer
  // -stefan
  return reinterpret_cast<const xmlAttr*>(Node::cobj());
}

} //namespace xmlpp

