#pragma once

#include <iostream>
#include <stdexcept>

class DataType {
public:
  enum Value : unsigned char {
    UNKNOWN,
    ROOT,
    SERVICE,
    INT_NUMBER,
    FLOAT_NUMBER,
    BRACKET,
    PUNCTUATION,
    OPERATOR,
    WORD,
    STRING
  };

  DataType() = default;
  DataType(Value value);
//  explicit DataType(const std::string& data_type);

//  std::string ToString() const;

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
  Value value_ = Value::UNKNOWN;
};

//std::ostream& operator<<(std::ostream& os, const DataType& data_type);
