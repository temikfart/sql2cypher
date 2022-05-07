#include "SCC/config.h"

Config config;

void end(int exit_code) {
  LOG(TRACE, "end-function is invoked");
  LOG(TRACE, "closing files...");

  bool is_all_files_closed = true;
  if (!config.CloseInputFile()) {
    LOG(ERROR, "closing input file error");
    is_all_files_closed = false;
  }
  if (!config.CloseOutputFile()) {
    LOG(ERROR, "closing output file error");
    is_all_files_closed = false;
  }
  if (!config.CloseTreeDumpFile()) {
    LOG(ERROR, "closing tree dump file error");
    is_all_files_closed = false;
  }
  if (is_all_files_closed) {
    LOG(TRACE, "all I/O files are closed");
  } else {
    LOG(ERROR, "some files was not closed");
    exit_code = EXIT_FAILURE;
  }

  LogLevel ec_lvl = INFO;
  if (exit_code != 0) {
    ec_lvl = ERROR;
  }
  LOG(ec_lvl, "System ended with exit code " << exit_code);
  exit(exit_code);
}

Config::Config() {
  mode_ = SCCMode::kInteractive;
  sql_path_ = cypher_path_ = tree_dump_path_ = this->GetConfigPath();
  sql_path_ += "../resources/sql_queries.sql";
  cypher_path_ += "../resources/cypher_queries.cypher";
  tree_dump_path_ += "../resources/tree_dump/tree_dump.txt";
}

void Config::set_mode(SCCMode mode) {
  this->ValidateMode(mode);
  mode_ = mode;
}
SCCMode Config::get_mode() const {
  return mode_;
}
void Config::set_sql_path(const std::string& new_sql_path) {
  this->ValidateSQLPath(new_sql_path);
  sql_path_ = new_sql_path;
}
std::string Config::get_sql_path() const {
  return sql_path_;
}
void Config::set_cypher_path(const std::string& new_cypher_path) {
  cypher_path_ = new_cypher_path;
}
std::string Config::get_cypher_path() const {
  return cypher_path_;
}
void Config::set_tree_dump_path(const std::string& new_tree_dump_path) {
  tree_dump_path_ = new_tree_dump_path;
}
std::string Config::get_tree_dump_path() const {
  return tree_dump_path_;
}

void Config::Start(int argc, char* argv[]) {
  LOG(TRACE, "configuring system...");

  if (argc > 1) {
    config.GetConsoleArguments(argc, argv);
  } else {
    // TODO: Parse config.ini
  }

  LOG(TRACE, "opening i/o files...");
  input_.open(sql_path_, std::ios::in);
  this->ValidateIsInputStreamOpen();
  output_.open(cypher_path_, std::ios::out);
  tree_dump_.open(tree_dump_path_, std::ios::out);

  this->ValidateCypherPath(cypher_path_);
  this->ValidateIsOutputStreamOpen();
  this->ValidateTreeDumpPath(tree_dump_path_);
  LOG(TRACE, "all i/o files are opened");

  if (config.get_mode() == SCCMode::kDaemon) {
    LOG(TRACE, "daemon mode cannot be activated "
               "(not implemented, for now)");
    // TODO: implement SCC mode=daemon behavior.
  }

  LOG(TRACE, "configuration is completed");
}
void Config::GetConsoleArguments(int argc, char* const* argv) {
  LOG(TRACE, "parsing console arguments...");

  int flag;
  opterr = 0;

  const char short_options[] = "dhil:m:v";
  const struct option long_options[] = {
      {"cypher", required_argument, nullptr, OptFlag::kCypherFlag},
      {"daemon", 0, nullptr, OptFlag::kDaemonFlag},
      {"dump", required_argument, nullptr, OptFlag::kSQLFlag},
      {"help", 0, nullptr, OptFlag::kHelpFlag},
      {"interactive", 0, nullptr, OptFlag::kInteractiveFlag},
      {"log", required_argument, nullptr, OptFlag::kLogFlag},
      {"mode", required_argument, nullptr, OptFlag::kModeFlag},
      {"sql", required_argument, nullptr, OptFlag::kSQLFlag},
      {"version", 0, nullptr, OptFlag::kVersionFlag},
      {nullptr, 0, nullptr, 0},
  };
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
      case OptFlag::kLogFlag:
        this->SetOptFlagLog(OF_flag);
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
        char invalid_flag = short_options[optind + 1];
        if (short_options[optind + 2] == ':') {
          LOG(ERROR,
              "flag must have an argument");
        } else {
          LOG(ERROR, "undefined flag: " << invalid_flag);
        }
        end(EXIT_FAILURE);
    }
  }
  LOG(DEBUG, "all console arguments are parsed");
}

