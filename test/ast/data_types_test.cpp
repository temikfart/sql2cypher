#include <sstream>

#include "gtest/gtest.h"

#include "SCC/ast/data_types.h"

//using namespace scc;
using namespace testing;

TEST(DataTypeValueEnumTests, IsUnsignedCharTest) {
  int N_0 = 0;
  EXPECT_EQ((unsigned char) N_0, DataType::Value(N_0));

  int N_neg1 = -1;
  EXPECT_EQ((unsigned char) N_neg1, DataType::Value(N_neg1));

  int N_50 = 50;
  EXPECT_EQ((unsigned char) N_50, DataType::Value(N_50));

  int N_128 = 128;
  EXPECT_EQ((unsigned char) N_128, DataType::Value(N_128));

  int N_256 = 256;
  EXPECT_EQ((unsigned char) N_256, DataType::Value(N_256));
}

TEST(DataTypeCtorTests, DefaultValueTest) {
  DataType type;
  EXPECT_EQ(DataType::kNone, (DataType::Value) type);
}

TEST(DataTypeCtorTests, ValueTest) {
  EXPECT_EQ(DataType::kNone, (DataType::Value) DataType(DataType::kNone));
  EXPECT_EQ(DataType::kRoot, (DataType::Value) DataType(DataType::kRoot));
  EXPECT_EQ(DataType::kInt, (DataType::Value) DataType(DataType::kInt));
  EXPECT_EQ(DataType::kBracket, (DataType::Value) DataType(DataType::kBracket));
  EXPECT_EQ(DataType::kString, (DataType::Value) DataType(DataType::kString));
}

TEST(DataTypeCtorTests, InvalidValueTest) {
  EXPECT_THROW(DataType(DataType::Value(-1)), std::invalid_argument);
  EXPECT_THROW(DataType(DataType::Value(100)), std::invalid_argument);
}

TEST(DataTypeCtorTests, StringValueTest) {
  DataType root(DT_ROOT);
  EXPECT_EQ(DataType::kRoot, (DataType::Value) root);

  DataType punctuation(DT_PUNCTUATION);
  EXPECT_EQ(DataType::kPunctuation, (DataType::Value) punctuation);

  DataType mixed_case_service("serViCE");
  EXPECT_EQ(DataType::kService, (DataType::Value) mixed_case_service);
}

TEST(DataTypeCtorTests, InvalidStringValueTest) {
  EXPECT_THROW(DataType("something wrong"), std::invalid_argument);
  EXPECT_THROW(DataType("oa8sdg"), std::invalid_argument);
  EXPECT_THROW(DataType("ro0t"), std::invalid_argument);
  EXPECT_THROW(DataType("non"), std::invalid_argument);
  EXPECT_THROW(DataType("braket"), std::invalid_argument);
}

TEST(DataTypeoStringTests, ToStringTest) {
  EXPECT_EQ(DT_NONE, DataType(DataType::kNone).ToString());
  EXPECT_EQ(DT_ROOT, DataType(DataType::kRoot).ToString());
  EXPECT_EQ(DT_INT, DataType(DataType::kInt).ToString());
  EXPECT_EQ(DT_SERVICE, DataType(DataType::kService).ToString());
  EXPECT_EQ(DT_WORD, DataType(DataType::kWord).ToString());
}

TEST(DataTypeCastTests, CastToValueTest) {
  DataType bracket(DataType::kBracket);
  auto bracket_val = (DataType::Value) bracket;
  EXPECT_EQ(DataType::kBracket, bracket_val);

  DataType punctuation(DataType::kPunctuation);
  auto punctuation_val = (DataType::Value) punctuation;
  EXPECT_EQ(DataType::kPunctuation, punctuation_val);
}

TEST(DataTypeOperatorsTests, OutputTest) {
  std::ostringstream oss_none;
  oss_none << DataType(DataType::kNone);
  EXPECT_EQ(DT_NONE, oss_none.str());

  std::ostringstream oss_punctuation;
  oss_punctuation << DataType(DataType::kPunctuation);
  EXPECT_EQ(DT_PUNCTUATION, oss_punctuation.str());

  std::ostringstream oss_operator;
  oss_operator << DataType(DataType::kOperator);
  EXPECT_EQ(DT_OPERATOR, oss_operator.str());

  std::ostringstream oss_string;
  oss_string << DataType(DataType::kString);
  EXPECT_EQ(DT_STRING, oss_string.str());
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
