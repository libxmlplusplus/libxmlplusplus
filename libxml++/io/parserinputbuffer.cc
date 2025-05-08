/* parserinputbuffer.cc
 * this file is part of libxml++
 *
 * copyright (C) 2003 by libxml++ developer's team
 *
 * this file is covered by the GNU Lesser General Public License,
 * which should be included with libxml++ as the file COPYING.
 */

#include <libxml++/io/parserinputbuffer.h>
#include <libxml++/exceptions/internal_error.h>

#include <libxml/globals.h> //Needed by libxml/xmlIO.h
#include <libxml/xmlIO.h>

namespace xmlpp
{

  struct ParserInputBufferCallback
  {
    static int on_read(void * context, char * buffer, int len)
    {
      auto tmp = static_cast<ParserInputBuffer*>(context);
      return tmp->do_read(buffer, len);
    }

    static int on_close(void * context)
    {
      auto tmp = static_cast<ParserInputBuffer*>(context);
      return tmp->do_close();
    }
  };


  ParserInputBuffer::ParserInputBuffer()
  {
    impl_ = xmlParserInputBufferCreateIO(
        &ParserInputBufferCallback::on_read,
        &ParserInputBufferCallback::on_close,
        static_cast<void*>(this),
        XML_CHAR_ENCODING_NONE);
    if (!impl_)
    {
      throw internal_error("Cannot initialise underlying xmlParserInputBuffer");
    }
  }

  ParserInputBuffer::~ParserInputBuffer() = default;

  bool ParserInputBuffer::on_close()
  {
    bool result = do_close();
    // the underlying structure is being freed by libxml, the pointer will soon be
    // invalid.
    impl_ = nullptr;

    return result;
  }

  int ParserInputBuffer::on_read(
      char * buffer,
      int len)
  {
    return do_read(buffer, len);
  }

  bool ParserInputBuffer::do_close()
  {
    return true;
  }

  _xmlParserInputBuffer* ParserInputBuffer::cobj() noexcept
  {
    return impl_;
  }

  const _xmlParserInputBuffer* ParserInputBuffer::cobj() const noexcept
  {
    return impl_;
  }

}
