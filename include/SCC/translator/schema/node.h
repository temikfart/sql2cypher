#pragma once

#include <algorithm>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/schema/property.h"

namespace scc::translator::schema {

class Node  {
public:
  std::string label;
  std::vector<Property> properties;

  explicit Node() = default;
  explicit Node(std::string label);
  explicit Node(std::string label, std::vector<Property> properties);

  Node& AddProperty(const Property& property);
  bool HasProperty(const std::string& name) const;
  const Property& FindPropertyOrThrow(const std::string& name) const;
  int PropertyIndex(const std::string& name) const;
  unsigned PropertyCount() const;

  bool Validate(const cypher::Node& node) const;

  bool operator==(const Node& other) const;
  bool operator!=(const Node& other) const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Node, label, properties)

} // scc::translator::schema
