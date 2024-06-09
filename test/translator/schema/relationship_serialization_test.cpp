#include "gtest/gtest.h"

#include "nlohmann/json.hpp"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/schema/relationship.h"

using namespace scc::translator::schema;
using namespace testing;
using nlohmann::json;

TEST(SchemaApplyConditionSerializationTests, ToJsonTest) {
  ApplyCondition empty_condition;
  ObjectToJsonTestBody<ApplyCondition>(empty_condition, create_json_string(empty_condition));

  ApplyCondition condition(5, 8);
  ObjectToJsonTestBody<ApplyCondition>(condition, create_json_string(condition));
}
TEST(SchemaApplyConditionSerializationTests, FromJsonTest)  {
  ApplyCondition empty_condition;
  ObjectFromJsonTestBody<ApplyCondition>(create_json(empty_condition), empty_condition);

  ApplyCondition condition(4, 2);
  ObjectFromJsonTestBody<ApplyCondition>(create_json(condition), condition);
}

TEST(SchemaRelationshipSerializationTests, ToJsonTest) {
  Relationship empty_relationship;
  ObjectToJsonTestBody<Relationship>(empty_relationship, create_json_string(empty_relationship));

  Relationship relationship("LOVES", "Maxim", "Katya");
  ObjectToJsonTestBody<Relationship>(relationship, create_json_string(relationship));

  relationship.AddCondition(ApplyCondition(1, 2));
  relationship.AddCondition(ApplyCondition(5, 7));
  ObjectToJsonTestBody<Relationship>(relationship, create_json_string(relationship));
}
TEST(SchemaRelationshipSerializationTests, FromJsonTest)  {
  Relationship empty_relationship;
  ObjectFromJsonTestBody<Relationship>(create_json(empty_relationship), empty_relationship);

  Relationship relationship("DIRECTED", "Nolan", "Batman");
  ObjectFromJsonTestBody<Relationship>(create_json(relationship), relationship);

  relationship.AddCondition(ApplyCondition(9, 0));
  ObjectFromJsonTestBody<Relationship>(create_json(relationship), relationship);
}
