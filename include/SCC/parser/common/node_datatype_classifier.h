#pragma once

#include "SCC/ast/nodes/inode.h"

namespace scc::parser::common {

class NodeDataTypeClassifier {
public:
  static bool IsNumber(const std::shared_ptr<ast::INode>& node);
  static bool IsBracket(const std::shared_ptr<ast::INode>& node);
  static bool IsPunctuation(const std::shared_ptr<ast::INode>& node);
  static bool IsOperator(const std::shared_ptr<ast::INode>& node);
  static bool IsWord(const std::shared_ptr<ast::INode>& node);
};

} // scc::parser::common
