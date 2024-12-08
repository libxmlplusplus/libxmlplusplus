/* attribute.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/attribute.h"
#include "libxml++/attributedeclaration.h"
#include "libxml++/attributenode.h"

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

std::optional<ustring> Attribute::get_value2() const
{
  auto attr_decl = dynamic_cast<const AttributeDeclaration*>(this);
  if (attr_decl)
    return attr_decl->get_value2();
  auto attr_node = dynamic_cast<const AttributeNode*>(this);
  if (attr_node)
    return attr_node->get_value2();
  return {};
}

} //namespace xmlpp
