#include <compare>

#include "gtest/gtest.h"

#include "SCC/config/scc_mode.h"
#include "SCC/fixtures_common/enum.h"

using namespace scc::config;
using namespace testing;

using UChar = unsigned char;

TEST(SCCModeValueEnumTests, IsUnsignedCharTest) {
  CastEnumTestBody<UChar, SCCMode::Value>(0);
  CastEnumTestBody<UChar, SCCMode::Value>(-1);
  CastEnumTestBody<UChar, SCCMode::Value>(50);
  CastEnumTestBody<UChar, SCCMode::Value>(128);
  CastEnumTestBody<UChar, SCCMode::Value>(256);
}

TEST(SCCModeCtorTests, DefaultValueTest) {
  SCCMode mode;
  EXPECT_EQ(SCCMode::kInteractive, (SCCMode::Value) mode);
}

TEST(SCCModeCtorTests, ValueTest) {
  CastWrapperToValueTestBody<SCCMode, SCCMode::Value>(SCCMode::kDaemon);
  CastWrapperToValueTestBody<SCCMode, SCCMode::Value>(SCCMode::kInteractive);
}

TEST(SCCModeCtorTests, InvalidValueTest) {
  CreateWithInvalidArgumentTestBody<SCCMode, SCCMode::Value>(-1);
  CreateWithInvalidArgumentTestBody<SCCMode, SCCMode::Value>(10);
}

TEST(SCCModeCtorTests, InvalidStringValueTest) {
  CreateWithInvalidArgumentTestBody<SCCMode, SCCMode::Value>("invalid mode");
  CreateWithInvalidArgumentTestBody<SCCMode, SCCMode::Value>("dakmon");
}

TEST(SCCModeCtorTests, StringValueTest) {
  CreateFromStringTestBody<SCCMode>(kMode_Interactive, SCCMode::kInteractive);
  CreateFromStringTestBody<SCCMode>(kMode_Daemon, SCCMode::kDaemon);
}

TEST(DataTypeCtorTests, MixedCaseStringTest) {
  CreateFromStringTestBody<SCCMode>("DaeMON", SCCMode::kDaemon);
  CreateFromStringTestBody<SCCMode>("interACTIVE", SCCMode::kInteractive);
}

TEST(SCCModeoStringTests, ToStringTest) {
  ToStringTestBody<SCCMode>(SCCMode::kDaemon, kMode_Daemon);
  ToStringTestBody<SCCMode>(SCCMode::kInteractive, kMode_Interactive);
}

TEST(SCCModeOperatorsTests, OutputTest) {
  OutputTestBody<SCCMode, SCCMode::Value>(SCCMode::kInteractive, kMode_Interactive);
  OutputTestBody<SCCMode, SCCMode::Value>(SCCMode::kDaemon, kMode_Daemon);
}

TEST(SCCModeOperatorsTests, CompareTwoEqualModesTest) {
  SCCMode first(SCCMode::kDaemon);
  SCCMode second(SCCMode::kDaemon);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(SCCModeOperatorsTests, CompareTwoDifferentModesTest) {
  SCCMode first(SCCMode::kInteractive);
  SCCMode second(SCCMode::kDaemon);

  EXPECT_TRUE(first != second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first < second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first >= second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::less, first <=> second);
  EXPECT_EQ(std::partial_ordering::greater, second <=> first);
}

TEST(SCCModeOperatorsTests, CompareWithEqualValueTest) {
  SCCMode mode(SCCMode::kDaemon);
  SCCMode::Value value = SCCMode::kDaemon;

  EXPECT_TRUE(mode == value);
  EXPECT_TRUE(mode <= value);
  EXPECT_TRUE(mode >= value);
  EXPECT_FALSE(mode != value);
  EXPECT_FALSE(mode < value);
  EXPECT_FALSE(mode > value);
}

TEST(SCCModeOperatorsTests, CompareWithDifferentValueTest) {
  SCCMode mode(SCCMode::kInteractive);
  SCCMode::Value value = SCCMode::kDaemon;

  EXPECT_TRUE(mode != value);
  EXPECT_TRUE(mode <= value);
  EXPECT_TRUE(mode < value);
  EXPECT_FALSE(mode == value);
  EXPECT_FALSE(mode >= value);
  EXPECT_FALSE(mode > value);
}
