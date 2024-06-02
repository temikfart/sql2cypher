#include "SCC/translator/cypher/graph/node.h"

namespace scc::translator::cypher {

Node::Label::Label(std::string name)
    : value(std::move(name)) {}

std::string Node::Label::ToString() const {
  return ":" + value;
}

std::ostream& operator<<(std::ostream& os, const Node::Label& label) {
  os << label.ToString();
  return os;
}
bool operator<(const Node::Label& lhs, const Node::Label& rhs) {
  return lhs.value < rhs.value;
}

Node::Node(std::string label_name)
    : labels({Label(std::move(label_name))}) {}
Node::Node(std::string label_name, std::string variable)
    : labels({Label(std::move(label_name))}), variable(std::move(variable)) {}
Node::Node(Label label)
    : labels({std::move(label)}) {}
Node::Node(Label label, std::vector<Property> properties)
    : labels({std::move(label)}), properties(std::move(properties)) {}
Node::Node(Label label, std::vector<Property> properties, std::string variable)
    : labels({std::move(label)}), properties(std::move(properties)),
      variable(std::move(variable)) {}

Node& Node::AddProperty(const Property& property) {
  properties.push_back(property);
  return *this;
}
Node& Node::AddProperty(const std::string& name, const std::string& value, PropertyType type) {
  properties.emplace_back(name, value, type);
  return *this;
}
bool Node::HasProperty(const std::string& name) const {
  static auto predicate = [name](const Property& property) { return property.name == name; };
  return std::find_if(properties.begin(), properties.end(), predicate) != properties.end();
}
unsigned Node::PropertyCount() const {
  return properties.size();
}
Node& Node::AddLabel(const Label& label) {
  labels.push_back(label);
  return *this;
}
Node& Node::AddLabel(const std::string& label) {
  labels.emplace_back(label);
  return *this;
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
    ss << label.ToString();
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

std::ostream& operator<<(std::ostream& os, const Node& node) {
  os << node.ToString();
  return os;
}

} // scc::translator::cypher
