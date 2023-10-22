#include "SCC/common/string_utils.h"

namespace scc::common {

std::string LowerCase(const std::string& str) {
  std::string res = str;
  std::transform(res.begin(), res.end(), res.begin(), tolower);
  return res;
}

} // scc::common
