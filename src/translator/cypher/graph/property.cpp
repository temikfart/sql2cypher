#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

Property::Property(std::string  name, std::string value, PropertyType type)
    : BaseProperty::BaseProperty(std::move(name), type), value(std::move(value)) {}
Property::Property(std::string name, std::string value, PropertyType type,
                   std::vector<ConstraintType> constraints)
    : BaseProperty::BaseProperty(std::move(name), type), value(std::move(value)),
      constraints(std::move(constraints)) {}

void Property::AddConstraint(ConstraintType constraint) {
  if (std::find(constraints.begin(), constraints.end(), constraint) == constraints.end()) {
    constraints.push_back(constraint);
  }
}
void Property::RemoveConstraint(ConstraintType constraint) {
  auto it = std::find(constraints.begin(), constraints.end(), constraint);
  if (it != constraints.end()) {
    constraints.erase(it);
  }
}
bool Property::MustBeUnique() const {
  return std::any_of(constraints.begin(), constraints.end(),
                     [](ConstraintType c) { return c == ConstraintType::kUniqueness; });
}
bool Property::MustBeNotNull() const {
  return std::any_of(constraints.begin(), constraints.end(),
                     [](ConstraintType c) { return c == ConstraintType::kExistence; });
}

std::string Property::ToString() const {
  return name + ": " + value;
}

bool operator==(const Property& lhs, const Property& rhs) {
  return lhs.name == rhs.name
      && lhs.value == rhs.value
      && lhs.type == rhs.type;
}
std::ostream& operator<<(std::ostream& os, const Property &property) {
  os << property.ToString();
  return os;
}
bool operator<(const Property& lhs, const Property& rhs) {
  if (lhs.type != rhs.type) {
    return lhs.type < rhs.type;
  }
  return lhs.name < rhs.name;
}

} // scc::translator::cypher
