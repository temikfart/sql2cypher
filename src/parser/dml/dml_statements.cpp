#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType Parser::GetDMLStType() {
  StmtType DMLStType = StmtType::kNone; // invalid value

  // Get first token
  int line = peek_first_token()->line;
  std::string fst_kw = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;
  pop_first_token();

  if (tokens_.empty()) {
    LOGE << "body of the DDL statement is missed in line " << line;
    end(EXIT_FAILURE);
  }

  // Define next rule
  bool is_UPDATE = fst_kw == "UPDATE";
  bool is_DELETE = fst_kw == "DELETE";
  bool is_INSERT = fst_kw == "INSERT";

  if (is_UPDATE) { DMLStType = StmtType::kUpdateStmt; }
  else if (is_DELETE) { DMLStType = StmtType::kDeleteStmt; }
  else if (is_INSERT) { DMLStType = StmtType::kInsertStmt; }
  else {
    LOGE << "unknown DML statement type in line " << line;
    end(EXIT_FAILURE);
  }

  return DMLStType;
}
NodePtr<INode> Parser::GetDMLSt() {
  LOGD << "getting DML statement...";
  NodePtr<INode> node = ASTUtils::CreateServiceNode(StmtType::kDmlStmt);

  int line = peek_first_token()->line;
  NodePtr<INode> statement;
  switch (GetDMLStType()) {
    case StmtType::kUpdateStmt:
      statement = GetUpdateSt();
      LOGD << "got UPDATE statement";
      break;
    case StmtType::kDeleteStmt:
      statement = GetDeleteSt();
      LOGD << "got DELETE statement";
      break;
    case StmtType::kInsertStmt:
      statement = GetInsertSt();
      LOGD << "got INSERT statement";
      break;
    default:
      LOGE << "unknown DML statement near line " << line;
      end(EXIT_FAILURE);
  }
  ASTUtils::Link(node, statement);

  return node;
}

// DML Statements

NodePtr<INode> Parser::GetInsertSt() {
  return ASTUtils::CreateServiceNode(StmtType::kInsertStmt);
}
NodePtr<INode> Parser::GetDeleteSt() {
  return ASTUtils::CreateServiceNode(StmtType::kDeleteStmt);
}
NodePtr<INode> Parser::GetUpdateSt() {
  return ASTUtils::CreateServiceNode(StmtType::kUpdateStmt);
}

} // scc::parser
