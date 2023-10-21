#pragma once

#include <exception>
#include <filesystem>

#include "argparse/argparse.hpp"
#include "logger/severity.hpp"
#include "SCC/config/scc_mode.h"

#define CONTRIBUTE_LINK "https://github.com/temikfart/sql2cypher"
#define PROGRAM_NAME "scc"

#ifdef SCC_VERSION
#define VERSION SCC_VERSION
#else
#define VERSION "0.9.2-rc"
#endif // SCC_VERSION

#ifdef SCC_MAINTAINERS
#define DEVELOPERS SCC_MAINTAINERS
#else
#define DEVELOPERS "Artyom Fartygin, Roman Korostinkiy"
#endif // SCC_MAINTAINERS

#if defined(DEBIAN_PACKAGE)
#define SCC_LOG_DIR "/var/log/SCC_log/"
#elif defined(EXE_PACKAGE)
#define SCC_LOG_DIR "C:/Program Files/SCC/log/"
#elif defined(APP_PACKAGE)
#define SCC_LOG_DIR ""
#else
#define SCC_LOG_DIR "../log"
#endif

namespace scc {

class no_argument_error : private std::runtime_error {
public:
  explicit no_argument_error(const std::string& message);
};

class SCCArgs : public argparse::ArgumentParser {
public:
  explicit SCCArgs();

  void ParseArgs(int argc, const char* const argv[]);

  template<typename T = std::string>
  auto Get(const std::string& arg_name) const
  -> std::conditional_t<argparse::details::IsContainer<T>, T, const T&> {
    try {
      return get<T>(arg_name);
    } catch (const std::logic_error& e) {
      throw no_argument_error(e.what());
    }
  }

  template<typename T = std::string>
  std::optional<T> Present(const std::string& arg_name) const {
    return present<T>(arg_name);
  }

  bool IsUsed(const std::string& arg_name) const;

private:
  void PrintHelpAndExit(int exit_code) const;
};

} // scc
