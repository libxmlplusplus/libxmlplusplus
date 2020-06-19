/* domparser.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_PARSERS_DOMPARSER_H
#define __LIBXMLPP_PARSERS_DOMPARSER_H

#include <libxml++/parsers/parser.h>
#include <libxml++/document.h>

namespace xmlpp {

/** DOM XML parser.
 * DOM = %Document Object Model
 */
class LIBXMLPP_API DomParser : public Parser
{
public:
  /** Create a parser with an empty document.
   * @throws xmlpp::internal_error If an empty document can't be created.
   */
  DomParser();

  /** Instantiate the parser and parse a document immediately.
   * @param filename The path to the file.
   * @param validate Whether the parser should validate the XML.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  explicit DomParser(const std::string& filename, bool validate = false);
  ~DomParser() override;

  /** Set whether and how the parser will perform XInclude substitution.
   *
   * @newin{3,2}
   *
   * @param process_xinclude Do XInclude substitution on the XML document.
   *        If <tt>false</tt>, the other parameters have no effect.
   * @param generate_xinclude_nodes Generate XIncludeStart and XIncludeEnd nodes.
   * @param fixup_base_uris Add or replace xml:base attributes in included element
   *        nodes, if necessary to preserve the target of relative URIs.
   */
  void set_xinclude_options(bool process_xinclude = true,
    bool generate_xinclude_nodes = true, bool fixup_base_uris = true) noexcept;

  /** Get whether and how the parser will perform XInclude substitution.
   *
   * @newin{3,2}
   *
   * @param[out] process_xinclude Do XInclude substitution on the XML document.
   * @param[out] generate_xinclude_nodes Generate XIncludeStart and XIncludeEnd nodes.
   * @param[out] fixup_base_uris Add or replace xml:base attributes in included element
   *        nodes, if necessary to preserve the target of relative URIs.
   */
  void get_xinclude_options(bool& process_xinclude,
    bool& generate_xinclude_nodes, bool& fixup_base_uris) const noexcept;

  /** Parse an XML document from a file.
   * If the parser already contains a document, that document and all its nodes
   * are deleted.
   * @param filename The path to the file.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  void parse_file(const std::string& filename) override;

  /** Parse an XML document from a string.
   * If the parser already contains a document, that document and all its nodes
   * are deleted.
   * @param contents The XML document as a string.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  void parse_memory(const ustring& contents) override;

  /** Parse an XML document from raw memory.
   * If the parser already contains a document, that document and all its nodes
   * are deleted.
   * @param contents The XML document as an array of bytes.
   * @param bytes_count The number of bytes in the @a contents array.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  void parse_memory_raw(const unsigned char* contents, size_type bytes_count) override;

  /** Parse an XML document from a stream.
   * If the parser already contains a document, that document and all its nodes
   * are deleted.
   * @param in The stream.
   * @throws xmlpp::internal_error
   * @throws xmlpp::parse_error
   * @throws xmlpp::validity_error
   */
  void parse_stream(std::istream& in) override;

  /** Test whether a document has been parsed.
   */
  explicit operator bool() const noexcept;

  /** Get the parsed document.
   * @returns A pointer to the parsed document, or <tt>nullptr</tt>.
   */
  Document* get_document() noexcept;

  /** Get the parsed document.
   * @returns A pointer to the parsed document, or <tt>nullptr</tt>.
   */
  const Document* get_document() const noexcept;

protected:
  void parse_context();
  void check_xinclude_and_finish_parsing();

  void release_underlying() override;

  int xinclude_options_ = 0;
  Document* doc_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_PARSERS_DOMPARSER_H
