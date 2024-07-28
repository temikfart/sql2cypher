#include <compare>

#include "gtest/gtest.h"

#include "SCC/fixtures_common/enum.h"
#include "SCC/translator/cypher/graph/constraint_types.h"

using namespace scc::translator::cypher;
using namespace testing;

using UChar = unsigned char;

TEST(ConstraintTypeValueEnumTests, IsUnsignedCharTest) {
  CastEnumTestBody<UChar, ConstraintType::Value>(0);
  CastEnumTestBody<UChar, ConstraintType::Value>(-1);
  CastEnumTestBody<UChar, ConstraintType::Value>(50);
  CastEnumTestBody<UChar, ConstraintType::Value>(128);
  CastEnumTestBody<UChar, ConstraintType::Value>(256);
}

TEST(ConstraintTypeCtorTests, DefaultValueTest) {
  ConstraintType type;
  EXPECT_EQ(ConstraintType::kNone, (ConstraintType::Value) type);
}

TEST(ConstraintTypeCtorTests, ValueTest) {
  CastWrapperToValueTestBody<ConstraintType, ConstraintType::Value>(ConstraintType::kNone);
  CastWrapperToValueTestBody<ConstraintType, ConstraintType::Value>(ConstraintType::kUniqueness);
  CastWrapperToValueTestBody<ConstraintType, ConstraintType::Value>(ConstraintType::kExistence);
}

TEST(ConstraintTypeCtorTests, InvalidValueTest) {
  CreateWithInvalidArgumentTestBody<ConstraintType, ConstraintType::Value>(-1);
  CreateWithInvalidArgumentTestBody<ConstraintType, ConstraintType::Value>(3);
}

TEST(ConstraintTypeCtorTests, InvalidStringValueTest) {
  CreateWithInvalidArgumentTestBody<ConstraintType, ConstraintType::Value>("invalid");
  CreateWithInvalidArgumentTestBody<ConstraintType, ConstraintType::Value>("n0ne");
  CreateWithInvalidArgumentTestBody<ConstraintType, ConstraintType::Value>("unqie");
}

TEST(ConstraintTypeCtorTests, StringValueTest) {
  CreateFromStringTestBody<ConstraintType>(kCT_None, ConstraintType::kNone);
  CreateFromStringTestBody<ConstraintType>(kCT_Uniqueness, ConstraintType::kUniqueness);
  CreateFromStringTestBody<ConstraintType>(kCT_Existence, ConstraintType::kExistence);
}

TEST(ConstraintTypeCtorTests, MixedCaseStringTest) {
  CreateFromStringTestBody<ConstraintType>("NONe", ConstraintType::kNone);
  CreateFromStringTestBody<ConstraintType>("UnIqUe", ConstraintType::kUniqueness);
  CreateFromStringTestBody<ConstraintType>("not NULL", ConstraintType::kExistence);
}

TEST(ConstraintTypeoStringTests, ToStringTest) {
  ToStringTestBody<ConstraintType>(ConstraintType::kNone, kCT_None);
  ToStringTestBody<ConstraintType>(ConstraintType::kUniqueness, kCT_Uniqueness);
  ToStringTestBody<ConstraintType>(ConstraintType::kExistence, kCT_Existence);
}

TEST(ConstraintTypeOperatorsTests, OutputTest) {
  OutputTestBody<ConstraintType, ConstraintType::Value>(ConstraintType::kNone, kCT_None);
  OutputTestBody<ConstraintType, ConstraintType::Value>(ConstraintType::kUniqueness, kCT_Uniqueness);
  OutputTestBody<ConstraintType, ConstraintType::Value>(ConstraintType::kExistence, kCT_Existence);
}

TEST(ConstraintTypeOperatorsTests, CompareTwoEqualTypesTest) {
  ConstraintType first(ConstraintType::kUniqueness);
  ConstraintType second(ConstraintType::kUniqueness);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(ConstraintTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  ConstraintType first(ConstraintType::kUniqueness);
  ConstraintType second(ConstraintType::kExistence);

  EXPECT_TRUE(first != second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first < second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first >= second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::less, first <=> second);
  EXPECT_EQ(std::partial_ordering::greater, second <=> first);
}

TEST(ConstraintTypeOperatorsTests, CompareWithEqualValueTest) {
  ConstraintType type(ConstraintType::kExistence);
  ConstraintType::Value value = ConstraintType::kExistence;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(ConstraintTypeOperatorsTests, CompareWithDifferentValueTest) {
  ConstraintType type(ConstraintType::kUniqueness);
  ConstraintType::Value value = ConstraintType::kExistence;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
