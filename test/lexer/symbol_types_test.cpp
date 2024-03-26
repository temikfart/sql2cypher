#include <sstream>
#include <stdexcept>

#include "gtest/gtest.h"

#include "SCC/lexer/symbol_types.h"

//using namespace scc;
using namespace testing;

TEST(SymbolTypeValueEnumTests, IsUnsignedCharTest) {
  int N_0 = 0;
  EXPECT_EQ((unsigned char) N_0, SymbolType::Value(N_0));

  int N_neg1 = -1;
  EXPECT_EQ((unsigned char) N_neg1, SymbolType::Value(N_neg1));

  int N_50 = 50;
  EXPECT_EQ((unsigned char) N_50, SymbolType::Value(N_50));

  int N_128 = 128;
  EXPECT_EQ((unsigned char) N_128, SymbolType::Value(N_128));

  int N_256 = 256;
  EXPECT_EQ((unsigned char) N_256, SymbolType::Value(N_256));
}

TEST(SymbolTypeCtorTests, DefaultValueTest) {
  SymbolType type;
  EXPECT_EQ(SymbolType::kUnknown, (SymbolType::Value) type);
}

TEST(SymbolTypeCtorTests, ValueTest) {
  EXPECT_EQ(SymbolType::kUnknown, (SymbolType::Value) SymbolType(SymbolType::kUnknown));
  EXPECT_EQ(SymbolType::kSpace, (SymbolType::Value) SymbolType(SymbolType::kSpace));
  EXPECT_EQ(SymbolType::kAlpha, (SymbolType::Value) SymbolType(SymbolType::kAlpha));
  EXPECT_EQ(SymbolType::kBracket, (SymbolType::Value) SymbolType(SymbolType::kBracket));
  EXPECT_EQ(SymbolType::kEOF, (SymbolType::Value) SymbolType(SymbolType::kEOF));
}

TEST(SymbolTypeCtorTests, InvalidValueTest) {
  EXPECT_THROW(SymbolType(SymbolType::Value(-1)), std::invalid_argument);
  EXPECT_THROW(SymbolType(SymbolType::Value(100)), std::invalid_argument);
}

TEST(SymbolTypeCtorTests, StringValueTest) {
  SymbolType space(SYMT_SPACE);
  EXPECT_EQ(SymbolType::kSpace, (SymbolType::Value) space);

  SymbolType operator_(SYMT_OPERATOR);
  EXPECT_EQ(SymbolType::kOperator, (SymbolType::Value) operator_);

  SymbolType mixed_case_bracket("brAcKEt");
  EXPECT_EQ(SymbolType::kBracket, (SymbolType::Value) mixed_case_bracket);
}

TEST(SymbolTypeCtorTests, InvalidStringValueTest) {
  EXPECT_THROW(SymbolType("invalid value"), std::invalid_argument);
  EXPECT_THROW(SymbolType("asd9g8"), std::invalid_argument);
  EXPECT_THROW(SymbolType("br@cket"), std::invalid_argument);
  EXPECT_THROW(SymbolType("abcd"), std::invalid_argument);
  EXPECT_THROW(SymbolType("punctuatio"), std::invalid_argument);
}

TEST(SymbolTypeoStringTests, ToStringTest) {
  EXPECT_EQ(SYMT_UNKNOWN, SymbolType(SymbolType::kUnknown).ToString());
  EXPECT_EQ(SYMT_ALPHA, SymbolType(SymbolType::kAlpha).ToString());
  EXPECT_EQ(SYMT_BRACKET, SymbolType(SymbolType::kBracket).ToString());
  EXPECT_EQ(SYMT_NULL_TERMINATOR, SymbolType(SymbolType::kNullTerminator).ToString());
  EXPECT_EQ(SYMT_EOF, SymbolType(SymbolType::kEOF).ToString());
}

TEST(SymbolTypeCastTests, CastToValueTest) {
  SymbolType eof(SymbolType::kEOF);
  auto eof_val = (SymbolType::Value) eof;
  EXPECT_EQ(SymbolType::kEOF, eof_val);

  SymbolType operator_(SymbolType::kOperator);
  auto operator_val = (SymbolType::Value) operator_;
  EXPECT_EQ(SymbolType::kOperator, operator_val);
}

TEST(SymbolTypeOperatorsTests, OutputTest) {
  std::ostringstream oss_unknown;
  oss_unknown << SymbolType(SymbolType::kUnknown);
  EXPECT_EQ(SYMT_UNKNOWN, oss_unknown.str());

  std::ostringstream oss_digit;
  oss_digit << SymbolType(SymbolType::kDigit);
  EXPECT_EQ(SYMT_DIGIT, oss_digit.str());

  std::ostringstream oss_space;
  oss_space << SymbolType(SymbolType::kSpace);
  EXPECT_EQ(SYMT_SPACE, oss_space.str());

  std::ostringstream oss_alpha;
  oss_alpha << SymbolType(SymbolType::kAlpha);
  EXPECT_EQ(SYMT_ALPHA, oss_alpha.str());
}

TEST(SymbolTypeOperatorsTests, CompareTwoEqualTypesTest) {
  SymbolType first(SymbolType::kSpace);
  SymbolType second(SymbolType::kSpace);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(SymbolTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  SymbolType first(SymbolType::kDigit);
  SymbolType second(SymbolType::kOperator);

  EXPECT_TRUE(first != second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first < second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first >= second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::less, first <=> second);
  EXPECT_EQ(std::partial_ordering::greater, second <=> first);
}

TEST(SymbolTypeOperatorsTests, CompareWithEqualValueTest) {
  SymbolType type(SymbolType::kBracket);
  SymbolType::Value value = SymbolType::kBracket;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(SymbolTypeOperatorsTests, CompareWithDifferentValueTest) {
  SymbolType type(SymbolType::kOperator);
  SymbolType::Value value = SymbolType::kEOF;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
