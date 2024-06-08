#include "gtest/gtest.h"

#include "nlohmann/json.hpp"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/cypher/graph/property_types.h"
#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"

using namespace scc::translator::schema;
using namespace testing;
using nlohmann::json;
using scc::translator::cypher::PropertyType;

TEST(SchemaNodeSerializationTests, ToJsonTest) {
  Node person("Person");
  ObjectToJsonTestBody<Node>(person, create_json_string(person));

  person.AddProperty(Property("firstname", PropertyType::kString));
  person.AddProperty(Property("age", PropertyType::kInteger));
  ObjectToJsonTestBody<Node>(person, create_json_string(person));
}
TEST(SchemaNodeSerializationTests, FromJsonTest) {
  Node laptop("Laptop");
  ObjectFromJsonTestBody<Node>(create_json(laptop), laptop);

  laptop.AddProperty(Property("brand", PropertyType::kString));
  laptop.AddProperty(Property("model", PropertyType::kString));
  laptop.AddProperty(Property("weight", PropertyType::kFloat));
  ObjectFromJsonTestBody<Node>(create_json(laptop), laptop);
}
