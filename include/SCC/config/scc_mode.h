#pragma once

#include <string>
#include <stdexcept>

#include "SCC/common/string_utils.h"

#define INTERACTIVE "interactive"
#define DAEMON "daemon"

class SCCMode {
public:
  enum Value: unsigned char {
    kInteractive,
    kDaemon,
  };

  SCCMode() = default;
  SCCMode(Value value);
  explicit SCCMode(const std::string& str_mode);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const SCCMode& mode) const = default;
  constexpr bool operator==(const Value& value) const { return value_ == value; }
  constexpr bool operator!=(const Value& value) const { return value_ != value; }
  constexpr bool operator<(const Value& value) const { return value_ < value; }
  constexpr bool operator<=(const Value& value) const { return value_ <= value; }
  constexpr bool operator>(const Value& value) const { return value_ > value; }
  constexpr bool operator>=(const Value& value) const { return value_ >= value; }

private:
  Value value_;
};
