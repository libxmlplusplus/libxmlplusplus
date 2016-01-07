/* dtd.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_DTD_H
#define __LIBXMLPP_DTD_H

#include <libxml++/noncopyable.h>
#include <glibmm/ustring.h>
#include <string>
#include <memory> // std::unique_ptr

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlDtd;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

//TODO: Derive from Node?
// See https://bugzilla.gnome.org/show_bug.cgi?id=754673#c8 for an explanation
// why it has not been done in libxml++ 3.0.
/** Represents an XML DTD for validating XML files.
 * DTD = Document Type Definition
 */
class Dtd : public NonCopyable
{
public:
  Dtd();

  /** Create a Dtd from the underlying libxml DTD element.
   * @param dtd A pointer to the libxml DTD element.
   * @param take_ownership If <tt>true</tt>, this Dtd instance takes ownership of
   *        the libxml DTD element. The caller must not delete it.<br>
   *        If <tt>false</tt>, this Dtd does not take ownership of the libxml
   *        DTD element. The caller must guarantee that the libxml DTD element
   *        exists as long as this Dtd keeps a pointer to it. The caller is
   *        responsible for deleting the libxml DTD element when it's no longer
   *        needed, unless it belongs to a Document, in which case it's deleted
   *        when the Document is deleted.
   */
  explicit Dtd(_xmlDtd* dtd, bool take_ownership = false);

  /** Create a Dtd and parse an external subset (DTD file) immediately.
   *
   * @newin{3,0}
   *
   * @param filename The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  explicit Dtd(const std::string& filename);

  /** Create a Dtd and parse an external subset (DTD file) immediately.
   *
   * @newin{3,0}
   *
   * @param external The external ID of the DTD.
   * @param system The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  Dtd(const Glib::ustring& external, const Glib::ustring& system);

  ~Dtd() override;

  /** Parse an external subset (DTD file).
   * If another DTD has been parsed before, that DTD is replaced by the new one
   * (deleted if this Dtd owns it).
   *
   * @newin{3,0}
   *
   * @param filename The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  void parse_file(const std::string& filename);

  /** Parse an external subset (DTD file).
   * If another DTD has been parsed before, that DTD is replaced by the new one
   * (deleted if this Dtd owns it).
   *
   * @newin{3,0}
   *
   * @param external The external ID of the DTD.
   * @param system The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  void parse_subset(const Glib::ustring& external, const Glib::ustring& system);

  /** Parse a DTD from a string.
   * If another DTD has been parsed before, that DTD is replaced by the new one
   * (deleted if this Dtd owns it).
   *
   * @newin{3,0}
   *
   * @param contents The DTD as a string.
   * @throws xmlpp::parse_error
   */
  void parse_memory(const Glib::ustring& contents);

  /** Parse a DTD from a stream.
   * If another DTD has been parsed before, that DTD is replaced by the new one
   * (deleted if this Dtd owns it).
   *
   * @newin{3,0}
   *
   * @param in The stream.
   * @throws xmlpp::parse_error
   */
  void parse_stream(std::istream& in);

  Glib::ustring get_name() const;
  Glib::ustring get_external_id() const;
  Glib::ustring get_system_id() const;

  /** Access the underlying libxml implementation.
   */
  _xmlDtd* cobj() noexcept;

  /** Access the underlying libxml implementation.
   */
  const _xmlDtd* cobj() const noexcept;

protected:
  void release_underlying();

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_DTD_H
