#pragma once

#include <format>
#include <string>

#include "nlohmann/json.hpp"

using std::format;
using nlohmann::json;

constexpr int indent = 2;
constexpr char indent_char = ' ';

inline std::string dump(const json& j) {
  return j.dump(indent, indent_char);
}
template<typename T>
inline std::string create_enum_json_string(T type) {
  return format(R"({{
  "value_": "{}"
}})", type.ToString());
}
template<typename T>
inline json create_enum_json(T type) {
  return json::parse(create_enum_json_string(type));
}

template<typename T>
inline void EnumToJsonTestBody(const T& type, const std::string_view& expected) {
  ASSERT_NO_THROW(
      json type_json = type;
      EXPECT_EQ(expected, type_json);
  );
}
template<typename T>
inline void EnumFromJsonTestBody(const std::string_view& json_str, const T& expected) {
  ASSERT_NO_THROW(
      json type_json = json_str;
      EXPECT_EQ(expected, type_json.template get<T>());
  );
}

template<typename T>
inline void EnumClassToJsonTestBody(const T& type) {
  ASSERT_NO_THROW(
      json type_json;
      to_json(type_json, type);
      EXPECT_EQ(create_enum_json_string(type), dump(type_json));
  );
}
template<typename T>
inline void EnumClassFromJsonTestBody(const T& type) {
  ASSERT_NO_THROW(
      T type_from_json;
      json type_json = create_enum_json(type);
      from_json(type_json, type_from_json);
      EXPECT_EQ(type, type_from_json);
  );
}
