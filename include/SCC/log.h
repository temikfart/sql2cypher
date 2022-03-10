#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <map>

#define LOG(level, msg) do {                                    \
  std::string path = __FILE__;                                  \
  std::ostringstream new_log;                                   \
  new_log << path.substr(path.find_last_of('/') + 1)            \
    << ": " << __FUNCTION__ << "(): " << __LINE__ << ": "       \
    << msg << std::endl;                                        \
  SCC_log.AddLog(level, std::move(new_log.str()));              \
  break;                                                        \
} while(true)

enum LogLevel {
  FATAL,
  ERROR,
  INFO,
  TRACE,
  DEBUG,
  LOG_LEVEL_COUNT
};

class Log {
private:
  std::string filename_;
  std::ofstream output_;
  LogLevel log_level_ = LogLevel::INFO;
  
  std::map<LogLevel, std::string> lvl2str_ = {
    {LogLevel::FATAL, "FATAL"},
    {LogLevel::ERROR, "ERROR"},
    {LogLevel::INFO,  "INFO"},
    {LogLevel::TRACE, "TRACE"},
    {LogLevel::DEBUG, "DEBUG"}
  };
  std::map<std::string, LogLevel> str2lvl_ = {
    {"FATAL", LogLevel::FATAL},
    {"ERROR", LogLevel::ERROR},
    {"INFO",  LogLevel::INFO},
    {"TRACE", LogLevel::TRACE},
    {"DEBUG", LogLevel::DEBUG}
  };
  
  std::string GetLogPath() const;
  static std::string get_timestamp();
  std::string TimeToLogName(std::string timestamp) const;
  void ValidateLogLevel(LogLevel level) const;
  void ValidateLogLevel(std::string& level) const;
  
public:
  Log();
  void AddLog(LogLevel level, const std::string& msg);
  LogLevel get_log_level() const;
  void set_log_level(LogLevel level);
  LogLevel StringToLogLevel(std::string& level) const;
  ~Log();
};

extern Log SCC_log;
