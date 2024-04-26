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
  LOGI << "Parsing is started";
  if (tokens_.empty()) {
    LOGI << "Parsing is finished. Nothing to parse";
    return {};
  }

  NodePtr<INode> root = ASTUtils::CreateRootNode(StmtType::kProgram);
  NodePtr<INode> query = ParseQuery();
  ASTUtils::Link(root, query);

  if (!tokens_.empty() && NodeDataClassifier::IsSemicolon(PeekToken())) {
    NodePtr<INode> next_queries = ParseNextQueries();
    ASTUtils::Link(root, next_queries);
  }

  ValidateHasNotTokens();
  LOGI << "Parsing is finished";
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

NodePtr<INode> Parser::ParseQuery() {
  const auto& peeked_token = PeekToken();
  ValidateIsWord(peeked_token);
  NodePtr<INode> query = ASTUtils::CreateServiceNode(StmtType::kQuery, peeked_token);

  NodePtr<INode> statement = ParseBaseStatement();
  ASTUtils::Link(query, statement);

  return query;
}
NodePtr<INode> Parser::ParseBaseStatement() {
  const auto& peeked_token = PeekToken();
  ValidateIsWord(peeked_token);
  std::string keyword = ASTUtils::CastToNodeType<StringNode>(peeked_token)->data;
  if (BaseStmtTypeClassifier::IsDDLKeyword(keyword)) {
    return ParseDDLStatement();
  } else if (BaseStmtTypeClassifier::IsDMLKeyword(keyword)) {
    return ParseDMLStatement();
  } else {
    throw parsing_error("Unknown Base Statement at line " + std::to_string(peeked_token->line));
  }
}
NodePtr<INode> Parser::ParseNextQueries() {
  NodePtr<INode> separator = ASTUtils::CreateServiceNode(StmtType::kSemicolonDelimiter,
                                                         NextToken());

  if (!tokens_.empty()) {
    NodePtr<INode> query = ParseQuery();
    ASTUtils::Link(separator, query);
  }

  if (!tokens_.empty() && NodeDataClassifier::IsSemicolon(PeekToken())) {
    NodePtr<INode> next_queries = ParseNextQueries();
    ASTUtils::Link(separator, next_queries);
  }

  return separator;
}

} // scc::parser
