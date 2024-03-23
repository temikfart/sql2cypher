#include "SCC/syntax_analyzer.h"

StmtType SyntaxAnalyzer::GetDDLStType() {
  StmtType DDLStType = StmtType::EMPTY_TYPE; // invalid value

  // Get first token
  int line = this->peek_first_token()->line;
  std::string fst_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  // Get second token
  if (tokens_array_.empty()) {
    LOGE << "invalid DDL query in line "
        << line << ": second key word is missed";
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  std::string snd_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOGE << "body of the DDL statement is missed in line " << line;
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
    if (is_DATABASE) { DDLStType = StmtType::createDatabaseStatement; }
    else if (is_TABLE) { DDLStType = StmtType::createTableStatement; }
    else {
      LOGE << "unknown CREATE DDL statement in line " << line;
      end(EXIT_FAILURE);
    }
  } else if (is_ALTER) {
    if (is_TABLE) { DDLStType = StmtType::alterTableStatement; }
    else {
      LOGE << "unknown ALTER DDL statement in line " << line;
      end(EXIT_FAILURE);
    }
  } else if (is_DROP) {
    if (is_DATABASE) { DDLStType = StmtType::dropDatabaseStatement; }
    else if (is_TABLE) { DDLStType = StmtType::dropTableStatement; }
    else {
      LOGE << "unknown DROP DDL statement in line " << line;
      end(EXIT_FAILURE);
    }
  }

  return DDLStType;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetDDLSt() {
  LOGD << "getting DDL statement...";
  std::shared_ptr<INode> node, statement;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::ddlStatement;

  int line = this->peek_first_token()->line;
  switch (this->GetDDLStType()) {
    case StmtType::createDatabaseStatement:
      statement = this->GetCreateDatabaseSt();
      LOGD << "got CREATE DATABASE statement";
      break;
    case StmtType::createTableStatement:
      statement = this->GetCreateTableSt();
      LOGD << "got CREATE TABLE statement";
      break;
    case StmtType::alterTableStatement:
      statement = this->GetAlterTableSt();
      LOGD << "got ALTER TABLE statement";
      break;
    case StmtType::dropDatabaseStatement:
      statement = this->GetDropDatabaseSt();
      LOGD << "got DROP DATABASE statement";
      break;
    case StmtType::dropTableStatement:
      statement = this->GetDropTableSt();
      LOGD << "got DROP TABLE statement";
      break;
    default:
      LOGE << "unknown DDL statement near line " << line;
      end(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(node, statement);

  return node;
}

// DDL Statements

std::shared_ptr<INode> SyntaxAnalyzer::GetCreateDatabaseSt() {
  std::shared_ptr<INode> node, database_name;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::createDatabaseStatement;

  if (tokens_array_.empty()) {
    LOGE << "database name is missed";
  }
  this->ValidateIsWord(this->peek_first_token());
  database_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, database_name);

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetCreateTableSt() {
  std::shared_ptr<INode> node, table_name, table_definition;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::createTableStatement;

  // Get tableName
  if (tokens_array_.empty()) {
    LOGE << "tableName is missed";
    return node;
  }
  int line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  if (tokens_array_.empty()) {
    LOGE << "expected opening round bracket in line " << line;
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  this->ValidateIsOpeningRoundBracket(this->peek_first_token());
  this->pop_first_token();

  if (tokens_array_.empty()) {
    LOGE << "expected table definition in line " << line;
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  table_definition = this->GetTableDefinition();
  SyntaxAnalyzer::MakeKinship(node, table_definition);

  if (tokens_array_.empty()) {
    LOGE << "expected closing round bracket in line " << line;
  }
  this->ValidateIsClosingRoundBracket(this->peek_first_token());
  this->pop_first_token();

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetAlterTableSt() {
  std::shared_ptr<INode> node, table_name, action_str_node, argument;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::alterTableStatement;

  int line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  // Get action (ADD | DROP)
  if (tokens_array_.empty()) {
    LOGE << "invalid alter table query in line "
        << line << ": expected action";
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  action_str_node = this->get_first_token();

  std::shared_ptr<INode> action =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());

  if (tokens_array_.empty()) {
    LOGE << "invalid alter table query: "
               "expected action's body in line " << line;
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  std::string action_str =
      std::dynamic_pointer_cast<StringNode>(action_str_node)->get_data();
  if (action_str == "ADD") {
    action->stmt_type = StmtType::alterActionADD;
    argument = this->GetTableDefinition();
  } else if (action_str == "DROP") {
    action->stmt_type = StmtType::alterActionDROP;
    argument = this->GetDropListDefinition();
  } else {
    LOGE << "invalid alter table query: incorrect action \'"
        << action_str << "\' in line " << line;
    end(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(node, action);
  SyntaxAnalyzer::MakeKinship(action, argument);

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetDropDatabaseSt() {
  std::shared_ptr<INode> node, database_name, separator;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::dropDatabaseStatement;

  // First database_name
  if (tokens_array_.empty()) {
    LOGE << "databaseName is missed";
  }
  this->ValidateIsWord(this->peek_first_token());
  database_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, database_name);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StmtType::name);
    SyntaxAnalyzer::MakeKinship(node, separator);
  }

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetDropTableSt() {
  std::shared_ptr<INode> node, table_name, separator;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::dropTableStatement;

  // First tableName
  if (tokens_array_.empty()) {
    LOGE << "table name is missed";
  }
  this->ValidateIsWord(peek_first_token());
  table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(node, table_name);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StmtType::name);
    SyntaxAnalyzer::MakeKinship(node, separator);
  }

  return node;
}

// DDL Basic Statements

std::shared_ptr<INode> SyntaxAnalyzer::GetTableDefinition() {
  std::shared_ptr<INode> node, argument, separator, next_arguments;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::tableDefinition;

  argument = this->GetTableDefinitionObject();
  SyntaxAnalyzer::MakeKinship(node, argument);

  if (!tokens_array_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StmtType::tableDefinition);
    SyntaxAnalyzer::MakeKinship(node, separator);
  }

  return node;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetTableDefinitionObject() {
  std::shared_ptr<INode> argument;

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

std::shared_ptr<INode> SyntaxAnalyzer::GetColumnDefinition() {
  std::shared_ptr<INode> column_def, column_name, datatype;
  column_def =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  column_def->stmt_type = StmtType::columnDefinition;

  // Get columnName
  int line = this->peek_first_token()->line;
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(column_def, column_name);

  // Get datatype
  if (tokens_array_.empty()) {
    LOGE << "expected column datatype in line " << line;
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
std::shared_ptr<INode> SyntaxAnalyzer::GetTableConstraint() {
  std::shared_ptr<INode> table_constraint, constraint_name, key;
  table_constraint =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  table_constraint->stmt_type = StmtType::tableConstraint;

  // Get full form if present
  int line = this->peek_first_token()->line;
  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  if (key_word == "CONSTRAINT") {
    this->pop_first_token();
    std::shared_ptr<INode> constraint_kw =
        std::dynamic_pointer_cast<ServiceNode>(
            std::make_shared<ServiceNode>());
    constraint_kw->stmt_type = StmtType::kw_constraint;
    SyntaxAnalyzer::MakeKinship(table_constraint, constraint_kw);

    if (tokens_array_.empty()) {
      LOGE << "expected constraint name in line " << line;
      end(EXIT_FAILURE);
    }
    constraint_name = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(constraint_kw, constraint_name);
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_array_.empty()) {
    LOGE << "expected constraint in line " << line;
    end(EXIT_FAILURE);
  }

  // Get 'PRIMARY' | 'FOREIGN' key word
  line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  std::string kind_of_key =
      std::dynamic_pointer_cast<StringNode>(
          this->get_first_token())->get_data();

  // Get 'KEY' key word
  if (tokens_array_.empty()) {
    LOGE << "invalid constraint definition: "
               "expected \'KEY\' in line " << line;
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  this->ValidateIsWord(this->peek_first_token());
  std::string KEY_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->get_first_token())->get_data();
  if (KEY_kw != "KEY") {
    LOGE << "expected \'KEY\', got \'"
        << KEY_kw << "\' in line " << line;
    end(EXIT_FAILURE);
  }

  // Get key definition
  if (tokens_array_.empty()) {
    LOGE << "expected constraint definition in line " << line;
    end(EXIT_FAILURE);
  }
  line = this->peek_first_token()->line;
  if (kind_of_key == "PRIMARY") {
    key = this->GetPrimaryKey();
  } else if (kind_of_key == "FOREIGN") {
    key = this->GetForeignKey();
  } else {
    LOGE << "unknown kind of constraint in line "
        << line << ": " << kind_of_key;
    end(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(table_constraint, key);

  return table_constraint;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetDropListDefinition() {
  std::shared_ptr<INode> node, objects, separator;
  node = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  node->stmt_type = StmtType::dropList;

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
std::shared_ptr<INode> SyntaxAnalyzer::GetDropList() {
  std::shared_ptr<INode> separator, objects, next_objects;
  this->pop_first_token();
  separator =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  separator->stmt_type = StmtType::delimiter_comma;

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
std::shared_ptr<INode> SyntaxAnalyzer::GetDropObject() {
  std::shared_ptr<INode> object, argument, next_arguments;
  // Mock
  object = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());

  int line = this->peek_first_token()->line;
  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->get_data();
  bool is_tableConstraint = key_word == "CONSTRAINT";
  bool is_column = key_word == "COLUMN";

  if (!(is_tableConstraint || is_column)) {
    LOGE << "invalid drop list: incorrect key word \'"
        << key_word << "\' in line " << line;
    end(EXIT_FAILURE);
  }

  line = this->peek_first_token()->line;
  object = this->get_first_token();
  if (tokens_array_.empty()) {
    LOGE << "invalid drop list: expected "
        << (is_column ? "constraint" : "column_name") << " in line " << line;
    end(EXIT_FAILURE);
  }

  if (is_tableConstraint) {
    object->stmt_type = StmtType::dropConstraint;
  } else {
    object->stmt_type = StmtType::dropColumn;
  }

  if (tokens_array_.empty()) {
    LOGE << "invalid drop list: expected identifier in line " << line;
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
        next_arguments = this->GetListOf(StmtType::identifier);
        SyntaxAnalyzer::MakeKinship(object, next_arguments);
      }
    }
  }

  return object;
}
