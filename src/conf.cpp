#include "SCC/conf.h"

using namespace std;

Log SCC_log;

void configure() {
  LOG(TRACE, "Configuring system...")
  LOG(TRACE, "Configuration is completed")
}

string Log::get_timestamp() {
  time_t now;
  time(&now);
  now += 3 * 3600;    // UTC+3 -- MSC
  char buf[sizeof "2011-10-08T07:07:09"];
  strftime(buf, sizeof buf, "%FT%T", gmtime(&now));
  
  return buf;
}
string Log::time_to_log_name(string timestamp) {
  replace(begin(timestamp), end(timestamp), ':', '-');
  return timestamp + ".log";
}

Log::Log() {
  filename_ = time_to_log_name(get_timestamp());
  std_output_.open("../log/" + filename_, ios::out);
}
void Log::add_log(LogLevel level, string msg) {
  ostringstream output;
  output << "[" << log_levels_.at(level) << "]\t"
         << get_timestamp() << " " << msg;
  
  std_output_ << output.str();
  if (log_level_ >= level) {
    cout << output.str();
  }
}
LogLevel Log::get_log_level() const {
  return log_level_;
}
Log::~Log() = default;
