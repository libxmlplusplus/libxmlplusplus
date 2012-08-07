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
  : sax_handler_( new _xmlSAXHandler )
{
  xmlSAXHandler temp = {
    SaxParserCallback::internal_subset,
    0,  // isStandalone
    0,  // hasInternalSubset
    0,  // hasExternalSubset
    0,  // resolveEntity
    use_get_entity ? SaxParserCallback::get_entity : 0, // getEntity
    SaxParserCallback::entity_decl, // entityDecl
    0,  // notationDecl
    0,  // attributeDecl
    0,  // elementDecl
    0,  // unparsedEntityDecl
    0,  // setDocumentLocator
    SaxParserCallback::start_document, // startDocument
    SaxParserCallback::end_document, // endDocument
    SaxParserCallback::start_element, // startElement
    SaxParserCallback::end_element, // endElement
    0,  // reference
    SaxParserCallback::characters, // characters
    0,  // ignorableWhitespace
    0,  // processingInstruction
    SaxParserCallback::comment,  // comment
    SaxParserCallback::warning,  // warning
    SaxParserCallback::error,  // error
    SaxParserCallback::fatal_error, // fatalError
    0,  // getParameterEntity
    SaxParserCallback::cdata_block, // cdataBlock
    0,  // externalSubset
    0,  // initialized
    0,  // private
    0,  // startElementNs
    0,  // endElementNs
    0,  // serror
  };
  *sax_handler_ = temp;
}

SaxParser::~SaxParser()
{
  release_underlying();
}

xmlEntityPtr SaxParser::on_get_entity(const Glib::ustring& name)
{
  return entity_resolver_doc_.get_entity(name);
}

void SaxParser::on_entity_declaration(const Glib::ustring& name, XmlEntityType type, const Glib::ustring& publicId, const Glib::ustring& systemId, const Glib::ustring& content)
{
  entity_resolver_doc_.set_entity_declaration(name, type, publicId, systemId, content);
}  

void SaxParser::on_start_document()
{
}

void SaxParser::on_end_document()
{
}

void SaxParser::on_start_element(const Glib::ustring& /* name */, const AttributeList& /* attributes */)
{
}

void SaxParser::on_end_element(const Glib::ustring& /* name */)
{
}

void SaxParser::on_characters(const Glib::ustring& /* text */)
{
}

void SaxParser::on_comment(const Glib::ustring& /* text */)
{
}

void SaxParser::on_warning(const Glib::ustring& /* text */)
{
}

void SaxParser::on_error(const Glib::ustring& /* text */)
{
}


void SaxParser::on_fatal_error(const Glib::ustring& text)
{
  throw parse_error("Fatal error: " + text);
}

void SaxParser::on_cdata_block(const Glib::ustring& /* text */)
{
}

void SaxParser::on_internal_subset(const Glib::ustring& name,
                         const Glib::ustring& publicId,
                         const Glib::ustring& systemId)
{
  entity_resolver_doc_.set_internal_subset(name, publicId, systemId);
}

// implementation of this function is inspired by the SAX documentation by James Henstridge.
// (http://www.daa.com.au/~james/gnome/xml-sax/implementing.html)
void SaxParser::parse()
{
  //TODO If this is not the first parsing with this SaxParser, the xmlDoc object
  // in entity_resolver_doc_ should be deleted and replaced by a new one.
  // Otherwise entity declarations from a previous parsing may erroneously affect
  // this parsing. This would be much easier if entity_resolver_doc_ were a
  // std::auto_ptr<Document>, so the xmlpp::Document could be deleted and a new
  // one created. A good place for such code would be in an overridden
  // SaxParser::initialize_context(). It would be an ABI break.

  if(!context_)
  {
    throw internal_error("Parser context not created.");
  }

  xmlSAXHandlerPtr old_sax = context_->sax;
  context_->sax = sax_handler_.get();

  xmlResetLastError();
  initialize_context();
  
  const int parseError = xmlParseDocument(context_);

  context_->sax = old_sax;

  Glib::ustring error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError == -1)
    error_str = "xmlParseDocument() failed.";

  release_underlying(); // Free context_

  check_for_exception();

  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}

