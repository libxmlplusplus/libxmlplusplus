/* xml++.cc
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
#include <cassert> // for assert()
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
    0  // externalSubset
  };
  *sax_handler_ = temp;
}

SaxParser::~SaxParser()
{
  release_underlying();
}

xmlEntityPtr SaxParser::on_get_entity(const std::string& name)
{
  return entity_resolver_doc_.get_entity(name);
}

void SaxParser::on_entity_declaration(const std::string& name, XmlEntityType type, const std::string& publicId, const std::string& systemId, const std::string& content)
{
  entity_resolver_doc_.set_entity_declaration(name, type, publicId, systemId, content);
}  

void SaxParser::on_start_document()
{
}

void SaxParser::on_end_document()
{
}

void SaxParser::on_start_element(const std::string& name, const AttributeList& attributes)
{
}

void SaxParser::on_end_element(const std::string& name)
{
}

void SaxParser::on_characters(const std::string& text)
{
}

void SaxParser::on_comment(const std::string& text)
{
}

void SaxParser::on_warning(const std::string& text)
{
}

void SaxParser::on_error(const std::string& text)
{
}

void SaxParser::on_fatal_error(const std::string& text)
{
  throw parse_error("Fatal error: " + text);
}

void SaxParser::on_cdata_block(const std::string& text)
{
}

void SaxParser::on_internal_subset(const std::string& name,
                         const std::string& publicId,
                         const std::string& systemId)
{
  entity_resolver_doc_.set_internal_subset(name, publicId, systemId);
}

// implementation of this function is inspired by the SAX documentation by James Henstridge.
// (http://www.daa.com.au/~james/gnome/xml-sax/implementing.html)
void SaxParser::parse()
{
  if(!context_)
    throw internal_error("Parse context not created.");

  xmlSAXHandlerPtr old_sax = context_->sax;
  context_->sax = sax_handler_.get();

  initialize_context();
  
  xmlParseDocument(context_);

  context_->sax = old_sax;

  if( (! context_->wellFormed)
      && (! exception_) )
    exception_ = new parse_error("Document not well-formed");

  release_underlying();

  check_for_exception();
}

void SaxParser::parse_file(const std::string& filename)
{
  if(context_)
    throw parse_error("Attempt to start a second parse while a parse is in progress.");

  KeepBlanks k(KeepBlanks::Default);

  context_ = xmlCreateFileParserCtxt(filename.c_str());
  parse();
}

void SaxParser::parse_memory(const std::string& contents)
{
  if(context_)
    throw parse_error("Attempt to start a second parse while a parse is in progress.");

  KeepBlanks k(KeepBlanks::Default);

  context_ = xmlCreateMemoryParserCtxt(contents.c_str(), contents.length());
  parse();
}

void SaxParser::parse_stream(std::istream& in)
{
  if(context_)
    throw parse_error("Attempt to start a second parse while a parse is in progress.");

  KeepBlanks k(KeepBlanks::Default);

  context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      0, // user_data
      0,
      0,
      ""); // This should be the filename. I don't know if it is a problem to leave it empty.

  initialize_context();

  std::string line;
  while( ( ! exception_ )
      && std::getline(in, line))
  {
    // since getline does not get the line separator, we have to add it since the parser care
    // about layout in certain cases.
    line += '\n';

    xmlParseChunk(context_, line.c_str(), line.length(), 0 /* don't terminate */);
  }

  if( ! exception_ )
    xmlParseChunk(context_, 0 /* chunk */, 0 /* size */, 1 /* terminate (1 or 0) */); //This seems to be called just to terminate parsing.

  release_underlying();

  check_for_exception();
}

void SaxParser::parse_chunk(const std::string& chunk)
{
  KeepBlanks k(KeepBlanks::Default);

  if(!context_)
  {
    context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      0, // user_data
      0,
      0,
      ""); // This should be the filename. I don't know if it is a problem to let it empty

    initialize_context();
  }
  
  if(!exception_)
    xmlParseChunk(context_, chunk.c_str(), chunk.size(), 0 /* don't terminate */);

  check_for_exception();
}


void SaxParser::release_underlying()
{
  Parser::release_underlying();
}

void SaxParser::finish_chunk_parsing()
{
  if(!context_)
  {
    context_ = xmlCreatePushParserCtxt(
      sax_handler_.get(),
      0, // this, // user_data
      0,
      0,
      ""); // This should be the filename. I don't know if it is a problem to leave it empty
  }
  
  if(!exception_)
    xmlParseChunk(context_, 0 /* chunk */, 0 /* size */, 1 /* terminate (1 or 0) */); //This seems to be called just to terminate parsing.

  release_underlying();

  check_for_exception();
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
      ( name ? std::string((const char*)name) : std::string()),
      static_cast<XmlEntityType>(type),
      ( publicId ? std::string((const char*)publicId) : std::string()),
      ( systemId ? std::string((const char*)systemId) : std::string()),
      ( content ? std::string((const char*)content) : std::string()) );
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
    parser->on_start_element(std::string((const char*) name), attributes);
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
    parser->on_end_element(std::string((const char*) name));
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
    parser->on_characters(std::string((const char*) ch, len));
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
    parser->on_comment(std::string((const char*) value));
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
    parser->on_warning(std::string(buff));
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
    parser->on_error(std::string(buff));
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
    parser->on_fatal_error(std::string(buff));
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
    parser->on_cdata_block(std::string((const char*)value, len));
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
    std::string pid = publicId ? std::string((const char*) publicId) : "";
    std::string sid = systemId ? std::string((const char*) systemId) : "";

    parser->on_internal_subset( std::string((const char*) name), pid, sid);
  }
  catch(const exception& e)
  {
    parser->handleException(e);
  } 
}

} // namespace xmlpp


