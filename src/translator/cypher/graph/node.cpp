#include "SCC/translator/cypher/graph/node.h"

namespace scc::translator::cypher {

using std::format;

Node::Node(std::string label)
    : labels({std::move(label)}) {}
Node::Node(std::string label_name, std::string variable)
    : labels({std::move(label_name)}), variable(std::move(variable)) {}
Node::Node(std::string label, std::vector<Property> properties)
    : labels({std::move(label)}), properties(std::move(properties)) {}
Node::Node(std::string label, std::vector<Property> properties, std::string variable)
    : labels({std::move(label)}), properties(std::move(properties)),
      variable(std::move(variable)) {}

Node& Node::AddProperty(const Property& property) {
  if (HasProperty(property.name)) {
    std::string msg = format("Property {} already exists", property.ToString());
    throw std::runtime_error(msg);
  }
  properties.push_back(property);
  return *this;
}
Node& Node::AddProperty(const std::string& name, const std::string& value, PropertyType type) {
  properties.emplace_back(name, value, type);
  return *this;
}
bool Node::HasProperty(const std::string& name) const {
  auto predicate = [name](const Property& property) { return property.name == name; };
  return std::any_of(properties.begin(), properties.end(), predicate);
}
unsigned Node::PropertyCount() const {
  return properties.size();
}
Node& Node::SetVariable(const std::string& variable) {
  this->variable = variable;
  return *this;
}

std::string Node::ToString() const {
  std::stringstream ss;
  ss << "(";
  if (!variable.empty()) {
    ss << variable;
  }
  for (const auto& label : labels) {
    ss << ":" << label;
  }
  if (!properties.empty()) {
    ss << " {";
    for (auto it = properties.begin(); it != properties.end(); ++it) {
      if (it != properties.begin()) {
        ss << ", ";
      }
      ss << it->ToString();
    }
    ss << "}";
  }
  ss << ")";
  return ss.str();
}

bool operator==(const Node& lhs, const Node& rhs)  {
  if (lhs.labels != rhs.labels) {
    return false;
  }

  if (lhs.properties != rhs.properties) {
    return false;
  }

  return true;
}
std::ostream& operator<<(std::ostream& os, const Node& node) {
  os << node.ToString();
  return os;
}

} // scc::translator::cypher
