#include "SCC/conf.h"

using namespace std;

Log SCC_log;
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

string Log::get_timestamp() {
  time_t now;
  time(&now);
  now += 3 * 3600;    // UTC+3 -- MSC
  char buf[sizeof "2011-10-08T07:07:09"];
  strftime(buf, sizeof buf, "%FT%T", gmtime(&now));
  
  return buf;
}
string Log::TimeToLogName(string timestamp) {
  replace(begin(timestamp), end(timestamp), ':', '-');
  return timestamp + ".log";
}
void Log::ValidateLogLevel(LogLevel level) const {
  try {
    if (LogLevel::LOG_LEVEL_COUNT <= level) {
      throw invalid_argument("incorrect SCC log level: " + to_string(level));
    }
  } catch (invalid_argument& ex) {
    cout << ex.what();
    exit(EXIT_FAILURE);
  }
}
void Log::ValidateLogLevel(std::string& level) const {
  try {
    if (str2log_lvl_.count(level) == 0) {
      throw invalid_argument("incorrect SCC log level: " + level);
    }
  } catch (invalid_argument& ex) {
    LOG(ERROR, ex.what());
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "log level is valid");
}

Log::Log() {
  filename_ = TimeToLogName(get_timestamp());
  output_.open("../log/" + filename_, ios::out);
}
void Log::add_log(LogLevel level, const string& msg) {
  ValidateLogLevel(level);
  ostringstream output;
  output << "[" << log_lvl2str_.at(level) << "]\t"
         << get_timestamp() << " " << msg;
  
  output_ << output.str();
  if (log_level_ >= level) {
    cout << output.str();
  }
}
LogLevel Log::get_log_level() const {
  return log_level_;
}
void Log::set_log_level(LogLevel level) {
  log_level_ = level;
}
LogLevel Log::StringToLogLevel(string& level) {
  for_each(begin(level), end(level),
           [](char& c) {
    c = (char)::toupper(c);
  });
  ValidateLogLevel(level);
  return str2log_lvl_.at(level);
}
Log::~Log() = default;


bool Config::IsFileExists(string& path) {
  struct stat buffer{};
  return (stat(path.c_str(), &buffer) == 0);
}
bool Config::IsFileExists(const ofstream& f) {
  return f.good();
}
void Config::ValidateMode(SCCMode mode) const {
  if (SCCMode::MODE_COUNT <= mode) {
    LOG(ERROR, "incorrect SCC mode: " << to_string(mode));
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "mode is valid");
}
void Config::ValidateMode(string& mode) const {
  if (str2modes_.count(mode) == 0) {
    LOG(ERROR, "invalid SCC mode: " << mode);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "mode is valid");
}
void Config::ValidateSQLPath(string& sql_path) const{
  if (!IsFileExists(sql_path)) {
    LOG(ERROR, "SQL file does not exist: " << sql_path);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "SQL path is valid");
}
void Config::ValidateCypherPath(string& cypher_path) const {
  if (!IsFileExists(output_)) {
    LOG(ERROR, "Cypher file does not exist: " << cypher_path);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "Cypher path is valid");
}
bool Config::IsFlagSet(OptFlag flag) {
  return is_config_set_.at(flag_to_config_.at(flag));
}
void Config::ValidateSetFlag(OptFlag flag) {
  if (IsFlagSet(flag)) {
    LOG(ERROR, "conflicting flags");
    exit(EXIT_FAILURE);
  }
}
void Config::SetFlag(OptFlag flag) {
  is_config_set_[flag_to_config_.at(flag)] = true;
}

Config::Config() {
  string cwf_path = __FILE__;
  string cwf = cwf_path.substr(cwf_path.find_last_of('/') + 1);
  string path = cwf_path.substr(0, cwf_path.find(cwf));

  mode_ = SCCMode::INTERACTIVE;
  sql_path_ = cypher_path_ = path;
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
  ValidateMode(mode);
  mode_ = mode;
}
void Config::set_sql_path(std::string& new_sql_path) {
  ValidateSQLPath(new_sql_path);
  sql_path_ = new_sql_path;
}
void Config::set_cypher_path(std::string& new_cypher_path) {
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
ifstream& Config::Input() {
  return input_;
}
ofstream& Config::Output() {
  return output_;
}

SCCMode Config::StringToSCCMode(std::string& mode) {
  for_each(begin(mode), end(mode),
           [](char& c) {
    c = (char)::toupper(c);
  });
  ValidateMode(mode);
  return str2modes_.at(mode);
}
string Config::SCCModeToString(SCCMode mode) {
  ValidateMode(mode);
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
        ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SCC mode = " << config.SCCModeToString(SCCMode::DAEMON));
        config.set_mode(SCCMode::DAEMON);
        SetFlag(OF_flag);
        break;
      case OptFlag::fHELP:
        LOG(INFO, "TODO: Get help");
        exit(EXIT_SUCCESS);
      case OptFlag::fINTERACTIVE:
        ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SCC mode = " << config.SCCModeToString(SCCMode::INTERACTIVE));
        config.set_mode(SCCMode::INTERACTIVE);
        SetFlag(OF_flag);
        break;
      case OptFlag::fLOG:
        LOG(TRACE, "set log level = " << optarg);
        {
          string tmp_log_level = optarg;
          config.set_log_level(config.StringToLogLevel(tmp_log_level));
        }
        break;
      case OptFlag::fMODE:
        ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SCC mode = " << optarg);
        {
          string tmp_mode = optarg;
          config.set_mode(config.StringToSCCMode(tmp_mode));
        }
        SetFlag(OF_flag);
        break;
      case OptFlag::fSQL:
        ValidateSetFlag(OF_flag);
        LOG(TRACE, "set SQL path = " << optarg);
        {
          string tmp = optarg;
          config.set_sql_path(tmp);
        }
        SetFlag(OF_flag);
        break;
      case OptFlag::fCYPHER:
        ValidateSetFlag(OF_flag);
        LOG(TRACE, "set Cypher path = " << optarg);
        {
          string tmp = optarg;
          config.set_cypher_path(tmp);
        }
        SetFlag(OF_flag);
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
