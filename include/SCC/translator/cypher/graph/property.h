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

class Property {
public:
  std::string name;
  PropertyType type;
  std::string value;

  explicit Property() = default;
  explicit Property(std::string name, std::string value, PropertyType type);

  std::string ToString() const;
};

bool operator==(const Property& lhs, const Property& rhs);
std::ostream& operator<<(std::ostream& os, const Property& property);
bool operator<(const Property& lhs, const Property& rhs);

} // scc::translator::cypher
