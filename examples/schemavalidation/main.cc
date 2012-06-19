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
#include <iostream>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  // Set the global C and C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  std::string schemafilepath("example.xsd"),
              docfilepath("example.xml");

  if(argc!=1 && argc!=3)
  {
    std::cout << "usage : " << argv[0] << " [document schema]" << std::endl;
    return EXIT_FAILURE;
  }

  if(argc == 3)
  {
    docfilepath = argv[1];
    schemafilepath = argv[2];
  }

  try
  {
    xmlpp::DomParser       parser(docfilepath);
    xmlpp::SchemaValidator validator(schemafilepath);

    try
    {
      validator.validate( parser.get_document() );
      std::cout << "Valid document" << std::endl;
    }
    catch( const xmlpp::validity_error& error)
    {
      std::cerr << "Error validating the document" << std::endl;
      std::cerr << error.what();
      return EXIT_FAILURE;
    }
  }
  catch( const xmlpp::parse_error& error)
  {
    std::cerr << "Error parsing the schema: " << error.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

