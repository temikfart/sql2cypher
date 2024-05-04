#pragma once

#include <string>
#include <sstream>

#include "SCC/common/string_utils.h"
#include "SCC/translator/cypher/graph/node.h"

namespace scc::translator::cypher {

class DropDatabaseClauseBuilder {
public:
  static std::string Build(const std::string& database_name);
};

class DropConstraintClauseBuilder {
public:
  static std::string Build(const std::string& constraint_name);
};

} // scc::translator::cypher
