#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#include "SCC/common/string_utils.h"

namespace scc::config {

constexpr std::string_view kMode_Interactive = "interactive";
constexpr std::string_view kMode_Daemon = "daemon";

class SCCMode {
public:
  enum Value : unsigned char {
    kInteractive,
    kDaemon,
  };

  SCCMode() = default;
  SCCMode(Value value);
  explicit SCCMode(const std::string_view& str_mode);

  std::string ToString() const;

  explicit operator bool() const = delete;
  constexpr operator Value() const { return value_; }
  constexpr std::strong_ordering operator<=>(const SCCMode&) const = default;
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

} // scc::config
