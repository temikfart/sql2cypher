#pragma once

#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "nlohmann/json.hpp"

#include "SCC/common/string_utils.h"

namespace scc::translator::cypher {

constexpr std::string_view kCT_None = "NONE";
constexpr std::string_view kCT_Uniqueness = "UNIQUE";
constexpr std::string_view kCT_Existence = "NOT NULL";

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

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConstraintType, value_)

private:
  Value value_ = Value::kNone;
};

NLOHMANN_JSON_SERIALIZE_ENUM(ConstraintType::Value, {
  { ConstraintType::Value::kNone, kCT_None },
  { ConstraintType::Value::kUniqueness, kCT_Uniqueness },
  { ConstraintType::Value::kExistence, kCT_Existence },
})

std::ostream& operator<<(std::ostream& os, const ConstraintType& type);

} // scc::translator::cypher
