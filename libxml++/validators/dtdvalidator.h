/* dtdvalidator.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson,
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_VALIDATORS_DTDVALIDATOR_H
#define __LIBXMLPP_VALIDATORS_DTDVALIDATOR_H

#include <libxml++/validators/validator.h>
#include <libxml++/dtd.h>
#include <libxml++/document.h>

namespace xmlpp {

/** XML DTD validator.
 * DTD = Document Type Definition
 */
class DtdValidator : public Validator
{
public:
  DtdValidator();

  /** Create a validator and parse an external subset (DTD file) immediately.
   * @param filename The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  explicit DtdValidator(const std::string& filename);

  /** Create a validator and parse an external subset (DTD file) immediately.
   * @param external The external ID of the DTD.
   * @param system The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  explicit DtdValidator(const Glib::ustring& external, const Glib::ustring& system);

  /** Create a validator.
   *
   * @newin{3,0}
   *
   * @param dtd A pointer to the DTD to use when validating XML documents.
   * @param take_ownership If <tt>true</tt>, the validator takes ownership of
   *        the DTD. The caller must not delete it.<br>
   *        If <tt>false</tt>, the validator does not take ownership of the DTD.
   *        The caller must guarantee that the DTD exists as long as the
   *        validator keeps a pointer to it. The caller is responsible for
   *        deleting the DTD when it's no longer needed.
   */
  explicit DtdValidator(Dtd* dtd, bool take_ownership);

  ~DtdValidator() override;

  /** Parse an external subset (DTD file).
   * If the validator already contains a DTD, that DTD is deleted.
   * @param filename The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  void parse_file(const std::string& filename) override;

  /** Parse an external subset (DTD file).
   * If the validator already contains a DTD, that DTD is deleted.
   * @param external The external ID of the DTD.
   * @param system The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  void parse_subset(const Glib::ustring& external, const Glib::ustring& system);

  /** Parse a DTD from a string.
   * If the validator already contains a DTD, that DTD is deleted.
   * @param contents The DTD as a string.
   * @throws xmlpp::parse_error
   */
  void parse_memory(const Glib::ustring& contents) override;

  /** Parse a DTD from a stream.
   * If the validator already contains a DTD, that DTD is deleted.
   * @param in The stream.
   * @throws xmlpp::parse_error
   */
  void parse_stream(std::istream& in);

  /** Set a DTD.
   * If the validator already contains a DTD, that DTD is released
   * (deleted if the validator owns the DTD).
   * @param dtd A pointer to the DTD to use when validating XML documents.
   * @param take_ownership If <tt>true</tt>, the validator takes ownership of
   *        the DTD. The caller must not delete it.<br>
   *        If <tt>false</tt>, the validator does not take ownership of the DTD.
   *        The caller must guarantee that the DTD exists as long as the
   *        validator keeps a pointer to it. The caller is responsible for
   *        deleting the DTD when it's no longer needed.
   */
  void set_dtd(Dtd* dtd, bool take_ownership);

  /** Test whether a DTD has been parsed.
   * For instance
   * @code
   * if (validator)
   *   do_something();
   * @endcode
   */
  explicit operator bool() const noexcept override;

  /** Get the parsed DTD.
   * @returns A pointer to the parsed DTD, or <tt>nullptr</tt>.
   */
  Dtd* get_dtd() noexcept;

  /** Get the parsed DTD.
   * @returns A pointer to the parsed DTD, or <tt>nullptr</tt>.
   */
  const Dtd* get_dtd() const noexcept;

  /** Validate a document, using a previously parsed DTD.
   * The internal subset (if present) is de-coupled (i.e. not used),
   * which could give problems if ID or IDREF is present.
   * @param document Pointer to the document.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  void validate(const Document* document) override;

protected:
  void initialize_context() override;
  void release_underlying() override;

private:
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATORS_DTDVALIDATOR_H
