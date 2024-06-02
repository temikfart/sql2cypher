#pragma once

#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

struct Relationship {
  enum class Direction {
    kLeft,
    kRight,
    kBoth,
  };

  std::string type;
  Node start;
  Node end;
  Direction direction;
  std::string variable;
  std::vector<Property> properties;

  explicit Relationship(std::string type, Node start, Node end,
                        Direction direction = Direction::kRight);
  explicit Relationship(std::string type, Node start, Node end, std::string variable,
                        Direction direction = Direction::kRight);

  std::string ToString() const;

private:
  std::string GetLeftArrow() const;
  std::string GetRightArrow() const;
};

std::ostream& operator<<(std::ostream& os, const Relationship& rel);

} // scc::translation::cypher
