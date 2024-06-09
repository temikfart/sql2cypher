#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/property.h"
#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::schema {

class PropertyConstraint {
public:
  std::string name;
  cypher::ConstraintType type;

  inline bool operator==(const PropertyConstraint& other) const {
    return name == other.name && type == other.type;
  }

  friend void to_json(nlohmann::json& j, const PropertyConstraint& constraint) {
    j["name"] = constraint.name;
    j["type"] = constraint.type.ToString();
  }
  friend void from_json(const nlohmann::json& j, PropertyConstraint& constraint) {
    j.at("name").get_to(constraint.name);
    constraint.type = cypher::ConstraintType(std::string(j.at("type")));
  }
};

class Property {
public:
  std::string name;
  cypher::PropertyType type;

  explicit Property() = default;
  explicit Property(std::string name, cypher::PropertyType type);
  explicit Property(std::string name, cypher::PropertyType type,
                    std::vector<PropertyConstraint> constraints);

  void AddConstraint(const PropertyConstraint& constraint);
  void RemoveConstraintsByPrefix(const std::string& prefix);
  bool MustBeUnique() const;
  bool MustBeNotNull() const;
  const std::vector<PropertyConstraint>& Constraints() const;

  bool Validate(const cypher::Property& property) const;

  std::string ToJsonString(int indent = 2, char indent_char = ' ') const;

  bool operator==(const Property& other) const;
  bool operator!=(const Property& other) const;

  friend void to_json(nlohmann::json& j, const Property& property) {
    j["name"] = property.name;
    j["type"] = property.type.ToString();
    j["constraints"] = property.constraints;
  }
  friend void from_json(const nlohmann::json& j, Property& property) {
    j.at("name").get_to(property.name);
    property.type = cypher::PropertyType(std::string(j.at("type")));
    j.at("constraints").get_to(property.constraints);
  }

private:
  std::vector<PropertyConstraint> constraints;

  bool HasConstraintType(cypher::ConstraintType type) const;
  bool HasConstraint(const PropertyConstraint& constraint) const;
};

} // scc::translator::schema
