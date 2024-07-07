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
                                                 const std::string& label,
                                                 const std::string& property_name,
                                                 ConstraintType type) {
  if (type == ConstraintType::kNone) {
    std::string msg = format("Could not build \'CREATE CONSTRAINT\' clause "
                             "for \'{}\' cypher constraint type", type.ToString());
    throw std::invalid_argument(msg);
  }

  std::stringstream ss;
  ss << "CREATE CONSTRAINT " << constraint_name << "\n";

  std::string var = std::string(kDefaultVar);
  ss << "FOR (" << var;
  ss << ":" << label;
  ss << ")" << "\n";

  ss << "REQUIRE (" << var << "." << property_name << ")";
  ss << " IS " << common::UpperCase(type.ToString());

  return ss.str();
}

std::string CreateRelationshipClauseBuilder::Build(const Relationship& relationship) {
  std::stringstream ss;
  ss << "MATCH " << relationship.start.ToString() << ", " << relationship.end.ToString() << "\n";
  ss << "CREATE " << relationship.ToString();
  return ss.str();
}
std::string CreateRelationshipClauseBuilder::Build(const std::string& start,
                                                   const std::string& start_prop,
                                                   const std::string& end,
                                                   const std::string& end_prop,
                                                   const std::string& relationship_type) {
  std::stringstream ss;
  std::string varA = "a", varB = "b";
  ss << format("MATCH ({}:{}), ({}:{})\n", varA, start, varB, end);
  ss << format("WHERE {}.{} = {}.{}\n", varA, start_prop, varB, end_prop);
  ss << format("CREATE ({})-[:{}]->({})", varA, relationship_type, varB);
  return ss.str();
}

} // scc::translator::cypher
