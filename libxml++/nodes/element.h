/* element.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_NODES_ELEMENT_H
#define __LIBXMLPP_NODES_ELEMENT_H

#include <libxml++/nodes/node.h>
#include <libxml++/attribute.h>
#include <libxml++/nodes/commentnode.h>
#include <libxml++/nodes/cdatanode.h>
#include <libxml++/nodes/textnode.h>
#include <libxml++/nodes/processinginstructionnode.h>
#include <libxml++/nodes/entityreference.h>

namespace xmlpp
{

/** %Element nodes have attributes as well as child nodes.
 * This will be instantiated by the parser.
 */
class LIBXMLPP_API Element : public Node
{
public:
  explicit Element(_xmlNode* node);
  ~Element() override;

  using AttributeList = std::list<Attribute*>;
  using const_AttributeList = std::list<const Attribute*>;

  /** Add a namespace declaration to this node which will apply to this node and all children.
   *
   * If the added namespace prefix is equal to the prefix associated to the node,
   * the associated namespace of the node itself is updated, but child nodes are
   * not updated. If you use this method on a node after children have been added,
   * it may be necessary to save the XML document and reparse it to get correct
   * namespaces on all nodes.
   *
   * @param ns_uri The namespace to associate with the prefix,
   *               or to use as the default namespace if no prefix is specified.
   * @param ns_prefix The namespace prefix. If no prefix is specified then the
   *                  namespace URI will be the default namespace.
   * @throws xmlpp::exception If a new namespace node cannot be created,
   *         e.g. because a namespace with the same prefix but another URI already exists.
   */
  void set_namespace_declaration(const ustring& ns_uri, const ustring& ns_prefix = ustring());

  /** Obtain the list of explicitly set attributes for this element.
   * @returns The list of explicitly set attributes.
   */
  AttributeList get_attributes();

  /** Obtain the list of explicitly set attributes for this element.
   * @returns The list of explicitly set attributes.
   */
  const_AttributeList get_attributes() const;

  /** Get the attribute with this name, and optionally with this namespace.
   * @param name The name of the attribute that will be retrieved.
   * @param ns_prefix Namespace prefix.
   * @return The attribute, or <tt>nullptr</tt> if no suitable Attribute was found.
   *         Is either an AttributeNode*, pointing to an explicitly set
   *         attribute, or an AttributeDeclaration*, pointing to the declaration
   *         of an attribute with a default value.
   */
  Attribute* get_attribute(const ustring& name,
                           const ustring& ns_prefix = ustring());

  /** Get the attribute with this name, and optionally with this namespace.
   * @param name The name of the attribute that will be retrieved.
   * @param ns_prefix Namespace prefix.
   * @return The attribute, or <tt>nullptr</tt> if no suitable Attribute was found.
   *         Is either an AttributeNode*, pointing to an explicitly set
   *         attribute, or an AttributeDeclaration*, pointing to the declaration
   *         of an attribute with a default value.
   */
  const Attribute* get_attribute(const ustring& name,
                                 const ustring& ns_prefix = ustring()) const;

#ifndef LIBXMLXX_DISABLE_DEPRECATED
  /** Get the value of the attribute with this name, and optionally with this namespace.
   * For finer control, you might use get_attribute() and use the methods of the Attribute class.
   * @param name The name of the attribute whose value will be retrieved.
   * @param ns_prefix Namespace prefix.
   * @return The text value of the attribute, or an empty string if no such attribute was found.
   * @deprecated 5.6: Use get_attribute_value2() instead.
   */
  ustring get_attribute_value(const ustring& name,
                                    const ustring& ns_prefix = ustring()) const;
#endif // LIBXMLXX_DISABLE_DEPRECATED

  /** Get the value of the attribute with this name, and optionally with this namespace.
   * For finer control, you might use get_attribute() and use the methods of the Attribute class.
   * @param name The name of the attribute whose value will be retrieved.
   * @param ns_prefix Namespace prefix.
   * @return The text value of the attribute, or no value if no such attribute was found.
   * @newin{5,6}
   */
  std::optional<ustring> get_attribute_value2(const ustring& name,
                                    const ustring& ns_prefix = {}) const;

