#include "SCC/parser/common/base_stmttype_classifier.h"

namespace scc::parser::common {

bool BaseStmtTypeClassifier::IsDDLKeyword(const std::string& keyword) {
  return AnyOfKeywords({"CREATE", "ALTER", "DROP"}, keyword);
}
bool BaseStmtTypeClassifier::IsDMLKeyword(const std::string& keyword) {
  return AnyOfKeywords({"UPDATE", "DELETE", "INSERT"}, keyword);
}

bool BaseStmtTypeClassifier::AnyOfKeywords(const std::vector<std::string>& keywords,
                                           const std::string& keyword) {
  static auto is_dml_keyword = [keyword](const std::string& st) { return (keyword == st); };
  return std::any_of(keywords.begin(), keywords.end(), is_dml_keyword);
}

} // scc::parser::common
