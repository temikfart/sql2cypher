#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SCC/config.h"

#include <cstdlib>
#include <cstring>

using namespace testing;

TEST(SetGetConfigTests, DefaultConfigTest) {
  Config tConf;
  
  EXPECT_EQ(SCCMode::kInteractive, tConf.get_mode())
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

  tConf.set_mode(SCCMode::kInteractive);
  EXPECT_EQ(SCCMode::kInteractive, tConf.get_mode());

  tConf.set_mode(SCCMode::kDaemon);
  EXPECT_EQ(SCCMode::kDaemon, tConf.get_mode());
}
TEST(SetGetConfigTests, InvalidModeTest) {
  Config tConf;

  EXPECT_EXIT(tConf.set_mode(SCCMode::kSCCModeCount),
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
  std::string Valid_SQL_path = SQL_path + "sql_path_test.sql";
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
  std::string Valid_Cypher_path = Cypher_path + "cypher_path_test.cypher";

  tConf.set_cypher_path(Valid_Cypher_path);
  EXPECT_EQ(Valid_Cypher_path, tConf.get_cypher_path())
                << Valid_Cypher_path << " should be set as Cypher path";
}

TEST(ConvertConfigTests, ValidStringToModeTest) {
  Config tConf;

  EXPECT_THAT(tConf.StringToSCCMode("InteRaCTivE"),
              Eq(SCCMode::kInteractive));
  EXPECT_THAT(tConf.StringToSCCMode("DaeMOn"),
              Eq(SCCMode::kDaemon));
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

  EXPECT_THAT(tConf.SCCModeToString(SCCMode::kInteractive),
              Eq("INTERACTIVE"));
  EXPECT_THAT(tConf.SCCModeToString(SCCMode::kDaemon),
              Eq("DAEMON"));
}
TEST(ConvertConfigTests, InvalidModeToStringTest) {
  Config tConf;

  EXPECT_EXIT(tConf.SCCModeToString(SCCMode::kSCCModeCount),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "SCCMODE_COUNT should not be converted to string";

  EXPECT_EXIT(tConf.SCCModeToString((SCCMode)(100500)),
              ExitedWithCode(EXIT_FAILURE),
              "")
    << "100500 should not be converted to string";
}

class GetConsoleArgumentsTests : public ::testing::Test {
protected:
  Config tConf;
  SCCMode def_mode;
  std::string def_sql_path;
  std::string def_cypher_path;

  int argc = 1;
  char* argv[10];


  void SetUp() override {
    def_mode = tConf.get_mode();
    def_sql_path = tConf.get_sql_path();
    def_cypher_path = tConf.get_cypher_path();

    for (auto & i : argv) {
      i = nullptr;
      i = (char*)(malloc(20));
    }
  }
  void TearDown() override {
    for (auto & i : argv) {
      free(i);
    }
  }
};
TEST_F(GetConsoleArgumentsTests, WithoutArguments) {
  argc = 1;
  strcpy(argv[0], "./SCC");

  tConf.GetConsoleArguments(argc, argv);

  EXPECT_THAT(tConf.get_mode(), Eq(def_mode));
  EXPECT_THAT(tConf.get_sql_path(), Eq(def_sql_path));
  EXPECT_THAT(tConf.get_cypher_path(), Eq(def_cypher_path));
}
// TODO: come up with how test GetConsoleArguments() method.
//TEST_F(GetConsoleArgumentsTests, ValidModeFlagTest1) {
//  argc = 2;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "--mode=inTEracTIVE");
//
//  tConf.GetConsoleArguments(argc, argv);
//
//  EXPECT_THAT(tConf.get_mode(), Eq(SCCMode::INTERACTIVE));
//  EXPECT_THAT(tConf.get_sql_path(), Eq(def_sql_path));
//  EXPECT_THAT(tConf.get_cypher_path(), Eq(def_cypher_path));
//}
//TEST_F(GetConsoleArgumentsTests, InvalidModeFlagTest1) {
//  argc = 2;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "--mode=inTEracT");
//
//  EXPECT_EXIT(tConf.GetConsoleArguments(argc, argv),
//              ExitedWithCode(1),
//              "")
//    << "\"inTEracT\" is an invalid mode";
//}
//TEST_F(GetConsoleArgumentsTests, ValidModeFlagTest2) {
//  argc = 3;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "--mode");
//  strcpy(argv[2], "dAeMON");
//
//  tConf.GetConsoleArguments(argc, argv);
//
//  EXPECT_THAT(tConf.get_mode(), Eq(SCCMode::DAEMON));
//  EXPECT_THAT(tConf.get_sql_path(), Eq(def_sql_path));
//  EXPECT_THAT(tConf.get_cypher_path(), Eq(def_cypher_path));
//}
//TEST_F(GetConsoleArgumentsTests, InvalidModeFlagTest2) {
//  argc = 3;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "--mode");
//
//  EXPECT_EXIT(tConf.GetConsoleArguments(argc, argv),
//              ExitedWithCode(EXIT_FAILURE),
//              "")
//    << "flag --mode must have an argument";
//}
//TEST_F(GetConsoleArgumentsTests, ValidModeFlagTest3) {
//  argc = 2;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "-m=interactive");
//
//  tConf.GetConsoleArguments(argc, argv);
//
//  EXPECT_THAT(tConf.get_mode(), Eq(SCCMode::INTERACTIVE));
//  EXPECT_THAT(tConf.get_sql_path(), Eq(def_sql_path));
//  EXPECT_THAT(tConf.get_cypher_path(), Eq(def_cypher_path));
//}
//TEST_F(GetConsoleArgumentsTests, InvalidModeFlagTest3) {
//  argc = 2;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "-m=nteractive");
//
//
//  EXPECT_EXIT(tConf.GetConsoleArguments(argc, argv),
//              ExitedWithCode(EXIT_FAILURE),
//              "")
//    << "\"nteractive\" is an invalid mode";
//}
//TEST_F(GetConsoleArgumentsTests, ValidModeFlagTest4) {
//  argc = 3;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "-m");
//  strcpy(argv[2], "DAEMON");
//
//  tConf.GetConsoleArguments(argc, argv);
//
//  EXPECT_THAT(tConf.get_mode(), Eq(SCCMode::DAEMON));
//  EXPECT_THAT(tConf.get_sql_path(), Eq(def_sql_path));
//  EXPECT_THAT(tConf.get_cypher_path(), Eq(def_cypher_path));
//}
//TEST_F(GetConsoleArgumentsTests, InvalidModeFlagTest4) {
//  argc = 3;
//  strcpy(argv[0], "./SCC");
//  strcpy(argv[1], "-m");
//
//  EXPECT_EXIT(tConf.GetConsoleArguments(argc, argv),
//              ExitedWithCode(EXIT_FAILURE),
//              "")
//    << "flag -m must have an argument";
//}

//TEST(GetConsoleArgumentsTests, InvalidSingleArgumentTest) {}
//TEST(GetConsoleArgumentsTests, ValidMultipleArgumentTest) {}
//TEST(GetConsoleArgumentsTests, DuplicatedArgumentTest) {}
