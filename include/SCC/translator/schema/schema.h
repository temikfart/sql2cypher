#pragma once

#include <algorithm>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/relationship.h"

namespace scc::translator::schema {

class Schema {
public:
  std::string database_name;

  explicit Schema() = default;
  explicit Schema(std::string database_name);

  void AddNode(const Node& node);
  void AddConditionalRelationship(const Relationship& relationship);

  std::string ToJsonString(const int indent = 2, const char indent_char = ' ') const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Schema, database_name, nodes, relationships);

private:
  std::vector<Node> nodes;
  std::vector<Relationship> relationships;
};

} // scc::translator::cypher
