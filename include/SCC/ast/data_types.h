#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

#define DT_NONE "none"
#define DT_ROOT "root"
#define DT_SERVICE "service"
#define DT_INT "int"
#define DT_FLOAT "float"
#define DT_BRACKET "bracket"
#define DT_PUNCTUATION "punctuation"
#define DT_OPERATOR "operator"
#define DT_WORD "word"
#define DT_STRING "string"

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
  explicit DataType(const std::string& std_data_type);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const DataType& mode) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::kNone;
};

std::ostream& operator<<(std::ostream& os, const DataType& data_type);
