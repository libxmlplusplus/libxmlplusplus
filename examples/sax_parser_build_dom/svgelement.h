/* svgelement.h
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

#ifndef __LIBXMLPP_SVGELEMENT_H
#define __LIBXMLPP_SVGELEMENT_H

#include "libxml++/ustring.h"
#include <libxml++/libxml++.h>
#include <libxml/tree.h>

namespace SVG {

class Element : public xmlpp::Element
{
public:

  explicit Element(xmlNode* node);
  ~Element() override;

  // example custom methods
  void set_style(const xmlpp::ustring& style);
  const xmlpp::ustring get_style() const;

  // TODO: add custom methods

private:
  // TODO: add custom properties

};

} //namespace SVG

#endif //__LIBXMLPP_SVGELEMENT_H
