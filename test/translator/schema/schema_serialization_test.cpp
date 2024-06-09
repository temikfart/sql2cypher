#include "gtest/gtest.h"

#include "nlohmann/json.hpp"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/property_types.h"
#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"
#include "SCC/translator/schema/relationship.h"
#include "SCC/translator/schema/schema.h"

using namespace scc::translator::schema;
using namespace testing;
using nlohmann::json;
using scc::translator::cypher::ConstraintType;
using scc::translator::cypher::PropertyType;

Node create_section_node() {
  Node section("Section");

  PropertyConstraint section_id_unique("unique_section", ConstraintType::kUniqueness);
  PropertyConstraint section_id_not_null("not_null_section", ConstraintType::kExistence);
  Property section_id("section_id", PropertyType::kInteger,
                      {section_id_unique, section_id_not_null});
  section.AddProperty(section_id);

  Property section_name("name", PropertyType::kString);
  section.AddProperty(section_name);

  return section;
}
Node create_report_node() {
  Node report("Report");

  PropertyConstraint report_id_unique("unique_report", ConstraintType::kUniqueness);
  PropertyConstraint report_id_not_null("not_null_report", ConstraintType::kExistence);
  Property report_id("report_id", PropertyType::kInteger,
                          {report_id_unique, report_id_not_null});
  report.AddProperty(report_id);

  Property report_title("title", PropertyType::kString);
  report.AddProperty(report_title);

  Property report_section_id("section_id", PropertyType::kInteger);
  report.AddProperty(report_section_id);

  return report;
}
Node create_participant_node() {
  Node participant("Participant");

  PropertyConstraint participant_id_unique("unique_participant", ConstraintType::kUniqueness);
  PropertyConstraint participant_id_not_null("not_null_participant", ConstraintType::kExistence);
  Property participant_id("participant_id", PropertyType::kInteger,
                          {participant_id_unique, participant_id_not_null});
  participant.AddProperty(participant_id);

  PropertyConstraint participant_name_unique("unique_name", ConstraintType::kUniqueness);
  Property participant_name("name", PropertyType::kString);
  participant.AddProperty(participant_name);

  Property participant_age("age", PropertyType::kInteger);
  participant.AddProperty(participant_age);

  Property participant_report_id("report_id", PropertyType::kInteger);
  participant.AddProperty(participant_report_id);

  return participant;
}
Relationship create_report_to_section_relationship() {
  Relationship relationship("REPORTED_ON", "Report", "Section");
  relationship.AddCondition(ApplyCondition(2, 0));
  return relationship;
}
Relationship create_participant_to_report_relationship() {
  Relationship relationship("REPORTS",  "Participant",  "Report");
  relationship.AddCondition(ApplyCondition(3, 0));
  return relationship;
}
Schema create_test_schema() {
  Schema schema("Conference");

  schema.AddNode(create_section_node());
  schema.AddNode(create_report_node());
  schema.AddNode(create_participant_node());

  schema.AddRelationship(create_report_to_section_relationship());
  schema.AddRelationship(create_participant_to_report_relationship());

  return schema;
}

TEST(SchemaSerializationTests, ToJsonTest) {
  Schema empty_schema;
  ObjectToJsonTestBody<Schema>(empty_schema, create_json_string(empty_schema));

  Schema schema = create_test_schema();
  ObjectToJsonTestBody<Schema>(schema, create_json_string(schema));
}
TEST(SchemaSerializationTests, FromJsonTest)  {
  Schema empty_schema;
  ObjectFromJsonTestBody<Schema>(create_json(empty_schema), empty_schema);

  Schema schema = create_test_schema();
  ObjectFromJsonTestBody<Schema>(create_json(schema), schema);
}
