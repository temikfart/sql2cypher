#include <compare>

#include "gtest/gtest.h"

#include "SCC/fixtures_common/enum.h"
#include "SCC/lexer/symbol_types.h"

using namespace scc::lexer;
using namespace testing;

using UChar = unsigned char;

TEST(SymbolTypeValueEnumTests, IsUnsignedCharTest) {
  CastEnumTestBody<UChar, SymbolType::Value>(0);
  CastEnumTestBody<UChar, SymbolType::Value>(-1);
  CastEnumTestBody<UChar, SymbolType::Value>(50);
  CastEnumTestBody<UChar, SymbolType::Value>(128);
  CastEnumTestBody<UChar, SymbolType::Value>(256);
}

TEST(SymbolTypeCtorTests, DefaultValueTest) {
  SymbolType type;
  EXPECT_EQ(SymbolType::kUnknown, (SymbolType::Value) type);
}

TEST(SymbolTypeCtorTests, ValueTest) {
  CastWrapperToValueTestBody<SymbolType, SymbolType::Value>(SymbolType::kUnknown);
  CastWrapperToValueTestBody<SymbolType, SymbolType::Value>(SymbolType::kSpace);
  CastWrapperToValueTestBody<SymbolType, SymbolType::Value>(SymbolType::kAlpha);
  CastWrapperToValueTestBody<SymbolType, SymbolType::Value>(SymbolType::kBracket);
  CastWrapperToValueTestBody<SymbolType, SymbolType::Value>(SymbolType::kEOF);
}

TEST(SymbolTypeCtorTests, InvalidValueTest) {
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>(-1);
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>(100);
}

TEST(SymbolTypeCtorTests, InvalidStringValueTest) {
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>("invalid value");
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>("asd9g8");
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>("br@cket");
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>("abcd");
  CreateWithInvalidArgumentTestBody<SymbolType, SymbolType::Value>("punctuatio");
}

TEST(SymbolTypeCtorTests, StringValueTest) {
  CreateFromStringTestBody<SymbolType>(kSYMT_Unknown, SymbolType::kUnknown);
  CreateFromStringTestBody<SymbolType>(kSYMT_Space, SymbolType::kSpace);
  CreateFromStringTestBody<SymbolType>(kSYMT_Digit, SymbolType::kDigit);
  CreateFromStringTestBody<SymbolType>(kSYMT_Alpha, SymbolType::kAlpha);
  CreateFromStringTestBody<SymbolType>(kSYMT_Operator, SymbolType::kOperator);
  CreateFromStringTestBody<SymbolType>(kSYMT_Bracket, SymbolType::kBracket);
  CreateFromStringTestBody<SymbolType>(kSYMT_Punctuation, SymbolType::kPunctuation);
  CreateFromStringTestBody<SymbolType>(kSYMT_EOF, SymbolType::kEOF);
  CreateFromStringTestBody<SymbolType>(kSYMT_NullTerminator, SymbolType::kNullTerminator);
}

TEST(SymbolTypeCtorTests, MixedCaseStringTest) {
  CreateFromStringTestBody<SymbolType>("unKNOWN", SymbolType::kUnknown);
  CreateFromStringTestBody<SymbolType>("SpAcE", SymbolType::kSpace);
  CreateFromStringTestBody<SymbolType>("DIGIT", SymbolType::kDigit);
  CreateFromStringTestBody<SymbolType>("alpha", SymbolType::kAlpha);
  CreateFromStringTestBody<SymbolType>("OPERator", SymbolType::kOperator);
  CreateFromStringTestBody<SymbolType>("brAcKEt", SymbolType::kBracket);
  CreateFromStringTestBody<SymbolType>("PUNCTuation", SymbolType::kPunctuation);
  CreateFromStringTestBody<SymbolType>("Eof", SymbolType::kEOF);
  CreateFromStringTestBody<SymbolType>("NULl", SymbolType::kNullTerminator);
}

TEST(SymbolTypeStringTests, ToStringTest) {
  ToStringTestBody<SymbolType>(SymbolType::kUnknown, kSYMT_Unknown);
  ToStringTestBody<SymbolType>(SymbolType::kSpace, kSYMT_Space);
  ToStringTestBody<SymbolType>(SymbolType::kDigit, kSYMT_Digit);
  ToStringTestBody<SymbolType>(SymbolType::kAlpha, kSYMT_Alpha);
  ToStringTestBody<SymbolType>(SymbolType::kOperator, kSYMT_Operator);
  ToStringTestBody<SymbolType>(SymbolType::kBracket, kSYMT_Bracket);
  ToStringTestBody<SymbolType>(SymbolType::kPunctuation, kSYMT_Punctuation);
  ToStringTestBody<SymbolType>(SymbolType::kEOF, kSYMT_EOF);
  ToStringTestBody<SymbolType>(SymbolType::kNullTerminator, kSYMT_NullTerminator);
}

TEST(SymbolTypeOperatorsTests, OutputTest) {
  OutputTestBody<SymbolType, SymbolType::Value>(SymbolType::kUnknown, kSYMT_Unknown);
  OutputTestBody<SymbolType, SymbolType::Value>(SymbolType::kDigit, kSYMT_Digit);
  OutputTestBody<SymbolType, SymbolType::Value>(SymbolType::kSpace, kSYMT_Space);
  OutputTestBody<SymbolType, SymbolType::Value>(SymbolType::kAlpha, kSYMT_Alpha);
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
