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

inline std::string create_json_string(const Property& property) {
  std::string constraints_json_string;
  for (unsigned i = 0; i < property.Constraints().size(); ++i) {
    bool first = i == 0;
    bool last = i == property.Constraints().size() - 1;
    if (first) {
      constraints_json_string += "\n";
    }
    const auto& constraint = property.Constraints()[i];
    std::string constraint_json_string = format(R"(    {{
      "name": "{}",
      "type": "{}"
    }}{})", constraint.name, constraint.type.ToString(), (last  ? "" : ",\n"));
    constraints_json_string += constraint_json_string;
    if (last) {
      constraints_json_string += "\n  ";
    }
  }

  return format(R"({{
  "constraints": [{}],
  "name": "{}",
  "type": "{}"
}})", constraints_json_string, property.name, property.type.ToString());
}
inline json create_json(const Property& property) {
  return json::parse(create_json_string(property));
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

TEST(SchemaPropertySerializationTests, ToJsonTest) {
  Property id_property("id", PropertyType::kInteger);
  ObjectToJsonTestBody<Property>(id_property, create_json_string(id_property));

  id_property.AddConstraint(PropertyConstraint("pk_constraint_0", ConstraintType::kUniqueness));
  id_property.AddConstraint(PropertyConstraint("pk_constraint_1", ConstraintType::kExistence));
  ObjectToJsonTestBody<Property>(id_property, create_json_string(id_property));
}
TEST(SchemaPropertySerializationTests, FromJsonTest)  {
  Property name_property("name", PropertyType::kString);
  ObjectFromJsonTestBody<Property>(create_json(name_property), name_property);

  name_property.AddConstraint(PropertyConstraint("constraint_name", ConstraintType::kExistence));
  ObjectFromJsonTestBody<Property>(create_json(name_property), name_property);
}
