// -*- C++ -*-

/* svgparser.cc
 *
 * By Dan Dennedy <dan@dennedy.org> 
 *
 * Copyright (C) 2003 The libxml++ development team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
#include <libxml/tree.h>
#include "svgparser.h"
#include "svgdocument.h"
#include "svgelement.h"
#include "svgpath.h"
#include "svggroup.h"

namespace SVG {

Parser::Parser(xmlpp::Document& document)
  : xmlpp::SaxParser(true), m_doc(document)
{
  set_substitute_entities(true);
}

Parser::~Parser()
{
}

void Parser::on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
{
  //This method replaces the normal libxml++ node
  //with an instance of a derived node.
  //This is not a recommended technique, and might not
  //work with future versions of libxml++.
  
  // Parse namespace prefix and save for later:
  Glib::ustring elementPrefix;
  Glib::ustring elementName = name;
  Glib::ustring::size_type idx = name.find(':'); 
  if (idx != Glib::ustring::npos) //If the separator was found
  {
    elementPrefix = name.substr(0, idx);
    elementName = name.substr(idx + 1);
  }

  xmlpp::Element* element_normal = nullptr;
  // Create a normal libxml++ node:
  if (m_doc.get_root_node() == nullptr)
  {
    // Create the root node if necessary:
    element_normal = m_doc.create_root_node(elementName);
  }
  else
  {
    // Create the other elements as child nodes of the last nodes:
    element_normal = m_context.top()->add_child(elementName);
  }

  // TODO: The following is a hack because it leverages knowledge of libxml++
  // implementation rather than interface - specifically that deleting the C++
  // instance will leave the underlying C instance intact and part of the libxml DOM tree.
  //
  // Delete the xmlpp::Element created above so we can link the libxml2
  // node with the derived Element object we create below.
  auto node = element_normal->cobj(); //Save it for later.
  delete element_normal;
  element_normal = nullptr;

  // TODO: Again, this requires knowledge of the libxml++ implemenation -
  // specifically that the base xmlpp::Node() constructor will reassociate
  // the underyling C instance with this new C++ instance, by seeting _private.
  //
  // Construct a custom Element based upon prefix and name.
  // This will then be deleted by libxml++, just as libxml++ would normally have
  // deleted its own node.
  // TODO: Don't delete the original (above) if it isn't one of these node names.
  xmlpp::Element* element_derived = nullptr;
  if (elementName == "g")
    element_derived = new SVG::Group(node);
  else if (elementName == "path")
    element_derived = new SVG::Path(node);
  else
    element_derived = new SVG::Element(node);

  if(element_derived)
  {
     //Set the context, so that child nodes will be added to this node,
     //until on_end_element():
     m_context.push(element_derived);

    // Copy the attributes form the old node to the new derived node:
    // In theory, you could change the attributes here.
    for(const auto& attr_pair : attributes)
    {
      const auto attr_name = attr_pair.name;
      const auto attr_value = attr_pair.value;
      const auto idx_colon = attr_name.find(':');
      if (idx_colon == Glib::ustring::npos) // If the separator was not found.
      {
        if (attr_name == "xmlns") // This is a namespace declaration.
        {
          //There is no second part, so this is a default namespace declaration.
          element_derived->set_namespace_declaration(attr_value);
        }
        else
        {
          //This is just an attribute value:
          element_derived->set_attribute(attr_name, attr_value);
        }
      }
      else
      {
        //The separator was found:
        auto prefix = attr_name.substr(0, idx_colon);
        auto suffix = attr_name.substr(idx_colon + 1);
        if (prefix == "xmlns") // This is a namespace declaration.
          element_derived->set_namespace_declaration(attr_value, suffix);
        else
        {
          //This is a namespaced attribute value.
          //(The namespace must have been declared already)
          auto attr = element_derived->set_attribute(suffix, attr_value);
          attr->set_namespace(prefix); //alternatively, we could have specified the whole name to set_attribute().
        }
      }
    }

    // We have to set the element namespace after the attributes because
    // an attribute might declare the namespace used in the actual element's name.
    if (!elementPrefix.empty())
      element_derived->set_namespace(elementPrefix);
  }
}

void Parser::on_end_element(const Glib::ustring& /* name */)
{
  // This causes the next child elements to be added to the sibling, not this node.
  m_context.pop();
}

void Parser::on_characters(const Glib::ustring& text)
{
  if(!m_context.empty())
    m_context.top()->add_child_text(text);
}

void Parser::on_comment(const Glib::ustring& text)
{
  if(!m_context.empty())
    m_context.top()->add_child_comment(text);
  else
    m_doc.add_comment(text);
}

void Parser::on_warning(const Glib::ustring& text)
{
  std::cout << "on_warning(): " << text << std::endl;
}

void Parser::on_error(const Glib::ustring& text)
{
  std::cout << "on_error(): " << text << std::endl;
}

void Parser::on_fatal_error(const Glib::ustring& text)
{
  std::cout << "on_fatal_error(): " << text << std::endl;
}

void Parser::on_cdata_block(const Glib::ustring& text)
{
  if(!m_context.empty())
    m_context.top()->add_child_cdata(text);
}

}
