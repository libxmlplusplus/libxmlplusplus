/* validator.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson,
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_VALIDATOR_H
#define __LIBXMLPP_VALIDATOR_H

#ifdef _MSC_VER //Ignore warnings about the Visual C++ Bug, where we can not do anything
#pragma warning (disable : 4786)
#endif

#include <libxml++/noncopyable.h>
#include <libxml++/exceptions/validity_error.h>
#include <libxml++/exceptions/internal_error.h>
#include <memory> // std::unique_ptr
#include <string>

extern "C" {
  struct _xmlValidCtxt;
}

namespace xmlpp {

class Document;

/** Base class for XML validators.
 */
class Validator : public NonCopyable
{
public:
  Validator() noexcept;
  ~Validator() override;

  /** Parse a schema definition file or an external subset (DTD file).
   * @param filename The URL of the schema or the DTD.
   * @throws xmlpp::parse_error
   */
  virtual void parse_file(const std::string& filename) = 0;

  /** Parse a schema definition or a DTD from a string.
   * @param contents The schema definition or the DTD as a string.
   * @throws xmlpp::parse_error
   */
  virtual void parse_memory(const Glib::ustring& contents) = 0;

  /** Validate a document, using a previously parsed schema or DTD.
   * @param document Pointer to the document.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  virtual void validate(const Document* document) = 0;

  /** Test whether a schema or a DTD has been parsed.
   * For instance
   * @code
   * if (validator)
   *   do_something();
   * @endcode
   */
  explicit virtual operator bool() const noexcept = 0;

protected:
  virtual void initialize_context();
  virtual void release_underlying();

  virtual void on_validity_error(const Glib::ustring& message);
  virtual void on_validity_warning(const Glib::ustring& message);

  /// To be called in an exception handler.
  virtual void handle_exception();
  virtual void check_for_exception();
  virtual void check_for_validity_messages();

  static void callback_validity_error(void* ctx, const char* msg, ...);
  static void callback_validity_warning(void* ctx, const char* msg, ...);

  std::unique_ptr<exception> exception_;
  // Built gradually - used in an exception at the end of validation.
  Glib::ustring validate_error_;
  Glib::ustring validate_warning_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATOR_H
