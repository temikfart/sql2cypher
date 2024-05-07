#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType Parser::ParseDDLStatementType() {
  auto next_token = NextToken();
  ValidateIsWord(next_token);
  auto first_keyword_token = ASTUtils::CastToNodeType<StringNode>(next_token);
  std::string first_keyword = first_keyword_token->data;

  ValidateHasTokens("Missed keyword for \'" + first_keyword + "\' DDL statement at line "
                        + std::to_string(first_keyword_token->line));
  next_token = NextToken();
  ValidateIsWord(next_token);
  auto second_keyword_token = ASTUtils::CastToNodeType<StringNode>(next_token);
  std::string second_keyword = second_keyword_token->data;

  std::string ddl_stmt = first_keyword + " " + second_keyword;
  try {
    StmtType ddl_stmt_type(ddl_stmt);
    return ddl_stmt_type;
  } catch (const std::invalid_argument& ia) {
    throw parsing_error("Unsupported DDL Statement \'" + ddl_stmt + "\' at line "
                            + std::to_string(first_keyword_token->line));
  }
}
NodePtr<INode> Parser::ParseDDLStatement() {
  auto peeked_token = PeekToken();
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kDdlStmt, peeked_token);

  StmtType ddl_stmt_type = ParseDDLStatementType();
  ValidateHasTokens("Missed body for \'" + scc::common::UpperCase(ddl_stmt_type.ToString())
                        + "\' statement at line" + std::to_string(peeked_token->line));
  NodePtr<INode> statement;
  switch (ddl_stmt_type) {
    case StmtType::kCreateDatabaseStmt:
      statement = ParseCreateDatabaseStatement();
      break;
    case StmtType::kCreateTableStmt:
      statement = ParseCreateTableStatement();
      break;
    case StmtType::kAlterTableStmt:
      statement = ParseAlterTableStatement();
      break;
    case StmtType::kDropDatabaseStmt:
      statement = ParseDropDatabaseStatement();
      break;
    case StmtType::kDropTableStmt:
      statement = ParseDropTableStatement();
      break;
    default:
      throw parsing_error("Unknown DDL Statement at line " + std::to_string(peeked_token->line));
  }
  ASTUtils::Link(service_node, statement);

  return service_node;
}

NodePtr<INode> Parser::ParseCreateDatabaseStatement() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kCreateDatabaseStmt);
  NodePtr<INode> database_name = ParseDatabaseName();
  ASTUtils::Link(service_node, database_name);
  return service_node;
}
NodePtr<INode> Parser::ParseCreateTableStatement() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kCreateTableStmt);

  auto peeked_token = PeekToken();
  NodePtr<INode> table_name = ParseTableName();
  ASTUtils::Link(service_node, table_name);

  ValidateHasTokens("Missed table definition at line " + std::to_string(peeked_token->line));
  NodePtr<INode> table_definition = ParseTableDefinition();
  ASTUtils::Link(service_node, table_definition);

  return service_node;
}
NodePtr<INode> Parser::ParseAlterTableStatement() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kAlterTableStmt);

  int line = PeekToken()->line;
  NodePtr<INode> table_name = ParseName();
  ASTUtils::Link(service_node, table_name);

  std::string incorrect_msg_prefix = "Incorrect \'ALTER TABLE\' definition at line ";

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(line)
                        + ": expected \'ADD\' or \'DROP\' keyword");
  auto next_token = NextToken();
  ValidateIsWord(next_token);
  std::string action_keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line) + ": expected \'"
                        + action_keyword + "\' definition");
  NodePtr<INode> argument;
  StmtType action_type = DetermineAlterTableActionType(action_keyword);
  switch (action_type) {
    case StmtType::kAddKW:
      argument = ParseAlterAddListDefinition();
      break;
    case StmtType::kDropKW:
      argument = ParseAlterDropListDefinition();
      break;
    default:
      line = PeekToken()->line;
      throw parsing_error(incorrect_msg_prefix + std::to_string(line) + ": unknown action \'"
                              + action_keyword + "\'");
  }
  NodePtr<INode> action = ASTUtils::CreateServiceNode(action_type);
  ASTUtils::Link(action, argument);
  ASTUtils::Link(service_node, action);

  return service_node;
}
NodePtr<INode> Parser::ParseDropDatabaseStatement() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kDropDatabaseStmt);

  NodePtr<INode> database_name = ParseName();
  ASTUtils::Link(service_node, database_name);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> names_list = ParseListOf(StmtType::kName);
    ASTUtils::Link(service_node, names_list);
  }

  return service_node;
}
NodePtr<INode> Parser::ParseDropTableStatement() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kDropTableStmt);

  NodePtr<INode> table_name = ParseName();
  ASTUtils::Link(service_node, table_name);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> names_list = ParseListOf(StmtType::kName);
    ASTUtils::Link(service_node, names_list);
  }

  return service_node;
}

