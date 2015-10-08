/* document.cc
 * this file is part of libxml++
 *
 * copyright (C) 2003 by the libxml++ development team
 *
 * this file is covered by the GNU Lesser General Public License,
 * which should be included with libxml++ as the file COPYING.
 */

#include <libxml++/document.h>
#include <libxml++/dtd.h>
#include <libxml++/attribute.h>
#include <libxml++/nodes/element.h>
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/keepblanks.h>
#include <libxml++/io/ostreamoutputbuffer.h>

#include <libxml/tree.h>
#include <libxml/xinclude.h>
#include <libxml/parser.h> // XML_PARSE_NOXINCNODE

#include <iostream>
#include <map>

namespace // anonymous
{
typedef std::map<xmlpp::Node*, xmlElementType> NodeMap;

// Find all C++ wrappers of 'node' and its descendants.
// Compare xmlpp::Node::free_wrappers().
void find_wrappers(xmlNode* node, NodeMap& node_map)
{
  if (!node)
    return;
    
  //If an entity declaration contains an entity reference, there can be cyclic
  //references between entity declarations and entity references. (It's not
  //a tree.) We must avoid an infinite recursion.
  //Compare xmlFreeNode(), which frees the children of all node types except
  //XML_ENTITY_REF_NODE.
  if (node->type != XML_ENTITY_REF_NODE)
  {
    // Walk the children list.
    for (auto child = node->children; child; child = child->next)
      find_wrappers(child, node_map);
  }

  // Find the local one
  bool has_attributes = true;
  switch (node->type)
  {
    // Node types that have no attributes.
    // These are not represented by struct xmlNode.
    case XML_DTD_NODE:
    case XML_ATTRIBUTE_NODE:
    case XML_ELEMENT_DECL:
    case XML_ATTRIBUTE_DECL:
    case XML_ENTITY_DECL:
    case XML_DOCUMENT_NODE:
      has_attributes = false;
      break;
    default:
      break;
  }

  if (node->_private)
    node_map[static_cast<xmlpp::Node*>(node->_private)] = node->type;

  if (!has_attributes)
    return;

  //Walk the attributes list.
  //Note that some "derived" structs have a different layout, so 
  //_xmlNode::properties would be a nonsense value, leading to crashes
  //(and shown as valgrind warnings), so we return above, to avoid 
  //checking it here.
  for (auto attr = node->properties; attr; attr = attr->next)
    find_wrappers(reinterpret_cast<xmlNode*>(attr), node_map);
}

// Remove from 'node_map' the pointers to the C++ wrappers that are found with
// unchanged type in 'node' and its descendants.
void remove_found_wrappers(xmlNode* node, NodeMap& node_map)
{
  if (!node)
    return;
    
  if (node->type != XML_ENTITY_REF_NODE)
  {
    // Walk the children list.
    for (auto child = node->children; child; child = child->next)
      remove_found_wrappers(child, node_map);
  }

  // Find the local one
  bool has_attributes = true;
  switch (node->type)
  {
    // Node types that have no attributes
    case XML_DTD_NODE:
    case XML_ATTRIBUTE_NODE:
    case XML_ELEMENT_DECL:
    case XML_ATTRIBUTE_DECL:
    case XML_ENTITY_DECL:
    case XML_DOCUMENT_NODE:
      has_attributes = false;
      break;
    default:
      break;
  }

  if (node->_private)
  {
    const auto iter =
      node_map.find(static_cast<xmlpp::Node*>(node->_private));
    if (iter != node_map.end())
    {
      if (iter->second == node->type)
        node_map.erase(iter);
      else
        node->_private = nullptr; // node->type has changed. The wrapper will be deleted.
    }
  }

  if (!has_attributes)
    return;

  // Walk the attributes list.
  for (auto attr = node->properties; attr; attr = attr->next)
    remove_found_wrappers(reinterpret_cast<xmlNode*>(attr), node_map);

}
} // anonymous

