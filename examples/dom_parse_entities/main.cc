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

#include <libxml++/libxml++.h>
#include <iostream>
#include <fstream>
#include <cstdlib>

std::ostream& operator<<(std::ostream& o, const std::optional<xmlpp::ustring>& s)
{
  o << s.value_or("{[(no value)]}");
  return o;
}

void print_node(const xmlpp::Node* node, bool substitute_entities, unsigned int indentation = 0)
{
  const std::string indent(indentation, ' ');
  std::cout << std::endl; //Separate nodes by an empty line.

  if (substitute_entities)
  {
    // Entities have been substituted. Print the text nodes.
    const auto nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
    if (nodeText && !nodeText->is_white_space())
    {
      std::cout << indent << "text = " << nodeText->get_content2() << std::endl;
    }
  }
  else
  {
    // Entities have not been substituted. Print the entity reference nodes.
    const auto nodeEntityReference = dynamic_cast<const xmlpp::EntityReference*>(node);
    if (nodeEntityReference)
    {
      std::cout << indent << "entity reference name = " << nodeEntityReference->get_name2() << std::endl;
      std::cout << indent << "  resolved text = " << nodeEntityReference->get_resolved_text2() << std::endl;
      std::cout << indent << "  original text = " << nodeEntityReference->get_original_text2() << std::endl;
    }
  } // end if (substitute_entities)

  const auto nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
  if(!nodeContent)
  {
    //Recurse through child nodes:
    for(const auto& child : node->get_children())
    {
      print_node(child, substitute_entities, indentation + 2); //recursive
    }
  }
}

int main(int argc, char* argv[])
{
  std::string filepath;
  if(argc > 1 )
    filepath = argv[1]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";

  // Parse first without, then with, entity substitution.
  int return_code = EXIT_SUCCESS;
  bool substitute_entities = false;
  while (true)
  {
    if (substitute_entities)
      std::cout << std::endl << "<<< With entity substitution >>>" << std::endl;
    else
      std::cout << std::endl << "<<< Without entity substitution >>>" << std::endl;

    try
    {
      xmlpp::DomParser parser;
      parser.set_validate();
      parser.set_substitute_entities(substitute_entities);

      // Two ways of reading the XML file.
      if (substitute_entities)
        parser.parse_file(filepath);
      else
      {
        std::ifstream instream(filepath.c_str());
        if (!instream)
          throw xmlpp::internal_error("Could not open file " + filepath);
        instream >> parser;
      }
      if(parser)
      {
        //Walk the tree:
        const auto pNode = parser.get_document()->get_root_node(); //deleted by DomParser.
        print_node(pNode, substitute_entities);
      }
    }
    catch(const std::exception& ex)
    {
      std::cerr << "Exception caught: " << ex.what() << std::endl;
      return_code = EXIT_FAILURE;
    }

    if (substitute_entities) break;

    substitute_entities = true;
  }

  return return_code;
}
