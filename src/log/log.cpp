#include "SCC/log.h"

using namespace logger;

namespace {

void ValidateSeverity(Severity severity) {
  if (Severity::trace < severity) {
    std::cerr << "incorrect SCC log level: "
              << std::to_string(severity) << std::endl;
    exit(EXIT_FAILURE);
  }
}

}

namespace scc::log {

void set_severity(Severity severity) {
  ValidateSeverity(severity);
  Logger::get()->set_severity(severity);
}
void set_severity(const std::string& str_severity) {
  Severity new_severity = to_severity(str_severity);
  set_severity(new_severity);
}
Severity to_severity(const std::string& str_severity) {
  return logger::to_severity(str_severity);
}
std::optional<Severity> get_severity() {
  AppenderType appender_type = AppenderType::console;
  return Logger::get()->severity(appender_type);
}

} // scc::log