namespace xmlpp
{

static const char* get_encoding_or_utf8(const Glib::ustring& encoding)
{
  if(encoding.empty())
  {
    //If we don't specify this to the xmlDocDump* functions (using nullptr instead),
    //then some other encoding is used, causing them to fail on non-ASCII characters.
    return "UTF-8";
  }
  else
    return encoding.c_str();
}


Document::Init::Init()
{
  xmlInitParser(); //Not always necessary, but necessary for thread safety.
}

Document::Init::~Init()
{
  //We don't call this because it breaks libxml generally and should only be
  //called at the very end of a process, such as at the end of a main().
  //libxml might still be used by the application, so we don't want to break
  //that.
  //This is important even here, which usually happens only when the library
  //is unloaded, because that might happen during normal application use,
  //if the application does dynamic library loading, for instance to load
  //plugins.
  //See http://xmlsoft.org/html/libxml-parser.html#xmlCleanupParser
  //xmlCleanupParser(); //As per xmlInitParser(), or memory leak will happen.
}

Document::Init Document::init_;

Document::Document(const Glib::ustring& version)
  : impl_(xmlNewDoc((const xmlChar*)version.c_str()))
{
  if (!impl_)
    throw internal_error("Could not create Document.");
  impl_->_private = this;
}

Document::Document(xmlDoc* doc)
  : impl_(doc)
{
  impl_->_private = this;
}

Document::~Document()
{
  Node::free_wrappers(reinterpret_cast<xmlNode*>(impl_));
  xmlFreeDoc(impl_);
}

Glib::ustring Document::get_encoding() const
{
  Glib::ustring encoding;
  if(impl_->encoding)
    encoding = (const char*)impl_->encoding;

  return encoding;
}

Dtd* Document::get_internal_subset() const
{
  auto dtd = xmlGetIntSubset(impl_);
  if(!dtd)
    return nullptr;

  if(!dtd->_private)
    dtd->_private = new Dtd(dtd);

  return reinterpret_cast<Dtd*>(dtd->_private);
}

void Document::set_internal_subset(const Glib::ustring& name,
                                   const Glib::ustring& external_id,
                                   const Glib::ustring& system_id)
{
  auto dtd = xmlCreateIntSubset(impl_,
				   (const xmlChar*)name.c_str(),
				   external_id.empty() ? nullptr : (const xmlChar*)external_id.c_str(),
				   system_id.empty() ? nullptr : (const xmlChar*)system_id.c_str());

  if (dtd && !dtd->_private)
    dtd->_private = new Dtd(dtd);
}

Element* Document::get_root_node() const
{
  auto root = xmlDocGetRootElement(impl_);
  if(root == nullptr)
    return nullptr;
  else
  {
    Node::create_wrapper(root);
    return reinterpret_cast<Element*>(root->_private);
  }
}

Element* Document::create_root_node(const Glib::ustring& name,
                                    const Glib::ustring& ns_uri,
                                    const Glib::ustring& ns_prefix)
{
  auto node = xmlNewDocNode(impl_, nullptr, (const xmlChar*)name.c_str(), nullptr);
  if (!node)
    throw internal_error("Could not create root element node " + name);

  node = xmlDocSetRootElement(impl_, node);
  if (node)
  {
    // An old root element node has been replaced.
    Node::free_wrappers(node);
    xmlFreeNode(node);
  }

  auto element = get_root_node();

  if( !ns_uri.empty() && element )
  {
    element->set_namespace_declaration(ns_uri, ns_prefix);
    element->set_namespace(ns_prefix);
  }

  return element;
}

Element* Document::create_root_node_by_import(const Node* node,
					      bool recursive)
{
  if (!node)
    return nullptr;

  //Create the node, by copying:
  auto imported_node = xmlDocCopyNode(const_cast<xmlNode*>(node->cobj()), impl_, recursive);
  if (!imported_node)
  {
    throw exception("Unable to copy the node that shall be imported");
  }

  auto old_node = xmlDocSetRootElement(impl_, imported_node);
  if (old_node)
  {
    // An old root element node has been replaced.
    Node::free_wrappers(old_node);
    xmlFreeNode(old_node);
  }

  return get_root_node();
}

CommentNode* Document::add_comment(const Glib::ustring& content)
{
  auto child = xmlNewComment((const xmlChar*)content.c_str());
 
  // Use the result, because child can be freed when merging text nodes:
  auto node = xmlAddChild((xmlNode*)impl_, child);
  if (!node)
  {
    xmlFreeNode(child);
    throw internal_error("Could not add comment node \"" + content + "\"");
  }
  Node::create_wrapper(node);
  return static_cast<CommentNode*>(node->_private);
}

ProcessingInstructionNode* Document::add_processing_instruction(
  const Glib::ustring& name, const Glib::ustring& content)
{
  auto child = xmlNewDocPI(impl_, (const xmlChar*)name.c_str(), (const xmlChar*)content.c_str());
  auto node = xmlAddChild((xmlNode*)impl_, child);
  if (!node)
  {
    xmlFreeNode(child);
    throw internal_error("Could not add processing instruction node " + name);
  }
  Node::create_wrapper(node);
  return static_cast<ProcessingInstructionNode*>(node->_private);
}

void Document::write_to_file(const Glib::ustring& filename, const Glib::ustring& encoding)
{
  do_write_to_file(filename, encoding, false);
}

void Document::write_to_file_formatted(const Glib::ustring& filename, const Glib::ustring& encoding)
{
  do_write_to_file(filename, encoding, true);
}

Glib::ustring Document::write_to_string(const Glib::ustring& encoding)
{
  return do_write_to_string(encoding, false);
}

Glib::ustring Document::write_to_string_formatted(const Glib::ustring& encoding)
{
  return do_write_to_string(encoding, true);
}

void Document::write_to_stream(std::ostream& output, const Glib::ustring& encoding)
{
  do_write_to_stream(output, encoding.empty()?get_encoding():encoding, false);
}

void Document::write_to_stream_formatted(std::ostream& output, const Glib::ustring& encoding)
{
  do_write_to_stream(output, encoding.empty()?get_encoding():encoding, true);
}

void Document::do_write_to_file(
    const Glib::ustring& filename,
    const Glib::ustring& encoding,
    bool format)
{
  KeepBlanks k(KeepBlanks::Default);
  xmlIndentTreeOutput = format?1:0;
  xmlResetLastError();
  const int result = xmlSaveFormatFileEnc(filename.c_str(), impl_,
    get_encoding_or_utf8(encoding), format?1:0);

  if(result == -1)
  {
    throw exception("do_write_to_file() failed.\n" + format_xml_error());
  }
}

Glib::ustring Document::do_write_to_string(
    const Glib::ustring& encoding,
    bool format)
{
  KeepBlanks k(KeepBlanks::Default);
  xmlIndentTreeOutput = format?1:0;
  xmlChar* buffer = nullptr;
  int length = 0;

  xmlResetLastError();
  xmlDocDumpFormatMemoryEnc(impl_, &buffer, &length,
    get_encoding_or_utf8(encoding), format?1:0);

  if(!buffer)
  {
    throw exception("do_write_to_string() failed.\n" + format_xml_error());
  }

  // Create a Glib::ustring copy of the buffer

  // Here we force the use of Glib::ustring::ustring( InputIterator begin, InputIterator end )
  // instead of Glib::ustring::ustring( const char*, size_type ) because it
  // expects the length of the string in characters, not in bytes.
  Glib::ustring result( reinterpret_cast<const char *>(buffer), reinterpret_cast<const char *>(buffer + length) );

  // Deletes the original buffer
  xmlFree(buffer);
  // Return a copy of the string
  return result;
}

void Document::do_write_to_stream(std::ostream& output, const Glib::ustring& encoding, bool format)
{
  // TODO assert document encoding is UTF-8 if encoding is different than UTF-8
  OStreamOutputBuffer buffer(output, encoding);
  xmlResetLastError();
  const int result = xmlSaveFormatFileTo(buffer.cobj(), impl_,
    get_encoding_or_utf8(encoding), format ? 1 : 0);
  
  if(result == -1)
  {
    throw exception("do_write_to_stream() failed.\n" + format_xml_error());
  }
}

void Document::set_entity_declaration(const Glib::ustring& name, XmlEntityType type,
                              const Glib::ustring& publicId, const Glib::ustring& systemId,
                              const Glib::ustring& content)
{
  auto entity = xmlAddDocEntity( impl_, (const xmlChar*) name.c_str(), type,
    publicId.empty() ? nullptr : (const xmlChar*)publicId.c_str(),
    systemId.empty() ? nullptr : (const xmlChar*)systemId.c_str(),
    (const xmlChar*) content.c_str() );
  if (!entity)
    throw internal_error("Could not add entity declaration " + name);
}

int Document::process_xinclude(bool generate_xinclude_nodes)
{
  NodeMap node_map;

  auto root = xmlDocGetRootElement(impl_);

  find_wrappers(root, node_map);

  xmlResetLastError();
  const int n_substitutions = xmlXIncludeProcessTreeFlags(root,
    generate_xinclude_nodes ? 0 : XML_PARSE_NOXINCNODE);

  remove_found_wrappers(reinterpret_cast<xmlNode*>(impl_), node_map);

  // Delete wrappers of nodes that have been deleted or have had their type changed.
  for (auto& the_pair : node_map)
  {
    auto node = the_pair.first;

    switch (the_pair.second)
    {
    case XML_DTD_NODE:
      delete reinterpret_cast<Dtd*>(node);
      break;
    case XML_DOCUMENT_NODE:
      delete reinterpret_cast<Document*>(node);
      break;
    default:
      delete node; // Node*
      break;
    }
  }

  if (n_substitutions < 0)
  {
    throw exception("Couldn't process XInclude\n" + format_xml_error());
  }

  return n_substitutions;
}

_xmlEntity* Document::get_entity(const Glib::ustring& name)
{
  return xmlGetDocEntity(impl_, (const xmlChar*) name.c_str());
}

_xmlDoc* Document::cobj()
{
  return impl_;
}

const _xmlDoc* Document::cobj() const
{
  return impl_;
}

} //namespace xmlpp
