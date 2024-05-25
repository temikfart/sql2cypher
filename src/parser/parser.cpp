#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

using std::format;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

Parser::Parser(std::deque<NodePtr<INode>>&& tokens) : tokens_(std::move(tokens)) {}

NodePtr<INode> Parser::Parse() {
  LOGI << "Parsing is started";
  if (!HasTokens()) {
    LOGI << "Parsing is finished. Nothing to parse";
    return {};
  }

  NodePtr<INode> root = ASTUtils::CreateRootNode(StmtType::kProgram);
  while (HasTokens()) {
    if (NodeDataClassifier::IsSemicolon(PeekToken())) {
      NextToken();
      continue;
    }

    NodePtr<INode> query = ParseQuery();
    ASTUtils::Link(root, query);
  }

  ValidateHasNotTokens();
  LOGI << "Parsing is finished";
  return root;
}

const NodePtr<INode>& Parser::PeekToken(unsigned idx) const {
  ValidateHasTokens(idx + 1);
  return tokens_[idx];
}
NodePtr<INode> Parser::NextToken() {
  ValidateHasTokens(1);
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
    throw parsing_error(format("Unknown Base Statement at line {}", peeked_token->line));
  }
}

} // scc::parser