  /** Set the value of the attribute with this name, and optionally with this namespace.
   * A matching attribute will be added if no matching attribute already exists.
   * For finer control, you might want to use get_attribute() and use the methods of the Attribute class.
   * @param name The name of the attribute whose value will change.
   * @param value The new value for the attribute
   * @param ns_prefix Namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @return The attribute that was changed, or <tt>nullptr</tt> is no suitable Attribute was found.
   * @throws xmlpp::exception
   */
  Attribute* set_attribute(const ustring& name, const ustring& value,
                           const ustring& ns_prefix = ustring());

  /** Remove the attribute with this name, and optionally with this namespace.
   * @param name The name of the attribute to be removed
   * @param ns_prefix Namespace prefix. If specified, the attribute will be removed only if the attribute has this namespace.
   */
  void remove_attribute(const ustring& name,
                        const ustring& ns_prefix = ustring());

  /** Add a child element to this node.
   *
   * @newin{3,0} Replaces Node::add_child()
   *
   * @param name The new node name
   * @param ns_prefix The namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @returns The newly-created element
   * @throws xmlpp::exception If a namespace prefix is specified, but has not been declared.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node cannot be created.
   */
  Element* add_child_element(const ustring& name,
    const ustring& ns_prefix = ustring());

  /** Add a child element to this node after the specified existing child node.
   *
   * @newin{3,0} Replaces Node::add_child()
   *
   * @param previous_sibling An existing child node.
   * @param name The new node name
   * @param ns_prefix The namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @returns The newly-created element
   * @throws xmlpp::exception If a namespace prefix is specified, but has not been declared.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node cannot be created.
   */
  Element* add_child_element(xmlpp::Node* previous_sibling, const ustring& name,
    const ustring& ns_prefix = ustring());

  /** Add a child element to this node before the specified existing child node.
   *
   * @newin{3,0} Replaces Node::add_child_before()
   *
   * @param next_sibling An existing child node.
   * @param name The new node name
   * @param ns_prefix The namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @returns The newly-created element
   * @throws xmlpp::exception If a namespace prefix is specified, but has not been declared.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node cannot be created.
   */
  Element* add_child_element_before(xmlpp::Node* next_sibling, const ustring& name,
    const ustring& ns_prefix = ustring());

  /** Add a child element to this node.
   *
   * @newin{3,0} Replaces Node::add_child_with_new_ns()
   *
   * @param name The new node name.
   * @param ns_uri The namespace to associate with the prefix,
   *               or to use as the default namespace if no prefix is specified.
   * @param ns_prefix The prefix of the node's namespace. If no prefix is specified
   *                  then the namespace URI will be the default namespace.
   * @returns The newly-created element.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node or the namespace node cannot be created.
   */
  Element* add_child_element_with_new_ns(const ustring& name,
    const ustring& ns_uri, const ustring& ns_prefix = ustring());

  /** Add a child element to this node after the specified existing child node.
   *
   * @newin{3,0} Replaces Node::add_child_with_new_ns()
   *
   * @param previous_sibling An existing child node.
   * @param name The new node name.
   * @param ns_uri The namespace to associate with the prefix,
   *               or to use as the default namespace if no prefix is specified.
   * @param ns_prefix The prefix of the node's namespace. If no prefix is specified
   *                  then the namespace URI will be the default namespace.
   * @returns The newly-created element.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node or the namespace node cannot be created.
   */
  Element* add_child_element_with_new_ns(xmlpp::Node* previous_sibling, const ustring& name,
    const ustring& ns_uri, const ustring& ns_prefix = ustring());

  /** Add a child element to this node before the specified existing child node.
   *
   * @newin{3,0} Replaces Node::add_child_before_with_new_ns()
   *
   * @param next_sibling An existing child node.
   * @param name The new node name.
   * @param ns_uri The namespace to associate with the prefix,
   *               or to use as the default namespace if no prefix is specified.
   * @param ns_prefix The prefix of the node's namespace. If no prefix is specified
   *                  then the namespace URI will be the default namespace.
   * @returns The newly-created element.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node or the namespace node cannot be created.
   */
  Element* add_child_element_before_with_new_ns(xmlpp::Node* next_sibling, const ustring& name,
    const ustring& ns_uri, const ustring& ns_prefix = ustring());

