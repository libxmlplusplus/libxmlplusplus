// -*- C++ -*-

/* myparser.h
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
 * License along with this library; if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __LIBXMLPP_EXAMPLES_MYPARSER_H
#define __LIBXMLPP_EXAMPLES_MYPARSER_H

#include <libxml++/libxml++.h>

class MySaxParser : public xmlpp::SaxParser
{
public:
  MySaxParser();
  ~MySaxParser() override;

protected:
  //overrides:
  void on_start_document() override;
  void on_end_document() override;
  void on_start_element(const xmlpp::ustring& name,
                                const AttributeList& properties) override;
  void on_end_element(const xmlpp::ustring& name) override;
  void on_characters(const xmlpp::ustring& characters) override;
  void on_comment(const xmlpp::ustring& text) override;
  void on_warning(const xmlpp::ustring& text) override;
  void on_error(const xmlpp::ustring& text) override;
  void on_fatal_error(const xmlpp::ustring& text) override;
};


#endif //__LIBXMLPP_EXAMPLES_MYPARSER_H
