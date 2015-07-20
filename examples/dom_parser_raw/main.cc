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

#include <libxml++/libxml++.h>

#include <iostream>
#include <fstream>
#include <glibmm/convert.h>
#include <stdlib.h>

void print_node(const xmlpp::Node* node, unsigned int indentation = 0)
{
  std::cout << std::endl; //Separate nodes by an empty line.
  
  std::cout << "Node name = " << node->get_name() << std::endl;

  //Recurse through child nodes:
  for(const auto& child : node->get_children())
  {
    print_node(child, indentation + 2); //recursive
  }
}

std::string read_from_disk(const std::string& filepath)
{
  std::string result;

  std::ifstream fStream(filepath.c_str());
  if(fStream.is_open())
  {
    while(!(fStream.eof()))
    {
      auto chTemp = fStream.get();
      if(!(fStream.eof()))
        result += chTemp;
    }
  }

  return result;
}

int main(int argc, char* argv[])
{
  // Set the global C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  std::string filepath;
  if(argc > 1 )
    filepath = argv[1]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";
  
  try
  {
    xmlpp::DomParser parser;
    parser.set_validate();
    parser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
   

    auto contents = read_from_disk(filepath);
    std::string contents_ucs2;

    try
    {
      contents_ucs2 = Glib::convert(contents, "UCS-2", "UTF-8");
    }
    catch(const Glib::Error& ex)
    {
      std::cerr << "Glib::convert failed: " << ex.what() << std::endl;
    }

    parser.parse_memory_raw((const unsigned char*)contents_ucs2.c_str(), contents_ucs2.size());

    //Look at the first few bytes, to see whether it really looks like UCS2.
    //Because UCS2 uses 2 bytes, we would expect every second byte to be zero for our simple example:
    std::cout << "First 10 bytes of the UCS-2 data:" << std::endl;
    for(std::string::size_type i = 0; (i < 10) && (i < contents_ucs2.size()); ++i)
    {
      std::cout << std::hex << (int)contents_ucs2[i] << ", ";
    }
    std::cout << std::endl;

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

