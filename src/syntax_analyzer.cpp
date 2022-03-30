#include "SCC/syntax_analyzer.h"

std::shared_ptr<Node> SyntaxAnalyzer::Analyze(
    std::deque<std::shared_ptr<Node>> tokens_array) {
  tokens_array_ = std::move(tokens_array);

  if (tokens_array_.empty()) {
    LOG(TRACE, "Empty tokens array");
    return nullptr;
  } else {
    return this->General();
  }
}

std::shared_ptr<Node> SyntaxAnalyzer::General() {
  std::shared_ptr<Node> root;

  root->set_st_type(StatementType::query);
  std::shared_ptr<Node> child = GetDL();
  root->AddChild(child);
  child->set_parent(root);

  return root;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<Node> node = nullptr;

  auto first_token = tokens_array_.front();
  std::shared_ptr<StringNode> key_word_node;

  this->ValidateIsFirstWord(first_token);
  key_word_node = std::static_pointer_cast<StringNode>(first_token);

  bool is_CREATE = key_word_node->get_data() == "CREATE";
  bool is_ALTER = key_word_node->get_data() == "ALTER";
  bool is_DROP = key_word_node->get_data() == "DROP";
  if (is_CREATE || is_ALTER || is_DROP) {   // It is DDL Statement
    node = GetDDLSt();
  } else {                                  // It is DML Statement
    node = GetDMLSt();
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDDLSt() {
  std::shared_ptr<Node> node = nullptr;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  std::shared_ptr<Node> node = nullptr;

  return node;
}
//std::shared_ptr<Node> SyntaxAnalyzer::GetCreateDatabaseSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetCreateTableSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetAlterTableSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetDropDatabaseSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetDropTableSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetInsertSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetDeleteSt() {}
//std::shared_ptr<Node> SyntaxAnalyzer::GetUpdateSt() {}

void SyntaxAnalyzer::ValidateIsFirstWord(std::shared_ptr<Node> &node) const {
  if (node->get_type() == DataType::WORD) {
    LOG(ERROR, "the first token is not a key word of DDL or DML");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "the first token is a key word (valid token)");
}
