#pragma once

#include <format>
#include <string>

#include "nlohmann/json.hpp"

constexpr int indent = 2;
constexpr char indent_char = ' ';

inline std::string dump(const nlohmann::json& j) {
  return j.dump(indent, indent_char);
}

template<typename T>
inline std::string create_enum_json_string(const T& type) {
  return std::format(R"({{
  "value_": "{}"
}})", type.ToString());
}
template<typename T>
inline nlohmann::json create_enum_json(const T& type) {
  return nlohmann::json::parse(create_enum_json_string(type));
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
