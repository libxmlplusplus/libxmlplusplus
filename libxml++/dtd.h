/* dtd.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_DTD_H
#define __LIBXMLPP_DTD_H

#include <libxml++/attribute.h>
#include <list>
#include <map>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlDtd;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS4

namespace xmlpp
{

/** Represents XML DTDs.
 *
 */
class Dtd //TODO: Derive from Node?
{
public:
  LIBXMLPP_API Dtd(_xmlDtd* dtd);
  LIBXMLPP_API ~Dtd();

  LIBXMLPP_API Glib::ustring get_name() const;
  LIBXMLPP_API Glib::ustring get_external_id() const;
  LIBXMLPP_API Glib::ustring get_system_id() const;
  
  /** Access the underlying libxml implementation.
   */
  LIBXMLPP_API _xmlDtd* cobj();

  /** Access the underlying libxml implementation.
   */
  LIBXMLPP_API const _xmlDtd* cobj() const;
private:
  _xmlDtd* impl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_DTD_H



