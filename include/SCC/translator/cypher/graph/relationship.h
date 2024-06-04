#pragma once

#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/node.h"
#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

enum class Direction {
  kLeft,
  kRight,
  kBoth,
};

NLOHMANN_JSON_SERIALIZE_ENUM(Direction, {
  { Direction::kLeft, "<" },
  { Direction::kRight, ">" },
  { Direction::kBoth, "-" },
})

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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Relationship, type, start, end, direction, properties, variable)

bool operator==(const Relationship& lhs, const Relationship& rhs);
std::ostream& operator<<(std::ostream& os, const Relationship& rel);

struct ApplyCondition {
public:
  int spi;
  int epi;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ApplyCondition, spi, epi)

struct ConditionalRelationship : public Relationship {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConditionalRelationship, conditions)

private:
  std::vector<ApplyCondition> conditions;
};

} // scc::translation::cypher
