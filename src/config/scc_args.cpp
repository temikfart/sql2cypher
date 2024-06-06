#include "SCC/config/scc_args.h"

namespace scc::config {

no_argument_error::no_argument_error(const std::string& message)
    : std::runtime_error(message) {}

SCCArgs::SCCArgs() : ArgumentParser(PROGRAM_NAME, VERSION, argparse::default_arguments::none) {
  set_assign_chars("= ");

  add_description("Translates SQL queries for MS SQL Server into queries for Neo4j DBMS.");
  add_epilog("Contribute to SCC: " CONTRIBUTE_LINK);

  add_argument("-h", "--help")
      .action([this](const std::string& /*unused*/) {
        PrintHelpAndExit(EXIT_SUCCESS);
      })
      .help("Show this info")
      .default_value(false)
      .implicit_value(true)
      .nargs(0);

  add_argument("-v", "--version")
      .action([](const std::string& /*unused*/) {
        std::cout << "scc version " VERSION << std::endl;
        exit(EXIT_SUCCESS);
      })
      .help("Show version info")
      .default_value(false)
      .implicit_value(true)
      .nargs(0);

  add_argument("--translate-schema")
      .help("Use this option to translate SQL schema migration queries into Cypher")
      .default_value(false)
      .implicit_value(true);

  add_argument("--sql-schema")
      .help("Specify path to the file with SQL schema migration queries to be converted")
      .metavar("FILENAME");

  std::string default_graph_schema_file =
      (std::filesystem::current_path() / "schema.json").string();
  add_argument("--graph-schema")
      .help("Specify path to the file with Neo4j graph schema")
      .metavar("FILENAME")
      .default_value(default_graph_schema_file);

  add_argument("--translate-data")
      .help("Use this option to translate SQL data migration queries into Cypher")
      .default_value(false)
      .implicit_value(true);

  add_argument("--sql")
      .help("Specify path to the file with SQL data migration queries to be converted")
      .metavar("FILENAME");

  std::string default_cypher_file = (std::filesystem::current_path() / "out.cql").string();
  add_argument("--cypher")
      .help("Specify path to the file with the result CypherQL queries")
      .metavar("FILENAME")
      .default_value(default_cypher_file);

  add_argument("--dump")
      .help("Specify path to the PNG image with the dump of SQL AST")
      .metavar("FILENAME");

  add_argument("-m", "--mode")
      .help("Run the SCC in special mode: interactive/daemon")
      .metavar("MODE")
      .default_value(SCCMode(SCCMode::kInteractive));

  add_argument("-d", "--daemon")
      .help("Run the SCC as daemon")
      .default_value(false)
      .implicit_value(true);

  add_argument("-l", "--log-severity")
      .help("Specify severity of logging. Acceptable values: silent/fatal/error/info/debug/trace")
      .metavar("SEVERITY")
      .default_value(logger::Severity::info);

  add_argument("--log-directory")
      .help("Specify path to the directory with log files")
      .metavar("DIRNAME")
      .default_value(std::string(SCC_LOG_DIR));
}

void SCCArgs::ParseArgs(int argc, const char* const argv[]) {
  if (argc == 1)
    PrintHelpAndExit(EXIT_SUCCESS);

  try {
    parse_args(argc, argv);
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    PrintHelpAndExit(EXIT_FAILURE);
  }
}

bool SCCArgs::IsUsed(const std::string& arg_name) const {
  return is_used(arg_name);
}

void SCCArgs::PrintHelpAndExit(int exit_code) const {
  std::cout << help().str();
  exit(exit_code);
}

} // scc::config
