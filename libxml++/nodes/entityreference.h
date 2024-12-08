/* entityreference.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_NODES_ENTITYREFERENCE_H
#define __LIBXMLPP_NODES_ENTITYREFERENCE_H

#include <libxml++/nodes/node.h>

namespace xmlpp
{

/** Entity references refer to previously declared entities. This will be instantiated by the parser.
 */
class LIBXMLPP_API EntityReference : public Node
{
public:
  explicit EntityReference(_xmlNode* node);
  ~EntityReference() override;

#ifndef LIBXMLXX_DISABLE_DEPRECATED
  /** Get the text with character references (like "&#xdf;") resolved.
   * If the corresponding entity declaration does not contain any reference to
   * another entity, this is the text that the reference would have resolved to
   * if the XML document had been parsed with Parser::set_substitute_entities(true).
   * @returns The text with character references unescaped.
   * @deprecated 4.4: Use get_resolved_text2() instead.
   */
  Glib::ustring get_resolved_text() const;

  /** Get the text as read from the XML or DTD file.
   * @returns The escaped text.
   * @deprecated 4.4: Use get_original_text2() instead.
   */
  Glib::ustring get_original_text() const;
#endif // LIBXMLXX_DISABLE_DEPRECATED

  /** Get the text with character references (like "&#xdf;") resolved.
   * If the corresponding entity declaration does not contain any reference to
   * another entity, this is the text that the reference would have resolved to
   * if the XML document had been parsed with Parser::set_substitute_entities(true).
   * @returns The text with character references unescaped, if any, else no value.
   * @newin{4,4}
   */
  std::optional<Glib::ustring> get_resolved_text2() const;

  /** Get the text as read from the XML or DTD file.
   * @returns The escaped text, if any, else no value.
   * @newin{4,4}
   */
  std::optional<Glib::ustring> get_original_text2() const;
};

} // namespace xmlpp

#endif //__LIBXMLPP_NODES_TEXTNODE_H




