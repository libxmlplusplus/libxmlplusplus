/* xml++.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/contentnode.h>
#include <libxml++/exceptions/internal_error.h>

#include <libxml/tree.h>

namespace xmlpp
{
  
ContentNode::ContentNode(xmlNode* node)
: Node(node)
{}

ContentNode::~ContentNode()
{}

std::string ContentNode::get_content() const
{
  if(cobj()->type == XML_ELEMENT_NODE)
    throw internal_error("this node type doesn't have content");

  return cobj()->content ? (char*)cobj()->content : "";
}

void ContentNode::set_content(const std::string& content)
{
   if(cobj()->type == XML_ELEMENT_NODE)
      throw internal_error("can't set content for this node type");

   xmlNodeSetContent(cobj(), (xmlChar*)content.c_str());
}

bool ContentNode::is_white_space() const
{
  return xmlIsBlankNode(const_cast<_xmlNode*>(cobj()));
}

} //namespace xmlpp

