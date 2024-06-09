#include "SCC/translator/cypher/clauses/set.h"

namespace scc::translator::cypher {

std::string SetPropertyClauseBuilder::Build(const std::string& label, const Property& property) {
  std::stringstream ss;
  Node node(label, "n");
  ss << "MATCH " << node.ToString() << "\n";
  ss << "SET " << node.variable << "." << property.name << " = ";
  if (property.type == PropertyType::kString) {
    ss << "\"" << property.value << "\"";
  } else {
    ss << property.value;
  }

  return ss.str();
}

std::string RemovePropertyClauseBuilder::Build(const Node& node, const std::string& property_name) {
  std::stringstream ss;
  ss << "MATCH " << node.ToString() << "\n";
  ss << "SET " << node.variable << "." << property_name << " = null";

  return ss.str();
}

} // scc::translator::cypher
