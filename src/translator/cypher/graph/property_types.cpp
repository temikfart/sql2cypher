#include "SCC/translator/cypher/graph/property_types.h"

namespace scc::translator::cypher {

PropertyType::PropertyType(Value value) {
  if (value > kString)
    throw std::invalid_argument("Incorrect value for Cypher Property Type");
  this->value_ = value;
}
PropertyType::PropertyType(const std::string_view& str_type) {
  std::string type = scc::common::UpperCase(std::string(str_type));
  if (type == kPT_Unknown)
    this->value_ = kUnknown;
  else if (type == kPT_Boolean)
    this->value_ = kBoolean;
  else if (type == kPT_Float)
    this->value_ = kFloat;
  else if (type == kPT_Integer)
    this->value_ = kInteger;
  else if (type == kPT_String)
    this->value_ = kString;
  else
    throw std::invalid_argument("No Cypher Property Type for '" + std::string(str_type) + "'");
}

std::string PropertyType::ToString() const {
  switch (value_) {
    case kUnknown:
      return std::string(kPT_Unknown);
    case kBoolean:
      return std::string(kPT_Boolean);
    case kFloat:
      return std::string(kPT_Float);
    case kInteger:
      return std::string(kPT_Integer);
    case kString:
      return std::string(kPT_String);
  }
}

std::ostream& operator<<(std::ostream& os, const PropertyType& type) {
  os << type.ToString();
  return os;
}

} // scc::translator::cypher
