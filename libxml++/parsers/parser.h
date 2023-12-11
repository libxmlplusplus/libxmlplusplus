/* parser.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_PARSER_H
#define __LIBXMLPP_PARSER_H

#include <libxml++/nodes/element.h>
#include <libxml++/exceptions/validity_error.h>
#include <libxml++/exceptions/internal_error.h>

#include <string>
#include <istream>
#include <cstdarg> // va_list
#include <memory> // std::unique_ptr

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlParserCtxt;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp {

extern "C" {
  /** Type of function pointer to callback function with C linkage.
   * @newin{5,2}
   */
  using ParserCallbackCFuncType = void (*)(void* ctx, const char* msg, ...);
}

/** XML parser.
 *
 * Abstract base class for DOM parser and SAX parser.
 */
class Parser : public NonCopyable
{
public:
  LIBXMLPP_API Parser();
  LIBXMLPP_API ~Parser() override;

  using size_type = unsigned int;

  /** By default, the parser will not validate the XML file.
   * @param val Whether the document should be validated.
   */
  LIBXMLPP_API
  void set_validate(bool val = true) noexcept;

  /** See set_validate().
   * @returns Whether the parser will validate the XML file.
   */
  LIBXMLPP_API
  bool get_validate() const noexcept;

  /** Set whether the parser will automatically substitute entity references with the text of the entities' definitions.
   * For instance, this affects the text returned by ContentNode::get_content().
   * By default, the parser will not substitute entities, so that you do not lose the entity reference information.
   * @param val Whether entities will be substitued.
   */
  LIBXMLPP_API
  void set_substitute_entities(bool val = true) noexcept;

  /** See set_substitute_entities().
   * @returns Whether entities will be substituted during parsing.
   */
  LIBXMLPP_API
  bool get_substitute_entities() const noexcept;

  /** Set whether the parser will collect and throw error and warning messages.
   *
   * If messages are collected, they are included in an exception thrown at the
   * end of parsing.
   *
   * - DOM parser
   *
   *   If the messages are not collected, they are written on stderr.
   *   The messages written on stderr are slightly different, and may be
   *   preferred in a program started from the command-line. The default, if
   *   set_throw_messages() is not called, is to collect and throw messages.
   *
   * - SAX parser
   *
   *   If the messages are not collected, the error handling on_*() methods in
   *   the user's SAX parser subclass are called. This is the default, if
   *   set_throw_messages() is not called.
   *
   * @newin{2,36}
   *
   * @param val Whether messages will be collected and thrown in an exception.
   */
  LIBXMLPP_API
  void set_throw_messages(bool val = true) noexcept;

  /** See set_throw_messages().
   *
   * @newin{2,36}
   *
   * @returns Whether messages will be collected and thrown in an exception.
   */
  LIBXMLPP_API
  bool get_throw_messages() const noexcept;

  /** Set whether default attribute values from the DTD shall be included in the node tree.
   * If set, attributes not assigned a value in the XML file, but with a default value
   * in the DTD file, will be included in the node tree that the parser creates.
   * These attributes will be represented by AttributeNode instances (not AttributeDeclaration
   * instances), just like attributes which are assigned a value in the XML file.
   *
   * @newin{2,38}
   *
   * @param val Whether attributes with default values will be included in the node tree.
   */
  LIBXMLPP_API
  void set_include_default_attributes(bool val = true) noexcept;

  /** See set_include_default_attributes().
   *
   * @newin{2,38}
   *
   * @returns Whether attributes with default values will be included in the node tree.
   */
  LIBXMLPP_API
  bool get_include_default_attributes() const noexcept;

  /** Set and/or clear parser option flags.
   * See the libxml2 documentation,
   * [enum xmlParserOption](https://gnome.pages.gitlab.gnome.org/libxml2/devhelp/libxml2-parser.html#xmlParserOption),
   * for a list of parser options.
   * This method overrides other methods that set parser options, such as set_validate(),
   * set_substitute_entities() and set_include_default_attributes(). Use set_parser_options()
   * only if no other method can set the parser options you want.
   *
   * @newin{2,38}
   *
   * @param set_options Set bits correspond to flags that shall be set during parsing.
   * @param clear_options Set bits correspond to flags that shall be cleared during parsing.
   *        Bits that are set in neither @a set_options nor @a clear_options are not affected.
   */
  LIBXMLPP_API
  void set_parser_options(int set_options = 0, int clear_options = 0) noexcept;

