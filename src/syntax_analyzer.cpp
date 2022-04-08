#include "SCC/syntax_analyzer.h"

std::shared_ptr<Node> SyntaxAnalyzer::Analyze(
    std::deque<std::shared_ptr<Node>> tokens_array) {
  tokens_array_ = std::move(tokens_array);

  if (tokens_array_.empty()) {
    LOG(TRACE, "empty tokens array");
    return nullptr;
  } else {
    return this->General();
  }
}

std::shared_ptr<Node> SyntaxAnalyzer::General() {
  std::shared_ptr<Node> root, child;
  root->set_st_type(StatementType::query);

  // TODO: root = program with a lot of queries as descendants
  child = this->GetDL();

  root->AddChild(child);
  child->set_parent(root);

  return root;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<Node> node = nullptr;

  auto first_token = tokens_array_.front();
  this->ValidateIsWord(first_token);

  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(first_token);

  std::string key_word = key_word_node->get_data();

  bool is_CREATE = key_word == "CREATE";
  bool is_ALTER = key_word == "ALTER";
  bool is_DROP = key_word == "DROP";

  bool is_UPDATE = key_word == "UPDATE";
  bool is_DELETE = key_word == "DELETE";
  bool is_INSERT = key_word == "INSERT";

  if (is_CREATE || is_ALTER || is_DROP) {             // It is DDL Statement
    node = this->GetDDLSt();
  } else if (is_UPDATE || is_DELETE || is_INSERT) {    // It is DML Statement
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

  if (is_CREATE) {
    if (is_DATABASE) { child = this->GetCreateDatabaseSt(); }
    else if (is_TABLE) { child = this->GetCreateTableSt(); }
    else {
      LOG(ERROR, "unknown CREATE DDL statement");
    }
  } else if (is_ALTER) {
    if (is_TABLE) { child = this->GetAlterTableSt(); }
    else {
      LOG(ERROR, "unknown ALTER DDL statement");
    }
  } else if (is_DROP) {
    if (is_DATABASE) { child = this->GetDropDatabaseSt(); }
    else if (is_TABLE) { child = this->GetDropTableSt(); }
    else {
      LOG(ERROR, "unknown DROP DDL statement");
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

  if (is_UPDATE) { child = this->GetUpdateSt(); }
  else if (is_DELETE) { child = this->GetDeleteSt(); }
  else if (is_INSERT) { child = this->GetInsertSt(); }
  else {
    LOG(ERROR, "unknown DML statement");
  }

  child->set_parent(node);
  node->AddChild(child);

  return node;
}

// DDL
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node->set_st_type(StatementType::createDatabaseStatement);

  if (tokens_array_.empty()) {
    LOG(ERROR, "databaseName is missed");
  } else {
    this->ValidateIsWord(tokens_array_.front());
    database_name = this->GetName();

    node->AddChild(database_name);
    database_name->set_parent(node);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateTableSt() {
  std::shared_ptr<Node> node, table_name, table_definition;
  node->set_st_type(StatementType::createTableStatement);

  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
    return node;
  } else {
    // Get tableName
    this->ValidateIsWord(tokens_array_.front());
    table_name = this->GetName();

    node->AddChild(table_name);
    table_name->set_parent(node);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsRoundBracket(tokens_array_.front()); // TODO: open bracket?
    tokens_array_.pop_front();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "table definition is missed");
    return node;
  } else {
    table_definition = this->GetTableDefinition();

    node->AddChild(table_definition);
    table_definition->set_parent(node);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
  } else {
    this->ValidateIsRoundBracket(tokens_array_.front());
    tokens_array_.pop_front();
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetAlterTableSt() {
  std::shared_ptr<Node> node;
  node->set_st_type(StatementType::alterTableStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node->set_st_type(StatementType::dropDatabaseStatement);

  // First database_name
  if (tokens_array_.empty()) {
    LOG(ERROR, "databaseName is missed");
  } else {
    this->ValidateIsWord(tokens_array_.front());
    database_name = this->GetName();

    node->AddChild(database_name);
    database_name->set_parent(node);
  }

  if (!tokens_array_.empty()) {
    // TODO: get ',' and other databaseNames
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropTableSt() {
  std::shared_ptr<Node> node, table_name;
  node->set_st_type(StatementType::dropTableStatement);

  // First tableName
  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
  } else {
    this->ValidateIsWord(tokens_array_.front());
    table_name = this->GetName();

    node->AddChild(table_name);
    table_name->set_parent(node);
  }

  if (!tokens_array_.empty()) {
    // TODO: get ',' and other tableNames
  }

  return node;
}

// DML
std::shared_ptr<Node> SyntaxAnalyzer::GetInsertSt() {
  std::shared_ptr<Node> node;
  node->set_st_type(StatementType::insertStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDeleteSt() {
  std::shared_ptr<Node> node;
  node->set_st_type(StatementType::deleteStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetUpdateSt() {
  std::shared_ptr<Node> node;
  node->set_st_type(StatementType::updateStatement);

  return node;
}

// Basic
std::shared_ptr<Node> SyntaxAnalyzer::GetTableDefinition() {
  std::shared_ptr<Node> node, argument, separator;
  std::shared_ptr<Node> comma =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(',', DataType::COMMA));

  this->ValidateIsWord(tokens_array_.front());
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(tokens_array_.front());

  std::string key_word = key_word_node->get_data();
  bool is_tableConstraint =
      key_word == "CONSTRAINT"
      || key_word == "PRIMARY"
      || key_word == "FOREIGN";

  if (is_tableConstraint) {
    argument = this->GetTableConstraint();
  } else {
    argument = this->GetColumnDefinition();
  }

  node->AddChild(argument);
  argument->set_parent(node);

  if (!tokens_array_.empty()) {
    if (Node::IsNodesEqual(tokens_array_.front(), comma)) {
      separator = this->GetTableDefinition();

      node->AddChild(separator);
      separator->set_parent(node);
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetColumnDefinition() {
  std::shared_ptr<Node> node, column_name, datatype;
  node->set_st_type(StatementType::columnDefinition);

  // Get columnName
  column_name = this->GetIdentifier();
  node->AddChild(column_name);
  column_name->set_parent(node);

  // Get datatype
  if (tokens_array_.empty()) {
    LOG(ERROR, "column datatype is missed");
    return node;
  } else {
    datatype = this->GetDataType();

    node->AddChild(datatype);
    datatype->set_parent(node);
  }

  // Get other options
  if (!tokens_array_.empty()) {
    // TODO: get options such as IDENTITY or (NOT) NULL
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDataType() {
  this->ValidateIsWord(tokens_array_.front());

  std::shared_ptr<StringNode> node =
      std::dynamic_pointer_cast<StringNode>(tokens_array_.front());
  tokens_array_.pop_front();

  std::string datatype = node->get_data();
  StatementType SQL_datatype;
  if (datatype == "int") { SQL_datatype = StatementType::SQL_int; }
  else if (datatype == "float") { SQL_datatype = StatementType::SQL_float; }
  else if (datatype == "char") { SQL_datatype = StatementType::SQL_char; }
  else if (datatype == "varchar") { SQL_datatype = StatementType::SQL_varchar; }
  else {
    LOG(ERROR, "invalid column datatype");
    return node;
  }

  node->set_st_type(SQL_datatype);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetTableConstraint() {
  std::shared_ptr<Node> node, constraint_name, key;
  node->set_st_type(StatementType::tableConstraint);

  this->ValidateIsWord(tokens_array_.front());
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(tokens_array_.front());

  std::string key_word = key_word_node->get_data();
  bool is_full_construction = key_word == "CONSTRAINT";
  if (is_full_construction) {
    tokens_array_.pop_front();
    node->AddChild(std::dynamic_pointer_cast<Node>(key_word_node));
    key_word_node->set_parent(node);

    if (tokens_array_.empty()) {
      LOG(ERROR, "constraint name is missed");
      return node;
    } else {
      this->ValidateIsWord(tokens_array_.front());
      constraint_name = tokens_array_.front();
      tokens_array_.pop_front();

      node->AddChild(constraint_name);
      constraint_name->set_parent(node);
    }
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "constraint is missed");
    return node;
  } else {
    // Get PRIMARY | FOREIGN
    this->ValidateIsWord(tokens_array_.front());
    std::shared_ptr<StringNode> kind_of_constraint_node =
        std::dynamic_pointer_cast<StringNode>(tokens_array_.front());
    tokens_array_.pop_front();
    std::string kind_of_key = kind_of_constraint_node->get_data();

    // Get KEY
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid constraint definition");
      return node;
    }
    this->ValidateIsWord(tokens_array_.front());
    std::shared_ptr<StringNode> KEY_key_word =
        std::dynamic_pointer_cast<StringNode>(tokens_array_.front());
    tokens_array_.pop_front();

    if (kind_of_key == "PRIMARY") {
      key = this->GetPrimaryKey();
    } else if (kind_of_key == "FOREIGN") {
      key = this->GetForeignKey();
    } else {
      LOG(ERROR, "unknown kind of constraint: " << kind_of_key);
      return node;
    }

    node->AddChild(key);
    key->set_parent(node);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPrimaryKey() {
  std::shared_ptr<Node> node, column_name;
  node->set_st_type(StatementType::primaryKey);

  // Get PRIMARY KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsRoundBracket(tokens_array_.front()); // TODO: open bracket?
    tokens_array_.pop_front();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return node;
  } else {
    column_name = this->GetIdentifier();

    node->AddChild(column_name);
    column_name->set_parent(node);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return node;
  } else {
    this->ValidateIsRoundBracket(tokens_array_.front());
    tokens_array_.pop_front();
  }
  // TODO: get listOf columnNames

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetForeignKey() {
  std::shared_ptr<Node> node, column_name;
  node->set_st_type(StatementType::foreignKey);

  // Get FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsRoundBracket(tokens_array_.front()); // TODO: open bracket?
    tokens_array_.pop_front();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return node;
  } else {
    column_name = this->GetIdentifier();

    node->AddChild(column_name);
    column_name->set_parent(node);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return node;
  } else {
    this->ValidateIsRoundBracket(tokens_array_.front());
    tokens_array_.pop_front();
  }

  // Get Referenced column
  // TODO:

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
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifier() {
  std::shared_ptr<Node> identifier = tokens_array_.front();
  tokens_array_.pop_front();
  this->ValidateIsWord(identifier);

  identifier->set_st_type(StatementType::identifier);

  return identifier;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifiers() {
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

template<typename GetFunc>
std::shared_ptr<Node> SyntaxAnalyzer::GetListOf(GetFunc GetArg) {
  std::shared_ptr<Node> argument, separator, next_separator;
  std::shared_ptr<Node> comma =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(',', DataType::COMMA));

  // Get separator (comma)
  separator = tokens_array_.front();
  tokens_array_.pop_front();
  if (!Node::IsNodesEqual(separator, comma)) {
    LOG(ERROR, "separator for the listOf is not a comma");
    return separator;
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "argument is missed");
    return separator;
  } else {
    argument = GetArg();

    separator->AddChild(argument);
    argument->set_parent(separator);
  }

  if (!tokens_array_.empty()) {
    if (Node::IsNodesEqual(tokens_array_.front(), comma)) {
      next_separator = this->GetListOf(GetArg);
      separator->AddChild(next_separator);
      next_separator->set_parent(separator);
    }
  }

  return separator;
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
