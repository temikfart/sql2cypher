#include "SCC/log.h"

Log SCC_log;

Log::Log() {
  filename_ = this->TimeToLogFilename(Log::GetTimestamp());
  output_.open(this->GetLogPath(), std::ios::out);
}
Log::~Log() = default;

void Log::set_log_level(LogLevel level) {
  Log::ValidateLogLevel(level);
  log_level_ = level;
}
LogLevel Log::get_log_level() const {
  return log_level_;
}

void Log::AddLog(LogLevel level, const std::string& msg) {
  Log::ValidateLogLevel(level);

  std::ostringstream output;
  output << "[" << lvl2str_.at(level) << "]\t"
         << Log::GetTimestamp() << " " << msg;
  output_ << output.str();
  output_.flush();

  if (log_level_ >= level) {
    std::cout << output.str();
  }
}
LogLevel Log::StringToLogLevel(std::string level) const {
  for_each(begin(level), end(level),
           [](char& c) {
             c = (char) ::toupper(c);
           });
  this->ValidateLogLevel(level);
  return str2lvl_.at(level);
}


std::string Log::GetLogPath() const {
  std::string cwf_path = __FILE__;
  std::string cwf = cwf_path.substr(cwf_path.find_last_of('/') + 1);
  std::string path = cwf_path.substr(0, cwf_path.find(cwf));
  path += "../log/" + filename_;

  return path;
}
std::string Log::GetTimestamp() {
  time_t now;
  time(&now);
  const int kTimeZone = 3;  // UTC + kTimeZone
  const int ksec_in_hour = 3600;
  now += kTimeZone * ksec_in_hour;
  char buf[sizeof "2011-10-08T07:07:09"];
  strftime(buf, sizeof buf, "%FT%T", gmtime(&now));

  return buf;
}
std::string Log::TimeToLogFilename(std::string timestamp) const {
  replace(begin(timestamp), end(timestamp), ':', '-');
  return (timestamp + ".log");
}

void Log::ValidateLogLevel(LogLevel level) {
  if (LogLevel::LOG_LEVEL_COUNT <= level) {
    std::cerr << "incorrect SCC log level: "
              << std::to_string(level) << std::endl;
    exit(EXIT_FAILURE);
  }
}
void Log::ValidateLogLevel(std::string& level) const {
  if (str2lvl_.count(level) == 0) {
    LOG(ERROR, "incorrect SCC log level: " + level);
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "log level is valid");
}