std::string Config::GetConfigPath() const {
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
std::ifstream& Config::ReadSQL() {
  this->ValidateIsInputStreamOpen();
  return input_;
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
  LOG(TRACE, "closing input file...");
  if (input_.is_open()) {
    input_.close();
    if (input_.good() || input_.eof()) {
      LOG(TRACE, "input file closed successfully");
    } else {
      LOG(ERROR, "input file close error");
      return false;
    }
  } else {
    LOG(TRACE, "input file is already closed");
  }
  return true;
}
bool Config::CloseOutputFile() {
  LOG(TRACE, "closing output file...");
  if (output_.is_open()) {
    output_.close();
    if (output_.good()) {
      LOG(TRACE, "output file closed successfully");
    } else {
      LOG(ERROR, "output file close error");
      return false;
    }
  } else {
    LOG(TRACE, "output file is already closed");
  }
  return true;
}
bool Config::CloseTreeDumpFile() {
  LOG(TRACE, "closing tree dump file...");
  if (tree_dump_.is_open()) {
    tree_dump_.close();
    if (tree_dump_.good()) {
      LOG(TRACE, "dot tree dump file closed successfully");
    } else {
      LOG(ERROR, "dot dump file close error");
      return false;
    }
  } else {
    LOG(TRACE, "dot tree dump file is already closed");
  }
  return true;
}

SCCMode Config::StringToSCCMode(std::string mode) const {
  for_each(begin(mode), end(mode),
           [](char& c) {
             c = (char) ::toupper(c);
           });
  this->ValidateMode(mode);
  return str2modes_.at(mode);
}
std::string Config::SCCModeToString(SCCMode mode) const {
  this->ValidateMode(mode);
  return modes2str_.at(mode);
}

bool Config::IsFileExists(const std::string& path) {
  struct stat buffer{};
  return (stat(path.c_str(), &buffer) == 0);
}
void Config::SetFlag(OptFlag flag) {
  is_config_set_[flag_to_config_.at(flag)] = true;
}
bool Config::IsFlagSet(OptFlag flag) const {
  return is_config_set_.at(flag_to_config_.at(flag));
}

void Config::PrintHelp() const {
  std::cout << std::left;
  std::cout << std::setw(20) << "-h, --help"
            << "Prints Usage and information about acceptable flags.\n";
  std::cout << std::setw(20) << "-v, --version"
            << "Prints current version.\n";
  std::cout << std::setw(20) << "-m, --mode=[mode]"
            << "Starts the SCC in special mode (by default: interactive)\n";
  std::cout << std::setw(20) << "-i, --interactive"
            << "Starts the SCC in interactive mode.\n";
  std::cout << std::setw(20) << ""
            << "(logs will be printed in console)\n";
  // TODO: uncomment when daemon mode will be implemented
//  std::cout << std::setw(20) << "-d, --daemon"
//            << "Starts the SCC in daemon mode.\n";
  std::cout << std::setw(20) << ""
            << "(logs will be printed in the special log files "
               "into \"log/\")\n";
  std::cout << std::setw(20) << "--log=lvl"
            << "Sets logging level to \"lvl\".\n";
  std::cout << std::setw(20) << ""
            << "Acceptable levels: SILENT, FATAL, "
               "ERROR, INFO, TRACE, DEBUG.\n";
  std::cout << std::setw(20) << "--sql=[path]"
            << "Path to the file with SQL queries, "
               "which will be converted (SQL).\n";
  std::cout << std::setw(20) << "--cypher=[path]"
            << "Path to the file, "
               "where will be converted queries (CQL).\n";
  std::cout << std::setw(20) << "--dump=[path]"
            << "Path to the file, "
               "where will be created image of AST of SQL queries\n";
  std::cout.flush();

  end(EXIT_SUCCESS);
}
void Config::PrintVersion() const {
  LOG(INFO, "printing version of the SCC...");

  std::cout << std::left;
  std::cout << "<===| SCC (The SQL to CypherQL Converter) |===>\n";
  std::cout << std::setw(13) << "Version:" << "rc-0.9\n";
  std::cout << std::setw(13) << "Developers:"
            << "Artyom Fartygin and Roman Korostinskiy";
  std::cout << std::endl;

  end(EXIT_SUCCESS);
}
void Config::SetOptFlagDaemon(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set SCC mode=DAEMON");
  this->set_mode(SCCMode::kDaemon);
  this->SetFlag(flag);
}
void Config::SetOptFlagInteractive(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set SCC mode=INTERACTIVE");
  this->set_mode(SCCMode::kInteractive);
  this->SetFlag(flag);
}
void Config::SetOptFlagLog(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set LogLevel=" << optarg);
  std::string tmp_log_level = optarg;
  SCC_log.set_log_level(SCC_log.StringToLogLevel(tmp_log_level));
  this->SetFlag(flag);
}
void Config::SetOptFlagMode(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set SCC mode=" << optarg);
  std::string tmp_mode = optarg;
  this->set_mode(this->StringToSCCMode(tmp_mode));
  this->SetFlag(flag);
}
void Config::SetOptFlagSQL(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set file with SQL queries as \'" << optarg << "\'");
  std::string tmp = optarg;
  this->set_sql_path(tmp);
  this->SetFlag(flag);
}
void Config::SetOptFlagCypher(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set file with CypherQL queries as \'" << optarg << "\'");
  std::string tmp = optarg;
  this->set_cypher_path(tmp);
  this->SetFlag(flag);
}
void Config::SetOptFlagTreeDump(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  LOG(TRACE, "set Tree Dump path = " << optarg);
  std::string tmp = optarg;
  this->set_tree_dump_path(tmp);
  this->SetFlag(flag);
}

void Config::ValidateMode(SCCMode mode) const {
  if (SCCMode::kSCCModeCount <= mode) {
    LOG(ERROR, "incorrect SCC mode: " << mode);
    end(EXIT_FAILURE);
  }
}
void Config::ValidateMode(const std::string& mode) const {
  if (str2modes_.count(mode) == 0) {
    LOG(ERROR, "invalid SCC mode: " << mode);
    end(EXIT_FAILURE);
  }
}
void Config::ValidateSQLPath(const std::string& sql_path) const {
  if (!(this->IsFileExists(sql_path))) {
    LOG(ERROR, "file with SQL queries does not exist: " << sql_path);
    end(EXIT_FAILURE);
  }
}
void Config::ValidateCypherPath(const std::string& cypher_path) const {
  if (!output_.good()) {
    LOG(ERROR, "file with CypherQL queries does not exist: " << cypher_path);
    end(EXIT_FAILURE);
  }
}
void Config::ValidateTreeDumpPath(const std::string& tree_dump_path) const {
  if (!(this->IsFileExists(tree_dump_path))) {
    LOG(ERROR, "Tree Dump file does not exist: " << tree_dump_path);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "Tree Dump path is valid");
}
void Config::ValidateIsInputStreamOpen() const {
  if (!input_.is_open()) {
    LOG(ERROR, "input file stream is not opened");
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsOutputStreamOpen() const {
  if (!output_.is_open()) {
    LOG(ERROR, "output file stream is not opened");
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsTreeDumpStreamOpen() const {
  if (!tree_dump_.is_open()) {
    LOG(ERROR, "dump file stream is not opened");
    end(EXIT_FAILURE);
  }
}
void Config::ValidateIsFlagSet(OptFlag flag) const {
  if (this->IsFlagSet(flag)) {
    LOG(ERROR, "conflicting flags");
    end(EXIT_FAILURE);
  }
}
