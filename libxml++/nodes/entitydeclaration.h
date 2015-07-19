/* entitydeclaration.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_NODES_ENTITYDECLARATION_H
#define __LIBXMLPP_NODES_ENTITYDECLARATION_H

#include <libxml++/nodes/contentnode.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlEntity;
}
#endif //#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Entity declaration. This will be instantiated by the parser.
 *
 * @newin{2,36}
 *
 */
class EntityDeclaration : public ContentNode
{
public:
  explicit EntityDeclaration(_xmlNode* node);
  ~EntityDeclaration() override;

  /** Get the text with character references (like "&#xdf;") resolved.
   * If the entity declaration does not contain any reference to another entity,
   * this is the text that an entity reference would have resolved to, if the XML
   * document had been parsed with Parser::set_substitute_entities(true).
   * @returns The text with character references unescaped.
   */
  Glib::ustring get_resolved_text() const;

  /** Get the text as read from the XML or DTD file.
   * @returns The escaped text.
   */
  Glib::ustring get_original_text() const;

  ///Access the underlying libxml implementation.
  _xmlEntity* cobj();

  ///Access the underlying libxml implementation.
  const _xmlEntity* cobj() const;
};

} // namespace xmlpp

#endif //__LIBXMLPP_NODES_ENTITYDECLARATION_H
