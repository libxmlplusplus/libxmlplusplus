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

  std::string dtdfilepath;
  if(argc > 1)
    dtdfilepath = argv[1]; //Allow the user to specify a different dtd file to use.
  else
    dtdfilepath = "example.dtd";

  int return_code = EXIT_SUCCESS;
  xmlpp::Document document;
  /* auto nodeRoot = */document.create_root_node("incorrect");

  try
  {
    xmlpp::DtdValidator validator( dtdfilepath );

    try
    {
      validator.validate( &document ); // Shall fail
      std::cerr << "Validation successful (not expected)" << std::endl;
      return_code = EXIT_FAILURE;
    }
    catch (const xmlpp::exception& ex)
    {
      std::cout << "Error validating the document (expected)" << std::endl;
      std::cout << ex.what() << std::endl;
    }

    /* auto nodeRoot2 = */document.create_root_node("example");
    auto child = document.get_root_node()->add_child("examplechild");
    child->set_attribute("id", "an_id");
    child->add_child("child_of_child");

    try
    {
      validator.validate( &document );
      std::cout << "Validation successful" << std::endl;
    }
    catch (const xmlpp::exception& ex)
    {
      std::cerr << "Error validating the document" << std::endl;
      std::cerr << ex.what() << std::endl;
      return_code = EXIT_FAILURE;
    }
  }
  catch (const xmlpp::exception& ex)
  {
    std::cerr << "Error parsing the dtd" << std::endl;
    std::cerr << ex.what() << std::endl;
    return_code = EXIT_FAILURE;
  }
  return return_code;
}

