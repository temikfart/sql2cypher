#include <format>

#include "gtest/gtest.h"

#include "nlohmann/json.hpp"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/cypher/graph/constraint_types.h"
#include "SCC/translator/cypher/graph/property_types.h"
#include "SCC/translator/schema/property.h"

using namespace scc::translator::schema;
using namespace testing;
using std::format;
using nlohmann::json;
using scc::translator::cypher::ConstraintType;
using scc::translator::cypher::PropertyType;

inline std::string create_json_string(const PropertyConstraint& constraint) {
  return format(R"({{
  "name": "{}",
  "type": "{}"
}})", constraint.name, constraint.type.ToString());
}
inline json create_json(const PropertyConstraint& constraint) {
  return json::parse(create_json_string(constraint));
}

TEST(SchemaPropertyConstraintSerializationTests, ToJsonTest) {
  PropertyConstraint empty_constraint("", ConstraintType::kNone);
  ObjectToJsonTestBody<PropertyConstraint>(empty_constraint, create_json_string(empty_constraint));

  PropertyConstraint constraint("to_json_constraint", ConstraintType::kExistence);
  ObjectToJsonTestBody<PropertyConstraint>(constraint, create_json_string(constraint));
}
TEST(SchemaPropertyConstraintSerializationTests, FromJsonTest)  {
  PropertyConstraint empty_constraint("", ConstraintType::kNone);
  ObjectFromJsonTestBody<PropertyConstraint>(create_json(empty_constraint), empty_constraint);

  PropertyConstraint constraint("from_json_constraint", ConstraintType::kUniqueness);
  ObjectFromJsonTestBody<PropertyConstraint>(create_json(constraint), constraint);
}
