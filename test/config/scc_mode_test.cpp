#include <compare>
#include <sstream>
#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/config/scc_mode.h"

using namespace scc;
using namespace testing;

TEST(SCCModeValueEnumTests, IsUnsignedCharTest) {
  int N_0 = 0;
  EXPECT_EQ((unsigned char) N_0, SCCMode::Value(N_0));

  int N_neg1 = -1;
  EXPECT_EQ((unsigned char) N_neg1, SCCMode::Value(N_neg1));

  int N_50 = 50;
  EXPECT_EQ((unsigned char) N_50, SCCMode::Value(N_50));

  int N_128 = 128;
  EXPECT_EQ((unsigned char) N_128, SCCMode::Value(N_128));

  int N_256 = 256;
  EXPECT_EQ((unsigned char) N_256, SCCMode::Value(N_256));
}

TEST(SCCModeCtorTests, DefaultValueTest) {
  SCCMode mode;
  EXPECT_EQ(SCCMode::kInteractive, (SCCMode::Value) mode);
}

TEST(SCCModeCtorTests, ValueTest) {
  EXPECT_EQ(SCCMode::kDaemon, (SCCMode::Value) SCCMode(SCCMode::kDaemon));
  EXPECT_EQ(SCCMode::kInteractive, (SCCMode::Value) SCCMode(SCCMode::Value(0)));
}

TEST(SCCModeCtorTests, InvalidValueTest) {
  EXPECT_THROW(SCCMode(SCCMode::Value(-1)), std::invalid_argument);
  EXPECT_THROW(SCCMode(SCCMode::Value(10)), std::invalid_argument);
}

TEST(SCCModeCtorTests, StringValueTest) {
  SCCMode interactive(INTERACTIVE);
  EXPECT_EQ(SCCMode::kInteractive, (SCCMode::Value) interactive);

  SCCMode daemon(DAEMON);
  EXPECT_EQ(SCCMode::kDaemon, (SCCMode::Value) daemon);

  SCCMode mixed_case_daemon("DaeMON");
  EXPECT_EQ(SCCMode::kDaemon, (SCCMode::Value) mixed_case_daemon);
}

TEST(SCCModeCtorTests, InvalidStringValueTest) {
  EXPECT_THROW(SCCMode("invalid mode"), std::invalid_argument);
  EXPECT_THROW(SCCMode("dakmon"), std::invalid_argument);
}

TEST(SCCModeoStringTests, ToStringTest) {
  EXPECT_EQ(DAEMON, SCCMode(SCCMode::kDaemon).ToString());
  EXPECT_EQ(INTERACTIVE, SCCMode(SCCMode::kInteractive).ToString());
}

TEST(SCCModeCastTests, CastToValueTest) {
  SCCMode interactive(SCCMode::kInteractive);
  auto interactive_val = (SCCMode::Value) interactive;
  EXPECT_EQ(SCCMode::kInteractive, interactive_val);

  SCCMode daemon(SCCMode::kDaemon);
  auto daemon_val = (SCCMode::Value) daemon;
  EXPECT_EQ(SCCMode::kDaemon, daemon_val);
}

TEST(SCCModeOperatorsTests, OutputTest) {
  std::ostringstream oss_interactive;
  oss_interactive << SCCMode(SCCMode::kInteractive);
  EXPECT_EQ(INTERACTIVE, oss_interactive.str());

  std::ostringstream oss_daemon;
  oss_daemon << SCCMode(SCCMode::kDaemon);
  EXPECT_EQ(DAEMON, oss_daemon.str());
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
