/* parser.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/exceptions/wrapped_exception.h"
#include "libxml++/parsers/parser.h"

#include <libxml/parser.h>

namespace
{
// C++ linkage
using ErrorOrWarningFuncType = void (*)(bool parser, bool error, void* ctx,
                                        const char* msg, va_list var_args);
ErrorOrWarningFuncType p_callback_error_or_warning;

extern "C"
{
static void c_callback_parser_error(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  p_callback_error_or_warning(true, true, ctx, msg, var_args);
  va_end(var_args);
}

static void c_callback_parser_warning(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  p_callback_error_or_warning(true, false, ctx, msg, var_args);
  va_end(var_args);
}
static void c_callback_validity_error(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  p_callback_error_or_warning(false, true, ctx, msg, var_args);
  va_end(var_args);
}

static void c_callback_validity_warning(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  p_callback_error_or_warning(false, false, ctx, msg, var_args);
  va_end(var_args);
}
} // extern "C"
} // anonymous namespace

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
  ustring parser_error_;
  ustring parser_warning_;
  ustring validate_error_;
  ustring validate_warning_;

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

bool Parser::get_include_default_attributes() const noexcept
{
  return pimpl_->include_default_attributes_;
}

void Parser::set_parser_options(int set_options, int clear_options) noexcept
{
  pimpl_->set_options_ = set_options;
  pimpl_->clear_options_ = clear_options;
}

void Parser::get_parser_options(int& set_options, int& clear_options) const noexcept
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
    context_->sax->fatalError = get_callback_parser_error_cfunc();
    context_->sax->error = get_callback_parser_error_cfunc();
    context_->sax->warning = get_callback_parser_warning_cfunc();
  }

  if (pimpl_->throw_messages_)
  {
    //Tell the validity context about the callbacks:
    //(These are only called if validation is on - see above)
    context_->vctxt.error = get_callback_validity_error_cfunc();
    context_->vctxt.warning = get_callback_validity_warning_cfunc();
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

void Parser::on_parser_error(const ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->parser_error_ += message;
}

void Parser::on_parser_warning(const ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->parser_warning_ += message;
}
void Parser::on_validity_error(const ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->validate_error_ += message;
}

void Parser::on_validity_warning(const ustring& message)
{
  //Throw an exception later when the whole message has been received:
  pimpl_->validate_warning_ += message;
}

void Parser::check_for_error_and_warning_messages()
{
  ustring msg(exception_ ? exception_->what() : "");
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
    exception_ = std::make_unique<validity_error>(msg);
  else if (parser_msg)
    exception_ = std::make_unique<parse_error>(msg);
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
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
  const bool is_parser = (msg_type == MsgType::ParserError) || (msg_type == MsgType::ParserWarning);
  const bool is_error = (msg_type == MsgType::ParserError) || (msg_type == MsgType::ValidityError);
  callback_error_or_warning(is_parser, is_error, ctx, msg, var_args);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

//static
ParserCallbackCFuncType Parser::get_callback_parser_error_cfunc()
{
  p_callback_error_or_warning = &callback_error_or_warning;
  return &c_callback_parser_error;
}

//static
ParserCallbackCFuncType Parser::get_callback_parser_warning_cfunc()
{
  p_callback_error_or_warning = &callback_error_or_warning;
  return &c_callback_parser_warning;
}

//static
ParserCallbackCFuncType Parser::get_callback_validity_error_cfunc()
{
  p_callback_error_or_warning = &callback_error_or_warning;
  return &c_callback_validity_error;
}

//static
ParserCallbackCFuncType Parser::get_callback_validity_warning_cfunc()
{
  p_callback_error_or_warning = &callback_error_or_warning;
  return &c_callback_validity_warning;
}

//static
void Parser::callback_error_or_warning(bool is_parser, bool is_error, void* ctx,
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
        if (is_parser)
        {
          if (is_error)
            parser->on_parser_error(ubuff);
          else
            parser->on_parser_warning(ubuff);
        }
        else // validator
        {
          if (is_error)
            parser->on_validity_error(ubuff);
          else
            parser->on_validity_warning(ubuff);
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
    exception_ = std::make_unique<wrapped_exception>(std::current_exception());
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
