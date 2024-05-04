#include "SCC/translator/cypher/clauses/drop.h"

namespace scc::translator::cypher {

std::string DropDatabaseClauseBuilder::Build(const std::string& database_name) {
  return "DROP DATABASE " + database_name;
}

std::string DropConstraintClauseBuilder::Build(const std::string& constraint_name) {
  return "DROP CONSTRAINT " + constraint_name;
}

} // scc::translator::cypher