void SaxParser::parse_file(const Glib::ustring& filename)
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
  
void SaxParser::parse_memory(const Glib::ustring& contents)
{
  parse_memory_raw((const unsigned char*)contents.c_str(), contents.bytes());
}

void SaxParser::parse_stream(std::istream& in)
{
  if(context_)
  {
    throw parse_error("Attempt to start a second parse while a parse is in progress.");
  }

  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      0, // user_data
      0, // chunk
      0, // size
      0); // no filename for fetching external entities

  if(!context_)
  {
    throw internal_error("Could not create parser context\n" + format_xml_error());
  }

  initialize_context();

  //TODO: Shouldn't we use a Glib::ustring here, and some alternative to std::getline()?
  int firstParseError = XML_ERR_OK;
  std::string line;
  while( ( ! exception_ )
      && std::getline(in, line))
  {
    // since getline does not get the line separator, we have to add it since the parser care
    // about layout in certain cases.
    line += '\n';

    const int parseError = xmlParseChunk(context_, line.c_str(),
      line.size() /* This is a std::string, not a ustring, so this is the number of bytes. */,
      0 /* don't terminate */);

    // Save the first error code if any, but read on.
    // More errors might be reported and then thrown by check_for_exception().
    if (parseError != XML_ERR_OK && firstParseError == XML_ERR_OK)
      firstParseError = parseError;
  }

  if( ! exception_ )
  {
     //This is called just to terminate parsing.
    const int parseError = xmlParseChunk(context_, 0 /* chunk */, 0 /* size */, 1 /* terminate (1 or 0) */);

    if (parseError != XML_ERR_OK && firstParseError == XML_ERR_OK)
      firstParseError = parseError;
  }

  Glib::ustring error_str = format_xml_parser_error(context_);
  if (error_str.empty() && firstParseError != XML_ERR_OK)
    error_str = "Error code from xmlParseChunk(): " + Glib::ustring::format(firstParseError);

  release_underlying(); // Free context_

  check_for_exception();

  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}

void SaxParser::parse_chunk(const Glib::ustring& chunk)
{
  parse_chunk_raw((const unsigned char*)chunk.c_str(), chunk.bytes());
}

void SaxParser::parse_chunk_raw(const unsigned char* contents, size_type bytes_count)
{
  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  if(!context_)
  {
    context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      0, // user_data
      0, // chunk
      0, // size
      0); // no filename for fetching external entities

    if(!context_)
    {
      throw internal_error("Could not create parser context\n" + format_xml_error());
    }
    initialize_context();
  }
  else
    xmlCtxtResetLastError(context_);
  
  int parseError = XML_ERR_OK;
  if(!exception_)
    parseError = xmlParseChunk(context_, (const char*)contents, bytes_count, 0 /* don't terminate */);

  check_for_exception();

  Glib::ustring error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError != XML_ERR_OK)
    error_str = "Error code from xmlParseChunk(): " + Glib::ustring::format(parseError);
  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}

void SaxParser::release_underlying()
{
  Parser::release_underlying();
}

void SaxParser::finish_chunk_parsing()
{
  xmlResetLastError();
  if(!context_)
  {
    context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      0, // this, // user_data
      0, // chunk
      0, // size
      0); // no filename for fetching external entities

    if(!context_)
    {
      throw internal_error("Could not create parser context\n" + format_xml_error());
    }
    initialize_context();
  }
  else
    xmlCtxtResetLastError(context_);

  int parseError = XML_ERR_OK;
  if(!exception_)
    //This is called just to terminate parsing.
    parseError = xmlParseChunk(context_, 0 /* chunk */, 0 /* size */, 1 /* terminate (1 or 0) */);

  Glib::ustring error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError != XML_ERR_OK)
    error_str = "Error code from xmlParseChunk(): " + Glib::ustring::format(parseError);

  release_underlying(); // Free context_

  check_for_exception();

  if(!error_str.empty())
  {
    throw parse_error(error_str);
  }
}


