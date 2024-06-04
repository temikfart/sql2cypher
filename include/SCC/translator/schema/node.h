#pragma once

#include <algorithm>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/schema/property.h"

namespace scc::translator::schema {

class Node  {
public:
  std::string label;
  std::vector<Property> properties;

  explicit Node(std::string label);
  explicit Node(std::string label, std::vector<Property> properties);

  Node& AddProperty(const Property& property);
  bool HasProperty(const std::string& name) const;

  bool operator==(const Node& other) const;
  bool operator!=(const Node& other) const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Node, label, properties)

inline void to_json(nlohmann::json& j, const std::vector<Node>& nodes) {
  j = nlohmann::json::array();
  for (const auto& node : nodes) {
    j.push_back(node);
  }
}
inline void from_json(const nlohmann::json& j, std::vector<Node>& nodes) {
  nodes = j.get<std::vector<Node>>();
}

} // scc::translator::schema
