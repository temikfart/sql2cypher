#pragma once

#include <optional>
#include <string>

#include "logger/log.hpp"

#if defined(DEBIAN_PACKAGE)
#define SCC_LOG_DIR "/var/log/SCC_log/"
#elif defined(EXE_PACKAGE)
#define SCC_LOG_DIR "C:/Program Files/SCC/log/"
#elif defined(APP_PACKAGE)
#define SCC_LOG_DIR ""
#else
#define SCC_LOG_DIR "../log"
#endif

// TODO: change to namespace without class.
class SCCLogger {
public:
  static void set_severity(logger::Severity severity);
  static void set_severity(std::string& str_severity);
  static logger::Severity to_severity(std::string& str_severity);
  static std::optional<logger::Severity> get_severity();

private:
  static void ValidateSeverity(logger::Severity severity);
};
