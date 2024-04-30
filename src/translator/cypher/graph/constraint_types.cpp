#include "SCC/translator/cypher/graph/constraint_types.h"

namespace scc::translator::cypher {

ConstraintType::ConstraintType(Value value) {
  if (value > kExistence)
    throw std::invalid_argument("Incorrect value for Cypher Constraint Type");
  this->value_ = value;
}
ConstraintType::ConstraintType(const std::string_view& str_type) {
  std::string type = scc::common::LowerCase(std::string(str_type));
  if (type == kCT_None)
    this->value_ = kNone;
  else if (type == kCT_Uniqueness)
    this->value_ = kUniqueness;
  else if (type == kCT_Existence)
    this->value_ = kExistence;
  else
    throw std::invalid_argument("No Cypher Constraint Type for '" + std::string(str_type) + "'");
}

std::string ConstraintType::ToString() const {
  switch (value_) {
    case kNone:
      return std::string(kCT_None);
    case kUniqueness:
      return std::string(kCT_Uniqueness);
    case kExistence:
      return std::string(kCT_Existence);
  }
}

std::ostream& operator<<(std::ostream& os, const ConstraintType& type) {
  os << type.ToString();
  return os;
}

} // scc::translator::cypher
