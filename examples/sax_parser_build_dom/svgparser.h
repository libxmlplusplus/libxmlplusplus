// -*- C++ -*-

/* svgparser.h
 *
 * By Dan Dennedy <dan@dennedy.org> 
 *
 * Copyright (C) 2003 The libxml++ development team
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
 * License along with this library; if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __LIBXMLPP_SVGPARSER_H
#define __LIBXMLPP_SVGPARSER_H

#include <stack>
#include <glibmm/ustring.h>
#include <libxml++/libxml++.h>

namespace SVG {

class Parser : public xmlpp::SaxParser
{
public:
  Parser(xmlpp::Document& document);
  ~Parser() override;

protected:
  // SAX parser callbacks
  void on_start_document() override {};
  void on_end_document() override {};
  void on_start_element(const Glib::ustring& name,
                                const AttributeList& properties) override;
  void on_end_element(const Glib::ustring& name) override;
  void on_characters(const Glib::ustring& characters) override;
  void on_comment(const Glib::ustring& text) override;
  void on_warning(const Glib::ustring& text) override;
  void on_error(const Glib::ustring& text) override;
  void on_fatal_error(const Glib::ustring& text) override;
  void on_cdata_block(const Glib::ustring& text) override;

private:
  // context is a stack to keep track of parent node while the SAX parser
  // descends the tree
  std::stack<xmlpp::Element*> m_context;
  xmlpp::Document& m_doc;
};

} //namespace SVG

#endif //__LIBXMLPP_SVGPARSER_H
