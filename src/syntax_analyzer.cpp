#include "SCC/syntax_analyzer.h"

std::shared_ptr<Node> SyntaxAnalyzer::Analyze(
    std::deque<std::shared_ptr<Node>> tokens_array) {
  tokens_array_ = std::move(tokens_array);

  std::shared_ptr<Node> root;
  root = std::dynamic_pointer_cast<Node>(std::make_shared<RootNode>());
  root->set_st_type(StatementType::Program);

  if (tokens_array_.empty()) {
    LOG(TRACE, "nothing to analyze: empty tokens array");
    return nullptr;
  } else {
    std::shared_ptr<Node> query, separator;

    query = this->GetDL();
    SyntaxAnalyzer::MakeKinship(root, query);

    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsSemicolon(this->peek_first_token())) {
        separator = this->General();
        SyntaxAnalyzer::MakeKinship(root, separator);
      }
    }

    if (!tokens_array_.empty()) {
      LOG(ERROR, "array is not empty");
      exit(EXIT_FAILURE);
    }

    return root;
  }
}

// Start

std::shared_ptr<Node> SyntaxAnalyzer::General() {
  std::shared_ptr<Node> separator, query, next_queries;
  this->pop_first_token();
  separator = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  separator->set_st_type(StatementType::delimiter_semicolon);

  if (!tokens_array_.empty()) {
    query = this->GetDL();
    SyntaxAnalyzer::MakeKinship(separator, query);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsSemicolon(this->peek_first_token())) {
      next_queries = this->General();
      SyntaxAnalyzer::MakeKinship(separator, next_queries);
    }
  }

  return separator;
}
StatementType SyntaxAnalyzer::GetDLStType() {
  StatementType DLStType;

  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();

  std::vector<std::string> ddlSt_kws = {
      "CREATE", "ALTER", "DROP"
  };
  std::vector<std::string> dmlSt_kws = {
      "UPDATE", "DELETE", "INSERT"
  };

  bool is_ddlSt = std::any_of(ddlSt_kws.begin(),
                              ddlSt_kws.end(),
                              [key_word](std::string &st) {
                                return (key_word == st);
                              });
  bool is_dmlSt = std::any_of(dmlSt_kws.begin(),
                              dmlSt_kws.end(),
                              [key_word](std::string &st) {
                                return (key_word == st);
                              });

  if (is_ddlSt) {
    DLStType = StatementType::ddlStatement;
  } else if (is_dmlSt) {
    DLStType = StatementType::dmlStatement;
  } else {
    LOG(ERROR,
        "unknown DL: tokens array does not contain DDL or DML query");
    exit(EXIT_FAILURE);
  }

  return DLStType;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<Node> query, statement;
  query = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  query->set_st_type(StatementType::query);

  this->ValidateIsWord(this->peek_first_token());

  switch (this->GetDLStType()) {
    case StatementType::ddlStatement:
      statement = this->GetDDLSt();
      break;
    case StatementType::dmlStatement:
      statement = this->GetDMLSt();
      break;
    default:
      LOG(ERROR, "unknown DL");
      exit(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(query, statement);

  return query;
}

StatementType SyntaxAnalyzer::GetDDLStType() {
  StatementType DDLStType = StatementType::StTypeCount; // invalid value

  // Get first token
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  // Get second token
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid DDL query: second key word is missed");
    exit(EXIT_FAILURE);
  }
  this->ValidateIsWord(this->peek_first_token());
  std::string snd_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOG(ERROR, "body of the DDL statement is missed");
    exit(EXIT_FAILURE);
  }

  // Define next rule
  bool is_CREATE = fst_kw == "CREATE";
  bool is_ALTER = fst_kw == "ALTER";
  bool is_DROP = fst_kw == "DROP";

  bool is_DATABASE = snd_kw == "DATABASE";
  bool is_TABLE = snd_kw == "TABLE";

  // Invoke necessary rule
  if (is_CREATE) {
    if (is_DATABASE) { DDLStType = StatementType::createDatabaseStatement; }
    else if (is_TABLE) { DDLStType = StatementType::createTableStatement; }
    else {
      LOG(ERROR, "unknown CREATE DDL statement");
      exit(EXIT_FAILURE);
    }
  } else if (is_ALTER) {
    if (is_TABLE) { DDLStType = StatementType::alterTableStatement; }
    else {
      LOG(ERROR, "unknown ALTER DDL statement");
      exit(EXIT_FAILURE);
    }
  } else if (is_DROP) {
    if (is_DATABASE) { DDLStType = StatementType::dropDatabaseStatement; }
    else if (is_TABLE) { DDLStType = StatementType::dropTableStatement; }
    else {
      LOG(ERROR, "unknown DROP DDL statement");
      exit(EXIT_FAILURE);
    }
  }

  return DDLStType;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDDLSt() {
  std::shared_ptr<Node> node, statement;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::ddlStatement);

  switch (this->GetDDLStType()) {
    case StatementType::createDatabaseStatement:
      statement = this->GetCreateDatabaseSt();
      break;
    case StatementType::createTableStatement:
      statement = this->GetCreateTableSt();
      break;
    case StatementType::alterTableStatement:
      statement = this->GetAlterTableSt();
      break;
    case StatementType::dropDatabaseStatement:
      statement = this->GetDropDatabaseSt();
      break;
    case StatementType::dropTableStatement:
      statement = this->GetDropTableSt();
      break;
    default:
      LOG(ERROR, "unknown DDL statement");
      exit(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(node, statement);

  return node;
}
StatementType SyntaxAnalyzer::GetDMLStType() {
  StatementType DMLStType = StatementType::StTypeCount; // invalid value

  // Get first token
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  // Define next rule
  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) { DMLStType = StatementType::updateStatement; }
  else if (is_DELETE) { DMLStType = StatementType::deleteStatement; }
  else if (is_INSERT) { DMLStType = StatementType::insertStatement; }
  else {
    LOG(ERROR, "unknown DML statement type");
    exit(EXIT_FAILURE);
  }

  return DMLStType;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDMLSt() {
  std::shared_ptr<Node> node, statement;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::dmlStatement);

  switch (this->GetDMLStType()) {
    case StatementType::updateStatement:
      statement = this->GetUpdateSt();
      break;
    case StatementType::deleteStatement:
      statement = this->GetDeleteSt();
      break;
    case StatementType::insertStatement:
      statement = this->GetInsertSt();
      break;
    default:
      LOG(ERROR, "unknown DML statement");
      exit(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(node, statement);

  return node;
}

// DDL Statements

std::shared_ptr<Node> SyntaxAnalyzer::GetCreateDatabaseSt() {
  std::shared_ptr<Node> node, database_name;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
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
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::createTableStatement);

  // Get tableName
  if (tokens_array_.empty()) {
    LOG(ERROR, "tableName is missed");
    return node;
  } else {
    this->ValidateIsWord(this->peek_first_token());
    table_name = this->GetName();
    SyntaxAnalyzer::MakeKinship(node, table_name);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "opening round bracket is missed");
    return node;
  } else {
    this->ValidateIsOpeningRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "table definition is missed");
    return node;
  } else {
    this->ValidateIsWord(this->peek_first_token());
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
  std::shared_ptr<Node> node, table_name, action_str_node, argument;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::alterTableStatement);

  this->ValidateIsWord(this->peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  // Get action (ADD | DROP)
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid alter table query: action is missed");
    return node;
  }
  this->ValidateIsWord(this->peek_first_token());
  action_str_node = this->get_first_token();

  std::shared_ptr<Node> action =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid alter table query:");
    return node;
  }
  this->ValidateIsWord(this->peek_first_token());
  std::string action_str =
      std::dynamic_pointer_cast<StringNode>(action_str_node)->get_data();
  if (action_str == "ADD") {
    action->set_st_type(StatementType::alterActionADD);
    argument = this->GetTableDefinition();
  } else if (action_str == "DROP") {
    action->set_st_type(StatementType::alterActionDROP);
    argument = this->GetDropListDefinition();
  } else {
    LOG(ERROR, "invalid alter table query: incorrect action \'"
        << action_str << "\'");
    return node;
  }
  SyntaxAnalyzer::MakeKinship(node, action);
  SyntaxAnalyzer::MakeKinship(action, argument);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropDatabaseSt() {
  std::shared_ptr<Node> node, database_name, separator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
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
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetListOf(StatementType::name);
      SyntaxAnalyzer::MakeKinship(node, separator);
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropTableSt() {
  std::shared_ptr<Node> node, table_name, separator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::dropTableStatement);

  // First tableName
  if (tokens_array_.empty()) {
    LOG(ERROR, "table name is missed");
  } else {
    this->ValidateIsWord(peek_first_token());
    table_name = this->GetName();
    SyntaxAnalyzer::MakeKinship(node, table_name);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetListOf(StatementType::name);
      SyntaxAnalyzer::MakeKinship(node, separator);
    }
  }

  return node;
}

