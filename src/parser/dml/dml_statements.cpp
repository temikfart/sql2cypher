#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

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
    throw parsing_error("Unsupported DML Statement \'" + keyword + "\' at line "
                            + std::to_string(keyword_token->line));
  }
}
NodePtr<INode> Parser::ParseDMLStatement() {
  auto peeked_token = PeekToken();
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kDmlStmt, peeked_token);

  StmtType dml_stmt_type = ParseDMLStatementType();
  ValidateHasTokens("Missed body for \'" + scc::common::UpperCase(dml_stmt_type.ToString())
                        + "\' statement at line" + std::to_string(peeked_token->line));
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
      throw parsing_error("Unknown DML Statement at line " + std::to_string(peeked_token->line));
  }
  ASTUtils::Link(service_node, statement);

  return service_node;
}

NodePtr<INode> Parser::ParseInsertStatement() {
  return ASTUtils::CreateServiceNode(StmtType::kInsertStmt);
}
NodePtr<INode> Parser::ParseDeleteStatement() {
  return ASTUtils::CreateServiceNode(StmtType::kDeleteStmt);
}
NodePtr<INode> Parser::ParseUpdateStatement() {
  return ASTUtils::CreateServiceNode(StmtType::kUpdateStmt);
}

} // scc::parser
