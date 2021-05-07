/* ostreamoutputbuffer.h
 * this file is part of libxml++
 *
 * copyright (C) 2003 by libxml++ developer's team
 *
 * this file is covered by the GNU Lesser General Public License,
 * which should be included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_OSTREAMOUTPUTBUFFER_H
#define __LIBXMLPP_OSTREAMOUTPUTBUFFER_H

#include <libxml++/io/outputbuffer.h>

#include <ostream>

namespace xmlpp
{
  /** An OutputBuffer implementation that send datas to a std::ostream.
   */
  class OStreamOutputBuffer: public OutputBuffer
  {
    public:
      /**
       * @param output The ostream datas will be send to
       * @param encoding Charset in which data will be encoded before being
       * sent to the stream
       */
      LIBXMLPP_API
      OStreamOutputBuffer(std::ostream& output, const Glib::ustring& encoding = Glib::ustring());
      LIBXMLPP_API ~OStreamOutputBuffer() override;

    private:
      LIBXMLPP_API bool do_write(const char * buffer, int len) override;
      LIBXMLPP_API bool do_close() override;

      std::ostream& output_;
  };
}

#endif
