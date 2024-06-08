#include "gtest/gtest.h"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/cypher/graph/constraint_types.h"

using namespace scc::translator::cypher;
using namespace testing;

TEST(CypherConstraintTypeSerializationTests, EnumToJsonTest) {
  EnumToJsonTestBody<ConstraintType::Value>(ConstraintType::Value::kNone, kCT_None);
  EnumToJsonTestBody<ConstraintType::Value>(ConstraintType::Value::kUniqueness, kCT_Uniqueness);
  EnumToJsonTestBody<ConstraintType::Value>(ConstraintType::Value::kExistence, kCT_Existence);
}
TEST(CypherConstraintTypeSerializationTests, EnumFromJsonTest) {
  EnumFromJsonTestBody<ConstraintType::Value>(kCT_None, ConstraintType::Value::kNone);
  EnumFromJsonTestBody<ConstraintType::Value>(kCT_Uniqueness, ConstraintType::Value::kUniqueness);
  EnumFromJsonTestBody<ConstraintType::Value>(kCT_Existence, ConstraintType::Value::kExistence);
}

TEST(CypherConstraintTypeSerializationTests, ToJsonTest) {
  EnumClassToJsonTestBody<ConstraintType>(ConstraintType::kNone);
  EnumClassToJsonTestBody<ConstraintType>(ConstraintType::kUniqueness);
  EnumClassToJsonTestBody<ConstraintType>(ConstraintType::kExistence);
}
TEST(CypherConstraintTypeSerializationTests, FromJsonTest) {
  EnumClassFromJsonTestBody<ConstraintType>(ConstraintType::kNone);
  EnumClassFromJsonTestBody<ConstraintType>(ConstraintType::kUniqueness);
  EnumClassFromJsonTestBody<ConstraintType>(ConstraintType::kExistence);
}
