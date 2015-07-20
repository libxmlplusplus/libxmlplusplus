// -*- C++ -*-

/* main.cc
 *
 * Copyright (C) 2002 The libxml++ development team
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../testutilities.h"
#include <libxml++/libxml++.h>
#include <iostream>
#include <cstdlib>

void print_node(const xmlpp::Node* node, unsigned int indentation = 0)
{
  const Glib::ustring indent(indentation, ' ');
  std::cout << std::endl; //Separate nodes by an empty line.
  
  const auto nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const auto nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const auto nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

  if(nodeText && nodeText->is_white_space()) //Let's ignore the indenting - you don't always want to do this.
    return;
    
  const auto nodename = node->get_name();

  if(!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
  {
    const auto namespace_prefix = node->get_namespace_prefix();

    std::cout << indent << "Node name = ";
    if(!namespace_prefix.empty())
      std::cout << CatchConvertError(namespace_prefix) << ":";
    std::cout << CatchConvertError(nodename) << std::endl;
  }
  else if(nodeText) //Let's say when it's text. - e.g. let's say what that white space is.
  {
    std::cout << indent << "Text Node" << std::endl;
  }

  //Treat the various node types differently: 
  if(nodeText)
  {
    std::cout << indent << "text = \"" << CatchConvertError(nodeText->get_content()) << "\"" << std::endl;
  }
  else if(nodeComment)
  {
    std::cout << indent << "comment = " << CatchConvertError(nodeComment->get_content()) << std::endl;
  }
  else if(nodeContent)
  {
    std::cout << indent << "content = " << CatchConvertError(nodeContent->get_content()) << std::endl;
  }
  else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    //A normal Element node:

    //line() works only for ElementNodes.
    std::cout << indent << "     line = " << node->get_line() << std::endl;

    //Print attributes:
    const auto attributes = nodeElement->get_attributes();
    for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
    {
      const auto attribute = *iter;
      const auto namespace_prefix = attribute->get_namespace_prefix();

      std::cout << indent << "  Attribute ";
      if(!namespace_prefix.empty())
        std::cout << CatchConvertError(namespace_prefix) << ":";
      std::cout << CatchConvertError(attribute->get_name()) << " = "
                << CatchConvertError(attribute->get_value()) << std::endl;
    }

    const auto attribute = nodeElement->get_attribute("title");
    if(attribute)
    {
      std::cout << indent;
      if (dynamic_cast<const xmlpp::AttributeNode*>(attribute))
        std::cout << "AttributeNode ";
      else if (dynamic_cast<const xmlpp::AttributeDeclaration*>(attribute))
        std::cout << "AttributeDeclaration ";
      std::cout << "title = " << CatchConvertError(attribute->get_value()) << std::endl;
    }
  }
  
  if(!nodeContent)
  {
    //Recurse through child nodes:
    auto list = node->get_children();
    for(const auto& child : node->get_children())
    {
      print_node(child, indentation + 2); //recursive
    }
  }
}

int main(int argc, char* argv[])
{
  // Set the global C++ locale to the user-specified locale. Then we can
  // hopefully use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  bool validate = false;
  bool set_throw_messages = false;
  bool throw_messages = false;
  bool substitute_entities = true;
  bool include_default_attributes = false;

  int argi = 1;
  while (argc > argi && *argv[argi] == '-') // option
  {
    switch (*(argv[argi]+1))
    {
      case 'v':
        validate = true;
        break;
      case 't':
        set_throw_messages = true;
        throw_messages = true;
        break;
      case 'e':
        set_throw_messages = true;
        throw_messages = false;
        break;
      case 'E':
        substitute_entities = false;
        break;
      case 'a':
        include_default_attributes = true;
        break;
     default:
       std::cout << "Usage: " << argv[0] << " [-v] [-t] [-e] [filename]" << std::endl
                 << "       -v  Validate" << std::endl
                 << "       -t  Throw messages in an exception" << std::endl
                 << "       -e  Write messages to stderr" << std::endl
                 << "       -E  Do not substitute entities" << std::endl
                 << "       -a  Include default attributes in the node tree" << std::endl;
       return EXIT_FAILURE;
     }
     argi++;
  }
  std::string filepath;
  if(argc > argi)
    filepath = argv[argi]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";
 
  try
  {
    xmlpp::DomParser parser;
    if (validate)
      parser.set_validate();
    if (set_throw_messages)
      parser.set_throw_messages(throw_messages);
    //We can have the text resolved/unescaped automatically.
    parser.set_substitute_entities(substitute_entities);
    parser.set_include_default_attributes(include_default_attributes);
    parser.parse_file(filepath);
    if(parser)
    {
      //Walk the tree:
      const auto pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
      print_node(pNode);
    }
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

