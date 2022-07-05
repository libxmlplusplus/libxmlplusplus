/* myparser.cc
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

#include "myparser.h"

#include <iostream>

MySaxParser::MySaxParser()
  : xmlpp::SaxParser(true /* override on_get_entity */)
{
}

MySaxParser::~MySaxParser() = default;

void MySaxParser::on_start_document()
{
  std::cout << "on_start_document()" << std::endl;
}

void MySaxParser::on_end_document()
{
  std::cout << "on_end_document()" << std::endl;
}

void MySaxParser::on_start_element(const xmlpp::ustring& name,
                                   const AttributeList& attributes)
{
  std::cout << "node name=" << name << std::endl;

  // Print attributes:
  for(const auto& attr_pair : attributes)
  {
    std::cout << "  Attribute " << attr_pair.name << " = " << attr_pair.value << std::endl;
  }
}

void MySaxParser::on_end_element(const xmlpp::ustring& /* name */)
{
  std::cout << "on_end_element()" << std::endl;
}

void MySaxParser::on_characters(const xmlpp::ustring& text)
{
  std::cout << "on_characters(): " << text << std::endl;
}

void MySaxParser::on_comment(const xmlpp::ustring& text)
{
  std::cout << "on_comment(): " << text << std::endl;
}

void MySaxParser::on_warning(const xmlpp::ustring& text)
{
  std::cout << "on_warning(): " << text << std::endl;
}

void MySaxParser::on_error(const xmlpp::ustring& text)
{
  std::cout << "on_error(): " << text << std::endl;
}

void MySaxParser::on_fatal_error(const xmlpp::ustring& text)
{
  std::cout << "on_fatal_error(): " << text << std::endl;
}

_xmlEntity* MySaxParser::on_get_entity(const xmlpp::ustring& name)
{
  std::cout << "on_get_entity(): " << name << std::endl;

  //Call the base class:
  return SaxParser::on_get_entity(name);
}

void MySaxParser::on_entity_declaration(const xmlpp::ustring& name, xmlpp::XmlEntityType type, const xmlpp::ustring& publicId, const xmlpp::ustring& systemId, const xmlpp::ustring& content)
{
  std::cout << "on_entity_declaration(): name=" << name << ", publicId=" << publicId << ", systemId=" << systemId << ", content=" << content << std::endl;

  //Call the base class:
  SaxParser::on_entity_declaration(name, type, publicId, systemId, content);
}
