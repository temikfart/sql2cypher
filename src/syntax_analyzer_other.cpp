#include "SCC/syntax_analyzer.h"

// Validation

void SyntaxAnalyzer::ValidateNotEmpty() const {
  if (tokens_array_.empty()) {
    LOG(ERROR, "token's array are empty");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token's array are not empty");
}
void SyntaxAnalyzer::ValidateIsWord(std::shared_ptr<Node>& node) const {
  if (node->get_type() != DataType::WORD) {
    LOG(ERROR, "token is not a word");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a word");
}
void SyntaxAnalyzer::ValidateIsOpeningRoundBracket(
    std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsOpeningRoundBracket(node)) {
    LOG(ERROR, "token is not a opening round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a opening round bracket");
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsClosingRoundBracket(node)) {
    LOG(ERROR, "token is not a closing round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a closing round bracket");
}
void SyntaxAnalyzer::ValidateIsSingleQuote(std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOG(ERROR, "token is not a single quote");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a single quote");
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(std::shared_ptr<Node>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOG(ERROR, "token is not a double quote");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a double quote");
}

// Defining Node Datatype

bool SyntaxAnalyzer::IsBracket(std::shared_ptr<Node>& node) {
  if (node->get_type() != DataType::BRACKET) {
    LOG(DEBUG, "token is not a bracket: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a bracket");
    return true;
  }
}
bool SyntaxAnalyzer::IsPunctuation(std::shared_ptr<Node>& node) {
  if (node->get_type() != DataType::PUNCTUATION) {
    LOG(DEBUG, "token does not apply to punctuation: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token applies to punctuation");
    return true;
  }
}
bool SyntaxAnalyzer::IsWord(std::shared_ptr<Node>& node) {
  if (node->get_type() != DataType::WORD) {
    LOG(DEBUG, "token is not a word: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a word");
    return true;
  }
}
bool SyntaxAnalyzer::IsNumber(std::shared_ptr<Node>& node) {
  bool is_int = node->get_type() != DataType::INT_NUMBER;
  bool is_float = node->get_type() != DataType::FLOAT_NUMBER;
  if (!is_int && !is_float) {
    LOG(DEBUG, "token is not a number: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a number");
    return true;
  }
}
bool SyntaxAnalyzer::IsOperator(std::shared_ptr<Node>& node) {
  if (node->get_type() != DataType::OPERATOR) {
    LOG(DEBUG, "token is not a operator: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a operator");
    return true;
  }
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
