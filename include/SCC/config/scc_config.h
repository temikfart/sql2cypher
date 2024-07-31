#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

#include "logger/severity.hpp"

#include "SCC/common/file_utils.h"
#include "SCC/common/singleton.h"
#include "SCC/config/scc_args.h"
#include "SCC/config/scc_mode.h"
#include "SCC/log/log.h"

void end(int exit_code);

namespace scc::config {

class SCCConfig : public common::Singleton<SCCConfig> {
public:
  logger::Severity log_severity;
  fs::path log_directory;
  SCCMode mode;
  bool translate_schema;
  bool translate_data;

  explicit SCCConfig(SCCArgs& args);

  static SCCConfig* Get();

  const std::filesystem::path& get_sql_schema_file() const;
  const std::filesystem::path& get_graph_schema_file() const;
  const std::filesystem::path& get_sql_file() const;
  const std::filesystem::path& get_cypher_file() const;
  const std::filesystem::path& get_ast_dump_file() const;

private:
  std::filesystem::path sql_schema_file_;
  std::filesystem::path graph_schema_file_;
  std::filesystem::path sql_file_;
  std::filesystem::path cypher_file_;
  std::filesystem::path ast_dump_file_;
};

void InitConfig(SCCArgs& args);

} // scc::config
