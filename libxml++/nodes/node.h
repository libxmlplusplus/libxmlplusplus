/* node.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_NODES_NODE_H
#define __LIBXMLPP_NODES_NODE_H

#include <libxml++/noncopyable.h>
#include <libxml++/exceptions/exception.h>
#include <glibmm/ustring.h>
#include <list>
#include <map>
#include <vector>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlNode;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

class TextNode;
class Element;
class Attribute;

class Node;
typedef std::vector<Node*> NodeSet;

// xmlpp::XPathResultType is similar to xmlXPathObjectType in libxml2.
/** An XPath expression is evaluated to yield a result, which
 * has one of the following four basic types:
 *   - node-set
 *   - boolean
 *   - number
 *   - string
 */
enum XPathResultType
{
    XPATH_RESULT_UNDEFINED = 0,
    XPATH_RESULT_NODESET = 1,
    XPATH_RESULT_BOOLEAN = 2,
    XPATH_RESULT_NUMBER = 3,
    XPATH_RESULT_STRING = 4
};

/** Represents XML Nodes.
 * You should never new or delete Nodes. The Parser will create and manage them for you.
 */
class Node : public NonCopyable
{
public:
  typedef std::list<Node*> NodeList;

  /** @throws xmlpp::internal_error If @a node is <tt>nullptr</tt>.
   */
  explicit Node(_xmlNode* node);
  ~Node() override;

  /** Get the name of this node.
   * @returns The node's name.
   */
  Glib::ustring get_name() const;

  /** Set the name of this node.
   * @param name The new name for the node.
   */
  void set_name(const Glib::ustring& name);

  /** Set the namespace prefix used by the node.
   * If no such namespace prefix has been declared then this method will throw an exception.
   * @param ns_prefix The namespace prefix.
   * @throws xmlpp::exception
   */
  void set_namespace(const Glib::ustring& ns_prefix);

  /** Get the namespace prefix of this node.
   * @returns The node's namespace prefix. Can be an empty string.
   */
  Glib::ustring get_namespace_prefix() const;

  /** Get the namespace URI of this node.
   * @returns The node's namespace URI. Can be an empty string.
   */
  Glib::ustring get_namespace_uri() const;

  /** Discover at what line number this node occurs in the XML file.
   * @returns The line number.
   */
  int get_line() const;
  
  /** Get the parent element for this node.
   * @returns The parent node, or <tt>nullptr</tt> if the node has no parent element.
   */
  const Element* get_parent() const;  

  /** Get the parent element for this node.
   * @returns The parent node, or <tt>nullptr</tt> if the node has no parent element.
   */
  Element* get_parent();  

  /** Get the next sibling for this node.
   * @returns The next sibling, or <tt>nullptr</tt> if the node has no next sibling.
   */
  const Node* get_next_sibling() const;  

  /** Get the next sibling for this node.
   * @returns The next sibling, or <tt>nullptr</tt> if the node has no next sibling.
   */
  Node* get_next_sibling();  

  /** Get the previous sibling for this node .
   * @returns The previous sibling, or <tt>nullptr</tt> if the node has no previous sibling.
   */
  const Node* get_previous_sibling() const;  

  /** Get the previous sibling for this node.
   * @returns The previous sibling, or <tt>nullptr</tt> if the node has no previous sibling.
   */
  Node* get_previous_sibling();  

  /** Get the first child of this node.
   * You may optionally get the first child node which has a certain name.
   * @param name The name of the requested child node, or an empty string.
   * @returns The first child, or <tt>nullptr</tt> if no child node (with the specified name) exists.
   *
   * @newin{2,36}
   */
  const Node* get_first_child(const Glib::ustring& name = Glib::ustring()) const;

  /** Get the first child of this node.
   * You may optionally get the first child node which has a certain name.
   * @param name The name of the requested child node, or an empty string.
   * @returns The first child, or <tt>nullptr</tt> if no child node (with the specified name) exists.
   *
   * @newin{2,36}
   */
  Node* get_first_child(const Glib::ustring& name = Glib::ustring());

  /** Obtain the list of child nodes. You may optionally obtain a list of only the child nodes which have a certain name.
   * @param name The names of the child nodes to get. If you do not specify a name, then the list will contain all nodes, regardless of their names.
   * @returns The list of child nodes.
   */
  NodeList get_children(const Glib::ustring& name = Glib::ustring());

