/* saxparser.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 *
 * 2002/01/05 Valentin Rusu - fixed some potential buffer overruns
 * 2002/01/21 Valentin Rusu - added CDATA handlers
 */

#include "libxml++/parsers/saxparser.h"
#include "libxml++/nodes/element.h"
#include "libxml++/keepblanks.h"

#include <libxml/parser.h>
#include <libxml/parserInternals.h> // for xmlCreateFileParserCtxt

#include <cstdarg> //For va_list.
#include <iostream>

namespace {
  extern "C" {
    static int _io_read_callback(void * context,
                                 char * buffer,
                                 int len)
    {
      auto in = static_cast<std::istream*>(context);
      in->read(buffer, len);
      return in->gcount();
    }
  }
}

namespace xmlpp {

struct SaxParserCallback
{
  static xmlEntityPtr get_entity(void* context, const xmlChar* name);
  static void entity_decl(void* context, const xmlChar* name, int type, const xmlChar* publicId, const xmlChar* systemId, xmlChar* content);
  static void start_document(void* context);
  static void end_document(void* context);
  static void start_element(void* context, const xmlChar* name, const xmlChar** p);
  static void end_element(void* context, const xmlChar* name);
  static void characters(void* context, const xmlChar* ch, int len);
  static void comment(void* context, const xmlChar* value);
  static void warning(void* context, const char* fmt, ...);
  static void error(void* context, const char* fmt, ...);
  static void fatal_error(void* context, const char* fmt, ...);
  static void cdata_block(void* context, const xmlChar* value, int len);
  static void internal_subset(void* context, const xmlChar* name, const xmlChar*publicId, const xmlChar*systemId);
};



SaxParser::SaxParser(bool use_get_entity)
  : sax_handler_(new _xmlSAXHandler), entity_resolver_doc_(new Document)
{
  xmlSAXHandler temp = {
    SaxParserCallback::internal_subset,
    nullptr, // isStandalone
    nullptr, // hasInternalSubset
    nullptr, // hasExternalSubset
    nullptr, // resolveEntity
    use_get_entity ? SaxParserCallback::get_entity : nullptr, // getEntity
    SaxParserCallback::entity_decl, // entityDecl
    nullptr, // notationDecl
    nullptr, // attributeDecl
    nullptr, // elementDecl
    nullptr, // unparsedEntityDecl
    nullptr, // setDocumentLocator
    SaxParserCallback::start_document, // startDocument
    SaxParserCallback::end_document, // endDocument
    SaxParserCallback::start_element, // startElement
    SaxParserCallback::end_element, // endElement
    nullptr, // reference
    SaxParserCallback::characters, // characters
    nullptr, // ignorableWhitespace
    nullptr, // processingInstruction
    SaxParserCallback::comment,  // comment
    SaxParserCallback::warning,  // warning
    SaxParserCallback::error,  // error
    SaxParserCallback::fatal_error, // fatalError
    nullptr, // getParameterEntity
    SaxParserCallback::cdata_block, // cdataBlock
    nullptr, // externalSubset
    0,       // initialized
    nullptr, // private
    nullptr, // startElementNs
    nullptr, // endElementNs
    nullptr, // serror
  };
  *sax_handler_ = temp;

  // The default action is to call on_warning(), on_error(), on_fatal_error().
  set_throw_messages(false);
}

SaxParser::~SaxParser()
{
  release_underlying();
}

xmlEntityPtr SaxParser::on_get_entity(const ustring& name)
{
  return entity_resolver_doc_->get_entity(name);
}

void SaxParser::on_entity_declaration(const ustring& name, XmlEntityType type, const ustring& publicId, const ustring& systemId, const ustring& content)
{
  entity_resolver_doc_->set_entity_declaration(name, type, publicId, systemId, content);
}

void SaxParser::on_start_document()
{
}

void SaxParser::on_end_document()
{
}

void SaxParser::on_start_element(const ustring& /* name */, const AttributeList& /* attributes */)
{
}

void SaxParser::on_end_element(const ustring& /* name */)
{
}

void SaxParser::on_characters(const ustring& /* text */)
{
}

void SaxParser::on_comment(const ustring& /* text */)
{
}

void SaxParser::on_warning(const ustring& /* text */)
{
}

void SaxParser::on_error(const ustring& /* text */)
{
}


void SaxParser::on_fatal_error(const ustring& text)
{
  throw parse_error("Fatal error: " + text);
}

void SaxParser::on_cdata_block(const ustring& /* text */)
{
}

void SaxParser::on_internal_subset(const ustring& name,
                         const ustring& publicId,
                         const ustring& systemId)
{
  entity_resolver_doc_->set_internal_subset(name, publicId, systemId);
}

// implementation of this function is inspired by the SAX documentation by James Henstridge.
// (http://www.daa.com.au/~james/gnome/xml-sax/implementing.html)
void SaxParser::parse()
{
  if(!context_)
  {
    throw internal_error("Parser context not created.");
  }

  auto old_sax = context_->sax;
  context_->sax = sax_handler_.get();

  xmlResetLastError();
  initialize_context();

  const int parseError = xmlParseDocument(context_);

  context_->sax = old_sax;

  auto error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError == -1)
    error_str = "xmlParseDocument() failed.";

