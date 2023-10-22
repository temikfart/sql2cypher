#include "SCC/config/scc_config.h"

void end(int exit_code) {
  exit(exit_code);
}

namespace {

inline void ThrowFailedConfigCreation(const std::exception& e) {
  throw std::runtime_error("Failed to create configuration: " + std::string(e.what()));
}

}

namespace scc {

namespace fs = std::filesystem;

SCCConfig::SCCConfig(const SCCArgs& args) {
  try {
    log_severity = args.Get<logger::Severity>("--log-severity");
    log_directory = fs::weakly_canonical(args.Get("--log-directory"));
    mode = args.Get<SCCMode>("--mode");

    std::string sql_file_path = args.Get("--sql");
    common::ValidateFileExists(sql_file_path);
    sql_file_ = fs::canonical(sql_file_path);
    cypher_file_ = fs::weakly_canonical(args.Get("--cypher"));
    if (args.IsUsed("--dump"))
      ast_dump_file_ = fs::weakly_canonical(args.Get("--dump"));
  } catch (const std::logic_error& e) {
    ThrowFailedConfigCreation(e);
  } catch (const std::runtime_error& e) {
    ThrowFailedConfigCreation(e);
  }
}

SCCConfig* SCCConfig::Get() {
  auto* instance = get_instance();
  if (instance == nullptr)
    throw std::runtime_error("The configuration has not been initialized yet");
  return instance;
}

const fs::path& SCCConfig::get_sql_file() const {
  return sql_file_;
}
const fs::path& SCCConfig::get_cypher_file() const {
  return cypher_file_;
}
const fs::path& SCCConfig::get_ast_dump_file() const {
  return ast_dump_file_;
}

SCCConfig& InitConfig(const SCCArgs& args) {
  static SCCConfig scc_config(args);
  return scc_config;
}

} // scc
