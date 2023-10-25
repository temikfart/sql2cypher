#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/config/scc_config.h"
#include "SCC/fixtures/scc_config_fixtures.h"

using namespace scc;
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

