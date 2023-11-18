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
#include <sstream>
#include <stdexcept>

class MySaxParser : public xmlpp::SaxParser
{
public:
  bool throw_on_start_doc = true;

protected:
  void on_start_document() override
  {
    if (throw_on_start_doc)
      throw std::runtime_error("some custom runtime exception");
  }
  void on_error(const xmlpp::ustring& /* text */) override
  {
    throw std::runtime_error("on_error() called");
  }
};

int main()
{
  {
    MySaxParser parser;

    bool exceptionThrown = false;
    try
    {
      // Depending on the libxml2 version, MySaxParser::on_start_document()
      // may or may not be called before MySaxParser::on_error().
      parser.throw_on_start_doc = false;
      parser.parse_chunk("<?");
      parser.finish_chunk_parsing();
    }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
    catch(const std::runtime_error& e)
#else
    catch(const xmlpp::exception& e)
#endif
    {
      exceptionThrown = true;
      assert(e.what() == xmlpp::ustring("on_error() called"));
    }
    assert(exceptionThrown);

    // Try parsing a stream now.
    exceptionThrown = false;
    try
    {
      parser.throw_on_start_doc = true;
      std::stringstream ss("<root></root>");
      parser.parse_stream(ss);
    }
    catch(const xmlpp::parse_error& e)
    {
      // An "Attempt to start a second parse while a parse is in progress." parse
      // error should not have been thrown.
      assert(false);
    }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
    catch(const std::runtime_error& e)
#else
    catch(const xmlpp::exception& e)
#endif
    {
      exceptionThrown = true;
      assert(e.what() == xmlpp::ustring("some custom runtime exception"));
    }
    assert(exceptionThrown);
  }

  return EXIT_SUCCESS;
}
