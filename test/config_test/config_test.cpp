#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/config.h"

using namespace testing;

TEST(SetGetConfigTests, DefaultConfigTest) {
  Config tConf;
  
  EXPECT_EQ(SCCMode::INTERACTIVE, tConf.get_mode())
    << "default mode should be INTERACTIVE";

  const std::string ConfigPath = tConf.GetConfigPath();
  std::string SQL_path = ConfigPath + "../resources/sql_queries.sql";
  std::string Cypher_path = ConfigPath + "../resources/cypher_queries.cypher";

  EXPECT_EQ(SQL_path, tConf.get_sql_path())
    << "default sql_path should be \"" << SQL_path << "\"";
  EXPECT_EQ(Cypher_path, tConf.get_cypher_path())
    << "default cypher_path should be \"" << Cypher_path << "\"";
}
TEST(SetGetConfigTests, ValidModeTest) {
  Config tConf;

  tConf.set_mode(SCCMode::INTERACTIVE);
  EXPECT_EQ(SCCMode::INTERACTIVE, tConf.get_mode());

  tConf.set_mode(SCCMode::DAEMON);
  EXPECT_EQ(SCCMode::DAEMON, tConf.get_mode());
}
TEST(SetGetConfigTests, InvalidModeTest) {
  Config tConf;

  EXPECT_EXIT(tConf.set_mode(SCCMode::SCCMODE_COUNT),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "SCCMODE_COUNT should not be set";

  EXPECT_EXIT(tConf.set_mode(SCCMode(100500)),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "100500 mode should not be set";
}
TEST(SetGetConfigTests, SQLPathTest) {
  Config tConf;

  std::string SQL_path = tConf.GetConfigPath()
                         + "../resources/config_test_resources/";
  std::string Valid_SQL_path = SQL_path + "test_sql_queries.sql";
  std::string Invalid_SQL_path = SQL_path + "invalid.sql";

  tConf.set_sql_path(Valid_SQL_path);
  EXPECT_EQ(Valid_SQL_path, tConf.get_sql_path())
                << Valid_SQL_path << " should be set as SQL path";

  EXPECT_EXIT(tConf.set_sql_path(Invalid_SQL_path),
              ExitedWithCode(EXIT_FAILURE),
              "")
                << Invalid_SQL_path
                << " does not exist, so should not be set as SQL path";
}
TEST(SetGetConfigTests, CypherPathTest) {
  Config tConf;

  std::string Cypher_path = tConf.GetConfigPath()
                            + "../resources/config_test_resources/";
  std::string Valid_Cypher_path = Cypher_path + "test_cypher_queries.cypher";

  tConf.set_cypher_path(Valid_Cypher_path);
  EXPECT_EQ(Valid_Cypher_path, tConf.get_cypher_path())
                << Valid_Cypher_path << " should be set as Cypher path";
}
TEST(ConvertConfigTests, ValidStringToModeTest) {
  Config tConf;

  EXPECT_THAT(tConf.StringToSCCMode("InteRaCTivE"),
              Eq(SCCMode::INTERACTIVE));
  EXPECT_THAT(tConf.StringToSCCMode("DaeMOn"),
              Eq(SCCMode::DAEMON));
}
TEST(ConvertConfigTests, InvalidStringToModeTest) {
  Config tConf;

  EXPECT_EXIT(tConf.StringToSCCMode(""),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "empty string should not be converted to SCC mode";

  EXPECT_EXIT(tConf.StringToSCCMode("Inter"),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "\"Inter\" should not be converted to SCC mode";

  EXPECT_EXIT(tConf.StringToSCCMode("DAEMO"),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "\"DAEMO\" should not be converted to SCC mode";
}
TEST(ConvertConfigTests, ValidModeToStringTest) {
  Config tConf;

  EXPECT_THAT(tConf.SCCModeToString(SCCMode::INTERACTIVE),
              Eq("INTERACTIVE"));
  EXPECT_THAT(tConf.SCCModeToString(SCCMode::DAEMON),
              Eq("DAEMON"));
}
TEST(ConvertConfigTests, InvalidModeToStringTest) {
  Config tConf;

  EXPECT_EXIT(tConf.SCCModeToString(SCCMode::SCCMODE_COUNT),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "SCCMODE_COUNT should not be converted to string";

  EXPECT_EXIT(tConf.SCCModeToString((SCCMode)(100500)),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "100500 should not be converted to string";
}
// TODO: come up with tests for GetConsoleArguments() method.
//TEST(GetConsoleArgumentsTests, WithoutArguments) {}
//TEST(GetConsoleArgumentsTests, ValidSingleArgumentTest) {}
//TEST(GetConsoleArgumentsTests, InvalidSingleArgumentTest) {}
//TEST(GetConsoleArgumentsTests, ValidMultipleArgumentTest) {}
//TEST(GetConsoleArgumentsTests, DuplicatedArgumentTest) {}
