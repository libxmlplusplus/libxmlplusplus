#include "validity_error.h"

namespace xmlpp {

validity_error::validity_error(const ustring& message)
: parse_error(message)
{
}

validity_error::~validity_error() noexcept = default;

void validity_error::raise() const
{
  throw *this;
}

exception* validity_error::clone() const
{
  return new validity_error(*this);
}

} //namespace xmlpp
