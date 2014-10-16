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
#include <cstdlib>

int main(int argc, char* argv[])
{
  // Set the global C and C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  std::string docfilepath("example.xml");
  std::string xsdschemafilepath("example.xsd");
  std::string rngschemafilepath("example.rng");

  if (argc != 1 && argc != 4)
  {
    std::cout << "usage : " << argv[0] << " [document schema.xsd schema.rng]" << std::endl;
    return EXIT_FAILURE;
  }

  if (argc == 4)
  {
    docfilepath = argv[1];
    xsdschemafilepath = argv[2];
    rngschemafilepath = argv[3];
  }

  Glib::ustring phase;
  try
  {
#ifndef LIBXMLXX_DISABLE_DEPRECATED
    phase = "XML"; // XSD schema, old validator class
    xmlpp::SchemaValidator schemavalidator(xsdschemafilepath);
#endif // LIBXMLXX_DISABLE_DEPRECATED

    phase = "XSD";
    xmlpp::XsdValidator xsdvalidator(xsdschemafilepath);

    phase = "RelaxNG";
    xmlpp::RelaxNGValidator rngvalidator(rngschemafilepath);

    try
    {
      phase = "parsing";
      xmlpp::DomParser parser(docfilepath);

#ifndef LIBXMLXX_DISABLE_DEPRECATED
      phase = "XML validating";
      schemavalidator.validate(parser.get_document());
      std::cout << "Valid document, SchemaValidator" << std::endl;
#endif // LIBXMLXX_DISABLE_DEPRECATED

      phase = "XSD validating";
      xsdvalidator.validate(parser.get_document());
      std::cout << "Valid document, XsdValidator" << std::endl;

      phase = "RelaxNG validating";
      rngvalidator.validate(parser.get_document());
      std::cout << "Valid document, RelaxNGValidator" << std::endl;
    }
    catch (const xmlpp::exception& ex)
    {
      std::cerr << "Error " << phase << " the document" << std::endl;
      std::cerr << ex.what() << std::endl;
      return EXIT_FAILURE;
    }
  }
  catch (const xmlpp::exception& ex)
  {
    std::cerr << "Error parsing the " << phase << " schema" << std::endl;
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