// DDL Basic Statements

std::shared_ptr<Node> SyntaxAnalyzer::GetTableDefinition() {
  std::shared_ptr<Node> node, argument, separator, next_arguments;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::tableDefinition);

  argument = this->GetTableDefinitionObject();
  SyntaxAnalyzer::MakeKinship(node, argument);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetListOf(StatementType::tableDefinition);
      SyntaxAnalyzer::MakeKinship(node, separator);
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetTableDefinitionObject() {
  std::shared_ptr<Node> argument;

  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  bool is_tableConstraint =
      key_word == "CONSTRAINT"
          || key_word == "PRIMARY"
          || key_word == "FOREIGN";

  if (is_tableConstraint) {
    argument = this->GetTableConstraint();
  } else {
    argument = this->GetColumnDefinition();
  }

  return argument;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetColumnDefinition() {
  std::shared_ptr<Node> column_def, column_name, datatype;
  column_def =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  column_def->set_st_type(StatementType::columnDefinition);

  // Get columnName
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(column_def, column_name);

  // Get datatype
  if (tokens_array_.empty()) {
    LOG(ERROR, "column datatype is missed");
    return column_def;
  } else {
    this->ValidateIsWord(this->peek_first_token());
    datatype = this->GetDataType();
    SyntaxAnalyzer::MakeKinship(column_def, datatype);
  }

  // Get other options
  if (!tokens_array_.empty()) {
    // TODO: get options such as IDENTITY or (NOT) NULL
  }

  return column_def;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetTableConstraint() {
  std::shared_ptr<Node> table_constraint, constraint_name, key;
  table_constraint =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  table_constraint->set_st_type(StatementType::tableConstraint);

  // Get full form if present
  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  if (key_word == "CONSTRAINT") {
    this->pop_first_token();
    std::shared_ptr<Node> constraint_kw =
        std::dynamic_pointer_cast<ServiceNode>(
            std::make_shared<ServiceNode>());
    constraint_kw->set_st_type(StatementType::kw_constraint);
    SyntaxAnalyzer::MakeKinship(table_constraint, constraint_kw);

    if (tokens_array_.empty()) {
      LOG(ERROR, "constraint name is missed");
      return table_constraint;
    }
    constraint_name = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(constraint_kw, constraint_name);
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "constraint is missed");
    return table_constraint;
  }

  // Get 'PRIMARY' | 'FOREIGN' key word
  this->ValidateIsWord(this->peek_first_token());
  std::string kind_of_key =
      std::dynamic_pointer_cast<StringNode>(
          this->get_first_token())->get_data();

  // Get 'KEY' key word
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid constraint definition: \'KEY\' is missed");
    return table_constraint;
  }
  this->ValidateIsWord(this->peek_first_token());
  std::string KEY_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->get_first_token())->get_data();
  if (KEY_kw != "KEY") {
    LOG(ERROR, "expected \'KEY\', got \'" << KEY_kw << "\'");
    return table_constraint;
  }

  // Get key definition
  if (tokens_array_.empty()) {
    LOG(ERROR, "constraint definition is missed");
    return table_constraint;
  }
  if (kind_of_key == "PRIMARY") {
    key = this->GetPrimaryKey();
  } else if (kind_of_key == "FOREIGN") {
    key = this->GetForeignKey();
  } else {
    LOG(ERROR, "unknown kind of constraint: " << kind_of_key);
    return table_constraint;
  }
  SyntaxAnalyzer::MakeKinship(table_constraint, key);

  return table_constraint;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetDropListDefinition() {
  std::shared_ptr<Node> node, objects, separator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::dropList);

  objects = this->GetDropObject();
  SyntaxAnalyzer::MakeKinship(node, objects);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetDropList();
      SyntaxAnalyzer::MakeKinship(node, separator);
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropList() {
  std::shared_ptr<Node> separator, objects, next_objects;
  this->pop_first_token();
  separator =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  separator->set_st_type(StatementType::delimiter_comma);

  this->ValidateIsWord(this->peek_first_token());
  objects = this->GetDropObject();
  SyntaxAnalyzer::MakeKinship(separator, objects);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      next_objects = this->GetDropList();

      SyntaxAnalyzer::MakeKinship(separator, next_objects);
    }
  }

  return separator;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDropObject() {
  std::shared_ptr<Node> object, argument, next_arguments;
  // Mock
  object = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());

  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  bool is_tableConstraint = key_word == "CONSTRAINT";
  bool is_column = key_word == "COLUMN";

  if (!(is_tableConstraint || is_column)) {
    LOG(ERROR, "invalid drop list: incorrect key word \'" << key_word << "\'");
    return object;
  }

  object = this->get_first_token();
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid drop list: "
        << (is_column ? "constraint" : "column_name") << " is missed");
    return object;
  }

  if (is_tableConstraint) {
    object->set_st_type(StatementType::dropConstraint);
  } else {
    object->set_st_type(StatementType::dropColumn);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid drop list: identifier is missed");
    return object;
  } else {
    argument = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(object, argument);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      // Check key word after the comma
      bool is_list = true;
      if (SyntaxAnalyzer::IsWord(tokens_array_[1])) {
        std::string checking_word =
            std::dynamic_pointer_cast<StringNode>(
                tokens_array_[1])->get_data();
        if (checking_word == "CONSTRAINT" || checking_word == "COLUMN") {
          is_list = false;
        }
      }

      // Get listOf identifiers if present
      if (is_list) {
        next_arguments = this->GetListOf(StatementType::identifier);
        SyntaxAnalyzer::MakeKinship(object, next_arguments);
      }
    }
  }

  return object;
}

