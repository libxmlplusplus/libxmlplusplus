/* node.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/element.h>
#include <libxml++/nodes/node.h>
#include <libxml++/nodes/entitydeclaration.h>
#include <libxml++/nodes/entityreference.h>
#include <libxml++/nodes/textnode.h>
#include <libxml++/nodes/commentnode.h>
#include <libxml++/nodes/cdatanode.h>
#include <libxml++/nodes/processinginstructionnode.h>
#include <libxml++/nodes/xincludestart.h>
#include <libxml++/nodes/xincludeend.h>
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/attributedeclaration.h>
#include <libxml++/attributenode.h>
#include <libxml++/document.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/tree.h>

#include <iostream>

namespace // anonymous
{
// Common part of xmlpp::Node::eval_to_[boolean|number|string]
xmlXPathObject* eval_common(const Glib::ustring& xpath,
  const xmlpp::Node::PrefixNsMap* namespaces,
  xmlpp::XPathResultType* result_type, xmlNode* node)
{
  auto ctxt = xmlXPathNewContext(node->doc);
  if (!ctxt)
    throw xmlpp::internal_error("Could not create XPath context for " + xpath);
  ctxt->node = node;

  if (namespaces)
  {
    for (xmlpp::Node::PrefixNsMap::const_iterator it = namespaces->begin();
         it != namespaces->end(); ++it)
      xmlXPathRegisterNs(ctxt,
        reinterpret_cast<const xmlChar*>(it->first.c_str()),
        reinterpret_cast<const xmlChar*>(it->second.c_str()));
  }

  auto xpath_value = xmlXPathEvalExpression(
    reinterpret_cast<const xmlChar*>(xpath.c_str()), ctxt);

  xmlXPathFreeContext(ctxt);

  if (!xpath_value)
  {
    if (result_type)
      *result_type = xmlpp::XPATH_RESULT_UNDEFINED;

    throw xmlpp::exception("Invalid XPath: " + xpath);
  }

  if (result_type)
  {
    if (xpath_value->type == XPATH_NODESET ||
        xpath_value->type == XPATH_BOOLEAN ||
        xpath_value->type == XPATH_NUMBER ||
        xpath_value->type == XPATH_STRING)
      *result_type = static_cast<xmlpp::XPathResultType>(xpath_value->type);
    else
      *result_type = xmlpp::XPATH_RESULT_UNDEFINED;
  }

  return xpath_value;
}

// Common part of all overloaded xmlpp::Node::eval_to_boolean() methods.
bool eval_common_to_boolean(const Glib::ustring& xpath,
  const xmlpp::Node::PrefixNsMap* namespaces,
  xmlpp::XPathResultType* result_type, xmlNode* node)
{
  auto xpath_value = eval_common(xpath, namespaces, result_type, node);
  const int result = xmlXPathCastToBoolean(xpath_value);
  xmlXPathFreeObject(xpath_value);
  return static_cast<bool>(result);
}

// Common part of all overloaded xmlpp::Node::eval_to_number() methods.
double eval_common_to_number(const Glib::ustring& xpath,
  const xmlpp::Node::PrefixNsMap* namespaces,
  xmlpp::XPathResultType* result_type, xmlNode* node)
{
  auto xpath_value = eval_common(xpath, namespaces, result_type, node);
  const double result = xmlXPathCastToNumber(xpath_value);
  xmlXPathFreeObject(xpath_value);
  return result;
}

// Common part of all overloaded xmlpp::Node::eval_to_string() methods.
Glib::ustring eval_common_to_string(const Glib::ustring& xpath,
  const xmlpp::Node::PrefixNsMap* namespaces,
  xmlpp::XPathResultType* result_type, xmlNode* node)
{
  auto xpath_value = eval_common(xpath, namespaces, result_type, node);
  xmlChar* result = xmlXPathCastToString(xpath_value);
  xmlXPathFreeObject(xpath_value);
  if (result)
  {
    const Glib::ustring uresult(reinterpret_cast<const char*>(result));
    xmlFree(result);
    return uresult;
  }
  return Glib::ustring();
}

// Common part of all add_child*() methods.
xmlpp::Element* add_child_common(const Glib::ustring& name, xmlNode* child, xmlNode* node)
{
  if (!node)
  {
    xmlFreeNode(child);
    throw xmlpp::internal_error("Could not add child element node " + name);
  }
  xmlpp::Node::create_wrapper(node);
  return static_cast<xmlpp::Element*>(node->_private);
}

} // anonymous namespace

namespace xmlpp
{

Node::Node(xmlNode* node)
  : impl_(node)
{
  if (!impl_)
    throw internal_error("xmlNode pointer cannot be nullptr");

  impl_->_private = this;
}

Node::~Node()
{}

const Element* Node::get_parent() const
{
  return const_cast<Node*>(this)->get_parent();
}

Element* Node::get_parent()
{
  if(!(cobj()->parent && cobj()->parent->type == XML_ELEMENT_NODE))
    return nullptr;

  Node::create_wrapper(cobj()->parent);
  return static_cast<Element*>(cobj()->parent->_private);
}

const Node* Node::get_next_sibling() const
{
  return const_cast<Node*>(this)->get_next_sibling();
}

Node* Node::get_next_sibling()
{
  if(!cobj()->next)
    return nullptr;

  Node::create_wrapper(cobj()->next);
  return static_cast<Node*>(cobj()->next->_private);
}

const Node* Node::get_previous_sibling() const
{
  return const_cast<Node*>(this)->get_previous_sibling();
}

Node* Node::get_previous_sibling()
{
  if(!cobj()->prev)
    return nullptr;

  Node::create_wrapper(cobj()->prev);
  return static_cast<Node*>(cobj()->prev->_private);
}

static Node* _convert_node(xmlNode* node)
{
  Node* res = nullptr;
  if(node)
  {
    Node::create_wrapper(node);
    res = static_cast<Node*>(node->_private);
  }
  return res;
}

Node* Node::get_first_child(const Glib::ustring& name)
{
  auto child = impl_->children;
  if(!child)
    return nullptr;

  do
  {
    if(name.empty() || name == (const char*)child->name)
      return _convert_node(child);
  }
  while((child = child->next));
   
  return nullptr;
}

const Node* Node::get_first_child(const Glib::ustring& name) const
{
  return const_cast<Node*>(this)->get_first_child(name);
}

Node::NodeList Node::get_children(const Glib::ustring& name)
{
   auto child = impl_->children;
   if(!child)
     return NodeList();

   NodeList children;
   do
   {
      if(name.empty() || name == (const char*)child->name)
        children.push_back(_convert_node(child));
   }
   while((child = child->next));
   
   return children;
}

const Node::NodeList Node::get_children(const Glib::ustring& name) const
{
  return const_cast<Node*>(this)->get_children(name);
}

Element* Node::add_child(const Glib::ustring& name,
                         const Glib::ustring& ns_prefix)
{
  auto child = create_new_child_node(name, ns_prefix);
  auto node = xmlAddChild(impl_, child);
  return add_child_common(name, child, node);
}

Element* Node::add_child(xmlpp::Node* previous_sibling, 
                         const Glib::ustring& name,
                         const Glib::ustring& ns_prefix)
{
  if (!previous_sibling)
    return nullptr;

  auto child = create_new_child_node(name, ns_prefix);
  auto node = xmlAddNextSibling(previous_sibling->cobj(), child);
  return add_child_common(name, child, node);
}

Element* Node::add_child_before(xmlpp::Node* next_sibling, 
                         const Glib::ustring& name,
                         const Glib::ustring& ns_prefix)
{
  if (!next_sibling)
    return nullptr;

  auto child = create_new_child_node(name, ns_prefix);
  auto node = xmlAddPrevSibling(next_sibling->cobj(), child);
  return add_child_common(name, child, node);
}

Element* Node::add_child_with_new_ns(const Glib::ustring& name,
  const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix)
{
  auto child = create_new_child_node_with_new_ns(name, ns_uri, ns_prefix);
  auto node = xmlAddChild(impl_, child);
  return add_child_common(name, child, node);
}

Element* Node::add_child_with_new_ns(xmlpp::Node* previous_sibling,
  const Glib::ustring& name,
  const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix)
{
  if (!previous_sibling)
    return nullptr;

  auto child = create_new_child_node_with_new_ns(name, ns_uri, ns_prefix);
  auto node = xmlAddNextSibling(previous_sibling->cobj(), child);
  return add_child_common(name, child, node);
}

Element* Node::add_child_before_with_new_ns(xmlpp::Node* next_sibling,
  const Glib::ustring& name,
  const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix)
{
  if (!next_sibling)
    return nullptr;

  auto child = create_new_child_node_with_new_ns(name, ns_uri, ns_prefix);
  auto node = xmlAddPrevSibling(next_sibling->cobj(), child);
  return add_child_common(name, child, node);
}

_xmlNode* Node::create_new_child_node(const Glib::ustring& name, const Glib::ustring& ns_prefix)
{
   xmlNs* ns = nullptr;

   if(impl_->type != XML_ELEMENT_NODE)
   {
     throw internal_error("You can only add child nodes to element nodes");
   }

   if(ns_prefix.empty())
   {
     //Retrieve default namespace if it exists
     ns = xmlSearchNs(impl_->doc, impl_, nullptr);
   }
   else
   {
     //Use the existing namespace if one exists:
     ns = xmlSearchNs(impl_->doc, impl_, (const xmlChar*)ns_prefix.c_str());
     if (!ns)
     {
       throw exception("The namespace prefix (" + ns_prefix + ") has not been declared.");
     }
   }

   return xmlNewNode(ns, (const xmlChar*)name.c_str());
}

_xmlNode* Node::create_new_child_node_with_new_ns(const Glib::ustring& name,
  const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix)
{
  if (impl_->type != XML_ELEMENT_NODE)
    throw internal_error("You can only add child nodes to element nodes.");

  auto child = xmlNewNode(nullptr, (const xmlChar*)name.c_str());
  if (!child)
    throw internal_error("Could not create new element node.");

  auto ns = xmlNewNs(child, (const xmlChar*)(ns_uri.empty() ? nullptr : ns_uri.c_str()),
                       (const xmlChar*)(ns_prefix.empty() ? nullptr : ns_prefix.c_str()) );
  // xmlNewNs() does not create a namespace node for the predefined xml prefix.
  // It's usually defined in the document and not in any specific node.
  if (!ns && ns_prefix == "xml")
  {
    ns = xmlSearchNs(impl_->doc, impl_, (const xmlChar*)ns_prefix.c_str());
    if (ns && (ns_uri != (ns->href ? (const char*)ns->href : "")))
      ns = nullptr;
  }
  if (!ns)
  {
    xmlFreeNode(child);
    throw internal_error("Could not create new namespace node.");
  }

  xmlSetNs(child, ns);

  return child;
}

void Node::remove_child(Node* node)
{
  //TODO: Allow a node to be removed without deleting it, to allow it to be moved?
  //This would require a more complex memory management API.
  if (!node)
    return;
  auto cnode = node->cobj();
  Node::free_wrappers(cnode); //This delete the C++ node (not this) itself.
  xmlUnlinkNode(cnode);
  xmlFreeNode(cnode);
}

Node* Node::import_node(const Node* node, bool recursive)
{
  if (!node)
    return nullptr;

  //Create the node, by copying:
  auto imported_node = xmlDocCopyNode(const_cast<xmlNode*>(node->cobj()), impl_->doc, recursive);
  if (!imported_node)
  {
    throw exception("Unable to copy the node that shall be imported");
  }

  if (imported_node->type == XML_ATTRIBUTE_NODE && impl_->type == XML_ELEMENT_NODE)
  {
    auto old_attr = xmlHasNsProp(impl_, imported_node->name,
      imported_node->ns ? imported_node->ns->href : nullptr);
    if (old_attr && old_attr->type != XML_ATTRIBUTE_DECL)
    {
      // *this has an attribute with the same name as the imported attribute.
      // xmlAddChild() will delete the existing attribute.
      // Delete the C++ wrapper before the call to xmlAddChild().
      Node::free_wrappers(reinterpret_cast<xmlNode*>(old_attr));
    }
  }

  //Add the node:
  auto added_node = xmlAddChild(this->cobj(), imported_node);

  if (!added_node)
  {
    Node::free_wrappers(imported_node);
    xmlFreeNode(imported_node);

    throw exception("Unable to add imported node to current node");
  }

  // Usually added_node == imported_node, but a text node is merged with an
  // adjacent text node. In that case, xmlAddChild() frees imported_node, and
  // added_node is a pointer to the old text node.
  Node::create_wrapper(added_node);
  return static_cast<Node*>(added_node->_private);
}

Glib::ustring Node::get_name() const
{
  return impl_->name ? (const char*)impl_->name : "";
}

void Node::set_name(const Glib::ustring& name)
{
  xmlNodeSetName( impl_, (const xmlChar *)name.c_str() );
}

int Node::get_line() const
{
   return XML_GET_LINE(impl_);
}


xmlNode* Node::cobj()
{
  return impl_;
}

const xmlNode* Node::cobj() const
{
  return impl_;
}

Glib::ustring Node::get_path() const
{
  xmlChar* path = xmlGetNodePath(impl_);
  Glib::ustring retn = path ? (char*)path : "";
  xmlFree(path);
  return retn;
}

static NodeSet find_impl(xmlXPathContext* ctxt, const Glib::ustring& xpath)
{
  auto result = xmlXPathEval((const xmlChar*)xpath.c_str(), ctxt);

  if(!result)
  {
    xmlXPathFreeContext(ctxt);

    throw exception("Invalid XPath: " + xpath);
  }

  if(result->type != XPATH_NODESET)
  {
    xmlXPathFreeObject(result);
    xmlXPathFreeContext(ctxt);

    throw internal_error("Only nodeset result types are supported.");
  }

  auto nodeset = result->nodesetval;
  NodeSet nodes;
  if( nodeset && !xmlXPathNodeSetIsEmpty(nodeset))
  {
    const int count = xmlXPathNodeSetGetLength(nodeset);
    nodes.reserve(count);
    for (int i = 0; i != count; ++i)
    {
      auto cnode = xmlXPathNodeSetItem(nodeset, i);
      if(!cnode)
      {
        std::cerr << "Node::find_impl: The xmlNode was null." << std::endl;
        continue;
      }

      if(cnode->type == XML_NAMESPACE_DECL)
      {
        //In this case we would cast it to a xmlNs*,
        //but this C++ method only returns Nodes.
        std::cerr << "Node::find_impl: ignoring an xmlNs object." << std::endl;
        continue;
      }
      
      //TODO: Check for other cnode->type values?
  
      Node::create_wrapper(cnode);
      auto cppNode = static_cast<Node*>(cnode->_private);
      nodes.push_back(cppNode);
    }
  }
  else
  {
    // return empty set
  }

  xmlXPathFreeObject(result);
  xmlXPathFreeContext(ctxt);

  return nodes;
}

NodeSet Node::find(const Glib::ustring& xpath) const
{
  auto ctxt = xmlXPathNewContext(impl_->doc);
  if (!ctxt)
    throw internal_error("Could not create XPath context for " + xpath);
  ctxt->node = impl_;
  
  return find_impl(ctxt, xpath);
}

NodeSet Node::find(const Glib::ustring& xpath,
		   const PrefixNsMap& namespaces) const
{
  auto ctxt = xmlXPathNewContext(impl_->doc);
  if (!ctxt)
    throw internal_error("Could not create XPath context for " + xpath);
  ctxt->node = impl_;

  for (PrefixNsMap::const_iterator it=namespaces.begin();
       it != namespaces.end(); it++)
    xmlXPathRegisterNs(ctxt,
		       reinterpret_cast<const xmlChar*>(it->first.c_str()),
		       reinterpret_cast<const xmlChar*>(it->second.c_str()));

  return find_impl(ctxt, xpath);
}

bool Node::eval_to_boolean(const Glib::ustring& xpath, XPathResultType* result_type) const
{
  return eval_common_to_boolean(xpath, nullptr, result_type, impl_);
}

bool Node::eval_to_boolean(const Glib::ustring& xpath, const PrefixNsMap& namespaces,
  XPathResultType* result_type) const
{
  return eval_common_to_boolean(xpath, &namespaces, result_type, impl_);
}

double Node::eval_to_number(const Glib::ustring& xpath, XPathResultType* result_type) const
{
  return eval_common_to_number(xpath, nullptr, result_type, impl_);
}

double Node::eval_to_number(const Glib::ustring& xpath, const PrefixNsMap& namespaces,
  XPathResultType* result_type) const
{
  return eval_common_to_number(xpath, &namespaces, result_type, impl_);
}

Glib::ustring Node::eval_to_string(const Glib::ustring& xpath, XPathResultType* result_type) const
{
  return eval_common_to_string(xpath, nullptr, result_type, impl_);
}

Glib::ustring Node::eval_to_string(const Glib::ustring& xpath, const PrefixNsMap& namespaces,
  XPathResultType* result_type) const
{
  return eval_common_to_string(xpath, &namespaces, result_type, impl_);
}

Glib::ustring Node::get_namespace_prefix() const
{
  if(impl_->type == XML_DOCUMENT_NODE || impl_->type == XML_ENTITY_DECL)
  {
    //impl_ is actually of type xmlDoc or xmlEntity, instead of just xmlNode.
    //libxml does not always use GObject-style inheritance, so xmlDoc and
    //xmlEntity do not have all the same struct fields as xmlNode.
    //Therefore, a call to impl_->ns would be invalid.
    //This can be an issue when calling this method on a Node returned by Node::find().
    //See the TODO comment on Document, suggesting that Document should derive from Node.

    return Glib::ustring();
  }
  else if (impl_->type == XML_ATTRIBUTE_DECL)
  {
    //impl_ is actually of type xmlAttribute, instead of just xmlNode.
    const xmlAttribute* const attr = reinterpret_cast<const xmlAttribute*>(impl_);
    return attr->prefix ? (const char*)attr->prefix : "";
  }
  else if(impl_->ns && impl_->ns->prefix)
    return (char*)impl_->ns->prefix;
  else
    return Glib::ustring();
}

Glib::ustring Node::get_namespace_uri() const
{
  if(impl_->type == XML_DOCUMENT_NODE ||
     impl_->type == XML_ENTITY_DECL ||
     impl_->type == XML_ATTRIBUTE_DECL)
  {
    //impl_ is actually of type xmlDoc, xmlEntity or xmlAttribute, instead of just xmlNode.
    //libxml does not always use GObject-style inheritance, so those structs
    //do not have all the same struct fields as xmlNode.
    //Therefore, a call to impl_->ns would be invalid.
    //This can be an issue when calling this method on a Node returned by Node::find().
    //See the TODO comment on Document, suggesting that Document should derived from Node.

    return Glib::ustring();
  }

  if(impl_->ns && impl_->ns->href)
    return (char*)impl_->ns->href;
  else
    return Glib::ustring();
}

void Node::set_namespace(const Glib::ustring& ns_prefix)
{
  if (impl_->type == XML_ATTRIBUTE_DECL)
  {
    throw exception("Can't set the namespace of an attribute declaration");
  }

  //Look for the existing namespace to use:
  auto ns = xmlSearchNs( cobj()->doc, cobj(), (xmlChar*)(ns_prefix.empty() ? nullptr : ns_prefix.c_str()) );
  if(ns)
  {
      //Use it for this element:
      xmlSetNs(cobj(), ns);
  }
  else
  {
    throw exception("The namespace (" + ns_prefix + ") has not been declared.");
  }
}

void Node::create_wrapper(xmlNode* node)
{
  if(node->_private)
  {
    //Node already wrapped, skip
    return;
  }

  switch (node->type)
  {
    case XML_ELEMENT_NODE:
    {
      node->_private = new xmlpp::Element(node);
      break;
    }
    case XML_ATTRIBUTE_NODE:
    {
      node->_private = new xmlpp::AttributeNode(node);
      break;
    }
    case XML_ATTRIBUTE_DECL:
    {
      node->_private = new xmlpp::AttributeDeclaration(node);
      break;
    }
    case XML_TEXT_NODE:
    {
      node->_private = new xmlpp::TextNode(node);
      break;
    }
    case XML_COMMENT_NODE:
    {
      node->_private = new xmlpp::CommentNode(node);
      break;
    }
    case XML_CDATA_SECTION_NODE:
    {
      node->_private = new xmlpp::CdataNode(node);
      break;
    }
    case XML_PI_NODE:
    {
      node->_private = new xmlpp::ProcessingInstructionNode(node);
      break;
    }
    case XML_DTD_NODE:
    {
      node->_private = new xmlpp::Dtd(reinterpret_cast<xmlDtd*>(node));
      break;
    }
    //case XML_ENTITY_NODE:
    //{
    //  assert(0 && "Warning: XML_ENTITY_NODE not implemented");
    //  //node->_private = new xmlpp::ProcessingInstructionNode(node);
    //  break;
    //}
    case XML_ENTITY_DECL:
    {
      node->_private = new xmlpp::EntityDeclaration(node);
      break;
    }
    case XML_ENTITY_REF_NODE:
    {
      node->_private = new xmlpp::EntityReference(node);
      break;
    }
    case XML_XINCLUDE_START:
    {
      node->_private = new xmlpp::XIncludeStart(node);
      break;
    }
    case XML_XINCLUDE_END:
    {
      node->_private = new xmlpp::XIncludeEnd(node);
      break;
    }
    case XML_DOCUMENT_NODE:
    {
      // do nothing. For Documents it's the wrapper that is the owner.
      break;
    }
    default:
    {
      // good default for release versions
      node->_private = new xmlpp::Node(node);
      std::cerr << G_STRFUNC << " Warning: new node of unknown type created: "
                << node->type << std::endl;
      break;
    }
  }
}

void Node::free_wrappers(xmlNode* node)
{
  if(!node)
    return;
    
  //If an entity declaration contains an entity reference, there can be cyclic
  //references between entity declarations and entity references. (It's not
  //a tree.) We must avoid an infinite recursion.
  //Compare xmlFreeNode(), which frees the children of all node types except
  //XML_ENTITY_REF_NODE.
  if (node->type != XML_ENTITY_REF_NODE)
  {
    //Walk the children list.
    for (auto child = node->children; child; child = child->next)
      free_wrappers(child);
  }

  //Delete the local one
  switch(node->type)
  {
    //Node types that have no properties
    case XML_DTD_NODE:
      delete static_cast<Dtd*>(node->_private);
      node->_private = nullptr;
      return;
    case XML_ATTRIBUTE_NODE:
    case XML_ELEMENT_DECL:
    case XML_ATTRIBUTE_DECL:
    case XML_ENTITY_DECL:
      delete static_cast<Node*>(node->_private);
      node->_private = nullptr;
      return;
    case XML_DOCUMENT_NODE:
      //Do not free now. The Document is usually the one who owns the caller.
      return;
    default:
      delete static_cast<Node*>(node->_private);
      node->_private = nullptr;
      break;
  }

  //Walk the attributes list.
  //Note that some "derived" struct have a different layout, so 
  //_xmlNode::properties would be a nonsense value, leading to crashes,
  //(and shown as valgrind warnings), so we return above, to avoid 
  //checking it here.
  for(auto attr = node->properties; attr; attr = attr->next)
    free_wrappers(reinterpret_cast<xmlNode*>(attr));
}


} //namespace xmlpp
