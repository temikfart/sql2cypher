#pragma once

#include "SCC/ast/nodes/inode.h"

namespace scc::parser::common {

class NodeDataTypeClassifier {
public:
  static bool IsBracket(std::shared_ptr<ast::INode>& node);
  static bool IsPunctuation(std::shared_ptr<ast::INode>& node);
  static bool IsWord(std::shared_ptr<ast::INode>& node);
  static bool IsNumber(std::shared_ptr<ast::INode>& node);
  static bool IsOperator(std::shared_ptr<ast::INode>& node);
};

} // scc::parser::common
