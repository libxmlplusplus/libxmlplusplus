/* entityreference.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/entityreference.h>
#include <libxml++/exceptions/internal_error.h>

#include <libxml/tree.h>

namespace xmlpp
{
  
EntityReference::EntityReference(xmlNode* node)
: Node(node)
{}

EntityReference::~EntityReference()
{}

Glib::ustring EntityReference::get_resolved_text() const
{
  Glib::ustring result;

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

Glib::ustring EntityReference::get_original_text() const
{
  Glib::ustring result;

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


} //namespace xmlpp

