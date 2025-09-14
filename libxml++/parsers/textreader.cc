#include <libxml++/parsers/textreader.h>
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/exceptions/parse_error.h>
#include <libxml++/exceptions/validity_error.h>
#include <libxml++/document.h>

#include <libxml/xmlreader.h>
#include <libxml/xmlversion.h>

namespace
{
//TODO: When we can break ABI, change on_libxml_error(), and change ErrorFuncType to
// using ErrorFuncType = void (*)(void* userData, const xmlError* error);
// C++ linkage
using ErrorFuncType = void (*)(void* arg, const char* msg, int severity, void* locator);
ErrorFuncType p_callback_error;

extern "C"
{
#if LIBXML_VERSION >= 21200
static void c_callback_error(void* userData, const xmlError* error)
#else
static void c_callback_error(void* userData, xmlError* error)
#endif
{
  const xmlpp::ustring msg = xmlpp::format_xml_error(error);

  // Compute severity as in libxml2's xmlreader.c file,
  // static (i.e. private) function xmlTextReaderStructuredRelay().
  xmlParserSeverities severity{};
  switch (error->domain)
  {
  case XML_FROM_VALID:
  case XML_FROM_DTD:
    severity = (error->level == XML_ERR_WARNING) ?
      XML_PARSER_SEVERITY_VALIDITY_WARNING :
      XML_PARSER_SEVERITY_VALIDITY_ERROR;
    break;
  default:
    severity = (error->level == XML_ERR_WARNING) ?
      XML_PARSER_SEVERITY_WARNING :
      XML_PARSER_SEVERITY_ERROR;
    break;
  }
  p_callback_error(userData, msg.c_str(), severity, nullptr);
}

} // extern "C"
} // anonymous namespace

namespace xmlpp
{

class TextReader::PropertyReader
{
public:
  explicit PropertyReader(TextReader& owner)
  : owner_(owner)
  {}

  int Int(int value);
  bool Bool(int value);
  char Char(int value);
  ustring String(xmlChar* value, bool should_free = false);
  ustring String(xmlChar const* value);
  std::optional<ustring> OptString(xmlChar* value);