  /** Get the first child text content node.
   * This is a convenience method, meant as an alternative to iterating over all the
   * child nodes to find the first suitable node and then getting the text directly.
   * @returns The first text node, if any.
   *
   * @newin{3,0} Replaces get_child_text().
   */
  TextNode* get_first_child_text();

   /** Get the first child text content node.
   * This is a convenience method, meant as an alternative to iterating over all the
   * child nodes to find the first suitable node and then getting the text directly.
   * @returns The first text node, if any.
   *
   * @newin{3,0} Replaces get_child_text().
   */
  const TextNode* get_first_child_text() const;

  /** Append a new text node.
   * @param content The text. This should be unescaped - see ContentNode::set_content().
   * @returns The new text node.
   * @throws xmlpp::internal_error
   */
  TextNode* add_child_text(const ustring& content = ustring());

  /** Add a new text node after the specified existing child node.
   *
   * @newin{2,24}
   *
   * @param previous_sibling An existing child node.
   * @param content The text. This should be unescaped - see ContentNode::set_content().
   * @returns The new text node.
   * @throws xmlpp::internal_error
   */
  TextNode* add_child_text(xmlpp::Node* previous_sibling, const ustring& content = ustring());

  /** Add a new text node before the specified existing child node.
   *
   * @newin{2,24}
   *
   * @param next_sibling An existing child node.
   * @param content The text. This should be unescaped - see ContentNode::set_content().
   * @returns The new text node.
   * @throws xmlpp::internal_error
   */
  TextNode* add_child_text_before(xmlpp::Node* next_sibling, const ustring& content = ustring());

  /** Set the text of the first text node, adding one if necessary.
   * This is a convenience method, meant as an alternative to iterating over all the
   * child nodes to find the first suitable node and then setting the text directly.
   * @param content The text. This should be unescaped - see ContentNode::set_content().
   * @throws xmlpp::internal_error
   *
   * @newin{3,0} Replaces set_child_text().
   */
  void set_first_child_text(const ustring& content);

  /** Discover whether one of the child nodes is a text node.
   * This is a convenience method, meant as an alternative to iterating over all the child nodes and examining them directly.
   * @returns Whether this node has a child text node.
   */
  bool has_child_text() const;

  /** Append a new comment node.
   * @param content The text. This should be unescaped - see ContentNode::set_content().
   * @returns The new comment node.
   * @throws xmlpp::internal_error
   */
  CommentNode* add_child_comment(const ustring& content);

  /** Append a new CDATA node.
   * @param content The raw text.
   * @returns The new CDATA node.
   * @throws xmlpp::internal_error
   */
  CdataNode* add_child_cdata(const ustring& content);

  /** Append a new entity reference node.
   * The reference can be either an entity reference ("name" or "&name;") or
   * a character reference ("#dec", "#xhex", "&#dec;", or "&#xhex;").
   *
   * '&' and ';' are optional. If they exist, they are stripped from the stored
   * copy of the name. Node::get_name() returns the name without '&' and ';'.
   * If the Document is written to an XML file, '&' and ';' are written.
   *
   * @newin{2,36}
   *
   * @param name The name of the entity.
   * @returns The new entity reference node.
   * @throws xmlpp::internal_error
   */
  EntityReference* add_child_entity_reference(const ustring& name);

  /** Append a new processing instruction node.
   *
   * @newin{2,36}
   *
   * @param name The name of the application to which the instruction is directed.
   * @param content The content of the instruction. This should be unescaped - see ContentNode::set_content().
   * @returns The new processing instruction node.
   * @throws xmlpp::internal_error
   */
  ProcessingInstructionNode* add_child_processing_instruction(
    const ustring& name, const ustring& content);

private:
  ustring get_namespace_uri_for_prefix(const ustring& ns_prefix) const;

  ///Create the C instance ready to be added to the parent node.
  _xmlNode* create_new_child_element_node(const ustring& name,
    const ustring& ns_prefix);

  ///Create the C instance ready to be added to the parent node.
  _xmlNode* create_new_child_element_node_with_new_ns(const ustring& name,
    const ustring& ns_uri, const ustring& ns_prefix);
};

} // namespace xmlpp

#endif //__LIBXMLPP_NODES_ELEMENT_H
