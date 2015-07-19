#include "parse_error.h"

namespace xmlpp {

parse_error::parse_error(const Glib::ustring& message)
: exception(message)
{
}

parse_error::~parse_error() noexcept
{}

void parse_error::Raise() const
{
  throw *this;
}

exception* parse_error::Clone() const
{
  return new parse_error(*this);
}

} //namespace xmlpp

