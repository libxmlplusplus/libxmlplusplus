/* node.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_NODES_COMMENTNODE_H
#define __LIBXMLPP_NODES_COMMENTNODE_H

#include <libxml++/nodes/contentnode.h>
#include <libxml++/api_export.h>

namespace xmlpp
{

/** Comment Node. This will be instantiated by the parser.
 */
class LIBXMLPP_API CommentNode : public ContentNode
{
public:
  explicit CommentNode(_xmlNode* node);
  virtual ~CommentNode();
};

} // namespace xmlpp

#endif //__LIBXMLPP_NODES_COMMENTNODE_H



