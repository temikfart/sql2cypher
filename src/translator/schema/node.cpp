#include "SCC/translator/schema/node.h"

namespace scc::translator::schema {

using std::format;

Node::Node(std::string label)
    : label(std::move(label)) {}
Node::Node(std::string label, std::vector<Property> properties)
    : label(std::move(label)), properties(std::move(properties)) {}

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
const Property& Node::FindPropertyOrThrow(const std::string& name) const {
  auto predicate = [name](const Property& property) { return property.name == name; };
  auto propIt = std::find_if(properties.begin(), properties.end(), predicate);
  if (propIt == properties.end())  {
    std::string msg = format(R"(Property with name '{}' does not exist in node '{}')",
                             name, label);
    throw std::runtime_error(msg);
  }
  return *propIt;
}
int Node::PropertyIndex(const std::string& name) const {
  auto predicate = [name](const Property& property) { return property.name == name; };
  auto it = std::find_if(properties.begin(), properties.end(), predicate);
  if (it == properties.end()) {
    return -1;
  }
  return std::distance(properties.begin(), it);
}
unsigned Node::PropertyCount() const {
  return properties.size();
}

bool Node::Validate(const cypher::Node& node) const {
  if (label != node.label) {
    return false;
  }

  if (PropertyCount() != node.PropertyCount())  {
    return false;
  }

  for (unsigned i = 0; i < PropertyCount(); ++i) {
    const auto& schema_property = properties[i];
    const auto& property = node.properties[i];
    if (!schema_property.Validate(property)) {
      return false;
    }
  }

  return true;
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
