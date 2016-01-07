/* noncopyable.h
 * libxml++ and this file are
 * copyright (C) 2000 by The libxml++ Development Team, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_NONCOPYABLE_H
#define __LIBXMLPP_NONCOPYABLE_H

namespace xmlpp
{

/** A base for classes which cannot be copied or moved.
 */
class NonCopyable
{
protected:
  NonCopyable() noexcept;
  virtual ~NonCopyable();

  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  NonCopyable(NonCopyable&&) = delete;
  NonCopyable& operator=(NonCopyable&&) = delete;
};

} // namespace xmlpp

#endif //__LIBXMLPP_NONCOPYABLE_H

