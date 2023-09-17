#include "SCC/syntax_analyzer.h"

// Validation

void SyntaxAnalyzer::ValidateNotEmpty() const {
  if (tokens_array_.empty()) {
    LOGE << "expected that tokens' array is not empty";
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsWord(std::shared_ptr<Node>& node) const {
  if (node->get_type() != DataType::WORD) {
    LOGE << "expected word in line " << node->get_line();
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsOpeningRoundBracket(
    std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsOpeningRoundBracket(node)) {
    LOGE << "expected an opening round bracket in line "
        << node->get_line();
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsClosingRoundBracket(node)) {
    LOGE << "expected a closing round bracket in line "
        << node->get_line();
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsSingleQuote(std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOGE << "expected a single quote in line " << node->get_line();
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOGE << "expected a double quote in line " << node->get_line();
    end(EXIT_FAILURE);
  }
}

// Defining Node Datatype

bool SyntaxAnalyzer::IsBracket(std::shared_ptr<Node>& node) {
  return (node->get_type() == DataType::BRACKET);
}
bool SyntaxAnalyzer::IsPunctuation(std::shared_ptr<Node>& node) {
  return (node->get_type() == DataType::PUNCTUATION);
}
bool SyntaxAnalyzer::IsWord(std::shared_ptr<Node>& node) {
  return (node->get_type() == DataType::WORD);
}
bool SyntaxAnalyzer::IsNumber(std::shared_ptr<Node>& node) {
  bool is_int = node->get_type() != DataType::INT_NUMBER;
  bool is_float = node->get_type() != DataType::FLOAT_NUMBER;
  return (is_int || is_float);
}
bool SyntaxAnalyzer::IsOperator(std::shared_ptr<Node>& node) {
  return (node->get_type() == DataType::OPERATOR);
}

// Defining Node data

bool SyntaxAnalyzer::IsDot(std::shared_ptr<Node>& node) {
  std::shared_ptr<Node> dot =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>('.', DataType::PUNCTUATION));
  return Node::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(std::shared_ptr<Node>& node) {
  std::shared_ptr<Node> comma =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(',', DataType::PUNCTUATION));
  return Node::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(std::shared_ptr<Node>& node) {
  // Opening Round Bracket
  std::shared_ptr<Node> ORB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              '(', DataType::BRACKET));
  return Node::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(std::shared_ptr<Node>& node) {
  // Closing Round Bracket
  std::shared_ptr<Node> CRB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              ')', DataType::BRACKET));
  return Node::IsNodesEqual(CRB, node);
}
bool SyntaxAnalyzer::IsSingleQuote(std::shared_ptr<Node>& node) {
  std::shared_ptr<Node> single_quote =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<StringNode>("\'", DataType::PUNCTUATION));
  return Node::IsNodesEqual(single_quote, node);
}
bool SyntaxAnalyzer::IsDoubleQuote(std::shared_ptr<Node>& node) {
  std::shared_ptr<Node> double_quote =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<StringNode>("\"", DataType::PUNCTUATION));
  return Node::IsNodesEqual(double_quote, node);
}
bool SyntaxAnalyzer::IsUnaryOperator(std::shared_ptr<Node>& node) {
  if (node->get_type() == DataType::OPERATOR) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->get_data();
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsBinaryOperator(std::shared_ptr<Node>& node) {
  if (node->get_type() == DataType::OPERATOR) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->get_data();
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
bool SyntaxAnalyzer::IsSemicolon(std::shared_ptr<Node>& node) {
  std::shared_ptr<Node> semicolon =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(';', DataType::PUNCTUATION));
  return Node::IsNodesEqual(semicolon, node);
}

void SyntaxAnalyzer::MakeKinship(std::shared_ptr<Node>& parent,
                                 std::shared_ptr<Node>& child) {
  parent->AddChild(child);
  child->set_parent(parent);
}

// Work with deque of tokens

std::shared_ptr<Node>& SyntaxAnalyzer::peek_first_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<Node>&>(tokens_array_.front());
}
std::shared_ptr<Node>& SyntaxAnalyzer::peek_last_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<Node>&>(tokens_array_.back());
}
std::shared_ptr<Node> SyntaxAnalyzer::get_first_token() {
  this->ValidateNotEmpty();
  std::shared_ptr<Node> node = tokens_array_.front();
  tokens_array_.pop_front();
  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::get_last_token() {
  this->ValidateNotEmpty();
  std::shared_ptr<Node> node = tokens_array_.back();
  tokens_array_.pop_back();
  return node;
}
void SyntaxAnalyzer::pop_first_token() {
  this->ValidateNotEmpty();
  tokens_array_.pop_front();
}
void SyntaxAnalyzer::pop_last_token() {
  this->ValidateNotEmpty();
  tokens_array_.pop_back();
}
