#include "SCC/lexer/symbol_types.h"

namespace scc::lexer {

SymbolType::SymbolType(Value value) {
  if (value > kNullTerminator)
    throw std::invalid_argument("Incorrect value for Symbol Type");
  this->value_ = value;
}
SymbolType::SymbolType(const std::string_view& str_type) {
  std::string type = scc::common::UpperCase(std::string(str_type));
  if (type == kSYMT_Unknown)
    this->value_ = kUnknown;
  else if (type == kSYMT_Space)
    this->value_ = kSpace;
  else if (type == kSYMT_Digit)
    this->value_ = kDigit;
  else if (type == kSYMT_Alpha)
    this->value_ = kAlpha;
  else if (type == kSYMT_Operator)
    this->value_ = kOperator;
  else if (type == kSYMT_Bracket)
    this->value_ = kBracket;
  else if (type == kSYMT_Punctuation)
    this->value_ = kPunctuation;
  else if (type == kSYMT_EOF)
    this->value_ = kEOF;
  else if (type == kSYMT_NullTerminator)
    this->value_ = kNullTerminator;
  else
    throw std::invalid_argument("No Symbol Type for '" + std::string(str_type) + "'");
}

std::string SymbolType::ToString() const {
  switch (value_) {
    case kUnknown:
      return std::string(kSYMT_Unknown);
    case kSpace:
      return std::string(kSYMT_Space);
    case kDigit:
      return std::string(kSYMT_Digit);
    case kAlpha:
      return std::string(kSYMT_Alpha);
    case kOperator:
      return std::string(kSYMT_Operator);
    case kBracket:
      return std::string(kSYMT_Bracket);
    case kPunctuation:
      return std::string(kSYMT_Punctuation);
    case kEOF:
      return std::string(kSYMT_EOF);
    case kNullTerminator:
      return std::string(kSYMT_NullTerminator);
  }
}

std::ostream& operator<<(std::ostream& os, const SymbolType& type) {
  os << type.ToString();
  return os;
}

} // scc::lexer
