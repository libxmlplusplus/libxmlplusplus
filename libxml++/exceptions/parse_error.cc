#include "parse_error.h"

namespace xmlpp {

parse_error::parse_error(const Glib::ustring& message)
: exception(message)
{
}

parse_error::~parse_error() noexcept
{}

} //namespace xmlpp
