#pragma once

#include <optional>
#include <string>

#include "logger/log.hpp"

namespace scc::log {

void set_severity(logger::Severity severity);
void set_severity(const std::string& str_severity);
logger::Severity to_severity(const std::string& str_severity);
std::optional<logger::Severity> get_severity();

} // scc::log
