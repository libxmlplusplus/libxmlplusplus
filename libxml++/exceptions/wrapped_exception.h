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

#ifndef __LIBXMLPP_WRAPPED_EXCEPTION_H
#define __LIBXMLPP_WRAPPED_EXCEPTION_H

#include <exception>

#include <libxml++/exceptions/exception.h>

namespace xmlpp
{

#ifndef DOXYGEN_SHOULD_SKIP_THIS
class wrapped_exception : public exception
{
public:
  explicit wrapped_exception(std::exception_ptr exception_ptr);
  ~wrapped_exception() noexcept override;

  void Raise() const override;
  exception* Clone() const override;

private:
  std::exception_ptr exception_ptr_;
};
#endif //DOXYGEN_SHOULD_SKIP_THIS

} // namespace xmlpp

#endif // __LIBXMLPP_WRAPPED_EXCEPTION_H
