#include <filesystem>
#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/config/scc_config.h"
#include "SCC/config/scc_mode.h"
#include "SCC/fixtures/scc_config_fixtures.h"
#include "SCC/log.h"

using namespace scc::config;
using namespace testing;

namespace fs = std::filesystem;

TEST_F(SCCConfigBaseTests, GetUninitializedConfigTest) {
  EXPECT_THROW(SCCConfig::Get(), std::runtime_error);
}
TEST_F(SCCConfigBaseTests, GetConfigTest) {
  EXPECT_NO_THROW(InitConfig(parser));
  EXPECT_NO_THROW(
      config = SCCConfig::Get();
      EXPECT_NE(config, nullptr);
  );
}
TEST_F(SCCConfigBaseTests, GetEqualConfigsTest) {
  EXPECT_NO_THROW(InitConfig(parser));
  EXPECT_NO_THROW(
      SCCConfig* config1 = SCCConfig::Get();
      SCCConfig* config2 = SCCConfig::Get();
      EXPECT_EQ(config1, config2);
  );
}

TEST_F(SCCConfigBaseTests, InitConfigTest) {
  EXPECT_NO_THROW(InitConfig(parser));
}
TEST_F(SCCConfigBaseTests, DoubleInitConfigTest) {
  EXPECT_NO_THROW(
      InitConfig(parser);
      InitConfig(parser);
  );
}

TEST_F(SCCConfigTests, DefaultConfigTest) {
  logger::Severity default_log_severity = logger::Severity::info;
  EXPECT_EQ(default_log_severity, config->log_severity);

  fs::path default_log_directory = fs::weakly_canonical(SCC_LOG_DIR);
  EXPECT_EQ(default_log_directory, config->log_directory);

  SCCMode default_mode = SCCMode::kInteractive;
  EXPECT_EQ(default_mode, config->mode);

  fs::path default_sql_file = fs::canonical(sql_path);
  EXPECT_EQ(default_sql_file, config->get_sql_file());

  fs::path default_cypher_file = fs::current_path() / "out.cypher";
  EXPECT_EQ(default_cypher_file, config->get_cypher_file());

  EXPECT_TRUE(config->get_ast_dump_file().empty());
}
TEST_F(CustomSCCConfigTests, CustomConfigTest) {
  std::string custom_sql_file = sql_path;
  AddArg("--sql", custom_sql_file);

  std::string custom_log_severity = std::string{logger::to_string(logger::Severity::fatal)};
  AddArg("--log-severity", custom_log_severity);

  std::string custom_log_directory = fs::current_path().string();
  AddArg("--log-directory", custom_log_directory);

  std::string custom_mode = SCCMode{SCCMode::kDaemon}.ToString();
  AddArg("--mode", custom_mode);

  std::string custom_cypher_file = (fs::current_path() / "custom.cypher").string();
  AddArg("--cypher", custom_cypher_file);

  EXPECT_NO_THROW(
      ParseArgsWrapper();
      InitializeConfig();
  );

  EXPECT_EQ(fs::canonical(custom_sql_file), config->get_sql_file());
  EXPECT_EQ(logger::to_severity(custom_log_severity), config->log_severity);
  EXPECT_EQ(fs::weakly_canonical(fs::path{custom_log_directory}), config->log_directory);
  EXPECT_EQ(SCCMode{custom_mode}, config->mode);
  EXPECT_EQ(fs::weakly_canonical(fs::path{custom_cypher_file}), config->get_cypher_file());
}
TEST_F(CustomSCCConfigTests, ConfigWithUnexistedSqlPathTest) {
  AddArg("--sql", "");

  EXPECT_NO_THROW(ParseArgsWrapper());
  EXPECT_THROW(InitializeConfig(), std::runtime_error);
}
