/* document.h
 * this file is part of libxml++
 *
 * parts of the code copyright (C) 2003 by Stefan Seefeld
 * others copyright (C) 2003 by libxml++ developer's team
 *
 * this file is covered by the GNU Lesser General Public License,
 * which should be included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_DOCUMENT_H
#define __LIBXMLPP_DOCUMENT_H

#include <libxml++/exceptions/parse_error.h>
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/nodes/element.h>
#include <libxml++/dtd.h>

#include <ostream>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlDoc;
  struct _xmlEntity;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

typedef enum {
    XML_INTERNAL_GENERAL_ENTITY = 1,
    XML_EXTERNAL_GENERAL_PARSED_ENTITY = 2,
    XML_EXTERNAL_GENERAL_UNPARSED_ENTITY = 3,
    XML_INTERNAL_PARAMETER_ENTITY = 4,
    XML_EXTERNAL_PARAMETER_ENTITY = 5,
    XML_INTERNAL_PREDEFINED_ENTITY = 6
} XmlEntityType;

class Document;

//TODO: Make Document inherit from Node, when we can break ABI one day?
//
//libxml might intend xmlDoc to derive (theoretically) from xmlNode.
//This is suggested because the xmlNodeSet returned by xmlXPathEval (see the Node::find() implementation) can contain either xmlNode or xmlDocument elements.
/**
 * Represents an XML document in the DOM model.
 */
class Document : NonCopyable
{
  //Ensure that libxml is properly initialised:
  class Init
  {
  public:
    Init();

    //TODO: Remove the virtual when we can break ABI?
    virtual ~Init();
  };

  friend class SaxParser;

public:
  /** Create a new document.
   * @param version XML version.
   * @throws xmlpp::internal_error If memory allocation fails.
   */
  explicit Document(const Glib::ustring& version = "1.0");
  
  /** Create a new C++ wrapper for an xmlDoc struct.
   * The created xmlpp::Document takes ownership of the xmlDoc.
   * When the Document is deleted, so is the xmlDoc and all its nodes.
   * @param doc A pointer to an xmlDoc struct. Must not be <tt>nullptr</tt>.
   */
  explicit Document(_xmlDoc* doc);
    
  ~Document() override;

  /** @return The encoding used in the source from which the document has been loaded.
   */
  Glib::ustring get_encoding() const;

  /** Get the internal subset of this document.
   * @returns A pointer to the DTD, or <tt>nullptr</tt> if not found.
   */
  Dtd* get_internal_subset() const;

  /** Create the internal subset of this document.
   * If the document already has an internal subset, a new one is not created.
   * @param name The DTD name.
   * @param external_id The external (PUBLIC) ID, or an empty string.
   * @param system_id The system ID, or an empty string.
   */
  void set_internal_subset(const Glib::ustring& name,
                           const Glib::ustring& external_id,
                           const Glib::ustring& system_id);

  //TODO: There should be a const and non-const version.
  //See the patch here: https://bugzilla.gnome.org/show_bug.cgi?id=632522
  /** Return the root node.
   * This function does @b not create a default root node if it doesn't exist.
   * @return A pointer to the root node if it exists, <tt>nullptr</tt> otherwise.
   */
  Element* get_root_node() const;

  /** Create the root element node.
   * If the document already contains a root element node, it is replaced, and
   * the old root element node and all its descendants are deleted.
   * @param name The node's name.
   * @param ns_uri The namespace URI. A namespace declaration will be added to
   *        this node, because it could not have been declared before.
   * @param ns_prefix The namespace prefix to associate with the namespace.
   *        If no namespace prefix is specified then the namespace URI will be the default namespace.
   * @return A pointer to the new root node.
   * @throws xmlpp::internal_error If memory allocation fails.
   * @throws xmlpp::exception If a new namespace node cannot be created.
   */
  Element* create_root_node(const Glib::ustring& name,
                            const Glib::ustring& ns_uri = Glib::ustring(),
                            const Glib::ustring& ns_prefix = Glib::ustring() );

  /** Create a root element node by importing the node from another document,
   * without affecting the source node.
   * If the document already contains a root element node, it is replaced, and
   * the old root element node and all its descendants are deleted.
   * @param node The node to copy and insert as the root node of the document.
   *             It must be an element node.
   * @param recursive Whether to import the child nodes also. Defaults to true.
   * @return A pointer to the new root node
   * @throws xmlpp::exception If the node can't be copied.
   */
  Element* create_root_node_by_import(const Node* node,
				      bool recursive = true);

  /** Append a new comment node.
   * @param content The text. This should be unescaped - see ContentNode::set_content().
   * @returns The new comment node.
   * @throws xmlpp::internal_error
   */
  CommentNode* add_comment(const Glib::ustring& content);

