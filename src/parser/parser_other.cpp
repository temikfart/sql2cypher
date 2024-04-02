#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

// Validation

void Parser::ValidateNotEmpty() const {
  if (tokens_.empty()) {
    LOGE << "expected that tokens' array is not empty";
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsWord(NodePtr<INode>& node) const {
  if (node->data_type != DataType::kWord) {
    LOGE << "expected word in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsOpeningRoundBracket(
    NodePtr<INode>& node) const {
  if (!IsOpeningRoundBracket(node)) {
    LOGE << "expected an opening round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsClosingRoundBracket(
    NodePtr<INode>& node) const {
  if (!IsClosingRoundBracket(node)) {
    LOGE << "expected a closing round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsSingleQuote(NodePtr<INode>& node) const {
  if (!IsSingleQuote(node)) {
    LOGE << "expected a single quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void Parser::ValidateIsDoubleQuote(NodePtr<INode>& node) const {
  if (!IsSingleQuote(node)) {
    LOGE << "expected a double quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}

// Defining Node Datatype

bool Parser::IsBracket(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kBracket);
}
bool Parser::IsPunctuation(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kPunctuation);
}
bool Parser::IsWord(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kWord);
}
bool Parser::IsNumber(NodePtr<INode>& node) const {
  bool is_int = node->data_type != DataType::kInt;
  bool is_float = node->data_type != DataType::kFloat;
  return (is_int || is_float);
}
bool Parser::IsOperator(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kOperator);
}

// Defining Node data

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

// Work with deque of tokens

NodePtr<INode>& Parser::peek_first_token() const {
  ValidateNotEmpty();
  return const_cast<NodePtr<INode>&>(tokens_.front());
}
NodePtr<INode> Parser::get_first_token() {
  ValidateNotEmpty();
  NodePtr<INode> node = tokens_.front();
  tokens_.pop_front();
  return node;
}
void Parser::pop_first_token() {
  ValidateNotEmpty();
  tokens_.pop_front();
}

} // scc::parser
