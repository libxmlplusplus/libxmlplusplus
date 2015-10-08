/* element.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/element.h>
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/document.h>

#include <libxml/tree.h>

namespace xmlpp
{

Element::Element(xmlNode* node)
: Node(node)
{}

Element::~Element()
{}

Element::AttributeList Element::get_attributes()
{
  AttributeList attributes;
  for(auto attr = cobj()->properties; attr; attr = attr->next)
  {
    Node::create_wrapper(reinterpret_cast<xmlNode*>(attr));
    attributes.push_back(reinterpret_cast<Attribute*>(attr->_private));
  }

  return attributes;
}

const Element::AttributeList Element::get_attributes() const
{
  return const_cast<Element*>(this)->get_attributes();
}

Attribute* Element::get_attribute(const Glib::ustring& name,
                                  const Glib::ustring& ns_prefix) const
{
  // An empty ns_prefix means "use no namespace".
  // The default namespace never applies to an attribute.
  Glib::ustring ns_uri;
  if (!ns_prefix.empty())
  {
    ns_uri = get_namespace_uri_for_prefix(ns_prefix);
    if (ns_uri.empty())
      return nullptr; // No such prefix.
  }

  // The return value of xmlHasNsProp() may be either an xmlAttr*, pointing to an
  // explicitly set attribute (XML_ATTRIBUTE_NODE), or an xmlAttribute*,
  // cast to an xmlAttr*, pointing to the declaration of an attribute with a
  // default value (XML_ATTRIBUTE_DECL).
  auto attr = xmlHasNsProp(const_cast<xmlNode*>(cobj()), (const xmlChar*)name.c_str(),
                               ns_uri.empty() ? nullptr : (const xmlChar*)ns_uri.c_str());
  if (attr)
  {
    Node::create_wrapper(reinterpret_cast<xmlNode*>(attr));
    return reinterpret_cast<Attribute*>(attr->_private);
  }

  return nullptr;
}

Glib::ustring Element::get_attribute_value(const Glib::ustring& name, const Glib::ustring& ns_prefix) const
{
  const auto attr = get_attribute(name, ns_prefix);
  return attr ? attr->get_value() : Glib::ustring();
}

Attribute* Element::set_attribute(const Glib::ustring& name, const Glib::ustring& value,
                                  const Glib::ustring& ns_prefix)
{
  xmlAttr* attr = nullptr;

  //Ignore the namespace if none was specified:
  if(ns_prefix.empty())
  {
    attr = xmlSetProp(cobj(), (const xmlChar*)name.c_str(), (const xmlChar*)value.c_str());
  }
  else
  {
    //If the namespace exists, then use it:
    auto ns = xmlSearchNs(cobj()->doc, cobj(), (const xmlChar*)ns_prefix.c_str());
    if (ns)
    {
      attr = xmlSetNsProp(cobj(), ns, (const xmlChar*)name.c_str(),
                          (const xmlChar*)value.c_str());
    }
    else
    {
      throw exception("The namespace prefix (" + ns_prefix + ") has not been declared.");
    }
  }

  if(attr)
  {
    Node::create_wrapper(reinterpret_cast<xmlNode*>(attr));
    return reinterpret_cast<Attribute*>(attr->_private);
  }
  else
    return nullptr;
}

void Element::remove_attribute(const Glib::ustring& name, const Glib::ustring& ns_prefix)
{
  if (ns_prefix.empty())
    xmlUnsetProp(cobj(), (const xmlChar*)name.c_str());
  else
  {
    auto ns = xmlSearchNs(cobj()->doc, cobj(), (const xmlChar*)ns_prefix.c_str());
    if (ns)
      xmlUnsetNsProp(cobj(), ns, (const xmlChar*)name.c_str());
  }
}

const TextNode* Element::get_child_text() const
{
  // FIXME: return only the first content node
  for(auto child = cobj()->children; child; child = child->next)
     if(child->type == XML_TEXT_NODE)
     {
       Node::create_wrapper(child);
       return static_cast<TextNode*>(child->_private);
     }

  return nullptr;
}

TextNode* Element::get_child_text()
{
  // TODO: This only returns the first content node.
  // What should we do instead? Update the documentation if we change this. murrayc.
  for(auto child = cobj()->children; child; child = child->next)
     if(child->type == XML_TEXT_NODE)
     {
       Node::create_wrapper(child);
       return static_cast<TextNode*>(child->_private);
     }

  return nullptr;
}

void Element::set_child_text(const Glib::ustring& content)
{
  auto node = get_child_text();
  if(node)
    node->set_content(content);
  else
    add_child_text(content);
}

TextNode* Element::add_child_text(const Glib::ustring& content)
{
  if(cobj()->type == XML_ELEMENT_NODE)
  {
    auto child = xmlNewText((const xmlChar*)content.c_str());

    // Use the result, because child can be freed when merging text nodes:
    auto node = xmlAddChild(cobj(), child);
    if (!node)
    {
      xmlFreeNode(child);
      throw internal_error("Could not add text node \"" + content + "\"");
    }
    Node::create_wrapper(node);
    return static_cast<TextNode*>(node->_private);
  }
  return nullptr;
}

TextNode* Element::add_child_text(xmlpp::Node* previous_sibling, const Glib::ustring& content)
{
  if(!previous_sibling)
    return nullptr;

  if(cobj()->type == XML_ELEMENT_NODE)
  {
    auto child = xmlNewText((const xmlChar*)content.c_str());

    // Use the result, because child can be freed when merging text nodes:
    auto node = xmlAddNextSibling(previous_sibling->cobj(), child);
    if (!node)
    {
      xmlFreeNode(child);
      throw internal_error("Could not add text node \"" + content + "\"");
    }
    Node::create_wrapper(node);
    return static_cast<TextNode*>(node->_private);
  }
  return nullptr;
}

TextNode* Element::add_child_text_before(xmlpp::Node* next_sibling, const Glib::ustring& content)
{
  if(!next_sibling)
    return nullptr;

  if(cobj()->type == XML_ELEMENT_NODE)
  {
    auto child = xmlNewText((const xmlChar*)content.c_str());

    // Use the result, because child can be freed when merging text nodes:
    auto node = xmlAddPrevSibling(next_sibling->cobj(), child);
    if (!node)
    {
      xmlFreeNode(child);
      throw internal_error("Could not add text node \"" + content + "\"");
    }
    Node::create_wrapper(node);
    return static_cast<TextNode*>(node->_private);
  }
  return nullptr;
}

bool Element::has_child_text() const
{
  return get_child_text() != nullptr;
}

void Element::set_namespace_declaration(const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix)
{
  //Create a new namespace declaration for this element:
  auto ns = xmlNewNs(cobj(), (const xmlChar*)(ns_uri.empty() ? nullptr : ns_uri.c_str()),
                       (const xmlChar*)(ns_prefix.empty() ? nullptr : ns_prefix.c_str()) );
  if (!ns)
  {
    // Not an error, if we try to assign the same uri to the prefix once again.
    ns = xmlSearchNs(cobj()->doc, cobj(),
                     (const xmlChar*)(ns_prefix.empty() ? nullptr : ns_prefix.c_str()));
    const char* const previous_href = (ns && ns->href) ? (const char*)ns->href : "";
    if (!ns || ns_uri != previous_href)
      throw exception("Could not add namespace declaration with URI=" + ns_uri +
                      ", prefix=" + ns_prefix);
  }
  if (ns_prefix == get_namespace_prefix())
  {
    // Assign namespace directly if the prefix equals the node's own namespace prefix.
    // This is not done by xmlNewNs(). (See https://bugzilla.gnome.org/show_bug.cgi?id=737682)
    set_namespace(ns_prefix);
  }
  //We ignore the returned xmlNs*. It's owned by the XML_ELEMENT_NODE.
}

Glib::ustring Element::get_namespace_uri_for_prefix(const Glib::ustring& ns_prefix) const
{
  Glib::ustring result;
  
  //Find the namespace:
  const auto ns = xmlSearchNs( cobj()->doc, const_cast<xmlNode*>(cobj()), (xmlChar*)ns_prefix.c_str() );
  if(ns)
  {
    //Get the namespace URI associated with this prefix:
    if(ns && ns->href)
      result = (const char*)ns->href;
  }
  
  return result;
}


CommentNode* Element::add_child_comment(const Glib::ustring& content)
{
  auto child = xmlNewComment((const xmlChar*)content.c_str());
 
  // Use the result, because child can be freed when merging text nodes:
  auto node = xmlAddChild(cobj(), child);
  if (!node)
  {
    xmlFreeNode(child);
    throw internal_error("Could not add comment node \"" + content + "\"");
  }
  Node::create_wrapper(node);
  return static_cast<CommentNode*>(node->_private);
}


CdataNode* Element::add_child_cdata(const Glib::ustring& content)
{
  auto child = xmlNewCDataBlock(cobj()->doc, (const xmlChar*)content.c_str(), content.bytes());
  auto node = xmlAddChild(cobj(), child);
  if (!node)
  {
    xmlFreeNode(child);
    throw internal_error("Could not add CDATA node \"" + content + "\"");
  }
  Node::create_wrapper(node);
  return static_cast<CdataNode*>(node->_private);
}

EntityReference* Element::add_child_entity_reference(const Glib::ustring& name)
{
  const auto extended_name = name + "  "; // This is at least two chars long.
  int ichar = 0;
  if (extended_name[ichar] == '&')
    ++ichar;

  // Is it an entity reference or a character reference?
  // libxml uses xmlNode::type == XML_ENTITY_REF_NODE for both.
  xmlNode* child = nullptr;
  if (extended_name[ichar] == '#')
    child = xmlNewCharRef(cobj()->doc, (const xmlChar*)name.c_str());
  else
    child = xmlNewReference(cobj()->doc, (const xmlChar*)name.c_str());
  auto node = xmlAddChild(cobj(), child);
  if (!node)
  {
    xmlFreeNode(child);
    throw internal_error("Could not add entity reference node " + name);
  }
  Node::create_wrapper(node);
  return static_cast<EntityReference*>(node->_private);
}

ProcessingInstructionNode* Element::add_child_processing_instruction(
  const Glib::ustring& name, const Glib::ustring& content)
{
  auto child = xmlNewDocPI(cobj()->doc, (const xmlChar*)name.c_str(), (const xmlChar*)content.c_str());
  auto node = xmlAddChild(cobj(), child);
  if (!node)
  {
    xmlFreeNode(child);
    throw internal_error("Could not add processing instruction node " + name);
  }
  Node::create_wrapper(node);
  return static_cast<ProcessingInstructionNode*>(node->_private);
}

} //namespace xmlpp
