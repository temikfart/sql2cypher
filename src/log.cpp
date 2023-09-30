#include "SCC/log.h"

void SCCLogger::set_severity(logger::Severity severity) {
  SCCLogger::ValidateSeverity(severity);
  logger::Logger::get()->set_severity(severity);
}
void SCCLogger::set_severity(const std::string& str_severity) {
  logger::Severity new_severity = to_severity(str_severity);
  set_severity(new_severity);
}
logger::Severity SCCLogger::to_severity(const std::string& str_severity) {
  return logger::to_severity(str_severity);
}
std::optional<logger::Severity> SCCLogger::get_severity() {
  logger::AppenderType appender_type = logger::AppenderType::console;
  return logger::Logger::get()->severity(appender_type);
}

void SCCLogger::ValidateSeverity(logger::Severity severity) {
  if (logger::Severity::trace < severity) {
    std::cerr << "incorrect SCC log level: "
              << std::to_string(severity) << std::endl;
    exit(EXIT_FAILURE);
  }
}
