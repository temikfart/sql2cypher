#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <map>

#define LOG(level, msg) {                                                     \
  std::string path = __FILE__;                                                \
  SCC_log.add_log(LogLevel::level) << path.substr(path.find_last_of('/') + 1) \
  << ": " << __FUNCTION__ << "(): " << __LINE__ << ": " << msg << std::endl; }

enum LogLevel {
  FATAL,
  ERROR,
  INFO,
  TRACE,
  DEBUG
};

void configure();

class Log {
private:
  std::string filename_;
  std::ofstream std_output_;
  std::map<LogLevel, std::string> log_levels_ = {
    {LogLevel::FATAL, "FATAL"},
    {LogLevel::ERROR, "ERROR"},
    {LogLevel::INFO, "INFO"},
    {LogLevel::TRACE, "TRACE"},
    {LogLevel::DEBUG, "DEBUG"}
  };
  LogLevel log_level_ = LogLevel::INFO;
  
  std::string get_timestamp();
  std::string time_to_log_name(std::string timestamp);
public:
  Log();
  std::ofstream& add_log(LogLevel level = INFO);
  LogLevel get_log_level() const;
  ~Log();
};

extern Log SCC_log;
