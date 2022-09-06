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
#include <libxml/xinclude.h>

#include <sstream>
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

namespace xmlpp
{

DomParser::DomParser()
: doc_(nullptr)
{
  //Start with an empty document:
  doc_ = new Document();
}

DomParser::DomParser(const std::string& filename, bool validate)
: doc_(nullptr)
{
  set_validate(validate);
  parse_file(filename);
}

DomParser::~DomParser()
{
  release_underlying();
}

void DomParser::set_xinclude_options(bool process_xinclude,
  bool generate_xinclude_nodes, bool fixup_base_uris) noexcept
{
  xinclude_options_ = 0;
  if (process_xinclude)
    xinclude_options_ |= XML_PARSE_XINCLUDE;
  if (!generate_xinclude_nodes)
    xinclude_options_ |= XML_PARSE_NOXINCNODE;
  if (!fixup_base_uris)
    xinclude_options_ |= XML_PARSE_NOBASEFIX;
}

void DomParser::get_xinclude_options(bool& process_xinclude,
  bool& generate_xinclude_nodes, bool& fixup_base_uris) const noexcept
{
  process_xinclude = (xinclude_options_ & XML_PARSE_XINCLUDE) != 0;
  generate_xinclude_nodes = (xinclude_options_ & XML_PARSE_NOXINCNODE) == 0;
  fixup_base_uris = (xinclude_options_ & XML_PARSE_NOBASEFIX) == 0;
}

void DomParser::parse_file(const std::string& filename)
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

void DomParser::parse_memory(const ustring& contents)
{
  parse_memory_raw((const unsigned char*)contents.c_str(), contents.size());
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

  check_xinclude_and_finish_parsing();
}

void DomParser::check_xinclude_and_finish_parsing()
{
  int set_options = 0;
  int clear_options = 0;
  get_parser_options(set_options, clear_options);

  int options = xinclude_options_;
  // Turn on/off any xinclude options.
  options |= set_options;
  options &= ~clear_options;

  if (options & XML_PARSE_XINCLUDE)
  {
    const int n_substitutions = xmlXIncludeProcessFlags(context_->myDoc, options);
    if (n_substitutions < 0)
    {
      throw parse_error("Couldn't process XInclude\n" + format_xml_error());
    }
  }

  doc_ = new Document(context_->myDoc);
  // This is to indicate to release_underlying() that we took the
  // ownership on the doc.
  context_->myDoc = nullptr;

  // Free the parser context because it's not needed anymore,
  // but keep the document alive so people can navigate the DOM tree:
  Parser::release_underlying();
}

void DomParser::parse_stream(std::istream& in)
{
  release_underlying(); //Free any existing document.

  KeepBlanks k(KeepBlanks::Default);
  xmlResetLastError();

  context_ = xmlCreateIOParserCtxt(
      nullptr, // Setting those two parameters to nullptr force the parser
      nullptr, // to create a document while parsing.
      _io_read_callback,
      nullptr, // inputCloseCallback
      &in,
      XML_CHAR_ENCODING_NONE);

  if(!context_)
  {
    throw internal_error("Could not create parser context\n" + format_xml_error());
  }

  parse_context();
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

DomParser::operator bool() const noexcept
{
  return doc_ != nullptr;
}

Document* DomParser::get_document() noexcept
{
  return doc_;
}

const Document* DomParser::get_document() const noexcept
{
  return doc_;
}

} // namespace xmlpp
