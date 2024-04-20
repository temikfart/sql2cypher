#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

Parser::Parser(std::deque<NodePtr<INode>>&& tokens) : tokens_(std::move(tokens)) {}

NodePtr<INode> Parser::Parse() {
  LOGI << "starting syntax analysis...";
  ValidateHasTokens();

  NodePtr<INode> root = ASTUtils::CreateRootNode(StmtType::kProgram);
  NodePtr<INode> query = GetDL();
  ASTUtils::Link(root, query);

  if (!tokens_.empty() && NodeDataClassifier::IsSemicolon(PeekToken())) {
    NodePtr<INode> separator = General();
    ASTUtils::Link(root, separator);
  }

  ValidateHasNotTokens();
  return root;
}

const NodePtr<INode>& Parser::PeekToken() const {
  ValidateHasTokens();
  return tokens_.front();
}
NodePtr<INode> Parser::NextToken() {
  ValidateHasTokens();
  NodePtr<INode> node = tokens_.front();
  tokens_.pop_front();
  return node;
}

// Start

NodePtr<INode> Parser::General() {
  NextToken();
  NodePtr<INode> separator = ASTUtils::CreateServiceNode(StmtType::kSemicolonDelimiter);

  if (!tokens_.empty()) {
    NodePtr<INode> query = ParseQuery();
    ASTUtils::Link(separator, query);
  }

  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsSemicolon(PeekToken())) {
      NodePtr<INode> next_queries = General();
      ASTUtils::Link(separator, next_queries);
    }
  }

  return separator;
}
StmtType Parser::GetDLStType() {
  StmtType DLStType = StmtType::kNone;   // invalid value

  std::string key_word = ASTUtils::CastToNodeType<StringNode>(PeekToken())->data;

  std::vector<std::string> ddlSt_kws = {
      "CREATE", "ALTER", "DROP"
  };
  std::vector<std::string> dmlSt_kws = {
      "UPDATE", "DELETE", "INSERT"
  };

  bool is_ddlSt = std::any_of(ddlSt_kws.begin(),
                              ddlSt_kws.end(),
                              [key_word](std::string& st) {
                                return (key_word == st);
                              });
  bool is_dmlSt = std::any_of(dmlSt_kws.begin(),
                              dmlSt_kws.end(),
                              [key_word](std::string& st) {
                                return (key_word == st);
                              });

  if (is_ddlSt) {
    DLStType = StmtType::kDdlStmt;
  } else if (is_dmlSt) {
    DLStType = StmtType::kDmlStmt;
  }

  return DLStType;
}
NodePtr<INode> Parser::ParseQuery() {
  NodePtr<INode> query = ASTUtils::CreateServiceNode(StmtType::kQuery);

  ValidateIsWord(PeekToken());

  NodePtr<INode> statement;
  switch (GetDLStType()) {
    case StmtType::kDdlStmt:
      statement = GetDDLSt();
      break;
    case StmtType::kDmlStmt:
      statement = GetDMLSt();
      break;
    default:
      LOGE << "unknown DL on line "
          << PeekToken()->line;
      end(EXIT_FAILURE);
  }
  ASTUtils::Link(query, statement);

  return query;
}

} // scc::parser
