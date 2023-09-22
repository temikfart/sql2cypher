#include "SCC/config/config.h"

Config config;

void end(int exit_code) {
  bool is_all_files_closed = true;
  if (!config.CloseInputFile()) {
    std::cerr << "closing input file error" << std::endl;
    is_all_files_closed = false;
  }
  if (!config.CloseOutputFile()) {
    std::cerr << "closing output file error" << std::endl;
    is_all_files_closed = false;
  }
  if (!config.CloseTreeDumpFile()) {
    std::cerr << "closing tree dump file error" << std::endl;
    is_all_files_closed = false;
  }
  if (!is_all_files_closed) {
    std::cerr << "some files was not closed" << std::endl;
    exit_code = EXIT_FAILURE;
  }

  exit(exit_code);
}

Config::Config() {
#ifndef CREATE_PACKAGE
  std::string config_path = Config::GetConfigPath();
  sql_file_ = config_path + "../../resources/sql_queries.sql";
  cypher_file_ = config_path + "../../resources/cypher_queries.cypher";
  ast_dump_file_ = config_path + "../../resources/tree_dump/tree_dump.txt";
#endif // CREATE_PACKAGE
}

void Config::set_sql_file(const fs::path& new_sql_file) {
  ValidateFileExists(new_sql_file);
  sql_file_ = new_sql_file;
}
const fs::path& Config::sql_file() const {
  return sql_file_;
}
void Config::set_cypher_file(const fs::path& new_cypher_file) {
  cypher_file_ = fs::weakly_canonical(new_cypher_file);
}
const fs::path& Config::cypher_file() const {
  return cypher_file_;
}
void Config::set_ast_dump_file(const fs::path& new_ast_dump_file) {
  ast_dump_file_ = fs::weakly_canonical(new_ast_dump_file);
}
const fs::path& Config::ast_dump_file() const {
  return ast_dump_file_;
}

void Config::Start(int argc, char* argv[]) {
  if (argc > 1) {
    config.GetConsoleArguments(argc, argv);
  } else {
    // TODO: print help if no arguments (tests failed, if printed)
#ifdef CREATE_PACKAGE
    this->PrintHelp();
#endif // CREATE_PACKAGE
    // TODO: Parse config.ini
  }

#ifdef CREATE_PACKAGE
  if (!this->IsFlagSet(OptFlag::kSQLFlag)) {
    std::cerr << "expected path to input SQL queries "
               "(the file must exist)" << std::endl;
    SCC_log.LoadBufferedLogs();
    end(EXIT_FAILURE);
  }
  if (!this->IsFlagSet(OptFlag::kCypherFlag)) {
    std::cerr << "expected path to output CypherQL file "
               "(the file will be created, if not exists)" << std::endl;
    SCC_log.LoadBufferedLogs();
    end(EXIT_FAILURE);
  }
#endif // CREATE_PACKAGE

  input_.open(sql_file_, std::ios::in);
  this->ValidateIsInputStreamOpen();

  output_.open(cypher_file_, std::ios::out);
  ValidateFileExists(cypher_file_);
  this->ValidateIsOutputStreamOpen();

  if (need_ast_dump) {
    tree_dump_.open(ast_dump_file_, std::ios::out);
    ValidateFileExists(ast_dump_file_);
    this->ValidateIsTreeDumpStreamOpen();
  }

  if (mode == SCCMode::kDaemon) {
    // TODO: implement SCC mode=daemon behavior.
  }
}
void Config::GetConsoleArguments(int argc, char* const* argv) {
  int flag;
  opterr = 0;

  const char short_options[] = "dhil:L:m:v";
  const struct option long_options[] = {
      // Has the short form
      {"daemon", 0, nullptr, OptFlag::kDaemonFlag},
      {"dump", required_argument, nullptr, OptFlag::kTreeDumpFlag},
      {"help", 0, nullptr, OptFlag::kHelpFlag},
      {"interactive", 0, nullptr, OptFlag::kInteractiveFlag},
      {"loglvl", required_argument, nullptr, OptFlag::kLogLvlFlag},
      {"logdir", required_argument, nullptr, OptFlag::kLogDirFlag},
      {"mode", required_argument, nullptr, OptFlag::kModeFlag},
      {"version", 0, nullptr, OptFlag::kVersionFlag},
      // Does not have the short form
      {"cypher", required_argument, nullptr, OptFlag::kCypherFlag},
      {"sql", required_argument, nullptr, OptFlag::kSQLFlag},
      {nullptr, 0, nullptr, 0},
  };

  std::map<char, bool> has_argument;
  for (int i = 0; long_options[i].name != nullptr; i++) {
    has_argument[(char) (long_options[i].val)] = long_options[i].has_arg;
  }

  while ((flag = getopt_long(argc,
                             argv,
                             short_options,
                             long_options,
                             nullptr)) != -1) {
    auto OF_flag = static_cast<OptFlag>(flag);
    switch (flag) {
      case OptFlag::kDaemonFlag:
        this->SetOptFlagDaemon(OF_flag);
        break;
      case OptFlag::kHelpFlag:
        this->PrintHelp();
        break;
      case OptFlag::kVersionFlag:
        this->PrintVersion();
        break;
      case OptFlag::kInteractiveFlag:
        this->SetOptFlagInteractive(OF_flag);
        break;
      case OptFlag::kLogLvlFlag:
        this->SetOptFlagLogLvl(OF_flag);
        break;
      case OptFlag::kLogDirFlag:
        this->SetOptFlagLogDir(OF_flag);
        break;
      case OptFlag::kModeFlag:
        this->SetOptFlagMode(OF_flag);
        break;
      case OptFlag::kSQLFlag:
        this->SetOptFlagSQL(OF_flag);
        break;
      case OptFlag::kCypherFlag:
        this->SetOptFlagCypher(OF_flag);
        break;
      case OptFlag::kTreeDumpFlag:
        this->SetOptFlagTreeDump(OF_flag);
        break;
      default:
        // TODO: change the conditions (use optind, now it's used incorrectly)
//        auto arg = argv[optind - 1];
//        if (has_argument.count(arg[1]) != 0
//            && has_argument.at(arg[1]) == 1) {
//          std::cerr << "flag \'-" << (char)(arg[1]) << "\' "
//                                                     "required an argument" << std::endl;
//        } else {
//          std::cerr << "invalid flag \'-" << (char)(arg[1]) << "\'" << std::endl;
//        }
        std::cerr << "invalid flag, see help" << std::endl;
        end(EXIT_FAILURE);
    }
  }
}

