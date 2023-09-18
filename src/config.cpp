#include "SCC/config.h"

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
  mode_ = SCCMode::kInteractive;
#ifndef CREATE_PACKAGE
  sql_path_ = cypher_path_ = Config::GetConfigPath();
  sql_path_ += "../resources/sql_queries.sql";
  cypher_path_ += "../resources/cypher_queries.cypher";
  tree_dump_path_ += "../resources/tree_dump/tree_dump.txt";
#endif // CREATE_PACKAGE
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
void Config::set_is_need_dump(bool value) {
  is_need_dump_ = value;
}
bool Config::get_is_need_dump() const {
  return is_need_dump_;
}
void Config::set_is_silent_print(bool value) {
  is_silent_print_ = value;
}
bool Config::get_is_silent_print() const {
  return is_silent_print_;
}
void Config::set_tree_dump_path(const std::string& new_tree_dump_path) {
  tree_dump_path_ = new_tree_dump_path;
}
std::string Config::get_tree_dump_path() const {
  return tree_dump_path_;
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

  input_.open(sql_path_, std::ios::in);
  this->ValidateIsInputStreamOpen();

  output_.open(cypher_path_, std::ios::out);
  this->ValidateCypherPath();
  this->ValidateIsOutputStreamOpen();

  if (is_need_dump_) {
    tree_dump_.open(tree_dump_path_, std::ios::out);
    this->ValidateTreeDumpPath();
    this->ValidateIsTreeDumpStreamOpen();
  }

  if (config.get_mode() == SCCMode::kDaemon) {
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

  this->set_is_silent_print(true);
  end(EXIT_SUCCESS);
}
void Config::PrintVersion() {
  std::cout << std::left;
  std::cout << "<===| SCC (The SQL to CypherQL Converter) |===>\n";
  std::cout << std::setw(13) << "Version:" << VERSION << "\n";
  std::cout << std::setw(13) << "Developers:" << DEVELOPERS;
  std::cout << std::endl;

  this->set_is_silent_print(true);
  end(EXIT_SUCCESS);
}
void Config::SetOptFlagDaemon(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  this->set_mode(SCCMode::kDaemon);
  this->SetFlag(flag);
}
void Config::SetOptFlagInteractive(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  this->set_mode(SCCMode::kInteractive);
  this->SetFlag(flag);
}
void Config::SetOptFlagLogLvl(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string severity = optarg;
  SCCLogger::set_severity(severity);
  this->SetFlag(flag);
}
void Config::SetOptFlagLogDir(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string dir = optarg;
  log_dir_ = dir;
  this->SetFlag(flag);
}
void Config::SetOptFlagMode(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string tmp_mode = optarg;
  this->set_mode(this->StringToSCCMode(tmp_mode));
  this->SetFlag(flag);
}
void Config::SetOptFlagSQL(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string path = optarg;
  this->set_sql_path(path);
  this->SetFlag(flag);
}
void Config::SetOptFlagCypher(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string tmp = optarg;
  this->set_cypher_path(tmp);
  this->SetFlag(flag);
}
void Config::SetOptFlagTreeDump(OptFlag flag) {
  this->ValidateIsFlagSet(flag);
  std::string tmp = optarg;
  this->set_is_need_dump(true);
  this->set_tree_dump_path(tmp);
  this->SetFlag(flag);
}

void Config::ValidateMode(SCCMode mode) const {
  if (SCCMode::kSCCModeCount <= mode) {
    std::cerr << "incorrect SCC mode: " << mode << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateMode(const std::string& mode) const {
  if (str2modes_.count(mode) == 0) {
    std::cerr << "invalid SCC mode: " << mode << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateSQLPath(const std::string& sql_path) const {
  if (!(this->IsFileExists(sql_path))) {
    std::cerr << "file with SQL queries does not exist: " << sql_path << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateCypherPath() const {
  if (!(this->IsFileExists(cypher_path_))) {
    std::cerr << "file with CypherQL queries does not exist: " << cypher_path_ << std::endl;
    end(EXIT_FAILURE);
  }
}
void Config::ValidateTreeDumpPath() const {
  if (!(this->IsFileExists(tree_dump_path_))) {
    std::cerr << "Tree Dump file does not exist: " << tree_dump_path_ << std::endl;
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
