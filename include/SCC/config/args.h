#pragma once

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

using namespace argparse;

class SCCArgs : private ArgumentParser {
public:
  explicit SCCArgs();

  void ParseArgs(int argc, char* argv[]);

  template<typename T = std::string>
  auto Get(const std::string& arg_name) const {
    return get<T>(arg_name);
  }

  template<typename T = std::string>
  std::optional<T> Present(const std::string& arg_name) const {
    return present(arg_name);
  }

  bool IsUsed(const std::string& arg_name) const;

  template<typename T = std::string>
  auto operator[](const std::string& arg_name) const {
    return Get<T>(arg_name);
  }

private:
  void PrintHelpAndExit() const;

  std::string version_;
};
