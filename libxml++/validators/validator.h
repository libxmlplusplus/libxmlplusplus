/* validator.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson,
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_VALIDATOR_H
#define __LIBXMLPP_VALIDATOR_H

#include <libxml++/nodes/element.h>
#include <libxml++/exceptions/validity_error.h>
#include <libxml++/exceptions/internal_error.h>

extern "C" {
  struct _xmlValidCtxt;
}

namespace xmlpp {

/** Base class for XML validators.
 */
class Validator : NonCopyable
{
public:
  LIBXMLPP_API Validator();
  LIBXMLPP_API ~Validator() override;

protected:
  LIBXMLPP_API
  virtual void initialize_valid();
  LIBXMLPP_API
  virtual void release_underlying();

  LIBXMLPP_API
  virtual void on_validity_error(const Glib::ustring& message);
  LIBXMLPP_API
  virtual void on_validity_warning(const Glib::ustring& message);

  //TODO: When we can break ABI/API, remove handleException() and make
  // handle_exception() protected virtual.
  LIBXMLPP_API
  virtual void handleException(const exception& e);
  LIBXMLPP_API
  virtual void check_for_exception();
  LIBXMLPP_API
  virtual void check_for_validity_messages();

  LIBXMLPP_API
  static void callback_validity_error(void* ctx, const char* msg, ...);
  LIBXMLPP_API
  static void callback_validity_warning(void* ctx, const char* msg, ...);

  _xmlValidCtxt* valid_;
  exception* exception_;
  Glib::ustring validate_error_;
  Glib::ustring validate_warning_; //Built gradually - used in an exception at the end of parsing.

private:
  /// To be called in an exception handler.
  LIBXMLPP_API
  void handle_exception();
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_H

