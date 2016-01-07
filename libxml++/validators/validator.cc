/* validator.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/exceptions/wrapped_exception.h"
#include "libxml++/validators/validator.h"

#include <libxml/parser.h>

#include <cstdarg> //For va_list.
#include <memory> //For unique_ptr.

namespace xmlpp {

Validator::Validator() noexcept
: exception_(nullptr)
{
}

Validator::~Validator()
{
  release_underlying();
}

void Validator::initialize_context()
{
  //Clear these temporary buffers:
  validate_error_.erase();
  validate_warning_.erase();
}

void Validator::release_underlying()
{
}

void Validator::on_validity_error(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  validate_error_ += message;
}

void Validator::on_validity_warning(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  validate_warning_ += message;
}

void Validator::check_for_validity_messages()
{
  Glib::ustring msg(exception_ ? exception_->what() : "");
  bool validity_msg = false;

  if (!validate_error_.empty())
  {
    validity_msg = true;
    msg += "\nValidity error:\n" + validate_error_;
    validate_error_.erase();
  }

  if (!validate_warning_.empty())
  {
    validity_msg = true;
    msg += "\nValidity warning:\n" + validate_warning_;
    validate_warning_.erase();
  }

  if (validity_msg)
    exception_.reset(new validity_error(msg));
}

void Validator::callback_validity_error(void* valid_, const char* msg, ...)
{
  auto validator = static_cast<Validator*>(valid_);

  if(validator)
  {
    //Convert the ... to a string:
    va_list arg;
    va_start(arg, msg);
    const Glib::ustring buff = format_printf_message(msg, arg);
    va_end(arg);

    try
    {
      validator->on_validity_error(buff);
    }
    catch (...)
    {
      validator->handle_exception();
    }
  }
}

void Validator::callback_validity_warning(void* valid_, const char* msg, ...)
{
  auto validator = static_cast<Validator*>(valid_);

  if(validator)
  {
    //Convert the ... to a string:
    va_list arg;
    va_start(arg, msg);
    const Glib::ustring buff = format_printf_message(msg, arg);
    va_end(arg);

    try
    {
      validator->on_validity_warning(buff);
    }
    catch (...)
    {
      validator->handle_exception();
    }
  }
}

void Validator::handle_exception()
{
  try
  {
    throw; // Re-throw current exception
  }
  catch (const exception& e)
  {
    exception_.reset(e.clone());
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch (...)
  {
    exception_.reset(new wrapped_exception(std::current_exception()));
  }
#else
  catch (const std::exception& e)
  {
    exception_.reset(new exception(e.what()));
  }
  catch (...)
  {
    exception_.reset(new exception("An exception was thrown that is not derived from std::exception or xmlpp::exception.\n"
      "It could not be caught and rethrown because this platform does not support std::exception_ptr."));
  }
#endif

  // Don't delete the DTD validation context or schema validation context
  // while validating. It would cause accesses to deallocated memory in libxml2
  // functions after the return from Validator::callback_validity_...().
  // Parser::handle_exception() calls xmlStopParser(), but there is no
  // xmlStopValidator() or similar function to call here.
  // We don't throw the exception here, since it would have to pass through
  // C functions. That's not guaranteed to work. It might work, but it depends
  // on the C compiler and the options used when building libxml2.

  //release_underlying();
}

void Validator::check_for_exception()
{
  check_for_validity_messages();

  if (exception_)
  {
    std::unique_ptr<exception> tmp(std::move(exception_));
    tmp->raise();
  }
}

} // namespace xmlpp
