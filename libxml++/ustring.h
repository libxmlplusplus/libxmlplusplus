/*
 * Copyright 2020, The libsigc++ Development Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef __LIBXMLPP_USTRING_H
#define __LIBXMLPP_USTRING_H

#include <string>

namespace xmlpp
{

/** This is just an indication to the programmer that the string contains UTF-8.
 * On the other hand, when std::string appears in the libxml++ API, the
 * encoding of the string may be UTF-8 or may be some other encoding.
 */
using ustring = std::string;

} // namespace xmlpp

#endif //__LIBXMLPP_USTRING_H

