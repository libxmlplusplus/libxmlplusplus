#include <iostream>
#include <stdexcept>
#include <glibmm/ustring.h>
#include <cstdlib>
#include <libxml++/libxml++.h>

using namespace xmlpp;
using namespace std;

int main (int /* argc */, char** /* argv */)
{
  // Set the global C and C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  try
  { 
    DomParser example1("example1.xml");
    DomParser example2("example2.xml");
    
    Document* doc1 = example1.get_document();
    Document* doc2 = example2.get_document();
    
    Element* root1 = doc1->get_root_node();
    Element* root2 = doc2->get_root_node();

    // find the first "child" element in example2
    Node::NodeList child_list2 = root2->get_children("child");
    Node* node_to_add = child_list2.front();

    // import the node under the root element (recursive is default)
    root1->import_node(node_to_add);

    // Import an attribute that will replace an existing attribute in the root element.
    Attribute* attribute_to_add = root2->get_attribute("name");
    root1->import_node(attribute_to_add);
    
    // Import an attribute that will be added to the root element.
    attribute_to_add = root2->get_attribute("type");
    root1->import_node(attribute_to_add);
    
    // Find the first text child of the first "child" element in example2.
    Element* first_child2 = dynamic_cast<Element*>(child_list2.front());
    if (!first_child2)
    {
      cerr << "first_child2 == 0" << endl;
      return EXIT_FAILURE;
    }
    TextNode* text_to_add = first_child2->get_child_text();

    // Import the text under the first "child" element in example1.
    // Adjacent text nodes are merged.
    Node* first_child1 = root1->get_first_child("child");
    first_child1->import_node(text_to_add);

    // print out the new doc1
    string doc1_string = doc1->write_to_string_formatted();
    cout << doc1_string;
  }
  catch (std::exception& ex)
  {
    cerr << "Caught exception " << ex.what() << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
