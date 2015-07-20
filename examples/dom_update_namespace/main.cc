/* Copyright (C) 2014 The libxml++ development team
 *
 * This file is part of libxml++.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libxml++/libxml++.h>
#include <iostream>
#include <cstdlib>
#include <exception>

class Tests
{
  public:
    virtual ~Tests() {}

  protected:
    template<typename RefType, typename ValueType>
    void assert_equal(const RefType& reference, const ValueType& value, const std::string& msg);
    template<typename RefType, typename ValueType>
    void assert_not_equal(const RefType& reference, const ValueType& value, const std::string& msg);

    template <typename RefType, typename ValueType>
    void fail(const RefType& reference, const ValueType& value, const std::string& msg);
};

class fail_exception : public std::exception
{
  public:
    fail_exception(const std::string& msg) : msg_(msg) {}
    ~fail_exception() noexcept override {}
    const char* what() const noexcept override { return msg_.c_str(); }

  private:
    std::string msg_;
};


////////////////////////////////////////////////////////////////////////////////

class TestNamespace : public Tests
{
  public:
    TestNamespace();
    void test_create_new_node_with_default_namespace();
    void test_create_new_node_using_existing_namespace_prefix();

  private:
    void setup(const std::string& filename);

    xmlpp::Node::PrefixNsMap nsmap_;
    xmlpp::DomParser parser_;
    xmlpp::Element* root_;
};


TestNamespace::TestNamespace()
:
root_(nullptr)
{
  nsmap_["ns0"] = "http://default.namespace/root";
  nsmap_["ns1"] = "http://default.namespace/child";
}

void TestNamespace::setup(const std::string& filename)
{
  parser_.set_throw_messages(true);
  parser_.set_substitute_entities(true);
  parser_.parse_file(filename);

  auto document = parser_.get_document();
  root_ = document->get_root_node();
}

void TestNamespace::test_create_new_node_with_default_namespace()
{
  const auto filename = "example1.xml";
  setup(filename);

  // Check original document
  assert_equal(1, root_->find("/ns0:root/ns0:child", nsmap_).size(),
      "Input file should have one child in default namespace");
  assert_equal(0, root_->find("/ns0:root/ns1:child", nsmap_).size(),
      "Input file shouldn't have any child in alternate default namespace");

  // Add child nodes in default namespace and check document again
  auto child = root_->add_child("child");
  child->set_namespace_declaration(nsmap_["ns1"], "");
  root_->add_child_text("\n");
  root_->add_child_with_new_ns("child", nsmap_["ns1"]);
  root_->add_child_text("\n");

  std::cout << "   File " << filename << " after modification" << std::endl
            << parser_.get_document()->write_to_string_formatted() << std::endl;

  // Here we should have three child nodes in two different namespaces
  assert_equal(1, root_->find("/ns0:root/ns0:child", nsmap_).size(),
      "Updated input file should have one child in default namespace");
  assert_equal(2, root_->find("/ns0:root/ns1:child", nsmap_).size(),
      "Updated input file should have two children in alternate default namespace");
}

void TestNamespace::test_create_new_node_using_existing_namespace_prefix()
{
  const auto filename = "example2.xml";
  setup(filename);

  // Check original document
  assert_equal(1, root_->find("/ns0:root/ns0:child", nsmap_).size(),
      "Input file should have one child in root namespace");
  assert_equal(0, root_->find("/ns0:root/ns1:child", nsmap_).size(),
      "Input file shouldn't have any child in child namespace");

  // Add child nodes with specific namespace and check document again
  auto child = root_->add_child("child", "ns0");
  child->set_namespace_declaration(nsmap_["ns1"], "");
  root_->add_child_text("\n");
  root_->add_child_with_new_ns("child", nsmap_["ns1"]);
  root_->add_child_text("\n");
  root_->add_child_with_new_ns("child", nsmap_["ns1"], "ns3");
  root_->add_child_text("\n");

  std::cout << "   File " << filename << " after modification" << std::endl
            << parser_.get_document()->write_to_string_formatted() << std::endl;

  // Here we should have four child nodes in two different namespaces
  assert_equal(2, root_->find("/ns0:root/ns0:child", nsmap_).size(),
      "Updated input file should have two children in root namespace");
  assert_equal(2, root_->find("/ns0:root/ns1:child", nsmap_).size(),
      "Updated input file should have two children in child namespace");
}


////////////////////////////////////////////////////////////////////////////////
template <typename RefType, typename ValueType>
void Tests::assert_equal(const RefType& reference, const ValueType& value, const std::string& msg)
{
  if (ValueType(reference) == value) return;
  fail(reference, value, msg);
}

template <typename RefType, typename ValueType>
void Tests::assert_not_equal(const RefType& reference, const ValueType& value, const std::string& msg)
{
  if (ValueType(reference) != value) return;
  fail(reference, value, msg);
}

template <typename RefType, typename ValueType>
void Tests::fail(const RefType& reference, const ValueType& value, const std::string& msg)
{
  std::stringstream error;
  error << msg << " (reference: " << reference << ", found: " << value << ")";
  throw fail_exception(error.str());
}


////////////////////////////////////////////////////////////////////////////////

int
main(int /* argc */, char** /* argv */)
{
  // Set the global C and C++ locale to the user-configured locale,
  // so we can use std::cout with UTF-8, via Glib::ustring, without exceptions.
  std::locale::global(std::locale(""));

  TestNamespace tests;

  try
  {
    tests.test_create_new_node_with_default_namespace();
    tests.test_create_new_node_using_existing_namespace_prefix();
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception caught: " << ex.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
