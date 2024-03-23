#include "SCC/common/string_utils.h"

namespace scc::common {

std::string LowerCase(const std::string& str) {
  std::string res = str;
  std::transform(res.begin(), res.end(), res.begin(), tolower);
  return res;
}
std::string UpperCase(const std::string& str) {
  std::string res = str;
  std::transform(res.begin(), res.end(), res.begin(), toupper);
  return res;
}

} // scc::common
