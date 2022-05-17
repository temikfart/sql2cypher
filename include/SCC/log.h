/**
 * \file
 * \brief Header file for the Log Class
 *
 * \details This file contains a declaration of Log Class, macros
 * for logging process and global variable for this process.
 * Also, this file contains necessary #include directives.
 */

#pragma once

#include <sys/stat.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

/**
 * \brief Macro for the logging process
 *
 * \details This macro allows to create one log in the special log file.
 *
 * \param[in] level Level of a current log
 * \param[in] msg Message, which will be appeared in the current log
 */
#define LOG(level, msg) do {                                    \
  std::string path = __FILE__;                                  \
  std::ostringstream new_log;                                   \
  new_log << path.substr(path.find_last_of('/') + 1)            \
    << ": " << __FUNCTION__ << "(): " << __LINE__ << ": "       \
    << msg << std::endl;                                        \
  SCC_log.AddLog(level, new_log.str());                         \
  break;                                                        \
} while(true)

#ifdef DEBIAN_PACKAGE
/**
 * \brief Macro, which changes standard log directory
 *
 * \details If the Debian .deb Package is created,
 * then this macro changes standard log directory.
 */
#define SCC_LOG_DIR "/var/log/SCC_log/"
#endif // DEBIAN_PACKAGE

#ifdef EXE_PACKAGE
/**
 * \brief Macro, which changes standard log directory
 *
 * \details If the Windows .exe Package is created,
 * then this macro changes standard log directory.
 */
#define SCC_LOG_DIR "C:/Program Files/SCC/log/"
#endif // EXE_PACKAGE

#ifdef APP_PACKAGE
/**
 * \brief Macro, which changes standard log directory
 *
 * \details If the Darwin bundle is created,
 * then this macro changes standard log directory.
 */
#define SCC_LOG_DIR ""
#endif // APP_PACKAGE

/**
 * \brief A suit of available log levels
 *
 * \details This suit contain set of the log levels. Every log level set
 * visibility for the its log message in the SCC interactive mode.
 */
enum LogLevel {
  SILENT,           ///< No message will be printed
  FATAL,            ///< Only fatal errors will be printed
  ERROR,            ///< Only errors will be printed (including fatal errors)
  INFO,             ///< User information and errors will be printed
  TRACE,            ///< Some info of function work will be printed
  DEBUG,            ///< Detailed info of function work will be printed
  LOG_LEVEL_COUNT   ///< Service state for checking if level is correct
};

/**
 * \brief Log Class for the logging process in the SCC
 *
 * \details This class contains methods, which allow to create logfiles, logs
 * in these files and in std output, also some set-get methods and validation.
 */
class Log {
public:
  /**
   * \brief Sets new log level for the logs
   *
   * \details Checks, that level is correct and set it as new log level.
   *
   * @param[in] level new log level
   */
  void set_log_level(LogLevel level);
  /**
   * @return current log level
   */
  LogLevel get_log_level() const;
  /**
   * \brief Sets path to the new logfile.
   *
   * \details Creates and sets with name in format "YYYY-MM-DDTHH-MM-SS.log"
   * (according to ISO). File located in the folder with path=dir.
   * If the ".exe" package is created, then the "dir" is checked for existence.
   *
   * @param[in] dir reference to the const string with path to the new log
   * directory (the last characters are truncated to the last "/")
   */
  void set_log_path(const std::string& dir);
  /**
   * @return the set path to the logfile
   */
  std::string get_log_path() const;
  /**
   * \brief Set field "is_system_configured_" to the "value"
   *
   * @param[in] value bool state of system's configuration (private field
   * "is_system_configured_")
   */
  void set_is_system_configured(bool value);
  /**
   * @return the boolean private field "is_system_configured_"
   */
  bool get_is_system_configured() const;
  /**
   * @return the boolean private filed "is_buffer_load"
   */
  bool get_is_buffer_load() const;
  /**
   * @param[in] value bool state of the set log directory (private field
   * "is_logdir_set_")
   */
  void set_is_logdir_set(bool value);

