/* textreader.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_XMLREADER_H
#define __LIBXMLPP_XMLREADER_H

#include <libxml++/noncopyable.h>
#include <libxml++/nodes/node.h>

#include "libxml++/ustring.h"

#include <memory>

extern "C"
{
  struct _xmlTextReader;
}

namespace xmlpp
{

/** A TextReader-style XML parser.
 * A reader that provides fast, non-cached, forward-only access to XML data,
 * in the style of .Net's <a href="http://msdn.microsoft.com/en-us/library/system.xml.xmltextreader.aspx">XmlTextReader</a> class.
 */
class TextReader: public NonCopyable
{
  public:
    // xmlpp::TextReader::NodeType is similar to xmlReaderTypes in libxml2.
    /** Node type of the current node.
     * See DotGNU's <a href="http://www.gnu.org/software/dotgnu/pnetlib-doc/System/Xml/XmlNodeType.html">XmlNodeType</a> enum.
     */
    enum class NodeType
    {
      InternalError = -1,
      None = 0,
      Element = 1,
      Attribute = 2,
      Text = 3,
      CDATA = 4,
      EntityReference = 5,
      Entity = 6,
      ProcessingInstruction = 7,
      Comment = 8,
      Document = 9,
      DocumentType = 10,
      DocumentFragment = 11,
      Notation = 12,
      Whitespace = 13,
      SignificantWhitespace = 14,
      EndElement = 15,
      EndEntity = 16,
      XmlDeclaration = 17
    };

    // xmlpp::TextReader::ReadState is similar to xmlTextReaderMode in libxml2.
    enum class ReadState
    {
      InternalError = -1,
      Initial = 0,
      Interactive = 1,
      Error = 2,
      EndOfFile = 3,
      Closed = 4,
      Reading = 5
    };

    // xmlpp::TextReader::ParserProperties is similar to xmlParserProperties in libxml2.
    enum class ParserProperties
    {
      LoadDtd = 1,
      DefaultAttrs = 2,
      Validate = 3,
      SubstEntities = 4
    };

  using size_type = unsigned int;

  public:
    /**
     * Wraps a TextReader object from an underlying libxml object. The TextReader
     * takes ownership of cobj.
     * @param cobj The underlying libxml xmlTextReader object.
     */
    LIBXMLPP_API
    TextReader(struct _xmlTextReader* cobj);

    /**
     * Creates a new TextReader object to parse a file or URI.
     * @param URI The URI to read.
     * @throws xmlpp::internal_error If an xmlTextReader object cannot be created.
     */
    LIBXMLPP_API
    TextReader(const ustring& URI);

    /**
     * Creates a new TextReader object which parses in memory data.
     * @param data The data to parse.
     * @param size The number of bytes in data.
     * @param uri The base URI to use for the document.
     * @throws xmlpp::internal_error If an xmlTextReader object cannot be created.
     */
    LIBXMLPP_API
    TextReader(const unsigned char* data, size_type size, const ustring& uri = ustring());

    LIBXMLPP_API ~TextReader() override;

    /** Moves the position of the current instance to the next node in the stream, exposing its properties.
     * @return true if the node was read successfully, false if there are no more nodes to read.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API bool read();

    /** Reads the contents of the current node, including child nodes and markup.
     * @return A ustring containing the XML content, or an empty ustring if the current node is neither an element nor attribute, or has no child nodes.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API ustring read_inner_xml();

    /** Reads the current node and its contents, including child nodes and markup.
     * @return A ustring containing the XML content, or an empty ustring if the current node is neither an element nor attribute.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API ustring read_outer_xml();

    /** Reads the contents of an element or a text node as a string.
     * @return A ustring containing the contents of the Element or Text node, or an empty ustring if the reader is positioned on any other type of node.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API ustring read_string();

    /** Parses an attribute value into one or more Text and EntityReference nodes.
     * @return A bool where true indicates the attribute value was parsed, and false indicates the reader was not positioned on an attribute node or all the attribute values have been read.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API
    bool read_attribute_value();

    /** Gets the number of attributes on the current node.
     * @return The number of attributes on the current node, or zero if the current node
     *         does not support attributes, or -1 in case of error.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API
    int get_attribute_count() const;

    /** Gets the base Uniform Resource Identifier (URI) of the current node.
     * @return The base URI of the current node or an empty ustring if not available.
     */
    LIBXMLPP_API ustring get_base_uri() const;

    /** Gets the depth of the current node in the XML document.
     * @return The depth of the current node in the XML document, or -1 in case of error.
     */
    LIBXMLPP_API
    int get_depth() const;

