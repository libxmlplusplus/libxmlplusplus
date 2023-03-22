/* validator.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson,
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_VALIDATOR_H
#define __LIBXMLPP_VALIDATOR_H

#include <libxml++/noncopyable.h>
#include <libxml++/exceptions/validity_error.h>
#include <libxml++/exceptions/internal_error.h>
#include <cstdarg> // va_list
#include <memory> // std::unique_ptr
#include <string>

extern "C" {
  struct _xmlValidCtxt;
}

namespace xmlpp {

extern "C" {
  /** Type of function pointer to callback function with C linkage.
   * @newin{5,2}
   */
  using ValidatorCallbackCFuncType = void (*)(void* ctx, const char* msg, ...);
}

class Document;

/** Base class for XML validators.
 */
class Validator : public NonCopyable
{
public:
  LIBXMLPP_API Validator() noexcept;
  LIBXMLPP_API ~Validator() override;

  /** Parse a schema definition file or an external subset (DTD file).
   * @param filename The URL of the schema or the DTD.
   * @throws xmlpp::parse_error
   */
  LIBXMLPP_API
  virtual void parse_file(const std::string& filename) = 0;

  /** Parse a schema definition or a DTD from a string.
   * @param contents The schema definition or the DTD as a string.
   * @throws xmlpp::parse_error
   */
  LIBXMLPP_API
  virtual void parse_memory(const ustring& contents) = 0;

  /** Validate a document, using a previously parsed schema or DTD.
   * @param document Pointer to the document.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  LIBXMLPP_API
  virtual void validate(const Document* document) = 0;

  /** Test whether a schema or a DTD has been parsed.
   * For instance
   * @code
   * if (validator)
   *   do_something();
   * @endcode
   */
  LIBXMLPP_API
  explicit virtual operator bool() const noexcept = 0;

protected:
  LIBXMLPP_API
  virtual void initialize_context();
  LIBXMLPP_API
  virtual void release_underlying();

  LIBXMLPP_API
  virtual void on_validity_error(const ustring& message);
  LIBXMLPP_API
  virtual void on_validity_warning(const ustring& message);

  /// To be called in an exception handler.
  LIBXMLPP_API
  virtual void handle_exception();
  LIBXMLPP_API
  virtual void check_for_exception();
  LIBXMLPP_API
  virtual void check_for_validity_messages();

#ifndef LIBXMLXX_DISABLE_DEPRECATED
  /** @deprecated Use get_callback_validity_error_cfunc() instead. */
  LIBXMLPP_API
  static void callback_validity_error(void* ctx, const char* msg, ...);
  /** @deprecated Use get_callback_validity_warning_cfunc() instead. */
  LIBXMLPP_API
  static void callback_validity_warning(void* ctx, const char* msg, ...);
#endif // LIBXMLXX_DISABLE_DEPRECATED

  /** @newin{5,2} */
  LIBXMLPP_API
  static ValidatorCallbackCFuncType get_callback_validity_error_cfunc();
  /** @newin{5,2} */
  LIBXMLPP_API
  static ValidatorCallbackCFuncType get_callback_validity_warning_cfunc();

  /** @newin{5,2} */
  LIBXMLPP_API
  static void callback_error_or_warning(bool error, void* ctx,
                                        const char* msg, va_list var_args);

  std::unique_ptr<exception> exception_;
  // Built gradually - used in an exception at the end of validation.
  ustring validate_error_;
  ustring validate_warning_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_H
