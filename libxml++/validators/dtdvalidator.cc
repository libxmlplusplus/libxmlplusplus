/* dtdvalidator.cpp
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "libxml++/validators/dtdvalidator.h"
#include "libxml++/dtd.h"
#include "libxml++/nodes/node.h"
#include "libxml++/exceptions/internal_error.h"
#include "libxml++/exceptions/validity_error.h"
#include "libxml++/io/istreamparserinputbuffer.h"
#include "libxml++/document.h"

#include <libxml/parser.h>

#include <sstream>
#include <iostream>

namespace xmlpp
{

DtdValidator::DtdValidator()
: dtd_(nullptr)
{
}

DtdValidator::DtdValidator(const Glib::ustring& file)
: dtd_(nullptr)
{
  parse_subset("",file);
}

DtdValidator::DtdValidator(const Glib::ustring& external,const Glib::ustring& system)
: dtd_(nullptr)
{
  parse_subset(external,system);
}

DtdValidator::~DtdValidator()
{
  release_underlying();
  Validator::release_underlying();
}

void DtdValidator::parse_file(const Glib::ustring& filename)
{
  parse_subset("",filename);
}

void DtdValidator::parse_subset(const Glib::ustring& external,const Glib::ustring& system)
{
  release_underlying(); // Free any existing dtd.
  xmlResetLastError();

  auto dtd = xmlParseDTD(
    external.empty() ? nullptr : (const xmlChar *)external.c_str(),
    system.empty() ? nullptr : (const xmlChar *)system.c_str());

  if (!dtd)
  {
    throw parse_error("Dtd could not be parsed.\n" + format_xml_error());
  }

  Node::create_wrapper(reinterpret_cast<xmlNode*>(dtd));
  dtd_ = static_cast<Dtd*>(dtd->_private);
}

void DtdValidator::parse_memory(const Glib::ustring& contents)
{
  // Prepare an istream with buffer
  std::istringstream is( contents );

  parse_stream( is );
}

void DtdValidator::parse_stream(std::istream& in)
{
  release_underlying(); // Free any existing dtd.
  xmlResetLastError();

  IStreamParserInputBuffer ibuff( in );

  auto dtd = xmlIOParseDTD(nullptr, ibuff.cobj(), XML_CHAR_ENCODING_UTF8);

  if (!dtd)
  {
    throw parse_error("Dtd could not be parsed.\n" + format_xml_error());
  }

  Node::create_wrapper(reinterpret_cast<xmlNode*>(dtd));
  dtd_ = static_cast<Dtd*>(dtd->_private);
}

void DtdValidator::release_underlying()
{
  if(dtd_)
  {
    //Make a local pointer to the underlying xmlDtd object as the wrapper is destroyed first.
    //After free_wrappers is called dtd_ will be invalid (e.g. delete dtd_)
    auto dtd = dtd_->cobj();
    Node::free_wrappers(reinterpret_cast<xmlNode*>(dtd));
    xmlFreeDtd(dtd);
    dtd_ = nullptr;
  }
}

DtdValidator::operator bool() const
{
  return dtd_ != nullptr;
}

Dtd* DtdValidator::get_dtd()
{
  return dtd_;
}

const Dtd* DtdValidator::get_dtd() const
{
  return dtd_;
}

bool DtdValidator::validate(const Document* doc)
{
  if (!doc)
  {
    throw internal_error("Document pointer cannot be nullptr.");
  }

  if (!dtd_)
  {
    throw internal_error("No DTD to use for validation.");
  }

  // A context is required at this stage only
  if (!valid_)
    valid_ = xmlNewValidCtxt();

  if(!valid_)
  {
    throw internal_error("Couldn't create validation context");
  }

  xmlResetLastError();
  initialize_valid();

  const bool res = (bool)xmlValidateDtd( valid_, (xmlDoc*)doc->cobj(), dtd_->cobj() );

  if (!res)
  {
    check_for_exception();
    throw validity_error("Document failed DTD validation\n" + format_xml_error());
  }

  return res;
}

} // namespace xmlpp
