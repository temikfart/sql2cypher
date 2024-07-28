#include <compare>

#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"
#include "SCC/fixtures_common/enum.h"

using namespace scc::ast;
using namespace testing;

using UChar = unsigned char;

TEST(DataTypeValueEnumTests, IsUnsignedCharTest) {
  CastEnumTestBody<UChar, DataType::Value>(0);
  CastEnumTestBody<UChar, DataType::Value>(-1);
  CastEnumTestBody<UChar, DataType::Value>(50);
  CastEnumTestBody<UChar, DataType::Value>(128);
  CastEnumTestBody<UChar, DataType::Value>(256);
}

TEST(DataTypeCtorTests, DefaultValueTest) {
  DataType type;
  EXPECT_EQ(DataType::kNone, (DataType::Value) type);
}

TEST(DataTypeCtorTests, ValueTest) {
  CastWrapperToValueTestBody<DataType, DataType::Value>(DataType::kNone);
  CastWrapperToValueTestBody<DataType, DataType::Value>(DataType::kRoot);
  CastWrapperToValueTestBody<DataType, DataType::Value>(DataType::kInt);
  CastWrapperToValueTestBody<DataType, DataType::Value>(DataType::kBracket);
  CastWrapperToValueTestBody<DataType, DataType::Value>(DataType::kString);
}

TEST(DataTypeCtorTests, InvalidValueTest) {
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>(-1);
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>(100);
}

TEST(DataTypeCtorTests, InvalidStringValueTest) {
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>("something wrong");
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>("oa8sdg");
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>("ro0t");
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>("non");
  CreateWithInvalidArgumentTestBody<DataType, DataType::Value>("braket");
}

TEST(DataTypeCtorTests, StringValueTest) {
  CreateFromStringTestBody<DataType>(kDT_None, DataType::kNone);
  CreateFromStringTestBody<DataType>(kDT_Root, DataType::kRoot);
  CreateFromStringTestBody<DataType>(kDT_Service, DataType::kService);
  CreateFromStringTestBody<DataType>(kDT_Int, DataType::kInt);
  CreateFromStringTestBody<DataType>(kDT_Float, DataType::kFloat);
  CreateFromStringTestBody<DataType>(kDT_Bracket, DataType::kBracket);
  CreateFromStringTestBody<DataType>(kDT_Punctuation, DataType::kPunctuation);
  CreateFromStringTestBody<DataType>(kDT_Operator, DataType::kOperator);
  CreateFromStringTestBody<DataType>(kDT_Word, DataType::kWord);
  CreateFromStringTestBody<DataType>(kDT_String, DataType::kString);
}

TEST(DataTypeCtorTests, MixedCaseStringTest) {
  CreateFromStringTestBody<DataType>("NoNe", DataType::kNone);
  CreateFromStringTestBody<DataType>("RoOT", DataType::kRoot);
  CreateFromStringTestBody<DataType>("service", DataType::kService);
  CreateFromStringTestBody<DataType>("iNT", DataType::kInt);
  CreateFromStringTestBody<DataType>("flOAT", DataType::kFloat);
  CreateFromStringTestBody<DataType>("BRAcket", DataType::kBracket);
  CreateFromStringTestBody<DataType>("PuNcTuAtIoN", DataType::kPunctuation);
  CreateFromStringTestBody<DataType>("OperaTOR", DataType::kOperator);
  CreateFromStringTestBody<DataType>("WORD", DataType::kWord);
  CreateFromStringTestBody<DataType>("STRINg", DataType::kString);
}

TEST(DataTypeStringTests, ToStringTest) {
  ToStringTestBody<DataType>(DataType::kNone, kDT_None);
  ToStringTestBody<DataType>(DataType::kRoot, kDT_Root);
  ToStringTestBody<DataType>(DataType::kService, kDT_Service);
  ToStringTestBody<DataType>(DataType::kInt, kDT_Int);
  ToStringTestBody<DataType>(DataType::kFloat, kDT_Float);
  ToStringTestBody<DataType>(DataType::kBracket, kDT_Bracket);
  ToStringTestBody<DataType>(DataType::kPunctuation, kDT_Punctuation);
  ToStringTestBody<DataType>(DataType::kOperator, kDT_Operator);
  ToStringTestBody<DataType>(DataType::kWord, kDT_Word);
  ToStringTestBody<DataType>(DataType::kString, kDT_String);
}

TEST(DataTypeOperatorsTests, OutputTest) {
  OutputTestBody<DataType, DataType::Value>(DataType::kNone, kDT_None);
  OutputTestBody<DataType, DataType::Value>(DataType::kPunctuation, kDT_Punctuation);
  OutputTestBody<DataType, DataType::Value>(DataType::kOperator, kDT_Operator);
  OutputTestBody<DataType, DataType::Value>(DataType::kString, kDT_String);
}

TEST(DataTypeOperatorsTests, CompareTwoEqualTypesTest) {
  DataType first(DataType::kInt);
  DataType second(DataType::kInt);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(DataTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  DataType first(DataType::kInt);
  DataType second(DataType::kBracket);

  EXPECT_TRUE(first != second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first < second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first >= second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::less, first <=> second);
  EXPECT_EQ(std::partial_ordering::greater, second <=> first);
}

TEST(DataTypeOperatorsTests, CompareWithEqualValueTest) {
  DataType type(DataType::kService);
  DataType::Value value = DataType::kService;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(DataTypeOperatorsTests, CompareWithDifferentValueTest) {
  DataType type(DataType::kRoot);
  DataType::Value value = DataType::kPunctuation;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