NodePtr<INode> Parser::ParseTableDefinition() {
  ValidateIsOpeningRoundBracket(NextToken());

  NodePtr<INode> table_definition = ASTUtils::CreateServiceNode(StmtType::kTableDef);

  while (!tokens_.empty()) {
    NodePtr<INode> table_definition_element = ParseTableDefinitionElement();
    ASTUtils::Link(table_definition, table_definition_element);

    if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
      auto next_token = NextToken();
      ValidateHasTokens("Invalid table definition at line " + std::to_string(next_token->line)
                            + ": expected column or constraint definition");
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  ValidateHasTokens("Missing closing round bracket at line "); // TODO: add line number
  ValidateIsClosingRoundBracket(NextToken());

  return table_definition;
}
NodePtr<INode> Parser::ParseTableDefinitionElement() {
  auto peeked_token = PeekToken();
  StmtType stmt_type = ParseTableDefinitionElementType();
  switch (stmt_type) {
    case StmtType::kColumnDef:
      return ParseColumnDefinition();
    case StmtType::kConstraintKW:
    case StmtType::kPrimaryKey:
    case StmtType::kForeignKey:
      return ParseTableConstraint(stmt_type);
    default:
      throw parsing_error("Unknown element in table definition at line "
                              + std::to_string(peeked_token->line));
  }
}
StmtType Parser::ParseTableDefinitionElementType() {
  ValidateIsWord(PeekToken());
  std::string next_word = ASTUtils::CastToNodeType<StringNode>(PeekToken())->data;
  if (DetermineIsFullConstraintDefinition(next_word)) {
    return StmtType::kConstraintKW;
  }
  StmtType constraint_type = DetermineConstraintType(next_word);
  if (constraint_type != StmtType::kNone) {
    return constraint_type;
  }
  return StmtType::kColumnDef;
}

NodePtr<INode> Parser::ParseColumnDefinition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kColumnDef);

  int line = PeekToken()->line;
  NodePtr<INode> column_name = ParseIdentifier();
  ASTUtils::Link(service_node, column_name);

  ValidateHasTokens("Missing column datatype at line " + std::to_string(line));
  NodePtr<INode> datatype = ParseDataType();
  ASTUtils::Link(service_node, datatype);

  if (!tokens_.empty()) {
    // TODO: parse options such as IDENTITY or (NOT) NULL.
  }

  return service_node;
}
NodePtr<INode> Parser::ParseTableConstraint(StmtType stmt_type) {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kTableConstraint);

  int line = PeekToken()->line;
  if (stmt_type == StmtType::kConstraintKW) {
    NodePtr<INode> constraint_keyword =
        ASTUtils::CreateServiceNode(StmtType::kConstraintKW, NextToken());
    ASTUtils::Link(service_node, constraint_keyword);

    ValidateHasTokens("Missing constraint name at line "
                          + std::to_string(constraint_keyword->line));
    NodePtr<INode> constraint_name = ParseIdentifier();
    line = constraint_name->line;
    ASTUtils::Link(constraint_keyword, constraint_name);
  }

  ValidateHasTokens("Missing constraint definition at line " + std::to_string(line));
  auto peeked_token = PeekToken();
  StmtType constraint_type;
  if (stmt_type != StmtType::kNone) {
    constraint_type = stmt_type;
  } else {
    ValidateIsWord(peeked_token);
    std::string constraint_type_keyword = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
    constraint_type = DetermineConstraintType(constraint_type_keyword);
  }

  NodePtr<INode> constraint_definition;
  switch (constraint_type) {
    case StmtType::kPrimaryKey:
      constraint_definition = ParsePrimaryKey();
      break;
    case StmtType::kForeignKey:
      constraint_definition = ParseForeignKey();
      break;
    default:
      throw parsing_error("Unknown constraint type at line " + std::to_string(peeked_token->line));
  }
  ASTUtils::Link(service_node, constraint_definition);

  return service_node;
}

NodePtr<INode> Parser::ParseAlterAddListDefinition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kAlterAddList);

  while (!tokens_.empty()) {
    NodePtr<INode> add_element = ParseAlterAddElement();
    ASTUtils::Link(service_node, add_element);

    if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
      auto next_token = NextToken();
      ValidateHasTokens("Missing \'ALTER TABLE ... ADD\' element in list at line "
                            + std::to_string(next_token->line));
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  return service_node;
}
NodePtr<INode> Parser::ParseAlterAddElement() {
  return ParseTableDefinitionElement();
}
NodePtr<INode> Parser::ParseAlterDropListDefinition() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kAlterDropList);

  while (!tokens_.empty()) {
    NodePtr<INode> drop_element = ParseAlterDropElement();
    ASTUtils::Link(service_node, drop_element);

    if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
      auto next_token = NextToken();
      ValidateHasTokens("Missing \'ALTER TABLE ... DROP\' element in list at line "
                            + std::to_string(next_token->line));
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  return service_node;
}
NodePtr<INode> Parser::ParseAlterDropElement() {
  auto next_token = NextToken();
  std::string keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  StmtType drop_element_type = DetermineDropElementType(keyword);
  NodePtr<INode> drop_element = ASTUtils::CreateServiceNode(drop_element_type);

  ValidateHasTokens("Missing \'ALTER TABLE ... DROP\' element definition at line "
                        + std::to_string(next_token->line));
  auto peeked_token = PeekToken();
  NodePtr<INode> argument;
  switch (drop_element_type) {
    case StmtType::kDropColumn:
      ValidateIsWord(peeked_token);
      argument = ParseName();
      break;
    case StmtType::kDropConstraint:
      argument = ParseTableConstraint();
      break;
    default:
      throw parsing_error("Unknown \'ALTER TABLE ... DROP\' element type at line "
                              + std::to_string(peeked_token->line));
  }
  ASTUtils::Link(drop_element, argument);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    // TODO: check that we have at least 2 tokens.
    if (NodeDataTypeClassifier::IsWord(tokens_[1])) {
      std::string checking_word = ASTUtils::CastToNodeType<StringNode>(tokens_[1])->data;
      if (DetermineDropElementType(checking_word) == StmtType::kNone) {
        // TODO: COLUMN keyword is recognized as an identifier. Ignore keywords in ParseListOf.
        NodePtr<INode> next_arguments = ParseListOf(StmtType::kName);
        ASTUtils::Link(drop_element, next_arguments);
      }
    }
  }

  return drop_element;
}

} // scc::parser
