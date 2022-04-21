#include "SCC/log.h"

using namespace std;

Log SCC_log;

string Log::GetLogPath() const {
  string cwf_path = __FILE__;
  string cwf = cwf_path.substr(cwf_path.find_last_of('/') + 1);
  string path = cwf_path.substr(0, cwf_path.find(cwf));
  path += "../log/" + filename_;
  
  return path;
}
string Log::get_timestamp() {
  time_t now;
  time(&now);
  now += 3 * 3600;    // UTC+3 -- MSC
  char buf[sizeof "2011-10-08T07:07:09"];
  strftime(buf, sizeof buf, "%FT%T", gmtime(&now));
  
  return buf;
}
string Log::TimeToLogName(string timestamp) const {
  replace(begin(timestamp), end(timestamp), ':', '-');
  return (timestamp + ".log");
}
void Log::ValidateLogLevel(LogLevel level) const {
  if (LogLevel::LOG_LEVEL_COUNT <= level) {
    // TODO: resolve this log
    cerr << "incorrect SCC log level: " << to_string(level) << endl;
    exit(EXIT_FAILURE);
  }
}
void Log::ValidateLogLevel(string& level) const {
  if (str2lvl_.count(level) == 0) {
    LOG(ERROR, "incorrect SCC log level: " + level);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "log level is valid");
}

Log::Log() {
  filename_ = this->TimeToLogName(this->get_timestamp());
  output_.open(this->GetLogPath(), ios::out);
}
void Log::AddLog(LogLevel level, const string& msg) {
  this->ValidateLogLevel(level);
  
  ostringstream output;
  output << "[" << lvl2str_.at(level) << "]\t"
         << this->get_timestamp() << " " << msg;
  output_ << output.str();
  
  if (log_level_ >= level) {
    cout << output.str();
  }
}
LogLevel Log::get_log_level() const {
  return log_level_;
}
void Log::set_log_level(LogLevel level) {
  this->ValidateLogLevel(level);
  log_level_ = level;
}
LogLevel Log::StringToLogLevel(string level) const {
  for_each(begin(level), end(level),
           [](char& c) {
             c = (char)::toupper(c);
           });
  this->ValidateLogLevel(level);
  return str2lvl_.at(level);
}
Log::~Log() = default;