// DML Statements

std::shared_ptr<Node> SyntaxAnalyzer::GetInsertSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::insertStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDeleteSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::deleteStatement);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetUpdateSt() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::updateStatement);

  return node;
}

// DML Basic statements

std::shared_ptr<Node> SyntaxAnalyzer::GetCondition() {
  std::shared_ptr<Node> node, OR_condition;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::condition);

  OR_condition = this->GetORCondition();

  SyntaxAnalyzer::MakeKinship(node, OR_condition);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetORCondition() {
  std::shared_ptr<Node> node, AND_condition, next_AND_conditions;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::ORcondition);

  AND_condition = this->GetANDCondition();

  SyntaxAnalyzer::MakeKinship(node, AND_condition);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->get_data() == "OR") {
      this->pop_first_token();

      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid OR-condition: AND-condition "
                   "after the \'OR\' operator is missed");
        return node;
      } else {
        next_AND_conditions = this->GetANDCondition();

        SyntaxAnalyzer::MakeKinship(node, next_AND_conditions);
      }
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetANDCondition() {
  std::shared_ptr<Node> node, NOT_condition, next_NOT_conditions;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::ANDcondition);

  NOT_condition = this->GetNOTCondition();

  SyntaxAnalyzer::MakeKinship(node, NOT_condition);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
      std::shared_ptr<StringNode> tmp =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (tmp->get_data() == "AND") {
        this->pop_first_token();

        if (tokens_array_.empty()) {
          LOG(ERROR, "invalid AND-condition: NOT-condition "
                     "after the \\'AND\\' operator is missed");
          return node;
        } else {
          next_NOT_conditions = this->GetNOTCondition();

          SyntaxAnalyzer::MakeKinship(node, next_NOT_conditions);
        }
      }
    }
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetNOTCondition() {
  std::shared_ptr<Node> node, NOT_operator, predicate;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::NOTcondition);

  // Get NOT if present
  if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<StringNode> tmp =
        std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
    if (tmp->get_data() == "NOT") {
      NOT_operator = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, NOT_operator);
    }
  }

  // Get predicate
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid NOT-condition: predicate is missed");
    return node;
  } else {
    predicate = this->GetPredicate();

    SyntaxAnalyzer::MakeKinship(node, predicate);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPredicate() {
  std::shared_ptr<Node> node, lhs, rhs, bin_operator;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::predicate);

  lhs = this->GetExpression();

  SyntaxAnalyzer::MakeKinship(node, lhs);

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid predicate: binary operator is missed");
    return node;
  } else {
    if (SyntaxAnalyzer::IsBinaryOperator(this->peek_first_token())) {
      bin_operator = this->get_first_token();

      SyntaxAnalyzer::MakeKinship(node, bin_operator);
    } else {
      LOG(ERROR, "invalid predicate: operator should be binary");
      return node;
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid predicate: right hand side expression is missed");
    return node;
  } else {
    rhs = this->GetExpression();

    SyntaxAnalyzer::MakeKinship(node, rhs);
  }

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetExpression() {
  std::shared_ptr<Node> node;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::expression);

  // Is it [table_name.] column ?
  if (SyntaxAnalyzer::IsWord(this->peek_first_token())) {
    std::shared_ptr<Node> name, dot;

    name = this->GetIdentifier();

    SyntaxAnalyzer::MakeKinship(node, name);

    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
        dot = this->GetIdentifiers();

        SyntaxAnalyzer::MakeKinship(node, dot);
      }
    }

    return node;
  }

  // Is it unary operator ?
  if (SyntaxAnalyzer::IsUnaryOperator(this->peek_first_token())) {
    std::shared_ptr<Node> u_operator, expression;
    u_operator = this->get_first_token();

    SyntaxAnalyzer::MakeKinship(node, u_operator);

    if (tokens_array_.empty()) {
      LOG(ERROR,
          "invalid expression: an unary operator without an operand");
      return node;
    } else {
      expression = this->GetExpression();

      SyntaxAnalyzer::MakeKinship(node, expression);
    }

    return node;
  }

  // Is it (expression) ?
  if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression: bad bracket sequence \'(.\'");
      return node;
    } else {
      node = this->GetExpression();
    }

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression: bad bracket sequence \'(.\'");
      return node;
    } else {
      this->pop_first_token();
    }

    return node;
  }

  // Is it string ?
  if (SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
      || SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
    this->pop_first_token();
    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid expression: invalid string");
      return node;
    } else {
      std::shared_ptr<Node> str = this->GetString();

      SyntaxAnalyzer::MakeKinship(node, str);
    }
  }

  // So, it is Math expression
  std::shared_ptr<Node> expression = this->GetMathExpression();

  SyntaxAnalyzer::MakeKinship(node, expression);

  return node;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetMathExpression() {
  std::shared_ptr<Node> node;

  // TODO: implement this PEG
  /* MathExpr   <-- Sum
   * Sum        <-- Product (('+' / '-') Product)*
   * Product    <-- Power (('*' / '/') Power)*
   * Power      <-- Value ('^' Power)?
   * Value      <-- [0-9]+ / '(' MathExpr ')'                   */

  node = this->GetMathSum();

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathSum() {
  std::shared_ptr<Node> product_1, op_node, product_2;

  product_1 = this->GetMathProduct();

  if (!tokens_array_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("+" | "-")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->get_data();
      if (operator_str != "+" || operator_str != "-") {
        LOG(ERROR, "invalid Math expression: wrong operator \'"
            << operator_str << "\'");
        return product_1;
      }

      // Get next powers
      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid Math expression: second operand for the \'"
            << operator_str << "\' is missed");
        return product_1;
      } else {
        product_2 = this->GetMathProduct();
      }

      SyntaxAnalyzer::MakeKinship(op_node, product_1);
      SyntaxAnalyzer::MakeKinship(op_node, product_2);
      product_1 = op_node;

      if (tokens_array_.empty()) {
        break;
      }
    }
  }

  return product_1;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathProduct() {
  std::shared_ptr<Node> power_1, op_node, power_2;

  power_1 = this->GetMathPower();

  if (!tokens_array_.empty()) {
    while (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      // Get ("*" | "/")
      op_node = this->get_first_token();
      std::string operator_str =
          std::dynamic_pointer_cast<StringNode>(op_node)->get_data();
      if (operator_str != "*" || operator_str != "/") {
        LOG(ERROR, "invalid Math expression: wrong operator \'"
            << operator_str << "\'");
        return power_1;
      }

      // Get next powers
      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid Math expression: second operand for the \'"
            << operator_str << "\' is missed");
        return power_1;
      } else {
        power_2 = this->GetMathPower();
      }

      SyntaxAnalyzer::MakeKinship(op_node, power_1);
      SyntaxAnalyzer::MakeKinship(op_node, power_2);
      power_1 = op_node;

      if (tokens_array_.empty()) {
        break;
      }
    }
  }

  return power_1;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathPower() {
  std::shared_ptr<Node> power, degree_op, degree;

  power = this->GetMathValue();

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsOperator(this->peek_first_token())) {
      std::shared_ptr<StringNode> op_node =
          std::dynamic_pointer_cast<StringNode>(this->peek_first_token());
      if (op_node->get_data() == "^") {
        degree_op = this->get_first_token();

        if (tokens_array_.empty()) {
          LOG(ERROR, "invalid Math expression: degree is missed");
          return power;
        }
        degree = this->GetMathPower();
        SyntaxAnalyzer::MakeKinship(degree_op, power);
        SyntaxAnalyzer::MakeKinship(degree_op, degree);

        return degree_op;
      }
    }
  }

  return power;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetMathValue() {
  std::shared_ptr<Node> value;

  if (SyntaxAnalyzer::IsNumber(this->peek_first_token())) {
    value = this->get_first_token();
  } else if (SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    this->pop_first_token();

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid Math expression: closing round bracket is missed");
      return value;
    } else {
      value = this->GetMathExpression();
    }

    if (SyntaxAnalyzer::IsClosingRoundBracket(this->peek_first_token())) {
      this->pop_first_token();
    } else {
      LOG(ERROR, "invalid Math expression: closing round bracket is missed");
      return value;
    }
  }

  return value;
}

