#include "SCC/translator/cypher/graph/node.h"

namespace scc::translator::cypher {

using std::format;

Node::Node(std::string label)
    : label(std::move(label)) {}
Node::Node(std::string label, std::string variable)
    : label(std::move(label)), variable(std::move(variable)) {}
Node::Node(std::string label, std::vector<Property> properties)
    : label(std::move(label)), properties(std::move(properties)) {}
Node::Node(std::string label, std::vector<Property> properties, std::string variable)
    : label(std::move(label)), properties(std::move(properties)), variable(std::move(variable)) {}

Node& Node::AddProperty(const Property& property) {
  if (HasProperty(property.name)) {
    std::string msg = format(R"(Property with name '{}' already exists in node '{}')",
                             property.name, label);
    throw std::runtime_error(msg);
  }
  properties.push_back(property);
  return *this;
}
Node& Node::AddProperty(const std::string& name, const std::string& value, PropertyType type) {
  return AddProperty(Property(name, value, type));
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
  ss << ":" << label;
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
  if (lhs.label != rhs.label) {
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
