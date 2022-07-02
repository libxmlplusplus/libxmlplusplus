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

#include <cstdlib>
#include <cstring> // std::memset()
#include <fstream>
#include <iostream>

#include "myparser.h"

int
main(int argc, char* argv[])
{
  std::string filepath;
  if(argc > 1 )
    filepath = argv[1]; //Allow the user to specify a different XML file to parse.
  else
    filepath = "example.xml";

  // Parse the entire document in one go:
  auto return_code = EXIT_SUCCESS;
  try
  {
    MySaxParser parser;
    parser.set_substitute_entities(true);
    parser.parse_file(filepath);
  }
  catch(const xmlpp::exception& ex)
  {
    std::cerr << "libxml++ exception: " << ex.what() << std::endl;
    return_code = EXIT_FAILURE;
  }

  // Incremental parsing, sometimes useful for network connections:
  try
  {
    std::cout << std::endl << "Incremental SAX Parser:" << std::endl;

    std::ifstream is(filepath.c_str());
    if (!is)
      throw xmlpp::exception("Could not open file " + filepath);

    char buffer[64];
    const size_t buffer_size = sizeof(buffer) / sizeof(char);

    //Parse the file:
    MySaxParser parser;
    parser.set_substitute_entities(true);
    do
    {
      std::memset(buffer, 0, buffer_size);
      is.read(buffer, buffer_size-1);
      if(is.gcount())
      {
        xmlpp::ustring input(buffer, buffer+is.gcount());
        parser.parse_chunk(input);
      }
    }
    while(is);

    parser.finish_chunk_parsing();
  }
  catch(const xmlpp::exception& ex)
  {
    std::cerr << "Incremental parsing, libxml++ exception: " << ex.what() << std::endl;
    return_code = EXIT_FAILURE;
  }

  return return_code;
}
