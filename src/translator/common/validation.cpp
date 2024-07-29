#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
using namespace scc::common;

using std::format;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

translation_error::translation_error(const std::string& message) : std::logic_error(message) {}

bool Translator::HasChildren(const std::shared_ptr<ast::INode>& node,
                             unsigned min_children_count) {
  return node->ChildrenCount() >= min_children_count;
}
bool Translator::IsCorrectStmtType(const std::shared_ptr<ast::INode>& node,
                                   ast::StmtType stmt_type) {
  return node->stmt_type == stmt_type;
}

void Translator::ValidateHasChildren(const NodePtr<INode>& node, unsigned min_children_count,
                                     const std::string& details) {
  if (!HasChildren(node, min_children_count)) {
    std::string msg = "Unexpected absence of child nodes";
    std::string cause = (details.empty() ? "" : (": " + details));
    throw translation_error(msg + cause);
  }
}
void Translator::ValidateIsCorrectStmtType(const NodePtr<INode>& node, StmtType stmt_type) {
  if (!IsCorrectStmtType(node, stmt_type)) {
    throw translation_error(format("Unexpected statement type \'{}\': expected \'{}\'",
                                   UpperCase(node->stmt_type.ToString()),
                                   UpperCase(stmt_type.ToString())));
  }
}

} // scc::translator
