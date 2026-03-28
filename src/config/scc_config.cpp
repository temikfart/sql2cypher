#include "SCC/config/scc_config.h"

void end(int exit_code) {
  exit(exit_code);
}

namespace {

inline void ThrowFailedConfigCreation(const std::exception& e) {
  throw std::runtime_error("Failed to create configuration: " + std::string(e.what()));
}

}

namespace scc::config {

namespace fs = std::filesystem;

SCCConfig::SCCConfig(SCCArgs& args) {
  try {
    log_severity = args.IsUsed("--log-severity")
                   ? logger::to_severity(args.Get("--log-severity"))
                   : args.Get<logger::Severity>("--log-severity");
    log_directory = fs::weakly_canonical(args.Get("--log-directory"));

    mode = args.IsUsed("--mode")
           ? SCCMode(args.Get("--mode"))
           : args.Get<SCCMode>("--mode");

    if (args.IsUsed("--dump")) {
      ast_dump_file_ = fs::weakly_canonical(args.Get("--dump"));
    }

    if (args.is_subcommand_used(SCC_SCHEMA_SUBCOMMAND)) {
      translate_schema = true;
      SCCArgumentParser& schema_subparser = args.subparser(SCC_SCHEMA_SUBCOMMAND);

      std::string sql_schema_file_path = schema_subparser.Get("--sql");
      scc::common::ValidateFileExists(sql_schema_file_path);
      sql_schema_file_ = fs::canonical(sql_schema_file_path);

      graph_schema_file_ = fs::weakly_canonical(schema_subparser.Get("--graph-schema"));
    }

    if (args.is_subcommand_used(SCC_DATA_SUBCOMMAND)) {
      translate_data = true;
      SCCArgumentParser& data_subparser = args.subparser(SCC_DATA_SUBCOMMAND);

      std::string graph_schema_file_path = data_subparser.Get("--graph-schema");
      scc::common::ValidateFileExists(graph_schema_file_path);
      graph_schema_file_ = fs::canonical(graph_schema_file_path);

      std::string sql_file_path = data_subparser.Get("--sql");
      scc::common::ValidateFileExists(sql_file_path);
      sql_file_ = fs::canonical(sql_file_path);

      cypher_file_ = fs::weakly_canonical(data_subparser.Get("--cypher"));
    }

    if (!(translate_schema || translate_data)) {
      throw std::logic_error("Either --translate-schema or --translate-data must be specified");
    }
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

const fs::path& SCCConfig::get_sql_schema_file() const {
  return sql_schema_file_;
}
const fs::path& SCCConfig::get_graph_schema_file() const {
  return graph_schema_file_;
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

void InitConfig(SCCArgs& args) {
  static SCCConfig scc_config(args);
}

} // scc::config
