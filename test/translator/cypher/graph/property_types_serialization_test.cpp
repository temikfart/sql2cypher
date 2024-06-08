#include "gtest/gtest.h"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/cypher/graph/property_types.h"

using namespace scc::translator::cypher;
using namespace testing;

TEST(CypherPropertyTypeSerializationTests, EnumToJsonTest) {
  EnumToJsonTestBody<PropertyType::Value>(PropertyType::Value::kUnknown, kPT_Unknown);
  EnumToJsonTestBody<PropertyType::Value>(PropertyType::Value::kBoolean, kPT_Boolean);
  EnumToJsonTestBody<PropertyType::Value>(PropertyType::Value::kFloat, kPT_Float);
  EnumToJsonTestBody<PropertyType::Value>(PropertyType::Value::kInteger, kPT_Integer);
  EnumToJsonTestBody<PropertyType::Value>(PropertyType::Value::kString, kPT_String);
}
TEST(CypherPropertyTypeSerializationTests, EnumFromJsonTest) {
  EnumFromJsonTestBody<PropertyType::Value>(kPT_Unknown, PropertyType::Value::kUnknown);
  EnumFromJsonTestBody<PropertyType::Value>(kPT_Boolean, PropertyType::Value::kBoolean);
  EnumFromJsonTestBody<PropertyType::Value>(kPT_Float, PropertyType::Value::kFloat);
  EnumFromJsonTestBody<PropertyType::Value>(kPT_Integer, PropertyType::Value::kInteger);
  EnumFromJsonTestBody<PropertyType::Value>(kPT_String, PropertyType::Value::kString);
}

TEST(CypherPropertyTypeSerializationTests, ToJsonTest) {
  EnumClassToJsonTestBody<PropertyType>(PropertyType::kUnknown);
  EnumClassToJsonTestBody<PropertyType>(PropertyType::kBoolean);
  EnumClassToJsonTestBody<PropertyType>(PropertyType::kFloat);
  EnumClassToJsonTestBody<PropertyType>(PropertyType::kInteger);
  EnumClassToJsonTestBody<PropertyType>(PropertyType::kString);
}
TEST(CypherPropertyTypeSerializationTests, FromJsonTest) {
  EnumClassFromJsonTestBody<PropertyType>(PropertyType::kUnknown);
  EnumClassFromJsonTestBody<PropertyType>(PropertyType::kBoolean);
  EnumClassFromJsonTestBody<PropertyType>(PropertyType::kFloat);
  EnumClassFromJsonTestBody<PropertyType>(PropertyType::kInteger);
  EnumClassFromJsonTestBody<PropertyType>(PropertyType::kString);
}
