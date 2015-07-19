/* attribute.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTE_H
#define __LIBXMLPP_ATTRIBUTE_H


#include <glibmm/ustring.h>

#include <libxml++/nodes/node.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlAttr;
}
#endif //#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Represents an XML Node attribute.
 * This will be instantiated by the parser.
 */
class Attribute : public Node
{
public:
  explicit Attribute(_xmlNode* node);
  ~Attribute() override;
  
  //TODO: Can we remove this and just use Node::get_name()?
  // Yes, when we can break ABI. /Kjell Ahlstedt 2012-02-09

  /** Get the name of this attribute.
   * See also Node::get_namespace_prefix() and Node::get_namespace_uri()
   * @returns The attribute's name.
   */
  Glib::ustring get_name() const;

  /** Get the value of this attribute.
   * Can be used for both an AttributeDeclaration and an AttributeNode.
   * @returns The attribute's value.
   */
  Glib::ustring get_value() const;

  /** Set the value of this attribute.
   *
   * If this is an AttributeDeclaration, the value will not be changed.
   * This method is here for backward compatibility. It may be moved to
   * AttributeNode in the future.
   */
  void set_value(const Glib::ustring& value);

  /** Access the underlying libxml implementation.
   *
   * If this is an AttributeDeclaration, use AttributeDeclaration::cobj() instead.
   * This method is here for backward compatibility. It may be moved to
   * AttributeNode in the future.
   */
  _xmlAttr* cobj();

  /** Access the underlying libxml implementation.
   *
   * If this is an AttributeDeclaration, use AttributeDeclaration::cobj() instead.
   * This method is here for backward compatibility. It may be moved to
   * AttributeNode in the future.
   */
  const _xmlAttr* cobj() const;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTE_H

