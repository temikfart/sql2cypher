#pragma once

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/inode.h"
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
  static std::shared_ptr<INode> CreateCharNode(char ch, DataType data_type);
  static std::shared_ptr<INode> CreateStringNode(const std::string& str, DataType data_type);
};

} // scc::ast::common
