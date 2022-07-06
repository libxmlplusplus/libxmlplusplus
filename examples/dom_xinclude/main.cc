/* Copyright (C) 2012 The libxml++ development team
 *
 * This file is part of libxml++.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <iostream>
#include <libxml++/libxml++.h>

void print_node(const xmlpp::Node* node, unsigned int indentation = 0)
{
  const std::string indent(indentation, ' ');

  const auto nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  const auto nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
  const auto nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

  //Let's ignore the indenting - you don't always want to do this.
  if (nodeText && nodeText->is_white_space())
    return;

  const auto nodename = node->get_name();

  if (!nodeText && !nodeComment && !nodename.empty()) //Let's not say "name: text".
  {
    const auto namespace_prefix = node->get_namespace_prefix();

    std::cout << indent << "Node name = ";
    if (!namespace_prefix.empty())
      std::cout << namespace_prefix << ":";
    std::cout << nodename << std::endl;
  }
  else if (nodeText) //Let's say when it's text.
  {
    std::cout << indent << "Text Node" << std::endl;
  }

  //Treat the various node types differently:
  if (nodeText)
  {
    std::cout << indent << "text = \"" << nodeText->get_content() << "\"" << std::endl;
  }
  else if (nodeComment)
  {
    std::cout << indent << "comment = " << nodeComment->get_content() << std::endl;
  }
  else if (nodeContent)
  {
    std::cout << indent << "content = " << nodeContent->get_content() << std::endl;
  }
  else if (auto nodeElement = dynamic_cast<const xmlpp::Element*>(node))
  {
    //A normal Element node:
    std::cout << indent << "     Element line = " << node->get_line() << std::endl;

    //Print attributes:
    for (const auto& attribute : nodeElement->get_attributes())
    {
      const auto namespace_prefix = attribute->get_namespace_prefix();

      std::cout << indent << "  Attribute ";
      if (!namespace_prefix.empty())
        std::cout << namespace_prefix  << ":";
      std::cout << attribute->get_name() << " = " << attribute->get_value() << std::endl;
    }

    const auto attribute = nodeElement->get_attribute("title");
    if (attribute)
    {
      std::cout << indent << "title = " << attribute->get_value() << std::endl;
    }
  }
  else if (dynamic_cast<const xmlpp::XIncludeStart*>(node))
  {
    std::cout << indent << "     " << "XIncludeStart line = " << node->get_line() << std::endl;
  }
  else if (dynamic_cast<const xmlpp::XIncludeEnd*>(node))
  {
    std::cout << indent << "     " << "XIncludeEnd" << std::endl;
  }

  if (!nodeContent)
  {
    //Recurse through child nodes:
    for(const auto& child : node->get_children())
    {
      print_node(child, indentation + 2); //recursive
    }
  }
}

int main(int argc, char* argv[])
{
  bool validate = false;
  bool set_throw_messages = false;
  bool throw_messages = false;
  bool substitute_entities = true;
  bool generate_xinclude_nodes = true;
  bool fixup_base_uris = true;

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
      case 'X':
        generate_xinclude_nodes = false;
        break;
      case 'B':
        fixup_base_uris = false;
        break;
     default:
       std::cout << "Usage: " << argv[0] << " [options]... [filename]" << std::endl
                 << "       -v  Validate" << std::endl
                 << "       -t  Throw messages in an exception" << std::endl
                 << "       -e  Write messages to stderr" << std::endl
                 << "       -E  Do not substitute entities" << std::endl
                 << "       -X  Do not generate XInclude nodes" << std::endl
                 << "       -B  Do not fix up base URIs" << std::endl;
       return EXIT_FAILURE;
     }
     argi++;
  }
  std::string filepath;
  if (argc > argi)
    filepath = argv[argi]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";

  try
  {
    xmlpp::DomParser parser;
    parser.set_validate(validate);
    if (set_throw_messages)
      parser.set_throw_messages(throw_messages);
    //We can have the text resolved/unescaped automatically.
    parser.set_substitute_entities(substitute_entities);
    parser.parse_file(filepath);
    if (parser)
    {
      //Walk the tree:
      auto pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
      print_node(pNode);

      std::cout << std::endl << ">>>>> Number of XInclude substitutions: "
                << parser.get_document()->process_xinclude(
                     generate_xinclude_nodes, fixup_base_uris)
                << std::endl << std::endl;

      std::cout << ">>>>> After XInclude processing with xmlpp::Document::process_xinclude(): "
                << std::endl << std::endl;
      pNode = parser.get_document()->get_root_node();
      print_node(pNode);

      // xmlpp::Document::write_to_string() does not write XIncludeStart and
      // XIncludeEnd nodes.
      const auto whole = parser.get_document()->write_to_string();
      std::cout << std::endl << whole << std::endl;
    }

    parser.set_xinclude_options(true, generate_xinclude_nodes, fixup_base_uris);
    parser.parse_file(filepath);
    if (parser)
    {
      std::cout << ">>>>> After XInclude processing with xmlpp::DomParser::parse_file(): "
                << std::endl << std::endl;
      print_node(parser.get_document()->get_root_node());

      const auto whole = parser.get_document()->write_to_string();
      std::cout << std::endl << whole << std::endl;
    }
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

