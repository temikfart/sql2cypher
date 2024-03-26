#include "SCC/config/scc_mode.h"

namespace scc::config {

SCCMode::SCCMode(Value value) {
  if (value > kDaemon)
    throw std::invalid_argument("Incorrect value for SCC Mode");
  this->value_ = value;
}
SCCMode::SCCMode(const std::string& str_mode) {
  std::string mode = common::LowerCase(str_mode);
  if (mode == INTERACTIVE)
    this->value_ = kInteractive;
  else if (mode == DAEMON)
    this->value_ = kDaemon;
  else
    throw std::invalid_argument("No SCC Mode for '" + str_mode + "'");
}

std::string SCCMode::ToString() const {
  switch (value_) {
    case kInteractive:
      return INTERACTIVE;
    case kDaemon:
      return DAEMON;
  }
}

std::ostream& operator<<(std::ostream& os, const SCCMode& mode) {
  os << mode.ToString();
  return os;
}

} // scc::config
