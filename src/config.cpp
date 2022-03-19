#include "SCC/config.h"

using namespace std;

Config config;

void Configure(int argc, char* argv[]) {
  LOG(TRACE, "Configuring system...");

  if (argc > 1) {
    config.GetConsoleArguments(argc, argv);
  } else {
    // TODO: Parse config.ini
  }
  config.Start();

  LOG(TRACE, "Configuration is completed");
}

bool Config::IsFileExists(const string& path) {
  struct stat buffer{};
  return (stat(path.c_str(), &buffer) == 0);
}
bool Config::IsFileExists(const ofstream& f) {
  return f.good();
}
void Config::ValidateMode(SCCMode mode) const {
  if (SCCMode::SCCMODE_COUNT <= mode) {
    LOG(ERROR, "incorrect SCC mode: " << to_string(mode));
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "mode is valid");
}
void Config::ValidateMode(const string& mode) const {
  if (str2modes_.count(mode) == 0) {
    LOG(ERROR, "invalid SCC mode: " << mode);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "mode is valid");
}
void Config::ValidateSQLPath(const string& sql_path) const {
  if (!(this->IsFileExists(sql_path))) {
    LOG(ERROR, "SQL file does not exist: " << sql_path);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "SQL path is valid");
}
void Config::ValidateCypherPath(const string& cypher_path) const {
  if (!(this->IsFileExists(output_))) {
    LOG(ERROR, "Cypher file does not exist: " << cypher_path);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "Cypher path is valid");
}
bool Config::IsFlagSet(OptFlag flag) const {
  return is_config_set_.at(flag_to_config_.at(flag));
}
void Config::ValidateSetFlag(OptFlag flag) const {
  if (this->IsFlagSet(flag)) {
    LOG(ERROR, "conflicting flags");
    exit(EXIT_FAILURE);
  }
}
void Config::SetFlag(OptFlag flag) {
  is_config_set_[flag_to_config_.at(flag)] = true;
}

Config::Config() {
  mode_ = SCCMode::INTERACTIVE;
  sql_path_ = cypher_path_ = this->GetConfigPath();
  sql_path_ += "../resources/sql_queries.sql";
  cypher_path_ += "../resources/cypher_queries.cypher";
}
void Config::Start() {
  input_.open(sql_path_, ios::in);
  output_.open(cypher_path_, ios::out);
  ValidateCypherPath(cypher_path_);
  // TODO: implement SCC mode behavior.
}
void Config::set_mode(SCCMode mode) {
  this->ValidateMode(mode);
  mode_ = mode;
}
void Config::set_sql_path(const string& new_sql_path) {
  this->ValidateSQLPath(new_sql_path);
  sql_path_ = new_sql_path;
}
void Config::set_cypher_path(const string& new_cypher_path) {
  cypher_path_ = new_cypher_path;
}
SCCMode Config::get_mode() const {
  return mode_;
}
string Config::get_sql_path() const {
  return sql_path_;
}
string Config::get_cypher_path() const {
  return cypher_path_;
}
string Config::GetConfigPath() const {
  string cwf_path = __FILE__;
  string cwf = cwf_path.substr(cwf_path.find_last_of('/') + 1);
  string path = cwf_path.substr(0, cwf_path.find(cwf));

  return path;
}
ifstream& Config::ReadSQL() {
  return input_;
}
ofstream& Config::WriteCypher() {
  return output_;
}
SCCMode Config::StringToSCCMode(string mode) const {
  for_each(begin(mode), end(mode),
           [](char& c) {
    c = (char)::toupper(c);
  });
  this->ValidateMode(mode);
  return str2modes_.at(mode);
}
string Config::SCCModeToString(SCCMode mode) const {
  this->ValidateMode(mode);
  return modes2str_.at(mode);
}
void Config::GetConsoleArguments(int argc, char* const* argv) {
  LOG(DEBUG, "parsing console arguments...");

  int flag;
  opterr = 0;

  const char short_options[] = "dhil:m:";
  const struct option long_options[] = {
          {"cypher", required_argument, nullptr, OptFlag::fCYPHER},
          {"daemon", 0, nullptr, OptFlag::fDAEMON},
          {"help", 0, nullptr, OptFlag::fHELP},
          {"interactive", 0, nullptr, OptFlag::fINTERACTIVE},
          {"log", required_argument, nullptr, OptFlag::fLOG},
          {"mode", required_argument, nullptr, OptFlag::fMODE},
          {"sql", required_argument, nullptr, OptFlag::fSQL},
          {nullptr, 0, nullptr, 0},
  };
  while ((flag = getopt_long(argc,
                             argv,
                             short_options,
                             long_options,
                             nullptr)) != -1) {
    auto OF_flag = static_cast<OptFlag>(flag);
    switch (flag) {
      case OptFlag::fDAEMON:
        this->ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SCC mode = "
                   << this->SCCModeToString(SCCMode::DAEMON));
        this->set_mode(SCCMode::DAEMON);
        this->SetFlag(OF_flag);
        break;
      case OptFlag::fHELP:
        LOG(INFO, "TODO: Get help");
        exit(EXIT_SUCCESS);
      case OptFlag::fINTERACTIVE:
        this->ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SCC mode = "
                   << this->SCCModeToString(SCCMode::INTERACTIVE));
        this->set_mode(SCCMode::INTERACTIVE);
        this->SetFlag(OF_flag);
        break;
      case OptFlag::fLOG:
        this->ValidateSetFlag(OF_flag);
        LOG(TRACE, "set log level = " << optarg);
        {
          string tmp_log_level = optarg;
          SCC_log.set_log_level(this->StringToLogLevel(tmp_log_level));
        }
        this->SetFlag(OF_flag);
        break;
      case OptFlag::fMODE:
        this->ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SCC mode = " << optarg);
        {
          string tmp_mode = optarg;
          this->set_mode(this->StringToSCCMode(tmp_mode));
        }
        this->SetFlag(OF_flag);
        break;
      case OptFlag::fSQL:
        this->ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SQL path = " << optarg);
        {
          string tmp = optarg;
          this->set_sql_path(tmp);
        }
        this->SetFlag(OF_flag);
        break;
      case OptFlag::fCYPHER:
        this->ValidateSetFlag(OF_flag);
        LOG(TRACE, "set Cypher path = " << optarg);
        {
          string tmp = optarg;
          this->set_cypher_path(tmp);
        }
        this->SetFlag(OF_flag);
        break;
      default:
        char f = short_options[optind+1];
        if (short_options[optind+2] == ':') {
          LOG(ERROR,
              "flag must have an argument");
        } else {
          LOG(ERROR, "undefined flag: " << f);
        }
        exit(EXIT_FAILURE);
    }
  }
  LOG(DEBUG, "all console arguments are parsed");
}

Config::~Config() {
  input_.close();
  output_.close();
}
