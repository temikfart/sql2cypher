#include <compare>

#include "gtest/gtest.h"

#include "SCC/fixtures_common/enum.h"
#include "SCC/translator/cypher/graph/property_types.h"

using namespace scc::translator::cypher;
using namespace testing;

using UChar = unsigned char;

TEST(PropertyTypeValueEnumTests, IsUnsignedCharTest) {
  CastEnumTestBody<UChar, PropertyType::Value>(0);
  CastEnumTestBody<UChar, PropertyType::Value>(-1);
  CastEnumTestBody<UChar, PropertyType::Value>(50);
  CastEnumTestBody<UChar, PropertyType::Value>(128);
  CastEnumTestBody<UChar, PropertyType::Value>(256);
}

TEST(PropertyTypeCtorTests, DefaultValueTest) {
  PropertyType type;
  EXPECT_EQ(PropertyType::kUnknown, (PropertyType::Value) type);
}

TEST(PropertyTypeCtorTests, ValueTest) {
  CastWrapperToValueTestBody<PropertyType, PropertyType::Value>(PropertyType::kUnknown);
  CastWrapperToValueTestBody<PropertyType, PropertyType::Value>(PropertyType::kBoolean);
  CastWrapperToValueTestBody<PropertyType, PropertyType::Value>(PropertyType::kInteger);
}

TEST(PropertyTypeCtorTests, InvalidValueTest) {
  CreateWithInvalidArgumentTestBody<PropertyType, PropertyType::Value>(-1);
  CreateWithInvalidArgumentTestBody<PropertyType, PropertyType::Value>(6);
}

TEST(PropertyTypeCtorTests, InvalidStringValueTest) {
  CreateWithInvalidArgumentTestBody<PropertyType, PropertyType::Value>("nil");
  CreateWithInvalidArgumentTestBody<PropertyType, PropertyType::Value>("int");
  CreateWithInvalidArgumentTestBody<PropertyType, PropertyType::Value>("flot");
}

TEST(PropertyTypeCtorTests, StringValueTest) {
  CreateFromStringTestBody<PropertyType>(kPT_Unknown, PropertyType::kUnknown);
  CreateFromStringTestBody<PropertyType>(kPT_Boolean, PropertyType::kBoolean);
  CreateFromStringTestBody<PropertyType>(kPT_Float, PropertyType::kFloat);
  CreateFromStringTestBody<PropertyType>(kPT_Integer, PropertyType::kInteger);
  CreateFromStringTestBody<PropertyType>(kPT_String, PropertyType::kString);
  CreateFromStringTestBody<PropertyType>(kPT_Null, PropertyType::kNull);
}

TEST(PropertyTypeCtorTests, MixedCaseStringTest) {
  CreateFromStringTestBody<PropertyType>("unknown", PropertyType::kUnknown);
  CreateFromStringTestBody<PropertyType>("bOOleAN", PropertyType::kBoolean);
  CreateFromStringTestBody<PropertyType>("FLOAT", PropertyType::kFloat);
  CreateFromStringTestBody<PropertyType>("InteGER", PropertyType::kInteger);
  CreateFromStringTestBody<PropertyType>("STRing", PropertyType::kString);
  CreateFromStringTestBody<PropertyType>("null", PropertyType::kNull);
}

TEST(PropertyTypeoStringTests, ToStringTest) {
  ToStringTestBody<PropertyType>(PropertyType::kUnknown, kPT_Unknown);
  ToStringTestBody<PropertyType>(PropertyType::kBoolean, kPT_Boolean);
  ToStringTestBody<PropertyType>(PropertyType::kFloat, kPT_Float);
  ToStringTestBody<PropertyType>(PropertyType::kInteger, kPT_Integer);
  ToStringTestBody<PropertyType>(PropertyType::kString, kPT_String);
  ToStringTestBody<PropertyType>(PropertyType::kNull, kPT_Null);
}

TEST(PropertyTypeOperatorsTests, OutputTest) {
  OutputTestBody<PropertyType, PropertyType::Value>(PropertyType::kUnknown, kPT_Unknown);
  OutputTestBody<PropertyType, PropertyType::Value>(PropertyType::kFloat, kPT_Float);
  OutputTestBody<PropertyType, PropertyType::Value>(PropertyType::kNull, kPT_Null);
}

TEST(PropertyTypeOperatorsTests, CompareTwoEqualTypesTest) {
  PropertyType first(PropertyType::kString);
  PropertyType second(PropertyType::kString);

  EXPECT_TRUE(first == second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first >= second);
  EXPECT_FALSE(first != second);
  EXPECT_FALSE(first < second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::equivalent, first <=> second);
}

TEST(PropertyTypeOperatorsTests, CompareTwoDifferentTypesTest) {
  PropertyType first(PropertyType::kFloat);
  PropertyType second(PropertyType::kNull);

  EXPECT_TRUE(first != second);
  EXPECT_TRUE(first <= second);
  EXPECT_TRUE(first < second);
  EXPECT_FALSE(first == second);
  EXPECT_FALSE(first >= second);
  EXPECT_FALSE(first > second);
  EXPECT_EQ(std::partial_ordering::less, first <=> second);
  EXPECT_EQ(std::partial_ordering::greater, second <=> first);
}

TEST(PropertyTypeOperatorsTests, CompareWithEqualValueTest) {
  PropertyType type(PropertyType::kBoolean);
  PropertyType::Value value = PropertyType::kBoolean;

  EXPECT_TRUE(type == value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type >= value);
  EXPECT_FALSE(type != value);
  EXPECT_FALSE(type < value);
  EXPECT_FALSE(type > value);
}

TEST(PropertyTypeOperatorsTests, CompareWithDifferentValueTest) {
  PropertyType type(PropertyType::kUnknown);
  PropertyType::Value value = PropertyType::kNull;

  EXPECT_TRUE(type != value);
  EXPECT_TRUE(type <= value);
  EXPECT_TRUE(type < value);
  EXPECT_FALSE(type == value);
  EXPECT_FALSE(type >= value);
  EXPECT_FALSE(type > value);
}
