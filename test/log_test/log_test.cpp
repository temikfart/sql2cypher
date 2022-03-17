#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/log.h"

using namespace testing;

TEST(SetGetLogTests, DefaultLogLevelTest) {
  Log tLog;

  EXPECT_EQ(LogLevel::INFO, tLog.get_log_level())
    << "default log level should be \'INFO\'";
}
TEST(SetGetLogTests, SetGetValidValuesTest) {
  Log tLog;
  tLog.set_log_level(LogLevel::SILENT);

  using LL = LogLevel;
  tLog.set_log_level(LL::SILENT);
  EXPECT_EQ(LL::SILENT, tLog.get_log_level());

  tLog.set_log_level(LL::TRACE);
  EXPECT_EQ(LL::TRACE, tLog.get_log_level());
}
TEST(SetGetLogTests, SetGetInvalidValuesTest) {
  Log tLog;
  tLog.set_log_level(LogLevel::SILENT);

  EXPECT_EXIT(tLog.set_log_level((LogLevel)(100500)),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "100500 should not be set as Log level";

  EXPECT_EXIT(tLog.set_log_level(LogLevel::LOG_LEVEL_COUNT),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "LogLevel::LOG_LEVEL_COUNT should not be set as Log level";
}
TEST(ConvertLogTests, ValidStringToLevelTest) {
  Log tLog;
  tLog.set_log_level(LogLevel::SILENT);

  EXPECT_THAT(tLog.StringToLogLevel("silENT"), LogLevel::SILENT);
  EXPECT_THAT(tLog.StringToLogLevel("faTaL"), LogLevel::FATAL);
  EXPECT_THAT(tLog.StringToLogLevel("ERROR"), LogLevel::ERROR);
  EXPECT_THAT(tLog.StringToLogLevel("info"), LogLevel::INFO);
  EXPECT_THAT(tLog.StringToLogLevel("TrAcE"), LogLevel::TRACE);
  EXPECT_THAT(tLog.StringToLogLevel("DebuG"), LogLevel::DEBUG);
}
TEST(ConvertLogTests, InvalidStringToLevelTest) {
  Log tLog;
  tLog.set_log_level(LogLevel::SILENT);

  EXPECT_EXIT(tLog.StringToLogLevel(""),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "empty string should not be converted to Log level";

  EXPECT_EXIT(tLog.StringToLogLevel("inf"),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "\"inf\" should not be converted to Log level";

  EXPECT_EXIT(tLog.StringToLogLevel("DEBUGGING"),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "\"DEBUGGING\" should not be converted to Log level";
}
// TODO: come up with new checks (instead of EXPECT_EXIT macros).
TEST(AddLogTests, DISABLED_AddValidLogTest) {
  Log tLog;
  tLog.set_log_level(LogLevel::SILENT);

  EXPECT_EXIT(tLog.AddLog(LogLevel::INFO, "Some info"),
              ExitedWithCode(EXIT_SUCCESS),
              "");

  EXPECT_EXIT(tLog.AddLog(LogLevel::FATAL, "FATAL error message"),
              ExitedWithCode(EXIT_SUCCESS),
              "");
}
TEST(AddLogTests, AddInvalidLogTest) {
  Log tLog;
  tLog.set_log_level(LogLevel::SILENT);

  EXPECT_EXIT(tLog.AddLog((LogLevel)(100500), "You should not see this message"),
              ExitedWithCode(EXIT_FAILURE),
              "");

  EXPECT_EXIT(tLog.AddLog(LogLevel::LOG_LEVEL_COUNT,
                          "You should not see this message"),
              ExitedWithCode(EXIT_FAILURE),
              "");
}
