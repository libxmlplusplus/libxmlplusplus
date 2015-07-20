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

#include <libxml++/libxml++.h>
#include <stdlib.h>
#include <iostream>

Glib::ustring result_type_to_ustring(xmlpp::XPathResultType result_type)
{
  switch (result_type)
  {
    case xmlpp::XPATH_RESULT_NODESET: return "nodeset";
    case xmlpp::XPATH_RESULT_BOOLEAN: return "boolean";
    case xmlpp::XPATH_RESULT_NUMBER:  return "number";
    case xmlpp::XPATH_RESULT_STRING:  return "string";

    case xmlpp::XPATH_RESULT_UNDEFINED:
    default:
      return "undefined";
  }
}

bool xpath_test(const xmlpp::Node* node, const Glib::ustring& xpath)
{
  bool result = true;
  std::cout << std::endl; //Separate tests by an empty line.
  std::cout << "searching with xpath '" << xpath << "' in root node: " << std::endl;

  try
  {
    auto set = node->find(xpath);

    std::cout << set.size() << " nodes have been found:" << std::endl;

    //Print the structural paths and the values:
    for(const auto& child : set)
    {
      std::cout << " " << child->get_path();

      auto attribute = dynamic_cast<xmlpp::Attribute*>(child);
      if (attribute)
        std::cout << ", value=\"" << attribute->get_value() << "\"";

      auto content_node = dynamic_cast<xmlpp::ContentNode*>(child);
      if (content_node)
        std::cout << ", content=\"" << content_node->get_content() << "\"";

      auto entity_reference = dynamic_cast<xmlpp::EntityReference*>(child);
      if (entity_reference)
        std::cout << ", text=\"" << entity_reference->get_original_text() << "\"";

      auto element = dynamic_cast<xmlpp::Element*>(child);
      if (element)
      {
        auto text_node = element->get_child_text();
        if (text_node)
          std::cout << ", child_text=\"" << text_node->get_content() << "\"";
      }
      std::cout << std::endl;
    }
  }
  catch (const xmlpp::exception& ex)
  {
    std::cerr << "Exception caught from find: " << ex.what() << std::endl;
    result = false;
  }

  try
  {
    xmlpp::XPathResultType result_type;
    std::cout << "Boolean=" << (node->eval_to_boolean(xpath) ? "true" : "false")
              << ", Number=" << node->eval_to_number(xpath, &result_type)
              << ", String=\"" << node->eval_to_string(xpath) << "\"";
    std::cout << ", Result_type=" << result_type_to_ustring(result_type) << std::endl;
  }
  catch (const xmlpp::exception& ex)
  {
    std::cerr << "Exception caught from eval: " << ex.what() << std::endl;
    result = false;
  }
  return result;
}

int main(int argc, char* argv[])
{
  // Set the global C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  std::string filepath;
  if (argc > 1)
    filepath = argv[1]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";

  bool result = true;
  try
  {
    xmlpp::DomParser parser(filepath);
    if(parser)
    {
      const auto root = parser.get_document()->get_root_node(); //deleted by DomParser.

      if(root)
      {
        // Find all sections, no matter where:
        result &= xpath_test(root, "//section");

        // Find the title node (if there is one):
        result &= xpath_test(root, "title");

        // Find all literal text, in any paragraph:
        result &= xpath_test(root, "//para/literal");

        // Evaluate some XPath expressions with result types other than nodeset:
        // These tests shall fail.
        std::cerr << "Expecting 3 exceptions" << std::endl;
        result &= !xpath_test(root, "boolean(//para/literal)");
        result &= !xpath_test(root, "number(//para/literal)+2");
        result &= !xpath_test(root, "concat(string(title),\" !\")");

        // Don't find anything:
        result &= xpath_test(root, "/wont_find");

        std::cout << std::endl;

        // And finally test whether intra-document links are well-formed.
        // To be well-formed, the 'linkend' attribute must refer to
        // an element in terms of its 'id'.
        //
        // Find out whether there are linkend attributes that don't have
        // corresponding 'id's
        std::cout << "searching for unresolved internal references "
                  << "(see docbook manual):" << std::endl;

        result &= xpath_test(root, "//@id");
        result &= xpath_test(root, "//xref/@linkend");
      }
    }
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    result = false;
  }

  return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
