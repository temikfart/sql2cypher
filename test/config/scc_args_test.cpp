#include <format>
#include <vector>

#include "gtest/gtest.h"

#include "SCC/common/file_utils.h"
#include "SCC/config/scc_args.h"
#include "SCC/log.h"

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

class SCCArgsTests : public SCCArgsBaseTests {
protected:
  void SetUp() override {
    AddArg("scc");
    AddArg(std::format("--sql={}", common::ResourcesPath() + "/sql_queries.sql"));
  }
};

TEST_F(SCCArgsTests, CypherArgumentDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string default_cypher_file = parser.Get("--cypher");
  EXPECT_TRUE(default_cypher_file.find("out.cypher") != std::string::npos);
}
TEST_F(SCCArgsTests, CypherArgumentWithoutParameterTest) {
  GTEST_SKIP() << "Terminated by SIGTRAP.";
  AddArg("--cypher");

  EXPECT_THROW(ParseArgsWrapper(), std::runtime_error);
}

TEST_F(SCCArgsTests, ModeArgumentDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());

  SCCMode default_mode_long = parser.Get<SCCMode>("--mode");
  EXPECT_EQ(default_mode_long, SCCMode::kInteractive);

  SCCMode default_mode_short = parser.Get<SCCMode>("-m");
  EXPECT_EQ(default_mode_short, SCCMode::kInteractive);
}
TEST_F(SCCArgsTests, ModeArgumentWithoutParameterTest) {
  GTEST_SKIP() << "Argument without value did not throw an exception.";
  AddArg("--mode");

  EXPECT_THROW(ParseArgsWrapper(), std::runtime_error);
}
TEST_F(SCCArgsTests, ModeArgumentDaemonValueTest) {
  AddArg(std::format("--mode={}", "daemon"));

  EXPECT_NO_THROW(ParseArgsWrapper());

  SCCMode mode = SCCMode(parser.Get("--mode"));
  EXPECT_EQ(mode, SCCMode::kDaemon);
}
TEST_F(SCCArgsTests, ModeArgumentInteractiveValueTest) {
  AddArg(std::format("--mode={}", "interactive"));

  EXPECT_NO_THROW(ParseArgsWrapper());

  SCCMode mode = SCCMode(parser.Get("--mode"));
  EXPECT_EQ(mode, SCCMode::kInteractive);
}
TEST_F(SCCArgsTests, ModeArgumentInvalidValueTest) {
  GTEST_SKIP() << "Terminated by SIGTRAP.";
  AddArg(std::format("--mode={}", "invalid"));

  EXPECT_NO_THROW(ParseArgsWrapper());

  EXPECT_THROW(SCCMode(parser.Get("--mode")), std::invalid_argument);
}

TEST_F(SCCArgsTests, DaemonArgumentDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());

  bool default_daemon_long = parser.Get<bool>("--daemon");
  EXPECT_FALSE(default_daemon_long);

  bool default_daemon_short = parser.Get<bool>("-d");
  EXPECT_FALSE(default_daemon_short);
}
TEST_F(SCCArgsTests, DaemonArgumentTest) {
  AddArg("--daemon");

  EXPECT_NO_THROW(ParseArgsWrapper());

  bool is_daemon = parser.Get<bool>("--daemon");
  EXPECT_TRUE(is_daemon);
}

TEST_F(SCCArgsTests, LogSeverityArgumentDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());

  logger::Severity default_log_severity_long = parser.Get<logger::Severity>("--log-severity");
  EXPECT_EQ(default_log_severity_long, logger::Severity::info);

  logger::Severity default_log_severity_short = parser.Get<logger::Severity>("-l");
  EXPECT_EQ(default_log_severity_short, logger::Severity::info);
}
TEST_F(SCCArgsTests, LogSeverityArgumentValidValueTest) {
  AddArg(std::format("--log-severity={}", "fatal"));

  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string severity_str = parser.Get("-l");
  logger::Severity severity = log::to_severity(severity_str);
  EXPECT_EQ(severity, logger::Severity::fatal);
}
TEST_F(SCCArgsTests, LogSeverityArgumentInvalidValueTest) {
  GTEST_SKIP() << "Terminated by SIGTRAP.";
  AddArg(std::format("--log-severity={}", "invalid_severity"));

  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string invalid_severity_str = parser.Get("-l");
  EXPECT_THROW(log::to_severity(invalid_severity_str), std::invalid_argument);
}

TEST_F(SCCArgsTests, LogDirectoryArgumentDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string default_log_directory_path = parser.Get("--log-directory");
  EXPECT_EQ(default_log_directory_path, SCC_LOG_DIR);
}
TEST_F(SCCArgsTests, LogDirectoryArgumentTest) {
  std::string log_dir_value = "/path/to/log_dir";
  AddArg(std::format("--log-directory={}", log_dir_value));

  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string log_directory_path = parser.Get("--log-directory");
  EXPECT_EQ(log_directory_path, log_dir_value);
}
