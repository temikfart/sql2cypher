#pragma once

#include <sys/stat.h>
#include <getopt.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "SCC/log.h"
#include "scc_mode.h"
#include "args.h"

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

namespace fs = std::filesystem;

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

  void set_sql_file(const fs::path& new_sql_file);
  const fs::path& sql_file() const;
  void set_cypher_file(const fs::path& new_cypher_file);
  const fs::path& cypher_file() const;
  void set_ast_dump_file(const fs::path& new_ast_dump_file);
  const fs::path& ast_dump_file() const;

  void Start(int argc, char* argv[]);
  void GetConsoleArguments(int argc, char* const* argv);

  static std::string GetConfigPath();
  char GetSQLSymbol();
  char PeekSQLSymbol();
  std::ofstream& WriteCypher();
  std::ofstream& WriteTreeDump();
  bool CloseInputFile();
  bool CloseOutputFile();
  bool CloseTreeDumpFile();

public:
  logger::Severity log_severity = logger::info;
  std::string log_directory = SCC_LOG_DIR;

  SCCMode mode = SCCMode::kInteractive;
  bool need_ast_dump = false;
private:
  std::filesystem::path sql_file_;
  std::filesystem::path cypher_file_;
  std::filesystem::path ast_dump_file_;

  std::ifstream input_;
  std::ofstream output_;
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

  static void ValidateFileExists(const fs::path& path);
  void ValidateIsInputStreamOpen() const;
  void ValidateIsOutputStreamOpen() const;
  void ValidateIsTreeDumpStreamOpen() const;
  void ValidateIsFlagSet(OptFlag flag) const;
};

extern Config config;
