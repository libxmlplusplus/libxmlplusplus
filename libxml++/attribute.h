/* attribute.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTE_H
#define __LIBXMLPP_ATTRIBUTE_H


#include <string>

#include <libxml++/nodes/node.h>
#include <libxml++/api_export.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlAttr;
}
#endif //#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Represents XML Node attributes.
 *
 */
class LIBXMLPP_API Attribute : public Node
{
public:
  explicit Attribute(_xmlNode* node);
  virtual ~Attribute();
  
  std::string get_name() const;
  std::string get_value() const;
  void set_value(const std::string& value);

  ///Access the underlying libxml implementation.
  _xmlAttr* cobj();

  ///Access the underlying libxml implementation.
  const _xmlAttr* cobj() const;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTE_H