  /** Obtain the list of child nodes. You may optionally obtain a list of only the child nodes which have a certain name.
   * @param name The names of the child nodes to get. If you do not specify a name, then the list will contain all nodes, regardless of their names.
   * @returns The list of child nodes.
   */
  const NodeList get_children(const Glib::ustring& name = Glib::ustring()) const;

  /** Add a child element to this node.
   * This node must be an element node.
   * @param name The new node name
   * @param ns_prefix The namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @returns The newly-created element
   * @throws xmlpp::exception If a namespace prefix is specified, but has not been declared.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node cannot be created.
   */
  Element* add_child(const Glib::ustring& name,
                     const Glib::ustring& ns_prefix = Glib::ustring());

  /** Add a child element to this node after the specified existing child node.
   * This node must be an element node.
   *
   * @newin{2,24}
   *
   * @param previous_sibling An existing child node.
   * @param name The new node name
   * @param ns_prefix The namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @returns The newly-created element
   * @throws xmlpp::exception If a namespace prefix is specified, but has not been declared.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node cannot be created.
   */
  Element* add_child(xmlpp::Node* previous_sibling, const Glib::ustring& name,
                     const Glib::ustring& ns_prefix = Glib::ustring());

  /** Add a child element to this node before the specified existing child node.
   * This node must be an element node.
   *
   * @newin{2,24}
   *
   * @param next_sibling An existing child node.
   * @param name The new node name
   * @param ns_prefix The namespace prefix. If the prefix has not been declared then this method will throw an exception.
   * @returns The newly-created element
   * @throws xmlpp::exception If a namespace prefix is specified, but has not been declared.
   * @throws xmlpp::internal_error If this node is not an element node,
   *         or the child node cannot be created.
   */
  Element* add_child_before(xmlpp::Node* next_sibling, const Glib::ustring& name,
                     const Glib::ustring& ns_prefix = Glib::ustring());

  /** Add a child element to this node.
   * This node must be an element node.
   *
   * @newin{2,38}
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
  Element* add_child_with_new_ns(const Glib::ustring& name,
    const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix = Glib::ustring());

  /** Add a child element to this node after the specified existing child node.
   * This node must be an element node.
   *
   * @newin{2,38}
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
  Element* add_child_with_new_ns(xmlpp::Node* previous_sibling, const Glib::ustring& name,
    const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix = Glib::ustring());

  /** Add a child element to this node before the specified existing child node.
   * This node must be an element node.
   *
   * @newin{2,38}
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
  Element* add_child_before_with_new_ns(xmlpp::Node* next_sibling, const Glib::ustring& name,
    const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix = Glib::ustring());

  /** Remove the child node.
   * @param node The child node to remove. This Node will be deleted and therefore unusable after calling this method.
   */
  void remove_child(Node* node);

  /** Import node(s) from another document under this node, without affecting the source node.
   *
   * If the imported node is an attribute node, and this node has an attribute with
   * the same name as the imported attribute, the existing attribute is destroyed
   * before the imported attribute is added. Any pointer to a destroyed attribute
   * node becomes invalid.
   *
   * @param node The node to copy and insert under the current node.
   * @param recursive Whether to import the child nodes also. Defaults to true.
   * @returns Usually the newly created node, but adjacent text nodes are merged,
   *          and the old text node with merged contents is returned.
   * @throws xmlpp::exception
   */
  Node* import_node(const Node* node, bool recursive = true);

  
  /** Get the XPath of this node.
   * @result The XPath of the node.
   */
  Glib::ustring get_path() const;

  /** Find nodes from an XPath expression.
   * @param xpath The XPath of the nodes.
   * @returns The resulting NodeSet.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error If the result type is not nodeset.
   */
  NodeSet find(const Glib::ustring& xpath) const;

  /** A map of namespace prefixes to namespace URIs.
   */
  typedef std::map<Glib::ustring, Glib::ustring> PrefixNsMap;

  /** Find nodes from an XPath expression.
   * @param xpath The XPath of the nodes.
   * @param namespaces A map of namespace prefixes to namespace URIs to be used while finding.
   * @returns The resulting NodeSet.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error If the result type is not nodeset.
   */
  NodeSet find(const Glib::ustring& xpath, const PrefixNsMap& namespaces) const;

