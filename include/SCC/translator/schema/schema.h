#pragma once

#include <algorithm>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"
#include "SCC/translator/schema/relationship.h"

namespace scc::translator::schema {

class Schema {
public:
  std::string database_name;

  explicit Schema() = default;
  explicit Schema(std::string database_name);

  void AddNode(const Node& node);
  void AddNodeProperty(const std::string& label, const Property& property);
  void AddNodePropertyConstraint(const std::string& label, const std::string& property_name,
                                 const PropertyConstraint& constraint);
  void AddRelationship(const Relationship& relationship);

  std::optional<std::reference_wrapper<const Node>> FindNode(const std::string& label) const;
  const Node& FindNodeOrThrow(const std::string& label) const;
  std::optional<std::reference_wrapper<Node>> GetNode(const std::string& label);
  Node& GetNodeOrThrow(const std::string& label);

  void RemoveNode(const std::string& label);
  void RemoveNodeProperty(const std::string& label, const std::string& property_name);
  void Clear();

  std::string ToJsonString(int indent = 2, char indent_char = ' ') const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Schema, database_name, nodes, relationships);

private:
  std::vector<Node> nodes;
  std::vector<Relationship> relationships;

  void RemovePropertyIdxFromConditions(const std::string& label, int pi);
};

} // scc::translator::cypher
