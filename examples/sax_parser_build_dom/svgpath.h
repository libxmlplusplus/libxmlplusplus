/* svgpath.h
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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __LIBXMLPP_SVGPATH_H
#define __LIBXMLPP_SVGPATH_H

#include <glibmm/ustring.h>
#include <libxml++/libxml++.h>
#include "svgelement.h"

namespace SVG {

class Path : public Element
{
public:
  Path(xmlNode* node)
    : Element(node)
    {}

  const Glib::ustring get_data() const
    {
      return get_attribute("d")->get_value();
    }
};

} //namespace SVG

#endif //__LIBXMLPP_SVGPATH_H
