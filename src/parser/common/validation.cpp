#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace common;

using std::format;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

parsing_error::parsing_error(const std::string& message) : std::logic_error(message) {}

bool Parser::HasTokens(unsigned min_count) const {
  return tokens_.size() >= min_count;
}

void Parser::ValidateHasTokens(const std::string& details) const {
  if (!HasTokens()) {
    std::string msg = "Unexpected empty tokens array";
    std::string cause = (details.empty() ? "" : (": " + details));
    throw parsing_error(msg + cause);
  }
}
void Parser::ValidateHasTokens(unsigned min_count) const {
  if (!HasTokens(min_count)) {
    std::string msg = format("Unexpected empty tokens array: expected at least {} tokens",
                             min_count);
    throw parsing_error(msg);
  }
}
void Parser::ValidateHasNotTokens() const {
  if (HasTokens()) {
    throw parsing_error("Unexpected non empty tokens array");
  }
}
void Parser::ValidateIsWord(const NodePtr<INode>& node) const {
  if (!NodeDataTypeClassifier::IsWord(node)) {
    std::string msg = format("Expected Word at line {}", node->line);
    throw parsing_error(msg);
  }
}
void Parser::ValidateIsOpeningRoundBracket(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsOpeningRoundBracket(node)) {
    std::string msg = format("Expected Opening Round Bracket at line {}", node->line);
    throw parsing_error(msg);
  }
}
void Parser::ValidateIsClosingRoundBracket(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsClosingRoundBracket(node)) {
    std::string msg = format("Expected Closing Round Bracket at line {}", node->line);
    throw parsing_error(msg);
  }
}
void Parser::ValidateIsSingleQuote(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsSingleQuote(node)) {
    std::string msg = format("Expected Single Quote at line {}", node->line);
    throw parsing_error(msg);
  }
}
void Parser::ValidateIsDoubleQuote(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsSingleQuote(node)) {
    std::string msg = format("Expected Double Quote at line {}", node->line);
    throw parsing_error(msg);
  }
}
void Parser::ValidateIsBinaryOperator(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsBinaryOperator(node)) {
    std::string msg = format("Expected Binary Operator at line {}", node->line);
    throw parsing_error(msg);
  }
}

void Parser::ValidateIsAssignmentOperator(const NodePtr<INode>& node) const {
  if (!NodeDataClassifier::IsAssignmentOperator(node)) {
    std::string msg = format("Expected Assignment Operator at line {}", node->line);
    throw parsing_error(msg);
  }
}

} // scc::parser
