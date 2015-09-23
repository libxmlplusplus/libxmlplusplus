#include "internal_error.h"

namespace xmlpp {

internal_error::internal_error(const Glib::ustring& message)
: exception(message)
{
}

internal_error::~internal_error() noexcept
{}

} //namespace xmlpp
