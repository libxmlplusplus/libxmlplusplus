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

#include "wrapped_exception.h"

namespace xmlpp
{

#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR

wrapped_exception::wrapped_exception(std::exception_ptr exception_ptr)
  : exception("Wrapped exception"), exception_ptr_(std::move(exception_ptr))
{
}

wrapped_exception::~wrapped_exception() noexcept
{
}

void wrapped_exception::raise() const
{
  std::rethrow_exception(exception_ptr_);
}

exception* wrapped_exception::clone() const
{
  return new wrapped_exception(exception_ptr_);
}

#endif // LIBXMLXX_HAVE_EXCEPTION_PTR

} // namespace xmlpp
