#include "SCC/translator/schema/property.h"

namespace scc::translator::schema {

using cypher::ConstraintType;
using cypher::PropertyType;

Property::Property(std::string name, PropertyType type)
    : name(std::move(name)), type(type) {}
Property::Property(std::string name, PropertyType type, std::vector<ConstraintType> constraints)
    : name(std::move(name)), type(type), constraints(std::move(constraints)) {}

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

bool Property::operator==(const Property& other) const {
  return name == other.name
      && type == other.type
      && constraints == other.constraints;
}
bool Property::operator!=(const Property& other) const {
  return !(*this == other);
}

} // scc::translator::schema
