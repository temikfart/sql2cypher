#include "SCC/common/string_utils.h"

std::string LowerCase(const std::string& str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) {
                   return std::tolower(c);
                 });
  return result;
}