  /**
   * \brief Starts logging system
   *
   * \details Creates new log file at the set before path (private field
   * "filepath_"); opens the log file in output filestream; validates, that
   * the log file is created and opened.
   *
   * If the .exe package is created and the log directory is not set, then
   * ends work of the program with error.
   */
  void Start();
  /**
   * \brief Adds log message to the log file
   *
   * \details At first, "level" is validated. If "level" is valid, then the
   * method adds log message to the log file and, if "level" less than current
   * log level, adds this message in standard output.
   *
   * If the private field "is_system_configured" is set to "false", then method
   * adds the log message in the buffer.
   *
   * If the private field "is_system_configured" is set to "true", then method
   * adds the buffered logs to the log file and standard output (if "level"
   * allows). After that, an adding log message is processed.
   *
   * @param[in] level level of the log message
   * @param[in] msg the log message
   */
  void AddLog(LogLevel level, const std::string& msg);
  /**
   * \brief Loads buffered logs to the log file
   *
   * \details Loads buffered logs to the log file and standard output (if log
   * level of this message allows it). After that, the private field
   * "is_buffer_load" is set to the "true".
   */
  void LoadBufferedLogs();
  /**
   * \brief Creates and return path to the log directory
   *
   * \details If Package is created, then uses "SCC_LOG_DIR" macro. If not -
   * log directory is a standard directory in the root of the project: proj/log.
   *
   * @return the path to the log directory
   */
  static std::string GetLogDir();
  /**
   * \brief Returns log level as enum member
   *
   * \details Transform all characters in "level" to uppercase and returns
   * appropriate member of the enum "LogLevel".
   *
   * @param level log level as string
   * @return the log level from the enum suit "LogLevel"
   */
  LogLevel StringToLogLevel(std::string level) const;
  /**
   * \brief Closes the log file if it is open
   *
   * \details Closes the log file if it is open. Occurs error of closing, if it
   * was ended unsuccessful.
   *
   * @return status of closing the log file (true - success, false - failure)
   */
  bool CloseLogFile();

private:
  std::string filepath_;    ///< is path to the log file
  std::ofstream output_;    ///< is output stream (opened log file)
  LogLevel log_level_ = LogLevel::INFO;   ///< current log level

  ///< log messages, which were been buffered for some moment
  std::vector<std::pair<LogLevel, std::string>> buffered_logs_;
  bool is_buffer_load_ = false;       ///< shows, that buffered logs were loaded
  bool is_logdir_set_ = false;        ///< shows, that config option log
                                      ///< directory was set
  bool is_system_configured_ = false; ///< shows, that the process of the system
                                      ///< configuration was ended

  std::map<LogLevel, std::string> lvl2str_ = {
      {LogLevel::SILENT, "SILENT"},
      {LogLevel::FATAL, "FATAL"},
      {LogLevel::ERROR, "ERROR"},
      {LogLevel::INFO, "INFO"},
      {LogLevel::TRACE, "TRACE"},
      {LogLevel::DEBUG, "DEBUG"},
  };      ///< map container, which contains pairs of log levels: enum -> string
  std::map<std::string, LogLevel> str2lvl_ = {
      {"SILENT", LogLevel::SILENT},
      {"FATAL", LogLevel::FATAL},
      {"ERROR", LogLevel::ERROR},
      {"INFO", LogLevel::INFO},
      {"TRACE", LogLevel::TRACE},
      {"DEBUG", LogLevel::DEBUG}
  };      ///< map container, which contains pairs of log levels: string -> enum

  /**
   * \brief Static method, which return datetime as string
   *
   * \details The method gets current time (MSC: GMT+3 timezone) and return it
   * as string in ISO formatting (YYYY-MM-DDThh:mm:ss).
   *
   * @return date and time as string in ISO formatting
   */
  static std::string GetTimestamp();
  /**
   * \brief Transformation timestamp in ISO formatting to the name for the log
   * file
   *
   * \details The method gets timestamp in ISO formatting (YYYY-MM-DDThh:mm:ss),
   * replaces all colons with dashes and add \".log\" at the end.
   *
   * @param[in] timestamp timestamp in ISO formatting (YYYY-MM-DDThh:mm:ss)
   * @return the name for the log file
   */
  std::string TimeToLogFilename(std::string timestamp) const;
  /**
   * \brief Checks filepath for existence
   *
   * \details Uses stat file information for know about existence of the file.
   *
   * @param[in] path path to the some file
   * @return true if file exists, false otherwise
   */
  static bool IsFileExists(const std::string& path);

  /**
   * \brief Validates log level (enum member candidate)
   *
   * \details if \"level\" is not contained in LogLevel enum, then error
   * occurred.
   *
   * @param[in] level checking log level
   */
  static void ValidateLogLevel(LogLevel level);
  /**
   * \brief Validates log level (string)
   *
   * \details If \"level\" in uppercase is not contained in the str2lvl, then
   * error occurred and the SCC ends its work.
   *
   * @param level log level as string
   */
  void ValidateLogLevel(std::string& level) const;
  /**
   * \brief Validates file existence
   *
   * \details If filepath incorrect or the file does not exist, then error
   * occurred and the SCC ends its work
   *
   * @param[in] path path to the file
   */
  void ValidateDoesFileExist(const std::string& path) const;
  /**
   * \brief Validates, that log file is opened
   *
   * \details If log file is not opened, then error occurred and the SCC ends
   * its work
   */
  void ValidateIsLogFileOpen() const;
};

extern Log SCC_log;   ///< global variable for the logging process
