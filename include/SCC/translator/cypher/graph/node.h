#pragma once

#include <algorithm>
#include <format>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <utility>

#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

constexpr std::string_view stub_str = "stub";

struct Node {
  struct Label {
    std::string value;

    explicit Label(std::string value);

    std::string ToString() const;
  };

  std::set<Label> labels;
  std::vector<Property> properties;
  std::string variable;

  explicit Node(std::string label_name);
  explicit Node(std::string label_name, std::string variable);
  explicit Node(Label label);
  explicit Node(Label label, std::vector<Property> properties);
  explicit Node(Label label, std::vector<Property> properties, std::string variable);

  Node& AddProperty(const Property& property);
  Node& AddProperty(const std::string& name, const std::string& value, PropertyType type);
  bool HasProperty(const std::string& name) const;
  unsigned PropertyCount() const;
  Node& AddLabel(const Label& label);
  Node& AddLabel(const std::string& label);
  Node& SetVariable(const std::string& variable);

  std::string ToString() const;
};

bool operator==(const Node::Label& lhs, const Node::Label& rhs);
std::ostream& operator<<(std::ostream& os, const Node::Label& label);
bool operator<(const Node::Label& lhs, const Node::Label& rhs);

bool operator==(const Node& lhs, const Node& rhs);
std::ostream& operator<<(std::ostream& os, const Node& node);

} // scc::translator::cypher
