#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

using std::format;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType Parser::ParseDMLStatementType() {
  auto next_token = NextToken();
  ValidateIsWord(next_token);
  auto keyword_token = ASTUtils::CastToNodeType<StringNode>(next_token);
  std::string keyword = keyword_token->data;

  try {
    StmtType ddl_stmt_type(keyword);
    return ddl_stmt_type;
  } catch (const std::invalid_argument& ia) {
    throw parsing_error(format("Unsupported DML Statement \'{}\' at line {}",
                               keyword, keyword_token->line));
  }
}
NodePtr<INode> Parser::ParseDMLStatement() {
  auto peeked_token = PeekToken();
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kDmlStmt, peeked_token);

  StmtType dml_stmt_type = ParseDMLStatementType();
  ValidateHasTokens(format("Missed body for \'{}\' statement at line {}",
                           scc::common::UpperCase(dml_stmt_type.ToString()), peeked_token->line));
  NodePtr<INode> statement;
  switch (dml_stmt_type) {
    case StmtType::kUpdateStmt:
      statement = ParseUpdateStatement();
      break;
    case StmtType::kDeleteStmt:
      statement = ParseDeleteStatement();
      break;
    case StmtType::kInsertStmt:
      statement = ParseInsertStatement();
      break;
    default:
      throw parsing_error(format("Unknown DML Statement at line {}", peeked_token->line));
  }
  ASTUtils::Link(service_node, statement);

  return service_node;
}

