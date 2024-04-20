#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType Parser::GetDDLStType() {
  StmtType DDLStType = StmtType::kNone; // invalid value

  // Get first token
  int line = peek_first_token()->line;
  std::string fst_kw = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;
  pop_first_token();

  // Get second token
  if (tokens_.empty()) {
    LOGE << "invalid DDL query in line "
        << line << ": second key word is missed";
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  std::string snd_kw = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;
  pop_first_token();

  if (tokens_.empty()) {
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
    if (is_DATABASE) { DDLStType = StmtType::kCreateDatabaseStmt; }
    else if (is_TABLE) { DDLStType = StmtType::kCreateTableStmt; }
    else {
      LOGE << "unknown CREATE DDL statement in line " << line;
      end(EXIT_FAILURE);
    }
  } else if (is_ALTER) {
    if (is_TABLE) { DDLStType = StmtType::kAlterTableStmt; }
    else {
      LOGE << "unknown ALTER DDL statement in line " << line;
      end(EXIT_FAILURE);
    }
  } else if (is_DROP) {
    if (is_DATABASE) { DDLStType = StmtType::kDropDatabaseStmt; }
    else if (is_TABLE) { DDLStType = StmtType::kDropTableStmt; }
    else {
      LOGE << "unknown DROP DDL statement in line " << line;
      end(EXIT_FAILURE);
    }
  }

  return DDLStType;
}
NodePtr<INode> Parser::GetDDLSt() {
  LOGD << "getting DDL statement...";
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kDdlStmt);

  int line = peek_first_token()->line;
  NodePtr<INode> statement;
  switch (GetDDLStType()) {
    case StmtType::kCreateDatabaseStmt:
      statement = GetCreateDatabaseSt();
      LOGD << "got CREATE DATABASE statement";
      break;
    case StmtType::kCreateTableStmt:
      statement = GetCreateTableSt();
      LOGD << "got CREATE TABLE statement";
      break;
    case StmtType::kAlterTableStmt:
      statement = GetAlterTableSt();
      LOGD << "got ALTER TABLE statement";
      break;
    case StmtType::kDropDatabaseStmt:
      statement = GetDropDatabaseSt();
      LOGD << "got DROP DATABASE statement";
      break;
    case StmtType::kDropTableStmt:
      statement = GetDropTableSt();
      LOGD << "got DROP TABLE statement";
      break;
    default:
      LOGE << "unknown DDL statement near line " << line;
      end(EXIT_FAILURE);
  }
  INode::Link(node, statement);

  return node;
}

// DDL Statements

