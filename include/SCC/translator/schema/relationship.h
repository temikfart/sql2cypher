#pragma once

#include <algorithm>
#include <string>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"

namespace scc::translator::schema {

struct ApplyCondition {
  int spi = -1;
  int epi = -1;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ApplyCondition, spi, epi)

constexpr bool operator==(const ApplyCondition& lhs, const ApplyCondition& rhs) {
  return lhs.spi == rhs.spi && lhs.epi == rhs.epi;
}

class Relationship {
public:
  std::string type;
  std::string start;
  std::string end;

  explicit Relationship() = default;
  explicit Relationship(std::string type, std::string start, std::string end);
  explicit Relationship(std::string type, std::string start, std::string end,
                        std::vector<ApplyCondition> conditions);

  Relationship& AddCondition(ApplyCondition condition);
  void RemoveConditionBySPI(int spi);
  void RemoveConditionByEPI(int epi);
  const std::vector<ApplyCondition>& Conditions() const;

  bool operator==(const Relationship& other) const;
  bool operator!=(const Relationship& other) const;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Relationship, type, start, end, conditions)

private:
  std::vector<ApplyCondition> conditions;
};

} // scc::translator::schema