std::string Config::GetConfigPath() {
  std::string cwf_path = __FILE__;
  std::string cwf = cwf_path.substr(cwf_path.find_last_of('/') + 1);
  std::string path = cwf_path.substr(0, cwf_path.find(cwf));

  return path;
}
char Config::GetSQLSymbol() {
  this->ValidateIsInputStreamOpen();
  return (char) input_.get();
}
char Config::PeekSQLSymbol() {
  this->ValidateIsInputStreamOpen();
  return (char) input_.peek();
}
std::ofstream& Config::WriteCypher() {
  this->ValidateIsOutputStreamOpen();
  return output_;
}
std::ofstream& Config::WriteTreeDump() {
  this->ValidateIsTreeDumpStreamOpen();
  return tree_dump_;
}
bool Config::CloseInputFile() {
  if (input_.is_open()) {
    input_.close();
    if (input_.good() || input_.eof()) {
      ;
    } else {
      std::cerr << "input file close error" << std::endl;
      return false;
    }
  }
  return true;
}
bool Config::CloseOutputFile() {
  if (output_.is_open()) {
    output_.close();
    if (output_.good()) {
      ;
    } else {
      std::cerr << "output file close error" << std::endl;
      return false;
    }
  }
  return true;
}
bool Config::CloseTreeDumpFile() {
  if (tree_dump_.is_open()) {
    tree_dump_.close();
    if (tree_dump_.good()) {
      ;
    } else {
      std::cerr << "dot dump file close error" << std::endl;
      return false;
    }
  }
  return true;
}

void Config::SetFlag(OptFlag flag) {
  is_config_set_[flag_to_config_.at(flag)] = true;
}
bool Config::IsFlagSet(OptFlag flag) const {
  return is_config_set_.at(flag_to_config_.at(flag));
}

