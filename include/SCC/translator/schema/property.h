#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::schema {

class PropertyConstraint {
public:
  std::string name;
  cypher::ConstraintType type;

  inline bool operator==(const PropertyConstraint& other) const {
    return name == other.name && type == other.type;
  }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PropertyConstraint, name, type)

class Property {
public:
  std::string name;
  cypher::PropertyType type;

  explicit Property(std::string name, cypher::PropertyType type);
  explicit Property(std::string name, cypher::PropertyType type,
                    std::vector<PropertyConstraint> constraints);

  void AddConstraint(const PropertyConstraint& constraint);
  void RemoveConstraintsByPrefix(const std::string& prefix);
  bool MustBeUnique() const;
  bool MustBeNotNull() const;

  bool operator==(const Property& other) const;
  bool operator!=(const Property& other) const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Property, name, type, constraints)

private:
  std::vector<PropertyConstraint> constraints;

  bool HasConstraintType(cypher::ConstraintType type) const;
  bool HasConstraint(const PropertyConstraint& constraint) const;
};

inline void to_json(nlohmann::json& j, const std::vector<Property>& properties) {
  j = nlohmann::json::array();
  for (const auto& property : properties) {
    j.push_back(property);
  }
}
inline void from_json(const nlohmann::json& j, std::vector<Property>& properties) {
  properties = j.get<std::vector<Property>>();
}

} // scc::translator::schema
