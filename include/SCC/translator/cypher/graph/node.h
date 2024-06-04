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

class BaseNode {
public:
  std::string label;
};

class Node : public BaseNode {
public:
  std::vector<Property> properties;
  std::string variable;

  explicit Node(std::string label);
  explicit Node(std::string label, std::string variable);
  explicit Node(std::string label, std::vector<Property> properties);
  explicit Node(std::string label, std::vector<Property> properties, std::string variable);

  Node& AddProperty(const Property& property);
  Node& AddProperty(const std::string& name, const std::string& value, PropertyType type);
  bool HasProperty(const std::string& name) const;
  unsigned PropertyCount() const;
  Node& SetVariable(const std::string& variable);

  std::string ToString() const;
};

bool operator==(const Node& lhs, const Node& rhs);
std::ostream& operator<<(std::ostream& os, const Node& node);

} // scc::translator::cypher
