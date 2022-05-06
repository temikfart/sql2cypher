#include "SCC/syntax_analyzer.h"

StatementType SyntaxAnalyzer::GetDDLStType() {
  StatementType DDLStType = StatementType::StTypeCount; // invalid value

  // Get first token
  int line = this->peek_first_token()->get_line();
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  // Get second token
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid DDL query in line "
        << line << ": second key word is missed");
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  std::string snd_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOG(ERROR, "body of the DDL statement is missed in line " << line);
    end(EXIT_FAILURE);
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
      LOG(ERROR, "unknown CREATE DDL statement in line " << line);
      end(EXIT_FAILURE);
    }
  } else if (is_ALTER) {
    if (is_TABLE) { DDLStType = StatementType::alterTableStatement; }
    else {
      LOG(ERROR, "unknown ALTER DDL statement in line " << line);
      end(EXIT_FAILURE);
    }
  } else if (is_DROP) {
    if (is_DATABASE) { DDLStType = StatementType::dropDatabaseStatement; }
    else if (is_TABLE) { DDLStType = StatementType::dropTableStatement; }
    else {
      LOG(ERROR, "unknown DROP DDL statement in line " << line);
      end(EXIT_FAILURE);
    }
  }

  return DDLStType;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetDDLSt() {
  LOG(TRACE, "getting DDL statement...");
  std::shared_ptr<Node> node, statement;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::ddlStatement);

  int line = this->peek_first_token()->get_line();
  switch (this->GetDDLStType()) {
    case StatementType::createDatabaseStatement:
      statement = this->GetCreateDatabaseSt();
      LOG(TRACE, "got CREATE DATABASE statement");
      break;
    case StatementType::createTableStatement:
      statement = this->GetCreateTableSt();
      LOG(TRACE, "got CREATE TABLE statement");
      break;
    case StatementType::alterTableStatement:
      statement = this->GetAlterTableSt();
      LOG(TRACE, "got ALTER TABLE statement");
      break;
    case StatementType::dropDatabaseStatement:
      statement = this->GetDropDatabaseSt();
      LOG(TRACE, "got DROP DATABASE statement");
      break;
    case StatementType::dropTableStatement:
      statement = this->GetDropTableSt();
      LOG(TRACE, "got DROP TABLE statement");
      break;
    default:
      LOG(ERROR, "unknown DDL statement near line " << line);
      end(EXIT_FAILURE);
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
    LOG(ERROR, "database name is missed");
  }
  this->ValidateIsWord(this->peek_first_token());
  database_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, database_name);

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
  }
  int line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  if (tokens_array_.empty()) {
    LOG(ERROR, "expected opening round bracket in line " << line);
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
  this->ValidateIsOpeningRoundBracket(this->peek_first_token());
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOG(ERROR, "expected table definition in line " << line);
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  table_definition = this->GetTableDefinition();
  SyntaxAnalyzer::MakeKinship(node, table_definition);

  if (tokens_array_.empty()) {
    LOG(ERROR, "expected closing round bracket in line " << line);
  }
  this->ValidateIsClosingRoundBracket(this->peek_first_token());
  this->pop_first_token();

  return node;
}
std::shared_ptr<Node> SyntaxAnalyzer::GetAlterTableSt() {
  std::shared_ptr<Node> node, table_name, action_str_node, argument;
  node = std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());
  node->set_st_type(StatementType::alterTableStatement);

  int line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  // Get action (ADD | DROP)
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid alter table query in line "
        << line << ": expected action");
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  action_str_node = this->get_first_token();

  std::shared_ptr<Node> action =
      std::dynamic_pointer_cast<Node>(std::make_shared<ServiceNode>());

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid alter table query: "
               "expected action's body in line " << line);
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
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
        << action_str << "\' in line " << line);
    end(EXIT_FAILURE);
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
  }
  this->ValidateIsWord(this->peek_first_token());
  database_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, database_name);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StatementType::name);
    SyntaxAnalyzer::MakeKinship(node, separator);
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
  }
  this->ValidateIsWord(peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StatementType::name);
    SyntaxAnalyzer::MakeKinship(node, separator);
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

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StatementType::tableDefinition);
    SyntaxAnalyzer::MakeKinship(node, separator);
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
  int line = this->peek_first_token()->get_line();
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(column_def, column_name);

  // Get datatype
  if (tokens_array_.empty()) {
    LOG(ERROR, "expected column datatype in line " << line);
    end(EXIT_FAILURE);
  }
  this->ValidateIsWord(this->peek_first_token());
  datatype = this->GetDataType();
  SyntaxAnalyzer::MakeKinship(column_def, datatype);

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
  int line = this->peek_first_token()->get_line();
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
      LOG(ERROR, "expected constraint name in line " << line);
      end(EXIT_FAILURE);
    }
    constraint_name = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(constraint_kw, constraint_name);
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_array_.empty()) {
    LOG(ERROR, "expected constraint in line " << line);
    end(EXIT_FAILURE);
  }

  // Get 'PRIMARY' | 'FOREIGN' key word
  line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  std::string kind_of_key =
      std::dynamic_pointer_cast<StringNode>(
          this->get_first_token())->get_data();

  // Get 'KEY' key word
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid constraint definition: "
               "expected \'KEY\' in line " << line);
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
  this->ValidateIsWord(this->peek_first_token());
  std::string KEY_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->get_first_token())->get_data();
  if (KEY_kw != "KEY") {
    LOG(ERROR, "expected \'KEY\', got \'"
        << KEY_kw << "\' in line " << line);
    end(EXIT_FAILURE);
  }

  // Get key definition
  if (tokens_array_.empty()) {
    LOG(ERROR, "expected constraint definition in line " << line);
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->get_line();
  if (kind_of_key == "PRIMARY") {
    key = this->GetPrimaryKey();
  } else if (kind_of_key == "FOREIGN") {
    key = this->GetForeignKey();
  } else {
    LOG(ERROR, "unknown kind of constraint in line "
        << line << ": " << kind_of_key);
    end(EXIT_FAILURE);
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

  int line = this->peek_first_token()->get_line();
  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  bool is_tableConstraint = key_word == "CONSTRAINT";
  bool is_column = key_word == "COLUMN";

  if (!(is_tableConstraint || is_column)) {
    LOG(ERROR, "invalid drop list: incorrect key word \'"
        << key_word << "\' in line " << line);
    end(EXIT_FAILURE);
  }

  line = this->peek_first_token()->get_line();
  object = this->get_first_token();
  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid drop list: expected "
        << (is_column ? "constraint" : "column_name") << " in line " << line);
    end(EXIT_FAILURE);
  }

  if (is_tableConstraint) {
    object->set_st_type(StatementType::dropConstraint);
  } else {
    object->set_st_type(StatementType::dropColumn);
  }

  if (tokens_array_.empty()) {
    LOG(ERROR, "invalid drop list: expected identifier in line " << line);
    end(EXIT_FAILURE);
  }
  argument = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(object, argument);

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
