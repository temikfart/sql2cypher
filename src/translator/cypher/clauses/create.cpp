#include "SCC/translator/cypher/clauses/create.h"

namespace scc::translator::cypher {

std::string CreateDatabaseClauseBuilder::Build(const std::string& database_name) {
  return "CREATE DATABASE " + database_name + ";";
}

std::string CreateNodeClauseBuilder::Build(const Node& node) {
  return "CREATE " + node.ToString() + ";";
}

} // scc::translator::cypher
