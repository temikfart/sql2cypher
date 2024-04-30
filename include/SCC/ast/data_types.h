#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

namespace scc::ast {

constexpr std::string_view kDT_None = "NONE";
constexpr std::string_view kDT_Root = "ROOT";
constexpr std::string_view kDT_Service = "SERVICE";
constexpr std::string_view kDT_Int = "INT";
constexpr std::string_view kDT_Float = "FLOAT";
constexpr std::string_view kDT_Bracket = "BRACKET";
constexpr std::string_view kDT_Punctuation = "PUNCTUATION";
constexpr std::string_view kDT_Operator = "OPERATOR";
constexpr std::string_view kDT_Word = "WORD";
constexpr std::string_view kDT_String = "STRING";

class DataType {
public:
  enum Value : unsigned char {
    kNone,
    kRoot,
    kService,
    kInt,
    kFloat,
    kBracket,
    kPunctuation,
    kOperator,
    kWord,
    kString,
  };

  DataType() = default;
  DataType(Value value);
  explicit DataType(const std::string_view& std_type);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const DataType&) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::kNone;
};

std::ostream& operator<<(std::ostream& os, const DataType& type);

} // scc::ast
