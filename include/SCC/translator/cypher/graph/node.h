#pragma once

#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::cypher {

struct Label {
  std::string value;

  explicit Label(std::string value);

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const Label& label);
constexpr bool operator<(const Label& lhs, const Label& rhs) {
  return lhs.value < rhs.value;
}

struct NodeProperty {
  std::string name;
  std::string value;
  PropertyType type;

  explicit NodeProperty(std::string name, std::string value, PropertyType type);

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const NodeProperty& property);
constexpr bool operator<(const NodeProperty& lhs, const NodeProperty& rhs) {
  if (lhs.type != rhs.type) {
    return lhs.type < rhs.type;
  }
  return lhs.name < rhs.name;
}

struct Node {
  std::set<Label> labels;
  std::set<NodeProperty> properties;
  std::string variable;

  explicit Node(Label label);
  explicit Node(Label label, std::set<NodeProperty> properties);
  explicit Node(Label label, std::set<NodeProperty> properties, std::string variable);

  Node& AddProperty(const NodeProperty& property);
  Node& AddProperty(const std::string& name, const std::string& value, PropertyType type);
  Node& AddLabel(const Label& label);
  Node& AddLabel(const std::string& label);
  Node& SetVariable(const std::string& variable);

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const Node& node);

} // scc::translator::cypher
