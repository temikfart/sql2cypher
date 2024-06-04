#include <utility>

#include "SCC/translator/cypher/graph/relationship.h"

namespace scc::translator::cypher {

Relationship::Relationship(std::string type, Node start, Node end, Direction direction)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)), direction(direction) {}
Relationship::Relationship(std::string type, Node start, Node end, std::string variable,
                           Direction direction)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)), direction(direction),
      variable(std::move(variable)) {}
Relationship::Relationship(std::string type, Node start, Node end, std::vector<Property> properties,
                           std::string variable, Direction direction)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)), direction(direction),
      properties(std::move(properties)), variable(std::move(variable)) {}

std::string Relationship::ToString() const {
  std::stringstream ss;
  ss << (start.variable.empty() ? start.ToString() : "(" + start.variable + ")");
  ss << GetLeftArrow();
  ss << "[";
  ss << variable << ":" << type;
  if (!properties.empty()) {
    ss << " {";
    for (auto it = properties.begin(); it != properties.end(); ++it) {
      if (it != properties.begin()) {
        ss << ", ";
      }
      ss << it->ToString();
    }
    ss << "}";
  }
  ss << "]";
  ss << GetRightArrow();
  ss << (end.variable.empty() ? end.ToString() : "(" + end.variable + ")");

  return ss.str();
}

std::string Relationship::GetLeftArrow() const {
  if (direction == Direction::kBoth || direction == Direction::kLeft)
    return "<-";
  return "-";
}
std::string Relationship::GetRightArrow() const {
  if (direction == Direction::kBoth || direction == Direction::kRight)
    return "->";
  return "-";
}

bool operator==(const Relationship& lhs, const Relationship& rhs) {
  if (lhs.properties != rhs.properties) {
    return false;
  }

  if (lhs.type != rhs.type
      || lhs.start != rhs.start
      || lhs.end != rhs.end
      || lhs.direction != rhs.direction) {
    return false;
  }

  return true;
}
std::ostream& operator<<(std::ostream& os, const Relationship& relationship) {
  os << relationship.ToString();
  return os;
}

ConditionalRelationship::ConditionalRelationship(const Relationship& relationship,
                                                 std::vector<ApplyCondition> conditions)
    : Relationship::Relationship(relationship.type, relationship.start, relationship.end,
                                 relationship.properties, relationship.variable,
                                 relationship.direction),
      conditions(std::move(conditions)) {}

} // scc::translator::cypher
