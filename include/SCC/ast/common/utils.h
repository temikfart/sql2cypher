#pragma once

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"
#include "SCC/ast/stmt_types.h"

namespace scc::ast::common {

class ASTUtils {
public:
  template<typename NodeType,
      typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
  static std::shared_ptr<INode> CastToINode(std::shared_ptr<NodeType> node) {
    return std::static_pointer_cast<INode>(node);
  }
  template<typename NodeType,
      typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
  static std::shared_ptr<NodeType> CastToNodeType(std::shared_ptr<INode> node) {
    return std::static_pointer_cast<NodeType>(node);
  }
  static std::shared_ptr<INode> CreateRootNode(StmtType stmt_type);
  static std::shared_ptr<INode> CreateServiceNode(StmtType stmt_type);
  static std::shared_ptr<INode> CreateServiceNode(StmtType stmt_type,
                                                  const std::shared_ptr<INode>& base);
  static std::shared_ptr<INode> CreateCharNode(char ch, DataType data_type);
  static std::shared_ptr<INode> CreateStringNode(const std::string& str, DataType data_type);

  static void Link(std::shared_ptr<INode>& parent, std::shared_ptr<INode>& child);
};

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
bool operator==(const std::shared_ptr<NodeType>& lhs, const std::shared_ptr<NodeType>& rhs) {
  LOGT << "Comparing nodes";

  if (lhs->line != rhs->line) {
    LOGT << "Line numbers are different";
    return false;
  }

  if (lhs->data_type != rhs->data_type) {
    LOGT << "DataTypes mismatch";
    return false;
  }

  if (lhs->stmt_type != rhs->stmt_type) {
    LOGT << "Different statement types";
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

  LOGT << "Nodes are equal";
  return true;
}
template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
bool operator!=(const std::shared_ptr<NodeType>& lhs, const std::shared_ptr<NodeType>& rhs) {
  return !(lhs == rhs);
}

} // scc::ast::common