NodePtr<INode> Parser::GetCreateDatabaseSt() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kCreateDatabaseStmt);

  if (tokens_.empty()) {
    LOGE << "database name is missed";
  }
  ValidateIsWord(peek_first_token());
  NodePtr<INode> database_name = GetName();
  INode::Link(node, database_name);

  return node;
}
NodePtr<INode> Parser::GetCreateTableSt() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kCreateTableStmt);

  // Get tableName
  if (tokens_.empty()) {
    LOGE << "tableName is missed";
    return node;
  }
  int line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  NodePtr<INode> table_name = GetName();
  INode::Link(node, table_name);

  if (tokens_.empty()) {
    LOGE << "expected opening round bracket in line " << line;
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  ValidateIsOpeningRoundBracket(peek_first_token());
  pop_first_token();

  if (tokens_.empty()) {
    LOGE << "expected table definition in line " << line;
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  NodePtr<INode> table_definition = GetTableDefinition();
  INode::Link(node, table_definition);

  if (tokens_.empty()) {
    LOGE << "expected closing round bracket in line " << line;
  }
  ValidateIsClosingRoundBracket(peek_first_token());
  pop_first_token();

  return node;
}
NodePtr<INode> Parser::GetAlterTableSt() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kAlterTableStmt);

  int line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  NodePtr<INode> table_name = GetName();
  INode::Link(node, table_name);

  // Get action (ADD | DROP)
  if (tokens_.empty()) {
    LOGE << "invalid alter table query in line "
        << line << ": expected action";
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  NodePtr<INode> action_str_node = get_first_token();

  if (tokens_.empty()) {
    LOGE << "invalid alter table query: "
               "expected action's body in line " << line;
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  std::string action_str = ASTUtils::CastToNodeType<StringNode>(action_str_node)->data;

  NodePtr<INode> argument;
  StmtType actionStmtType;
  if (action_str == "ADD") {
    actionStmtType = StmtType::kAlterActionAdd;
    argument = GetTableDefinition();
  } else if (action_str == "DROP") {
    actionStmtType = StmtType::kAlterActionDrop;
    argument = GetDropListDefinition();
  } else {
    LOGE << "invalid alter table query: incorrect action \'"
        << action_str << "\' in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> action = ASTUtils::CreateServiceNode(actionStmtType);
  INode::Link(node, action);
  INode::Link(action, argument);

  return node;
}
NodePtr<INode> Parser::GetDropDatabaseSt() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kDropDatabaseStmt);

  // First database_name
  if (tokens_.empty()) {
    LOGE << "databaseName is missed";
  }
  ValidateIsWord(peek_first_token());
  NodePtr<INode> database_name = GetName();
  INode::Link(node, database_name);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(peek_first_token())) {
    NodePtr<INode> separator = GetListOf(StmtType::kName);
    INode::Link(node, separator);
  }

  return node;
}
NodePtr<INode> Parser::GetDropTableSt() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kDropTableStmt);

  // First tableName
  if (tokens_.empty()) {
    LOGE << "table name is missed";
  }
  ValidateIsWord(peek_first_token());
  NodePtr<INode> table_name = GetName();
  INode::Link(node, table_name);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(peek_first_token())) {
    NodePtr<INode> separator = GetListOf(StmtType::kName);
    INode::Link(node, separator);
  }

  return node;
}

// DDL Basic Statements

NodePtr<INode> Parser::GetTableDefinition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kTableDef);

  NodePtr<INode> argument = GetTableDefinitionObject();
  INode::Link(node, argument);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(peek_first_token())) {
    NodePtr<INode> separator = GetListOf(StmtType::kTableDef);
    INode::Link(node, separator);
  }

  return node;
}
NodePtr<INode> Parser::GetTableDefinitionObject() {
  NodePtr<INode> argument;

  std::string key_word = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;
  bool is_tableConstraint =
      key_word == "CONSTRAINT"
          || key_word == "PRIMARY"
          || key_word == "FOREIGN";

  if (is_tableConstraint) {
    argument = GetTableConstraint();
  } else {
    argument = GetColumnDefinition();
  }

  return argument;
}

