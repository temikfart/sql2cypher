#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::schema {

class Property {
public:
  std::string name;
  cypher::PropertyType type;

  explicit Property(std::string name, cypher::PropertyType type);
  explicit Property(std::string name, cypher::PropertyType type,
                    std::vector<cypher::ConstraintType> constraints);

  void AddConstraint(cypher::ConstraintType constraint);
  void RemoveConstraint(cypher::ConstraintType constraint);
  bool MustBeUnique() const;
  bool MustBeNotNull() const;

  bool operator==(const Property& other) const;
  bool operator!=(const Property& other) const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Property, name, type, constraints)

private:
  std::vector<cypher::ConstraintType> constraints;
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