// Basic statements

std::shared_ptr<Node> SyntaxAnalyzer::GetDataType() {
  std::shared_ptr<Node> node = this->get_first_token();

  std::string datatype =
      std::dynamic_pointer_cast<StringNode>(node)->get_data();
  StatementType SQL_datatype;
  if (datatype == "int" || datatype == "integer") {
    SQL_datatype = StatementType::SQL_int;
  } else if (datatype == "float") {
    SQL_datatype = StatementType::SQL_float;
  } else if (datatype == "char") {
    SQL_datatype = StatementType::SQL_char;
  } else if (datatype == "varchar") {
    SQL_datatype = StatementType::SQL_varchar;
  } else {
    LOG(ERROR, "invalid column datatype");
    return node;
  }
  node->set_st_type(SQL_datatype);

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetPrimaryKey() {
  std::shared_ptr<Node> primary_key, column_name, separator;
  primary_key =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  primary_key->set_st_type(StatementType::primaryKey);

  this->ValidateIsOpeningRoundBracket(this->peek_first_token());
  this->pop_first_token();

  // Get PRIMARY KEY definition
  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return primary_key;
  } else {
    column_name = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(primary_key, column_name);

    // Get listOf(column_names)
    if (!tokens_array_.empty()) {
      if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
        separator = this->GetListOf(StatementType::identifier);
        SyntaxAnalyzer::MakeKinship(primary_key, separator);
      }
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return primary_key;
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  return primary_key;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetForeignKey() {
  std::shared_ptr<Node> foreign_key, column_name, separator, reference;
  foreign_key =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  foreign_key->set_st_type(StatementType::foreignKey);

  this->ValidateIsOpeningRoundBracket(this->peek_first_token());
  this->pop_first_token();

  // Get FOREIGN KEY definition
  if (tokens_array_.empty()) {
    LOG(ERROR, "columnName is missed");
    return foreign_key;
  } else {
    column_name = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(foreign_key, column_name);

    // Get listOf(column_names)
    if (!tokens_array_.empty()
        && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetListOf(StatementType::identifier);
      SyntaxAnalyzer::MakeKinship(foreign_key, separator);
    }
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "closing round bracket is missed");
    return foreign_key;
  } else {
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  // Get Reference
  if (tokens_array_.empty()) {
    LOG(ERROR, "reference is missed");
    return foreign_key;
  } else {
    this->ValidateIsWord(this->peek_first_token());
    reference = this->GetReference();
    SyntaxAnalyzer::MakeKinship(foreign_key, reference);
  }

  return foreign_key;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetReference() {
  std::shared_ptr<Node> reference, ref_table_name;

  // Get REFERENCES
  std::string ref_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  if (ref_kw != "REFERENCES") {
    LOG(ERROR, "incorrect reference: " << ref_kw);
    return reference;
  }
  this->pop_first_token();
  reference = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
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
  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    std::shared_ptr<Node> ref_column_name, next_ref_column_names;

    this->ValidateIsOpeningRoundBracket(this->peek_first_token());
    this->pop_first_token();

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid reference: incorrect list of column names");
      return reference;
    } else {
      ref_column_name = this->GetIdentifier();
      SyntaxAnalyzer::MakeKinship(ref_table_name, ref_column_name);

      if (tokens_array_.empty()) {
        LOG(ERROR, "invalid reference: closing round bracket is missed");
        return reference;
      } else {
        if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
          next_ref_column_names =
              this->GetListOf(StatementType::identifier);
          SyntaxAnalyzer::MakeKinship(ref_table_name, next_ref_column_names);
        }
      }
    }

    if (tokens_array_.empty()) {
      LOG(ERROR, "invalid reference: closing round bracket is missed");
      return reference;
    } else {
      this->ValidateIsClosingRoundBracket(this->peek_first_token());
      this->pop_first_token();
    }
  }

  return reference;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetString() {
  std::shared_ptr<Node> node;

  bool is_single_quote =
      SyntaxAnalyzer::IsSingleQuote(this->peek_first_token());
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid expression: invalid string");
    return node;
  }
  std::shared_ptr<StringNode> str =
      std::make_shared<StringNode>("", DataType::STRING);
  while (!SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
      || !SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
    std::shared_ptr<Node> tmp = this->get_first_token();
    DataType tmp_type = tmp->get_type();
    std::string new_data;
    if (!str->get_data().empty()) {
      new_data = str->get_data() + " ";
    }
    switch (tmp_type) {
      case DataType::INT_NUMBER:
        new_data += std::to_string(
            std::dynamic_pointer_cast<IntNumNode>(tmp)->get_data());
        break;
      case DataType::FLOAT_NUMBER:
        new_data += std::to_string(
            std::dynamic_pointer_cast<FloatNumNode>(tmp)->get_data());
        break;
      case DataType::WORD:
      case DataType::OPERATOR:
        new_data += std::dynamic_pointer_cast<StringNode>(tmp)->get_data();
        break;
      case DataType::BRACKET:
      case DataType::PUNCTUATION:
        new_data += std::to_string(
            std::dynamic_pointer_cast<CharNode>(tmp)->get_data());
        break;
      default:LOG(ERROR,
                  "invalid string: "
                  "incorrect type of token inside the string");
        return node;
    }
    str->set_data(new_data);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid string: closing quote is missed");
    return node;
  } else {
    if (is_single_quote) {
      this->ValidateIsSingleQuote(this->peek_first_token());
    } else {
      this->ValidateIsDoubleQuote(this->peek_first_token());
    }
    this->pop_first_token();
  }

  node = std::dynamic_pointer_cast<Node>(str);

  return node;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetName() {
  std::shared_ptr<Node> name, identifier, next_identifiers;
  name = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  name->set_st_type(StatementType::name);

  identifier = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(name, identifier);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
      next_identifiers = this->GetIdentifiers();
      SyntaxAnalyzer::MakeKinship(name, next_identifiers);
    }
  }


  return name;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifiers() {
  std::shared_ptr<Node> dot, identifier, next_identifiers;
  this->pop_first_token();
  dot = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  dot->set_st_type(StatementType::delimiter_dot);

  if (tokens_array_.empty()) {
    LOG(ERROR, "Bad name, which ends in dot");
    return nullptr;
  } else {
    identifier = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(dot, identifier);
  }

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
      next_identifiers = this->GetIdentifiers();
      SyntaxAnalyzer::MakeKinship(dot, next_identifiers);
    }
  }

  return dot;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetIdentifier() {
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<Node> identifier;
  identifier =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  identifier->set_st_type(StatementType::identifier);

  std::shared_ptr<Node> argument = this->get_first_token();
  SyntaxAnalyzer::MakeKinship(identifier, argument);

  return identifier;
}

