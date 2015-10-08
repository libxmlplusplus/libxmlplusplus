#include <libxml++/parsers/textreader.h>
#include <libxml++/exceptions/internal_error.h>
#include <libxml++/exceptions/parse_error.h>
#include <libxml++/exceptions/validity_error.h>
#include <libxml++/document.h>

#include <libxml/xmlreader.h>

namespace xmlpp
{
  
class TextReader::PropertyReader
{
public:
  PropertyReader(TextReader& owner)
  : owner_(owner)
  {}

  int Int(int value);
  bool Bool(int value);
  char Char(int value);
  Glib::ustring String(xmlChar* value, bool free = false);
  Glib::ustring String(xmlChar const* value);

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
	const Glib::ustring& uri)
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
    const Glib::ustring& URI)
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

Glib::ustring TextReader::read_inner_xml()
{
  return propertyreader->String(
      xmlTextReaderReadInnerXml(impl_), true);
}

Glib::ustring TextReader::read_outer_xml()
{
  return propertyreader->String(
      xmlTextReaderReadOuterXml(impl_), true);
}

Glib::ustring TextReader::read_string()
{
  return propertyreader->String(
      xmlTextReaderReadString(impl_), true);
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

Glib::ustring TextReader::get_base_uri() const
{
  return propertyreader->String(
      xmlTextReaderBaseUri(impl_), true);
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

Glib::ustring TextReader::get_local_name() const
{
  return propertyreader->String(
      xmlTextReaderLocalName(impl_), true);
}

Glib::ustring TextReader::get_name() const
{
  return propertyreader->String(
      xmlTextReaderName(impl_), true);
}

Glib::ustring TextReader::get_namespace_uri() const
{
  return propertyreader->String(
      xmlTextReaderNamespaceUri(impl_), true);
}

TextReader::xmlNodeType TextReader::get_node_type() const
{
  int result = xmlTextReaderNodeType(impl_);
  if(result == -1)
    check_for_exceptions();
  return (xmlNodeType)result;
}

Glib::ustring TextReader::get_prefix() const
{
  return propertyreader->String(
      xmlTextReaderPrefix(impl_), true);
}

char TextReader::get_quote_char() const
{
  return propertyreader->Char(
      xmlTextReaderQuoteChar(impl_));
}

Glib::ustring TextReader::get_value() const
{
  return propertyreader->String(
      xmlTextReaderValue(impl_), true);
}

Glib::ustring TextReader::get_xml_lang() const
{
  return propertyreader->String(
      xmlTextReaderXmlLang(impl_));
}

TextReader::xmlReadState TextReader::get_read_state() const
{
  int result = xmlTextReaderReadState(impl_);
  if(result == -1)
    check_for_exceptions();
  return (xmlReadState)result;
}

void TextReader::close()
{
  if(xmlTextReaderClose(impl_) == -1)
    check_for_exceptions();
}

Glib::ustring TextReader::get_attribute(int number) const
{
  return propertyreader->String(
      xmlTextReaderGetAttributeNo(impl_, number), true);
}

Glib::ustring TextReader::get_attribute(
    const Glib::ustring& name) const
{
  return propertyreader->String(
      xmlTextReaderGetAttribute(impl_, (const xmlChar *)name.c_str()), true);
}

Glib::ustring TextReader::get_attribute(
    const Glib::ustring& localName,
    const Glib::ustring& namespaceURI) const
{
  return propertyreader->String(
      xmlTextReaderGetAttributeNs(impl_, (const xmlChar *)localName.c_str(), (const xmlChar *)namespaceURI.c_str()), true);
}

Glib::ustring TextReader::lookup_namespace(
    const Glib::ustring& prefix) const
{
  return propertyreader->String(
      xmlTextReaderLookupNamespace(impl_, (const xmlChar *)prefix.c_str()), true);
}

bool TextReader::move_to_attribute(int number)
{
  return propertyreader->Bool(
      xmlTextReaderMoveToAttributeNo(impl_, number));
}

bool TextReader::move_to_attribute(
    const Glib::ustring& name)
{
  return propertyreader->Bool(
      xmlTextReaderMoveToAttribute(impl_, (const xmlChar *)name.c_str()));
}

bool TextReader::move_to_attribute(
    const Glib::ustring& localName,
    const Glib::ustring& namespaceURI)
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
  xmlTextReaderErrorFunc func = nullptr;
  void* arg = nullptr; 

  // We respect any other error handlers already setup:
  xmlTextReaderGetErrorHandler(impl_, &func, &arg);
  if(!func)
  {
     func = (xmlTextReaderErrorFunc)&TextReader::on_libxml_error;
     xmlTextReaderSetErrorHandler(impl_, func, this);
  }
}

void TextReader::on_libxml_error(void* arg, const char* msg, int severity, void* /* locator */)
{
  auto ths = (TextReader*)arg;
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

Glib::ustring TextReader::PropertyReader::String(xmlChar* value, bool free)
{
  owner_.check_for_exceptions();
  
  if (!value)
    return Glib::ustring();
    
  const Glib::ustring result = (char *)value;

  if(free)
    xmlFree(value);

  return result;
}

Glib::ustring TextReader::PropertyReader::String(xmlChar const* value)
{
  owner_.check_for_exceptions();

  if (!value)
    return Glib::ustring();

  return (const char*)value;
}

} // namespace xmlpp
