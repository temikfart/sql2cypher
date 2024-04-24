#include "SCC/parser/common/node_data_classifier.h"

namespace scc::parser::common {

using namespace ast;
using namespace ast::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool NodeDataClassifier::IsDot(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kPunctuation)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == '.';
}
bool NodeDataClassifier::IsComma(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kPunctuation)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == ',';
}
bool NodeDataClassifier::IsOpeningRoundBracket(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kBracket)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == '(';
}
bool NodeDataClassifier::IsClosingRoundBracket(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kBracket)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == ')';
}
bool NodeDataClassifier::IsSingleQuote(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kPunctuation)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == '\'';
}
bool NodeDataClassifier::IsDoubleQuote(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kPunctuation)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == '\"';
}
bool NodeDataClassifier::IsQuote(const NodePtr<INode>& node) {
  return IsSingleQuote(node) || IsDoubleQuote(node);
}
bool NodeDataClassifier::IsUnaryOperator(const NodePtr<INode>& node) {
  if (IsCorrectDataType(node, DataType::kOperator)) {
    std::string data = ASTUtils::CastToNodeType<StringNode>(node)->data;
    return data == "+" || data == "-";
  }
  return false;
}
bool NodeDataClassifier::IsBinaryOperator(const NodePtr<INode>& node) {
  if (IsCorrectDataType(node, DataType::kOperator)) {
    std::string data = ASTUtils::CastToNodeType<StringNode>(node)->data;
    static const std::vector<std::string> bin_operators = {
        "!=", "=", "!>", ">", ">=", "!<", "<", "<=", "<>"
    };
    return std::find(bin_operators.begin(), bin_operators.end(), data) != bin_operators.end();
  }
  return false;
}
bool NodeDataClassifier::IsSemicolon(const NodePtr<INode>& node) {
  return IsCorrectDataType(node, DataType::kPunctuation)
      && ASTUtils::CastToNodeType<CharNode>(node)->data == ';';
}

bool NodeDataClassifier::IsCorrectDataType(const NodePtr<INode>& node_to_check,
                                           DataType correct_data_type) {
  return node_to_check->data_type == correct_data_type;
}

} // scc::parser::common
