#include "SCC/translator/schema/node.h"

namespace scc::translator::schema {

using cypher::BaseNode;

Node::Node(std::string label)
    : BaseNode::BaseNode(std::move(label)) {}
Node::Node(std::string label, std::vector<Property> properties)
    : BaseNode::BaseNode(std::move(label)), properties(std::move(properties)) {}

Node& Node::AddProperty(const Property& property) {
  if (HasProperty(property.name)) {
    std::string msg = format(R"(Property with name '{}' already exists in node '{}')",
                             property.name, label);
    throw std::runtime_error(msg);
  }
  properties.push_back(property);
  return *this;
}
bool Node::HasProperty(const std::string& name) const {
  auto predicate = [name](const Property& property) { return property.name == name; };
  return std::any_of(properties.begin(), properties.end(), predicate);
}

bool Node::operator==(const Node& other) const {
  if (label != other.label) {
    return false;
  }

  if (properties != other.properties) {
    return false;
  }

  return true;
}
bool Node::operator!=(const Node& other) const {
  return !(*this == other);
}

} // scc::translator::schema