NodePtr<INode> Parser::ParseInsertStatement() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kInsertStmt);

  auto peeked_token = PeekToken();
  ValidateIsWord(peeked_token);
  std::string next_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
  try {
    StmtType into_kw(next_word);
    if (into_kw != StmtType::kIntoKW) {
      throw parsing_error(format("Expected \'INTO\' keyword at line {}", peeked_token->line));
    }
    NextToken();
  } catch (const std::invalid_argument& ignored) {}

  ValidateHasTokens(format("Missing table name at line {}", peeked_token->line));
  NodePtr<INode> table_name = ParseTableName();
  ASTUtils::Link(service_node, table_name);

  ValidateHasTokens(format("Incorrect \'INSERT\' statement: "
                           "expected column list or \'VALUES\' keyword at line {}",
                           peeked_token->line));
  peeked_token = PeekToken();
  if (NodeDataClassifier::IsOpeningRoundBracket(peeked_token)) {
    auto next_token = NextToken();
    ValidateHasTokens(format("Missing column list where to insert at line {}", next_token->line));

    while (HasTokens()) {
      auto column_name = ParseColumnName();
      ASTUtils::Link(service_node, column_name);

      if (HasTokens() && NodeDataClassifier::IsComma(PeekToken())) {
        next_token = NextToken();
        ValidateHasTokens(format("Missing column name after comma at line {}", next_token->line));
        ValidateIsWord(PeekToken());
      } else {
        break;
      }
    }

    ValidateHasTokens(format("Missing closing round bracket at line {}", next_token->line));
    peeked_token = PeekToken();
    ValidateIsClosingRoundBracket(NextToken());
  }

  ValidateHasTokens(format("Missing VALUES clause at line {}", peeked_token->line));
  peeked_token = PeekToken();
  ValidateIsWord(peeked_token);
  auto values_list = ParseInsertValuesList();
  ASTUtils::Link(service_node, values_list);

  return service_node;
}
NodePtr<INode> Parser::ParseUpdateStatement() {
  auto service_node = ASTUtils::CreateServiceNode(StmtType::kUpdateStmt);

  NodePtr<INode> table_name = ParseTableName();
  ASTUtils::Link(service_node, table_name);

  auto peeked_token = PeekToken();
  ValidateIsWord(peeked_token);
  std::string next_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
  try {
    StmtType set_kw(next_word);
    if (set_kw != StmtType::kSetKW) {
      throw parsing_error(format("Expected \'SET\' keyword at line {}", peeked_token->line));
    }
    NextToken();
  } catch (const std::invalid_argument& ignored) {}

  ValidateHasTokens(format("Missing update columns list at line {}", peeked_token->line));
  while (HasTokens()) {
    auto update_column_element = ParseUpdateColumnElement();
    ASTUtils::Link(service_node, update_column_element);

    if (HasTokens() && NodeDataClassifier::IsComma(PeekToken())) {
      auto next_token = NextToken();
      ValidateHasTokens(format("Missing update column statement after comma at line {}",
                               next_token->line));
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  if (HasTokens() && NodeDataTypeClassifier::IsWord(PeekToken())) {
    auto where_condition = ParseWhereStatement();
    ASTUtils::Link(service_node, where_condition);
  }

  return service_node;
}
NodePtr<INode> Parser::ParseDeleteStatement() {
  auto service_node = ASTUtils::CreateServiceNode(StmtType::kDeleteStmt);

  auto peeked_token = PeekToken();
  ValidateIsWord(peeked_token);
  std::string next_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
  try {
    StmtType from_kw(next_word);
    if (from_kw != StmtType::kFromKW) {
      throw parsing_error(format("Expected \'FROM\' keyword at line {}", peeked_token->line));
    }
    NextToken();
  } catch (const std::invalid_argument& ignored) {}

  ValidateHasTokens(format("Missing table name at line {}", peeked_token->line));
  auto table_name = ParseTableName();
  ASTUtils::Link(service_node, table_name);

  ValidateHasTokens(format("Incorrect \'DELETE\' statement: expected condition at line {}",
                           table_name->line));
  auto where_condition = ParseWhereStatement();
  ASTUtils::Link(service_node, where_condition);

  return service_node;
}

NodePtr<INode> Parser::ParseInsertValuesList() {
  auto peeked_token = PeekToken();
  std::string values_keyword = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
  std::string invalid_values_keyword_error = format("Expected \'VALUES\' keyword at line {}",
                                                    peeked_token->line);
  try {
    StmtType values_kw(values_keyword);
    if (values_kw != StmtType::kValuesKW) {
      throw parsing_error(invalid_values_keyword_error);
    }
    NextToken();
  } catch (const std::invalid_argument& ignored) {
    throw parsing_error(invalid_values_keyword_error);
  }
  auto values_list = ASTUtils::CreateServiceNode(StmtType::kValuesKW);

  ValidateHasTokens(format("Incorrect \'INSERT\' statement: expected expression list at line {}",
                           values_list->line));
  peeked_token = PeekToken();
  if (NodeDataClassifier::IsOpeningRoundBracket(peeked_token)) {
    auto next_token = NextToken();
    ValidateHasTokens(format("Missing expression list at line {}", next_token->line));

    while (HasTokens()) {
      auto expression_or_null = ParseExpressionOrNull();
      ASTUtils::Link(values_list, expression_or_null);

      if (HasTokens() && NodeDataClassifier::IsComma(PeekToken())) {
        next_token = NextToken();
        ValidateHasTokens(format("Missing expression after comma at line {}", next_token->line));
      } else {
        break;
      }
    }

    ValidateHasTokens(format("Missing closing round bracket at line {}", next_token->line));
    ValidateIsClosingRoundBracket(NextToken());
  }

  return values_list;
}
NodePtr<INode> Parser::ParseUpdateColumnElement() {
  auto service_node = ASTUtils::CreateServiceNode(StmtType::kUpdateColumn);

  auto column_name = ParseColumnName();

  ValidateHasTokens(format("Missing assignment operator at line {}", column_name->line));
  auto assignment_operator = NextToken();
  ValidateIsAssignmentOperator(assignment_operator);
  ASTUtils::Link(service_node, column_name);

  ValidateHasTokens(format("Missing value for column at line {}", assignment_operator->line));
  auto expression_or_null = ParseExpressionOrNull();
  ASTUtils::Link(service_node, expression_or_null);

  return service_node;
}

NodePtr<INode> Parser::ParseExpressionOrNull() {
  try {
    auto peeked_token = PeekToken();
    if (NodeDataTypeClassifier::IsWord(peeked_token)) {
      std::string null_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
      StmtType null_value(null_word);
      if (null_value == StmtType::kNullValue) {
        return ASTUtils::CreateServiceNode(StmtType::kNullValue, NextToken());
      }
    }
  } catch (const std::invalid_argument& ignored) {}
  return ParseExpression();
}
NodePtr<INode> Parser::ParseWhereStatement() {
  auto peeked_token = PeekToken();
  std::string next_word = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
  std::string invalid_where_keyword_error = format("Expected \'WHERE\' keyword at line {}",
                                                   peeked_token->line);
  try {
    StmtType where_kw(next_word);
    if (where_kw != StmtType::kWhereKW) {
      throw parsing_error(invalid_where_keyword_error);
    }
    NextToken();
  } catch (const std::invalid_argument& ignored) {
    throw parsing_error(invalid_where_keyword_error);
  }

  return ParseCondition();
}

} // scc::parser
