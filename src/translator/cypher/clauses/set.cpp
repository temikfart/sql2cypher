#include "SCC/translator/cypher/clauses/set.h"

namespace scc::translator::cypher {

std::string SetPropertyClauseBuilder::Build(const Node& node, const NodeProperty& property) {
  std::stringstream ss;
  ss << "MATCH " << node.ToString() << "\n";
  ss << "SET " << node.variable << "." << property.ToString();

  return ss.str();
}

std::string RemovePropertyClauseBuilder::Build(const Node& node, const std::string& property_name) {
  if (!node.HasProperty(property_name)) {
    throw std::invalid_argument("Could not build Remove Property clause: property \'"
                                    + property_name + " does not exist on node \'" + node.ToString()
                                    + "\'");
  }
  std::stringstream ss;
  ss << "MATCH " << node.ToString() << "\n";
  ss << "SET " << node.variable << "." << property_name << " = null";

  return ss.str();
}

} // scc::translator::cypher
