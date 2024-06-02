#pragma once

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

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

private:
  std::vector<ConstraintType> constraints;
};

std::ostream& operator<<(std::ostream& os, const Property& property);
bool operator<(const Property& lhs, const Property& rhs);

} // scc::translator::cypher
