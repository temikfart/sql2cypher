#include "SCC/translator/schema/relationship.h"

namespace scc::translator::schema {

Relationship::Relationship(std::string type, std::string start, std::string end)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)) {}
Relationship::Relationship(std::string type, std::string start, std::string end,
                           std::vector<ApplyCondition> conditions)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)),
      conditions(std::move(conditions)) {}

Relationship& Relationship::AddCondition(ApplyCondition condition) {
  if (std::find(conditions.begin(), conditions.end(), condition) == conditions.end()) {
    conditions.push_back(condition);
  }
  return *this;
}
void Relationship::RemoveConditionBySPI(int spi) {
  std::remove_if(conditions.begin(), conditions.end(), [spi](ApplyCondition& condition) {
    return condition.spi == spi;
  });
}
void Relationship::RemoveConditionByEPI(int epi) {
  std::remove_if(conditions.begin(), conditions.end(), [epi](ApplyCondition& condition) {
    return condition.epi == epi;
  });
}

bool Relationship::operator==(const Relationship& other) const {
  if (type != other.type
      || start != other.start
      || end != other.end) {
    return false;
  }
  
  if (conditions != other.conditions) {
    return false;
  }

  return true;
}
bool Relationship::operator!=(const Relationship& other) const {
  return !(*this == other);
}

} // scc::translator::schema
