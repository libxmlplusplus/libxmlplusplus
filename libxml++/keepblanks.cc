/* keepblanks.cc
 * libxml++ and this file are
 * copyright (C) 2003 by The libxml++ Development Team, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

// xmlKeepBlanksDefault() is deprecated since libxml2 2.12.0.
// xmlIndentTreeOutput is deprecated since libxml2 2.15.0.
// Ignore deprecations here.
#define XML_DEPRECATED

#include <libxml++/keepblanks.h>
#include <libxml/globals.h>

namespace xmlpp
{
#if _MSC_VER == 1200 // detect MSVC 6.0
      const bool KeepBlanks::Default = true;
#endif

  KeepBlanks::KeepBlanks(bool value) noexcept
  {
    oldIndentTreeOutput_ = xmlIndentTreeOutput;
    oldKeepBlanksDefault_ = xmlKeepBlanksDefault( value?1:0 );
  }

  KeepBlanks::~KeepBlanks() noexcept
  {
    xmlKeepBlanksDefault(oldKeepBlanksDefault_);
    xmlIndentTreeOutput = oldIndentTreeOutput_;
  }
}
