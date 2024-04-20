#pragma once

#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"

namespace scc::parser::common {

class ParserUtils {
public:
  template<typename NodeType,
      typename std::enable_if<std::is_base_of<ast::INode, NodeType>::value>::type* = nullptr>
  static std::shared_ptr<ast::INode> CastToINode(std::shared_ptr<NodeType> node) {
    return std::static_pointer_cast<ast::INode>(node);
  }
  template<typename NodeType,
      typename std::enable_if<std::is_base_of<ast::INode, NodeType>::value>::type* = nullptr>
  static std::shared_ptr<NodeType> CastToNodeType(std::shared_ptr<ast::INode> node) {
    return std::static_pointer_cast<NodeType>(node);
  }
  static std::shared_ptr<ast::INode> CreateRootNode(ast::StmtType stmt_type);
  static std::shared_ptr<ast::INode> CreateServiceNode(ast::StmtType stmt_type);
  static std::shared_ptr<ast::INode> CreateCharNode(char ch, ast::DataType data_type);
  static std::shared_ptr<ast::INode> CreateStringNode(const std::string& str,
                                                      ast::DataType data_type);
};

} // scc::parser::common
