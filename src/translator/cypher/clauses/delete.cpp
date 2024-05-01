#include "SCC/translator/cypher/clauses/delete.h"

namespace scc::translator::cypher {

std::string DeleteNodeClauseBuilder::Build(const Node& node) {
  std::stringstream ss;
  ss << "MATCH " << node.ToString() << "\n";
  ss << "DELETE " << node.variable;

  return ss.str();
}

std::string DeleteRelationshipClauseBuilder::Build(const Relationship& relationship) {
  std::stringstream ss;
  ss << "MATCH " << relationship.ToString() << "\n";
  ss << "DELETE " << relationship.variable;

  return ss.str();
}

} // scc::translator::cypher
