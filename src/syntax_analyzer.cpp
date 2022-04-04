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
  std::shared_ptr<Node> child = this->GetDL();
  root->AddChild(child);
  child->set_parent(root);

  return root;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<Node> node = nullptr;

  auto first_token = tokens_array_.front();
  std::shared_ptr<StringNode> key_word_node;

  this->ValidateIsWord(first_token);
  key_word_node = std::dynamic_pointer_cast<StringNode>(first_token);

  std::string key_word = key_word_node->get_data();

  bool is_CREATE = key_word == "CREATE";
  bool is_ALTER = key_word == "ALTER";
  bool is_DROP = key_word == "DROP";

  bool is_UPDATE = key_word == "UPDATE";
  bool is_DELETE = key_word == "DELETE";
  bool is_INSERT = key_word == "INSERT";

  if (is_CREATE || is_ALTER || is_DROP) {             // It is DDL Statement
    node = this->GetDDLSt();
  } else if (is_UPDATE || is_DELETE || is_INSERT){    // It is DML Statement
    node = this->GetDMLSt();
  } else {
    LOG(TRACE, "tokens array does not contain DDL or DML query");
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDDLSt() {
  std::shared_ptr<Node> node, child;
  node->set_st_type(StatementType::ddlStatement);

  // Get first token
  std::shared_ptr<StringNode> first_key_word_node =
      std::dynamic_pointer_cast<StringNode>(tokens_array_.front());
  tokens_array_.pop_front();

  // Get second token
  if (tokens_array_.empty()) {
    return node;
  }
  this->ValidateIsWord(tokens_array_.front());
  std::shared_ptr<StringNode> second_key_word_node =
      std::dynamic_pointer_cast<StringNode>(tokens_array_.front());
  tokens_array_.pop_front();

  // Define next rule
  std::string fst_kw = first_key_word_node->get_data();
  std::string snd_kw = second_key_word_node->get_data();

  bool is_CREATE = fst_kw == "CREATE";
  bool is_ALTER = fst_kw == "ALTER";
  bool is_DROP = fst_kw == "DROP";

  bool is_DATABASE = snd_kw == "DATABASE";
  bool is_TABLE = snd_kw == "TABLE";

  if (is_CREATE) {                            // CREATE
    if (is_DATABASE) {
      child = this->GetCreateDatabaseSt();
    } else if (is_TABLE) {
      child = this->GetCreateTableSt();
    }
  } else if (is_ALTER) {                      // ALTER
    if (is_TABLE) {
      child->set_st_type(StatementType::alterTableStatement);
      child = this->GetAlterTableSt();
    }
  } else if (is_DROP) {                       // DROP
    if (is_DATABASE) {
      child->set_st_type(StatementType::dropDatabaseStatement);
      child = this->GetDropDatabaseSt();
    } else if (is_TABLE){
      child->set_st_type(StatementType::dropTableStatement);
      child = this->GetDropTableSt();
    }
  }

  child->set_parent(node);
  node->AddChild(child);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  std::shared_ptr<Node> node, child;
  node->set_st_type(StatementType::dmlStatement);

  // Get first token
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(tokens_array_.front());
  tokens_array_.pop_front();

  // Define next rule
  std::string fst_kw = key_word_node->get_data();

  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) {                  // UPDATE
    child->set_st_type(StatementType::updateStatement);
    child = this->GetUpdateSt();
  } else if (is_DELETE) {           // DELETE
    child->set_st_type(StatementType::deleteStatement);
    child = this->GetDeleteSt();
  } else if (is_INSERT) {           // INSERT
    child->set_st_type(StatementType::insertStatement);
    child = this->GetInsertSt();
  }

  child->set_parent(node);
  node->AddChild(child);

  return node;
}

