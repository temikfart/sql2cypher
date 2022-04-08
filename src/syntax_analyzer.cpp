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
  SyntaxAnalyzer::MakeKinship(root, child);

  return root;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<Node> node = nullptr;

  auto first_token = this->peek_first_token();
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
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

  // Get second token
  if (tokens_array_.empty()) {
    return node;
  }
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<StringNode> second_key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

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

  SyntaxAnalyzer::MakeKinship(node, child);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  std::shared_ptr<Node> node, child;
  node->set_st_type(StatementType::dmlStatement);

  // Get first token
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

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

  SyntaxAnalyzer::MakeKinship(node, child);

  return node;
}

// DDL
std::shared_ptr<Node> SyntaxAnalyzer::GetCreateDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node->set_st_type(StatementType::createDatabaseStatement);

  if (tokens_array_.empty()) {
    LOG(ERROR, "databaseName is missed");
  } else {
    this->ValidateIsWord(this->peek_first_token());
    database_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, database_name);
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
    this->ValidateIsWord(this->peek_first_token());
    table_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, table_name);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token()); // TODO: open bracket?
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "table definition is missed");
    return node;
  } else {
    table_definition = this->GetTableDefinition();

    SyntaxAnalyzer::MakeKinship(node, table_definition);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
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
    this->ValidateIsWord(this->peek_first_token());
    database_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, database_name);
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
    this->ValidateIsWord(peek_first_token());
    table_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(node, table_name);
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

  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<StringNode> key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());

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

  SyntaxAnalyzer::MakeKinship(node, argument);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetTableDefinition();

      SyntaxAnalyzer::MakeKinship(node, separator);
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetColumnDefinition() {
  std::shared_ptr<Node> node, column_name, datatype;
  node->set_st_type(StatementType::columnDefinition);

  // Get columnName
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(node, column_name);

  // Get datatype
  if (tokens_array_.empty()) {
    LOG(ERROR, "column datatype is missed");
    return node;
  } else {
    datatype = this->GetDataType();

    SyntaxAnalyzer::MakeKinship(node, datatype);
  }

  // Get other options
  if (!tokens_array_.empty()) {
    // TODO: get options such as IDENTITY or (NOT) NULL
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDataType() {
  this->ValidateIsWord(this->peek_first_token());

  std::shared_ptr<StringNode> node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  this->pop_first_token();

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

  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<Node> key_word_node = this->peek_first_token();

  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(key_word_node)->get_data();
  bool is_full_construction = key_word == "CONSTRAINT";
  if (is_full_construction) {
    this->pop_first_token();
    SyntaxAnalyzer::MakeKinship(node, key_word_node);

    if (tokens_array_.empty()) {
      LOG(ERROR, "constraint name is missed");
      return node;
    } else {
      this->ValidateIsWord(this->peek_first_token());
      constraint_name = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, constraint_name);
    }
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "constraint is missed");
    return node;
  } else {
    // Get PRIMARY | FOREIGN
    this->ValidateIsWord(this->peek_first_token());
    std::shared_ptr<StringNode> kind_of_constraint_node =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    this->pop_first_token();
    std::string kind_of_key = kind_of_constraint_node->get_data();

    // Get KEY
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid constraint definition");
      return node;
    }
    this->ValidateIsWord(this->peek_first_token());
    std::shared_ptr<StringNode> KEY_key_word =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    this->pop_first_token();

    if (kind_of_key == "PRIMARY") {
      key = this->GetPrimaryKey();
    } else if (kind_of_key == "FOREIGN") {
      key = this->GetForeignKey();
    } else {
      LOG(ERROR, "unknown kind of constraint: " << kind_of_key);
      return node;
    }

    SyntaxAnalyzer::MakeKinship(node, key);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPrimaryKey() {
  std::shared_ptr<Node> node, column_name, separator;
  node->set_st_type(StatementType::primaryKey);

  // Get PRIMARY KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token()); // TODO: open bracket?
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return node;
  } else {
    column_name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, column_name);

    // Get listOf(column_names)
    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
        separator = this->GetListOf(StatementType::identifier);

        SyntaxAnalyzer::MakeKinship(node, separator);
      }
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return node;
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetForeignKey() {
  std::shared_ptr<Node> node, column_name, separator, reference;
  node->set_st_type(StatementType::foreignKey);

  // Get FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token()); // TODO: open bracket?
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return node;
  } else {
    column_name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, column_name);

    // Get listOf(column_names)
    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
        separator = this->GetListOf(StatementType::identifier);

        SyntaxAnalyzer::MakeKinship(node, separator);
      }
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return node;
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  // Get Reference
  if (tokens_array_.empty()) {
    LOG(ERROR, "reference is missed");
    return node;
  } else {
    reference = this->GetReference();

    SyntaxAnalyzer::MakeKinship(node, reference);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetReference() {
  std::shared_ptr<Node> reference, ref_table_name;

  // Get REFERENCES
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<StringNode> ref_key_word_node =
      std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
  std::string ref_key_word = ref_key_word_node->get_data();

  if (ref_key_word != "REFERENCES") {
    LOG(ERROR, "incorrect reference");
    return reference;
  }

  reference = get_first_token();
  reference->set_st_type(StatementType::reference);

  // Get referenced table or columns
  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
    return reference;
  } else {
    ref_table_name = this->GetName();

    SyntaxAnalyzer::MakeKinship(reference, ref_table_name);
  }

  // Get columns if present
  if (!tokens_array_.empty()) {
    std::shared_ptr<Node> ref_column_name, next_ref_column_names;

    if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
      this->pop_first_token();
    }

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid foreign key: incorrect list of columnNames");
      return reference;
    } else {
      ref_column_name = this->GetName();

      SyntaxAnalyzer::MakeKinship(reference, ref_column_name);

      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid foreign key: incorrect list of columnNames");
        return reference;
      } else {
        if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
          next_ref_column_names =
              this->GetListOf(StatementType::identifier);

          SyntaxAnalyzer::MakeKinship(reference, next_ref_column_names);
        }
      }
    }
  }

  return reference;
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

  this->ValidateIsWord(identifier);
  identifier = this->get_first_token();
  identifier->set_st_type(StatementType::identifier);

  if (!tokens_array_.empty()) {
    next_identifiers =
        std::dynamic_pointer_cast<Node>(this->peek_first_token());
    if (SyntaxAnalyzer::IsDot(next_identifiers)) {
      next_identifiers = this->GetIdentifiers();

      SyntaxAnalyzer::MakeKinship(node, next_identifiers);
    }
  }

  SyntaxAnalyzer::MakeKinship(node, identifier);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifier() {
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<Node> identifier = this->get_first_token();
  identifier->set_st_type(StatementType::identifier);

  return identifier;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifiers() {
  std::shared_ptr<Node> dot, identifier, next_identifiers;

  dot = this->get_first_token();
  dot->set_st_type(StatementType::delimiter_dot);

  if (tokens_array_.empty()) {
    LOG(ERROR, "Bad name, which ends on dot");
    return nullptr;
  } else {
    identifier = this->get_first_token();
    identifier->set_st_type(StatementType::identifier);

    SyntaxAnalyzer::MakeKinship(dot, identifier);
  }

  if (!tokens_array_.empty()) {
    next_identifiers = this->peek_first_token();
    if (SyntaxAnalyzer::IsDot(next_identifiers)) {
      next_identifiers = this->GetIdentifiers();

      SyntaxAnalyzer::MakeKinship(dot, next_identifiers);
    }
  }

  return dot;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetListOf(
    StatementType get_function_type) {
  std::shared_ptr<Node> argument, separator, next_separator;

  // Get separator (comma)
  separator = this->get_first_token();
  if (!SyntaxAnalyzer::IsComma(separator)) {
    LOG(ERROR, "separator for the listOf is not a comma");
    return separator;
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "argument is missed");
    return separator;
  } else {
    switch (get_function_type) {
      case StatementType::identifier:
        argument = this->GetIdentifier();
        break;
      case StatementType::columnDefinition:
        argument = this->GetColumnDefinition();
        break;
      case StatementType::tableConstraint:
        argument = this->GetTableConstraint();
        break;
      default:
        LOG(ERROR, "unknown statement type for the listOf");
        break;
    }

    SyntaxAnalyzer::MakeKinship(separator, argument);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      next_separator = GetListOf(get_function_type);

      SyntaxAnalyzer::MakeKinship(separator, next_separator);
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
void SyntaxAnalyzer::ValidateIsOpeningRoundBracket(
    std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsOpeningRoundBracket(node)) {
    LOG(ERROR, "token is not a opening round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a opening round bracket");
}
void SyntaxAnalyzer::ValidateIsClosingRoundBracket(
    std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsClosingRoundBracket(node)) {
    LOG(ERROR, "token is not a closing round bracket");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a closing round bracket");
}

bool SyntaxAnalyzer::IsPunctuation(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::PUNCTUATION) {
    LOG(DEBUG, "token does not apply to punctuation: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token applies to punctuation");
    return true;
  }
}

bool SyntaxAnalyzer::IsDot(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> dot =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>('.', DataType::DOT));
  return Node::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> comma =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(',', DataType::COMMA));
  return Node::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(std::shared_ptr<Node> &node) {
  // Opening Round Bracket
  std::shared_ptr<Node> ORB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              '(', DataType::OpeningRoundBracket));
  return Node::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(std::shared_ptr<Node> &node) {
  // Closing Round Bracket
  std::shared_ptr<Node> CRB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              ')', DataType::ClosingRoundBracket));
  return Node::IsNodesEqual(CRB, node);
}

void SyntaxAnalyzer::MakeKinship(std::shared_ptr<Node>& parent,
                                 std::shared_ptr<Node>& child) {
  parent->AddChild(child);
  child->set_parent(parent);
}

std::shared_ptr<Node>& SyntaxAnalyzer::peek_first_token() {
  return tokens_array_.front();
}
std::shared_ptr<Node>& SyntaxAnalyzer::peek_last_token() {
  return tokens_array_.back();
}
std::shared_ptr<Node> SyntaxAnalyzer::get_first_token() {
  std::shared_ptr<Node> node = tokens_array_.front();
  tokens_array_.pop_front();
  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::get_last_token() {
  std::shared_ptr<Node> node = tokens_array_.back();
  tokens_array_.pop_back();
  return node;
}
void SyntaxAnalyzer::pop_first_token() {
  tokens_array_.pop_front();
}
void SyntaxAnalyzer::pop_last_token() {
  tokens_array_.pop_back();
}
