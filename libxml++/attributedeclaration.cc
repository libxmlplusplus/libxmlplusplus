/* attributedeclaration.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/attributedeclaration.h"

#include <libxml/tree.h>

namespace xmlpp
{

AttributeDeclaration::AttributeDeclaration(xmlNode* node)
  : Attribute(node)
{
}

AttributeDeclaration::~AttributeDeclaration()
{
}

Glib::ustring AttributeDeclaration::get_value() const
{
  return (const char*)cobj()->defaultValue;
}

xmlAttribute* AttributeDeclaration::cobj() noexcept
{
  // An XML_ATTRIBUTE_DECL is represented by an xmlAttribute struct. Reinterpret
  // the xmlNode pointer stored in the base class as an xmlAttribute pointer.
  return reinterpret_cast<xmlAttribute*>(Node::cobj());
}

const xmlAttribute* AttributeDeclaration::cobj() const noexcept
{
  // An XML_ATTRIBUTE_DECL is represented by an xmlAttribute struct. Reinterpret
  // the xmlNode pointer stored in the base class as an xmlAttribute pointer.
  return reinterpret_cast<const xmlAttribute*>(Node::cobj());
}

} //namespace xmlpp
