#include "validity_error.h"

namespace xmlpp {

validity_error::validity_error(const Glib::ustring& message)
: parse_error(message)
{
}

validity_error::~validity_error() noexcept
{}

} //namespace xmlpp
