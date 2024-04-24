#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

parsing_error::parsing_error(const std::string& message) : std::logic_error(message) {}

void Parser::ValidateHasTokens(const std::string& details) const {
  if (tokens_.empty()) {
    std::string msg = "Unexpected empty tokens array";
    std::string cause = (details.empty() ? "" : (": " + details));
    throw parsing_error(msg + cause);
  }
}
void Parser::ValidateHasNotTokens() const {
  if (!tokens_.empty()) {
    throw parsing_error("Unexpected non empty tokens array");
  }
}
void Parser::ValidateIsWord(const NodePtr<INode>& node) const {
  if (!NodeDataTypeClassifier::IsWord(node)) {
    throw parsing_error("Expected Word at line " + std::to_string(node->line));
  }
}
void Parser::ValidateIsOpeningRoundBracket(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsOpeningRoundBracket(node)) {
    throw parsing_error("Expected Opening Round Bracket at line " + std::to_string(node->line));
  }
}
void Parser::ValidateIsClosingRoundBracket(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsClosingRoundBracket(node)) {
    throw parsing_error("Expected Closing Round Bracket at line " + std::to_string(node->line));
  }
}
void Parser::ValidateIsSingleQuote(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsSingleQuote(node)) {
    throw parsing_error("Expected Single Quote at line " + std::to_string(node->line));
  }
}
void Parser::ValidateIsDoubleQuote(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsSingleQuote(node)) {
    throw parsing_error("Expected Double Quote at line " + std::to_string(node->line));
  }
}
void Parser::ValidateIsBinaryOperator(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsBinaryOperator(node)) {
    throw parsing_error("Expected Binary Operator at line " + std::to_string(node->line));
  }
}

} // scc::parser
