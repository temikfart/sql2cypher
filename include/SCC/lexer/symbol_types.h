#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

#define SYMT_UNKNOWN "unknown"
#define SYMT_SPACE "space"
#define SYMT_DIGIT "digit"
#define SYMT_ALPHA "alpha"
#define SYMT_OPERATOR "operator"
#define SYMT_BRACKET "bracket"
#define SYMT_PUNCTUATION "punctuation"
#define SYMT_EOF "eof"
#define SYMT_NULL_TERMINATOR "null terminator"

namespace scc::lexer {

class SymbolType {
public:
  enum Value : unsigned char {
    kUnknown,
    kSpace,
    kDigit,
    kAlpha,
    kOperator,
    kBracket,
    kPunctuation,
    kEOF,
    kNullTerminator,
  };

  SymbolType() = default;
  SymbolType(Value value);
  explicit SymbolType(const std::string& str_sym_type);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const SymbolType& sym_type) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::kUnknown;
};

std::ostream& operator<<(std::ostream& os, const SymbolType& sym_type);

} // scc::lexer
