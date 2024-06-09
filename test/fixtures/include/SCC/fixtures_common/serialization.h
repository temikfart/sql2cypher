#pragma once

#include <format>
#include <string>

#include "nlohmann/json.hpp"

#include "SCC/translator/cypher/graph/property_types.h"
#include "SCC/translator/schema/node.h"
#include "SCC/translator/schema/property.h"
#include "SCC/translator/schema/relationship.h"

inline std::string dump(const nlohmann::json& j) {
  return j.dump();
}

template<typename T>
inline std::string create_enum_json_string(const T& type) {
  return std::format(R"({{"value_":"{}"}})", type.ToString());
}
template<typename T>
inline nlohmann::json create_enum_json(const T& type) {
  return nlohmann::json::parse(create_enum_json_string(type));
}

inline std::string create_json_string(const scc::translator::schema::PropertyConstraint& constraint) {
  return std::format(R"({{"name":"{}","type":"{}"}})", constraint.name, constraint.type.ToString());
}
inline nlohmann::json create_json(const scc::translator::schema::PropertyConstraint& constraint) {
  return nlohmann::json::parse(create_json_string(constraint));
}

inline std::string create_json_string(const scc::translator::schema::Property& property) {
  std::string constraints_json_string;
  for (unsigned i = 0; i < property.Constraints().size(); ++i) {
    bool first = i == 0;
    const auto& constraint = property.Constraints()[i];
    std::string constraint_json_string = std::format(R"({{"name":"{}","type":"{}"}})",
                                                     constraint.name, constraint.type.ToString());
    constraints_json_string += (first ? "" : ",") + constraint_json_string;
  }

  return std::format(R"({{"constraints":[{}],"name":"{}","type":"{}"}})",
                     constraints_json_string, property.name, property.type.ToString());
}
inline nlohmann::json create_json(const scc::translator::schema::Property& property) {
  return nlohmann::json::parse(create_json_string(property));
}

inline std::string create_json_string(const scc::translator::schema::Node& node) {
  std::string properties_json_string;
  for (unsigned i = 0; i < node.properties.size(); ++i) {
    bool first = i == 0;
    const auto& property = node.properties[i];
    std::string property_json_string = create_json_string(property);
    properties_json_string += (first ? "" : ",") + property_json_string;
  }

  return std::format(R"({{"label":"{}","properties":[{}]}})", node.label, properties_json_string);
}
inline nlohmann::json create_json(const scc::translator::schema::Node& node) {
  return nlohmann::json::parse(create_json_string(node));
}

inline std::string create_json_string(const scc::translator::schema::ApplyCondition& condition) {
  return std::format(R"({{"epi":{},"spi":{}}})", condition.epi, condition.spi);
}
inline nlohmann::json create_json(const scc::translator::schema::ApplyCondition& condition) {
  return nlohmann::json::parse(create_json_string(condition));
}

template<typename T>
inline void EnumToJsonTestBody(const T& type, const std::string_view& expected) {
  ASSERT_NO_THROW(
      nlohmann::json type_json = type;
      EXPECT_EQ(expected, type_json);
  );
}
template<typename T>
inline void EnumFromJsonTestBody(const std::string_view& json_str, const T& expected) {
  ASSERT_NO_THROW(
      nlohmann::json type_json = json_str;
      EXPECT_EQ(expected, type_json.template get<T>());
  );
}

template<typename T>
inline void EnumClassToJsonTestBody(const T& type) {
  ASSERT_NO_THROW(
      nlohmann::json type_json;
      to_json(type_json, type);
      EXPECT_EQ(create_enum_json_string(type), dump(type_json));
  );
}
template<typename T>
inline void EnumClassFromJsonTestBody(const T& type) {
  ASSERT_NO_THROW(
      T type_from_json;
      nlohmann::json type_json = create_enum_json(type);
      from_json(type_json, type_from_json);
      EXPECT_EQ(type, type_from_json);
  );
}

template<typename T>
inline void ObjectToJsonTestBody(const T& object, const std::string& expected) {
  ASSERT_NO_THROW(
      nlohmann::json object_json;
      to_json(object_json, object);
      EXPECT_EQ(expected, dump(object_json));
  );
}
template<typename T>
inline void ObjectFromJsonTestBody(const nlohmann::json& object_json, const T& expected_object) {
  ASSERT_NO_THROW(
      T object_from_json;
      from_json(object_json, object_from_json);
      EXPECT_EQ(expected_object, object_from_json);
  );
}
