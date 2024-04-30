#pragma once

#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

namespace scc::translator::cypher {

constexpr std::string_view kCT_None = "none";
constexpr std::string_view kCT_Uniqueness = "uniqueness";
constexpr std::string_view kCT_Existence = "existence";

class ConstraintType {
public:
  enum Value : unsigned char {
    kNone,
    kUniqueness,
    kExistence,
  };

  ConstraintType() = default;
  ConstraintType(Value value);
  explicit ConstraintType(const std::string_view& str_type);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const ConstraintType&) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_ = Value::kNone;
};

std::ostream& operator<<(std::ostream& os, const ConstraintType& type);

} // scc::translator::cypher
