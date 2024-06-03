#pragma once

#include <format>
#include <stdexcept>
#include <string>
#include <sstream>

#include "SCC/common/string_utils.h"
#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/cypher/graph/relationship.h"

namespace scc::translator::cypher {

constexpr std::string_view kDefaultVar = "n";

class CreateDatabaseClauseBuilder {
public:
  static std::string Build(const std::string& database_name);
};

class CreateNodeClauseBuilder {
public:
  static std::string Build(const Node& node);
};

class CreateConstraintClauseBuilder {
public:
  static std::string Build(const std::string& constraint_name, const Node& node,
                           const Property& property, ConstraintType type);
};

class CreateRelationshipClauseBuilder {
public:
  static std::string Build(const Relationship& relationship);
};

} // scc::translator::cypher
