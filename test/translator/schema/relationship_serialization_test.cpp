#include "gtest/gtest.h"

#include "nlohmann/json.hpp"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/schema/relationship.h"

using namespace scc::translator::schema;
using namespace testing;
using nlohmann::json;

TEST(SchemaApplyConditionSerializationTests, ToJsonTest) {
  ApplyCondition empty_condition;
  ObjectToJsonTestBody(empty_condition, create_json_string(empty_condition));

  ApplyCondition condition(5, 8);
  ObjectToJsonTestBody(condition, create_json_string(condition));
}
TEST(SchemaApplyConditionSerializationTests, FromJsonTest)  {
  ApplyCondition empty_condition;
  ObjectFromJsonTestBody(create_json(empty_condition), empty_condition);

  ApplyCondition condition(4, 2);
  ObjectFromJsonTestBody(create_json(condition), condition);
}
