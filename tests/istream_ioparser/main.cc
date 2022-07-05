/* Copyright (C) 2022  The libxml++ development team
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

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>

class test_streambuf : public std::streambuf
{
public:
  test_streambuf() :
    uflow_calls(0),
    underflow_calls(0),
    ofs(0),
    buf("<root>\n</root>")
    {
    }

protected:
  /* Simulate some kind of streambuf impl that doesn't setg() */
  int_type underflow() override
  {
    ++underflow_calls;
    if (ofs >= (sizeof(buf)-1))
      return traits_type::eof();
    return traits_type::to_int_type(buf[ofs]);
  }

  int_type uflow() override
  {
    ++uflow_calls;
    if (ofs >= (sizeof(buf)-1))
      return traits_type::eof();
    return traits_type::to_int_type(buf[ofs++]);
  }

  std::streamsize showmanyc() override
  {
    if (ofs >= (sizeof(buf)-1))
      return traits_type::eof();
    return sizeof(buf)-1-ofs;
  }

  std::streamsize xsgetn(char_type* s, std::streamsize count) override
  {
    auto n = std::min(count, static_cast<std::streamsize>(sizeof(buf)-1-ofs));
    memcpy(s, buf + ofs, n);
    ofs += n;
    return n;
  }

public:
  int uflow_calls;
  int underflow_calls;

private:
  size_t ofs;
  char buf[15];
};

class MySaxParser : public xmlpp::SaxParser {
public:
  bool saw_root = false;
protected:
  void on_start_document() override
  {
      saw_root = false;
  }
  void on_end_element(const xmlpp::ustring &name) override
  {
    if (name == "root")
      saw_root = true;
  }
};

int main()
{
  { // Check DomParser works well with normal and custom istreams
    xmlpp::DomParser parser;
    try
    {
      std::stringstream ss("<root></root>");
      parser.parse_stream(ss);
    }
    catch(...)
    {
      assert(false);
    }

    {
      auto doc = parser.get_document();
      assert(doc->get_root_node()->get_name() == "root");
    }

    {
      test_streambuf buf;
      try {
        std::istream is(&buf);
        parser.parse_stream(is);
      } catch (...) {
          assert(false);
      }
      assert(buf.underflow_calls + buf.uflow_calls < 3);
      auto doc = parser.get_document();
      assert(doc->get_root_node()->get_name() == "root");
    }
  }
  { // Check SaxParser works well with normal and custom istreams.
    MySaxParser parser;
    try
    {
      std::stringstream ss("<root></root>");
      parser.parse_stream(ss);
    }
    catch(...)
    {
      assert(false);
    }
    assert(parser.saw_root);

    {
      test_streambuf buf;
      try {
        std::istream is(&buf);
        parser.parse_stream(is);
      } catch (...) {
          assert(false);
      }
      assert(buf.underflow_calls + buf.uflow_calls < 3);
      assert(parser.saw_root);
    }
  }
  assert(true);
  return EXIT_SUCCESS;
}