NodePtr<INode> Parser::GetColumnDefinition() {
  NodePtr<INode> column_def = ASTUtils::CreateServiceNode(StmtType::kColumnDef);

  // Get columnName
  int line = peek_first_token()->line;
  NodePtr<INode> column_name = GetIdentifier();
  INode::Link(column_def, column_name);

  // Get datatype
  if (tokens_.empty()) {
    LOGE << "expected column datatype in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsWord(peek_first_token());
  NodePtr<INode> datatype = GetDataType();
  INode::Link(column_def, datatype);

  // Get other options
  if (!tokens_.empty()) {
    // TODO: get options such as IDENTITY or (NOT) NULL
  }

  return column_def;
}
NodePtr<INode> Parser::GetTableConstraint() {
  NodePtr<INode> table_constraint = ASTUtils::CreateServiceNode(StmtType::kTableConstraint);

  // Get full form if present
  int line = peek_first_token()->line;
  std::string key_word = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;
  if (key_word == "CONSTRAINT") {
    pop_first_token();
    NodePtr<INode> constraint_kw =
        ASTUtils::CastToNodeType<ServiceNode>(std::make_shared<ServiceNode>());
    constraint_kw->stmt_type = StmtType::kConstraintKW;
    INode::Link(table_constraint, constraint_kw);

    if (tokens_.empty()) {
      LOGE << "expected constraint name in line " << line;
      end(EXIT_FAILURE);
    }
    NodePtr<INode> constraint_name = GetIdentifier();
    INode::Link(constraint_kw, constraint_name);
  }

  // Get PRIMARY | FOREIGN KEY
  if (tokens_.empty()) {
    LOGE << "expected constraint in line " << line;
    end(EXIT_FAILURE);
  }

  // Get 'PRIMARY' | 'FOREIGN' key word
  line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  std::string kind_of_key = ASTUtils::CastToNodeType<StringNode>(get_first_token())->data;

  // Get 'KEY' key word
  if (tokens_.empty()) {
    LOGE << "invalid constraint definition: "
               "expected \'KEY\' in line " << line;
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  ValidateIsWord(peek_first_token());
  std::string KEY_kw = ASTUtils::CastToNodeType<StringNode>(get_first_token())->data;
  if (KEY_kw != "KEY") {
    LOGE << "expected \'KEY\', got \'"
        << KEY_kw << "\' in line " << line;
    end(EXIT_FAILURE);
  }

  // Get key definition
  if (tokens_.empty()) {
    LOGE << "expected constraint definition in line " << line;
    end(EXIT_FAILURE);
  }
  line = peek_first_token()->line;
  NodePtr<INode> key;
  if (kind_of_key == "PRIMARY") {
    key = GetPrimaryKey();
  } else if (kind_of_key == "FOREIGN") {
    key = GetForeignKey();
  } else {
    LOGE << "unknown kind of constraint in line "
        << line << ": " << kind_of_key;
    end(EXIT_FAILURE);
  }
  INode::Link(table_constraint, key);

  return table_constraint;
}

NodePtr<INode> Parser::GetDropListDefinition() {
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kDropList);

  NodePtr<INode> objects = GetDropObject();
  INode::Link(node, objects);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(peek_first_token())) {
    NodePtr<INode> separator = GetDropList();
    INode::Link(node, separator);
  }

  return node;
}
NodePtr<INode> Parser::GetDropList() {
  pop_first_token();
  NodePtr<INode> separator = ASTUtils::CreateServiceNode(StmtType::kCommaDelimiter);

  ValidateIsWord(peek_first_token());
  NodePtr<INode> objects = GetDropObject();
  INode::Link(separator, objects);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(peek_first_token())) {
      NodePtr<INode> next_objects = GetDropList();
      INode::Link(separator, next_objects);
  }

  return separator;
}
NodePtr<INode> Parser::GetDropObject() {
  int line = peek_first_token()->line;
  std::string key_word = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;
  bool is_tableConstraint = key_word == "CONSTRAINT";
  bool is_column = key_word == "COLUMN";

  if (!(is_tableConstraint || is_column)) {
    LOGE << "invalid drop list: incorrect key word \'"
        << key_word << "\' in line " << line;
    end(EXIT_FAILURE);
  }

  line = peek_first_token()->line;
  StmtType objectStmtType;
  if (is_tableConstraint) {
    objectStmtType = StmtType::kDropConstraint;
  } else {
    objectStmtType = StmtType::kDropColumn;
  }
  NodePtr<INode> object = ASTUtils::CreateServiceNode(objectStmtType);
  pop_first_token();
  if (tokens_.empty()) {
    LOGE << "invalid drop list: expected "
        << (is_column ? "constraint" : "column_name") << " in line " << line;
    end(EXIT_FAILURE);
  }

  if (tokens_.empty()) {
    LOGE << "invalid drop list: expected identifier in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> argument = GetIdentifier();
  INode::Link(object, argument);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(peek_first_token())) {
    // Check key word after the comma
    bool is_list = true;
    if (NodeDataTypeClassifier::IsWord(tokens_[1])) {
      std::string checking_word = ASTUtils::CastToNodeType<StringNode>(tokens_[1])->data;
      if (checking_word == "CONSTRAINT" || checking_word == "COLUMN") {
        is_list = false;
      }
    }

    // Get listOf identifiers if present
    if (is_list) {
      NodePtr<INode> next_arguments = GetListOf(StmtType::kIdentifier);
      INode::Link(object, next_arguments);
    }
  }

  return object;
}

} // scc::parser
