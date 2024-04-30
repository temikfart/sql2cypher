#pragma once

#include <string>
#include <sstream>

#include "SCC/common/string_utils.h"
#include "SCC/translator/cypher/graph/node.h"

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

} // scc::translator::cypher