  /** See set_parser_options().
   *
   * @newin{2,38}
   *
   * @param[out] set_options Set bits correspond to flags that shall be set during parsing.
   * @param[out] clear_options Set bits correspond to flags that shall be cleared during parsing.
   *        Bits that are set in neither @a set_options nor @a clear_options are not affected.
   */
  LIBXMLPP_API
  void get_parser_options(int& set_options, int& clear_options) const noexcept;

  /** Parse an XML document from a file.
   * @throw exception
   * @param filename The path to the file.
   */
  LIBXMLPP_API
  virtual void parse_file(const std::string& filename) = 0;

  /** Parse an XML document from raw memory.
   * @throw exception
   * @param contents The XML document as an array of bytes.
   * @param bytes_count The number of bytes in the @a contents array.
   */
  LIBXMLPP_API
  virtual void parse_memory_raw(const unsigned char* contents, size_type bytes_count) = 0;

  /** Parse an XML document from a string.
   * @throw exception
   * @param contents The XML document as a string.
   */
  LIBXMLPP_API
  virtual void parse_memory(const ustring& contents) = 0;

  /** Parse an XML document from a stream.
   * @throw exception
   * @param in The stream.
   */
  LIBXMLPP_API
  virtual void parse_stream(std::istream& in) = 0;

  //TODO: Add stop_parser()/stop_parsing(), wrapping xmlStopParser()?

protected:
  LIBXMLPP_API
  virtual void initialize_context();
  LIBXMLPP_API
  virtual void release_underlying();

  LIBXMLPP_API
  virtual void on_parser_error(const ustring& message);
  LIBXMLPP_API
  virtual void on_parser_warning(const ustring& message);
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
  virtual void check_for_error_and_warning_messages();

#ifndef LIBXMLXX_DISABLE_DEPRECATED
  /** @deprecated Use get_callback_parser_error_cfunc() instead. */
  LIBXMLPP_API
  static void callback_parser_error(void* ctx, const char* msg, ...);
  /** @deprecated Use get_callback_parser_warning_cfunc() instead. */
  LIBXMLPP_API
  static void callback_parser_warning(void* ctx, const char* msg, ...);
  /** @deprecated Use get_callback_validity_error_cfunc() instead. */
  LIBXMLPP_API
  static void callback_validity_error(void* ctx, const char* msg, ...);
  /** @deprecated Use get_callback_validity_warning_cfunc() instead. */
  LIBXMLPP_API
  static void callback_validity_warning(void* ctx, const char* msg, ...);

  /** @deprecated */
  enum class MsgType
  {
    ParserError,
    ParserWarning,
    ValidityError,
    ValidityWarning
  };

  /** @deprecated Use the other callback_error_or_warning() overload instead. */
  LIBXMLPP_API
  static void callback_error_or_warning(MsgType msg_type, void* ctx,
                                        const char* msg, va_list var_args);
#endif // LIBXMLXX_DISABLE_DEPRECATED

  /** @newin{5,2} */
  LIBXMLPP_API
  static ParserCallbackCFuncType get_callback_parser_error_cfunc();
  /** @newin{5,2} */
  LIBXMLPP_API
  static ParserCallbackCFuncType get_callback_parser_warning_cfunc();
  /** @newin{5,2} */
  LIBXMLPP_API
  static ParserCallbackCFuncType get_callback_validity_error_cfunc();
  /** @newin{5,2} */
  LIBXMLPP_API
  static ParserCallbackCFuncType get_callback_validity_warning_cfunc();

  /** @newin{5,2} */
  LIBXMLPP_API
  static void callback_error_or_warning(bool is_parser, bool is_error, void* ctx,
                                        const char* msg, va_list var_args);

  _xmlParserCtxt* context_;
  std::unique_ptr<exception> exception_;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

/** Equivalent to Parser::parse_stream().
 *
 * @newin{2,38}
 */
inline std::istream& operator>>(std::istream& in, Parser& parser)
{
  parser.parse_stream(in);
  return in;
}

} // namespace xmlpp

#endif //__LIBXMLPP_PARSER_H

