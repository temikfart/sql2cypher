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
bool ASTUtils::Equal(const std::shared_ptr<INode>& lhs, const std::shared_ptr<INode>& rhs) {
  LOGT << "Comparing nodes";

  if (lhs->data_type != rhs->data_type) {
    LOGT << "DataTypes mismatch";
    return false;
  }

  if (lhs->ChildrenCount() != rhs->ChildrenCount()) {
    LOGT << "Children counts mismatch";
    return false;
  }

  switch (lhs->data_type) {
    case DataType::kNone:
    case DataType::kRoot:
    case DataType::kService:
      break;
    case DataType::kInt:
      if (ASTUtils::CastToNodeType<IntNumNode>(lhs)->data !=
          ASTUtils::CastToNodeType<IntNumNode>(rhs)->data) {
        LOGT << "IntNumNode data mismatch";
        return false;
      }
      break;
    case DataType::kFloat:
      if (ASTUtils::CastToNodeType<FloatNumNode>(lhs)->data !=
          ASTUtils::CastToNodeType<FloatNumNode>(rhs)->data) {
        LOGT << "FloatNumNode data mismatch";
        return false;
      }
      break;
    case DataType::kBracket:
    case DataType::kPunctuation:
      if (ASTUtils::CastToNodeType<CharNode>(lhs)->data !=
          ASTUtils::CastToNodeType<CharNode>(rhs)->data) {
        LOGT << "CharNode data mismatch";
        return false;
      }
      break;
    case DataType::kOperator:
    case DataType::kWord:
    case DataType::kString:
      if (ASTUtils::CastToNodeType<StringNode>(lhs)->data !=
          ASTUtils::CastToNodeType<StringNode>(rhs)->data) {
        LOGT << "StringNode data mismatch";
        return false;
      }
      break;
  }

  for (std::size_t i = 0; i < lhs->ChildrenCount(); i++) {
    if (!Equal(lhs->get_child(i), rhs->get_child(i))) {
      LOGT << "not equal: different subtrees";
      return false;
    }
  }

  LOGT << "Nodes are equal";
  return true;
}

} // scc::ast::common
