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
  if (!SyntaxAnalyzer::IsOpeningRoundBracket(node)) {
    LOGE << "expected an opening round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    NodePtr<INode>& node) const {
  if (!SyntaxAnalyzer::IsClosingRoundBracket(node)) {
    LOGE << "expected a closing round bracket in line "
         << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsSingleQuote(NodePtr<INode>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOGE << "expected a single quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(NodePtr<INode>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOGE << "expected a double quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}

// Defining Node Datatype

bool SyntaxAnalyzer::IsBracket(NodePtr<INode>& node) {
  return (node->data_type == DataType::kBracket);
}
bool SyntaxAnalyzer::IsPunctuation(NodePtr<INode>& node) {
  return (node->data_type == DataType::kPunctuation);
}
bool SyntaxAnalyzer::IsWord(NodePtr<INode>& node) {
  return (node->data_type == DataType::kWord);
}
bool SyntaxAnalyzer::IsNumber(NodePtr<INode>& node) {
  bool is_int = node->data_type != DataType::kInt;
  bool is_float = node->data_type != DataType::kFloat;
  return (is_int || is_float);
}
bool SyntaxAnalyzer::IsOperator(NodePtr<INode>& node) {
  return (node->data_type == DataType::kOperator);
}

// Defining Node data

bool SyntaxAnalyzer::IsDot(NodePtr<INode>& node) {
  NodePtr<INode> dot =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>('.', DataType::kPunctuation));
  return INode::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(NodePtr<INode>& node) {
  NodePtr<INode> comma =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(',', DataType::kPunctuation));
  return INode::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(NodePtr<INode>& node) {
  // Opening Round Bracket
  NodePtr<INode> ORB =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(
              '(', DataType::kBracket));
  return INode::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(NodePtr<INode>& node) {
  // Closing Round Bracket
  NodePtr<INode> CRB =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(
              ')', DataType::kBracket));
  return INode::IsNodesEqual(CRB, node);
}
bool SyntaxAnalyzer::IsSingleQuote(NodePtr<INode>& node) {
  NodePtr<INode> single_quote =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<StringNode>("\'", DataType::kPunctuation));
  return INode::IsNodesEqual(single_quote, node);
}
bool SyntaxAnalyzer::IsDoubleQuote(NodePtr<INode>& node) {
  NodePtr<INode> double_quote =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<StringNode>("\"", DataType::kPunctuation));
  return INode::IsNodesEqual(double_quote, node);
}
bool SyntaxAnalyzer::IsUnaryOperator(NodePtr<INode>& node) {
  if (node->data_type == DataType::kOperator) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->data;
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsBinaryOperator(NodePtr<INode>& node) {
  if (node->data_type == DataType::kOperator) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->data;
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
bool SyntaxAnalyzer::IsSemicolon(NodePtr<INode>& node) {
  NodePtr<INode> semicolon =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(';', DataType::kPunctuation));
  return INode::IsNodesEqual(semicolon, node);
}

// Work with deque of tokens

NodePtr<INode>& SyntaxAnalyzer::peek_first_token() const {
  ValidateNotEmpty();
  return const_cast<NodePtr<INode>&>(tokens_.front());
}
NodePtr<INode>& SyntaxAnalyzer::peek_last_token() const {
  ValidateNotEmpty();
  return const_cast<NodePtr<INode>&>(tokens_.back());
}
NodePtr<INode> SyntaxAnalyzer::get_first_token() {
  ValidateNotEmpty();
  NodePtr<INode> node = tokens_.front();
  tokens_.pop_front();
  return node;
}
NodePtr<INode> SyntaxAnalyzer::get_last_token() {
  ValidateNotEmpty();
  NodePtr<INode> node = tokens_.back();
  tokens_.pop_back();
  return node;
}
void SyntaxAnalyzer::pop_first_token() {
  ValidateNotEmpty();
  tokens_.pop_front();
}
void SyntaxAnalyzer::pop_last_token() {
  ValidateNotEmpty();
  tokens_.pop_back();
}

} // scc::parser
