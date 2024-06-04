#pragma once

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/relationship.h"
#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"

namespace scc::translator::schema {

struct ApplyCondition {
  int spi;
  int epi;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ApplyCondition, spi, epi)

constexpr bool operator==(const ApplyCondition& lhs, const ApplyCondition& rhs) {
  return lhs.spi == rhs.spi && lhs.epi == rhs.epi;
}

class Relationship : public cypher::BaseRelationship {
public:
  Node start;
  Node end;
  std::vector<Property> properties;

  explicit Relationship(std::string type, Node start, Node end, std::vector<Property> properties,
                        std::vector<ApplyCondition> conditions);

  Relationship& AddCondition(ApplyCondition condition);

  bool operator==(const Relationship& other) const;
  bool operator!=(const Relationship& other) const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Relationship, type, start, end, properties, conditions)

private:
  std::vector<ApplyCondition> conditions;
};

inline void to_json(nlohmann::json& j, const std::vector<Relationship>& relationships) {
  j = nlohmann::json::array();
  for (const auto& relationship : relationships) {
    j.push_back(relationship);
  }
}
inline void from_json(const nlohmann::json& j, std::vector<Relationship>& relationships) {
  relationships = j.get<std::vector<Relationship>>();
}

} // scc::translator::schema
