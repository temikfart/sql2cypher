#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "SCC/ast/stmt_types.h"

namespace scc::parser::common {

class BaseStmtTypeClassifier {
public:
  static bool IsDDLKeyword(const std::string& keyword);
  static bool IsDMLKeyword(const std::string& keyword);

private:
  static bool AnyOfKeywords(const std::vector<std::string>& keywords, const std::string& keyword);
};

} // scc::parser::common
