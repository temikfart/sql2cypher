#pragma once

#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

namespace scc::translator::cypher {

constexpr std::string_view kPT_Unknown = "UNKNOWN";
constexpr std::string_view kPT_Boolean = "BOOLEAN";
constexpr std::string_view kPT_Float = "FLOAT";
constexpr std::string_view kPT_Integer = "INTEGER";
constexpr std::string_view kPT_String = "STRING";

class PropertyType {
public:
  enum Value : unsigned char {
    kUnknown,
    kBoolean,
    kFloat,
    kInteger,
    kString,
  };

  PropertyType() = default;
  PropertyType(Value value);
  explicit PropertyType(const std::string_view& str_type);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const PropertyType&) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::kUnknown;
};

std::ostream& operator<<(std::ostream& os, const PropertyType& type);

} // scc::translator::cypher
