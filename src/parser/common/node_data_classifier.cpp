#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

bool Parser::IsDot(NodePtr<INode>& node) const {
  NodePtr<INode> dot = CreateCharNode('.', DataType::kPunctuation);
  return INode::IsNodesEqual(dot, node);
}
bool Parser::IsComma(NodePtr<INode>& node) const {
  NodePtr<INode> comma = CreateCharNode(',', DataType::kPunctuation);
  return INode::IsNodesEqual(comma, node);
}
bool Parser::IsOpeningRoundBracket(NodePtr<INode>& node) const {
  // Opening Round Bracket
  NodePtr<INode> ORB = CreateCharNode('(', DataType::kBracket);
  return INode::IsNodesEqual(ORB, node);
}
bool Parser::IsClosingRoundBracket(NodePtr<INode>& node) const {
  // Closing Round Bracket
  NodePtr<INode> CRB = CreateCharNode(')', DataType::kBracket);
  return INode::IsNodesEqual(CRB, node);
}
bool Parser::IsSingleQuote(NodePtr<INode>& node) const {
  NodePtr<INode> single_quote = CreateStringNode("\'", DataType::kPunctuation);
  return INode::IsNodesEqual(single_quote, node);
}
bool Parser::IsDoubleQuote(NodePtr<INode>& node) const {
  NodePtr<INode> double_quote = CreateStringNode("\"", DataType::kPunctuation);
  return INode::IsNodesEqual(double_quote, node);
}
bool Parser::IsUnaryOperator(NodePtr<INode>& node) const {
  if (node->data_type == DataType::kOperator) {
    std::string data = CastToNodeType<StringNode>(node)->data;
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool Parser::IsBinaryOperator(NodePtr<INode>& node) const {
  if (node->data_type == DataType::kOperator) {
    std::string data = CastToNodeType<StringNode>(node)->data;
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
bool Parser::IsSemicolon(NodePtr<INode>& node) const {
  NodePtr<INode> semicolon = CreateCharNode(';', DataType::kPunctuation);
  return INode::IsNodesEqual(semicolon, node);
}

} // scc::parser
