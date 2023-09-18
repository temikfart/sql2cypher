#include "SCC/log.h"

void SCCLogger::set_severity(logger::Severity severity) {
  SCCLogger::ValidateSeverity(severity);
  logger::Logger::get()->set_severity(severity);
}
void SCCLogger::set_severity(std::string& severity) {
  logger::Severity new_severity = logger::to_severity(severity);
  set_severity(new_severity);
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
