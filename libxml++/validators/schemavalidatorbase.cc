/* Copyright (C) 2014 The libxml++ development team
 *
 * This file is part of libxml++.
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
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "libxml++/validators/schemavalidatorbase.h"
#include "libxml++/schemabase.h"

namespace xmlpp
{

SchemaValidatorBase::SchemaValidatorBase() noexcept = default;

SchemaValidatorBase::~SchemaValidatorBase()
{
  release_underlying();
}

void SchemaValidatorBase::release_underlying()
{
  Validator::release_underlying();
}

void SchemaValidatorBase::initialize_context()
{
  Validator::initialize_context();
}

} // namespace xmlpp
