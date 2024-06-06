#include "SCC/translator/schema/schema.h"

namespace scc::translator::schema {

using std::format;
using nlohmann::json;
using cypher::ConstraintType;

template<typename T>
using RefWrapper = std::reference_wrapper<T>;
using ConstNodeRefWrapper = RefWrapper<const Node>;
using NodeRefWrapper = RefWrapper<Node>;
template<typename T>
using Optional = std::optional<T>;

Schema::Schema(std::string database_name) : database_name(std::move(database_name)) {}

void Schema::AddNode(const Node& node) {
  auto it = std::find(nodes.begin(), nodes.end(), node);
  if (it != nodes.end()) {
    std::string msg =
        format(R"(Could not add new node: Node '{}' already exists in schema for database '{}')",
               node.label, database_name);
    throw std::runtime_error(msg);
  }
  nodes.push_back(node);
}
void Schema::AddNodeProperty(const std::string& label, const Property& property) {
  auto& node = GetNodeOrThrow(label);
  node.AddProperty(property);
}
void Schema::AddNodePropertyConstraint(const std::string& label, const std::string& property_name,
                                       const PropertyConstraint& constraint) {
  auto& node = GetNodeOrThrow(label);
  auto& properties = node.properties;
  auto propertyIt = std::find_if(properties.begin(), properties.end(),
                                 [property_name](const Property& property) {
                                   return property.name == property_name;
                                 });
  if (propertyIt == properties.end()) {
    std::string msg = format("Could not add new property constraint: "
                             "Property \'{}\' not found in node \'{}\' for database \'{}\'",
                             property_name, label, database_name);
    throw std::runtime_error(msg);
  }

  propertyIt->AddConstraint(constraint);
}
void Schema::AddRelationship(const Relationship& relationship) {
  auto it = std::find(relationships.begin(), relationships.end(), relationship);
  if (it != relationships.end()) {
    std::string msg = format("Could not add new relationship: "
                             "Relationship \'{}\' already exists in schema for database \'{}\'",
                             relationship.type, database_name);
    throw std::runtime_error(msg);
  }
  relationships.push_back(relationship);
}

Optional<ConstNodeRefWrapper> Schema::FindNode(const std::string& label) const {
  auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                             [label](const Node& node) { return node.label == label; });
  if (nodeIt == nodes.end()) {
    return {};
  }
  return {ConstNodeRefWrapper(*nodeIt)};
}
const Node& Schema::FindNodeOrThrow(const std::string& label) const {
  auto node_res = FindNode(label);
  if (!node_res.has_value()) {
    std::string msg = format(R"(Node '{}' not found in schema for database '{}')",
                             label, database_name);
    throw std::runtime_error(msg);
  }
  return node_res.value().get();
}
Optional<NodeRefWrapper> Schema::GetNode(const std::string& label) {
  auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                             [label](const Node& node) { return node.label == label; });
  if (nodeIt == nodes.end()) {
    return {};
  }
  return {NodeRefWrapper(*nodeIt)};
}
Node& Schema::GetNodeOrThrow(const std::string& label) {
  auto node_res = GetNode(label);
  if (!node_res.has_value()) {
    std::string msg = format(R"(Node '{}' not found in schema for database '{}')",
                             label, database_name);
    throw std::runtime_error(msg);
  }
  return node_res.value().get();
}

void Schema::RemoveNode(const std::string& label) {
  auto nodeIt = std::find_if(nodes.begin(), nodes.end(),
                             [label](const Node& node) { return node.label == label; });
  if (nodeIt == nodes.end()) {
    std::string msg = format("Could not find node: "
                             "Node \'{}\' not found in schema for database \'{}\'",
                             label, database_name);
    throw std::runtime_error(msg);
  }
  nodes.erase(nodeIt);

  for (auto relIt = relationships.begin(); relIt != relationships.end(); ++relIt) {
    if (relIt->start == label || relIt->end == label) {
      relationships.erase(relIt);
    }
  }
}
void Schema::RemoveNodeProperty(const std::string& label, const std::string& property_name) {
  auto& node = GetNodeOrThrow(label);
  auto& properties  = node.properties;
  auto propertyIt = std::find_if(properties.begin(), properties.end(),
                                 [property_name](const Property& property) {
                                   return property.name == property_name;
                                 });
  if (propertyIt == properties.end()) {
    return;
  }
  int pi = std::distance(properties.begin(), propertyIt);
  properties.erase(propertyIt);
  RemovePropertyIdxFromConditions(label, pi);
}
void Schema::RemoveNodePropertyConstraints(const std::string& label,
                                           const std::string& constraint_prefix) {
  auto& node = GetNodeOrThrow(label);
  auto& properties  = node.properties;
  for (auto& property : properties) {
    property.RemoveConstraintsByPrefix(constraint_prefix);
  }
}
void Schema::RemoveRelationship(const std::string& type) {
  std::remove_if(relationships.begin(), relationships.end(),
                 [type](Relationship& relationship) {
                   return relationship.type == type;
                 });
}
void Schema::Clear() {
  database_name.clear();
  nodes.clear();
  relationships.clear();
}

std::string Schema::ToJsonString(int indent, char indent_char) const {
  json schema_json;
  to_json(schema_json, *this);
  return schema_json.dump(indent, indent_char);
}

void Schema::RemovePropertyIdxFromConditions(const std::string& label, int pi) {
  for (auto& relationship : relationships) {
    if (relationship.start == label) {
      relationship.RemoveConditionBySPI(pi);
    } else if (relationship.end == label) {
      relationship.RemoveConditionByEPI(pi);
    }
  }
}

} // scc::translator::cypher
