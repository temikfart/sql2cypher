#pragma once

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::cypher {

struct Property {
  std::string name;
  std::string value;
  PropertyType type;

  explicit Property(std::string name, std::string value, PropertyType type);
  explicit Property(std::string name, std::string value, PropertyType type,
                    std::vector<ConstraintType> constraints);

  void AddConstraint(ConstraintType constraint);
  void RemoveConstraint(ConstraintType constraint);
  bool MustBeUnique() const;
  bool MustBeNotNull() const;

  std::string ToString() const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Property, name, value, type, constraints)

private:
  std::vector<ConstraintType> constraints;
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

bool operator==(const Property& lhs, const Property& rhs);
std::ostream& operator<<(std::ostream& os, const Property& property);
bool operator<(const Property& lhs, const Property& rhs);

} // scc::translator::cypher
