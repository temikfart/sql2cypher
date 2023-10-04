#include "gtest/gtest.h"

#include "SCC/common/string_utils.h"

using namespace scc;
using namespace testing;

#define CAMEL_CASE_STR "CamelCase"
#define CAMEL_CASE_STR_L "camelcase"

#define SNAKE_CASE_STR "SNAKE_CASE"
#define SNAKE_CASE_STR_L "snake_case"

#define WITH_SYMBOLS_STR "W1Th_//_SYMB0ls"
#define WITH_SYMBOLS_STR_L "w1th_//_symb0ls"

#define UPPER_CASE_STR "UPPERCASE"
#define UPPER_CASE_STR_L "uppercase"

TEST(LowerCaseTests, EmptyStringTest) {
  EXPECT_EQ("", common::LowerCase(""));
}

TEST(LowerCaseTests, RValueStringTest) {
  EXPECT_EQ(CAMEL_CASE_STR_L, common::LowerCase(CAMEL_CASE_STR));
  EXPECT_EQ(SNAKE_CASE_STR_L, common::LowerCase(SNAKE_CASE_STR));
  EXPECT_EQ(WITH_SYMBOLS_STR_L, common::LowerCase(WITH_SYMBOLS_STR));
  EXPECT_EQ(UPPER_CASE_STR_L, common::LowerCase(UPPER_CASE_STR));
}

TEST(LowerCaseTests, LValueStringTest) {
  std::string camel_case = CAMEL_CASE_STR;
  EXPECT_EQ(CAMEL_CASE_STR_L, common::LowerCase(camel_case));

  std::string snake_case = SNAKE_CASE_STR;
  EXPECT_EQ(SNAKE_CASE_STR_L, common::LowerCase(snake_case));

  std::string with_symbols = WITH_SYMBOLS_STR;
  EXPECT_EQ(WITH_SYMBOLS_STR_L, common::LowerCase(with_symbols));

  std::string upper_case = UPPER_CASE_STR;
  EXPECT_EQ(UPPER_CASE_STR_L, common::LowerCase(upper_case));
}