  release_underlying(); // Free context_

  check_for_exception();

  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}

void SaxParser::parse_file(const std::string& filename)
{
  if(context_)
  {
    throw parse_error("Attempt to start a second parse while a parse is in progress.");
  }

  KeepBlanks k(KeepBlanks::Default);

  context_ = xmlCreateFileParserCtxt(filename.c_str());
  parse();
}

void SaxParser::parse_memory_raw(const unsigned char* contents, size_type bytes_count)
{
  if(context_)
  {
    throw parse_error("Attempt to start a second parse while a parse is in progress.");
  }

  KeepBlanks k(KeepBlanks::Default);

  context_ = xmlCreateMemoryParserCtxt((const char*)contents, bytes_count);
  parse();
}

void SaxParser::parse_memory(const ustring& contents)
{
  parse_memory_raw((const unsigned char*)contents.c_str(), contents.size());
}

void SaxParser::parse_stream(std::istream& in)
{
  if(context_)
  {
    throw parse_error("Attempt to start a second parse while a parse is in progress.");
  }

  KeepBlanks k(KeepBlanks::Default);

  context_ = xmlCreateIOParserCtxt(
      sax_handler_.get(),
      nullptr, // user_data
      _io_read_callback,
      nullptr, // inputCloseCallback
      &in,
      XML_CHAR_ENCODING_NONE);
  parse();
}

void SaxParser::parse_chunk(const ustring& chunk)
{
  parse_chunk_raw((const unsigned char*)chunk.c_str(), chunk.size());
}

void SaxParser::parse_chunk_raw(const unsigned char* contents, size_type bytes_count)
{
  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  if(!context_)
  {
    context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      nullptr,  // user_data
      nullptr,  // chunk
      0,        // size
      nullptr); // no filename for fetching external entities

    if(!context_)
    {
      throw internal_error("Could not create parser context\n" + format_xml_error());
    }
    initialize_context();
  }
  else
    xmlCtxtResetLastError(context_);

  int parseError = XML_ERR_OK;
  if (!exception_)
    parseError = xmlParseChunk(context_, (const char*)contents, bytes_count, 0 /* don't terminate */);

  check_for_exception();

  auto error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError != XML_ERR_OK)
    error_str = "Error code from xmlParseChunk(): " + std::to_string(parseError);
  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}

void SaxParser::finish_chunk_parsing()
{
  xmlResetLastError();
  if(!context_)
  {
    context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      nullptr,  // user_data
      nullptr,  // chunk
      0,        // size
      nullptr); // no filename for fetching external entities

    if(!context_)
    {
      throw internal_error("Could not create parser context\n" + format_xml_error());
    }
    initialize_context();
  }
  else
    xmlCtxtResetLastError(context_);

  int parseError = XML_ERR_OK;
  if (!exception_)
    //This is called just to terminate parsing.
    parseError = xmlParseChunk(context_, nullptr /* chunk */, 0 /* size */, 1 /* terminate (1 or 0) */);

  auto error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError != XML_ERR_OK)
    error_str = "Error code from xmlParseChunk(): " + std::to_string(parseError);

  release_underlying(); // Free context_

  check_for_exception();

  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}

