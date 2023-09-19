#pragma once

#include <sys/stat.h>
#include <getopt.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "SCC/log.h"

#ifdef SCC_MAINTAINERS
#define DEVELOPERS SCC_MAINTAINERS
#else
#define DEVELOPERS "Artyom Fartygin, Roman Korostinkiy"
#endif // SCC_MAINTAINERS

#ifdef SCC_VERSION
#define VERSION SCC_VERSION
#else
#define VERSION "0.9.0-rc"
#endif // SCC_VERSION

enum SCCMode {
  kInteractive,
  kDaemon,

  kSCCModeCount
};

enum OptFlag {
  kDaemonFlag = 'd',
  kHelpFlag = 'h',
  kInteractiveFlag = 'i',
  kLogLvlFlag = 'l',
  kLogDirFlag = 'L',
  kModeFlag = 'm',
  kVersionFlag = 'v',
  kCypherFlag = 'z' + 1,
  kSQLFlag = 'z' + 2,
  kTreeDumpFlag = 'z' + 3
};

enum ConfigIsSet {
  kConfigLogLvl,
  kConfigLogDir,
  kConfigMode,
  kConfigSQL,
  kConfigCypher,
  kConfigTreeDump
};

void end(int exit_code);

class Config {
public:
  Config();

  void set_mode(SCCMode mode);
  SCCMode get_mode() const;
  void set_sql_path(const std::string& new_sql_path);
  std::string get_sql_path() const;
  void set_cypher_path(const std::string& new_cypher_path);
  std::string get_cypher_path() const;
  void set_is_need_dump(bool value);
  bool get_is_need_dump() const;
  void set_is_silent_print(bool value);
  bool get_is_silent_print() const;
  void set_tree_dump_path(const std::string& new_tree_dump_path);
  std::string get_tree_dump_path() const;

  void Start(int argc, char* argv[]);
  void GetConsoleArguments(int argc, char* const* argv);

  static std::string GetConfigPath();
  char GetSQLSymbol();
  char PeekSQLSymbol();
  std::ifstream& ReadSQL();
  std::ofstream& WriteCypher();
  std::ofstream& WriteTreeDump();
  bool CloseInputFile();
  bool CloseOutputFile();
  bool CloseTreeDumpFile();

  SCCMode StringToSCCMode(std::string mode) const;
  std::string SCCModeToString(SCCMode mode) const;

public:
  logger::Severity log_severity = logger::info;
  std::string log_directory = SCC_LOG_DIR;

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

  bool is_silent_print_ = false;
  
  bool is_need_dump_ = false;
  std::string tree_dump_path_;
  std::ofstream tree_dump_;

  std::map<ConfigIsSet, bool> is_config_set_ = {
      {ConfigIsSet::kConfigLogLvl, false},
      {ConfigIsSet::kConfigLogDir, false},
      {ConfigIsSet::kConfigMode, false},
      {ConfigIsSet::kConfigSQL, false},
      {ConfigIsSet::kConfigCypher, false},
      {ConfigIsSet::kConfigTreeDump, false}
  };
  const std::map<OptFlag, ConfigIsSet> flag_to_config_ = {
      {OptFlag::kDaemonFlag, ConfigIsSet::kConfigMode},
      {OptFlag::kInteractiveFlag, ConfigIsSet::kConfigMode},
      {OptFlag::kLogLvlFlag, ConfigIsSet::kConfigLogLvl},
      {OptFlag::kLogDirFlag, ConfigIsSet::kConfigLogDir},
      {OptFlag::kModeFlag, ConfigIsSet::kConfigMode},
      {OptFlag::kCypherFlag, ConfigIsSet::kConfigCypher},
      {OptFlag::kSQLFlag, ConfigIsSet::kConfigSQL},
      {OptFlag::kTreeDumpFlag, ConfigIsSet::kConfigTreeDump}
  };

  static bool IsFileExists(const std::string& path);
  void SetFlag(OptFlag flag);
  bool IsFlagSet(OptFlag flag) const;

  void PrintHelp();
  void PrintVersion();
  void SetOptFlagDaemon(OptFlag flag);
  void SetOptFlagInteractive(OptFlag flag);
  void SetOptFlagLogLvl(OptFlag flag);
  void SetOptFlagLogDir(OptFlag flag);
  void SetOptFlagMode(OptFlag flag);
  void SetOptFlagSQL(OptFlag flag);
  void SetOptFlagCypher(OptFlag flag);
  void SetOptFlagTreeDump(OptFlag flag);

  void ValidateMode(SCCMode mode) const;
  void ValidateMode(const std::string& mode) const;
  void ValidateSQLPath(const std::string& sql_path) const;
  void ValidateCypherPath() const;
  void ValidateTreeDumpPath() const;
  void ValidateIsInputStreamOpen() const;
  void ValidateIsOutputStreamOpen() const;
  void ValidateIsTreeDumpStreamOpen() const;
  void ValidateIsFlagSet(OptFlag flag) const;
};

extern Config config;
