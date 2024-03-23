#include "SCC/syntax_analyzer.h"

// Validation

void SyntaxAnalyzer::ValidateNotEmpty() const {
  if (tokens_array_.empty()) {
    LOGE << "expected that tokens' array is not empty";
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsWord(std::shared_ptr<INode>& node) const {
  if (node->data_type != DataType::kWord) {
    LOGE << "expected word in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsOpeningRoundBracket(
    std::shared_ptr<INode>& node) const {
  if (!SyntaxAnalyzer::IsOpeningRoundBracket(node)) {
    LOGE << "expected an opening round bracket in line "
        << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    std::shared_ptr<INode>& node) const {
  if (!SyntaxAnalyzer::IsClosingRoundBracket(node)) {
    LOGE << "expected a closing round bracket in line "
        << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsSingleQuote(std::shared_ptr<INode>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOGE << "expected a single quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(std::shared_ptr<INode>& node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOGE << "expected a double quote in line " << node->line;
    end(EXIT_FAILURE);
  }
}

// Defining Node Datatype

bool SyntaxAnalyzer::IsBracket(std::shared_ptr<INode>& node) {
  return (node->data_type == DataType::kBracket);
}
bool SyntaxAnalyzer::IsPunctuation(std::shared_ptr<INode>& node) {
  return (node->data_type == DataType::kPunctuation);
}
bool SyntaxAnalyzer::IsWord(std::shared_ptr<INode>& node) {
  return (node->data_type == DataType::kWord);
}
bool SyntaxAnalyzer::IsNumber(std::shared_ptr<INode>& node) {
  bool is_int = node->data_type != DataType::kInt;
  bool is_float = node->data_type != DataType::kFloat;
  return (is_int || is_float);
}
bool SyntaxAnalyzer::IsOperator(std::shared_ptr<INode>& node) {
  return (node->data_type == DataType::kOperator);
}

// Defining Node data

bool SyntaxAnalyzer::IsDot(std::shared_ptr<INode>& node) {
  std::shared_ptr<INode> dot =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>('.', DataType::kPunctuation));
  return INode::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(std::shared_ptr<INode>& node) {
  std::shared_ptr<INode> comma =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(',', DataType::kPunctuation));
  return INode::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(std::shared_ptr<INode>& node) {
  // Opening Round Bracket
  std::shared_ptr<INode> ORB =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(
              '(', DataType::kBracket));
  return INode::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(std::shared_ptr<INode>& node) {
  // Closing Round Bracket
  std::shared_ptr<INode> CRB =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(
              ')', DataType::kBracket));
  return INode::IsNodesEqual(CRB, node);
}
bool SyntaxAnalyzer::IsSingleQuote(std::shared_ptr<INode>& node) {
  std::shared_ptr<INode> single_quote =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<StringNode>("\'", DataType::kPunctuation));
  return INode::IsNodesEqual(single_quote, node);
}
bool SyntaxAnalyzer::IsDoubleQuote(std::shared_ptr<INode>& node) {
  std::shared_ptr<INode> double_quote =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<StringNode>("\"", DataType::kPunctuation));
  return INode::IsNodesEqual(double_quote, node);
}
bool SyntaxAnalyzer::IsUnaryOperator(std::shared_ptr<INode>& node) {
  if (node->data_type == DataType::kOperator) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->data;
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsBinaryOperator(std::shared_ptr<INode>& node) {
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
bool SyntaxAnalyzer::IsSemicolon(std::shared_ptr<INode>& node) {
  std::shared_ptr<INode> semicolon =
      std::dynamic_pointer_cast<INode>(
          std::make_shared<CharNode>(';', DataType::kPunctuation));
  return INode::IsNodesEqual(semicolon, node);
}

void SyntaxAnalyzer::MakeKinship(std::shared_ptr<INode>& parent,
                                 std::shared_ptr<INode>& child) {
  parent->AddChild(child);
  child->parent = parent;
}

// Work with deque of tokens

std::shared_ptr<INode>& SyntaxAnalyzer::peek_first_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<INode>&>(tokens_array_.front());
}
std::shared_ptr<INode>& SyntaxAnalyzer::peek_last_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<INode>&>(tokens_array_.back());
}
std::shared_ptr<INode> SyntaxAnalyzer::get_first_token() {
  this->ValidateNotEmpty();
  std::shared_ptr<INode> node = tokens_array_.front();
  tokens_array_.pop_front();
  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::get_last_token() {
  this->ValidateNotEmpty();
  std::shared_ptr<INode> node = tokens_array_.back();
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
