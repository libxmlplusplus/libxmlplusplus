/* entityreference.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/entityreference.h>

#include <libxml/entities.h>

namespace xmlpp
{

EntityReference::EntityReference(xmlNode* node)
: Node(node)
{}

EntityReference::~EntityReference() = default;

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring EntityReference::get_resolved_text() const
{
  ustring result;

  //Get the child xmlEntity node (there should only be 1).
  auto cChild = cobj()->children;
  if(cChild && cChild->type == XML_ENTITY_DECL)
  {
      auto cEntity = (xmlEntity*)cChild;
      const xmlChar* pch = cEntity->content;
      if(pch)
        result = (const char*)pch;
  }

  return result;
}

ustring EntityReference::get_original_text() const
{
  ustring result;

  //Get the child xmlEntity node (there should only be 1).
  auto cChild = cobj()->children;
  if(cChild && cChild->type == XML_ENTITY_DECL)
  {
      auto cEntity = (xmlEntity*)cChild;
      const xmlChar* pch = cEntity->orig;
      if(pch)
        result = (const char*)pch;
  }
  return result;
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> EntityReference::get_resolved_text2() const
{
  // Get the child xmlEntity node (there should only be 1).
  auto cChild = cobj()->children;
  if (!(cChild && cChild->type == XML_ENTITY_DECL))
    return {};
  auto cEntity = (xmlEntity*)cChild;
  if (!cEntity->content)
    return {};
  return (const char*)cEntity->content;
}

std::optional<ustring> EntityReference::get_original_text2() const
{
  // Get the child xmlEntity node (there should only be 1).
  auto cChild = cobj()->children;
  if (!(cChild && cChild->type == XML_ENTITY_DECL))
    return {};
  auto cEntity = (xmlEntity*)cChild;
  if (!cEntity->orig)
    return {};
  return (const char*)cEntity->orig;
}

} //namespace xmlpp

