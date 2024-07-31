#include "SCC/config/scc_args.h"

namespace scc::config {

using namespace argparse;

no_argument_error::no_argument_error(const std::string& message)
    : std::runtime_error(message) {}

SCCArgumentParser::SCCArgumentParser(std::string_view program_name)
    : ArgumentParser(std::string(program_name), VERSION, default_arguments::none) {
  set_assign_chars("= ");
}

SCCArgumentParser& SCCArgumentParser::subparser(std::string_view name) {
  auto& subparser = at<ArgumentParser>(name);
  return static_cast<SCCArgumentParser&>(subparser);
}

bool SCCArgumentParser::IsUsed(const std::string& arg_name) const {
  return is_used(arg_name);
}

SCCArgs::SCCArgs() : SCCArgumentParser(PROGRAM_NAME) {
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

void SCCArgs::PrintHelpAndExit(int exit_code) const {
  std::cout << help().str();
  exit(exit_code);
}

SCCSubcommand::SCCSubcommand(std::string_view name, const std::string& description)
    : SCCArgumentParser(name) {
  add_description(description);

  add_argument("-h", "--help")
      .action([this](const std::string& /*unused*/) {
        std::cout << help().str();
        exit(EXIT_SUCCESS);
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
}

SCCSubcommandSchema::SCCSubcommandSchema()
    : SCCSubcommand(SCC_SCHEMA_SUBCOMMAND, "Translate SQL schema migration queries into Cypher.") {
  add_argument("--sql")
      .help("Specify path to the file with SQL schema migration queries to be converted")
      .metavar("FILENAME");

  std::string default_graph_schema_file =
      (std::filesystem::current_path() / "schema.json").string();
  add_argument("--graph-schema")
      .help("Specify path to the file with Neo4j graph schema")
      .metavar("FILENAME")
      .default_value(default_graph_schema_file);
}

SCCSubcommandData::SCCSubcommandData()
    : SCCSubcommand(SCC_DATA_SUBCOMMAND, "Translate SQL data migration queries into Cypher.") {
  add_argument("--graph-schema")
      .help("Specify path to the file with Neo4j graph schema")
      .metavar("FILENAME");

  add_argument("--sql")
      .help("Specify path to the file with SQL data migration queries to be converted")
      .metavar("FILENAME");

  std::string default_cypher_file = (std::filesystem::current_path() / "out.cql").string();
  add_argument("--cypher")
      .help("Specify path to the file with the result CypherQL queries")
      .metavar("FILENAME")
      .default_value(default_cypher_file);
}

} // scc::config
