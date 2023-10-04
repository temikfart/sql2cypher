#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

#define INTERACTIVE "interactive"
#define DAEMON "daemon"

namespace scc {

class SCCMode {
public:
  enum Value : unsigned char {
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
  Value value_ = Value::kInteractive;
};

std::ostream& operator<<(std::ostream& os, const SCCMode& mode);

} // scc
