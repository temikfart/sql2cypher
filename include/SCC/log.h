#pragma once

#include <optional>
#include <string>

#include "logger/log.hpp"

#ifdef DEBIAN_PACKAGE
#define SCC_LOG_DIR "/var/log/SCC_log/"
#endif // DEBIAN_PACKAGE

#ifdef EXE_PACKAGE
#define SCC_LOG_DIR "C:/Program Files/SCC/log/"
#endif // EXE_PACKAGE

#ifdef APP_PACKAGE
#define SCC_LOG_DIR ""
#endif // APP_PACKAGE

class SCCLogger {
public:
  static void set_severity(logger::Severity severity);
  static void set_severity(std::string& severity);
  static std::optional<logger::Severity> get_severity();

private:
  static void ValidateSeverity(logger::Severity severity);
};
