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

TEST(SchemaRelationshipSerializationTests, ToJsonTest) {
  Relationship empty_relationship;
  ObjectToJsonTestBody(empty_relationship, create_json_string(empty_relationship));

  Relationship relationship("LOVES", "Maxim", "Katya");
  ObjectToJsonTestBody(relationship, create_json_string(relationship));

  relationship.AddCondition(ApplyCondition(1, 2));
  relationship.AddCondition(ApplyCondition(5, 7));
  ObjectToJsonTestBody(relationship, create_json_string(relationship));
}
TEST(SchemaRelationshipSerializationTests, FromJsonTest)  {
  Relationship empty_relationship;
  ObjectFromJsonTestBody(create_json(empty_relationship), empty_relationship);

  Relationship relationship("DIRECTED", "Nolan", "Batman");
  ObjectFromJsonTestBody(create_json(relationship), relationship);

  relationship.AddCondition(ApplyCondition(9, 0));
  ObjectFromJsonTestBody(create_json(relationship), relationship);
}
