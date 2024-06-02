#pragma once

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::cypher {

struct Property {
  std::string name;
  std::string value;
  PropertyType type;

  explicit Property(std::string name, std::string value, PropertyType type);

  std::string ToString() const;
};

std::ostream& operator<<(std::ostream& os, const Property& property);
bool operator<(const Property& lhs, const Property& rhs);

} // scc::translator::cypher