void Config::PrintHelp() {
  std::cout << "Usage:\n";
  std::cout << "\tscc --sql=file1 --cypher=file2 [options]...\n";
  std::cout << "Options:\n";
  std::cout << std::left;
  std::cout << "  " << std::setw(20) << "-h, --help"
            << "Prints Usage and information about acceptable flags.\n";
  std::cout << "  " << std::setw(20) << "-v, --version"
            << "Prints current version.\n";
  std::cout << "  " << std::setw(20) << "-m, --mode=[mode]"
            << "Starts the SCC in special mode (by default: interactive)\n";
  std::cout << "  " << std::setw(20) << "-i, --interactive"
            << "Starts the SCC in interactive mode.\n";
//  std::cout << "  " << std::setw(20) << ""
//            << "(logs will be printed in console)\n";
  // TODO: uncomment when daemon mode will be implemented
//  std::cout << "  " << std::setw(20) << "-d, --daemon"
//            << "Starts the SCC in daemon mode.\n";
//  std::cout << "  " << std::setw(20) << ""
//            << "(logs will be printed in the special log files "
//               "into \"log/\")\n";
  std::cout << "  " << std::setw(20) << "--dump=[path]"
            << "Creates Tree Dump image of the AST in the [path].\n";
  std::cout << "  " << std::setw(20) << "-l, --loglvl=lvl"
            << "Sets logging level to \"lvl\".\n";
  std::cout << "  " << std::setw(20) << ""
            << "Acceptable levels: SILENT, FATAL, "
               "ERROR, INFO, TRACE, DEBUG.\n";
  std::cout << "  " << std::setw(20) << "--logdir=[path]"
            << "Path to the log directory, where will be created log file\n";
  std::cout << "  " << std::setw(20) << "--sql=[path]"
            << "Path to the file with SQL queries, "
               "which will be converted (SQL).\n";
  std::cout << "  " << std::setw(20) << "--cypher=[path]"
            << "Path to the file, "
               "where will be converted queries (CQL).\n";
  std::cout << "  " << std::setw(20) << "--dump=[path]"
            << "Path to the file, "
               "where will be created image of AST of SQL queries \n";
  std::cout << "  " << std::setw(20) << ""
            << "(by default, the image is not created)\n";
  std::cout.flush();

  end(EXIT_SUCCESS);
}
void Config::PrintVersion() {
  std::cout << std::left;
  std::cout << "<===| SCC (The SQL to CypherQL Converter) |===>\n";
  std::cout << std::setw(13) << "Version:" << VERSION << "\n";
  std::cout << std::setw(13) << "Developers:" << DEVELOPERS;
  std::cout << std::endl;

  end(EXIT_SUCCESS);
}
void Config::SetOptFlagDaemon(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  mode = SCCMode::kDaemon;
  this->SetFlag(flag);
}
void Config::SetOptFlagInteractive(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  mode = SCCMode::kInteractive;
  this->SetFlag(flag);
}
void Config::SetOptFlagLogLvl(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string severity = optarg;
  log_severity = SCCLogger::to_severity(severity);
  this->SetFlag(flag);
}
void Config::SetOptFlagLogDir(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string dir = optarg;
  log_directory = dir;
  this->SetFlag(flag);
}
void Config::SetOptFlagMode(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string tmp_mode = optarg;
  mode = SCCMode(tmp_mode);
  this->SetFlag(flag);
}
void Config::SetOptFlagSQL(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string path = optarg;
  this->set_sql_file(path);
  this->SetFlag(flag);
}
void Config::SetOptFlagCypher(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string tmp = optarg;
  this->set_cypher_file(tmp);
  this->SetFlag(flag);
}
void Config::SetOptFlagTreeDump(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string tmp = optarg;
  need_ast_dump = true;
  this->set_ast_dump_file(tmp);
  this->SetFlag(flag);
}

void Config::ValidateFileExists(const fs::path& path) {
  if (!fs::exists(path)) {
    std::cerr << "file does not exist: " << path << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsInputStreamOpen() const {
  if (!input_.is_open()) {
    std::cerr << "input file stream is not opened" << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsOutputStreamOpen() const {
  if (!output_.is_open()) {
    std::cerr << "output file stream is not opened" << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsTreeDumpStreamOpen() const {
  if (!tree_dump_.is_open()) {
    std::cerr << "dump file stream is not opened" << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsFlagSet(OptFlag flag) const {
  if (this->IsFlagSet(flag)) {
    std::cerr << "conflicting flags" << std::endl;
    end(EXIT_FAILURE);
  }
}
