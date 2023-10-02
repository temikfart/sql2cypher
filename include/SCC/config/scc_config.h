#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

#include "logger/severity.hpp"

#include "SCC/common/singleton.h"
#include "SCC/config/args.h"
#include "SCC/config/scc_mode.h"
#include "SCC/log.h"

void end(int exit_code);

namespace scc {

namespace fs = std::filesystem;

class SCCConfig : public common::Singleton<SCCConfig> {
public:
  logger::Severity log_severity;
  fs::path log_directory;
  SCCMode mode;

  static SCCConfig* Get();

  void SetParamsFrom(const SCCArgs& args);

  const fs::path& get_sql_file() const;
  const fs::path& get_cypher_file() const;
  const fs::path& get_ast_dump_file() const;

private:
  fs::path sql_file_;
  fs::path cypher_file_;
  fs::path ast_dump_file_;
};

SCCConfig& InitConfig(const SCCArgs& args);

} // scc
