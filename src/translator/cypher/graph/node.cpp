#include "SCC/translator/cypher/graph/node.h"

namespace scc::translator::cypher {

Label::Label(std::string name)
    : value(std::move(name)) {}

std::string Label::ToString() const {
  return ":" + value;
}

std::ostream& operator<<(std::ostream& os, const Label& label) {
  os << label.ToString();
  return os;
}
bool operator<(const Label& lhs, const Label& rhs) {
  return lhs.value < rhs.value;
}

NodeProperty::NodeProperty(std::string  name, std::string value, PropertyType type)
    : name(std::move(name)), value(std::move(value)), type(type) {}

std::string NodeProperty::ToString() const {
  return name + ": " + value;
}

std::ostream& operator<<(std::ostream& os, const NodeProperty &property) {
  os << property.ToString();
  return os;
}
bool operator<(const NodeProperty& lhs, const NodeProperty& rhs) {
  if (lhs.type != rhs.type) {
    return lhs.type < rhs.type;
  }
  return lhs.name < rhs.name;
}

Node::Node(Label label)
    : labels({std::move(label)}) {}
Node::Node(Label label, std::vector<NodeProperty> properties)
    : labels({std::move(label)}), properties(std::move(properties)) {}
Node::Node(Label label, std::vector<NodeProperty> properties, std::string variable)
    : labels({std::move(label)}), properties(std::move(properties)),
      variable(std::move(variable)) {}

Node& Node::AddProperty(const NodeProperty& property) {
  properties.push_back(property);
  return *this;
}
Node& Node::AddProperty(const std::string& name, const std::string& value, PropertyType type) {
  properties.emplace_back(name, value, type);
  return *this;
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
      ss << it->ToString();
      if (it != properties.end()) {
        ss << ", ";
      }
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
