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

struct DtdValidator::Impl
{
  Impl() noexcept : dtd(nullptr), is_dtd_owner(false), context(nullptr) {}

  Dtd* dtd;
  bool is_dtd_owner;
  _xmlValidCtxt* context;
};


DtdValidator::DtdValidator()
: pimpl_(new Impl)
{
}

DtdValidator::DtdValidator(const std::string& filename)
: pimpl_(new Impl)
{
  parse_file(filename);
}

DtdValidator::DtdValidator(const Glib::ustring& external, const Glib::ustring& system)
: pimpl_(new Impl)
{
  parse_subset(external, system);
}

DtdValidator::DtdValidator(Dtd* dtd, bool take_ownership)
: pimpl_(new Impl)
{
  set_dtd(dtd, take_ownership);
}

DtdValidator::~DtdValidator()
{
  release_underlying();
}

void DtdValidator::parse_file(const std::string& filename)
{
  set_dtd(new Dtd(filename), true);
}

void DtdValidator::parse_subset(const Glib::ustring& external, const Glib::ustring& system)
{
  set_dtd(new Dtd(external, system), true);
}

void DtdValidator::parse_memory(const Glib::ustring& contents)
{
  std::unique_ptr<Dtd> dtd(new Dtd());
  dtd->parse_memory(contents);
  set_dtd(dtd.release(), true);
}

void DtdValidator::parse_stream(std::istream& in)
{
  std::unique_ptr<Dtd> dtd(new Dtd());
  dtd->parse_stream(in);
  set_dtd(dtd.release(), true);
}

void DtdValidator::set_dtd(Dtd* dtd, bool take_ownership)
{
  release_underlying();
  pimpl_->dtd = dtd;
  pimpl_->is_dtd_owner = take_ownership;
}

void DtdValidator::initialize_context()
{
  Validator::initialize_context();

  if (pimpl_->context)
  {
    //Tell the validation context about the callbacks:
    pimpl_->context->error = &callback_validity_error;
    pimpl_->context->warning = &callback_validity_warning;

    //Allow the callback_validity_*() methods to retrieve the C++ instance:
    pimpl_->context->userData = this;
  }
}

void DtdValidator::release_underlying()
{
  if (pimpl_->context)
  {
    pimpl_->context->userData = nullptr; //Not really necessary.

    xmlFreeValidCtxt(pimpl_->context);
    pimpl_->context = nullptr;
  }

  if (pimpl_->dtd)
  {
    if (pimpl_->is_dtd_owner)
      delete pimpl_->dtd;
    pimpl_->dtd = nullptr;
  }

  Validator::release_underlying();
}

DtdValidator::operator bool() const noexcept
{
  return pimpl_->dtd && pimpl_->dtd->cobj();
}

Dtd* DtdValidator::get_dtd() noexcept
{
  return pimpl_->dtd;
}

const Dtd* DtdValidator::get_dtd() const noexcept
{
  return pimpl_->dtd;
}

void DtdValidator::validate(const Document* document)
{
  if (!document)
  {
    throw internal_error("Document pointer cannot be nullptr.");
  }

  if (!pimpl_->dtd)
  {
    throw internal_error("No DTD to use for validation.");
  }

  // A context is required at this stage only
  if (!pimpl_->context)
    pimpl_->context = xmlNewValidCtxt();

  if (!pimpl_->context)
  {
    throw internal_error("Couldn't create validation context");
  }

  xmlResetLastError();
  initialize_context();

  const bool res = (bool)xmlValidateDtd(pimpl_->context, (xmlDoc*)document->cobj(),
                   pimpl_->dtd->cobj());

  if (!res)
  {
    check_for_exception();
    throw validity_error("Document failed DTD validation\n" + format_xml_error());
  }
}

} // namespace xmlpp
