// -*- C++ -*-

/* svgdocument.cc
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

#include "svgdocument.h"

namespace SVG {

SVG::Element* Document::get_root() const
{
   return dynamic_cast<SVG::Element*>(get_root_node()); // RTTI
}

} //namespace SVG
