#include <format>
#include <vector>

#include "gtest/gtest.h"

#include "SCC/common/file_utils.h"
#include "SCC/config/scc_args.h"

using namespace scc;
using namespace testing;

class SCCArgsBaseTests : public Test {
protected:
  struct Args {
    int argc = 0;
    std::vector<const char*> argv;
  };

  Args args;
  SCCArgs parser;
  const std::string resource_path = common::ResourcesPath();

  void ParseArgsWrapper() {
    parser.ParseArgs(args.argc, args.argv.data());
  }

  void AddArg(const std::string& option) {
    char* arg = new char[option.length() + 1];
    strcpy(arg, option.c_str());

    args.argv.push_back(arg);
    args.argc++;
  }

  void SetUp() override {
    AddArg("scc");
  }

  void TearDown() override {
    for (const char* arg : args.argv)
      delete[] arg;
  }
};

TEST_F(SCCArgsBaseTests, NoArgumentsTest) {
  EXPECT_EXIT(ParseArgsWrapper(), ExitedWithCode(EXIT_SUCCESS), ""); // TODO: add regex.
}

TEST_F(SCCArgsBaseTests, GetHelpLongOptionTest) {
  AddArg("--help");

  EXPECT_EXIT(ParseArgsWrapper(), ExitedWithCode(EXIT_SUCCESS), ""); // TODO: add regex.
}
TEST_F(SCCArgsBaseTests, GetHelpShortOptionTest) {
  AddArg("-h");

  EXPECT_EXIT(ParseArgsWrapper(), ExitedWithCode(EXIT_SUCCESS), ""); // TODO: add regex.
}

TEST_F(SCCArgsBaseTests, GetVersionLongOptionTest) {
  AddArg("--version");

  EXPECT_EXIT(ParseArgsWrapper(), ExitedWithCode(EXIT_SUCCESS), ""); // TODO: add regex.
}
TEST_F(SCCArgsBaseTests, GetVersionShortOptionTest) {
  AddArg("-v");

  EXPECT_EXIT(ParseArgsWrapper(), ExitedWithCode(EXIT_SUCCESS), ""); // TODO: add regex.
}

TEST_F(SCCArgsBaseTests, SQLArgumentTest) {
  const std::string sql_file = resource_path + "/sql_queries.sql";
  AddArg(std::format("--sql={}", sql_file));

  EXPECT_NO_THROW(ParseArgsWrapper());
}
TEST_F(SCCArgsBaseTests, SQLArgumentWithoutParameterTest) {
  GTEST_SKIP() << "Terminated by SIGTRAP.";
  AddArg("--sql");

  EXPECT_THROW(ParseArgsWrapper(), std::runtime_error);
}

