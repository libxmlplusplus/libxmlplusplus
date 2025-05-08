/* processinginstructionsnode.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/nodes/processinginstructionnode.h>

#include <libxml/tree.h>

namespace xmlpp
{

ProcessingInstructionNode::ProcessingInstructionNode(xmlNode* node)
: ContentNode(node)
{}

ProcessingInstructionNode::~ProcessingInstructionNode() = default;

} //namespace xmlpp
