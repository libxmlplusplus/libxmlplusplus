/* attributenode.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTENODE_H
#define __LIBXMLPP_ATTRIBUTENODE_H


#include <glibmm/ustring.h>

#include <libxml++/attribute.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlAttr;
}
#endif //#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Represents an explicit attribute of an XML Element node.
 * This will be instantiated by the parser.
 *
 * @newin{2,36}
 */
class AttributeNode : public Attribute
{
public:
  explicit AttributeNode(_xmlNode* node);
  ~AttributeNode() override;

  /** Get the value of this attribute.
   * @returns The attribute's value.
   */
  Glib::ustring get_value() const override;

  /** Set the value of this attribute.
   *
   * @newin{3,0} Replaces Attribute::set_value()
   */
  void set_value(const Glib::ustring& value);

  /** Access the underlying libxml implementation.
   *
   * @newin{3,0} Replaces Attribute::cobj()
   */
  _xmlAttr* cobj() noexcept;

  /** Access the underlying libxml implementation.
   *
   * @newin{3,0} Replaces Attribute::cobj() const
   */
  const _xmlAttr* cobj() const noexcept;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTENODE_H
