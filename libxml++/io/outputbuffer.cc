/* document.h
 * this file is part of libxml++
 *
 * copyright (C) 2003 by libxml++ developer's team
 *
 * this file is covered by the GNU Lesser General Public License,
 * which should be included with libxml++ as the file COPYING.
 */

#include <libxml++/io/outputbuffer.h>
#include <libxml++/exceptions/internal_error.h>

#include <libxml/globals.h> //Needed by libxml/xmlIO.h
#include <libxml/xmlIO.h>

namespace xmlpp
{
  struct OutputBufferCallback
  {
    static int on_write(void * context, const char * buffer, int len)
    {
      auto tmp = static_cast<OutputBuffer*>(context);
      return tmp->on_write(buffer, len)?len:-1;
    }

    static int on_close(void * context)
    {
      auto tmp = static_cast<OutputBuffer*>(context);
      return tmp->on_close()?0:-1;
    }
  };


  OutputBuffer::OutputBuffer(
      const ustring& encoding)
  {
    // we got to initialise the char encoding handler
    // The code is almost cut/paste from xmlSaveFormatFileEnc
    // TODO wrap the handler ? I don't think so but...

    xmlCharEncodingHandlerPtr handler = nullptr;
    if( ! encoding.empty() )
    {
      auto enc = xmlParseCharEncoding(encoding.c_str());

      // TODO we assume that the source will be UTF-8 encoded. Any user of the class
      // should pay attention to this.

      if( enc != XML_CHAR_ENCODING_UTF8 )
      {
        handler = xmlFindCharEncodingHandler(encoding.c_str());
        if(handler == nullptr)
        {
          throw internal_error("Cannot initialise an encoder to " + encoding);
        }
      }
    }

    impl_ = xmlOutputBufferCreateIO(
        &OutputBufferCallback::on_write,
        &OutputBufferCallback::on_close,
        static_cast<void*>(this),
        handler);
    if(impl_ == nullptr)
    {
      throw internal_error("Cannot initialise underlying xmlOutputBuffer");
    }
  }

  OutputBuffer::~OutputBuffer() = default;

  bool OutputBuffer::on_close()
  {
    bool result = do_close();
    // the underlying structure is being freed by libxml, the pointer will soon be
    // invalid.
    impl_ = nullptr;

    return result;
  }

  bool OutputBuffer::on_write(
      const char * buffer,
      int len)
  {
    return do_write(buffer, len);
  }

  bool OutputBuffer::do_close()
  {
    return true;
  }

  _xmlOutputBuffer* OutputBuffer::cobj() noexcept
  {
    return impl_;
  }

  const _xmlOutputBuffer* OutputBuffer::cobj() const noexcept
  {
    return impl_;
  }

}
