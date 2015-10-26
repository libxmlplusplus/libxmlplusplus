/* parser.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

// Include glibmm/threads.h first. It must be the first file to include glib.h,
// because it temporarily undefines G_DISABLE_DEPRECATED while it includes glib.h.
#include <glibmm/threads.h> // For Glib::Threads::Mutex. Needed until the next API/ABI break.

#include "libxml++/exceptions/wrapped_exception.h"
#include "libxml++/parsers/parser.h"

#include <libxml/parser.h>

#include <memory> //For unique_ptr.
#include <map>

//TODO: See several TODOs in parser.h for changes at the next API/ABI break.

namespace // anonymous
{
// These are new data members that can't be added to xmlpp::Parser now,
// because it would break ABI.
struct ExtraParserData
{
  // Strange default values for throw_*_messages chosen for backward compatibility.
  ExtraParserData()
  : throw_parser_messages_(false), throw_validity_messages_(true),
  include_default_attributes_(false), set_options_(0), clear_options_(0)
  {}

  Glib::ustring parser_error_;
  Glib::ustring parser_warning_;
  bool throw_parser_messages_;
  bool throw_validity_messages_;
  bool include_default_attributes_;
  int set_options_;
  int clear_options_;
};

std::map<const xmlpp::Parser*, ExtraParserData> extra_parser_data;
// Different Parser instances may run in different threads.
// Accesses to extra_parser_data must be thread-safe.
Glib::Threads::Mutex extra_parser_data_mutex;

void on_parser_error(const xmlpp::Parser* parser, const Glib::ustring& message)
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  //Throw an exception later when the whole message has been received:
  extra_parser_data[parser].parser_error_ += message;
}

void on_parser_warning(const xmlpp::Parser* parser, const Glib::ustring& message)
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  //Throw an exception later when the whole message has been received:
  extra_parser_data[parser].parser_warning_ += message;
}
} // anonymous

namespace xmlpp {

Parser::Parser()
: context_(nullptr), exception_(nullptr), validate_(false), substitute_entities_(false) //See doxygen comment on set_substiute_entities().
{

}

Parser::~Parser()
{
  release_underlying();
  delete exception_;
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  extra_parser_data.erase(this);
}

void Parser::set_validate(bool val)
{
  validate_ = val;
}

bool Parser::get_validate() const
{
  return validate_;
}

void Parser::set_substitute_entities(bool val)
{
  substitute_entities_ = val;
}

bool Parser::get_substitute_entities() const
{
  return substitute_entities_;
}

void Parser::set_throw_messages(bool val)
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  extra_parser_data[this].throw_parser_messages_ = val;
  extra_parser_data[this].throw_validity_messages_ = val;
}

bool Parser::get_throw_messages() const
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  return extra_parser_data[this].throw_parser_messages_;
}

void Parser::set_include_default_attributes(bool val)
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  extra_parser_data[this].include_default_attributes_ = val;
}

bool Parser::get_include_default_attributes()
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  return extra_parser_data[this].include_default_attributes_;
}

void Parser::set_parser_options(int set_options, int clear_options)
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  extra_parser_data[this].set_options_ = set_options;
  extra_parser_data[this].clear_options_ = clear_options;
}

void Parser::get_parser_options(int& set_options, int& clear_options)
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  set_options = extra_parser_data[this].set_options_;
  clear_options = extra_parser_data[this].clear_options_;
}

void Parser::initialize_context()
{
  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);

  //Clear these temporary buffers:
  extra_parser_data[this].parser_error_.erase();
  extra_parser_data[this].parser_warning_.erase();
  validate_error_.erase();
  validate_warning_.erase();

  // Take a copy of the extra data, so we don't have to access
  // the extra_parser_data map more than necessary.
  const auto extra_parser_data_this = extra_parser_data[this];
  lock.release();

  //Disactivate any non-standards-compliant libxml1 features.
  //These are disactivated by default, but if we don't deactivate them for each context
  //then some other code which uses a global function, such as xmlKeepBlanksDefault(),
  // could cause this to use the wrong settings:
  context_->linenumbers = 1; // TRUE - This is the default anyway.

  //Turn on/off validation, entity substitution and default attribute inclusion.
  int options = context_->options;
  if (validate_)
    options |= XML_PARSE_DTDVALID;
  else
    options &= ~XML_PARSE_DTDVALID;

  if (substitute_entities_)
    options |= XML_PARSE_NOENT;
  else
    options &= ~XML_PARSE_NOENT;

  if (extra_parser_data_this.include_default_attributes_)
    options |= XML_PARSE_DTDATTR;
  else
    options &= ~XML_PARSE_DTDATTR;

  //Turn on/off any parser options.
  options |= extra_parser_data_this.set_options_;
  options &= ~extra_parser_data_this.clear_options_;

  xmlCtxtUseOptions(context_, options);

  if (context_->sax && extra_parser_data_this.throw_parser_messages_)
  {
    //Tell the parser context about the callbacks.
    context_->sax->fatalError = &callback_parser_error;
    context_->sax->error = &callback_parser_error;
    context_->sax->warning = &callback_parser_warning;
  }

  if (extra_parser_data_this.throw_validity_messages_)
  {
    //Tell the validity context about the callbacks:
    //(These are only called if validation is on - see above)
    context_->vctxt.error = &callback_validity_error;
    context_->vctxt.warning = &callback_validity_warning;
  }

  //Allow the callback_validity_*() methods to retrieve the C++ instance:
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

void Parser::on_validity_error(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  validate_error_ += message;
}

void Parser::on_validity_warning(const Glib::ustring& message)
{
  //Throw an exception later when the whole message has been received:
  validate_warning_ += message;
}

void Parser::check_for_validity_messages() // Also checks parser messages
{
  Glib::ustring msg(exception_ ? exception_->what() : "");
  bool parser_msg = false;
  bool validity_msg = false;

  Glib::Threads::Mutex::Lock lock(extra_parser_data_mutex);
  if (!extra_parser_data[this].parser_error_.empty())
  {
    parser_msg = true;
    msg += "\nParser error:\n" + extra_parser_data[this].parser_error_;
    extra_parser_data[this].parser_error_.erase();
  }

  if (!extra_parser_data[this].parser_warning_.empty())
  {
    parser_msg = true;
    msg += "\nParser warning:\n" + extra_parser_data[this].parser_warning_;
    extra_parser_data[this].parser_warning_.erase();
  }

  if (!validate_error_.empty())
  {
    validity_msg = true;
    msg += "\nValidity error:\n" + validate_error_;
    validate_error_.erase();
  }

  if (!validate_warning_.empty())
  {
    validity_msg = true;
    msg += "\nValidity warning:\n" + validate_warning_;
    validate_warning_.erase();
  }

  if (parser_msg || validity_msg)
  {
    delete exception_;
    if (validity_msg)
      exception_ = new validity_error(msg);
    else
      exception_ = new parse_error(msg);
  }
}
  
void Parser::callback_parser_error(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgParserError, ctx, msg, var_args);
  va_end(var_args);
}

void Parser::callback_parser_warning(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgParserWarning, ctx, msg, var_args);
  va_end(var_args);
}

void Parser::callback_validity_error(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgValidityError, ctx, msg, var_args);
  va_end(var_args);
}

void Parser::callback_validity_warning(void* ctx, const char* msg, ...)
{
  va_list var_args;
  va_start(var_args, msg);
  callback_error_or_warning(MsgValidityWarning, ctx, msg, var_args);
  va_end(var_args);
}

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
        char buff[1024];

        vsnprintf(buff, sizeof(buff)/sizeof(buff[0]), msg, var_args);
        ubuff = buff;
      }

      try
      {
        switch (msg_type)
        {
          case MsgParserError:
            on_parser_error(parser, ubuff);
            break;
          case MsgParserWarning:
            on_parser_warning(parser, ubuff);
            break;
          case MsgValidityError:
            parser->on_validity_error(ubuff);
            break;
          case MsgValidityWarning:
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

void Parser::handleException(const exception& e)
{
  delete exception_;
  exception_ = e.Clone();

  if(context_)
    xmlStopParser(context_);

  //release_underlying();
}

void Parser::handle_exception()
{
  delete exception_;
  exception_ = nullptr;

  try
  {
    throw; // Rethrow current exception
  }
  catch (const exception& e)
  {
    exception_ = e.Clone();
  }
#ifdef LIBXMLXX_HAVE_EXCEPTION_PTR
  catch (...)
  {
    exception_ = new wrapped_exception(std::current_exception());
  }
#else
  catch (const std::exception& e)
  {
    exception_ = new exception(e.what());
  }
  catch (...)
  {
    exception_ = new exception("An exception was thrown that is not derived from std::exception or xmlpp::exception.\n"
      "It could not be caught and rethrown because this platform does not support std::exception_ptr.");
  }
#endif

  if (context_)
    xmlStopParser(context_);

  //release_underlying();
}

void Parser::check_for_exception()
{
  check_for_validity_messages();
  
  if(exception_)
  {
    std::unique_ptr<exception> tmp(exception_);
    exception_ = nullptr;
    tmp->Raise();
  }
}

} // namespace xmlpp

