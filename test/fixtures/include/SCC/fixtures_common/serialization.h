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
