#include "SCC/translator/schema/property.h"

namespace scc::translator::schema {

using nlohmann::json;
using cypher::ConstraintType;
using cypher::PropertyType;

Property::Property(std::string name, PropertyType type)
    : name(std::move(name)), type(type) {}
Property::Property(std::string name, PropertyType type, std::vector<PropertyConstraint> constraints)
    : name(std::move(name)), type(type), constraints(std::move(constraints)) {}

void Property::AddConstraint(const PropertyConstraint& constraint) {
  if (!HasConstraint(constraint)) {
    constraints.push_back(constraint);
  }
}
void Property::RemoveConstraintsByPrefix(const std::string& prefix) {
  std::remove_if(constraints.begin(), constraints.end(),
                 [prefix](PropertyConstraint& constraint) {
                   return constraint.name.starts_with(prefix);
                 });
}
bool Property::MustBeUnique() const {
  return HasConstraintType(ConstraintType::kUniqueness);
}
bool Property::MustBeNotNull() const {
  return HasConstraintType(ConstraintType::kExistence);
}
const std::vector<PropertyConstraint>& Property::Constraints() const {
  return constraints;
}

bool Property::Validate(const cypher::Property& property) const {
  if (name != property.name) {
    return false;
  }

  if (property.type == PropertyType::kNull) {
    return true;
  }

  if (type != property.type) {
    return false;
  }

  return true;
}

std::string Property::ToJsonString(int indent, char indent_char) const {
  json property_json;
  to_json(property_json, *this);
  return property_json.dump(indent, indent_char);
}

bool Property::operator==(const Property& other) const {
  return name == other.name
      && type == other.type
      && constraints == other.constraints;
}
bool Property::operator!=(const Property& other) const {
  return !(*this == other);
}

bool Property::HasConstraintType(ConstraintType constraint_type) const {
  return std::any_of(constraints.begin(), constraints.end(),
                     [constraint_type](const PropertyConstraint& c) {
                       return c.type == constraint_type;
                     });
}
bool Property::HasConstraint(const PropertyConstraint& constraint) const {
  return std::find(constraints.begin(), constraints.end(), constraint) != constraints.end();
}

} // scc::translator::schema