// DDL
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node->set_st_type(StatementType::createDatabaseStatement);

  if (!tokens_array_.empty()) {
    this->ValidateIsWord(tokens_array_.front());
    database_name = this->GetName();
    node->AddChild(database_name);
    database_name->set_parent(node);
  } else {
    LOG(ERROR, "databaseName is missed");
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateTableSt() {
  std::shared_ptr<Node> node, tableName, content;
  node->set_st_type(StatementType::createTableStatement);

  if (!tokens_array_.empty()) {
    this->ValidateIsWord(tokens_array_.front());
    tableName = this->GetName();
    node->AddChild(tableName);
    tableName->set_parent(node);
  }

  if (!tokens_array_.empty()) {
    this->ValidateIsRoundBracket(tokens_array_.front());
    tokens_array_.pop_front();
  } else {
    // Error
  }

  if (!tokens_array_.empty()) {
    // TODO: Get list of ColDef or Constaint
  } else {
    // Error
  }

  if (!tokens_array_.empty()) {
    this->ValidateIsRoundBracket(tokens_array_.front());
    tokens_array_.pop_front();
  } else {
    // Error
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetAlterTableSt() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropDatabaseSt() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropTableSt() {
  std::shared_ptr<Node> node;

  return node;
}

// DML
std::shared_ptr<Node> SyntaxAnalyzer::GetInsertSt() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDeleteSt() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetUpdateSt() {
  std::shared_ptr<Node> node;

  return node;
}

// Basic
std::shared_ptr<Node> SyntaxAnalyzer::GetColumnDefinition() {
  std::shared_ptr<Node> node;

  // name -- datatype -- [] -- ,
  if (tokens_array_.empty()) {

  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDataType() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetTableConstraint()  {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPrimaryKey() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetForeignKey() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetCondition() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetORCondition() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetANDCondition() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetNOTCondition() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPredicate() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetExpression() {
  std::shared_ptr<Node> node;

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetName() {
  std::shared_ptr<Node> node, identifier, next_identifiers;
  node->set_st_type(StatementType::name);

  identifier = std::dynamic_pointer_cast<Node>(tokens_array_.front());
  identifier->set_st_type(StatementType::identifier);
  tokens_array_.pop_front();
  this->ValidateIsWord(identifier);

  if (!tokens_array_.empty()) {
    next_identifiers = std::dynamic_pointer_cast<Node>(tokens_array_.front());
    if (IsDot(next_identifiers)) {
      next_identifiers = this->GetIdentifiers();
      node->AddChild(next_identifiers);
      next_identifiers->set_parent(node);
    }
  }

  node->AddChild(identifier);
  identifier->set_parent(node);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifiers(){
  std::shared_ptr<Node> dot, identifier, next_identifiers;

  dot = std::dynamic_pointer_cast<Node>(tokens_array_.front());
  tokens_array_.pop_front();
  dot->set_st_type(StatementType::delimiter_dot);

  if (tokens_array_.empty()) {
    LOG(ERROR, "Bad name, which ends on dot");
    return nullptr;
  } else {
    identifier = std::dynamic_pointer_cast<Node>(tokens_array_.front());
    tokens_array_.pop_front();
    identifier->set_st_type(StatementType::identifier);
    dot->AddChild(identifier);
    identifier->set_parent(dot);
  }

  if (!tokens_array_.empty()) {
    next_identifiers = std::dynamic_pointer_cast<Node>(tokens_array_.front());
    if (IsDot(next_identifiers)) {
      next_identifiers = this->GetIdentifiers();
      dot->AddChild(next_identifiers);
      next_identifiers->set_parent(dot);
    }
  }

  return dot;
}

void SyntaxAnalyzer::ValidateIsWord(std::shared_ptr<Node> &node) const {
  if (node->get_type() == DataType::WORD) {
    LOG(ERROR, "token is not a word");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a word");
}
void SyntaxAnalyzer::ValidateIsRoundBracket(std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsRoundBracket(node)) {
    LOG(ERROR, "token is not a round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a round bracket");
}

bool SyntaxAnalyzer::IsPunctuation(std::shared_ptr<Node> &node) {
  if (node->get_type() == DataType::PUNCTUATION) {
    LOG(DEBUG, "token does not apply to punctuation: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token applies to punctuation");
    return true;
  }
}

bool SyntaxAnalyzer::IsDot(std::shared_ptr<Node> &node) {
  if (SyntaxAnalyzer::IsPunctuation(node)) {
    if (node->get_type() == DataType::DOT) {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsRoundBracket(std::shared_ptr<Node> &node) {
  if (SyntaxAnalyzer::IsPunctuation(node)) {
    if (node->get_type() == DataType::ROUND_BRACKET) {
      return true;
    }
  }
  return false;
}
