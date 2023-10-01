#include "SCC/config/scc_config.h"

namespace scc {

SCCConfig* SCCConfig::Get() {
  auto *instance = get_instance();
  if (instance == nullptr)
    std::cerr << "SCC: ERROR! Could not start: the configuration is not initialized." << std::endl;
  return instance;
}

void SCCConfig::SetParamsFrom(const SCCArgs& args) {
  log_severity = args.Get<logger::Severity>("--log-severity");
  const std::string& dir = args.Get("--log-directory");
  log_directory = fs::weakly_canonical(dir);
  mode = SCCMode(args.Get<SCCMode>("--mode"));

  sql_file_ = fs::canonical(args.Get("--sql"));
  cypher_file_ = fs::weakly_canonical(args.Get("--cypher"));
}

const fs::path& SCCConfig::get_sql_file() const {
  return sql_file_;
}
const fs::path& SCCConfig::get_cypher_file() const {
  return cypher_file_;
}

SCCConfig& InitConfig(const SCCArgs& args) {
  static SCCConfig scc_config;
  scc_config.SetParamsFrom(args);
  return scc_config;
}

} // scc
