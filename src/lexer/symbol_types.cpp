#include "SCC/lexer/symbol_types.h"

SymbolType::SymbolType(Value value) {
  if (value > kNullTerminator)
    throw std::invalid_argument("Incorrect value for Symbol Type");
  this->value_ = value;
}
SymbolType::SymbolType(const std::string& str_sym_type) {
  std::string sym_type = scc::common::LowerCase(str_sym_type);
  if (sym_type == SYMT_UNKNOWN)
    this->value_ = kUnknown;
  else if (sym_type == SYMT_SPACE)
    this->value_ = kSpace;
  else if (sym_type == SYMT_DIGIT)
    this->value_ = kDigit;
  else if (sym_type == SYMT_ALPHA)
    this->value_ = kAlpha;
  else if (sym_type == SYMT_OPERATOR)
    this->value_ = kOperator;
  else if (sym_type == SYMT_BRACKET)
    this->value_ = kBracket;
  else if (sym_type == SYMT_PUNCTUATION)
    this->value_ = kPunctuation;
  else if (sym_type == SYMT_EOF)
    this->value_ = kEOF;
  else if (sym_type == SYMT_NULL_TERMINATOR)
    this->value_ = kNullTerminator;
  else
    throw std::invalid_argument("No Symbol Type for '" + str_sym_type + "'");
}

std::string SymbolType::ToString() const {
  switch (value_) {
    case kUnknown:
      return SYMT_UNKNOWN;
    case kSpace:
      return SYMT_SPACE;
    case kDigit:
      return SYMT_DIGIT;
    case kAlpha:
      return SYMT_ALPHA;
    case kOperator:
      return SYMT_OPERATOR;
    case kBracket:
      return SYMT_BRACKET;
    case kPunctuation:
      return SYMT_PUNCTUATION;
    case kEOF:
      return SYMT_EOF;
    case kNullTerminator:
      return SYMT_NULL_TERMINATOR;
  }
}

std::ostream& operator<<(std::ostream& os, const SymbolType& mode) {
  os << mode.ToString();
  return os;
}
