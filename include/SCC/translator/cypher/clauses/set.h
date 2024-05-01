#pragma once

#include <string>
#include <sstream>

#include "SCC/common/string_utils.h"
#include "SCC/translator/cypher/graph/node.h"

namespace scc::translator::cypher {

class SetPropertyClauseBuilder {
public:
  static std::string Build(const Node& node, const NodeProperty& property);
};

class RemovePropertyClauseBuilder {
public:
  static std::string Build(const Node& node, const std::string& property_name);
};

} // scc::translator::cypher
