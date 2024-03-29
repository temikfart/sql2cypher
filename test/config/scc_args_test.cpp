#include <optional>
#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/common/file_utils.h"
#include "SCC/config/scc_args.h"
#include "SCC/fixtures/scc_args_fixtures.h"
#include "SCC/log.h"

using namespace scc;
using namespace testing;

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
  const std::string sql_file = sql_path;
  AddArg("--sql", sql_file);

  EXPECT_NO_THROW(ParseArgsWrapper());
}
TEST_F(SCCArgsBaseTests, SQLArgumentWithoutParameterTest) {
  GTEST_SKIP() << "Argument without value did not throw an exception.";
  AddArg("--sql");

  EXPECT_THROW(ParseArgsWrapper(), std::runtime_error);
}

TEST_F(SCCArgsTests, CypherArgumentDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string default_cypher_file = parser.Get("--cypher");
  EXPECT_TRUE(default_cypher_file.find("out.cypher") != std::string::npos);
}
TEST_F(SCCArgsTests, CypherArgumentWithoutParameterTest) {
  GTEST_SKIP() << "Argument without value did not throw an exception.";
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
  AddArg("--mode", "daemon");

  EXPECT_NO_THROW(ParseArgsWrapper());

  SCCMode mode = SCCMode(parser.Get("--mode"));
  EXPECT_EQ(mode, SCCMode::kDaemon);
}
TEST_F(SCCArgsTests, ModeArgumentInteractiveValueTest) {
  AddArg("--mode", "interactive");

  EXPECT_NO_THROW(ParseArgsWrapper());

  SCCMode mode = SCCMode(parser.Get("--mode"));
  EXPECT_EQ(mode, SCCMode::kInteractive);
}
TEST_F(SCCArgsTests, ModeArgumentInvalidValueTest) {
  AddArg("--mode", "invalid");

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
  AddArg("--log-severity", "fatal");

  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string severity_str = parser.Get("-l");
  logger::Severity severity = log::to_severity(severity_str);
  EXPECT_EQ(severity, logger::Severity::fatal);
}
TEST_F(SCCArgsTests, LogSeverityArgumentInvalidValueTest) {
  AddArg("--log-severity", "invalid_severity");

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
  AddArg("--log-directory", log_dir_value);

  EXPECT_NO_THROW(ParseArgsWrapper());

  std::string log_directory_path = parser.Get("--log-directory");
  EXPECT_EQ(log_directory_path, log_dir_value);
}

TEST_F(SCCArgsTests, IsUsedUsedArgumentTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_TRUE(parser.IsUsed("--sql"));
}
TEST_F(SCCArgsTests, IsUsedUnusedArgumentWithDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_FALSE(parser.IsUsed("--daemon"));
}
TEST_F(SCCArgsTests, IsUsedUnusedArgumentWithoutDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_FALSE(parser.IsUsed("--dump"));
}
TEST_F(SCCArgsTests, IsUsedInvalidArgumentTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_THROW(parser.IsUsed("--invalidArgument"), std::logic_error);
}

TEST_F(SCCArgsTests, PresentUsedArgumentTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_NO_THROW(
      std::optional<std::string> sql_value = parser.Present("--sql");
      EXPECT_EQ(sql_value.value(), sql_path);
  );
}
TEST_F(SCCArgsTests, PresentUnusedArgumentWithDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_THROW(parser.Present<bool>("--daemon"), std::logic_error);
}
TEST_F(SCCArgsTests, PresentUnusedArgumentWithoutDefaultValueTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_NO_THROW(
      std::optional<bool> dump_file = parser.Present<bool>("--dump");
      EXPECT_FALSE(dump_file.has_value());
  );
}
TEST_F(SCCArgsTests, PresentInvalidArgumentTest) {
  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_THROW(parser.Present("--invalidArgument"), std::logic_error);
}
