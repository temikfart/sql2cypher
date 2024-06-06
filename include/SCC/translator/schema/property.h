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

inline void to_json(nlohmann::json& j, const PropertyConstraint& constraint)  {
  j["name"] = constraint.name;
  j["type"] = constraint.type.ToString();
}
inline void from_json(const nlohmann::json& j, PropertyConstraint& constraint) {
  j.at("name").get_to(constraint.name);
  j.at("type").get_to(constraint.type);
}

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

  friend void to_json(nlohmann::json& j, const Property& property) {
    j["name"] = property.name;
    j["type"] = property.type.ToString();
    j["constraints"] = property.constraints;
  }
  friend void from_json(const nlohmann::json& j, Property& property) {
    j.at("name").get_to(property.name);
    j.at("type").get_to(property.type);
    j.at("constraints").get_to(property.constraints);
  }

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
