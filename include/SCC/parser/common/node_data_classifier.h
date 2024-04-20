#pragma once

#include "SCC/ast/data_types.h"
#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/common/utils.h"

namespace scc::parser::common {

class NodeDataClassifier {
public:
  static bool IsDot(const std::shared_ptr<ast::INode>& node);
  static bool IsComma(const std::shared_ptr<ast::INode>& node);
  static bool IsOpeningRoundBracket(const std::shared_ptr<ast::INode>& node);
  static bool IsClosingRoundBracket(const std::shared_ptr<ast::INode>& node);
  static bool IsSingleQuote(const std::shared_ptr<ast::INode>& node);
  static bool IsDoubleQuote(const std::shared_ptr<ast::INode>& node);
  static bool IsQuote(const std::shared_ptr<ast::INode>& node);
  static bool IsUnaryOperator(const std::shared_ptr<ast::INode>& node);
  static bool IsBinaryOperator(const std::shared_ptr<ast::INode>& node);
  static bool IsSemicolon(const std::shared_ptr<ast::INode>& node);

private:
  static bool SameDataType(const std::shared_ptr<ast::INode>& node, ast::DataType data_type);
};

} // scc::parser::common
