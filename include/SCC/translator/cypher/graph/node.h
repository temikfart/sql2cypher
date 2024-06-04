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

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

constexpr std::string_view stub_str = "stub";

struct Node {
public:
  std::string label;
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Node, label, properties, variable)

inline void to_json(nlohmann::json& j, const std::vector<Node>& nodes) {
  j = nlohmann::json::array();
  for (const auto& node : nodes) {
    j.push_back(node);
  }
}

inline void from_json(const nlohmann::json& j, std::vector<Node>& nodes) {
  nodes = j.get<std::vector<Node>>();
}

bool operator==(const Node& lhs, const Node& rhs);
std::ostream& operator<<(std::ostream& os, const Node& node);

} // scc::translator::cypher
