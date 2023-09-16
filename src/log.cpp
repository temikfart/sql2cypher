#include "SCC/log.h"

Log SCC_log;

void Log::set_log_level(LogLevel level) {
  Log::ValidateLogLevel(level);
  log_level_ = level;
}
LogLevel Log::get_log_level() const {
  return log_level_;
}
void Log::set_log_path(const std::string& dir) {
  filepath_ = dir.substr(0, dir.find_last_of('/') + 1);
#ifndef EXE_PACKAGE
  this->ValidateDoesFileExist(filepath_);
#endif // EXE_PACKAGE
  filepath_ += this->TimeToLogFilename(Log::GetTimestamp());
}
std::string Log::get_log_path() const {
  return filepath_;
}
void Log::set_is_system_configured(bool value) {
  is_system_configured_ = value;
}
bool Log::get_is_system_configured() const {
  return is_system_configured_;
}
bool Log::get_is_buffer_load() const {
  return is_buffer_load_;
}
void Log::set_is_logdir_set(bool value) {
  is_logdir_set_ = value;
}

void Log::Start() {
#ifdef EXE_PACKAGE
  if (!is_logdir_set_) {
    std::cout << "log directory must be set with flag \"logdir\"";
    exit(EXIT_FAILURE);
  }
#endif // EXE_PACKAGE
  this->CloseLogFile();
  output_.open(filepath_, std::ios::out);
  this->ValidateDoesFileExist(filepath_);
  this->ValidateIsLogFileOpen();
}
void Log::AddLog(LogLevel level, const std::string& msg) {
  Log::ValidateLogLevel(level);

  std::ostringstream output;
  output << "[" << lvl2str_.at(level) << "]\t"
         << Log::GetTimestamp() << " " << msg;
  if (!is_system_configured_) {
    buffered_logs_.emplace_back(level, output.str());
    return;
  } else {
    if (!is_buffer_load_) {
      this->LoadBufferedLogs();
    }
  }
#ifndef EXE_PACKAGE
  output_ << output.str();
  output_.flush();
#endif // EXE_PACKAGE

  if (log_level_ >= level) {
    std::cout << output.str();
  }
}
void Log::LoadBufferedLogs() {
  for (const auto& m: buffered_logs_) {
#ifndef EXE_PACKAGE
    output_ << m.second;
#endif // EXE_PACKAGE
    if (log_level_ >= m.first) {
      std::cout << m.second;
    }
  }
  is_buffer_load_ = true;
}
std::string Log::GetLogDir() {
  std::string path;
#ifdef CREATE_PACKAGE
  path = SCC_LOG_DIR;
#else
  std::string cwf_path = __FILE__;
  std::string cwf = cwf_path.substr(cwf_path.find_last_of('/') + 1);
  path = cwf_path.substr(0, cwf_path.find(cwf));
  path += "../log/";
#endif // CREATE_PACKAGE

  return path;
}
LogLevel Log::StringToLogLevel(std::string level) const {
  for_each(begin(level), end(level),
           [](char& c) {
             c = (char) ::toupper(c);
           });
  this->ValidateLogLevel(level);
  return str2lvl_.at(level);
}
bool Log::CloseLogFile() {
  LOG_OLD(TRACE, "closing output file...");
  if (output_.is_open()) {
    output_.close();
    if (output_.good()) {
      LOG_OLD(TRACE, "output file closed successfully");
    } else {
      LOG_OLD(ERROR, "output file close error");
      return false;
    }
  } else {
    LOG_OLD(TRACE, "output file is already closed");
  }
  return true;
}

std::string Log::GetTimestamp() {
  time_t now;
  time(&now);
  const int kTimeZone = 3;  // UTC + kTimeZone
  const int ksec_in_hour = 3600;
  now += kTimeZone * ksec_in_hour;
  char buf[sizeof "2011-10-08T07:07:09"];
//  strftime(buf, sizeof buf, "%FT%T", gmtime(&now));
  strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S", gmtime(&now));

  return buf;
}
std::string Log::TimeToLogFilename(std::string timestamp) const {
  replace(begin(timestamp), end(timestamp), ':', '-');
  return (timestamp + ".log");
}
bool Log::IsFileExists(const std::string& path) {
  struct stat buffer{};
  return (stat(path.c_str(), &buffer) == 0);
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
    LOG_OLD(ERROR, "incorrect SCC log level: " + level);
    exit(EXIT_FAILURE);
  }
  LOG_OLD(DEBUG, "log level is valid");
}
void Log::ValidateDoesFileExist(const std::string& path) const {
  if (!(this->IsFileExists(path))) {
    std::cout << "file or directory does not exist: " << path << std::endl;
    exit(EXIT_FAILURE);
  }
}
void Log::ValidateIsLogFileOpen() const {
  if (!output_.is_open()) {
    std::cout << "input file stream is not opened" << std::endl;
    exit(EXIT_FAILURE);
  }
}
