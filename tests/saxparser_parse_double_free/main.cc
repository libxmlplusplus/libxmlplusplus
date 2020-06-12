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

#include <libxml++/libxml++.h>

#include <cassert>
#include <cstdlib>
#include <stdexcept>

class OnCdataBlockTestParser : public xmlpp::SaxParser
{
protected:
  void on_cdata_block(const xmlpp::ustring& text) override
  {
    assert(text == xmlpp::ustring("some CDATA"));
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
    assert(e.what() == xmlpp::ustring("on_cdata_block runtime exception"));
  }
  assert(exceptionThrown);
}


class OnCharactersTestParser : public xmlpp::SaxParser
{
protected:
  void on_characters(const xmlpp::ustring& characters) override
  {
    assert(characters == xmlpp::ustring("abc"));
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
    assert(e.what() == xmlpp::ustring("on_characters runtime exception"));
  }
  assert(exceptionThrown);
}


class OnCommentTestParser : public xmlpp::SaxParser
{
protected:
  void on_comment(const xmlpp::ustring& text) override
  {
    assert(text == xmlpp::ustring("a comment"));
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
    assert(e.what() == xmlpp::ustring("on_comment runtime exception"));
  }
  assert(exceptionThrown);
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
    assert(e.what() == xmlpp::ustring("on_end_document runtime exception"));
  }
  assert(exceptionThrown);
}


class OnEndElementTestParser : public xmlpp::SaxParser
{
protected:
  void on_end_element(const xmlpp::ustring& name) override
  {
    assert(name == xmlpp::ustring("a:root"));
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
    assert(e.what() == xmlpp::ustring("on_end_element runtime exception"));
  }
  assert(exceptionThrown);
}


class OnEntityDeclarationTestParser : public xmlpp::SaxParser
{
protected:
  void on_entity_declaration(const xmlpp::ustring& name, xmlpp::XmlEntityType /* type */,
    const xmlpp::ustring& /* publicId */, const xmlpp::ustring& /* systemId */,
    const xmlpp::ustring& content) override
  {
    assert(name == xmlpp::ustring("number"));
    assert(content.c_str() == xmlpp::ustring("42"));
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
    assert(e.what() == xmlpp::ustring("on_entity_declaration runtime exception"));
  }
  assert(exceptionThrown);
}


class OnErrorTestParser : public xmlpp::SaxParser
{
protected:
  void on_error(const xmlpp::ustring& /* text */) override
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
    assert(e.what() == xmlpp::ustring("on_error runtime exception"));
  }
  assert(exceptionThrown);
}


class OnGetEntityTestParser : public xmlpp::SaxParser
{
public:
  OnGetEntityTestParser()
    : xmlpp::SaxParser(true)
  {
  }
protected:
  _xmlEntity* on_get_entity(const xmlpp::ustring& name) override
  {
    assert(name == xmlpp::ustring("number"));
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
    assert(e.what() == xmlpp::ustring("on_get_entity runtime exception"));
  }
  assert(exceptionThrown);
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
    assert(e.what() == xmlpp::ustring("on_start_document runtime exception"));
  }
  assert(exceptionThrown);
}


class OnStartElementTestParser : public xmlpp::SaxParser
{
protected:
  void on_start_element(const xmlpp::ustring& name, const xmlpp::SaxParser::AttributeList& attributes) override
  {
    assert(name == xmlpp::ustring("b:root"));
    assert(attributes.size() == 2);
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
    assert(e.what() == xmlpp::ustring("on_start_element runtime exception"));
  }
  assert(exceptionThrown);
}


int main()
{
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
