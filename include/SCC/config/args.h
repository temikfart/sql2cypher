#pragma once

#include "argparse/argparse.hpp"

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

private:
  void PrintHelpAndExit() const;

  std::string version_;
};
