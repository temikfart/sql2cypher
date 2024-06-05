#include "SCC/translator/schema/schema.h"

namespace scc::translator::schema {

using nlohmann::json;

Schema::Schema(std::string database_name) : database_name(std::move(database_name)) {}

void Schema::AddNode(const Node& node) {
  auto it = std::find(nodes.begin(), nodes.end(), node);
  if (it != nodes.end()) {
    std::string msg = format(R"(Node '{}' already exists in schema for database '{}')",
                             node.label, database_name);
    throw std::runtime_error(msg);
  }
  nodes.push_back(node);
}
void Schema::AddConditionalRelationship(const Relationship& relationship) {
  auto it = std::find(relationships.begin(), relationships.end(), relationship);
  if (it != relationships.end()) {
    std::string msg = format(R"(Relationship '{}' already exists in schema for database '{}')",
                             relationship.type, database_name);
    throw std::runtime_error(msg);
  }
  relationships.push_back(relationship);
}

std::string Schema::ToJsonString(const int indent, const char indent_char) const {
  json schema_json;
  to_json(schema_json, *this);
  return schema_json.dump(indent, indent_char);
}

} // scc::translator::cypher