  /** Append a new processing instruction node.
   *
   * @newin{2,36}
   *
   * @param name The name of the application to which the instruction is directed.
   * @param content The content of the instruction. This should be unescaped - see ContentNode::set_content().
   * @returns The new processing instruction node.
   * @throws xmlpp::internal_error
   */
  ProcessingInstructionNode* add_processing_instruction(
    const Glib::ustring& name, const Glib::ustring& content);

  //TODO: Use std::string for filenames.
  /** Write the document to a file.
   * @param filename
   * @param encoding If not provided, UTF-8 is used
   * @throws xmlpp::exception
   */
  void write_to_file(const Glib::ustring& filename, const Glib::ustring& encoding = Glib::ustring());

  /** Write the document to a file.
   * The output is formatted by inserting whitespaces, which is easier to read for a human,
   * but may insert unwanted significant whitespaces. Use with care !
   * @param filename
   * @param encoding If not provided, UTF-8 is used
   * @throws xmlpp::exception
   */
  void write_to_file_formatted(const Glib::ustring& filename, const Glib::ustring& encoding = Glib::ustring());

  /** Write the document to the memory.
   * @param encoding If not provided, UTF-8 is used
   * @returns The written document.
   * @throws xmlpp::exception
   */
  Glib::ustring write_to_string(const Glib::ustring& encoding = Glib::ustring());

  /** Write the document to the memory.
   * The output is formatted by inserting whitespaces, which is easier to read for a human,
   * but may insert unwanted significant whitespaces. Use with care !
   * @param encoding If not provided, UTF-8 is used
   * @returns The written document.
   * @throws xmlpp::exception
   */
  Glib::ustring write_to_string_formatted(const Glib::ustring& encoding = Glib::ustring());

  /** Write the document to a std::ostream.
   * @param output A reference to the stream in which the document will be written
   * @param encoding If not provided, UTF-8 is used
   * @throws xmlpp::exception
   * @throws xmlpp::internal_error
   * @warning This method is much less efficient than write_to_string if you want to dump the
   * document to a buffer or the standard output. Writing to a fstream is almost as fast as write_to_file
   */
  void write_to_stream(std::ostream& output, const Glib::ustring& encoding = Glib::ustring());

  /** Write the document to a std::ostream.
   * The output is formatted by inserting whitespaces, which is easier to read for a human,
   * but may insert unwanted significant whitespaces. Use with care !
   * @param output A reference to the stream in which the document will be written
   * @param encoding If not provided, UTF-8 is used
   * @throws xmlpp::exception
   * @throws xmlpp::internal_error
   * @warning See write_to_stream
   */
  void write_to_stream_formatted(std::ostream & output, const Glib::ustring& encoding = Glib::ustring());

  /** Add an Entity declaration to the document.
   * @param name The name of the entity that will be used in an entity reference.
   * @param type The type of entity.
   * @param publicId The public ID of the subset.
   * @param systemId The system ID of the subset.
   * @param content The value of the Entity. In entity reference substitutions, this
   * is the replacement value.
   * @throws xmlpp::internal_error
   */
  virtual void set_entity_declaration(const Glib::ustring& name, XmlEntityType type,
                                      const Glib::ustring& publicId, const Glib::ustring& systemId,
                                      const Glib::ustring& content);

  /** Perform XInclude substitution on the XML document.
   * XInclude substitution may both add and delete nodes in the document,
   * as well as change the type of some nodes. All pointers to deleted nodes
   * and nodes whose type is changed become invalid.
   * (The node type represented by an underlying xmlNode struct can change.
   * The type of a C++ wrapper can't change. The old wrapper is deleted, and a
   * new one is created if and when it's required.)
   *
   * @newin{2,36}
   *
   * @param generate_xinclude_nodes Generate XIncludeStart and XIncludeEnd nodes.
   * @returns The number of substitutions.
   * @throws xmlpp::exception
   */
  int process_xinclude(bool generate_xinclude_nodes = true);

  ///Access the underlying libxml implementation.
  _xmlDoc* cobj();

  ///Access the underlying libxml implementation.
  const _xmlDoc* cobj() const;

protected:
  /** Retrieve an Entity.
   * The entity can be from an external subset or internally declared.
   * @param name The name of the entity to get.
   * @returns A pointer to the libxml2 entity structure, or <tt>nullptr</tt> if not found.
   */
  _xmlEntity* get_entity(const Glib::ustring& name);

private:
  //TODO: Remove virtuals when we can break ABI.

  virtual void do_write_to_file(const Glib::ustring& filename, const Glib::ustring& encoding, bool format);
  virtual Glib::ustring do_write_to_string(const Glib::ustring& encoding, bool format);
  virtual void do_write_to_stream(std::ostream& output, const Glib::ustring& encoding, bool format);

  static Init init_;

  _xmlDoc* impl_;
};

} //namespace xmlpp

#endif //__LIBXMLPP_DOCUMENT_H
