#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

Parser::Parser(std::deque<NodePtr<INode>>&& tokens)
    : tokens_(std::move(tokens)) {}

NodePtr<INode> Parser::Parse() {
  LOGI << "starting syntax analysis...";

  NodePtr<INode> root = ASTUtils::CreateRootNode(StmtType::kProgram);

  if (tokens_.empty()) {
    LOGI << "syntax analysis is ended: empty tokens' array";
    return nullptr;
  }

  NodePtr<INode> query = GetDL();
  ASTUtils::Link(root, query);

  if (!tokens_.empty() && NodeDataClassifier::IsSemicolon(peek_first_token())) {
    NodePtr<INode> separator = General();
    ASTUtils::Link(root, separator);
  }

  if (!tokens_.empty()) {
    LOGE << "syntax analysis is ended with not empty tokens' array";
    end(EXIT_FAILURE);
  }

  LOGI << "syntax analysis is ended";
  return root;
}

NodePtr<INode>& Parser::peek_first_token() const {
  ValidateNotEmpty();
  return const_cast<NodePtr<INode>&>(tokens_.front());
}
NodePtr<INode> Parser::get_first_token() {
  ValidateNotEmpty();
  NodePtr<INode> node = tokens_.front();
  tokens_.pop_front();
  return node;
}
void Parser::pop_first_token() {
  ValidateNotEmpty();
  tokens_.pop_front();
}

// Start

NodePtr<INode> Parser::General() {
  pop_first_token();
  NodePtr<INode> separator = ASTUtils::CreateServiceNode(StmtType::kSemicolonDelimiter);

  if (!tokens_.empty()) {
    NodePtr<INode> query = GetDL();
    ASTUtils::Link(separator, query);
  }

  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsSemicolon(peek_first_token())) {
      NodePtr<INode> next_queries = General();
      ASTUtils::Link(separator, next_queries);
    }
  }

  return separator;
}
StmtType Parser::GetDLStType() {
  StmtType DLStType = StmtType::kNone;   // invalid value

  std::string key_word = ASTUtils::CastToNodeType<StringNode>(peek_first_token())->data;

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
NodePtr<INode> Parser::GetDL() {
  NodePtr<INode> query = ASTUtils::CreateServiceNode(StmtType::kQuery);

  ValidateIsWord(peek_first_token());

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
          << peek_first_token()->line;
      end(EXIT_FAILURE);
  }
  ASTUtils::Link(query, statement);

  return query;
}

} // scc::parser
