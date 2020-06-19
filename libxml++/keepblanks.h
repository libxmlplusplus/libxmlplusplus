/* keepblanks.h
 * libxml++ and this file are
 * copyright (C) 2003 by The libxml++ Development Team, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_KEEPBLANKS_H
#define __LIBXMLPP_KEEPBLANKS_H

#include <libxml++config.h>

namespace xmlpp
{
  /** This class sets KeepBlanksDefault and IndentTreeOutput of libxmlpp
   * and restores their initial values in its destructor. As a consequence
   * the wanted setting is kept during instance lifetime.
   */
  class LIBXMLPP_API KeepBlanks {
    public:
      static const bool Default = true;

    public:
      KeepBlanks(bool value) noexcept;
      ~KeepBlanks() noexcept;

    private:
      int oldKeepBlanksDefault_;
      int oldIndentTreeOutput_;
  };
}

#endif // __LIBXMLPP_KEEPBLANKS_H
