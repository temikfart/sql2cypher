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
  EXPECT_EQ(DataType::UNKNOWN, (DataType::Value) type);
}

TEST(DataTypeCtorTests, ValueTest) {
  EXPECT_EQ(DataType::UNKNOWN, (DataType::Value) DataType(DataType::UNKNOWN));
  EXPECT_EQ(DataType::ROOT, (DataType::Value) DataType(DataType::ROOT));
  EXPECT_EQ(DataType::INT_NUMBER, (DataType::Value) DataType(DataType::INT_NUMBER));
  EXPECT_EQ(DataType::BRACKET, (DataType::Value) DataType(DataType::BRACKET));
  EXPECT_EQ(DataType::STRING, (DataType::Value) DataType(DataType::STRING));
}

TEST(DataTypeCtorTests, InvalidValueTest) {
  EXPECT_THROW(DataType(DataType::Value(-1)), std::invalid_argument);
  EXPECT_THROW(DataType(DataType::Value(100)), std::invalid_argument);
}

TEST(DataTypeCastTests, CastToValueTest) {
  DataType bracket(DataType::BRACKET);
  auto bracket_val = (DataType::Value) bracket;
  EXPECT_EQ(DataType::BRACKET, bracket_val);

  DataType punctuation(DataType::PUNCTUATION);
  auto punctuation_val = (DataType::Value) punctuation;
  EXPECT_EQ(DataType::PUNCTUATION, punctuation_val);
}

TEST(DataTypeOperatorsTests, CompareTwoEqualTypesTest) {
  DataType first(DataType::INT_NUMBER);
  DataType second(DataType::INT_NUMBER);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(DataTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  DataType first(DataType::INT_NUMBER);
  DataType second(DataType::BRACKET);

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
  DataType type(DataType::SERVICE);
  DataType::Value value = DataType::SERVICE;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(DataTypeOperatorsTests, CompareWithDifferentValueTest) {
  DataType type(DataType::ROOT);
  DataType::Value value = DataType::PUNCTUATION;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
