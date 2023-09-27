#include "SCC/config/args.h"

SCCArgs::SCCArgs() : ArgumentParser(PROGRAM_NAME, VERSION, default_arguments::none) {
  add_description("Translates SQL queries for MS SQL Server into queries for Neo4j DBMS.");
  add_epilog("Contribute to SCC: " CONTRIBUTE_LINK);

  add_argument("-h", "--help")
      .action([this](const std::string& /*unused*/) {
        PrintHelpAndExit();
      })
      .help("Show this message")
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

void SCCArgs::ParseArgs(int argc, char* argv[]) {
  if (argc == 1)
    PrintHelpAndExit();

  parse_args(argc, argv);
}

void SCCArgs::PrintHelpAndExit() const {
  std::cout << help().str();
  exit(EXIT_SUCCESS);
}
