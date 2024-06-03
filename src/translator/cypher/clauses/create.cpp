#include "SCC/translator/cypher/clauses/create.h"

namespace scc::translator::cypher {

using std::format;

std::string CreateDatabaseClauseBuilder::Build(const std::string& database_name) {
  return "CREATE DATABASE " + database_name;
}

std::string CreateNodeClauseBuilder::Build(const Node& node) {
  return "CREATE " + node.ToString();
}

std::string CreateConstraintClauseBuilder::Build(const std::string& constraint_name,
                                                 const Node& node, const Property& property,
                                                 ConstraintType type) {
  if (type == ConstraintType::kNone) {
    std::string msg = format("Could not build \'CREATE CONSTRAINT\' clause "
                             "for \'{}\' cypher constraint type", type.ToString());
    throw std::invalid_argument(msg);
  }

  std::stringstream ss;
  ss << "CREATE CONSTRAINT " << constraint_name << "\n";

  std::string variable_name = node.variable.empty() ? std::string(kDefaultVar) : node.variable;
  ss << "FOR (" << variable_name;
  ss << ":" << node.label;
  ss << ")" << "\n";

  ss << "REQUIRE (" << variable_name << "." << property.name << ")";
  ss << " IS " << common::UpperCase(type.ToString());

  return ss.str();
}

std::string CreateRelationshipClauseBuilder::Build(const Relationship& relationship) {
  std::stringstream ss;
  ss << "MATCH " << relationship.start.ToString() << ", " << relationship.end.ToString() << "\n";
  ss << "CREATE " << relationship.ToString();
  return ss.str();
}

} // scc::translator::cypher
