/* attribute.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTE_H
#define __LIBXMLPP_ATTRIBUTE_H

#include <glibmm/ustring.h>
#include <libxml++/nodes/node.h>

//TODO: When we can break API/ABI, remove get_value(), rename get_value2()
// to get_value(), make it virtual. Do the same in AttributeDeclaration and
// AttributeNmode.
namespace xmlpp
{

/** Represents an XML attribute node or attribute declaration.
 * This will be instantiated by the parser.
 */
class LIBXMLPP_API Attribute : public Node
{
public:
  explicit Attribute(_xmlNode* node);
  ~Attribute() override;

  /** Get the value of this attribute.
   * @returns The attribute's value.
   * @deprecated 4.4: Use get_value2() instead.
   */
  virtual Glib::ustring get_value() const = 0;

  /** Get the value of this attribute.
   * @returns The attribute's value, or no value if the attribute has no value.
   * @newin{4,4}
   */
  std::optional<Glib::ustring> get_value2() const;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTE_H

