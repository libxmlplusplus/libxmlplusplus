/* attribute.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTE_H
#define __LIBXMLPP_ATTRIBUTE_H


#include <glibmm/ustring.h>

#include <libxml++/nodes/node.h>

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

  /** Get the value of this attribute.
   * @returns The attribute's value.
   */
  virtual Glib::ustring get_value() const = 0;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTE_H

