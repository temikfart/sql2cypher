#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

Property::Property(std::string  name, std::string value, PropertyType type)
    : name(std::move(name)), value(std::move(value)), type(type) {}

std::string Property::ToString() const {
  return name + ": " + value;
}

std::ostream& operator<<(std::ostream& os, const Property &property) {
  os << property.ToString();
  return os;
}
bool operator<(const Property& lhs, const Property& rhs) {
  if (lhs.type != rhs.type) {
    return lhs.type < rhs.type;
  }
  return lhs.name < rhs.name;
}

} // scc::translator::cypher
