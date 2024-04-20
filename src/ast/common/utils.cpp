#include "SCC/ast/common/utils.h"

namespace scc::ast::common {

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> ASTUtils::CreateRootNode(StmtType stmt_type) {
  return CastToINode(std::make_shared<RootNode>(stmt_type));
}
NodePtr<INode> ASTUtils::CreateServiceNode(StmtType stmt_type) {
  return CastToINode(std::make_shared<ServiceNode>(stmt_type));
}
NodePtr<INode> ASTUtils::CreateCharNode(char ch, DataType data_type) {
  return CastToINode(std::make_shared<CharNode>(ch, data_type));
}
NodePtr<INode> ASTUtils::CreateStringNode(const std::string& str, DataType data_type) {
  return CastToINode(std::make_shared<StringNode>(str, data_type));
}

void ASTUtils::Link(std::shared_ptr<INode>& parent, std::shared_ptr<INode>& child) {
  parent->AddChild(child);
  child->parent = parent;
}

} // scc::ast::common
