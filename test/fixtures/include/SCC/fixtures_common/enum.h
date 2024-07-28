#pragma once

#include <type_traits>
#include <stdexcept>
#include <string>
#include <sstream>

template<typename EnumBaseType, typename EnumT, typename = std::enable_if_t<std::is_enum_v<EnumT>>>
void CastEnumTestBody(int n) {
  EXPECT_EQ(static_cast<EnumBaseType>(n), EnumT(n));
}

template<typename EnumWrapperT, typename EnumT, typename = std::enable_if_t<std::is_enum_v<EnumT>>>
void CastWrapperToValueTestBody(EnumWrapperT element) {
  EXPECT_EQ(element, static_cast<EnumT>(EnumWrapperT(element)));
}

template<typename EnumWrapperT, typename EnumT, typename = std::enable_if_t<std::is_enum_v<EnumT>>>
void CreateWithInvalidArgumentTestBody(int v) {
  EXPECT_THROW(EnumWrapperT(EnumT(v)), std::invalid_argument);
}

template<typename EnumWrapperT, typename EnumT, typename = std::enable_if_t<std::is_enum_v<EnumT>>>
void CreateWithInvalidArgumentTestBody(const std::string& v) {
  EXPECT_THROW(EnumWrapperT element(v), std::invalid_argument);
}

template<typename EnumWrapperT>
void CreateFromStringTestBody(const std::string_view& str, EnumWrapperT expected) {
  EnumWrapperT element(str);
  EXPECT_EQ(expected, element);
}

template<typename EnumWrapperT>
void ToStringTestBody(EnumWrapperT element, const std::string_view& expected) {
  EXPECT_EQ(expected, element.ToString());
}

template<typename EnumWrapperT, typename EnumT, typename = std::enable_if_t<std::is_enum_v<EnumT>>>
void OutputTestBody(EnumT element, const std::string_view& expected) {
  std::ostringstream oss;
  oss << EnumWrapperT(element);
  EXPECT_EQ(expected, oss.str());
}
