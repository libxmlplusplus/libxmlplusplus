/* attributenode.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTENODE_H
#define __LIBXMLPP_ATTRIBUTENODE_H


#include <glibmm/ustring.h>

#include <libxml++/attribute.h>

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
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTENODE_H
