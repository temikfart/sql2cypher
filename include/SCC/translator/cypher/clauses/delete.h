#pragma once

#include <string>
#include <sstream>

#include "SCC/common/string_utils.h"
#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/cypher/graph/relationship.h"

namespace scc::translator::cypher {

class DeleteNodeClauseBuilder {
public:
  static std::string Build(const Node& node);
};

class DeleteRelationshipClauseBuilder {
public:
  static std::string Build(const Relationship& relationship);
};

} // scc::translator::cypher