  /** Evaluate an XPath expression.
   * @param xpath The XPath expression.
   * @param[out] result_type Result type of the XPath expression before conversion
   *             to boolean. If <tt>nullptr</tt>, the result type is not returned.
   * @returns The value of the XPath expression. If the value is not of type boolean,
   *          it is converted to boolean.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error
   *
   * @newin{2,36}
   */
  bool eval_to_boolean(const Glib::ustring& xpath, XPathResultType* result_type = nullptr) const;


  /** Evaluate an XPath expression.
   * @param xpath The XPath expression.
   * @param namespaces A map of namespace prefixes to namespace URIs to be used while evaluating.
   * @param[out] result_type Result type of the XPath expression before conversion
   *             to boolean. If <tt>nullptr</tt>, the result type is not returned.
   * @returns The value of the XPath expression. If the value is not of type boolean,
   *          it is converted to boolean.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error
   *
   * @newin{2,36}
   */
  bool eval_to_boolean(const Glib::ustring& xpath, const PrefixNsMap& namespaces,
    XPathResultType* result_type = nullptr) const;

  /** Evaluate an XPath expression.
   * @param xpath The XPath expression.
   * @param[out] result_type Result type of the XPath expression before conversion
   *             to number. If <tt>nullptr</tt>, the result type is not returned.
   * @returns The value of the XPath expression. If the value is not of type number,
   *          it is converted to number.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error
   *
   * @newin{2,36}
   */
  double eval_to_number(const Glib::ustring& xpath, XPathResultType* result_type = nullptr) const;

  /** Evaluate an XPath expression.
   * @param xpath The XPath expression.
   * @param namespaces A map of namespace prefixes to namespace URIs to be used while evaluating.
   * @param[out] result_type Result type of the XPath expression before conversion
   *             to number. If <tt>nullptr</tt>, the result type is not returned.
   * @returns The value of the XPath expression. If the value is not of type number,
   *          it is converted to number.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error
   *
   * @newin{2,36}
   */
  double eval_to_number(const Glib::ustring& xpath, const PrefixNsMap& namespaces,
    XPathResultType* result_type = nullptr) const;

  /** Evaluate an XPath expression.
   * @param xpath The XPath expression.
   * @param[out] result_type Result type of the XPath expression before conversion
   *             to string. If <tt>nullptr</tt>, the result type is not returned.
   * @returns The value of the XPath expression. If the value is not of type string,
   *          it is converted to string.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error
   *
   * @newin{2,36}
   */
  Glib::ustring eval_to_string(const Glib::ustring& xpath, XPathResultType* result_type = nullptr) const;

  /** Evaluate an XPath expression.
   * @param xpath The XPath expression.
   * @param namespaces A map of namespace prefixes to namespace URIs to be used while evaluating.
   * @param[out] result_type Result type of the XPath expression before conversion
   *             to string. If <tt>nullptr</tt>, the result type is not returned.
   * @returns The value of the XPath expression. If the value is not of type string,
   *          it is converted to string.
   * @throws xmlpp::exception If the XPath expression cannot be evaluated.
   * @throws xmlpp::internal_error
   *
   * @newin{2,36}
   */
  Glib::ustring eval_to_string(const Glib::ustring& xpath, const PrefixNsMap& namespaces,
    XPathResultType* result_type = nullptr) const;

  ///Access the underlying libxml implementation.
  _xmlNode* cobj();

  ///Access the underlying libxml implementation.
  const _xmlNode* cobj() const;

  /** Construct the correct C++ instance for a given libxml C struct instance.
   *
   * This is only for use by the libxml++ implementation.
   *
   * @param node A pointer to an xmlNode or a "derived" struct, such as xmlDoc, xmlAttr, etc.
   */
  static void create_wrapper(_xmlNode* node);
  
  /** Delete the C++ instance for a given libxml C struct instance, and also 
   * recursively destroy the C++ instances for any children.
   *
   * This is only for use by the libxml++ implementation.
   * @param node A pointer to an xmlNode or a "derived" struct, such as xmlDoc, xmlAttr, etc.
   */
  static void free_wrappers(_xmlNode* node);
  
protected:

  ///Create the C instance ready to be added to the parent node.
  _xmlNode* create_new_child_node(const Glib::ustring& name, const Glib::ustring& ns_prefix);

  ///Create the C instance ready to be added to the parent node.
  _xmlNode* create_new_child_node_with_new_ns(const Glib::ustring& name,
    const Glib::ustring& ns_uri, const Glib::ustring& ns_prefix);

private:
  _xmlNode* impl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_NODES_NODE_H
