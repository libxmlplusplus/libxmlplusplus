/* xml++.h
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
 */
class DtdValidator : public Validator
{
public:
  DtdValidator();

  /** Create a validator and parse an external subset (DTD file) immediately.
   * @param file The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  explicit DtdValidator(const Glib::ustring& file);

  /** Create a validator and parse an external subset (DTD file) immediately.
   * @param external The external ID of the DTD.
   * @param system The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  explicit DtdValidator(const Glib::ustring& external,const Glib::ustring& system);

  ~DtdValidator() override;

  //TODO: Remove virtuals when we can break ABI,
  //or really put these in the base class.

  /** Parse an external subset (DTD file).
   * If the validator already contains a DTD, that DTD is deleted.
   * @param external The external ID of the DTD.
   * @param system The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  virtual void parse_subset(const Glib::ustring& external,const Glib::ustring& system);

  /** Parse an external subset (DTD file).
   * If the validator already contains a DTD, that DTD is deleted.
   * @param filename The URL of the DTD.
   * @throws xmlpp::parse_error
   */
  virtual void parse_file(const Glib::ustring& filename);

  /** Parse a DTD from a string.
   * If the validator already contains a DTD, that DTD is deleted.
   * @param contents The DTD as a string.
   * @throws xmlpp::parse_error
   */
  virtual void parse_memory(const Glib::ustring& contents);

  /** Parse a DTD from a stream.
   * If the validator already contains a DTD, that DTD is deleted.
   * @param in The stream.
   * @throws xmlpp::parse_error
   */
  virtual void parse_stream(std::istream& in);

  /** Test whether a DTD has been parsed.
   */
  operator bool() const;

  /** Get the parsed DTD.
   * @returns A pointer to the parsed DTD, or <tt>nullptr</tt>.
   */
  Dtd* get_dtd();

  /** Get the parsed DTD.
   * @returns A pointer to the parsed DTD, or <tt>nullptr</tt>.
   */
  const Dtd* get_dtd() const;

  /** Validate a document, using a previously parsed DTD.
   * The internal subset (if present) is de-coupled (i.e. not used),
   * which could give problems if ID or IDREF is present.
   * @param doc Pointer to the document.
   * @returns Whether the document is valid.
   * @throws xmlpp::internal_error
   * @throws xmlpp::validity_error
   */
  bool validate(const Document* doc);

protected:
  void release_underlying() override;

  Dtd* dtd_;
};

} // namespace xmlpp

#endif //__LIBXMLPP_VALIDATORS_DTDVALIDATOR_H

