/* domparser.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/parsers/domparser.h"
#include "libxml++/dtd.h"
#include "libxml++/nodes/element.h"
#include "libxml++/nodes/textnode.h"
#include "libxml++/nodes/commentnode.h"
#include "libxml++/keepblanks.h"
#include "libxml++/exceptions/internal_error.h"
#include <libxml/parserInternals.h>//For xmlCreateFileParserCtxt().

#include <sstream>
#include <iostream>

namespace xmlpp
{

DomParser::DomParser()
: doc_(nullptr)
{
  //Start with an empty document:
  doc_ = new Document();
}

DomParser::DomParser(const Glib::ustring& filename, bool validate)
: doc_(nullptr)
{
  set_validate(validate);
  parse_file(filename);
}

DomParser::~DomParser()
{ 
  release_underlying();
}

void DomParser::parse_file(const Glib::ustring& filename)
{
  release_underlying(); //Free any existing document.

  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  //The following is based on the implementation of xmlParseFile(), in xmlSAXParseFileWithData():
  context_ = xmlCreateFileParserCtxt(filename.c_str());

  if(!context_)
  {
    throw internal_error("Could not create parser context\n" + format_xml_error());
  }

  if(context_->directory == nullptr)
  {
    context_->directory = xmlParserGetDirectory(filename.c_str());
  }

  parse_context();
}

void DomParser::parse_memory_raw(const unsigned char* contents, size_type bytes_count)
{
  release_underlying(); //Free any existing document.

  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  //The following is based on the implementation of xmlParseFile(), in xmlSAXParseFileWithData():
  context_ = xmlCreateMemoryParserCtxt((const char*)contents, bytes_count);

  if(!context_)
  {
    throw internal_error("Could not create parser context\n" + format_xml_error());
  }

  parse_context();
}

void DomParser::parse_memory(const Glib::ustring& contents)
{
  parse_memory_raw((const unsigned char*)contents.c_str(), contents.bytes());
}

void DomParser::parse_context()
{
  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  //The following is based on the implementation of xmlParseFile(), in xmlSAXParseFileWithData():
  //and the implementation of xmlParseMemory(), in xmlSaxParseMemoryWithData().
  initialize_context();

  if(!context_)
  {
    throw internal_error("Parser context not initialized\n" + format_xml_error());
  }

  const int parseError = xmlParseDocument(context_);

  try
  {
    check_for_exception();
  }
  catch (...)
  {
    release_underlying(); //Free doc_ and context_
    throw; // re-throw exception
  }

  auto error_str = format_xml_parser_error(context_);
  if (error_str.empty() && parseError == -1)
    error_str = "xmlParseDocument() failed.";

  if(!error_str.empty())
  {
    release_underlying(); //Free doc_ and context_
    throw parse_error(error_str);
  }

  doc_ = new Document(context_->myDoc);
  // This is to indicate to release_underlying that we took the
  // ownership on the doc.
  context_->myDoc = nullptr;

  //Free the parse context, but keep the document alive so people can navigate the DOM tree:
  //TODO: Why not keep the context alive too?
  Parser::release_underlying();
}

void DomParser::parse_stream(std::istream& in)
{
  release_underlying(); //Free any existing document.

  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  context_ = xmlCreatePushParserCtxt(
      nullptr,  // Setting those two parameters to nullptr force the parser
      nullptr,  // to create a document while parsing.
      nullptr,  // chunk
      0,        // size
      nullptr); // no filename for fetching external entities

  if(!context_)
  {
    throw internal_error("Could not create parser context\n" + format_xml_error());
  }

  initialize_context();

  //TODO: Shouldn't we use a Glib::ustring here, and some alternative to std::getline()?
  int firstParseError = XML_ERR_OK;
  std::string line;
  while(std::getline(in, line))
  {
    // since getline does not get the line separator, we have to add it since the parser cares
    // about layout in certain cases.
    line += '\n';

    const int parseError = xmlParseChunk(context_, line.c_str(),
      line.size() /* This is a std::string, not a ustring, so this is the number of bytes. */, 0);

    // Save the first error code if any, but read on.
    // More errors might be reported and then thrown by check_for_exception().
    if (parseError != XML_ERR_OK && firstParseError == XML_ERR_OK)
      firstParseError = parseError;
  }

  const int parseError = xmlParseChunk(context_, nullptr, 0, 1 /* last chunk */);
  if (parseError != XML_ERR_OK && firstParseError == XML_ERR_OK)
    firstParseError = parseError;

  try
  {
    check_for_exception();
  }
  catch (...)
  {
    release_underlying(); //Free doc_ and context_
    throw; // re-throw exception
  }

  auto error_str = format_xml_parser_error(context_);
  if (error_str.empty() && firstParseError != XML_ERR_OK)
    error_str = "Error code from xmlParseChunk(): " + Glib::ustring::format(firstParseError);

  if(!error_str.empty())
  {
    release_underlying(); //Free doc_ and context_
    throw parse_error(error_str);
  }

  doc_ = new Document(context_->myDoc);
  // This is to indicate to release_underlying that we took the
  // ownership on the doc.
  context_->myDoc = nullptr;


  //Free the parse context, but keep the document alive so people can navigate the DOM tree:
  //TODO: Why not keep the context alive too?
  Parser::release_underlying();
}

void DomParser::release_underlying()
{
  if(doc_)
  {
    delete doc_;
    doc_ = nullptr;
  }

  Parser::release_underlying();
}

DomParser::operator bool() const
{
  return doc_ != nullptr;
}

Document* DomParser::get_document()
{
  return doc_;
}

const Document* DomParser::get_document() const
{
  return doc_;
}

} // namespace xmlpp
