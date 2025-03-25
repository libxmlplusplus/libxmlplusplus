/* internal_error.h
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

#ifndef __LIBXMLPP_INTERNAL_ERROR_H
#define __LIBXMLPP_INTERNAL_ERROR_H

#include <libxml++/exceptions/exception.h>

namespace xmlpp {

class internal_error : public exception
{
public:
  LIBXMLPP_API
  explicit internal_error(const ustring& message);
  LIBXMLPP_API ~internal_error() noexcept override;

  LIBXMLPP_API void raise() const override;
  LIBXMLPP_API exception* clone() const override;
};

} // namespace xmlpp

#endif // __LIBXMLPP_INTERNAL_ERROR_H
