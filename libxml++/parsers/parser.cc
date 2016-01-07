/* parser.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/exceptions/wrapped_exception.h"
#include "libxml++/parsers/parser.h"

#include <libxml/parser.h>

namespace xmlpp
{

struct Parser::Impl
{
  Impl()
  :
  throw_messages_(true), validate_(false), substitute_entities_(false),
  include_default_attributes_(false), set_options_(0), clear_options_(0)
  {}

  // Built gradually - used in an exception at the end of parsing.
  Glib::ustring parser_error_;
  Glib::ustring parser_warning_;
  Glib::ustring validate_error_;
  Glib::ustring validate_warning_;

  bool throw_messages_;
  bool validate_;
  bool substitute_entities_;
  bool include_default_attributes_;
  int set_options_;
  int clear_options_;
};

Parser::Parser()
: context_(nullptr), exception_(nullptr), pimpl_(new Impl)
{
}

Parser::~Parser()
{
  release_underlying();
}

void Parser::set_validate(bool val) noexcept
{
  pimpl_->validate_ = val;
}

bool Parser::get_validate() const noexcept
{
  return pimpl_->validate_;
}

void Parser::set_substitute_entities(bool val) noexcept
{
  pimpl_->substitute_entities_ = val;
}

bool Parser::get_substitute_entities() const noexcept
{
  return pimpl_->substitute_entities_;
}

void Parser::set_throw_messages(bool val) noexcept
{
  pimpl_->throw_messages_ = val;
}

bool Parser::get_throw_messages() const noexcept
{
  return pimpl_->throw_messages_;
}

void Parser::set_include_default_attributes(bool val) noexcept
{
  pimpl_->include_default_attributes_ = val;
}

bool Parser::get_include_default_attributes() noexcept
{
  return pimpl_->include_default_attributes_;
}

void Parser::set_parser_options(int set_options, int clear_options) noexcept
{
  pimpl_->set_options_ = set_options;
  pimpl_->clear_options_ = clear_options;
}

void Parser::get_parser_options(int& set_options, int& clear_options) noexcept
{
  set_options = pimpl_->set_options_;
  clear_options = pimpl_->clear_options_;
}

void Parser::initialize_context()
{
  //Clear these temporary buffers:
  pimpl_->parser_error_.erase();
  pimpl_->parser_warning_.erase();
  pimpl_->validate_error_.erase();
  pimpl_->validate_warning_.erase();

  //Disactivate any non-standards-compliant libxml1 features.
  //These are disactivated by default, but if we don't deactivate them for each context
  //then some other code which uses a global function, such as xmlKeepBlanksDefault(),
  // could cause this to use the wrong settings:
  context_->linenumbers = 1; // TRUE - This is the default anyway.

  //Turn on/off validation, entity substitution and default attribute inclusion.
  int options = context_->options;
  if (pimpl_->validate_)
    options |= XML_PARSE_DTDVALID;
  else
    options &= ~XML_PARSE_DTDVALID;

  if (pimpl_->substitute_entities_)
    options |= XML_PARSE_NOENT;
  else
    options &= ~XML_PARSE_NOENT;

  if (pimpl_->include_default_attributes_)
    options |= XML_PARSE_DTDATTR;
  else
    options &= ~XML_PARSE_DTDATTR;

  //Turn on/off any parser options.
  options |= pimpl_->set_options_;
  options &= ~pimpl_->clear_options_;

  xmlCtxtUseOptions(context_, options);

  if (context_->sax && pimpl_->throw_messages_)
  {
    //Tell the parser context about the callbacks.
    context_->sax->fatalError = &callback_parser_error;
    context_->sax->error = &callback_parser_error;
    context_->sax->warning = &callback_parser_warning;
  }

  if (pimpl_->throw_messages_)
  {
    //Tell the validity context about the callbacks:
    //(These are only called if validation is on - see above)
    context_->vctxt.error = &callback_validity_error;
    context_->vctxt.warning = &callback_validity_warning;
  }

  //Allow callback_error_or_warning() to retrieve the C++ instance:
  context_->_private = this;
}

void Parser::release_underlying()
{
  if(context_)
  {
    context_->_private = nullptr; //Not really necessary.

    if( context_->myDoc != nullptr )
    {
      xmlFreeDoc(context_->myDoc);
    }

    xmlFreeParserCtxt(context_);
    context_ = nullptr;
  }
}

void Parser::on_parser_error(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->parser_error_ += message;
}

void Parser::on_parser_warning(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->parser_warning_ += message;
}
void Parser::on_validity_error(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->validate_error_ += message;
}

void Parser::on_validity_warning(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->validate_warning_ += message;
}

void Parser::check_for_error_and_warning_messages()
{
  Glib::ustring msg(exception_ ? exception_->what() : "");
  bool parser_msg = false;
  bool validity_msg = false;

  if (!pimpl_->parser_error_.empty())
  {
    parser_msg = true;
    msg += "\nParser error:\n" + pimpl_->parser_error_;
    pimpl_->parser_error_.erase();
  }

  if (!pimpl_->parser_warning_.empty())
  {
    parser_msg = true;
    msg += "\nParser warning:\n" + pimpl_->parser_warning_;
    pimpl_->parser_warning_.erase();
  }

  if (!pimpl_->validate_error_.empty())
  {
    validity_msg = true;
    msg += "\nValidity error:\n" + pimpl_->validate_error_;
    pimpl_->validate_error_.erase();
  }

  if (!pimpl_->validate_warning_.empty())
  {
    validity_msg = true;
    msg += "\nValidity warning:\n" + pimpl_->validate_warning_;
    pimpl_->validate_warning_.erase();
  }

  if (validity_msg)
    exception_.reset(new validity_error(msg));
  else if (parser_msg)
    exception_.reset(new parse_error(msg));
}

//static
void Parser::callback_parser_error(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgType::ParserError, ctx, msg, var_args);
  va_end(var_args);
}

//static
void Parser::callback_parser_warning(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgType::ParserWarning, ctx, msg, var_args);
  va_end(var_args);
}

//static
void Parser::callback_validity_error(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgType::ValidityError, ctx, msg, var_args);
  va_end(var_args);
}

//static
void Parser::callback_validity_warning(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgType::ValidityWarning, ctx, msg, var_args);
  va_end(var_args);
}

//static
void Parser::callback_error_or_warning(MsgType msg_type, void* ctx,
                                       const char* msg, va_list var_args)
{
  //See xmlHTMLValidityError() in xmllint.c in libxml for more about this:

  auto context = (xmlParserCtxtPtr)ctx;
  if(context)
  {
    auto parser = static_cast<Parser*>(context->_private);
    if(parser)
    {
      auto ubuff = format_xml_error(&context->lastError);
      if (ubuff.empty())
      {
        // Usually the result of formatting var_args with the format string msg
        // is the same string as is stored in context->lastError.message.
        // It's unnecessary to use msg and var_args, if format_xml_error()
        // returns an error message (as it usually does).

        //Convert the ... to a string:
        ubuff = format_printf_message(msg, var_args);
      }

      try
      {
        switch (msg_type)
        {
          case MsgType::ParserError:
            parser->on_parser_error(ubuff);
            break;
          case MsgType::ParserWarning:
            parser->on_parser_warning(ubuff);
            break;
          case MsgType::ValidityError:
            parser->on_validity_error(ubuff);
            break;
          case MsgType::ValidityWarning:
            parser->on_validity_warning(ubuff);
            break;
        }
      }
      catch (...)
      {
        parser->handle_exception();
      }
    }
  }
}

void Parser::handle_exception()
{
  try
  {
    throw; // Re-throw current exception
  }
  catch (const exception& e)
  {
    exception_.reset(e.clone());
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch (...)
  {
    exception_.reset(new wrapped_exception(std::current_exception()));
  }
#else
  catch (const std::exception& e)
  {
    exception_.reset(new exception(e.what()));
  }
  catch (...)
  {
    exception_.reset(new exception("An exception was thrown that is not derived from std::exception or xmlpp::exception.\n"
      "It could not be caught and rethrown because this platform does not support std::exception_ptr."));
  }
#endif

  if (context_)
    xmlStopParser(context_);

  //release_underlying();
}

void Parser::check_for_exception()
{
  check_for_error_and_warning_messages();

  if (exception_)
  {
    std::unique_ptr<exception> tmp(std::move(exception_));
    tmp->raise();
  }
}

} // namespace xmlpp
