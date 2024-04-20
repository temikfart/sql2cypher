#pragma once

#include "SCC/ast/nodes/inode.h"
#include "SCC/parser/common/utils.h"

namespace scc::parser::common {

class NodeDataClassifier {
public:
  static bool IsDot(std::shared_ptr<ast::INode>& node);
  static bool IsComma(std::shared_ptr<ast::INode>& node);
  static bool IsOpeningRoundBracket(std::shared_ptr<ast::INode>& node);
  static bool IsClosingRoundBracket(std::shared_ptr<ast::INode>& node);
  static bool IsSingleQuote(std::shared_ptr<ast::INode>& node);
  static bool IsDoubleQuote(std::shared_ptr<ast::INode>& node);
  static bool IsQuote(std::shared_ptr<ast::INode>& node);
  static bool IsUnaryOperator(std::shared_ptr<ast::INode>& node);
  static bool IsBinaryOperator(std::shared_ptr<ast::INode>& node);
  static bool IsSemicolon(std::shared_ptr<ast::INode>& node);
};

} // scc::parser::common
