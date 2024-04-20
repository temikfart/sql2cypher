#include "SCC/parser/common/node_data_classifier.h"

namespace scc::parser::common {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool NodeDataClassifier::IsDot(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kPunctuation)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == '.';
}
bool NodeDataClassifier::IsComma(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kPunctuation)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == ',';
}
bool NodeDataClassifier::IsOpeningRoundBracket(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kBracket)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == '(';
}
bool NodeDataClassifier::IsClosingRoundBracket(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kBracket)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == ')';
}
bool NodeDataClassifier::IsSingleQuote(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kPunctuation)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == '\'';
}
bool NodeDataClassifier::IsDoubleQuote(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kPunctuation)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == '\"';
}
bool NodeDataClassifier::IsQuote(const NodePtr<INode>& node) {
  return IsSingleQuote(node) || IsDoubleQuote(node);
}
bool NodeDataClassifier::IsUnaryOperator(const NodePtr<INode>& node) {
  if (SameDataType(node, DataType::kOperator)) {
    std::string data = ParserUtils::CastToNodeType<StringNode>(node)->data;
    return data == "+" || data == "-";
  }
  return false;
}
bool NodeDataClassifier::IsBinaryOperator(const NodePtr<INode>& node) {
  if (SameDataType(node, DataType::kOperator)) {
    std::string data = ParserUtils::CastToNodeType<StringNode>(node)->data;
    static const std::vector<std::string> bin_operators = {
        "!=", "=", "!>", ">", ">=", "!<", "<", "<=", "<>"
    };
    return std::find(bin_operators.begin(), bin_operators.end(), data) != bin_operators.end();
  }
  return false;
}
bool NodeDataClassifier::IsSemicolon(const NodePtr<INode>& node) {
  return SameDataType(node, DataType::kPunctuation)
      && ParserUtils::CastToNodeType<CharNode>(node)->data == ';';
}

bool NodeDataClassifier::SameDataType(const NodePtr<INode>& node, DataType data_type) {
  return node->data_type == data_type;
}

} // scc::parser::common
