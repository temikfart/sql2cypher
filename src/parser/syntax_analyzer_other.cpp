#include "SCC/parser/syntax_analyzer.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

// Validation

void SyntaxAnalyzer::ValidateNotEmpty() const {
  if (tokens_.empty()) {
    LOGE << "expected that tokens' array is not empty";
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsWord(NodePtr<INode>& node) const {
  if (node->data_type != DataType::kWord) {
    LOGE << "expected word in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsOpeningRoundBracket(
    NodePtr<INode>& node) const {
  if (!IsOpeningRoundBracket(node)) {
    LOGE << "expected an opening round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    NodePtr<INode>& node) const {
  if (!IsClosingRoundBracket(node)) {
    LOGE << "expected a closing round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsSingleQuote(NodePtr<INode>& node) const {
  if (!IsSingleQuote(node)) {
    LOGE << "expected a single quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(NodePtr<INode>& node) const {
  if (!IsSingleQuote(node)) {
    LOGE << "expected a double quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}

// Defining Node Datatype

bool SyntaxAnalyzer::IsBracket(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kBracket);
}
bool SyntaxAnalyzer::IsPunctuation(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kPunctuation);
}
bool SyntaxAnalyzer::IsWord(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kWord);
}
bool SyntaxAnalyzer::IsNumber(NodePtr<INode>& node) const {
  bool is_int = node->data_type != DataType::kInt;
  bool is_float = node->data_type != DataType::kFloat;
  return (is_int || is_float);
}
bool SyntaxAnalyzer::IsOperator(NodePtr<INode>& node) const {
  return (node->data_type == DataType::kOperator);
}

// Defining Node data

bool SyntaxAnalyzer::IsDot(NodePtr<INode>& node) const {
  NodePtr<INode> dot = CreateCharNode('.', DataType::kPunctuation);
  return INode::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(NodePtr<INode>& node) const {
  NodePtr<INode> comma = CreateCharNode(',', DataType::kPunctuation);
  return INode::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(NodePtr<INode>& node) const {
  // Opening Round Bracket
  NodePtr<INode> ORB = CreateCharNode('(', DataType::kBracket);
  return INode::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(NodePtr<INode>& node) const {
  // Closing Round Bracket
  NodePtr<INode> CRB = CreateCharNode(')', DataType::kBracket);
  return INode::IsNodesEqual(CRB, node);
}
bool SyntaxAnalyzer::IsSingleQuote(NodePtr<INode>& node) const {
  NodePtr<INode> single_quote = CreateStringNode("\'", DataType::kPunctuation);
  return INode::IsNodesEqual(single_quote, node);
}
bool SyntaxAnalyzer::IsDoubleQuote(NodePtr<INode>& node) const {
  NodePtr<INode> double_quote = CreateStringNode("\"", DataType::kPunctuation);
  return INode::IsNodesEqual(double_quote, node);
}
bool SyntaxAnalyzer::IsUnaryOperator(NodePtr<INode>& node) const {
  if (node->data_type == DataType::kOperator) {
    std::string data = CastToNodeType<StringNode>(node)->data;
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsBinaryOperator(NodePtr<INode>& node) const {
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
bool SyntaxAnalyzer::IsSemicolon(NodePtr<INode>& node) const {
  NodePtr<INode> semicolon = CreateCharNode(';', DataType::kPunctuation);
  return INode::IsNodesEqual(semicolon, node);
}

// Work with deque of tokens

NodePtr<INode>& SyntaxAnalyzer::peek_first_token() const {
  ValidateNotEmpty();
  return const_cast<NodePtr<INode>&>(tokens_.front());
}
NodePtr<INode> SyntaxAnalyzer::get_first_token() {
  ValidateNotEmpty();
  NodePtr<INode> node = tokens_.front();
  tokens_.pop_front();
  return node;
}
void SyntaxAnalyzer::pop_first_token() {
  ValidateNotEmpty();
  tokens_.pop_front();
}

} // scc::parser
