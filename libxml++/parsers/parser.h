/* parser.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_PARSER_H
#define __LIBXMLPP_PARSER_H

#ifdef _MSC_VER //Ignore warnings about the Visual C++ Bug, where we can not do anything
#pragma warning (disable : 4786)
#endif

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

/** XML parser.
 *
 * Abstract base class for DOM parser and SAX parser.
 */
class Parser : public NonCopyable
{
public:
  Parser();
  ~Parser() override;

  using size_type = unsigned int;

  /** By default, the parser will not validate the XML file.
   * @param val Whether the document should be validated.
   */
  void set_validate(bool val = true) noexcept;

  /** See set_validate().
   * @returns Whether the parser will validate the XML file.
   */
  bool get_validate() const noexcept;

  /** Set whether the parser will automatically substitute entity references with the text of the entities' definitions.
   * For instance, this affects the text returned by ContentNode::get_content().
   * By default, the parser will not substitute entities, so that you do not lose the entity reference information.
   * @param val Whether entities will be substitued.
   */
  void set_substitute_entities(bool val = true) noexcept;

  /** See set_substitute_entities().
   * @returns Whether entities will be substituted during parsing.
   */
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
  void set_throw_messages(bool val = true) noexcept;

  /** See set_throw_messages().
   *
   * @newin{2,36}
   *
   * @returns Whether messages will be collected and thrown in an exception.
   */
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
  void set_include_default_attributes(bool val = true) noexcept;

  /** See set_include_default_attributes().
   *
   * @newin{2,38}
   *
   * @returns Whether attributes with default values will be included in the node tree.
   */
  bool get_include_default_attributes() noexcept;

  /** Set and/or clear parser option flags.
   * See the libxml2 documentation, enum xmlParserOption, for a list of parser options.
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
  void set_parser_options(int set_options = 0, int clear_options = 0) noexcept;

  /** See set_parser_options().
   *
   * @newin{2,38}
   *
   * @param [out] set_options Set bits correspond to flags that shall be set during parsing.
   * @param [out] clear_options Set bits correspond to flags that shall be cleared during parsing.
   *        Bits that are set in neither @a set_options nor @a clear_options are not affected.
   */
  void get_parser_options(int& set_options, int& clear_options) noexcept;

  /** Parse an XML document from a file.
   * @throw exception
   * @param filename The path to the file.
   */
  virtual void parse_file(const std::string& filename) = 0;

  /** Parse an XML document from raw memory.
   * @throw exception
   * @param contents The XML document as an array of bytes.
   * @param bytes_count The number of bytes in the @a contents array.
   */
  virtual void parse_memory_raw(const unsigned char* contents, size_type bytes_count) = 0;

  /** Parse an XML document from a string.
   * @throw exception
   * @param contents The XML document as a string.
   */
  virtual void parse_memory(const Glib::ustring& contents) = 0;

  /** Parse an XML document from a stream.
   * @throw exception
   * @param in The stream.
   */
  virtual void parse_stream(std::istream& in) = 0;

  //TODO: Add stop_parser()/stop_parsing(), wrapping xmlStopParser()?

protected:
  virtual void initialize_context();
  virtual void release_underlying();

  virtual void on_parser_error(const Glib::ustring& message);
  virtual void on_parser_warning(const Glib::ustring& message);
  virtual void on_validity_error(const Glib::ustring& message);
  virtual void on_validity_warning(const Glib::ustring& message);

  /// To be called in an exception handler.
  virtual void handle_exception();
  virtual void check_for_exception();

  virtual void check_for_error_and_warning_messages();

  static void callback_parser_error(void* ctx, const char* msg, ...);
  static void callback_parser_warning(void* ctx, const char* msg, ...);
  static void callback_validity_error(void* ctx, const char* msg, ...);
  static void callback_validity_warning(void* ctx, const char* msg, ...);

  enum class MsgType
  {
    ParserError,
    ParserWarning,
    ValidityError,
    ValidityWarning
  };

  static void callback_error_or_warning(MsgType msg_type, void* ctx,
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

