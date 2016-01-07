/* attributedeclaration.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_ATTRIBUTEDECLARATION_H
#define __LIBXMLPP_ATTRIBUTEDECLARATION_H

#include <glibmm/ustring.h>

#include <libxml++/attribute.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlAttribute;
}
#endif //#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Represents the default value of an attribute of an XML Element node.
 * This will be instantiated by the parser.
 *
 * @newin{2,36}
 */
class AttributeDeclaration : public Attribute
{
public:
  explicit AttributeDeclaration(_xmlNode* node);
  ~AttributeDeclaration() override;

  /** Get the default value of this attribute.
   * @returns The attribute's default value.
   */
  Glib::ustring get_value() const override;

  ///Access the underlying libxml implementation.
  _xmlAttribute* cobj() noexcept;

  ///Access the underlying libxml implementation.
  const _xmlAttribute* cobj() const noexcept;
};

} // namespace xmlpp

#endif //__LIBXMLPP_ATTRIBUTEDECLARATION_H
