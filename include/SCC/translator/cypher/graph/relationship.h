#pragma once

#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

enum class Direction {
  kLeft,
  kRight,
  kBoth,
};

struct Relationship {
  std::string type;
  Node start;
  Node end;
  Direction direction;
  std::vector<Property> properties;
  std::string variable;

  explicit Relationship(std::string type, Node start, Node end,
                        Direction direction = Direction::kRight);
  explicit Relationship(std::string type, Node start, Node end, std::string variable,
                        Direction direction = Direction::kRight);

  std::string ToString() const;

private:
  std::string GetLeftArrow() const;
  std::string GetRightArrow() const;
};

bool operator==(const Relationship& lhs, const Relationship& rhs);
std::ostream& operator<<(std::ostream& os, const Relationship& rel);

} // scc::translation::cypher
