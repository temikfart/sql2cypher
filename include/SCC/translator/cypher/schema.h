#pragma once

#include <algorithm>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/cypher/graph/relationship.h"

namespace scc::translator::cypher {

class Schema {
public:
  std::string database_name;

  explicit Schema() = default;
  explicit Schema(std::string database_name);

private:
  std::vector<Node> nodes;
  std::vector<ConditionalRelationship> relationships;
};

} // scc::translator::cypher
