## Copyright (C) 2015 The libxml++ development team
##
## This file is part of libxml++.
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library. If not, see <http://www.gnu.org/licenses/>.

## LIBXMLXX_CXX_HAS_EXCEPTION_PTR()
##
## Test whether std::exception_ptr, std::current_exception() and
## std::rethrow_exception() are defined.
##
## On success, #define LIBXMLXX_HAVE_EXCEPTION_PTR to 1.
##
AC_DEFUN([LIBXMLXX_CXX_HAS_EXCEPTION_PTR],
[
  AC_CACHE_CHECK(
    [whether C++ library supports std::exception_ptr],
    [libxmlxx_cv_cxx_has_exception_ptr],
  [
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
    [[
      #include <exception>
    ]],[[
      try
      {
        throw "custom error";
      }
      catch(...)
      {
        std::exception_ptr ep = std::current_exception();
        std::rethrow_exception(ep);
      }
    ]])],
      [libxmlxx_cv_cxx_has_exception_ptr='yes'],
      [libxmlxx_cv_cxx_has_exception_ptr='no']
    )
  ])

  AS_IF([test "x${libxmlxx_cv_cxx_has_exception_ptr}" = 'xyes'],
  [AC_DEFINE([LIBXMLXX_HAVE_EXCEPTION_PTR], [1], [Defined if the C++ library supports std::exception_ptr.])])
])
