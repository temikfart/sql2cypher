#include "SCC/ast/data_types.h"

namespace scc::ast {

DataType::DataType(Value value) {
  if (value > kString)
    throw std::invalid_argument("Incorrect value for Data Type");
  this->value_ = value;
}
DataType::DataType(const std::string_view& str_type) {
  std::string type = scc::common::UpperCase(std::string(str_type));
  if (type == kDT_None)
    this->value_ = kNone;
  else if (type == kDT_Root)
    this->value_ = kRoot;
  else if (type == kDT_Service)
    this->value_ = kService;
  else if (type == kDT_Int)
    this->value_ = kInt;
  else if (type == kDT_Float)
    this->value_ = kFloat;
  else if (type == kDT_Bracket)
    this->value_ = kBracket;
  else if (type == kDT_Punctuation)
    this->value_ = kPunctuation;
  else if (type == kDT_Operator)
    this->value_ = kOperator;
  else if (type == kDT_Word)
    this->value_ = kWord;
  else if (type == kDT_String)
    this->value_ = kString;
  else
    throw std::invalid_argument("No Data Type for '" + std::string(str_type) + "'");
}

std::string DataType::ToString() const {
  switch (value_) {
    case kNone:
      return std::string(kDT_None);
    case kRoot:
      return std::string(kDT_Root);
    case kService:
      return std::string(kDT_Service);
    case kInt:
      return std::string(kDT_Int);
    case kFloat:
      return std::string(kDT_Float);
    case kBracket:
      return std::string(kDT_Bracket);
    case kPunctuation:
      return std::string(kDT_Punctuation);
    case kOperator:
      return std::string(kDT_Operator);
    case kWord:
      return std::string(kDT_Word);
    case kString:
      return std::string(kDT_String);
  }
}

std::ostream& operator<<(std::ostream& os, const DataType& type) {
  os << type.ToString();
  return os;
}

} // scc::ast