std::shared_ptr<Node> SyntaxAnalyzer::GetListOf(
    StatementType get_function_type) {
  std::shared_ptr<Node> argument, separator, next_separator;

  // Get separator (comma)
  this->pop_first_token();
  separator = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  separator->set_st_type(StatementType::delimiter_comma);

  if (tokens_array_.empty()) {
    LOG(ERROR, "argument is missed");
    return separator;
  }
  switch (get_function_type) {
    case StatementType::identifier:
      argument = this->GetIdentifier();
      break;
    case StatementType::tableDefinition:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetTableDefinitionObject();
      break;
    case StatementType::columnDefinition:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetColumnDefinition();
      break;
    case StatementType::tableConstraint:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetTableConstraint();
      break;
    case StatementType::name:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetName();
      break;
    default:
      LOG(ERROR, "unknown statement type for the listOf");
      exit(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(separator, argument);

  if (!tokens_array_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      next_separator = this->GetListOf(get_function_type);
      SyntaxAnalyzer::MakeKinship(separator, next_separator);
    }
  }

  return separator;
}

// Validation

void SyntaxAnalyzer::ValidateNotEmpty() const {
  if (tokens_array_.empty()) {
    LOG(ERROR, "token's array are empty");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token's array are not empty");
}
void SyntaxAnalyzer::ValidateIsWord(std::shared_ptr<Node> &node) const {
  if (node->get_type() != DataType::WORD) {
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
void SyntaxAnalyzer::ValidateIsSingleQuote(std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOG(ERROR, "token is not a single quote");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a single quote");
}
void SyntaxAnalyzer::ValidateIsDoubleQuote(std::shared_ptr<Node> &node) const {
  if (!SyntaxAnalyzer::IsSingleQuote(node)) {
    LOG(ERROR, "token is not a double quote");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "token is a double quote");
}

// Defining Node Datatype

bool SyntaxAnalyzer::IsBracket(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::BRACKET) {
    LOG(DEBUG, "token is not a bracket: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a bracket");
    return true;
  }
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
bool SyntaxAnalyzer::IsWord(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::WORD) {
    LOG(DEBUG, "token is not a word: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a word");
    return true;
  }
}
bool SyntaxAnalyzer::IsNumber(std::shared_ptr<Node> &node) {
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
bool SyntaxAnalyzer::IsOperator(std::shared_ptr<Node> &node) {
  if (node->get_type() != DataType::OPERATOR) {
    LOG(DEBUG, "token is not a operator: " << node->get_type());
    return false;
  } else {
    LOG(DEBUG, "token is a operator");
    return true;
  }
}

// Defining Node data

bool SyntaxAnalyzer::IsDot(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> dot =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>('.', DataType::PUNCTUATION));
  return Node::IsNodesEqual(dot, node);
}
bool SyntaxAnalyzer::IsComma(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> comma =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(',', DataType::PUNCTUATION));
  return Node::IsNodesEqual(comma, node);
}
bool SyntaxAnalyzer::IsOpeningRoundBracket(std::shared_ptr<Node> &node) {
  // Opening Round Bracket
  std::shared_ptr<Node> ORB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              '(', DataType::BRACKET));
  return Node::IsNodesEqual(ORB, node);
}
bool SyntaxAnalyzer::IsClosingRoundBracket(std::shared_ptr<Node> &node) {
  // Closing Round Bracket
  std::shared_ptr<Node> CRB =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(
              ')', DataType::BRACKET));
  return Node::IsNodesEqual(CRB, node);
}
bool SyntaxAnalyzer::IsSingleQuote(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> single_quote =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<StringNode>("\'", DataType::PUNCTUATION));
  return Node::IsNodesEqual(single_quote, node);
}
bool SyntaxAnalyzer::IsDoubleQuote(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> double_quote =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<StringNode>("\"", DataType::PUNCTUATION));
  return Node::IsNodesEqual(double_quote, node);
}
bool SyntaxAnalyzer::IsUnaryOperator(std::shared_ptr<Node> &node) {
  if (node->get_type() == DataType::OPERATOR) {
    std::string data = std::dynamic_pointer_cast<StringNode>(node)->get_data();
    if (data == "+" || data == "-") {
      return true;
    }
  }
  return false;
}
bool SyntaxAnalyzer::IsBinaryOperator(std::shared_ptr<Node> &node) {
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
bool SyntaxAnalyzer::IsSemicolon(std::shared_ptr<Node> &node) {
  std::shared_ptr<Node> semicolon =
      std::dynamic_pointer_cast<Node>(
          std::make_shared<CharNode>(';', DataType::PUNCTUATION));
  return Node::IsNodesEqual(semicolon, node);
}

void SyntaxAnalyzer::MakeKinship(std::shared_ptr<Node> &parent,
                                 std::shared_ptr<Node> &child) {
  parent->AddChild(child);
  child->set_parent(parent);
}

// Work with deque of tokens

std::shared_ptr<Node> &SyntaxAnalyzer::peek_first_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<Node> &>(tokens_array_.front());
}
std::shared_ptr<Node> &SyntaxAnalyzer::peek_last_token() const {
  this->ValidateNotEmpty();
  return const_cast<std::shared_ptr<Node> &>(tokens_array_.back());
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
