#include <utility>

#include "SCC/translator/cypher/graph/relationship.h"

namespace scc::translator::cypher {

Relationship::Relationship(std::string type, Node start, Node end, Direction direction)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)), direction(direction) {}
Relationship::Relationship(std::string type, Node start, Node end, std::string variable,
                           Direction direction)
    : type(std::move(type)), start(std::move(start)), end(std::move(end)), direction(direction),
      variable(std::move(variable)) {}

std::string Relationship::ToString() const {
  std::stringstream ss;
  ss << (start.variable.empty() ? start.ToString() : "(" + start.variable + ")");
  ss << GetLeftArrow();
  ss << "[" << variable << ":" << type << "]";
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

std::ostream& operator<<(std::ostream& os, const Relationship& relationship) {
  os << relationship.ToString();
  return os;
}

} // scc::translator::cypher
