/* attributenode.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/attributenode.h"

#include <libxml/tree.h>

namespace xmlpp
{

AttributeNode::AttributeNode(xmlNode* node)
  : Attribute(node)
{
}

AttributeNode::~AttributeNode()
{
}

Glib::ustring AttributeNode::get_value() const
{
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

void AttributeNode::set_value(const Glib::ustring& value)
{
  if (cobj()->ns)
    xmlSetNsProp(cobj()->parent, cobj()->ns, cobj()->name, (const xmlChar*)value.c_str());
  else
    xmlSetProp(cobj()->parent, cobj()->name, (const xmlChar*)value.c_str());
}

xmlAttr* AttributeNode::cobj() noexcept
{
  // An XML_ATTRIBUTE_NODE is represented by an xmlAttr struct. Reinterpret
  // the xmlNode pointer stored in the base class as an xmlAttr pointer.
  return reinterpret_cast<xmlAttr*>(Node::cobj());
}

const xmlAttr* AttributeNode::cobj() const noexcept
{
  // An XML_ATTRIBUTE_NODE is represented by an xmlAttr struct. Reinterpret
  // the xmlNode pointer stored in the base class as an xmlAttr pointer.
  return reinterpret_cast<const xmlAttr*>(Node::cobj());
}

} //namespace xmlpp
