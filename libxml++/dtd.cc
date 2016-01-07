/* dtd.cc
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include <libxml++/dtd.h>
#include <libxml++/exceptions/parse_error.h>
#include <libxml++/io/istreamparserinputbuffer.h>

#include <libxml/tree.h>

namespace xmlpp
{

struct Dtd::Impl
{
  Impl() noexcept : dtd(nullptr), is_dtd_owner(false) {}

  _xmlDtd* dtd;
  bool is_dtd_owner;
};

Dtd::Dtd()
: pimpl_(new Impl)
{
}

Dtd::Dtd(_xmlDtd* dtd, bool take_ownership)
: pimpl_(new Impl)
{
  pimpl_->dtd = dtd;
  if (dtd)
  {
    pimpl_->dtd->_private = this;
    pimpl_->is_dtd_owner = take_ownership;
  }
}

Dtd::Dtd(const std::string& filename)
: pimpl_(new Impl)
{
  parse_subset("", filename);
}

Dtd::Dtd(const Glib::ustring& external, const Glib::ustring& system)
: pimpl_(new Impl)
{
  parse_subset(external, system);
}

Dtd::~Dtd()
{
  release_underlying();
}

void Dtd::parse_file(const std::string& filename)
{
  parse_subset("", filename);
}

void Dtd::parse_subset(const Glib::ustring& external, const Glib::ustring& system)
{
  release_underlying(); // Free any existing dtd.
  xmlResetLastError();

  auto dtd = xmlParseDTD(
    external.empty() ? nullptr : (const xmlChar*)external.c_str(),
    system.empty() ? nullptr : (const xmlChar*)system.c_str());

  if (!dtd)
  {
    throw parse_error("Dtd could not be parsed.\n" + format_xml_error());
  }

  pimpl_->dtd = dtd;
  pimpl_->dtd->_private = this;
  pimpl_->is_dtd_owner = true;
}

void Dtd::parse_memory(const Glib::ustring& contents)
{
  // Prepare an istream with buffer
  std::istringstream is(contents);

  parse_stream(is);
}

void Dtd::parse_stream(std::istream& in)
{
  release_underlying(); // Free any existing dtd.
  xmlResetLastError();

  IStreamParserInputBuffer ibuff(in);

  auto dtd = xmlIOParseDTD(nullptr, ibuff.cobj(), XML_CHAR_ENCODING_UTF8);

  if (!dtd)
  {
    throw parse_error("Dtd could not be parsed.\n" + format_xml_error());
  }

  pimpl_->dtd = dtd;
  pimpl_->dtd->_private = this;
  pimpl_->is_dtd_owner = true;
}

Glib::ustring Dtd::get_name() const
{
  return (pimpl_->dtd && pimpl_->dtd->name) ? (const char*)pimpl_->dtd->name : "";
}

Glib::ustring Dtd::get_external_id() const
{
  return (pimpl_->dtd && pimpl_->dtd->ExternalID) ? (const char*)pimpl_->dtd->ExternalID : "";
}

Glib::ustring Dtd::get_system_id() const
{
  return (pimpl_->dtd && pimpl_->dtd->SystemID) ? (const char*)pimpl_->dtd->SystemID : "";
}

_xmlDtd* Dtd::cobj() noexcept
{
  return pimpl_->dtd;
}

const _xmlDtd* Dtd::cobj() const noexcept
{
  return pimpl_->dtd;
}

void Dtd::release_underlying()
{
  if (pimpl_->dtd)
  {
    pimpl_->dtd->_private = nullptr;
    if (pimpl_->is_dtd_owner)
    {
      xmlFreeDtd(pimpl_->dtd);
      pimpl_->is_dtd_owner = false;
    }
    pimpl_->dtd = nullptr;
  }
}

} //namespace xmlpp
