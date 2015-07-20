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

int
main(int /* argc */, char** /* argv */)
{
  // Set the global C and C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  try
  {
    xmlpp::Document document;
    document.set_internal_subset("example_xml_doc", "", "example_xml_doc.dtd");
    document.set_entity_declaration("example1", xmlpp::XML_INTERNAL_GENERAL_ENTITY,
      "", "example_xml_doc.dtd", "Entity content");
    document.add_processing_instruction("application1", "This is an example document");
    document.add_comment("First comment");

    //foo is the default namespace prefix.
    auto nodeRoot = document.create_root_node("exampleroot", "http://foo", "foo"); //Declares the namespace and uses its prefix for this node
    nodeRoot->set_namespace_declaration("http://foobar", "foobar"); //Also associate this prefix with this namespace: 

    nodeRoot->set_child_text("\n");
    auto nodeChild = nodeRoot->add_child("examplechild");

    //Associate prefix with namespace:
    nodeChild->set_namespace_declaration("http://bar", "bar"); 
     
    nodeChild->set_namespace("bar"); //So it will be bar::examplechild.
    nodeChild->set_attribute("id", "1", "foo"); //foo is the namespace prefix. You could also just use a name of foo:id".
    nodeChild->set_child_text("\nSome content\n");
    nodeChild->add_child_comment("Some comments");
    nodeChild->add_child_entity_reference("example1");
    nodeChild->add_child_entity_reference("#x20ac"); // €
    nodeChild->add_child_text("\n");
    nodeChild->add_child_processing_instruction("application1", "This is an example node");
    nodeChild->add_child_text("\n");
    nodeChild->add_child("child_of_child", "bar");

    nodeChild = nodeRoot->add_child("examplechild", "foobar"); //foobar is the namespace prefix
    nodeChild->set_attribute("id", "2", "foobar"); //foobar is the namespace prefix.

    auto whole = document.write_to_string();
    std::cout << "XML built at runtime: " << std::endl << whole << std::endl;
    std::cout << "namespace of root node: " << nodeRoot->get_namespace_uri() << std::endl;
  }
  catch(const std::exception& ex)
  {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

