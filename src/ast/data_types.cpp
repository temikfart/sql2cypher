#include "SCC/ast/data_types.h"

DataType::DataType(Value value) {
  if (value > kString)
    throw std::invalid_argument("Incorrect value for Data Type");
  this->value_ = value;
}
DataType::DataType(const std::string& str_data_type) {
  std::string data_type = scc::common::LowerCase(str_data_type);
  if (data_type == DT_NONE)
    this->value_ = kNone;
  else if (data_type == DT_ROOT)
    this->value_ = kRoot;
  else if (data_type == DT_SERVICE)
    this->value_ = kService;
  else if (data_type == DT_INT)
    this->value_ = kInt;
  else if (data_type == DT_FLOAT)
    this->value_ = kFloat;
  else if (data_type == DT_BRACKET)
    this->value_ = kBracket;
  else if (data_type == DT_PUNCTUATION)
    this->value_ = kPunctuation;
  else if (data_type == DT_OPERATOR)
    this->value_ = kOperator;
  else if (data_type == DT_WORD)
    this->value_ = kWord;
  else if (data_type == DT_STRING)
    this->value_ = kString;
  else
    throw std::invalid_argument("No Data Type for '" + str_data_type + "'");
}

std::string DataType::ToString() const {
  switch (value_) {
    case kNone:
      return DT_NONE;
    case kRoot:
      return DT_ROOT;
    case kService:
      return DT_SERVICE;
    case kInt:
      return DT_INT;
    case kFloat:
      return DT_FLOAT;
    case kBracket:
      return DT_BRACKET;
    case kPunctuation:
      return DT_PUNCTUATION;
    case kOperator:
      return DT_OPERATOR;
    case kWord:
      return DT_WORD;
    case kString:
      return DT_STRING;
  }
}

std::ostream& operator<<(std::ostream& os, const DataType& data_type) {
  os << data_type.ToString();
  return os;
}
