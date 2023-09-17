#pragma once

#include <sys/stat.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "logger/log.hpp"

#define LOG_OLD(level, msg) do {                                    \
  std::string path = __FILE__;                                  \
  std::ostringstream new_log;                                   \
  new_log << path.substr(path.find_last_of('/') + 1)            \
    << ": " << __FUNCTION__ << "(): " << __LINE__ << ": "       \
    << msg << std::endl;                                        \
  SCC_log.AddLog(level, new_log.str());                         \
} while(false)

#ifdef DEBIAN_PACKAGE
#define SCC_LOG_DIR "/var/log/SCC_log/"
#endif // DEBIAN_PACKAGE

#ifdef EXE_PACKAGE
#define SCC_LOG_DIR "C:/Program Files/SCC/log/"
#endif // EXE_PACKAGE

#ifdef APP_PACKAGE
#define SCC_LOG_DIR ""
#endif // APP_PACKAGE

enum LogLevel {
  SILENT,
  FATAL,
  ERROR,
  INFO,
  TRACE,
  DEBUG,
  LOG_LEVEL_COUNT
};

class Log {
public:
  void set_log_level(LogLevel level);
  LogLevel get_log_level() const;
  void set_log_path(const std::string& dir);
  std::string get_log_path() const;
  void set_is_system_configured(bool value);
  bool get_is_system_configured() const;
  bool get_is_buffer_load() const;
  void set_is_logdir_set(bool value);

  void Start();
  void AddLog(LogLevel level, const std::string& msg);
  void LoadBufferedLogs();
  static std::string GetLogDir();
  LogLevel StringToLogLevel(std::string level) const;
  bool CloseLogFile();

private:
  std::string filepath_;
  std::ofstream output_;
  LogLevel log_level_ = LogLevel::INFO;

  std::vector<std::pair<LogLevel, std::string>> buffered_logs_;
  bool is_buffer_load_ = false;
  bool is_logdir_set_ = false;
  bool is_system_configured_ = false;

  std::map<LogLevel, std::string> lvl2str_ = {
      {LogLevel::SILENT, "SILENT"},
      {LogLevel::FATAL, "FATAL"},
      {LogLevel::ERROR, "ERROR"},
      {LogLevel::INFO, "INFO"},
      {LogLevel::TRACE, "TRACE"},
      {LogLevel::DEBUG, "DEBUG"},
  };
  std::map<std::string, LogLevel> str2lvl_ = {
      {"SILENT", LogLevel::SILENT},
      {"FATAL", LogLevel::FATAL},
      {"ERROR", LogLevel::ERROR},
      {"INFO", LogLevel::INFO},
      {"TRACE", LogLevel::TRACE},
      {"DEBUG", LogLevel::DEBUG}
  };

  static std::string GetTimestamp();
  std::string TimeToLogFilename(std::string timestamp) const;
  static bool IsFileExists(const std::string& path);

  static void ValidateLogLevel(LogLevel level);
  void ValidateLogLevel(std::string& level) const;
  void ValidateDoesFileExist(const std::string& path) const;
  void ValidateIsLogFileOpen() const;
};

extern Log SCC_log;
