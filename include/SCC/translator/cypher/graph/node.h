#pragma once

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::cypher {

constexpr std::string_view stub_str = "stub";

struct Label {
  std::string value;

  explicit Label(std::string value);

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const Label& label);
bool operator<(const Label& lhs, const Label& rhs);

struct Property {
  std::string name;
  std::string value;
  PropertyType type;

  explicit Property(std::string name, std::string value, PropertyType type);

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const Property& property);
bool operator<(const Property& lhs, const Property& rhs);

struct Node {
  std::vector<Label> labels = {};
  std::vector<Property> properties = {};
  std::string variable = "";

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

std::ostream& operator<<(std::ostream& os, const Node& node);

} // scc::translator::cypher
