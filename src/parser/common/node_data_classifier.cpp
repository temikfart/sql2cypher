#include "SCC/parser/common/node_data_classifier.h"

namespace scc::parser::common {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool NodeDataClassifier::IsDot(NodePtr<INode>& node) {
  NodePtr<INode> dot = ParserUtils::CreateCharNode('.', DataType::kPunctuation);
  return INode::IsNodesEqual(dot, node);
}
bool NodeDataClassifier::IsComma(NodePtr<INode>& node) {
  NodePtr<INode> comma = ParserUtils::CreateCharNode(',', DataType::kPunctuation);
  return INode::IsNodesEqual(comma, node);
}
bool NodeDataClassifier::IsOpeningRoundBracket(NodePtr<INode>& node) {
  NodePtr<INode> ORB = ParserUtils::CreateCharNode('(', DataType::kBracket);
  return INode::IsNodesEqual(ORB, node);
}
bool NodeDataClassifier::IsClosingRoundBracket(NodePtr<INode>& node) {
  NodePtr<INode> CRB = ParserUtils::CreateCharNode(')', DataType::kBracket);
  return INode::IsNodesEqual(CRB, node);
}
bool NodeDataClassifier::IsSingleQuote(NodePtr<INode>& node) {
  NodePtr<INode> single_quote = ParserUtils::CreateStringNode("\'", DataType::kPunctuation);
  return INode::IsNodesEqual(single_quote, node);
}
bool NodeDataClassifier::IsDoubleQuote(NodePtr<INode>& node) {
  NodePtr<INode> double_quote = ParserUtils::CreateStringNode("\"", DataType::kPunctuation);
  return INode::IsNodesEqual(double_quote, node);
}
bool NodeDataClassifier::IsQuote(NodePtr<INode>& node) {
  return IsSingleQuote(node) || IsDoubleQuote(node);
}
bool NodeDataClassifier::IsUnaryOperator(NodePtr<INode>& node) {
  if (node->data_type == DataType::kOperator) {
    std::string data = ParserUtils::CastToNodeType<StringNode>(node)->data;
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool NodeDataClassifier::IsBinaryOperator(NodePtr<INode>& node) {
  if (node->data_type == DataType::kOperator) {
    std::string data = ParserUtils::CastToNodeType<StringNode>(node)->data;
    std::vector<std::string> bin_operators = {
        "=", "<>", "!=",
        ">", ">=", "!>",
        "<", "<=", "!<"
    };
    if (std::find(bin_operators.begin(),
                  bin_operators.end(),
                  data) != bin_operators.end()) {
      return true;
    }
  }
  return false;
}
bool NodeDataClassifier::IsSemicolon(NodePtr<INode>& node) {
  NodePtr<INode> semicolon = ParserUtils::CreateCharNode(';', DataType::kPunctuation);
  return INode::IsNodesEqual(semicolon, node);
}

} // scc::parser::common
