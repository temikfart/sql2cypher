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
  fs::path config_path = Config::GetConfigPath();
  set_sql_file(config_path / "../../resources/sql_queries.sql");
  set_cypher_file(config_path / "../../resources/cypher_queries.cypher");
  set_ast_dump_file(config_path / "../../resources/tree_dump/tree_dump.txt");
#endif // CREATE_PACKAGE
}

void Config::set_sql_file(const fs::path& new_sql_file) {
  ValidateFileExists(new_sql_file);
  sql_file_ = fs::weakly_canonical(new_sql_file);
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
    return;
    switch (flag) {
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
