/* entitydeclaration.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/entitydeclaration.h>
#include <libxml/entities.h>

namespace xmlpp
{

EntityDeclaration::EntityDeclaration(xmlNode* node)
: ContentNode(node)
{}

EntityDeclaration::~EntityDeclaration() = default;

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring EntityDeclaration::get_resolved_text() const
{
  return cobj()->content ? (const char*)cobj()->content : "";
}

ustring EntityDeclaration::get_original_text() const
{
  return cobj()->orig ? (const char*)cobj()->orig : "";
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> EntityDeclaration::get_resolved_text2() const
{
  if (!cobj()->content)
    return {};
  return (const char*)cobj()->content;
}

std::optional<ustring> EntityDeclaration::get_original_text2() const
{
  if (!cobj()->orig)
    return {};
  return (const char*)cobj()->orig;
}

xmlEntity* EntityDeclaration::cobj() noexcept
{
  // An XML_ENTITY_DECL is represented by an xmlEntity struct. Reinterpret
  // the xmlNode pointer stored in the base class as an xmlEntity pointer.
  return reinterpret_cast<xmlEntity*>(Node::cobj());
}

const xmlEntity* EntityDeclaration::cobj() const noexcept
{
  // An XML_ENTITY_DECL is represented by an xmlEntity struct. Reinterpret
  // the xmlNode pointer stored in the base class as an xmlEntity pointer.
  return reinterpret_cast<const xmlEntity*>(Node::cobj());
}

} //namespace xmlpp
