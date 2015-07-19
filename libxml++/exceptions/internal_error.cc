#include "internal_error.h"


namespace xmlpp {

internal_error::internal_error(const Glib::ustring& message)
: exception(message)
{
}

internal_error::~internal_error() noexcept
{}

void internal_error::Raise() const
{
  throw *this;
}

exception * internal_error::Clone() const
{
  return new internal_error(*this);
}

} //namespace xmlpp


