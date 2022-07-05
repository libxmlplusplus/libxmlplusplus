#include "internal_error.h"

namespace xmlpp {

internal_error::internal_error(const ustring& message)
: exception(message)
{
}

internal_error::~internal_error() noexcept = default;

void internal_error::raise() const
{
  throw *this;
}

exception* internal_error::clone() const
{
  return new internal_error(*this);
}

} //namespace xmlpp
