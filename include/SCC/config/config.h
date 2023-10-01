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
#include "SCC/common/singleton.h"
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

void end(int exit_code);

class Config {
public:
  logger::Severity log_severity = logger::info;
  std::string log_directory = SCC_LOG_DIR;

  SCCMode mode = SCCMode::kInteractive;
  bool need_ast_dump = false;

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

private:
  std::filesystem::path sql_file_;
  std::filesystem::path cypher_file_;
  std::filesystem::path ast_dump_file_;

  std::ifstream input_;
  std::ofstream output_;
  std::ofstream tree_dump_;

  static void ValidateFileExists(const fs::path& path);
  void ValidateIsInputStreamOpen() const;
  void ValidateIsOutputStreamOpen() const;
  void ValidateIsTreeDumpStreamOpen() const;
};

extern Config config;
