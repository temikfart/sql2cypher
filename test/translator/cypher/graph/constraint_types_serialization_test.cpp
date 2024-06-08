#include <string>

#include "gtest/gtest.h"

#include "nlohmann/json.hpp"

#include "SCC/fixtures_common/serialization.h"
#include "SCC/translator/cypher/graph/constraint_types.h"

using namespace scc::translator::cypher;
using namespace testing;
using nlohmann::json;

TEST(CypherConstraintTypeSerializationTests, EnumToJsonTest) {
  ASSERT_NO_THROW(
      json none_json = ConstraintType::Value::kNone;
      EXPECT_EQ(kCT_None, none_json);
  );

  ASSERT_NO_THROW(
      json unique_json = ConstraintType::Value::kUniqueness;
      EXPECT_EQ(kCT_Uniqueness, unique_json);
  );

  ASSERT_NO_THROW(
      json not_null_json = ConstraintType::Value::kExistence;
      EXPECT_EQ(kCT_Existence, not_null_json);
  );
}
TEST(CypherConstraintTypeSerializationTests, EnumFromJsonTest) {
  ASSERT_NO_THROW(
      json none_json = kCT_None;
      EXPECT_EQ(ConstraintType::Value::kNone,
                none_json.template get<ConstraintType::Value>());
  );

  ASSERT_NO_THROW(
      json unique_json = kCT_Uniqueness;
      EXPECT_EQ(ConstraintType::Value::kUniqueness,
                unique_json.template get<ConstraintType::Value>());
  );

  ASSERT_NO_THROW(
      json not_null_json = kCT_Existence;
      EXPECT_EQ(ConstraintType::Value::kExistence,
                not_null_json.template get<ConstraintType::Value>());
  );
}

TEST(CypherConstraintTypeSerializationTests, ToJsonTest) {
  ASSERT_NO_THROW(
      ConstraintType none_type = ConstraintType::kNone;

      json none_json;
      to_json(none_json, none_type);
      EXPECT_EQ(create_enum_json_string(none_type), dump(none_json));
  );

  ASSERT_NO_THROW(
      ConstraintType unique_type = ConstraintType::kUniqueness;

      json unique_json;
      to_json(unique_json, unique_type);
      EXPECT_EQ(create_enum_json_string(unique_type), dump(unique_json));
  );

  ASSERT_NO_THROW(
      ConstraintType not_null_type = ConstraintType::kExistence;

      json not_null_json;
      to_json(not_null_json, not_null_type);
      EXPECT_EQ(create_enum_json_string(not_null_type), dump(not_null_json));
  );
}
TEST(CypherConstraintTypeSerializationTests, FromJsonTest) {
  ASSERT_NO_THROW(
      ConstraintType none_type = ConstraintType::kNone;
      ConstraintType none_type_from_json;

      json none_json = create_enum_json(none_type);
      from_json(none_json, none_type_from_json);
      EXPECT_EQ(none_type, none_type_from_json);
  );

  ASSERT_NO_THROW(
      ConstraintType unique_type = ConstraintType::kUniqueness;
      ConstraintType unique_type_from_json;

      json unique_json = create_enum_json(unique_type);
      from_json(unique_json, unique_type_from_json);
      EXPECT_EQ(unique_json, unique_type_from_json);
  );

  ASSERT_NO_THROW(
      ConstraintType not_null_type = ConstraintType::kExistence;
      ConstraintType not_null_type_from_json;

      json not_null_json = create_enum_json(not_null_type);
      from_json(not_null_json, not_null_type_from_json);
      EXPECT_EQ(not_null_json, not_null_type_from_json);
  );
}
