#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <map>
#include <getopt.h>
#include <sys/stat.h>
#include <unistd.h>

#define LOG(level, msg) do {                                    \
  std::string path = __FILE__;                                  \
  std::ostringstream new_log;                                   \
  new_log << path.substr(path.find_last_of('/') + 1)            \
    << ": " << __FUNCTION__ << "(): " << __LINE__ << ": "       \
    << msg << std::endl;                                        \
  SCC_log.add_log(level, std::move(new_log.str()));             \
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

void Configure(int argc, char* argv[]);

class Log {
private:
  std::string filename_;
  std::ofstream output_;

  LogLevel log_level_ = LogLevel::INFO;
  std::map<LogLevel, std::string> log_lvl2str_ = {
    {LogLevel::FATAL, "FATAL"},
    {LogLevel::ERROR, "ERROR"},
    {LogLevel::INFO, "INFO"},
    {LogLevel::TRACE, "TRACE"},
    {LogLevel::DEBUG, "DEBUG"}
  };
  std::map<std::string, LogLevel> str2log_lvl_ = {
    {"FATAL", LogLevel::FATAL},
    {"ERROR", LogLevel::ERROR},
    {"INFO", LogLevel::INFO},
    {"TRACE", LogLevel::TRACE},
    {"DEBUG", LogLevel::DEBUG}
  };

  static std::string get_timestamp();
  std::string TimeToLogName(std::string timestamp);

  void ValidateLogLevel(LogLevel level) const;
  void ValidateLogLevel(std::string& level) const;
public:
  Log();
  void add_log(LogLevel level, const std::string& msg);
  LogLevel get_log_level() const;
  void set_log_level(LogLevel level);
  LogLevel StringToLogLevel(std::string& level);
  ~Log();
};

extern Log SCC_log;

enum SCCMode {
  INTERACTIVE,
  DAEMON,
  MODE_COUNT
};

enum OptFlag {
  fDAEMON = 'd',
  fHELP = 'h',
  fINTERACTIVE = 'i',
  fLOG = 'l',
  fMODE = 'm',
  fCYPHER = 'z' + 1,
  fSQL
};

enum ConfigIsSet {
  ConfMODE,
  ConfSQL,
  ConfCYPHER
};

class Config: public Log {
public:
  SCCMode mode_ = SCCMode::INTERACTIVE;
  std::map<std::string, SCCMode> str2modes_ = {
          {"INTERACTIVE", SCCMode::INTERACTIVE},
          {"DAEMON", SCCMode::DAEMON},
  };
  std::map<SCCMode, std::string> modes2str_ = {
          {SCCMode::INTERACTIVE, "INTERACTIVE"},
          {SCCMode::DAEMON, "DAEMON"}
  };

  std::string sql_path_;
  std::ifstream input_;

  std::string cypher_path_;
  std::ofstream output_;

  std::map<ConfigIsSet, bool> is_config_set_ = {
          {ConfigIsSet::ConfMODE,   false},
          {ConfigIsSet::ConfSQL,    false},
          {ConfigIsSet::ConfCYPHER, false},
  };
  std::map<OptFlag, ConfigIsSet> flag_to_config_ = {
          {OptFlag::fDAEMON,      ConfigIsSet::ConfMODE},
          {OptFlag::fINTERACTIVE, ConfigIsSet::ConfMODE},
          {OptFlag::fMODE,        ConfigIsSet::ConfMODE},
          {OptFlag::fCYPHER,      ConfigIsSet::ConfSQL},
          {OptFlag::fSQL,         ConfigIsSet::ConfCYPHER}
  };

  void ValidateMode(SCCMode mode) const;
  void ValidateMode(std::string& mode) const;
  static bool IsFileExists(std::string& path);
  static bool IsFileExists(const std::ofstream& f);
  void ValidateSQLPath(std::string& sql_path) const;
  void ValidateCypherPath(std::string& cypher_path) const;
  bool IsFlagSet(OptFlag flag);
  void ValidateSetFlag(OptFlag flag);
  void SetFlag(OptFlag flag);

public:
  Config();
  void Start();

  void set_mode(SCCMode mode);
  void set_sql_path(std::string& new_sql_path);
  void set_cypher_path(std::string& new_cypher_path);

  SCCMode get_mode() const;
  std::string get_sql_path() const;
  std::string get_cypher_path() const;
  std::ifstream& Input();
  std::ofstream& Output();

  void GetConsoleArguments(int argc, char* const* argv);
  SCCMode StringToSCCMode(std::string& mode);
  std::string SCCModeToString(SCCMode mode);

  ~Config();
};

extern Config config;