    /** Gets a value indicating whether the current node has any attributes.
     * @return true if the current has attributes, false otherwise.
     */
    LIBXMLPP_API
    bool has_attributes() const;

    /** Whether the node can have a text value.
     * @return true if the current node can have an associated text value, false otherwise.
     */
    LIBXMLPP_API
    bool has_value() const;

    /** Whether an Attribute node was generated from the default value defined in the DTD or schema.
     * @return true if defaulted, false otherwise.
     */
    LIBXMLPP_API
    bool is_default() const;

    /** Check if the current node is empty
     * @return true if empty, false otherwise.
     */
    LIBXMLPP_API
    bool is_empty_element() const;

    LIBXMLPP_API ustring get_local_name() const;
    LIBXMLPP_API ustring get_name() const;
    LIBXMLPP_API ustring get_namespace_uri() const;

    /** Get the node type of the current node.
     * @returns The xmlpp::TextReader::NodeType of the current node.
     *          In case of error, either returns xmlpp::TextReader::NodeType::InternalError
     *          or throws an exception.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API
    NodeType get_node_type() const;

    /** Get the namespace prefix associated with the current node.
     * @returns The namespace prefix, or an empty string if not available.
     */
    LIBXMLPP_API ustring get_prefix() const;

    /** Get the quotation mark character used to enclose the value of an attribute.
     * @returns Returns " or ' and -1 in case of error.
     */
    LIBXMLPP_API
    char get_quote_char() const;

    LIBXMLPP_API ustring get_value() const;
    LIBXMLPP_API ustring get_xml_lang() const;

    LIBXMLPP_API
    ReadState get_read_state() const;

    LIBXMLPP_API void close();

    LIBXMLPP_API
    ustring get_attribute(int number) const;
    LIBXMLPP_API
    ustring get_attribute(const ustring& name) const;
    LIBXMLPP_API
    ustring get_attribute(const ustring& local_name, const ustring& ns_uri) const;

    // TODO InputBuffer GetRemainder;

    LIBXMLPP_API
    ustring lookup_namespace(const ustring& prefix) const;

    LIBXMLPP_API
    bool move_to_attribute(int number);
    LIBXMLPP_API
    bool move_to_attribute(const ustring& name);
    LIBXMLPP_API
    bool move_to_attribute(const ustring& local_name, const ustring& ns_uri);
    LIBXMLPP_API
    bool move_to_first_attribute();
    LIBXMLPP_API
    bool move_to_next_attribute();
    LIBXMLPP_API
    bool move_to_element();

    LIBXMLPP_API
    bool get_normalization() const;
    LIBXMLPP_API
    void set_normalization(bool value);

    LIBXMLPP_API
    bool get_parser_property(ParserProperties property) const;
    LIBXMLPP_API
    void set_parser_property(ParserProperties property, bool value);

    /** Get a pointer to the current node.
     * @warning This is dangerous because the underlying node may be destroyed on the next read.
     * The C++ wrapper is not deleted. Using this method causes memory leaks,
     * unless you call xmlpp::Node::free_wrappers(), which is not intended to be
     * called by the application.
     * @returns A pointer to the current node, or <tt>nullptr</tt> in case of error.
     */
    LIBXMLPP_API
    Node* get_current_node();

    /** Get a pointer to the current node.
     * @warning See the non-const get_current_node().
     * @returns A pointer to the current node, or <tt>nullptr</tt> in case of error.
     */
    LIBXMLPP_API
    const Node* get_current_node() const;

//    Document* CurrentDocument();

    /** Expand the current node.
     * Reads the contents of the current node and the full subtree. It then makes
     * the subtree available until the next call to read() or next().
     * @warning The C++ wrappers are not deleted. Using this method causes memory leaks,
     * unless you call xmlpp::Node::free_wrappers(), which is not intended to be
     * called by the application.
     * @returns A pointer to the current node, or <tt>nullptr</tt> in case of error.
     * @throws xmlpp::parse_error
     * @throws xmlpp::validity_error
     */
    LIBXMLPP_API Node* expand();

    LIBXMLPP_API bool next();
    LIBXMLPP_API bool is_valid() const;

  private:
    class PropertyReader;
    friend class PropertyReader;

    LIBXMLPP_API
    void setup_exceptions();
    LIBXMLPP_API
    static void on_libxml_error(void * arg, const char *msg, int severity,
                              void * locator);
    LIBXMLPP_API
    void check_for_exceptions() const;

    std::unique_ptr<PropertyReader> propertyreader;
    _xmlTextReader* impl_;
    int severity_;
    ustring error_;
};

}

#endif
