/* Copyright (C) 2015  The libxml++ development team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <config.h>
#include <libxml++/libxml++.h>

#include <cstdlib>
#include <glibmm.h>
#include <stdexcept>

class OnCdataBlockTestParser : public xmlpp::SaxParser
{
protected:
  void on_cdata_block(const Glib::ustring& text) override
  {
    g_assert_cmpstr(text.c_str(), ==, "some CDATA");
    throw std::runtime_error("on_cdata_block runtime exception");
  }
};

void test_on_cdata_block()
{
  OnCdataBlockTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<root><![CDATA[some CDATA]]></root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_cdata_block runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnCharactersTestParser : public xmlpp::SaxParser
{
protected:
  void on_characters(const Glib::ustring& characters) override
  {
    g_assert_cmpstr(characters.c_str(), ==, "abc");
    throw std::runtime_error("on_characters runtime exception");
  }
};

void test_on_characters()
{
  OnCharactersTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<root>abc</root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_characters runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnCommentTestParser : public xmlpp::SaxParser
{
protected:
  void on_comment(const Glib::ustring& text) override
  {
    g_assert_cmpstr(text.c_str(), ==, "a comment");
    throw std::runtime_error("on_comment runtime exception");
  }
};

void test_on_comment()
{
  OnCommentTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<root><!--a comment--></root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_comment runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnEndDocumentTestParser : public xmlpp::SaxParser
{
protected:
  void on_end_document() override
  {
    throw std::runtime_error("on_end_document runtime exception");
  }
};

void test_on_end_document()
{
  OnEndDocumentTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<root></root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_end_document runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnEndElementTestParser : public xmlpp::SaxParser
{
protected:
  void on_end_element(const Glib::ustring& name) override
  {
    g_assert_cmpstr(name.c_str(), ==, "a:root");
    throw std::runtime_error("on_end_element runtime exception");
  }
};

void test_on_end_element()
{
  OnEndElementTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<a:root xmlns:a=\"urn:test\"></a:root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_end_element runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnEntityDeclarationTestParser : public xmlpp::SaxParser
{
protected:
  void on_entity_declaration(const Glib::ustring& name, xmlpp::XmlEntityType /* type */,
    const Glib::ustring& /* publicId */, const Glib::ustring& /* systemId */,
    const Glib::ustring& content) override
  {
    g_assert_cmpstr(name.c_str(), ==, "number");
    g_assert_cmpstr(content.c_str(), ==, "42");
    throw std::runtime_error("on_entity_declaration runtime exception");
  }
};

void test_on_entity_declaration()
{
  OnEntityDeclarationTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<!DOCTYPE MyDocument [<!ENTITY number \"42\">]><root></root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_entity_declaration runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnErrorTestParser : public xmlpp::SaxParser
{
protected:
  void on_error(const Glib::ustring& /* text */) override
  {
    throw std::runtime_error("on_error runtime exception");
  }
};

void test_on_error()
{
  OnErrorTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<root>&unknown;</root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_error runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnGetEntityTestParser : public xmlpp::SaxParser
{
public:
  OnGetEntityTestParser()
    : xmlpp::SaxParser(true)
  {
  }
protected:
  _xmlEntity* on_get_entity(const Glib::ustring& name) override
  {
    g_assert_cmpstr(name.c_str(), ==, "number");
    throw std::runtime_error("on_get_entity runtime exception");
  }
};

void test_on_get_entity()
{
  OnGetEntityTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<!DOCTYPE MyDocument [<!ENTITY number \"42\">]><root>&number;</root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_get_entity runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnStartDocumentTestParser : public xmlpp::SaxParser
{
protected:
  void on_start_document() override
  {
    throw std::runtime_error("on_start_document runtime exception");
  }
};

void test_on_start_document()
{
  OnStartDocumentTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<root></root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_start_document runtime exception");
  }
  g_assert_true(exceptionThrown);
}


class OnStartElementTestParser : public xmlpp::SaxParser
{
protected:
  void on_start_element(const Glib::ustring& name, const xmlpp::SaxParser::AttributeList& attributes) override
  {
    g_assert_cmpstr(name.c_str(), ==, "b:root");
    g_assert_cmpint(attributes.size(), ==, 2);
    throw std::runtime_error("on_start_element runtime exception");
  }
};

void test_on_start_element()
{
  OnStartElementTestParser parser;
  bool exceptionThrown = false;
  try
  {
    parser.parse_memory("<b:root xmlns:b=\"urn:test\" someattr=\"test\"></b:root>");
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch(const std::runtime_error& e)
#else
  catch(const xmlpp::exception& e)
#endif
  {
    exceptionThrown = true;
    g_assert_cmpstr(e.what(), ==, "on_start_element runtime exception");
  }
  g_assert_true(exceptionThrown);
}


int main()
{
  Glib::init();

  test_on_cdata_block();
  test_on_characters();
  test_on_comment();
  test_on_end_document();
  test_on_end_element();
  test_on_entity_declaration();
  test_on_error();
  // TODO test on_fatal_error()
  // This is not currently possible because the fatalError handler is never called;
  // error is called for all errors.
  // http://xmlsoft.org/html/libxml-parser.html#fatalErrorSAXFunc
  test_on_get_entity();
  // TODO test on_internal_subset()
  test_on_start_document();
  test_on_start_element();
  // TODO test on_warning(), on_validity_error(), on_validity_warning()

  return EXIT_SUCCESS;
}