xmlEntityPtr SaxParserCallback::get_entity(void* context, const xmlChar* name)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);
  xmlEntityPtr result = 0;

  try
  {
    result = parser->on_get_entity((const char*)name);
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
  
  return result;
}

void SaxParserCallback::entity_decl(void* context, const xmlChar* name, int type, const xmlChar* publicId, const xmlChar* systemId, xmlChar* content)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_entity_declaration(
      ( name ? Glib::ustring((const char*)name) : ""),
      static_cast<XmlEntityType>(type),
      ( publicId ? Glib::ustring((const char*)publicId) : ""),
      ( systemId ? Glib::ustring((const char*)systemId) : ""),
      ( content ? Glib::ustring((const char*)content) : "") );
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::start_document(void* context)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_start_document();
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::end_document(void* context)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  if(parser->exception_)
    return;

  try
  {
    parser->on_end_document();
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::start_element(void* context,
                                        const xmlChar* name,
                                        const xmlChar** p)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  SaxParser::AttributeList attributes;

  if(p)
    for(const xmlChar** cur = p; cur && *cur; cur += 2)
      attributes.push_back(
			  SaxParser::Attribute( (char*)*cur, (char*)*(cur + 1) ));

  try
  {
    parser->on_start_element(Glib::ustring((const char*) name), attributes);
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::end_element(void* context, const xmlChar* name)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_end_element(Glib::ustring((const char*) name));
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::characters(void * context, const xmlChar* ch, int len)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    // Here we force the use of Glib::ustring::ustring( InputIterator begin, InputIterator end )
    // instead of Glib::ustring::ustring( const char*, size_type ) because it
    // expects the length of the string in characters, not in bytes.
    parser->on_characters(
        Glib::ustring(
          reinterpret_cast<const char *>(ch),
          reinterpret_cast<const char *>(ch + len) ) );
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::comment(void* context, const xmlChar* value)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    parser->on_comment(Glib::ustring((const char*) value));
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::warning(void* context, const char* fmt, ...)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  va_list arg;
  char buff[1024]; //TODO: Larger/Shared

  va_start(arg, fmt);
  vsnprintf(buff, sizeof(buff)/sizeof(buff[0]), fmt, arg);
  va_end(arg);

  try
  {
    parser->on_warning(Glib::ustring(buff));
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::error(void* context, const char* fmt, ...)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  va_list arg;
  char buff[1024]; //TODO: Larger/Shared

  if(parser->exception_)
    return;

  va_start(arg, fmt);
  vsnprintf(buff, sizeof(buff)/sizeof(buff[0]), fmt, arg);
  va_end(arg);

  try
  {
    parser->on_error(Glib::ustring(buff));
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::fatal_error(void* context, const char* fmt, ...)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  va_list arg;
  char buff[1024]; //TODO: Larger/Shared

  va_start(arg, fmt);
  vsnprintf(buff, sizeof(buff)/sizeof(buff[0]), fmt, arg);
  va_end(arg);

  try
  {
    parser->on_fatal_error(Glib::ustring(buff));
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::cdata_block(void* context, const xmlChar* value, int len)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);

  try
  {
    // Here we force the use of Glib::ustring::ustring( InputIterator begin, InputIterator end )
    // see comments in SaxParserCallback::characters
    parser->on_cdata_block(
        Glib::ustring(
          reinterpret_cast<const char *>(value),
          reinterpret_cast<const char *>(value + len) ) );
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

void SaxParserCallback::internal_subset(void* context, const xmlChar* name,
  const xmlChar* publicId, const xmlChar* systemId)
{
  _xmlParserCtxt* the_context = static_cast<_xmlParserCtxt*>(context);
  SaxParser* parser = static_cast<SaxParser*>(the_context->_private);
  
  try
  {
    const Glib::ustring pid = publicId ? Glib::ustring((const char*) publicId) : "";
    const Glib::ustring sid = systemId ? Glib::ustring((const char*) systemId) : "";

    parser->on_internal_subset( Glib::ustring((const char*) name), pid, sid);
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  }
}

} // namespace xmlpp
