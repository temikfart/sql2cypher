#pragma once

#include <sys/stat.h>
#include <getopt.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "SCC/log.h"

enum SCCMode {
  kInteractive,
  kDaemon,

  kSCCModeCount
};

enum OptFlag {
  kDaemonFlag = 'd',
  kHelpFlag = 'h',
  kInteractiveFlag = 'i',
  kLogFlag = 'l',
  kModeFlag = 'm',
  kVersionFlag = 'v',
  kCypherFlag = 'z' + 1,
  kSQLFlag = 'z' + 2
};

enum ConfigIsSet {
  kConfigLog,
  kConfigMode,
  kConfigSQL,
  kConfigCypher,
};

class Config {
public:
  Config();
  ~Config();

  void set_mode(SCCMode mode);
  SCCMode get_mode() const;
  void set_sql_path(const std::string& new_sql_path);
  std::string get_sql_path() const;
  void set_cypher_path(const std::string& new_cypher_path);
  std::string get_cypher_path() const;

  void Start(int argc, char *argv[]);
  void GetConsoleArguments(int argc, char *const *argv);

  std::string GetConfigPath() const;
  char GetSQLSymbol();
  char PeekSQLSymbol();
  std::ifstream& ReadSQL();
  std::ofstream& WriteCypher();
  void CloseInputFile();
  void CloseOutputFile();

  SCCMode StringToSCCMode(std::string mode) const;
  std::string SCCModeToString(SCCMode mode) const;

private:
  SCCMode mode_ = SCCMode::kInteractive;

  const std::map<std::string, SCCMode> str2modes_ = {
      {"INTERACTIVE", SCCMode::kInteractive},
      {"DAEMON", SCCMode::kDaemon},
  };
  const std::map<SCCMode, std::string> modes2str_ = {
      {SCCMode::kInteractive, "INTERACTIVE"},
      {SCCMode::kDaemon, "DAEMON"}
  };

  std::string sql_path_;
  std::ifstream input_;

  std::string cypher_path_;
  std::ofstream output_;

  std::map<ConfigIsSet, bool> is_config_set_ = {
      {ConfigIsSet::kConfigLog, false},
      {ConfigIsSet::kConfigMode, false},
      {ConfigIsSet::kConfigSQL, false},
      {ConfigIsSet::kConfigCypher, false},
  };
  const std::map<OptFlag, ConfigIsSet> flag_to_config_ = {
      {OptFlag::kDaemonFlag, ConfigIsSet::kConfigMode},
      {OptFlag::kInteractiveFlag, ConfigIsSet::kConfigMode},
      {OptFlag::kLogFlag, ConfigIsSet::kConfigLog},
      {OptFlag::kModeFlag, ConfigIsSet::kConfigMode},
      {OptFlag::kCypherFlag, ConfigIsSet::kConfigSQL},
      {OptFlag::kSQLFlag, ConfigIsSet::kConfigCypher}
  };

  static bool IsFileExists(const std::string& path);
  void SetFlag(OptFlag flag);
  bool IsFlagSet(OptFlag flag) const;

  void PrintHelp() const;
  void PrintVersion() const;
  void SetOptFlagDaemon(OptFlag flag);
  void SetOptFlagInteractive(OptFlag flag);
  void SetOptFlagLog(OptFlag flag);
  void SetOptFlagMode(OptFlag flag);
  void SetOptFlagSQL(OptFlag flag);
  void SetOptFlagCypher(OptFlag flag);

  void ValidateMode(SCCMode mode) const;
  void ValidateMode(const std::string& mode) const;
  void ValidateSQLPath(const std::string& sql_path) const;
  void ValidateCypherPath(const std::string& cypher_path) const;
  void ValidateIsInputStreamOpen() const;
  void ValidateIsOutputStreamOpen() const;
  void ValidateIsFlagSet(OptFlag flag) const;
};

extern Config config;
