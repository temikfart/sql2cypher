#pragma once

#include <optional>
#include <string>

#include "logger/log.hpp"

// TODO: change to namespace without class.
class SCCLogger {
public:
  static void set_severity(logger::Severity severity);
  static void set_severity(const std::string& str_severity);
  static logger::Severity to_severity(const std::string& str_severity);
  static std::optional<logger::Severity> get_severity();

private:
  static void ValidateSeverity(logger::Severity severity);
};
