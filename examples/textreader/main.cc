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

#include <libxml++/libxml++.h>
#include <cstdlib>
#include <iostream>

struct indent {
  int depth_;
  explicit indent(int depth): depth_(depth) {};
};

std::ostream & operator<<(std::ostream & o, indent const & in)
{
  for(int i = 0; i != in.depth_; ++i)
  {
    o << "  ";
  }
  return o;
}

std::ostream& operator<<(std::ostream& o, const std::optional<xmlpp::ustring>& s)
{
  o << s.value_or("{[(no value)]}");
  return o;
}

int main(int /* argc */, char** /* argv */)
{
  try
  {
    xmlpp::TextReader reader("example.xml");

    while(reader.read())
    {
      int depth = reader.get_depth();
      std::cout << indent(depth) << "--- node ---" << std::endl;
      std::cout << indent(depth) << "name: " << reader.get_name2() << std::endl;
      std::cout << indent(depth) << "depth: " << reader.get_depth() << std::endl;

      if(reader.has_attributes())
      {
        std::cout << indent(depth) << "attributes: " << std::endl;
        std::cout << indent(depth) << "attribute 0: " << reader.get_attribute2(0) << std::endl;
        std::cout << indent(depth) << "attribute 9: " << reader.get_attribute2(9) << std::endl;
        reader.move_to_first_attribute();
        do
        {
          std::cout << indent(depth) << "  " << reader.get_name2() << ": " << reader.get_value2() << std::endl;
        } while(reader.move_to_next_attribute());
        reader.move_to_element();
      }
      else
      {
        std::cout << indent(depth) << "no attributes" << std::endl;
      }

      std::cout << indent(depth) << "value: '" << reader.get_value2() << "'" << std::endl;
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

