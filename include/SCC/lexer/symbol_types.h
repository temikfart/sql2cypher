#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

namespace scc::lexer {

constexpr std::string_view kSYMT_Unknown = "UNKNOWN";
constexpr std::string_view kSYMT_Space = "SPACE";
constexpr std::string_view kSYMT_Digit = "DIGIT";
constexpr std::string_view kSYMT_Alpha = "ALPHA";
constexpr std::string_view kSYMT_Operator = "OPERATOR";
constexpr std::string_view kSYMT_Bracket = "BRACKET";
constexpr std::string_view kSYMT_Punctuation = "PUNCTUATION";
constexpr std::string_view kSYMT_EOF = "EOF";
constexpr std::string_view kSYMT_NullTerminator = "NULL";

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
  explicit SymbolType(const std::string_view& str_type);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const SymbolType&) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::kUnknown;
};

std::ostream& operator<<(std::ostream& os, const SymbolType& type);

} // scc::lexer
