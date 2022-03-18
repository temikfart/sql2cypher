#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <getopt.h>
#include <sys/stat.h>
#include "SCC/log.h"

void Configure(int argc, char* argv[]);

enum SCCMode {
  INTERACTIVE,
  DAEMON,
  SCCMODE_COUNT
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
  ConfLOG,
  ConfMODE,
  ConfSQL,
  ConfCYPHER,
  CONF_COUNT
};

class Config: public Log {
private:
  SCCMode mode_ = SCCMode::INTERACTIVE;
  
  std::map<std::string, SCCMode> str2modes_ = {
    {"INTERACTIVE", SCCMode::INTERACTIVE},
    {"DAEMON",      SCCMode::DAEMON},
  };
  std::map<SCCMode, std::string> modes2str_ = {
    {SCCMode::INTERACTIVE,  "INTERACTIVE"},
    {SCCMode::DAEMON,       "DAEMON"}
  };
  
  std::string sql_path_;
  std::ifstream input_;
  
  std::string cypher_path_;
  std::ofstream output_;

  std::map<ConfigIsSet, bool> is_config_set_ = {
          {ConfigIsSet::ConfLOG,    false},
          {ConfigIsSet::ConfMODE,   false},
          {ConfigIsSet::ConfSQL,    false},
          {ConfigIsSet::ConfCYPHER, false},
  };
  std::map<OptFlag, ConfigIsSet> flag_to_config_ = {
          {OptFlag::fDAEMON,      ConfigIsSet::ConfMODE},
          {OptFlag::fINTERACTIVE, ConfigIsSet::ConfMODE},
          {OptFlag::fLOG,         ConfigIsSet::ConfLOG},
          {OptFlag::fMODE,        ConfigIsSet::ConfMODE},
          {OptFlag::fCYPHER,      ConfigIsSet::ConfSQL},
          {OptFlag::fSQL,         ConfigIsSet::ConfCYPHER}
  };

  std::string GetConfigPath() const;
  static bool IsFileExists(std::string& path);
  static bool IsFileExists(const std::ofstream& f);
  void ValidateMode(SCCMode mode) const;
  void ValidateMode(std::string& mode) const;
  void ValidateSQLPath(std::string& sql_path) const;
  void ValidateCypherPath(std::string& cypher_path) const;
  void ValidateInputFileStream(std::ifstream const& input);
  bool IsFlagSet(OptFlag flag) const;
  void ValidateSetFlag(OptFlag flag) const;
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
  std::ifstream& ReadSQL();
  char GetSQLSymb();
  char PeekSQLSymb();
  std::ofstream& WriteCypher();
  SCCMode StringToSCCMode(std::string& mode) const;
  std::string SCCModeToString(SCCMode mode) const;
  void GetConsoleArguments(int argc, char* const* argv);
  ~Config();
};

extern Config config;