  TextReader & owner_;
};

TextReader::TextReader(
  struct _xmlTextReader* cobj)
  : propertyreader(new PropertyReader(*this)), impl_( cobj ),
    severity_( 0 )
{
  setup_exceptions();
}

TextReader::TextReader(
	const unsigned char* data,
	size_type size,
	const ustring& uri)
	: propertyreader(new PropertyReader(*this)),
	  impl_( xmlReaderForMemory ((const char*)data, size, uri.c_str(), nullptr, 0) ),
    severity_( 0 )
{
  if( ! impl_ )
  {
    throw internal_error("Cannot instantiate underlying libxml2 structure");
  }

  setup_exceptions();
}

TextReader::TextReader(
    const ustring& URI)
  : propertyreader(new PropertyReader(*this)), impl_( xmlNewTextReaderFilename(URI.c_str()) ),
    severity_( 0 )
{
  if( ! impl_ )
  {
    throw internal_error("Cannot instantiate underlying libxml2 structure");
  }

  setup_exceptions();
}

TextReader::~TextReader()
{
  xmlFreeTextReader(impl_);
}

bool TextReader::read()
{
  return propertyreader->Bool(
      xmlTextReaderRead(impl_));
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring TextReader::read_inner_xml()
{
  return propertyreader->String(
      xmlTextReaderReadInnerXml(impl_), true);
}

ustring TextReader::read_outer_xml()
{
  return propertyreader->String(
      xmlTextReaderReadOuterXml(impl_), true);
}

ustring TextReader::read_string()
{
  return propertyreader->String(
      xmlTextReaderReadString(impl_), true);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> TextReader::read_inner_xml2()
{
  return propertyreader->OptString(xmlTextReaderReadInnerXml(impl_));
}

std::optional<ustring> TextReader::read_outer_xml2()
{
  return propertyreader->OptString(xmlTextReaderReadOuterXml(impl_));
}

std::optional<ustring> TextReader::read_string2()
{
  return propertyreader->OptString(xmlTextReaderReadString(impl_));
}

bool TextReader::read_attribute_value()
{
  return propertyreader->Bool(
      xmlTextReaderReadAttributeValue(impl_));
}

int TextReader::get_attribute_count() const
{
  return propertyreader->Int(
      xmlTextReaderAttributeCount(impl_));
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring TextReader::get_base_uri() const
{
  return propertyreader->String(
      xmlTextReaderBaseUri(impl_), true);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> TextReader::get_base_uri2() const
{
  return propertyreader->OptString(xmlTextReaderBaseUri(impl_));
}

int TextReader::get_depth() const
{
  return propertyreader->Int(
      xmlTextReaderDepth(impl_));
}

bool TextReader::has_attributes() const
{
  return propertyreader->Bool(
      xmlTextReaderHasAttributes(impl_));
}

bool TextReader::has_value() const
{
  return propertyreader->Bool(
      xmlTextReaderHasValue(impl_));
}

bool TextReader::is_default() const
{
  return propertyreader->Bool(
      xmlTextReaderIsDefault(impl_));
}

bool TextReader::is_empty_element() const
{
  return propertyreader->Bool(
      xmlTextReaderIsEmptyElement(impl_));
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring TextReader::get_local_name() const
{
  return propertyreader->String(
      xmlTextReaderLocalName(impl_), true);
}

ustring TextReader::get_name() const
{
  return propertyreader->String(
      xmlTextReaderName(impl_), true);
}

ustring TextReader::get_namespace_uri() const
{
  return propertyreader->String(
      xmlTextReaderNamespaceUri(impl_), true);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> TextReader::get_local_name2() const
{
  return propertyreader->OptString(xmlTextReaderLocalName(impl_));
}

std::optional<ustring> TextReader::get_name2() const
{
  return propertyreader->OptString(xmlTextReaderName(impl_));
}

std::optional<ustring> TextReader::get_namespace_uri2() const
{
  return propertyreader->OptString(xmlTextReaderNamespaceUri(impl_));
}

TextReader::NodeType TextReader::get_node_type() const
{
  int result = xmlTextReaderNodeType(impl_);
  if(result == -1)
    check_for_exceptions();
  return static_cast<NodeType>(result);
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring TextReader::get_prefix() const
{
  return propertyreader->String(
      xmlTextReaderPrefix(impl_), true);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> TextReader::get_prefix2() const
{
  return propertyreader->OptString(xmlTextReaderPrefix(impl_));
}

char TextReader::get_quote_char() const
{
  return propertyreader->Char(
      xmlTextReaderQuoteChar(impl_));
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring TextReader::get_value() const
{
  return propertyreader->String(
      xmlTextReaderValue(impl_), true);
}

ustring TextReader::get_xml_lang() const
{
  return propertyreader->String(
      xmlTextReaderXmlLang(impl_), true);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> TextReader::get_value2() const
{
  return propertyreader->OptString(xmlTextReaderValue(impl_));
}

std::optional<ustring> TextReader::get_xml_lang2() const
{
  return propertyreader->OptString(xmlTextReaderXmlLang(impl_));
}

TextReader::ReadState TextReader::get_read_state() const
{
  int result = xmlTextReaderReadState(impl_);
  if(result == -1)
    check_for_exceptions();
  return static_cast<ReadState>(result);
}

void TextReader::close()
{
  if(xmlTextReaderClose(impl_) == -1)
    check_for_exceptions();
}

#ifndef LIBXMLXX_DISABLE_DEPRECATED
ustring TextReader::get_attribute(int number) const
{
  return propertyreader->String(
      xmlTextReaderGetAttributeNo(impl_, number), true);
}

ustring TextReader::get_attribute(
    const ustring& name) const
{
  return propertyreader->String(
      xmlTextReaderGetAttribute(impl_, (const xmlChar *)name.c_str()), true);
}

ustring TextReader::get_attribute(
    const ustring& localName,
    const ustring& namespaceURI) const
{
  return propertyreader->String(
      xmlTextReaderGetAttributeNs(impl_, (const xmlChar *)localName.c_str(),
      (const xmlChar *)namespaceURI.c_str()), true);
}

ustring TextReader::lookup_namespace(
    const ustring& prefix) const
{
  return propertyreader->String(
      xmlTextReaderLookupNamespace(impl_, (const xmlChar *)prefix.c_str()), true);
}
#endif // LIBXMLXX_DISABLE_DEPRECATED

std::optional<ustring> TextReader::get_attribute2(int number) const
{
  return propertyreader->OptString(xmlTextReaderGetAttributeNo(impl_, number));
}

std::optional<ustring> TextReader::get_attribute2(const ustring& name) const
{
  return propertyreader->OptString(
    xmlTextReaderGetAttribute(impl_, (const xmlChar*)name.c_str()));
}

std::optional<ustring> TextReader::get_attribute2(
    const ustring& localName,
    const ustring& namespaceURI) const
{
  return propertyreader->OptString(
      xmlTextReaderGetAttributeNs(impl_, (const xmlChar*)localName.c_str(),
      (const xmlChar *)namespaceURI.c_str()));
}

std::optional<ustring> TextReader::lookup_namespace2(const ustring& prefix) const
{
  return propertyreader->OptString(xmlTextReaderLookupNamespace(
    impl_, prefix.empty() ? nullptr : (const xmlChar*)prefix.c_str()));
}

bool TextReader::move_to_attribute(int number)
{
  return propertyreader->Bool(
      xmlTextReaderMoveToAttributeNo(impl_, number));
}

bool TextReader::move_to_attribute(
    const ustring& name)
{
  return propertyreader->Bool(
      xmlTextReaderMoveToAttribute(impl_, (const xmlChar *)name.c_str()));
}

bool TextReader::move_to_attribute(
    const ustring& localName,
    const ustring& namespaceURI)
{
  return propertyreader->Bool(
      xmlTextReaderMoveToAttributeNs(impl_, (const xmlChar *)localName.c_str(), (const xmlChar *)namespaceURI.c_str()));
}

bool TextReader::move_to_first_attribute()
{
  return propertyreader->Bool(
      xmlTextReaderMoveToFirstAttribute(impl_));
}

bool TextReader::move_to_next_attribute()
{
  return propertyreader->Bool(
      xmlTextReaderMoveToNextAttribute(impl_));
}

bool TextReader::move_to_element()
{
  return propertyreader->Bool(
      xmlTextReaderMoveToElement(impl_));
}

bool TextReader::get_normalization() const
{
  return propertyreader->Bool(
      xmlTextReaderNormalization(impl_));
}

bool TextReader::get_parser_property(
    ParserProperties property) const
{
  return propertyreader->Bool(
      xmlTextReaderGetParserProp(impl_, (int)property));
}

void TextReader::set_parser_property(
    ParserProperties property,
    bool value)
{
  if(xmlTextReaderSetParserProp(impl_, (int)property, value?1:0))
    check_for_exceptions();
}

Node* TextReader::get_current_node()
{
  auto node = xmlTextReaderCurrentNode(impl_);
  if(node)
  {
    Node::create_wrapper(node);
    return static_cast<Node*>(node->_private);
  }

  check_for_exceptions();
  return nullptr;
}

const Node* TextReader::get_current_node() const
{
  return const_cast<TextReader*>(this)->get_current_node();
}

/*
TODO: add a private constructor to Document.
Document* TextReader::CurrentDocument()
{
  auto doc = xmlTextReaderCurrentDoc(impl_);
  if(doc)
    return new Document(doc);
}
*/

Node* TextReader::expand()
{
  auto node = xmlTextReaderExpand(impl_);
  if(node)
  {
    Node::create_wrapper(node);
    return static_cast<Node*>(node->_private);
  }

  check_for_exceptions();
  return nullptr;
}

bool TextReader::next()
{
  return propertyreader->Bool(
      xmlTextReaderNext(impl_));
}

bool TextReader::is_valid() const
{
  return propertyreader->Bool(
      xmlTextReaderIsValid(impl_));
}

void TextReader::setup_exceptions()
{
  p_callback_error = &on_libxml_error;
  xmlTextReaderSetStructuredErrorHandler(impl_, &c_callback_error, this);
}

void TextReader::on_libxml_error(void* arg, const char* msg, int severity, void* /* locator */)
{
  //TODO: Change this function when we can break ABI.
  // It was created when setup_exceptions() called xmlTextReaderSetErrorHandler()
  // instead of xmlTextReaderSetStructuredErrorHandler().

  auto ths = static_cast<TextReader*>(arg);
  ths->severity_ = severity;
  ths->error_ = msg ? msg : "unknown parse error";
}

void TextReader::check_for_exceptions() const
{
  if( severity_ == 0 )
    return;

  auto ths = const_cast<TextReader*>(this);

  int severity = severity_;
  ths->severity_ = 0;

  if( severity == XML_PARSER_SEVERITY_ERROR )
    throw parse_error(error_);
  else if( severity == XML_PARSER_SEVERITY_VALIDITY_ERROR )
    throw validity_error(error_);
}

int TextReader::PropertyReader::Int(int value)
{
  if(value == -1)
    owner_.check_for_exceptions();

  return value;
}

bool TextReader::PropertyReader::Bool(int value)
{
  if(value == -1)
    owner_.check_for_exceptions();

  return value > 0;
}

char TextReader::PropertyReader::Char(int value)
{
  owner_.check_for_exceptions();
  return value;
}

ustring TextReader::PropertyReader::String(xmlChar* value, bool should_free)
{
  owner_.check_for_exceptions();

  if (!value)
    return ustring();

  ustring result = (char *)value;

  if(should_free)
    xmlFree(value);

  return result;
}

ustring TextReader::PropertyReader::String(xmlChar const* value)
{
  owner_.check_for_exceptions();

  if (!value)
    return ustring();

  return (const char*)value;
}

std::optional<ustring> TextReader::PropertyReader::OptString(xmlChar* value)
{
  owner_.check_for_exceptions();

  if (!value)
    return {};

  std::optional<ustring> result = (char*)value;
  xmlFree(value);
  return result;
}

} // namespace xmlpp