void SaxParser::release_underlying()
{
  Parser::release_underlying();
}

void SaxParser::initialize_context()
{
  Parser::initialize_context();
  // Start with an empty Document for entity resolution.
  entity_resolver_doc_ = std::make_unique<Document>();
}


xmlEntityPtr SaxParserCallback::get_entity(void* context, const xmlChar* name)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);
  xmlEntityPtr result = nullptr;

  try
  {
    result = parser->on_get_entity((const char*)name);
  }
  catch (...)
  {
    parser->handle_exception();
  }

  return result;
}

void SaxParserCallback::entity_decl(void* context, const xmlChar* name, int type, const xmlChar* publicId, const xmlChar* systemId, xmlChar* content)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_entity_declaration(
      ( name ? ustring((const char*)name) : ""),
      static_cast<XmlEntityType>(type),
      ( publicId ? ustring((const char*)publicId) : ""),
      ( systemId ? ustring((const char*)systemId) : ""),
      ( content ? ustring((const char*)content) : "") );
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::start_document(void* context)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_start_document();
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::end_document(void* context)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  if (parser->exception_)
    return;

  try
  {
    parser->on_end_document();
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::start_element(void* context,
                                        const xmlChar* name,
                                        const xmlChar** p)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  SaxParser::AttributeList attributes;

  if(p)
    for(const xmlChar** cur = p; cur && *cur; cur += 2)
      attributes.push_back(
			  SaxParser::Attribute( (char*)*cur, (char*)*(cur + 1) ));

  try
  {
    parser->on_start_element(ustring((const char*) name), attributes);
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::end_element(void* context, const xmlChar* name)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_end_element(ustring((const char*) name));
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::characters(void * context, const xmlChar* ch, int len)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    // Here we force the use of ustring::ustring( InputIterator begin, InputIterator end )
    // instead of ustring::ustring( const char*, size_type ) because it
    // expects the length of the string in characters, not in bytes.
    parser->on_characters(
        ustring(
          reinterpret_cast<const char *>(ch),
          reinterpret_cast<const char *>(ch + len) ) );
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::comment(void* context, const xmlChar* value)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_comment(ustring((const char*) value));
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::warning(void* context, const char* fmt, ...)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  va_list arg;
  va_start(arg, fmt);
  const ustring buff = format_printf_message(fmt, arg);
  va_end(arg);

  try
  {
    parser->on_warning(buff);
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::error(void* context, const char* fmt, ...)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  if (parser->exception_)
    return;

  va_list arg;
  va_start(arg, fmt);
  const ustring buff = format_printf_message(fmt, arg);
  va_end(arg);

  try
  {
    parser->on_error(buff);
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::fatal_error(void* context, const char* fmt, ...)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  va_list arg;
  va_start(arg, fmt);
  const ustring buff = format_printf_message(fmt, arg);
  va_end(arg);

  try
  {
    parser->on_fatal_error(buff);
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::cdata_block(void* context, const xmlChar* value, int len)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    // Here we force the use of ustring::ustring( InputIterator begin, InputIterator end )
    // see comments in SaxParserCallback::characters
    parser->on_cdata_block(
        ustring(
          reinterpret_cast<const char *>(value),
          reinterpret_cast<const char *>(value + len) ) );
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

void SaxParserCallback::internal_subset(void* context, const xmlChar* name,
  const xmlChar* publicId, const xmlChar* systemId)
{
  auto the_context = static_cast<_xmlParserCtxt*>(context);
  auto parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    const auto pid = publicId ? ustring((const char*) publicId) : "";
    const auto sid = systemId ? ustring((const char*) systemId) : "";

    parser->on_internal_subset( ustring((const char*) name), pid, sid);
  }
  catch (...)
  {
    parser->handle_exception();
  }
}

} // namespace xmlpp
