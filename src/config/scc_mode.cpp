#include "SCC/config/scc_mode.h"

namespace scc::config {

SCCMode::SCCMode(Value value) {
  if (value > kDaemon)
    throw std::invalid_argument("Incorrect value for SCC Mode");
  this->value_ = value;
}
SCCMode::SCCMode(const std::string_view& str_mode) {
  std::string mode = common::LowerCase(std::string(str_mode));
  if (mode == kMode_Interactive)
    this->value_ = kInteractive;
  else if (mode == kMode_Daemon)
    this->value_ = kDaemon;
  else
    throw std::invalid_argument("No SCC Mode for '" + std::string(str_mode) + "'");
}

std::string SCCMode::ToString() const {
  switch (value_) {
    case kInteractive:
      return std::string(kMode_Interactive);
    case kDaemon:
      return std::string(kMode_Daemon);
  }
}

std::ostream& operator<<(std::ostream& os, const SCCMode& mode) {
  os << mode.ToString();
  return os;
}

} // scc::config
