#include "SCC/translator/schema/relationship.h"

namespace scc::translator::schema {

Relationship::Relationship(std::string type, Node start, Node end, std::vector<Property> properties,
                           std::vector<ApplyCondition> conditions)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)),
      properties(std::move(properties)), conditions(std::move(conditions)) {}

Relationship& Relationship::AddCondition(ApplyCondition condition) {
  if (std::find(conditions.begin(), conditions.end(), condition) == conditions.end()) {
    conditions.push_back(condition);
  }
  return *this;
}

bool Relationship::operator==(const Relationship& other) const {
  if (type != other.type
      || start != other.start
      || end != other.end) {
    return false;
  }

  if (properties != other.properties) {
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
