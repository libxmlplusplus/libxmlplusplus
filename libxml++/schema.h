/* schema.h
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson, and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#ifndef __LIBXMLPP_SCHEMA_H
#define __LIBXMLPP_SCHEMA_H

#include <libxml++/attribute.h>
#include <libxml++/document.h>
#include <list>
#include <map>

#ifndef LIBXMLXX_DISABLE_DEPRECATED

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern "C" {
  struct _xmlSchema;
}
#endif //DOXYGEN_SHOULD_SKIP_THIS

namespace xmlpp
{

/** Represents an XML Schema.
 *
 * @newin{2,24}
 *
 * @deprecated Use XsdSchema instead.
 */
class Schema : NonCopyable
{
public:
  /** Create a schema from the underlying libxml schema element.
   * @deprecated Use XsdSchema instead.
   */
  explicit Schema(_xmlSchema* schema);

  /** Create a schema from an XML document.
   * @param document XMLSchema document, <tt>nullptr</tt> to create an empty schema document.
   * @param embed If true, the document will be deleted when
   *   the schema is deleted or another document is set.
   * @throws xmlpp::parse_error
   * @throws xmlpp::internal_error If an empty schema document can't be created.
   * @deprecated Use XsdSchema instead.
   */
  explicit Schema(Document* document = nullptr, bool embed = false);
  ~Schema();

  //TODO: Remove virtual when we can break ABI?

  /** Set a new document to the schema.
   * If the old schema document is owned by the schema (embed == true), the old
   * schema document and all its nodes are deleted.
   * @param document XMLSchema document, <tt>nullptr</tt> to create an empty schema document.
   * @param embed If true, the document will be deleted when the schema is deleted or another document is set.
   * @throws xmlpp::parse_error
   * @throws xmlpp::internal_error If an empty schema document can't be created.
   * @deprecated Use XsdSchema::parse_document() instead.
   */
  virtual void set_document(Document* document = nullptr, bool embed = false);

  /** @deprecated There is no replacement.
   */
  Glib::ustring get_name() const;

  /** @deprecated There is no replacement.
   */
  Glib::ustring get_target_namespace() const;

  /** @deprecated There is no replacement.
   */
  Glib::ustring get_version() const;

  /** Get the schema document.
   * @returns A pointer to the schema document, or <tt>nullptr</tt> if none exists.
   * @deprecated There is no replacement.
   */
  Document* get_document();

  /** Get the schema document.
   * @returns A pointer to the schema document, or <tt>nullptr</tt> if none exists.
   * @deprecated There is no replacement.
   */
  const Document* get_document() const;
  
  /** Access the underlying libxml implementation.
   * @deprecated Use XsdSchema::cobj() instead.
   */
  _xmlSchema* cobj();

  /** Access the underlying libxml implementation.
   * @deprecated Use XsdSchema::cobj() instead.
   */
  const _xmlSchema* cobj() const;

protected:
  virtual void release_underlying();

private:
  _xmlSchema* impl_;

  /** If the base document is created with the schema. */
  bool embedded_doc_;
};

} // namespace xmlpp

#endif // LIBXMLXX_DISABLE_DEPRECATED
#endif //__LIBXMLPP_SCHEMA_H
