#include "SCC/translator/cypher/graph/property.h"

namespace scc::translator::cypher {

Property::Property(std::string  name, std::string value, PropertyType type)
    : name(std::move(name)), type(type), value(std::move(value)) {}

std::string Property::ToString() const {
  std::stringstream ss;
  ss << name << ": ";
  if (type == PropertyType::kString)  {
    ss << "\"" << value << "\"";
  } else {
    ss << value;
  }
  return ss.str();
}

bool operator==(const Property& lhs, const Property& rhs) {
  return lhs.name == rhs.name
      && lhs.value == rhs.value
      && lhs.type == rhs.type;
}
bool operator!=(const Property& lhs, const Property& rhs) {
  return !(lhs == rhs);
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
