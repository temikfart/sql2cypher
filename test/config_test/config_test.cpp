#include "gtest/gtest.h"
#include "SCC/config.h"

TEST(SetGetConfigTests, DefaultModeTest) {
  Config tConf;
  
  EXPECT_EQ(SCCMode::INTERACTIVE, tConf.get_mode())
    << "default mode should be INTERACTIVE";
}
TEST(SetGetConfigTests, SetGetModeTest) {
  Config tConf;
  
  SCCMode tType = SCCMode::INTERACTIVE;
  tConf.set_mode(tType);
  ASSERT_EQ(tType, tConf.get_mode());
  
  tType = SCCMode::DAEMON;
  tConf.set_mode(tType);
  ASSERT_EQ(tType, tConf.get_mode());
}
//TODO: catch EXIT_FAILURE from ValidateMode()
TEST(SetGetConfigTests, DISABLED_InvalidModeTest) {
  Config tConf;
  
  SCCMode tType = SCCMode::SCCMODE_COUNT;
  tConf.set_mode(tType);
  ASSERT_NE(tType, tConf.get_mode())
    << "SCCMODE_COUNT should not be set";
  
  int t = SCCMode::SCCMODE_COUNT + 5;
  tConf.set_mode(static_cast<SCCMode>(t));
  ASSERT_NE(t, tConf.get_mode())
    << "invalid mode should not be set";
}

TEST(SetGetConfigTests, DefaultSQLPathTest) {
  Config tConf;
  
  std::string default_SQL_path =
    tConf.GetConfigPath() + "../resources/sql_queries.sql";
  EXPECT_EQ(default_SQL_path, tConf.get_sql_path())
    << "default SQL path should be";
}
TEST(SetGetConfigTests, SetGetSQLPathTest) {
  Config tConf;
  
  std::string tSQL_path = tConf.GetConfigPath();
  tSQL_path += "../resources/config_test_resources/";
  
  std::string tSQL_path_1 = tSQL_path + "test_sql_queries.sql";
  tConf.set_sql_path(tSQL_path_1);
  ASSERT_EQ(tSQL_path_1, tConf.get_sql_path());
}
// TODO: catch EXIT_FAILURE from ValidateSQLPath()
TEST(SetGetConfigTests, DISABLED_InvalidSQLPathTest) {
  Config tConf;
  
  std::string tSQL_path = tConf.GetConfigPath();
  tSQL_path += "../resources/config_test_resources/";
  
  std::string tSQL_path_2 = tSQL_path + "unexisting.sql";
  tConf.set_sql_path(tSQL_path_2);
  ASSERT_NE(tSQL_path_2, tConf.get_sql_path())
    << "invalid SQL path should not be set";
}
